#include <iostream>
#include "../Utility/Strings.h"
#include "../Utility/Utils.h"
#include "CppCodeGenerator.h"

namespace PandaProto {
namespace Compiler {

std::map<FIELD_TYPE, std::string> pbCppTypeMap{
  {INT32, "int"},
  {INT64, "long long"},
  {UINT32, "unsigned int"},
  {UINT64, "unsigned long long"},
  {DOUBLE, "double"},
  {STRING, "std::string"},
  {BOOL, "bool"},
};

void CppCodeGenerator::GenerateCode() {
  GenerateHeader();
  GenerateCC();
}

void CppCodeGenerator::GenerateHeader() {
  std::string outfile;
  outfile = proto_file_.substr(0, proto_file_.length() - 6) + "_pb";

  // Open .h file
  if (!printer.Open(outfile + ".h")) {
    fprintf(stderr, "ERROR: Open output file %s.h failed\n", outfile.c_str());
  }

  std::vector<std::string> result = StringUtils::Split(outfile, '/');
  std::string filename = result[result.size() - 1];

  printer.Print("#ifndef " + StringUtils::Upper(filename) + "_H_\n");
  printer.Print("#define " + StringUtils::Upper(filename) + "_H_\n\n");
  printer.Print("#include <string>\n");
  printer.Print("#include <vector>\n\n");

  // Declare global enums.
  for (auto& e: enums_map_) {
    DeclareGlobalEnum(e.second.get());
  }

  // Declare classes.
  for (auto& message: messages_list_) {
    DeclareMessageClass(message.get());
  }

  CheckoutNameSpace(pkg_stack_, std::vector<std::string>());
  printer.Print("\n#endif  /* " + StringUtils::Upper(filename) + "_H_ */\n");
  printer.Flush();
}

void CppCodeGenerator::DeclareGlobalEnum(EnumType* enum_p) {
  CheckoutNameSpace(pkg_stack_, enum_p->pkg_stack());
  printer.Print("enum " + enum_p->name() + " {\n");
  for (auto& enumvalue: enum_p->enums()) {
    printer.Print("  " + enumvalue + ",\n");
  }
  printer.Print("};\n\n");
}

void CppCodeGenerator::DeclareMessageClass(Message* message) {
  CheckoutNameSpace(pkg_stack_, message->pkg_stack());
  printer.Print("class " + message->name() + " {\n");

  // Public:
  printer.Print(" public:\n");

  // Declare nested enums.
  DeclareNestedEnums(message);

  // Declare con/destructor, assigner and swapper.
  DeclarePrimitiveMethods(message);

  // Declare public accessors.
  printer.Print("  // --- Field accessors --- //\n\n");
  for (auto& field: message->fields_list()) {
    DeclareAccessors(message, field.get());
  }

  // Private:
  printer.Print(" private:\n");

  // Declare private fields.
  DeclarePrivateFields(message);
  printer.Print("};\n\n");
}

void CppCodeGenerator::DeclareNestedEnums(Message* message) {
  for (auto& e: message->enums_map()) {
    EnumType* enum_p = e.second.get();
    printer.Print("  enum " + enum_p->name() + " {\n");
    for (auto& enumvalue: enum_p->enums()) {
      printer.Print("    " + enumvalue + ",\n");
    }
    printer.Print("  };\n\n");
  }
}

void CppCodeGenerator::DeclarePrimitiveMethods(Message* message) {
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("  // constructors and destructor //\n");
  printer.Print("  " + message->name() + "() = default;\n");
  printer.Print("  ~" + message->name() + "();\n");
  printer.Print(
      "  ${msg_name}(const ${msg_name}& other);  // copy constructor\n",
      msg_match);
  printer.Print(
      "  ${msg_name}(${msg_name}&& other);  // move constructor\n",
      msg_match);
  printer.Print(
      "  ${msg_name}& operator=(const ${msg_name}& other);"
      "  // copy assignment\n",
      msg_match);
  printer.Print(
      "  ${msg_name}& operator=(${msg_name}&& other);  // move assignment\n",
      msg_match);
  printer.Print("  void Swap(${msg_name}* other);\n", msg_match);
  printer.Print("\n");
}

void CppCodeGenerator::DeclarePrivateFields(Message* message) {
  for (auto& field: message->fields_list()) {
    std::map<std::string, std::string> matches =
        GetFieldMatchMap(message, field.get());
    // repeated fields are "vector<Bar>* foo" type
    if (field->modifier() == MessageField::REPEATED) {
      matches["type_name"] = "std::vector<" + matches["type_name"] + ">";
    }

    std::string declearation_line = "  ${type_name} ${field_name}_";
    if (field->IsSingularMessageType()) {
      declearation_line = "  ${type_name}* ${field_name}_";
    }
    printer.Print(declearation_line, matches);
    // add default value if exists.
    if (field->has_user_default_value()) {
      printer.Print(" = ${default_value}", matches);
    }
    printer.Print(";\n");
  }
}

void CppCodeGenerator::GenerateCC() {
  std::string outfile;
  outfile = proto_file_.substr(0, proto_file_.length() - 6) + "_pb";

  // Open .cpp file
  if (!printer.Open(outfile + ".cpp")) {
    fprintf(stderr, "ERROR: Open output file %s.p failed\n", outfile.c_str());
  }

  // Include proto_name.pb.h file
  std::vector<std::string> result = StringUtils::Split(outfile, '/');
  std::string filename = result[result.size() - 1];
  printer.Print("#include \"" + filename + ".h\"\n\n");

  // Print class methods.
  for (auto& message: messages_list_) {
    DefineClassMethods(message.get());
  }

  CheckoutNameSpace(pkg_stack_, std::vector<std::string>());
  printer.Flush();
}

void CppCodeGenerator::DefineClassMethods(Message* message) {
  // Check out name space.
  CheckoutNameSpace(pkg_stack_, message->pkg_stack());
  // Print constructors and destructor.
  DefineCopyConstructor(message);
  DefineMoveConstructor(message);
  DefineCopyAssigner(message);
  DefineMoveAssigner(message);
  DefineSwapper(message);
  DefineDestructor(message);
  // Print accessors.
  for (auto& field : message->fields_list()) {
    DefineAccessors(message, field.get());
  }
}

void CppCodeGenerator::DefineDestructor(Message* message) {
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };

