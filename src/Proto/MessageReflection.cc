#include "Proto/Common.h"
#include "Proto/RepeatedFields.h"
#include "Proto/MessageFactory.h"
#include "Proto/MessageReflection.h"
#include "Proto/Descriptors_internal.h"

namespace proto {

int indent_num = 0;
std::string indent(){
  std::string re = "";
  for (int i = 0; i < indent_num; i++) {
    re += "  ";
  }
  return re;
}

MessageReflection::MessageReflection(
    const MessageDescriptor* message_descirptor,
    const Message* defatult_instance,
    const int* fields_offset,
    int has_bits_offset) :
  message_descirptor_(message_descirptor),
  defatult_instance_(defatult_instance),
  has_bits_offset_(has_bits_offset) {
  int num_fields = message_descirptor->num_fields();
  fields_offset_ = new int[num_fields];
  for (int i = 0; i < num_fields; i++) {
    fields_offset_[i] = fields_offset[i];
  }
}

MessageReflection::~MessageReflection() {
  if (fields_offset_) {
    delete[] fields_offset_;
  }
}

const MessageDescriptor* MessageReflection::descriptor() const {
  return message_descirptor_.get();
}

const Message* MessageReflection::defatult_instance() const {
  return defatult_instance_;
}

int MessageReflection::FieldOffset(const FieldDescriptor* field) const {
  return fields_offset_[field->parse_index_];
}

// -------------------------------------------------------------------------- //
// ---------------------------- Serialize routines -------------------------- //
// -------------------------------------------------------------------------- //
bool MessageReflection::RepeatedFieldEmpty(
    const Message* message,
    const FieldDescriptor* field) const {
  const char* ptr =
      reinterpret_cast<const char*>(message) + FieldOffset(field);
  return (reinterpret_cast<const RepeatedFieldBase*>(ptr)) -> size() == 0;
}

SerializedMessage* MessageReflection::Serialize(const Message* message) const {
  SerializedMessage* sdmsg = new SerializedMessage();
  // Begin serializing a message
  for (const auto& field: message_descirptor_->impl_->fields_list_) {
    if ((field->IsSingularType() && !HasField(message, field->tag())) ||
        (field->IsRepeatedType() && RepeatedFieldEmpty(message, field.get()))) {
      continue;
    }
    // Message Type
    if (field->IsMessageType()) {
      if (!field->IsRepeatedType()) {
        sdmsg->AddField(CreateSerializedSingularMessage(message, field.get()));
      }
      else {
        sdmsg->AddField(CreateSerializedRepeatedMessage(message, field.get()));
      }
    }
    // Primitive Type and String type
    else {
      if (!field->IsRepeatedType()) {
        sdmsg->AddField(
            CreateSerializedSingularPrimitive(message, field.get()));
      }
      else {
        sdmsg->AddField(
            CreateSerializedRepeatedPrimitive(message, field.get()));
      }
    }
  }
  return sdmsg;
}

std::shared_ptr<SerializedObjectInterface>
MessageReflection::CreateSerializedSingularMessage(
    const Message* message,
    const FieldDescriptor* field)  const {
  // Serialize a singular nested message
  // std::cout << indent() << "serializing singular nested message: "
  //           << field->type_descriptor()->full_name()
  //           << std::endl;
  indent_num++;
  const MessageReflection* nested_msg_reflection =
      MessageFactory::GetMessageReflection(
          field->type_descriptor()->full_name());
  if (!nested_msg_reflection) {
    throw std::runtime_error(
        "No cpp generated class type " +
        field->type_descriptor()->full_name() +
        " exists.");
  }
  SerializedMessage* nested_sdmsg = nested_msg_reflection->Serialize(
    *reinterpret_cast<Message* const*>(
      (reinterpret_cast<const char*>(message) + FieldOffset(field))
    )
  );
  // Encdoe meta data:
  // | TagType | Payload Size | ... |
  WireFormat::EncodeTag(field->tag(), WireFormat::WIRETYPE_LENGTH_DELIMITED,
                        nested_sdmsg->meta_data());
  WireFormat::WriteUInt32(nested_sdmsg->size(), nested_sdmsg->meta_data());
  nested_sdmsg->ReCalculateSize();
  indent_num--;
  return std::shared_ptr<SerializedObjectInterface>(nested_sdmsg);
}

std::shared_ptr<SerializedObjectInterface>
MessageReflection::CreateSerializedRepeatedMessage(
    const Message* message,
    const FieldDescriptor* field)  const {
  // std::cout << indent() << "serializing repeated nested message: "
  //           << field->name() << std::endl;
  indent_num++;
  // Create a new SerializedMessage to store repeated messages.
  SerializedMessage* sdmsg = new SerializedMessage(true);
  // get reflection
  const MessageReflection* nested_msg_reflection =
      MessageFactory::GetMessageReflection(
          field->type_descriptor()->full_name());
  if (!nested_msg_reflection) {
    throw std::runtime_error(
        "No cpp generated class type " +
        field->type_descriptor()->full_name() +
        " exists.");
  }
  // Get RepeatedMessagePtr
  const char* field_addr =
      reinterpret_cast<const char*>(message) + FieldOffset(field);
  const auto repeated_field =
      reinterpret_cast<const RepeatedFieldBase*>(field_addr);
  // Serialize repeated messages one by one.
  for (uint32 i = 0; i < repeated_field->size(); i++) {
    //std::cout << indent() << "[... element " << i << " ...]" << std::endl;
    const char* element_ptr = repeated_field->GetElementPtr(i);
    SerializedMessage* nested_sdmsg = nested_msg_reflection->Serialize(
      reinterpret_cast<const Message*>(element_ptr)
    );
    // Encdoe meta data for element message:
    // | Paylod Size | ... Data ... |
    WireFormat::WriteUInt32(nested_sdmsg->size(), nested_sdmsg->meta_data());
    nested_sdmsg->ReCalculateSize();
    sdmsg->AddField(std::shared_ptr<SerializedObjectInterface>(nested_sdmsg));
  }
  // Encode meta data for RepeatedPtrField:
  // | TagType | List Size | Element1 |  Element2 | ... ... | Element N |
  //                           |
  //                           --> | Payload Size | ... Data ... |
  WireFormat::EncodeTag(field->tag(), WireFormat::WIRETYPE_LENGTH_DELIMITED,
                        sdmsg->meta_data());
  WireFormat::WriteUInt32(repeated_field->size(), sdmsg->meta_data());
  sdmsg->ReCalculateSize();
  indent_num--;
  return std::shared_ptr<SerializedObjectInterface>(sdmsg);
}


#define ENCODE_SINGULAR_PRITIMIVE(CPP_TYPE, WIRE_TYPENAME)                   \
  WireFormat::Encode##WIRE_TYPENAME(                                         \
    field->tag(),                                                            \
    *reinterpret_cast<const CPP_TYPE*>(msg_addr + FieldOffset(field)),    \
    sdprim->mutable_ostream());


