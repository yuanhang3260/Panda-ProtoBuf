#include "Proto/MessageReflection.h"

#include "Base/Log.h"
#include "Proto/Common.h"
#include "Proto/RepeatedFields.h"
#include "Proto/MessageFactory.h"
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
  fields_offset_(fields_offset),
  has_bits_offset_(has_bits_offset) { 
}

MessageReflection::~MessageReflection() {
}

const MessageDescriptor* MessageReflection::descriptor() const {
  return message_descirptor_.get();
}

const Message* MessageReflection::defatult_instance() const {
  return defatult_instance_;
}

int MessageReflection::num_fields() const {
  return message_descirptor_->num_fields();
}

int MessageReflection::num_nested_enum_types() const {
  return message_descirptor_->num_nested_enums();
}

const FieldDescriptor*
MessageReflection::FindFieldByName(const std::string& name) const {
  return message_descirptor_->FindFieldByName(name);
}

const FieldDescriptor* MessageReflection::FindFieldByTag(int tag) const {
  return message_descirptor_->FindFieldByTag(tag);
}

const EnumDescriptor*
MessageReflection::FindNestedEnumByName(const std::string& name) const {
  return message_descirptor_->FindNestedEnumTypeByName(name);
}

bool MessageReflection::HasField(const Message* message, int tag) const {
  if (message_descirptor_.get() != message->GetDescriptor()) {
    LogFATAL("reflection is of message %s, but given message is %s",
             message_descirptor_->full_name().c_str(),
             message->GetDescriptor()->full_name().c_str());
  }
  if (message_descirptor_->FindFieldByTag(tag) == nullptr) {
    LogERROR("No field has tag %d in message %s",
             tag,  message_descirptor_->full_name().c_str());
  }
  const char* has_bits =
    reinterpret_cast<const char*>(message) + has_bits_offset_;
  return (has_bits[tag / 8] & (0x1 << (tag % 8))) != 0;
}

bool MessageReflection::HasField(const Message* message,
                                 const FieldDescriptor* field) const {
  if (!message || !field ||
      !field->container_message() ||! message->GetDescriptor()) {
    LogFATAL("nullptr input to HasField");
  }
  if (field->container_message() != message->GetDescriptor()) {
    LogFATAL("field %s is in message %s, but given message is %s",
             field->name().c_str(),
             field->container_message()->full_name().c_str(),
             message->GetDescriptor()->full_name().c_str());
  }
  if (message_descirptor_.get() != message->GetDescriptor()) {
    LogFATAL("reflection is of message %s, but given message is %s",
             message_descirptor_->full_name().c_str(),
             message->GetDescriptor()->full_name().c_str());
  }
  const char* has_bits =
    reinterpret_cast<const char*>(message) + has_bits_offset_;
  return (has_bits[field->tag() / 8] & (0x1 << (field->tag() % 8))) != 0;
}

Message* MessageReflection::NewObj() const {
  return defatult_instance_->New();
}

#define CHECK_ACCESSORS_ARGS(TYPE)                                             \
    if (!message || !field ||                                                  \
        !field->container_message() ||! message->GetDescriptor()) {            \
      LogFATAL("nullptr input to get %s", TypeAsString(TYPE).c_str());         \
    }                                                                          \
    if (field->container_message() != message->GetDescriptor()) {              \
      LogFATAL("field %s is in message %s, but given message is %s",           \
               field->name().c_str(),                                          \
               field->container_message()->full_name().c_str(),                \
               message->GetDescriptor()->full_name().c_str());                 \
    }                                                                          \
    if (message_descirptor_.get() != message->GetDescriptor()) {               \
      LogFATAL("reflection is of message %s, but given message is %s",         \
               message_descirptor_->full_name().c_str(),                       \
               message->GetDescriptor()->full_name().c_str());                 \
    }                                                                          \
    if (field->type() != TYPE) {                                               \
      LogFATAL("Expect field type %s, but given %s",                           \
               TypeAsString(TYPE).c_str(),TypeAsString(field->type()).c_str());\
    }                                                                          \