  printer.Print("// destructor\n");
  printer.Print("${msg_name}::~${msg_name}() {\n", msg_match);
  if (message->has_message_field()) {
    for (auto& field : message->fields_list()) {
      if (field->type() == MESSAGETYPE &&
          field->modifier() != MessageField::REPEATED) {
        printer.Print("  delete " + field->name() + "_;\n");
      }
    }
  }
  printer.Print("}\n\n");
}

void CppCodeGenerator::PrintCopyClassCode(Message* message) {
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };

  for (auto& field : message->fields_list()) {
    std::map<std::string, std::string> matches =
        GetFieldMatchMap(message, field.get());

    if (field->IsSingularMessageType()) {
      printer.Print(
        "  if (!${field_name}_) {\n"
        "    ${field_name}_ = new ${type_name}();\n"
        "  }\n"
        "  *${field_name}_ = other.${field_name}();\n", matches);
    }
    else {
      printer.Print("  ${field_name}_ = other.${field_name}();\n", matches);
    }
  }
  printer.Print("}\n\n");
}

void CppCodeGenerator::DefineCopyConstructor(Message* message) {
  printer.Print("// copy constructor\n");
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print(
      "${msg_name}::${msg_name}(const ${msg_name}& other) {\n",
      msg_match);
  
  PrintCopyClassCode(message);
}

void CppCodeGenerator::DefineCopyAssigner(Message* message) {
  printer.Print("// copy assignment\n");
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print(
      "${msg_name}& ${msg_name}::operator=(const ${msg_name}& other) {\n",
      msg_match);
  
  PrintCopyClassCode(message);
}