std::shared_ptr<SerializedObjectInterface>
MessageReflection::CreateSerializedSingularPrimitive(
    const Message* message,
    const FieldDescriptor* field)  const {
  // std::cout << indent() << "serializing singular primitive: "
  //           << field->name() << std::endl;
  indent_num++;
  SerializedPrimitive* sdprim = new SerializedPrimitive(field->type());
  const char* msg_addr = reinterpret_cast<const char*>(message);
  switch (field->type()) {
    case UINT32:
      ENCODE_SINGULAR_PRITIMIVE(uint32, UInt32)
      break;
    case UINT64:
      ENCODE_SINGULAR_PRITIMIVE(uint64, UInt64)
      break;
    case INT32:
      ENCODE_SINGULAR_PRITIMIVE(int32, SInt32)
      break;
    case INT64:
      ENCODE_SINGULAR_PRITIMIVE(int64, SInt64)
      break;
    case BOOL:
      ENCODE_SINGULAR_PRITIMIVE(bool, Bool)
      break;
    case DOUBLE:
      ENCODE_SINGULAR_PRITIMIVE(double, Double)
      break;
    case STRING:
      ENCODE_SINGULAR_PRITIMIVE(std::string, String)
      break;
    case ENUMTYPE:
      ENCODE_SINGULAR_PRITIMIVE(uint32, UInt32)
      break;
    default:
      throw std::runtime_error(
                "type " +
                ProtoParser::PbCommon::GetTypeAsString(field->type()) +
                " is not primitive");
      break;
  }
  indent_num--;
  return std::shared_ptr<SerializedObjectInterface>(sdprim);
}