#define DEFINE_PRIMITIVE_GETTERS(NAME, CPP_TYPE, TYPE)                         \
  CPP_TYPE MessageReflection::Get##NAME(const Message* message,                \
                                        const FieldDescriptor* field) const {  \
    CHECK_ACCESSORS_ARGS(TYPE)                                                 \
    if (field->IsRepeatedType()) {                                             \
      LogFATAL("Expect optional/required field, given repeated field %s",      \
               field->name().c_str());                                         \
    }                                                                          \
    return GetField<CPP_TYPE>(message, field);                                 \
  }                                                                            \
                                                                               \
  CPP_TYPE MessageReflection::GetRepeated##NAME(const Message* message,        \
                                                const FieldDescriptor* field,  \
                                                int index) const {             \
    CHECK_ACCESSORS_ARGS(TYPE)                                                 \
    if (!field->IsRepeatedType()) {                                            \
      LogFATAL("Expect repeated field, given field is of label_%s",            \
               field->name().c_str());                                         \
    }                                                                          \
    return GetRepeatedField<CPP_TYPE>(message, field, index);                  \
  }                                                                            \

DEFINE_PRIMITIVE_GETTERS(UInt32, uint32, UINT32);
DEFINE_PRIMITIVE_GETTERS(UInt64, uint64, UINT64);
DEFINE_PRIMITIVE_GETTERS(Int32, int32, INT32);
DEFINE_PRIMITIVE_GETTERS(Int64, int64, INT64);
DEFINE_PRIMITIVE_GETTERS(Double, double, DOUBLE);
DEFINE_PRIMITIVE_GETTERS(Bool, bool, BOOL);
DEFINE_PRIMITIVE_GETTERS(Enum, uint32, ENUMTYPE);

std::string MessageReflection::GetString(const Message* message,
                                         const FieldDescriptor* field) const {
  CHECK_ACCESSORS_ARGS(STRING)
  if (field->IsRepeatedType()) {
    LogFATAL("Expect optional/required field, given repeated field %s",
             field->name().c_str());
  }
  return GetField<std::string>(message, field);
}

std::string MessageReflection::GetRepeatedString(const Message* message,
                                                 const FieldDescriptor* field,
                                                 int index) const {  
  CHECK_ACCESSORS_ARGS(STRING)
  if (!field->IsRepeatedType()) {
    LogFATAL("Expect repeated field, given repeated field %s",
             field->name().c_str());
  }
  auto array = Const_Raw<const RepeatedPtrField<std::string>>(message, field);
  if (index < 0 || index >= (int)array->size()) {
    LogFATAL("index out of repeated field bound");
  }
  return array->Get(index);
}

const Message&
MessageReflection::GetMessage(const Message* message,
                              const FieldDescriptor* field) const {
  CHECK_ACCESSORS_ARGS(MESSAGETYPE)
  if (field->IsRepeatedType()) {
    LogFATAL("Expect optional/required field, given repeated field %s",
             field->name().c_str());
  }
  const char* ptr = reinterpret_cast<const char*>(message);
  return *(*reinterpret_cast<Message* const*>(ptr + FieldOffset(field)));
}

const Message&
MessageReflection::GetRepeatedMessage(const Message* message,
                                      const FieldDescriptor* field,
                                      int index) const {
  CHECK_ACCESSORS_ARGS(MESSAGETYPE)
  if (!field->IsRepeatedType()) {
    LogFATAL("Expect repeated field, given repeated field %s",
             field->name().c_str());
  }
  const char* ptr = reinterpret_cast<const char*>(message);
  const auto array =
      reinterpret_cast<const RepeatedFieldBase*>(ptr + FieldOffset(field));
  if (index < 0 || index >= (int)array->size()) {
    LogFATAL("index out of repeated field bound");
  }
  const char* element_ptr = array->GetElementPtr(index);
  return *reinterpret_cast<const Message*>(element_ptr);
}

