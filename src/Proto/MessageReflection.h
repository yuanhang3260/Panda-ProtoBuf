#ifndef GENERATED_MESSAGE_REFLECTION_
#define GENERATED_MESSAGE_REFLECTION_

#include <memory>
#include <stdexcept>

#include "Base/MacroUtils.h"
#include "Proto/Common.h"
#include "Proto/Descriptor.h"
#include "Proto/WireFormat.h"
#include "Proto/SerializedMessage.h"
#include "Proto/SerializedPrimitive.h"
#include "Proto/Descriptor.h"
#include "Proto/Message.h"

namespace proto {

#define PROTO_MESSAGE_FIELD_OFFSET(TYPE, FIELD)                \
  static_cast<int>(                                            \
      reinterpret_cast<const char*>(                           \
          &reinterpret_cast<const TYPE*>(16)->FIELD) -         \
      reinterpret_cast<const char*>(16))


class MessageReflection {
 public:
  MessageReflection(
      const MessageDescriptor* message_descirptor,
      const Message* defatult_instance,
      const int* fields_offset,
      int has_bits_offset);

  ~MessageReflection();

  const MessageDescriptor* descriptor() const;
  const Message* defatult_instance() const;

  int num_fields() const;
  int num_nested_enum_types() const;

  // Find a field descriptor by field name.
  const FieldDescriptor* FindFieldByName(const std::string& name) const;

  // Find a field descriptor by tag name.
  const FieldDescriptor* FindFieldByTag(int tag) const;

  // Find a nested enum type by the type name. The name being used is the local
  // name of this enum type, not full name with package prefix.
  const EnumDescriptor* FindNestedEnumByName(const std::string& name) const;

  bool HasField(const Message* message, int tag) const;
  bool HasField(const Message* message, const FieldDescriptor* field) const;

  Message* NewObj() const;

  // Get singular field value.
  uint32 GetUInt32(const Message* message, const FieldDescriptor* field) const;
  uint64 GetUInt64(const Message* message, const FieldDescriptor* field) const;
  int32 GetInt32(const Message* message, const FieldDescriptor* field) const;
  int64 GetInt64(const Message* message, const FieldDescriptor* field) const;
  bool GetBool(const Message* message, const FieldDescriptor* field) const;
  double GetDouble(const Message* message, const FieldDescriptor* field) const;
  uint32 GetEnum(const Message* message, const FieldDescriptor* field) const;
  std::string GetString(const Message* message,
                        const FieldDescriptor* field) const;
  const Message& GetMessage(const Message* message,
                            const FieldDescriptor* field) const;

  // Get repeated field value.
  uint32 GetRepeatedUInt32(const Message* message,
                           const FieldDescriptor* field, int index) const;
  uint64 GetRepeatedUInt64(const Message* message,
                           const FieldDescriptor* field, int index) const;
  int32 GetRepeatedInt32(const Message* message,
                         const FieldDescriptor* field, int index) const;
  int64 GetRepeatedInt64(const Message* message,
                         const FieldDescriptor* field, int index) const;
  bool GetRepeatedBool(const Message* message,
                       const FieldDescriptor* field, int index) const;
  double GetRepeatedDouble(const Message* message,
                           const FieldDescriptor* field, int index) const;
  uint32 GetRepeatedEnum(const Message* message,
                         const FieldDescriptor* field, int index) const;
  std::string GetRepeatedString(const Message* message,
                                const FieldDescriptor* field, int index) const;
  const Message& GetRepeatedMessage(const Message* message,
                                    const FieldDescriptor* field,
                                    int index) const;