#define ENCODE_REPEATED_PRITIMIVE(WIRE_TYPE, CPP_TYPE, WIRE_TYPENAME)        \
  WireFormat::EncodeTag(                                                     \
    field->tag(), WIRE_TYPE, sdprim->mutable_ostream());                     \
  const auto& repeated_field =                                               \
      *reinterpret_cast<const RepeatedField<CPP_TYPE>*>(field_addr);         \
  WireFormat::WriteUInt32(repeated_field.size(), sdprim->mutable_ostream()); \
  for (const auto& value: repeated_field) {                                  \
    WireFormat::Write##WIRE_TYPENAME(value, sdprim->mutable_ostream());      \
  }


std::shared_ptr<SerializedObjectInterface>
MessageReflection::CreateSerializedRepeatedPrimitive(
    const Message* message,
    const FieldDescriptor* field)  const {
  // std::cout << indent() << "serializing repeated primitive: "
  //           << field->name() << std::endl;
  indent_num++;
  SerializedPrimitive* sdprim = new SerializedPrimitive(field->type());
  const char* field_addr =
      reinterpret_cast<const char*>(message) + FieldOffset(field);
  switch (field->type()) {
    case UINT32: {
      ENCODE_REPEATED_PRITIMIVE(WireFormat::WIRETYPE_VARIANT, uint32, UInt32)
      break;
    }
    case UINT64: {
      ENCODE_REPEATED_PRITIMIVE(WireFormat::WIRETYPE_VARIANT, uint64, UInt64)
      break;
    }
    case INT32: {
      ENCODE_REPEATED_PRITIMIVE(WireFormat::WIRETYPE_VARIANT, int32, SInt32)
      break;
    }
    case INT64: {
      ENCODE_REPEATED_PRITIMIVE(WireFormat::WIRETYPE_VARIANT, int64, SInt64)
      break;
    }
    case BOOL:{
      ENCODE_REPEATED_PRITIMIVE(WireFormat::WIRETYPE_VARIANT, bool, Bool)
      break;
    }
    case DOUBLE: {
      ENCODE_REPEATED_PRITIMIVE(WireFormat::WIRETYPE_FIXD64, double, Double)
      break;
    }
    case STRING: {
      // String is special because it's stored in RepeatedPtrField.
      WireFormat::EncodeTag(field->tag(), WireFormat::WIRETYPE_LENGTH_DELIMITED,
                            sdprim->mutable_ostream());
      const auto& repeated_field =
          *reinterpret_cast<const RepeatedPtrField<std::string>*>(field_addr);
      WireFormat::WriteUInt32(repeated_field.size(), sdprim->mutable_ostream());
      for (const auto& value: repeated_field) {
        WireFormat::WriteString(value, sdprim->mutable_ostream());
      }
      break;
    }
    case ENUMTYPE: {
      // Enum is speical because we can't cast field_field_addr raw pointer to
      // the templated RepeatedField. So instead we directly cast enum value to
      // uint32 by manipulating the underlying pointer.
      WireFormat::EncodeTag(
        field->tag(), WireFormat::WIRETYPE_VARIANT, sdprim->mutable_ostream());
      const auto repeated_field =
          reinterpret_cast<const RepeatedFieldBase*>(field_addr);
      WireFormat::WriteUInt32(repeated_field->size(),
                              sdprim->mutable_ostream());
      for (uint32 i = 0; i < repeated_field->size(); i++) {
        uint32 value =
            *reinterpret_cast<const uint32*>(repeated_field->GetElementPtr(i));
        WireFormat::WriteUInt32(value, sdprim->mutable_ostream());
      }
      break;
    }
    default:
      throw std::runtime_error(
                "type " +
                ProtoParser::PbCommon::GetTypeAsString(field->type()) +
                " is not primitive");
      break;
  }
  indent_num--;
  return std::shared_ptr<SerializedObjectInterface>(sdprim);
}