/// Field setters
#define DEFINE_PRIMITIVE_SETTERS(NAME, CPP_TYPE, TYPE)                         \
  void MessageReflection::Set##NAME(Message* message,                          \
                                    const FieldDescriptor* field,              \
                                    CPP_TYPE value) const {                    \
    CHECK_ACCESSORS_ARGS(TYPE)                                                 \
    if (field->IsRepeatedType()) {                                             \
      LogFATAL("Expect optional/required field, given repeated field %s",      \
               field->name().c_str());                                         \
    }                                                                          \
    SetField<CPP_TYPE>(message, field, value);                                 \
    SetHasBit(message, field->tag());                                                 \
  }                                                                            \
                                                                               \
  void MessageReflection::SetRepeated##NAME(Message* message,                  \
                                    const FieldDescriptor* field,              \
                                    int index, CPP_TYPE value) const {         \
    CHECK_ACCESSORS_ARGS(TYPE)                                                 \
    if (!field->IsRepeatedType()) {                                            \
      LogFATAL("Expect repeated field, given field is of label_%s",            \
               field->name().c_str());                                         \
    }                                                                          \
    SetRepeatedField<CPP_TYPE>(message, field, index, value);                  \
  }                                                                            \
                                                                               \
  void MessageReflection::Add##NAME(Message* message,                          \
                                    const FieldDescriptor* field,              \
                                    CPP_TYPE value) const {                    \
    CHECK_ACCESSORS_ARGS(TYPE)                                                 \
    if (!field->IsRepeatedType()) {                                            \
      LogFATAL("Expect repeated field, given repeated field %s",               \
               field->name().c_str());                                         \
    }                                                                          \
    AddField<CPP_TYPE>(message, field, value);                                 \
  }                                                                            \

DEFINE_PRIMITIVE_SETTERS(UInt32, uint32, UINT32);
DEFINE_PRIMITIVE_SETTERS(UInt64, uint64, UINT64);
DEFINE_PRIMITIVE_SETTERS(Int32, int32, INT32);
DEFINE_PRIMITIVE_SETTERS(Int64, int64, INT64);
DEFINE_PRIMITIVE_SETTERS(Double, double, DOUBLE);
DEFINE_PRIMITIVE_SETTERS(Bool, bool, BOOL);

void MessageReflection::SetEnum(Message* message,
                                const FieldDescriptor* field,
                                uint32 value) const {
  CHECK_ACCESSORS_ARGS(ENUMTYPE)
  if (field->IsRepeatedType()) {
    LogFATAL("Expect optional/required field, given repeated field %s",
             field->name().c_str());
  }
  auto enum_descriptor =
            dynamic_cast<const EnumDescriptor*>(field->type_descriptor());
  if (!enum_descriptor->ContainsEnum(value)) {
    LogFATAL("enum type %s doesn't contain enum value %d",
             enum_descriptor->full_name().c_str(), value);
  }
  SetField<uint32>(message, field, value);
  SetHasBit(message, field->tag());
}

void MessageReflection::SetRepeatedEnum(Message* message,
                                        const FieldDescriptor* field,
                                        int index, uint32 value) const {
  CHECK_ACCESSORS_ARGS(ENUMTYPE)
  if (!field->IsRepeatedType()) {
    LogFATAL("Expect repeated field, given field is of label_%s",
             field->name().c_str());
  }
  auto enum_descriptor =
            dynamic_cast<const EnumDescriptor*>(field->type_descriptor());
  if (!enum_descriptor->ContainsEnum(value)) {
    LogFATAL("enum type %s doesn't contain enum value %d",
             enum_descriptor->full_name().c_str(), value);
  }
  SetRepeatedField<uint32>(message, field, index, value);
}