  // Set singular field value.
  void SetUInt32(Message* message,
                 const FieldDescriptor* field, uint32 value) const;
  void SetUInt64(Message* message,
                 const FieldDescriptor* field, uint64 value) const;
  void SetInt32(Message* message,
                const FieldDescriptor* field, int32 value) const;
  void SetInt64(Message* message,
                const FieldDescriptor* field, int64 value) const;
  void SetBool(Message* message,
               const FieldDescriptor* field, bool value) const;
  void SetDouble(Message* message,
                 const FieldDescriptor* field, double value) const;
  void SetEnum(Message* message,
               const FieldDescriptor* field, uint32 value) const;
  void SetString(Message* message, const FieldDescriptor* field,
                 const std::string& value) const;
  Message* MutableMessage(Message* message,
                          const FieldDescriptor* field) const;
  void SetAllocatedMessage(Message* message,
                           const FieldDescriptor* field,
                           Message* submessage) const;
  Message* ReleaseMessage(Message* message,
                          const FieldDescriptor* field) const;

  // Set repeated field value.
  void SetRepeatedUInt32(Message* message, const FieldDescriptor* field,
                         int index, uint32 value) const;
  void SetRepeatedUInt64(Message* message, const FieldDescriptor* field,
                         int index, uint64 value) const;
  void SetRepeatedInt32(Message* message, const FieldDescriptor* field,
                        int index, int32 value) const;
  void SetRepeatedInt64(Message* message, const FieldDescriptor* field,
                        int index, int64 value) const;
  void SetRepeatedBool(Message* message, const FieldDescriptor* field,
                       int index, bool value) const;
  void SetRepeatedDouble(Message* message, const FieldDescriptor* field,
                         int index, double value) const;
  void SetRepeatedEnum(Message* message, const FieldDescriptor* field,
                       int index, uint32 value) const;
  void SetRepeatedString(Message* message, const FieldDescriptor* field,
                         int index, const std::string& value) const;
  Message* MutableRepeatedMessage(Message* message,
                                  const FieldDescriptor* field,
                                  int index) const;

  // Add repeated field value.
  void AddUInt32(Message* message,
                 const FieldDescriptor* field, uint32 value) const;
  void AddUInt64(Message* message,
                 const FieldDescriptor* field, uint64 value) const;
  void AddInt32(Message* message,
                const FieldDescriptor* field, int32 value) const;
  void AddInt64(Message* message,
                const FieldDescriptor* field, int64 value) const;
  void AddBool(Message* message,
               const FieldDescriptor* field, bool value) const;
  void AddDouble(Message* message,
                 const FieldDescriptor* field, double value) const;
  void AddEnum(Message* message,
               const FieldDescriptor* field, uint32 value) const;
  void AddString(Message* message,
                 const FieldDescriptor* field, std::string value) const;
  void AddMessage(Message* message,
                  const FieldDescriptor* field,
                  Message* submessage) const;

  // Serialize and deserialize.
  SerializedMessage* Serialize(const Message* message) const;
  void DeSerialize(Message* message, const char* buf, uint32 size) const;

 private:
  // Get raw field ptr from message.
  template <typename T>
  inline const T* Const_Raw(const Message* message,
                            const FieldDescriptor* field) const;

  template <typename T>
  inline T* Mutable_Raw(Message* message,
                        const FieldDescriptor* field) const;

  // Check a repeated field is empty.
  bool RepeatedFieldEmpty(
      const Message* message,
      const FieldDescriptor* field) const;

  // Field accessors.
  template <typename T>
  inline T GetField(const Message* message,
                    const FieldDescriptor* field) const;

  template <typename T>
  inline T GetRepeatedField(const Message* message,
                            const FieldDescriptor* field,
                            int index) const;

  template <typename T>
  inline void SetField(Message* message,
                       const FieldDescriptor* field, T value) const;

  template <typename T>
  inline void SetRepeatedField(Message* message,
                               const FieldDescriptor* field,
                               int index, T value) const;

  template <typename T>
  inline void AddField(Message* message,
                       const FieldDescriptor* field, T value) const;

  // Set message field routines
  inline uint32 SetUInt32FromCord(
      Message* message,
      const FieldDescriptor* field, const char* buf) const;