void CppCodeGenerator::PrintMoveClassCode(Message* message) {
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };

  for (auto& field : message->fields_list()) {
    std::map<std::string, std::string> matches =
        GetFieldMatchMap(message, field.get());

    if (field->modifier() == MessageField::REPEATED ||
        field->type() == STRING) {
      printer.Print(
        "  ${field_name}_ = std::move(other.mutable_${field_name}());\n",
        matches);
    }
    else if (field->type() == MESSAGETYPE) {
      printer.Print(
        "  if (${field_name}_ ) {\n"
        "    delete ${field_name}_;\n"
        "  }\n"
        "  ${field_name}_ = other.release_${field_name}();\n",
        matches);
    }
    else {
      printer.Print("  ${field_name}_ = other.${field_name}();\n", matches);
      printer.Print("  other.clear_${field_name}();\n", matches);
    }
  }
  printer.Print("}\n\n");
}

void CppCodeGenerator::DefineMoveConstructor(Message* message) {
  printer.Print("// move constructor\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("${msg_name}::${msg_name}(${msg_name}&& other) {\n", msg_match);

  PrintMoveClassCode(message);
}

void CppCodeGenerator::DefineMoveAssigner(Message* message) {
  printer.Print("// move assignment\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("${msg_name}& ${msg_name}::operator=(${msg_name}&& other) {\n",
                msg_match);

  PrintMoveClassCode(message);
}

void CppCodeGenerator::DefineSwapper(Message* message) {
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };

  printer.Print("// swapper\n");
  printer.Print("void ${msg_name}::Swap(${msg_name}* other) {", msg_match);
  for (auto& field : message->fields_list()) {
    printer.Print("\n");

    std::map<std::string, std::string> matches =
        GetFieldMatchMap(message, field.get());
    // repeated field should have std::vector<Bar> type
    if (field->modifier() == MessageField::REPEATED) {
      matches["type_name"] = "std::vector<" + matches["type_name"] + ">";
    }

    if (field->modifier() == MessageField::REPEATED ||
        field->type() == STRING) {  // repeated type / string type
      printer.Print(
          "  ${type_name} ${field_name}_tmp__ = std::move(other->mutable_${field_name}());\n"
          "  other->mutable_${field_name}() = std::move(${field_name}_);\n"
          "  ${field_name}_ = std::move(${field_name}_tmp__);\n",
          matches);
    }
    else if (field->type() == MESSAGETYPE) {  // message type
      printer.Print(
          "  ${type_name}* ${field_name}_tmp__ = other->release_${field_name}();\n"
          "  other->set_allocated_${field_name}(this->release_${field_name}());\n"
          "  set_allocated_${field_name}(${field_name}_tmp__);\n",
          matches);
    }
    else {  // regular primitive type / enum type
      printer.Print(
          "  ${type_name} ${field_name}_tmp__ = other->${field_name}();\n"
          "  other->set_${field_name}(${field_name}_);\n"
          "  set_${field_name}(${field_name}_tmp__);\n",
          matches);
    }
  }
  printer.Print("}\n\n");
}

void CppCodeGenerator::DeclareAccessors(Message* message, MessageField* field) {
  if (field->IsSingularNumericType()) {
    DeclareSingularNumericTypeAccessors(message, field);
  }
  else if (field->IsSingularStringType()) {
    DeclareSingularStringTypeAccessors(message, field);
  }
  else if (field->IsSingularMessageType()) {
    DeclareSingularMessageTypeAccessors(message, field);
  }
  else if (field->IsRepeatedNumericType()) {
    DeclareRepeatedNumericTypeAccessors(message, field);
  }
  else if (field->IsRepeatedStringType() || field->IsRepeatedMessageType()) {
    DeclareRepeatedNonNumericTypeAccessors(message, field);
  }
  else {
    // hehe
  }
}

void CppCodeGenerator::DeclareSingularNumericTypeAccessors(
    Message* message,
    MessageField* field)
{
  std::map<std::string, std::string> matches = GetFieldMatchMap(message, field);

  printer.Print("  // \"${field_name}\" = ${tag}\n", matches);

  // Declare - int32 foo() const;
  printer.Print("  ${type_name} ${field_name}() const;\n",
                matches);
  
  // Declare - void set_foo(int32 value);
  printer.Print("  void set_${field_name}(${type_name} ${field_name});\n",
                matches);
  
  // Declare - void clear_foo(int32 value);
  printer.Print("  void clear_${field_name}();\n",
                matches);

  printer.Print("\n");
}

void CppCodeGenerator::DeclareSingularStringTypeAccessors(
    Message* message,
    MessageField* field)
{
  std::map<std::string, std::string> matches = GetFieldMatchMap(message, field);

  printer.Print("  // \"${field_name}\" = ${tag}\n", matches);
  
  // Declare - std::string foo() const;
  printer.Print("  const std::string& ${field_name}() const;\n",
                matches);
  
  // Declare - void set_foo(std::string value);
  printer.Print("  void set_${field_name}(const std::string& ${field_name});\n",
                matches);

  // Declare - void set_foo(const char* value);
  printer.Print("  void set_${field_name}(const char* ${field_name});\n",
                matches);

  // Declare - void set_foo(const char* value, int size);
  printer.Print("  void set_${field_name}(const char* ${field_name}, int size);\n",
                matches);

  // Declare - std::string& mutable_foo();
  printer.Print("  std::string mutable_${field_name}();\n",
                matches);

  // Declare - void clear_foo();
  printer.Print("  void clear_${field_name}();\n", matches);

  printer.Print("\n");
}

void CppCodeGenerator::DeclareSingularMessageTypeAccessors(
    Message* message,
    MessageField* field)
{
  std::map<std::string, std::string> matches = GetFieldMatchMap(message, field);

  printer.Print("  // \"${field_name}\" = ${tag}\n", matches);

  // Declare - const Bar& foo() const;
  printer.Print("  const ${type_name}& ${field_name}() const;\n",
                matches);

  // Declare - Bar* mutable_foo();
  printer.Print("  ${type_name}* mutable_${field_name}();\n",
                matches);

  // Declare - void set_alocated_foo(Bar* value);
  printer.Print("  void set_allocated_${field_name}(${type_name}* ${field_name});\n",
                matches);

  // Declare - Bar* release_foo();
  printer.Print("  ${type_name}* release_${field_name}();\n",
                matches);

  // Declare - void clear_foo();
  printer.Print("  void clear_${field_name}();\n",
                matches);

  printer.Print("\n");
}

void CppCodeGenerator::DeclareRepeatedNumericTypeAccessors(
    Message* message,
    MessageField* field)
{
  std::map<std::string, std::string> matches = GetFieldMatchMap(message, field);

  printer.Print("  // \"${field_name}\" = ${tag}\n", matches);

  // Declare - int foo_size() const;
  printer.Print("  int ${field_name}_size() const;\n",
                matches);

  // Declare - Bar foo(int index) const;
  printer.Print("  ${type_name} ${field_name}(int index);\n",
                matches);

  // Declare - void set_foo(int index, Bar& value);
  printer.Print("  void set_${field_name}(int index, ${type_name} value);\n",
                matches);

  // Declare - void add_foo(Bar& value);
  printer.Print("  void add_${field_name}(${type_name} value);\n",
                matches);

  // Declare - void clear_foo();
  printer.Print("  void clear_${field_name}();\n",
                matches);

  // Declare - const std::vector<Bar>& foo() const;
  printer.Print("  const std::vector<${type_name}> ${field_name}() const;\n",
                matches);

  // Declare - std::vector<Bar>& mutable_foo();
  printer.Print("  std::vector<${type_name}> mutable_${field_name}();\n",
                matches);

  printer.Print("\n");
}

void CppCodeGenerator::DeclareRepeatedNonNumericTypeAccessors(
    Message* message,
    MessageField* field)
{
  std::map<std::string, std::string> matches = GetFieldMatchMap(message, field);

  printer.Print("  // \"${field_name}\" = ${tag}\n", matches);

  // Declare - int foo_size() const;
  printer.Print("  int ${field_name}_size() const;\n", matches);

  // Declare - const ${type_name}& foo(int index) const;
  printer.Print("  const ${type_name}& ${field_name}(int index);\n", matches);

  // Declare - void set_foo(int index, const Bar& value);
  printer.Print("  void set_${field_name}(int index, const ${type_name}& value);\n",
                matches);

  // String type can also have value passed by const char*
  if (field->IsRepeatedStringType()) {
    // Declare - void set_foo(int index, const char* value);
    printer.Print("  void set_${field_name}(int index, const char* value);\n",
                  matches);

    // Declare - void set_foo(int index, const char* value, int size);
    printer.Print("  void set_${field_name}(int index, const char* value, int size);\n",
                  matches);
  }

  // Declare - void add_foo(Bar& value);
  printer.Print("  void add_${field_name}(const ${type_name}& value);\n",
                matches);

  // String type can also have value passed by const char*
  if (field->IsRepeatedStringType()) {
    // Declare - void add_foo(const char* value);
    printer.Print("  void add_${field_name}(const char* value);\n",
                  matches);

    // Declare - void add_foo(const char* value, int size);
    printer.Print("  void add_${field_name}(const char* value, int size);\n",
                  matches);
  }

  // Declare - Bar& mutable_foo(int index);
  printer.Print("  ${type_name}& mutable_${field_name}(int index);\n", matches);  

  // Declare - void clear_foo();
  printer.Print("  void clear_${field_name}();\n", matches);

  // Declare - const std::vector<Bar>& foo() const;
  printer.Print("  const std::vector<${type_name}> ${field_name}() const;\n",
                matches);

  // Declare - std::vector<Bar>& mutable_foo();
  printer.Print("  std::vector<${type_name}> mutable_${field_name}();\n",
                matches);

  printer.Print("\n");
}

void CppCodeGenerator::DefineAccessors(Message* message, MessageField* field) {
  if (field->IsSingularNumericType()) {
    DefineSingularNumericTypeAccessors(message, field);
  }
  else if (field->IsSingularStringType()) {
    DefineSingularStringTypeAccessors(message, field);
  }
  else if (field->IsSingularMessageType()) {
    DefineSingularMessageTypeAccessors(message, field);
  }
  else if (field->IsRepeatedNumericType()) {
    DefineRepeatedNumericTypeAccessors(message, field);
  }
  else if (field->IsRepeatedStringType() || field->IsRepeatedMessageType()) {
    DefineRepeatedNonNumericTypeAccessors(message, field);
  }
  else {
    // hehe
  }
}

void CppCodeGenerator::DefineSingularNumericTypeAccessors(
    Message* message,
    MessageField* field)
{
  std::map<std::string, std::string> matches = GetFieldMatchMap(message, field);

  printer.Print("// \"${field_name}\" = ${tag}\n", matches);

  // TODO: implement has_foo()
  // printer.Print("bool ${msg_name}::has_${field_name}() {\n");
  
  // Implement - int32 foo() const;
  printer.Print("${type_name} ${msg_name}::${field_name}() const {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);
  
  // Implement - void set_foo(int32 value);
  printer.Print("void ${msg_name}::set_${field_name}(${type_name} ${field_name}) {\n"
                "  ${field_name}_ = ${field_name};\n"
                // TODO: set_foo() requires setting has-bits.
                "}\n\n",
                matches);
  
  // Implement - void clear_foo(int32 value);
  printer.Print("void ${msg_name}::clear_${field_name}() {\n"
                "  ${field_name}_ = ${default_value};\n"
                // TODO: clear_foo() requires setting has-bits.
                "}\n\n",
                matches);
}

void CppCodeGenerator::DefineSingularStringTypeAccessors(
    Message* message,
    MessageField* field)
{
  std::map<std::string, std::string> matches = GetFieldMatchMap(message, field);

  printer.Print("// \"${field_name}\" = ${tag}\n", matches);

  // TODO: implement has_foo()
  // printer.Print("bool ${msg_name}::has_${field_name}() {\n");
  
  // Implement - std::string foo() const;
  printer.Print("const std::string& ${msg_name}::${field_name}() const {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);
  
  // Implement - void set_foo(std::string value);
  printer.Print("void ${msg_name}::set_${field_name}(const std::string& ${field_name}) {\n"
                "  ${field_name}_ = ${field_name};\n"
                // TODO: set_foo() requires setting has-bits.
                "}\n\n",
                matches);

  // Implement - void set_foo(const char* value);
  printer.Print("void ${msg_name}::set_${field_name}(const char* ${field_name}) {\n"
                "  ${field_name}_ = std::string(${field_name});\n"
                // TODO: set_foo() requires setting has-bits.
                "}\n\n",
                matches);

  // Implement - void set_foo(const char* value, int size);
  printer.Print("void ${msg_name}::set_${field_name}(const char* ${field_name}, int size) {\n"
                "  ${field_name}_ = std::string(${field_name}, size);\n"
                // TODO: set_foo() requires setting has-bits.
                "}\n\n",
                matches);

  // Implement - std::string& mutable_foo();
  printer.Print("std::string ${msg_name}::mutable_${field_name}() {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);

  // Implement - void clear_foo();
  printer.Print("void ${msg_name}::clear_${field_name}() {\n"
                "  ${field_name}_ = \"\";\n"
                // TODO: need reset has-bits.
                "}\n\n",
                matches);
}

void CppCodeGenerator::DefineSingularMessageTypeAccessors(
    Message* message,
    MessageField* field)
{
  std::map<std::string, std::string> matches = GetFieldMatchMap(message, field);

  printer.Print("// \"${field_name}\" = ${tag}\n", matches);

  // TODO: implement has_foo()
  // printer.Print("bool ${msg_name}::has_${field_name}() {\n");
  
  // Implement - const Bar& foo() const;
  printer.Print("const ${type_name}& ${msg_name}::${field_name}() const {\n"
                "  return *${field_name}_;\n"
                "}\n\n",
                matches);

  // Implement - Bar* mutable_foo();
  printer.Print("${type_name}* ${msg_name}::mutable_${field_name}() {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);

  // Implement - void set_alocated_foo(Bar* value);
  printer.Print("void ${msg_name}::set_allocated_${field_name}(${type_name}* ${field_name}) {\n"
                "  ${field_name}_ = ${field_name};\n"
                // TODO: set_alocated_foo() requires setting has-bits.
                "}\n\n",
                matches);

  // Implement - Bar* release_foo();
  printer.Print("${type_name}* ${msg_name}::release_${field_name}() {\n"
                "  ${type_name}* ${field_name}_tmp__ = ${field_name}_;\n"
                "  ${field_name}_ = nullptr;\n"
                // TODO: release_foo() requires setting has-bits.
                "  return ${field_name}_tmp__;\n"
                "}\n\n",
                matches);

  // Implement - void clear_foo();
  printer.Print("void ${msg_name}::clear_${field_name}() {\n"
                "  if (${field_name}_) {\n"
                "    delete ${field_name}_;\n"
                "  }\n"
                "  ${field_name}_ = nullptr;\n"
                // TODO: clear_foo() requires setting has-bits.
                "}\n\n",
                matches);
}

void CppCodeGenerator::DefineRepeatedNumericTypeAccessors(
    Message* message,
    MessageField* field)
{
  std::map<std::string, std::string> matches = GetFieldMatchMap(message, field);

  printer.Print("// \"${field_name}\" = ${tag}\n", matches);

  // Implement - int foo_size() const;
  printer.Print("int ${msg_name}::${field_name}_size() const {\n"
                "  return ${field_name}_.size();\n"
                "}\n\n",
                matches);

  // Implement - Bar foo(int index) const;
  printer.Print("${type_name} ${msg_name}::${field_name}(int index) {\n"
                "  return ${field_name}_[index];\n"
                "}\n\n",
                matches);

  // Implement - void set_foo(int index, Bar& value);
  printer.Print("void ${msg_name}::set_${field_name}(int index, ${type_name} value) {\n"
                "  if (${field_name}_.size() > index) {\n"
                "    ${field_name}_[index] = value;\n"
                "  }\n"
                "}\n\n",
                matches);

  // Implement - void add_foo(Bar& value);
  printer.Print("void ${msg_name}::add_${field_name}(${type_name} value) {\n"
                "    ${field_name}_.push_back(value);\n"
                "}\n\n",
                matches);

  // Implement - void clear_foo();
  printer.Print("void ${msg_name}::clear_${field_name}() {\n"
                "  ${field_name}_ .clear();\n"
                "}\n\n",
                matches);

  // Implement - const std::vector<Bar>& foo() const;
  printer.Print("const std::vector<${type_name}> ${msg_name}::${field_name}() const {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);

  // Implement - std::vector<Bar>& mutable_foo();
  printer.Print("std::vector<${type_name}> ${msg_name}::mutable_${field_name}() {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);
}

void CppCodeGenerator::DefineRepeatedNonNumericTypeAccessors(
    Message* message,
    MessageField* field)
{
  std::map<std::string, std::string> matches = GetFieldMatchMap(message, field);

  printer.Print("// \"${field_name}\" = ${tag}\n", matches);

  // Implement - int foo_size() const;
  printer.Print("int ${msg_name}::${field_name}_size() const {\n"
                "  return ${field_name}_.size();\n"
                "}\n\n",
                matches);

  // Implement - const ${type_name}& foo(int index) const;
  printer.Print("const ${type_name}& ${msg_name}::${field_name}(int index) {\n"
                "  return ${field_name}_[index];\n"
                "}\n\n",
                matches);

  // Implement - void set_foo(int index, const Bar& value);
  printer.Print("void ${msg_name}::set_${field_name}(int index, const ${type_name}& value) {\n"
                "  if (index < ${field_name}_.size()) {\n"
                "    ${field_name}_[index] = value;\n"
                "  }\n"
                "}\n\n",
                matches);

  // String type can also have value passed by const char*
  if (field->IsRepeatedStringType()) {
    // Implement - void set_foo(int index, const char* value);
    printer.Print("void ${msg_name}::set_${field_name}(int index, const char* value) {\n"
                  "  if (index < ${field_name}_.size()) {\n"
                  "    ${field_name}_[index] = std::string(value);\n"
                  "  }\n"
                  "}\n\n",
                  matches);

    // Implement - void set_foo(int index, const char* value, int size);
    printer.Print("void ${msg_name}::set_${field_name}(int index, const char* value, int size) {\n"
                  "  if (index < ${field_name}_.size()) {\n"
                  "    ${field_name}_[index] = std::string(value, size);\n"
                  "  }\n"
                  "}\n\n",
                  matches);
  }

  // Implement - void add_foo(Bar& value);
  printer.Print("void ${msg_name}::add_${field_name}(const ${type_name}& value) {\n"
                "    ${field_name}_.push_back(value);\n"
                "}\n\n",
                matches);

  // String type can also have value passed by const char*
  if (field->IsRepeatedStringType()) {
    // Implement - void add_foo(const char* value);
    printer.Print("void ${msg_name}::add_${field_name}(const char* value) {\n"
                  "    ${field_name}_.push_back(std::string(value));\n"
                  "}\n\n",
                  matches);

    // Implement - void add_foo(const char* value, int size);
    printer.Print("void ${msg_name}::add_${field_name}(const char* value, int size) {\n"
                  "    ${field_name}_.push_back(std::string(value, size));\n"
                  "}\n\n",
                  matches);
  }

  // Implement - Bar& mutable_foo(int index);
  printer.Print("${type_name}& ${msg_name}::mutable_${field_name}(int index) {\n"
                "  return ${field_name}_[index];\n"
                "}\n\n",
                matches);  

  // Implement - void clear_foo();
  printer.Print("void ${msg_name}::clear_${field_name}() {\n"
                "  ${field_name}_.clear();\n"
                "}\n\n",
                matches);

  // Implement - const std::vector<Bar>& foo() const;
  printer.Print("const std::vector<${type_name}> ${msg_name}::${field_name}() const {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);

  // Implement - std::vector<Bar>& mutable_foo();
  printer.Print("std::vector<${type_name}> ${msg_name}::mutable_${field_name}() {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);
}

std::map<std::string, std::string>
CppCodeGenerator::GetFieldMatchMap(Message* message, MessageField* field) {
  std::string field_name = field->name();
  std::string type_name;
  std::string default_value = field->default_value();
  if (field->type() != ENUMTYPE &&
      field->type() != MESSAGETYPE) {
    type_name = pbCppTypeMap.at(field->type());
  }
  else {
    // Add namespace prefix for enum and message types.
    std::string prefix = GetNameSpacePrefix(message->pkg_stack(),
                                            field->type_class()->pkg_stack());
    type_name = prefix + field->type_name();
    if (field->type() != MESSAGETYPE) {
      // Message type default value should always be "nullptr".
      default_value = prefix + default_value;
    }
  }
  std::map<std::string, std::string> matches {
    {"msg_name", message->name()},
    {"field_name", field_name},
    {"type_name", type_name},
    {"default_value", default_value},
    {"tag", std::to_string(field->tag())},
  };
  return matches;
}

void CppCodeGenerator::CheckoutNameSpace(
    std::vector<std::string>& context_stk,
    const std::vector<std::string>& target_stk) {
  int len = Utils::Min(context_stk.size(), target_stk.size());
  int i, index = len;
  for (i = 0; i < len; i++) {
    if (context_stk[i] != target_stk[i]) {
      index = i;
      break;
    }
  }
  bool printed = false;
  for (i = context_stk.size() - 1; i >= index; i--) {
    printer.Print("}  // namespace " + context_stk[i] + "\n");
    printed = true;
  }
  context_stk.resize(index);
  if (printed) {
    printer.Print("\n");
  }
  
  printed = false;
  for (i = index; i < (int)target_stk.size(); i++) {
    printer.Print("namespace " + target_stk[i] + " {\n");
    context_stk.push_back(target_stk[i]);
    printed = true;
  }
  if (printed) {
    printer.Print("\n");
  }
}

std::string CppCodeGenerator::GetNameSpacePrefix(
    const std::vector<std::string>& context_stk, // AA.DD
    const std::vector<std::string>& target_stk) { // AA
  int len = Utils::Min(context_stk.size(), target_stk.size());
  int i, index = len;
  for (i = 0; i < len; i++) {
    if (context_stk[i] != target_stk[i]) {
      index = i;
      break;
    }
  }

  // std::cout << "index = " << index << ", " << context_stk.size()
  //           << ", " << target_stk.size() << std::endl;
  // for (auto& s: context_stk) {
  //   std::cout << s << ".";
  // }
  // std::cout << std::endl;
  // for (auto& s: target_stk) {
  //   std::cout << s << ".";
  // }
  // std::cout << std::endl;

  std::string prefix = "";
  if (index < (int)context_stk.size()) {
    for (i = 0; i < (int)target_stk.size(); i++) {
      prefix += (target_stk[i] + "::");
    }
  }
  else {
    for (i = context_stk.size(); i < (int)target_stk.size(); i++) {
      prefix += (target_stk[i] + "::");
    }
  }
  std::cout << "prefix = " << prefix << std::endl;
  return prefix;
}

}  // Compiler
}  // PandaProto
