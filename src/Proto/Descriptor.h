#ifndef PROTO_DESCRIPTOR_
#define PROTO_DESCRIPTOR_

#include <string>
//#include "EnumType.h"

namespace proto {

class FieldDescriptor {
 public:
  FieldDescriptor(FieldLabel modifier, FieldType type, /* PbType* type_class , */
                  std::string name, int tag, std::string default_value);
  virtual ~FieldDescriptor();

  static FieldLabel GetMessageFieldModifier(std::string modifier);
  static std::string GetModifierAsString(FieldLabel modifier);

  FieldLabel modifier() const { return modifier_; }
  FieldType type() const { return type_; }
  //const PbType* type_class() const { return type_class_; }
  std::string name() const { return name_; }
  unsigned int tag() const { return tag_; }
  std::string default_value() const { return default_value_; }
  std::string type_name() const { return type_name_; }
  int field_offset() const { return field_offset_; }
  bool has_user_default_value() const { return has_user_default_value_; }

  void set_field_offset(const int offset) { field_offset_ = offset; }
  void set_type_name(std::string type_name) { type_name_ = type_name; }

  bool IsPrimitiveType() const;
  bool IsMessageType() const;
  bool IsSingularType() const;
  bool IsRepeatedType() const;
  bool IsSingularNumericType() const;
  bool IsSingularStringType() const;
  bool IsSingularMessageType() const;
  bool IsRepeatedNumericType() const;
  bool IsRepeatedStringType() const;
  bool IsRepeatedMessageType() const;

 private:
  FieldLabel modifier_;
  FieldType type_;
  //const PbType* type_class_;
  std::string name_;
  unsigned int tag_;
  std::string default_value_;
  std::string type_name_;
  bool has_user_default_value_ = false;
  int field_offset_ = -1;
};

}

#endif  /* PROTO_DESCRIPTOR_ */