void MessageReflection::AddEnum(Message* message,
                                const FieldDescriptor* field,
                                uint32 value) const {
  CHECK_ACCESSORS_ARGS(ENUMTYPE)
  if (!field->IsRepeatedType()) {
    LogFATAL("Expect repeated field, given field is of label_%s",
             field->name().c_str());
  }
  auto enum_descriptor =
            dynamic_cast<const EnumDescriptor*>(field->type_descriptor());
  if (!enum_descriptor->ContainsEnum(value)) {
    LogFATAL("enum type %s doesn't contain enum value %d",
             enum_descriptor->full_name().c_str(), value);
  }
  AddField<uint32>(message, field, value);
}

void MessageReflection::SetString(Message* message,
                                  const FieldDescriptor* field,
                                  const std::string& value) const {
  CHECK_ACCESSORS_ARGS(STRING)
  if (field->IsRepeatedType()) {
    LogFATAL("Expect optional/required field, given repeated field %s",
             field->name().c_str());
  }
  SetField<std::string>(message, field, value);
}

void MessageReflection::SetRepeatedString(Message* message,
                                          const FieldDescriptor* field,
                                          int index,
                                          const std::string& value) const {
  CHECK_ACCESSORS_ARGS(STRING)
  if (!field->IsRepeatedType()) {
    LogFATAL("Expect repeated field, given repeated field %s",
             field->name().c_str());
  }
  char* ptr = reinterpret_cast<char*>(message);
  auto array = reinterpret_cast<RepeatedPtrField<std::string>*>(
                                              ptr + FieldOffset(field));
  if (index < 0 || index >= (int)array->size()) {
    LogFATAL("index out of repeated field bound");
  }
  array->Set(index, value);
}

void MessageReflection::AddString(Message* message,
                                  const FieldDescriptor* field,
                                  std::string value) const {
  CHECK_ACCESSORS_ARGS(STRING)
  if (!field->IsRepeatedType()) {
    LogFATAL("Expect repeated field, given repeated field %s",
             field->name().c_str());
  }
  char* ptr = reinterpret_cast<char*>(message);
  auto array = reinterpret_cast<RepeatedPtrField<std::string>*>(
                                              ptr + FieldOffset(field));
  array->AddAllocated(new std::string(value));
}

Message* MessageReflection::MutableMessage(Message* message,
                                           const FieldDescriptor* field) const {
  CHECK_ACCESSORS_ARGS(MESSAGETYPE)
  if (field->IsRepeatedType()) {
    LogFATAL("Expect optional/required field, given repeated field %s",
             field->name().c_str());
  }
  char* ptr = reinterpret_cast<char*>(message);
  Message** field_ptr = reinterpret_cast<Message**>(ptr + FieldOffset(field));
  if (!HasField(message, field->tag())) {
    std::string nested_sdmsg_name = field->type_descriptor()->full_name();
    const MessageReflection* nested_msg_reflection =
                  MessageFactory::GetMessageReflection(nested_sdmsg_name);
    *field_ptr = nested_msg_reflection->NewObj();
    SetHasBit(message, field->tag());
  }
  return *field_ptr;
}

void MessageReflection::SetAllocatedMessage(Message* message,
                                            const FieldDescriptor* field,
                                            Message* submessage) const {
  CHECK_ACCESSORS_ARGS(MESSAGETYPE)
  if (field->IsRepeatedType()) {
    LogFATAL("Expect optional/required field, given repeated field %s",
             field->name().c_str());
  }
  char* ptr = reinterpret_cast<char*>(message);
  Message** field_ptr = reinterpret_cast<Message**>(ptr + FieldOffset(field));
  if (HasField(message, field->tag())) {
    delete *field_ptr;
  }
  SetHasBit(message, field->tag());
  *field_ptr = submessage;
}

Message* MessageReflection::ReleaseMessage(Message* message,
                                           const FieldDescriptor* field) const {
  CHECK_ACCESSORS_ARGS(MESSAGETYPE)
  if (field->IsRepeatedType()) {
    LogFATAL("Expect optional/required field, given repeated field %s",
             field->name().c_str());
  }
  char* ptr = reinterpret_cast<char*>(message);
  if (!HasField(message, field->tag())) {
    return nullptr;
  }
  Message** field_ptr = reinterpret_cast<Message**>(ptr + FieldOffset(field));
  Message* tmp = *field_ptr;
  *field_ptr = nullptr;
  ClearHasBit(message, field->tag());
  return tmp;
}