bool MessageReflection::HasField(const Message* message, int tag) const {
  const char* has_bits =
    reinterpret_cast<const char*>(message) + has_bits_offset_;
  return (has_bits[tag / 8] & (0x1 << (tag % 8))) != 0;
}

// -------------------------------------------------------------------------- //
// --------------------------- DeSerialize routines ------------------------- //
// -------------------------------------------------------------------------- //
template <typename T>
inline T* MessageReflection::Mutable_Raw(
    Message* message,
    const FieldDescriptor* field) const {
  char* ptr = reinterpret_cast<char*>(message) + FieldOffset(field);
  return reinterpret_cast<T*>(ptr);
}

template <typename T>
inline void MessageReflection::SetField(
    Message* message,
    const FieldDescriptor* field, T value) const {
  *Mutable_Raw<T>(message, field) = value;
}

template <typename T>
inline void MessageReflection::AddField(
    Message* message,
    const FieldDescriptor* field, T value) const {
  Mutable_Raw<RepeatedField<T>>(message, field) -> Add(value);
}

#define DEFINE_PRIMITIVE_ACCESSORS(CPP_TYPE, WIRE_TYPENAME)                \
  inline uint32 MessageReflection::Set##WIRE_TYPENAME(                     \
      Message* message,                                                    \
      const FieldDescriptor* field,                              \
      const char* buf) const {                                             \
    uint32 parsed_size;                                                    \
    CPP_TYPE value = WireFormat::Decode##WIRE_TYPENAME(buf, &parsed_size); \
    SetField<CPP_TYPE>(message, field, value);                             \
    SetHasBit(message, field->tag());                                      \
    return parsed_size;                                                    \
  }                                                                        \
                                                                           \
  inline uint32 MessageReflection::Add##WIRE_TYPENAME(                     \
      Message* message,                                                    \
      const FieldDescriptor* field,                              \
      const char* buf) const {                                             \
    uint32 parsed_size;                                                    \
    CPP_TYPE value = WireFormat::Decode##WIRE_TYPENAME(buf, &parsed_size); \
    AddField<CPP_TYPE>(message, field, value);                             \
    return parsed_size;                                                    \
  }                                                                        \


DEFINE_PRIMITIVE_ACCESSORS(uint32, UInt32)
DEFINE_PRIMITIVE_ACCESSORS(uint64, UInt64)
DEFINE_PRIMITIVE_ACCESSORS(int32, SInt32)
DEFINE_PRIMITIVE_ACCESSORS(int64, SInt64)
DEFINE_PRIMITIVE_ACCESSORS(bool, Bool)
DEFINE_PRIMITIVE_ACCESSORS(double, Double)

// String is special. Singular string type is nested while repeated string
// is allocated in RepeatedPtrField rather than RepeatedField.
inline uint32 MessageReflection::SetString(
    Message* message,
    const FieldDescriptor* field,
    const char* buf) const {
  uint32 parsed_size;
  std::string value = WireFormat::DecodeString(buf, &parsed_size);
  SetField<std::string>(message, field, value);
  SetHasBit(message, field->tag());
  return parsed_size;
}

inline uint32 MessageReflection::AddString(
    Message* message,
    const FieldDescriptor* field,
    const char* buf) const {
  uint32 parsed_size;
  std::string value = WireFormat::DecodeString(buf, &parsed_size);
  *(Mutable_Raw<RepeatedPtrField<std::string>>(message, field)->Add()) = value;
  return parsed_size;
}


void MessageReflection::SetHasBit(Message* message, const uint32 tag) const {
  char* has_bits =
    reinterpret_cast<char*>(message) + has_bits_offset_;
  has_bits[tag / 8] |= (0x1 << (tag % 8));
}

