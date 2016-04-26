#ifndef GENERATED_MESSAGE_REFLECTION_
#define GENERATED_MESSAGE_REFLECTION_

#include <memory>
#include <stdexcept>

#include "Base/MacroUtils.h"
#include "Compiler/Message.h"
#include "Compiler/PbCommon.h"
#include "WireFormat.h"
#include "SerializedMessage.h"
#include "SerializedPrimitive.h"
#include "Descriptor.h"
#include "Message.h"

namespace proto {

#define PROTO_MESSAGE_FIELD_OFFSET(TYPE, FIELD)                \
  static_cast<int>(                                            \
      reinterpret_cast<const char*>(                           \
          &reinterpret_cast<const TYPE*>(16)->FIELD) -         \
      reinterpret_cast<const char*>(16))


class MessageReflection {
 public:
  MessageReflection(
      std::shared_ptr<::proto::ProtoParser::Message> message_descirptor,
      Message* defatult_instance,
      int has_bits_offset);

  const ::proto::ProtoParser::Message* descriptor() const;
  const Message* defatult_instance() const;
  SerializedMessage* Serialize(const Message* message) const;
  void DeSerialize(Message* message, const char* buf, uint32 size) const;

 private:
  bool HasField(const Message* message, int tag) const;

  // Get mutable raw field ptr from message.
  template <typename T>
  inline T* Mutable_Raw(Message* message,
                        const ProtoParser::MessageField* field) const;

  // Check a repeated field is empty.
  bool RepeatedFieldEmpty(
      const Message* message,
      const ProtoParser::MessageField* field) const;

  // Set field in message.
  template <typename T>
  inline void SetField(Message* message,
                       const ProtoParser::MessageField* field, T value) const;

  // Set field in message.
  template <typename T>
  inline void AddField(Message* message,
                       const ProtoParser::MessageField* field, T value) const;

  // Set message field routines
  inline uint32 SetUInt32(
      Message* message,
      const ProtoParser::MessageField* field, const char* buf) const;

  inline uint32 SetUInt64(
      Message* message,
      const ProtoParser::MessageField* field, const char* buf) const;

  inline uint32 SetSInt32(
      Message* message,
      const ProtoParser::MessageField* field, const char* buf) const;

  inline uint32 SetSInt64(
      Message* message,
      const ProtoParser::MessageField* field, const char* buf) const;

  inline uint32 SetBool(
      Message* message,
      const ProtoParser::MessageField* field, const char* buf) const;

  inline uint32 SetDouble(
      Message* message,
      const ProtoParser::MessageField* field, const char* buf) const;

  inline uint32 SetString(
      Message* message,
      const ProtoParser::MessageField* field, const char* buf) const;

  // Add repeated field routines
  inline uint32 AddUInt32(
      Message* message,
      const ProtoParser::MessageField* field, const char* buf) const;

  inline uint32 AddUInt64(
      Message* message,
      const ProtoParser::MessageField* field, const char* buf) const;

  inline uint32 AddSInt32(
      Message* message,
      const ProtoParser::MessageField* field, const char* buf) const;

  inline uint32 AddSInt64(
      Message* message,
      const ProtoParser::MessageField* field, const char* buf) const;

  inline uint32 AddBool(
      Message* message,
      const ProtoParser::MessageField* field, const char* buf) const;

  inline uint32 AddDouble(
      Message* message,
      const ProtoParser::MessageField* field, const char* buf) const;

  inline uint32 AddString(
      Message* message,
      const ProtoParser::MessageField* field, const char* buf) const;

  // Set has_bit in message for a field.
  void SetHasBit(Message* message, const uint32 tag) const;

  // Check wire type matches field type
  void CheckWireType(WireFormat::WireType wire_type,
                     FieldType type, FieldLabel modifier) const;

  //std::string indent() const;

  // Create a serialized singular message object.
  std::shared_ptr<SerializedObjectInterface>
  CreateSerializedSingularMessage(
    const Message* message,
    const ProtoParser::MessageField* field)  const;

  // Create a serialized repeated message object.
  std::shared_ptr<SerializedObjectInterface>
  CreateSerializedRepeatedMessage(
    const Message* message,
    const ProtoParser::MessageField* field)  const;

  // Create a serialized singular primitive object.
  std::shared_ptr<SerializedObjectInterface>
  CreateSerializedSingularPrimitive(
    const Message* message,
    const ProtoParser::MessageField* field) const;

  // Create a repeated singular primitive object.
  std::shared_ptr<SerializedObjectInterface>
  CreateSerializedRepeatedPrimitive(
    const Message* message,
    const ProtoParser::MessageField* field) const;


  // Deserialize a singular primitive field.
  uint32 DeSerializeSingularPrimitive(
    Message* message,
    const ProtoParser::MessageField* field,
    const char* buf) const;

  // Deserialize a repeated primitive field.
  uint32 DeSerializeRepeatedPrimitive(
    Message* message,
    const ProtoParser::MessageField* field,
    const char* buf) const;

  // Deserialize a singular message field.
  uint32 DeSerializeSingularMessage(
    Message* message,
    const ProtoParser::MessageField* field,
    const char* buf) const;

  // Deserialize a repeated message field.
  uint32 DeSerializeRepeatedMessage(
    Message* message,
    const ProtoParser::MessageField* field,
    const char* buf) const;

 private:
  std::shared_ptr<::proto::ProtoParser::Message> message_descirptor_;
  Message* defatult_instance_;
  int has_bits_offset_;
};

}  // namespace proto

#endif  /* GENERATED_MESSAGE_REFLECTION_ */