Message* MessageReflection::MutableRepeatedMessage(Message* message,
                                                   const FieldDescriptor* field,
                                                   int index) const {
  CHECK_ACCESSORS_ARGS(MESSAGETYPE)
  if (!field->IsRepeatedType()) {
    LogFATAL("Expect repeated field, given repeated field %s",
             field->name().c_str());
  }
  char* ptr = reinterpret_cast<char*>(message);
  auto array =
      reinterpret_cast<RepeatedPtrFieldBase*>(ptr + FieldOffset(field));
  if (index < 0 || index >= (int)array->size()) {
    LogFATAL("index out of repeated field bound");
  }
  char* element_ptr = array->GetMutableRaw(index);
  return reinterpret_cast<Message*>(element_ptr);
}

void MessageReflection::AddMessage(Message* message,
                                   const FieldDescriptor* field,
                                   Message* submessage) const {
  CHECK_ACCESSORS_ARGS(MESSAGETYPE)
  if (!field->IsRepeatedType()) {
    LogFATAL("Expect repeated field, given repeated field %s",
             field->name().c_str());
  }
  char* ptr = reinterpret_cast<char*>(message);
  auto array =
      reinterpret_cast<RepeatedPtrFieldBase*>(ptr + FieldOffset(field));
  array->Add_Allocated_Raw(submessage);
}

// -------------------------------------------------------------------------- //
// ---------------------------- Serialize routines -------------------------- //
// -------------------------------------------------------------------------- //
int MessageReflection::FieldOffset(const FieldDescriptor* field) const {
  return fields_offset_[field->parse_index_];
}

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
inline const T* MessageReflection::Const_Raw(
    const Message* message,
    const FieldDescriptor* field) const {
  const char* ptr = reinterpret_cast<const char*>(message) + FieldOffset(field);
  return reinterpret_cast<const T*>(ptr);
}

template <typename T>
inline T MessageReflection::GetField(
    const Message* message,
    const FieldDescriptor* field) const {
  return *Const_Raw<T>(message, field);
}

template <typename T>
inline T MessageReflection::GetRepeatedField(
    const Message* message,
    const FieldDescriptor* field,
    int index) const {
  auto array = Const_Raw<RepeatedField<T>>(message, field);
  if (index < 0 || index >= (int)array->size()) {
    LogFATAL("index out of repeated field bound");
  }
  return array->Get(index);
}

template <typename T>
inline void MessageReflection::SetField(
    Message* message,
    const FieldDescriptor* field, T value) const {
  *Mutable_Raw<T>(message, field) = value;
}

template <typename T>
inline void MessageReflection::SetRepeatedField(
    Message* message,
    const FieldDescriptor* field,
    int index, T value) const {
  auto array = Mutable_Raw<RepeatedField<T>>(message, field);
  if (index < 0 || index >= (int)array->size()) {
    LogFATAL("index out of repeated field bound");
  }
  array->Set(index, value);
}

template <typename T>
inline void MessageReflection::AddField(
    Message* message,
    const FieldDescriptor* field, T value) const {
  Mutable_Raw<RepeatedField<T>>(message, field) -> Add(value);
}