void MessageReflection::CheckWireType(
    WireFormat::WireType wire_type,
    FieldType type,
    FieldLabel label) const {
  if (type == UINT32 || type == UINT64 || type == INT32  || type == INT64  ||
      type == BOOL   || type == ENUMTYPE) {
    if (wire_type == WireFormat::WIRETYPE_VARIANT) {
      return;
    }
  }
  else if (type == DOUBLE) {
    if (wire_type == WireFormat::WIRETYPE_FIXD64) {
      return;
    }
  }
  else if (type == STRING || type == MESSAGETYPE) {
    if (wire_type == WireFormat::WIRETYPE_LENGTH_DELIMITED) {
      return;
    }
  }
  throw std::runtime_error(
      "WireType " + WireFormat::WireTypeAsString(wire_type) +
      " mismatch with  " +
      GetLabelAsString(label) + " "
      " FieldType " + ProtoParser::PbCommon::GetTypeAsString(type));
}

void MessageReflection::DeSerialize(
    Message* message,
    const char* buf,
    uint32 size) const {
  uint32 tag;
  WireFormat::WireType wire_type;
  uint32 parsed_size, offset = 0;
  // std::cout << indent() << "DeSerialize Size = " << size << std::endl;
  while (offset < size) {
    WireFormat::DecodeTag(buf + offset, &tag, &wire_type, &parsed_size);
    //std::cout << "parsed tag size = " << parsed_size << std::endl;
    offset += parsed_size;
    //std::cout << indent() << "tag = " << tag << std::endl;
    //std::cout << "tag parsed_size = " << parsed_size << std::endl;
    const FieldDescriptor* field =
        message_descirptor_->FindFieldByTag(tag);
    if (!field) {
      throw std::runtime_error(
        "Message " + message_descirptor_->name() +
        " has no field with tag " + std::to_string(tag));
    }

    CheckWireType(wire_type, field->type(), field->label());

    if (field->IsMessageType()) {
      if (field->IsSingularType()) {
        offset += DeSerializeSingularMessage(message, field, buf + offset);
      }
      else {
        offset += DeSerializeRepeatedMessage(message, field, buf + offset);
      }
    }
    // Primitive Type and String type
    else {
      if (field->IsSingularType()) {
        offset += DeSerializeSingularPrimitive(message, field, buf + offset);
      }
      else {
        offset += DeSerializeRepeatedPrimitive(message, field, buf + offset);
      }
    }
  }
  if (offset != size) {
    throw std::runtime_error(
        "parsed size exceeds for message " + message_descirptor_->name());
  }
}

uint32 MessageReflection::DeSerializeSingularMessage(
    Message* message,
    const FieldDescriptor* field,
    const char* buf) const {
  // std::cout << indent() << "DeSerializing singular nested message " << std::endl;
  indent_num++;
  uint32 offset = 0;
  uint32 obj_size = WireFormat::DecodeUInt32(buf, &offset);
  std::string class_name = 
      field->type_descriptor()->full_name();
  const MessageReflection* nested_msg_reflection = 
      MessageFactory::GetMessageReflection(class_name);
  if (!nested_msg_reflection) {
    throw std::runtime_error(
        "No cpp generated class type " + class_name + " exists.");
  }
  Message* new_obj = nested_msg_reflection->defatult_instance()->New();
  nested_msg_reflection->DeSerialize(new_obj, buf + offset, obj_size);
  char* field_addr = reinterpret_cast<char*>(message) + FieldOffset(field);
  *reinterpret_cast<Message**>(field_addr) = new_obj;
  SetHasBit(message, field->tag());
  indent_num--;
  return offset + obj_size;
}