  inline uint32 SetUInt64FromCord(
      Message* message,
      const FieldDescriptor* field, const char* buf) const;

  inline uint32 SetSInt32FromCord(
      Message* message,
      const FieldDescriptor* field, const char* buf) const;

  inline uint32 SetSInt64FromCord(
      Message* message,
      const FieldDescriptor* field, const char* buf) const;

  inline uint32 SetBoolFromCord(
      Message* message,
      const FieldDescriptor* field, const char* buf) const;

  inline uint32 SetDoubleFromCord(
      Message* message,
      const FieldDescriptor* field, const char* buf) const;

  inline uint32 SetStringFromCord(
      Message* message,
      const FieldDescriptor* field, const char* buf) const;

  // Add repeated field routines
  inline uint32 AddUInt32FromCord(
      Message* message,
      const FieldDescriptor* field, const char* buf) const;

  inline uint32 AddUInt64FromCord(
      Message* message,
      const FieldDescriptor* field, const char* buf) const;

  inline uint32 AddSInt32FromCord(
      Message* message,
      const FieldDescriptor* field, const char* buf) const;

  inline uint32 AddSInt64FromCord(
      Message* message,
      const FieldDescriptor* field, const char* buf) const;

  inline uint32 AddBoolFromCord(
      Message* message,
      const FieldDescriptor* field, const char* buf) const;

  inline uint32 AddDoubleFromCord(
      Message* message,
      const FieldDescriptor* field, const char* buf) const;

  inline uint32 AddStringFromCord(
      Message* message,
      const FieldDescriptor* field, const char* buf) const;

  // Set has_bit in message for a field.
  void SetHasBit(Message* message, const uint32 tag) const;
  void ClearHasBit(Message* message, const uint32 tag) const;

  // Check wire type matches field type
  void CheckWireType(WireFormat::WireType wire_type,
                     FieldType type, FieldLabel modifier) const;

  //std::string indent() const;

  // Create a serialized singular message object.
  std::shared_ptr<SerializedObjectInterface>
  CreateSerializedSingularMessage(
    const Message* message,
    const FieldDescriptor* field)  const;

  // Create a serialized repeated message object.
  std::shared_ptr<SerializedObjectInterface>
  CreateSerializedRepeatedMessage(
    const Message* message,
    const FieldDescriptor* field)  const;

  // Create a serialized singular primitive object.
  std::shared_ptr<SerializedObjectInterface>
  CreateSerializedSingularPrimitive(
    const Message* message,
    const FieldDescriptor* field) const;

  // Create a repeated singular primitive object.
  std::shared_ptr<SerializedObjectInterface>
  CreateSerializedRepeatedPrimitive(
    const Message* message,
    const FieldDescriptor* field) const;


  // Deserialize a singular primitive field.
  uint32 DeSerializeSingularPrimitive(
    Message* message,
    const FieldDescriptor* field,
    const char* buf) const;

  // Deserialize a repeated primitive field.
  uint32 DeSerializeRepeatedPrimitive(
    Message* message,
    const FieldDescriptor* field,
    const char* buf) const;

  // Deserialize a singular message field.
  uint32 DeSerializeSingularMessage(
    Message* message,
    const FieldDescriptor* field,
    const char* buf) const;

  // Deserialize a repeated message field.
  uint32 DeSerializeRepeatedMessage(
    Message* message,
    const FieldDescriptor* field,
    const char* buf) const;

 private:
  int FieldOffset(const FieldDescriptor* field) const;

  std::shared_ptr<const ::proto::MessageDescriptor> message_descirptor_;
  const Message* defatult_instance_;
  const int* fields_offset_ = nullptr;
  int has_bits_offset_;

  FORBID_COPY_AND_ASSIGN(MessageReflection);
};

}  // namespace proto

#endif  /* GENERATED_MESSAGE_REFLECTION_ */