#define DEFINE_PRIMITIVE_DECODERS(CPP_TYPE, WIRE_TYPENAME)                \
  inline uint32 MessageReflection::Set##WIRE_TYPENAME##FromCord(           \
      Message* message,                                                    \
      const FieldDescriptor* field,                                        \
      const char* buf) const {                                             \
    uint32 parsed_size;                                                    \
    CPP_TYPE value = WireFormat::Decode##WIRE_TYPENAME(buf, &parsed_size); \
    SetField<CPP_TYPE>(message, field, value);                             \
    SetHasBit(message, field->tag());                                      \
    return parsed_size;                                                    \
  }                                                                        \
                                                                           \
  inline uint32 MessageReflection::Add##WIRE_TYPENAME##FromCord(           \
      Message* message,                                                    \
      const FieldDescriptor* field,                                        \
      const char* buf) const {                                             \
    uint32 parsed_size;                                                    \
    CPP_TYPE value = WireFormat::Decode##WIRE_TYPENAME(buf, &parsed_size); \
    AddField<CPP_TYPE>(message, field, value);                             \
    return parsed_size;                                                    \
  }                                                                        \


DEFINE_PRIMITIVE_DECODERS(uint32, UInt32)
DEFINE_PRIMITIVE_DECODERS(uint64, UInt64)
DEFINE_PRIMITIVE_DECODERS(int32, SInt32)
DEFINE_PRIMITIVE_DECODERS(int64, SInt64)
DEFINE_PRIMITIVE_DECODERS(bool, Bool)
DEFINE_PRIMITIVE_DECODERS(double, Double)

// String is special. Singular string type is nested while repeated string
// is allocated in RepeatedPtrField rather than RepeatedField.
inline uint32 MessageReflection::SetStringFromCord(
    Message* message,
    const FieldDescriptor* field,
    const char* buf) const {
  uint32 parsed_size;
  std::string value = WireFormat::DecodeString(buf, &parsed_size);
  SetField<std::string>(message, field, value);
  SetHasBit(message, field->tag());
  return parsed_size;
}

inline uint32 MessageReflection::AddStringFromCord(
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

void MessageReflection::ClearHasBit(Message* message, const uint32 tag) const {
  char* has_bits =
    reinterpret_cast<char*>(message) + has_bits_offset_;
  has_bits[tag / 8] &= (~(0x1 << (tag % 8)));
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
      LabelAsString(label) + " "
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
      offset = SetUInt32FromCord(message, field, buf);
      break;
    }
    case UINT64: {
      offset = SetUInt64FromCord(message, field, buf);
      break;
    }
    case INT32: {
      offset = SetSInt32FromCord(message, field, buf);
      break;
    }
    case INT64: {
      offset = SetSInt64FromCord(message, field, buf);
      break;
    }
    case BOOL:{
      offset = SetBoolFromCord(message, field, buf);
      break;
    }
    case DOUBLE: {
      offset = SetDoubleFromCord(message, field, buf);
      break;
    }
    case ENUMTYPE: {
      offset = SetUInt32FromCord(message, field, buf);
      break;
    }
    case STRING: {
      offset = SetStringFromCord(message, field, buf);
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
        offset += AddUInt32FromCord(message, field, buf + offset);
      }
      break;
    }
    case UINT64: {
      for (uint32 i = 0; i < list_size; i++) {
        offset += AddUInt64FromCord(message, field, buf + offset);
      }
      break;
    }
    case INT32: {
      for (uint32 i = 0; i < list_size; i++) {
        offset += AddSInt32FromCord(message, field, buf + offset);
      }
      break;
    }
    case INT64: {
      for (uint32 i = 0; i < list_size; i++) {
        offset += AddSInt64FromCord(message, field, buf + offset);
      }
      break;
    }
    case BOOL:{
      for (uint32 i = 0; i < list_size; i++) {
        offset += AddBoolFromCord(message, field, buf + offset);
      }
      break;
    }
    case DOUBLE: {
      for (uint32 i = 0; i < list_size; i++) {
        offset += AddDoubleFromCord(message, field, buf + offset);
      }
      break;
    }
    case ENUMTYPE: {
      for (uint32 i = 0; i < list_size; i++) {
        offset += AddUInt32FromCord(message, field, buf + offset);
      }
      break;
    }
    case STRING: {
      for (uint32 i = 0; i < list_size; i++) {
        offset += AddStringFromCord(message, field, buf + offset);
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