uint32 MessageReflection::DeSerializeRepeatedMessage(
    Message* message,
    const FieldDescriptor* field,
    const char* buf) const {
  // std::cout << indent() << "DeSerializing repeated nested message " << std::endl;
  indent_num++;
  uint32 offset = 0;
  uint32 list_size = WireFormat::DecodeUInt32(buf, &offset);
  std::string class_name = 
      field->type_descriptor()->full_name();
  const MessageReflection* nested_msg_reflection =
      MessageFactory::GetMessageReflection(class_name);
  if (!nested_msg_reflection) {
    throw std::runtime_error(
        "No cpp generated class type " + class_name + " exists.");
  }

  char* field_addr = reinterpret_cast<char*>(message) + FieldOffset(field);
  // std::cout << indent()
  //           << "DeSerializing " << list_size << " objects" << std::endl;
  for (uint32 i = 0; i < list_size; i++) {
    // Parse object size first.
    // std::cout << indent() << "[... element " << i << " ...]" << std::endl;
    uint32 parsed_size = 0;
    uint32 obj_size = WireFormat::DecodeUInt32(buf + offset, &parsed_size);
    offset += parsed_size;
    // Deserialize object data.
    Message* new_obj = nested_msg_reflection->defatult_instance()->New();
    nested_msg_reflection->DeSerialize(new_obj, buf + offset, obj_size);
    reinterpret_cast<RepeatedPtrFieldBase*>(field_addr)->
                                                Add_Allocated_Raw(new_obj);
    offset += obj_size;
  }
  indent_num--;
  return offset;
}


uint32 MessageReflection::DeSerializeSingularPrimitive(
    Message* message,
    const FieldDescriptor* field,
    const char* buf) const {
  int offset = 0;
  //std::cout << indent() << "DeSerializing singular primitive " << std::endl;
  indent_num++;
  switch (field->type()) {
    case UINT32: {
      offset = SetUInt32(message, field, buf);
      break;
    }
    case UINT64: {
      offset = SetUInt64(message, field, buf);
      break;
    }
    case INT32: {
      offset = SetSInt32(message, field, buf);
      break;
    }
    case INT64: {
      offset = SetSInt64(message, field, buf);
      break;
    }
    case BOOL:{
      offset = SetBool(message, field, buf);
      break;
    }
    case DOUBLE: {
      offset = SetDouble(message, field, buf);
      break;
    }
    case ENUMTYPE: {
      offset = SetUInt32(message, field, buf);
      break;
    }
    case STRING: {
      offset = SetString(message, field, buf);
      break;
    }
    default:
      throw std::runtime_error(
                "type " +
                ProtoParser::PbCommon::GetTypeAsString(field->type()) +
                " is not primitive");
      break;
  }
  indent_num--;
  return offset;
}

uint32 MessageReflection::DeSerializeRepeatedPrimitive(
    Message* message,
    const FieldDescriptor* field,
    const char* buf) const {
  //std::cout << indent() << "DeSerializing repeated primitive " << std::endl;
  indent_num++;
  uint32 offset = 0;
  uint32 list_size = WireFormat::DecodeUInt32(buf, &offset);
  switch (field->type()) {
    case UINT32: {
      for (uint32 i = 0; i < list_size; i++) {
        offset += AddUInt32(message, field, buf + offset);
      }
      break;
    }
    case UINT64: {
      for (uint32 i = 0; i < list_size; i++) {
        offset += AddUInt64(message, field, buf + offset);
      }
      break;
    }
    case INT32: {
      for (uint32 i = 0; i < list_size; i++) {
        offset += AddSInt32(message, field, buf + offset);
      }
      break;
    }
    case INT64: {
      for (uint32 i = 0; i < list_size; i++) {
        offset += AddSInt64(message, field, buf + offset);
      }
      break;
    }
    case BOOL:{
      for (uint32 i = 0; i < list_size; i++) {
        offset += AddBool(message, field, buf + offset);
      }
      break;
    }
    case DOUBLE: {
      for (uint32 i = 0; i < list_size; i++) {
        offset += AddDouble(message, field, buf + offset);
      }
      break;
    }
    case ENUMTYPE: {
      for (uint32 i = 0; i < list_size; i++) {
        offset += AddUInt32(message, field, buf + offset);
      }
      break;
    }
    case STRING: {
      for (uint32 i = 0; i < list_size; i++) {
        offset += AddString(message, field, buf + offset);
      }
      break;
    }
    default:
      throw std::runtime_error(
                "type " +
                ProtoParser::PbCommon::GetTypeAsString(field->type()) +
                " is not primitive");
      break;
  }
  indent_num--;
  return offset;
}

}  // namespace proto