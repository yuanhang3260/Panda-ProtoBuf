#include <iostream>
#include "../Utility/Strings.h"
#include "../Utility/Utils.h"
#include "CppCodeGenerator.h"

namespace proto {
namespace ProtoParser {

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
  printer.Print("#include \"Proto/Message.h\"\n");
  printer.Print("#include \"Proto/RepeatedFields.h\"\n\n");

  GenerateProtoPathName();
  printer.Print("void static_init" + proto_path_name_ + "();\n");
  printer.Print(
    "void static_init_default_instances" + proto_path_name_ + "();\n\n");

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

void CppCodeGenerator::GenerateProtoPathName() {
  std::string suffix = proto_file_.substr(0, proto_file_.length() - 6);
  proto_path_name_ = suffix.substr(StringUtils::findFirstMatch(suffix, "/"));
  StringUtils::replaceWith(proto_path_name_, '/', '_');
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
  printer.Print("class " + message->name() + ": public ::proto::Message {\n");

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
  printer.Print("  " + message->name() + "();\n");
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
  printer.Print(
      "  inline ::proto::Message* New() override;  // New()\n",
      msg_match);
  printer.Print("  void Swap(${msg_name}* other);\n\n", msg_match);
  printer.Print("  static const ${msg_name}& default_instance();\n\n",
                msg_match);
}

void CppCodeGenerator::DeclarePrivateFields(Message* message) {
  // has-bits
  int has_bits_len = (message->highest_tag() - 1) / 8 + 1;
  printer.Print("  // has bits\n"
                "  char has_bits_[$];\n",
                std::vector<std::string>{std::to_string(has_bits_len)});

  printer.Print("  // message fields\n");
  for (auto& field: message->fields_list()) {
    std::map<std::string, std::string> matches =
        GetFieldMatchMap(message, field.get());
    // repeated fields are "vector<Bar>* foo" type
    if (field->modifier() == MessageField::REPEATED) {
      if (field->IsPrimitiveType()) {
        matches["type_name"] =
            "::proto::RepeatedField<" + matches["type_name"] + ">";
      }
      else {
        matches["type_name"] =
            "::proto::RepeatedPtrField<" + matches["type_name"] + ">";
      }
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

  printer.Print(
      "\n  // InitAsDefaultInstance()\n"
      "  void InitAsDefaultInstance() override;\n");
  printer.Print("  // default instance\n"
                "  static ${msg_name}* default_instance_;\n\n",
                std::map<std::string, std::string>{
                  {"msg_name", message->name()}
                });

  printer.Print("  friend void ::static_init" + proto_path_name_ + "();\n");
  printer.Print("  friend void ::static_init_default_instances" + proto_path_name_ + "();\n");
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
  printer.Print("#include <memory>\n\n");
  printer.Print("#include \"Compiler/Message.h\"\n");
  printer.Print("#include \"Compiler/ProtoParser.h\"\n");
  printer.Print("#include \"Proto/MessageReflection.h\"\n");
  printer.Print("#include \"Proto/MessageFactory.h\"\n\n");
  printer.Print("#include \"" + filename + ".h\"\n\n");

  // Define message reflection and descriptors.
  DefineStaticMetadata();
  DefineStaticInitDefaultInstances();
  DefineStaticInit();

  // Print class methods.
  printer.Print("\n");
  for (auto& message: messages_list_) {
    DefineClassMethods(message.get());
  }

  CheckoutNameSpace(pkg_stack_, std::vector<std::string>());
  printer.Flush();
}

void CppCodeGenerator::DefineStaticMetadata() {
  printer.Print("namespace {\n\n");
  for (const auto& message: messages_list_) {
    std::map<std::string, std::string> matches {
      {"msg_name", message->name()},
    };
    printer.Print("std::shared_ptr<::proto::ProtoParser::Message> ${msg_name}_descriptor_;\n"
                  "std::shared_ptr<::proto::MessageReflection> ${msg_name}_reflection_;\n",
                  matches);
  }
  printer.Print("\n}  // namepsace\n\n");
}

void CppCodeGenerator::DefineStaticInitDefaultInstances() {
  std::map<std::string, std::string> matches;
  std::string static_init_func = "static_init_default_instances" +
                                 proto_path_name_;
  printer.Print("void " + static_init_func + "() {\n");
  printer.Print("  static bool already_called = false;\n"
                "  if (already_called) return;\n"
                "  already_called = true;\n\n");
  for (const auto& message: messages_list_) {
    matches["msg_name"] = message->name();
    std::string prefix = GetNameSpacePrefix(std::vector<std::string>(),
                                            message->pkg_stack());
    matches["whole_msg_name"] = prefix + message->name();
    printer.Print("  if (${whole_msg_name}::default_instance_ == NULL) {\n"
                  "    ${whole_msg_name}::default_instance_ = new ${whole_msg_name}();\n"
                  "    ${whole_msg_name}::default_instance_->InitAsDefaultInstance();\n"
                  "  }\n",
                  matches);
  }
  printer.Print("}\n\n");
}

void CppCodeGenerator::DefineStaticInit() {
  std::string static_init_func = "static_init" + proto_path_name_;
  printer.Print("void " + static_init_func + "() {\n");
  
  std::map<std::string, std::string> matches{
    {"proto_file_", proto_file_},
    {"proto_path_name", proto_path_name_},
  };

  printer.Print("  ::proto::ProtoParser::ProtoParser parser(\n"
                "      ::proto::ProtoParser::ProtoParser::CPP,\n"
                "      \"${proto_file_}\");\n"
                "  CHECK(parser.ParseProto(),\n"
                "        \"static class initialization for ${proto_file_} failed\");\n"
                "\n",
                matches);
  printer.Print("  static_init_default_instances${proto_path_name}();\n\n"
                "  int i = 0;\n",
                matches);
  int message_index = 0;
  for (const auto& message: messages_list_) {
    matches["msg_name"] = message->name();
    matches["msg_index"] = std::to_string(message_index++);
    matches["number_fields"] = std::to_string(message->fields_list().size());
    printer.Print("  // static init for class ${msg_name}\n"
                  "  static const int ${msg_name}_offsets_[${number_fields}] = {\n",
                  matches);
    std::string prefix = GetNameSpacePrefix(std::vector<std::string>(),
                                            message->pkg_stack());
    matches["whole_msg_name"] = prefix + message->name();
    for (const auto& field: message->fields_list()) {
      matches["field_name"] = field->name();
      printer.Print("    PROTO_MESSAGE_FIELD_OFFSET(${whole_msg_name}, ${field_name}_),\n",
                    matches);
    }
    printer.Print("  };\n");
    printer.Print("  i = 0;\n"
                  "  for (auto& field: parser.mutable_messages_list()[${msg_index}]->mutable_fields_list()) {\n"
                  "    field->set_field_offset(${msg_name}_offsets_[i++]);\n"
                  "  }\n"
                  "  ${msg_name}_descriptor_ = parser.mutable_messages_list()[${msg_index}];\n"
                  "  ${msg_name}_reflection_.reset(\n"
                  "      new ::proto::MessageReflection(\n"
                  "          ${msg_name}_descriptor_,\n"
                  "          ${whole_msg_name}::default_instance_)\n"
                  "  );\n"
                  "  ::proto::MessageFactory::RegisterGeneratedMessage(${msg_name}_reflection_);\n"
                  "\n",
                  matches);
  }

  printer.Print("}\n\n");

  // Define a struct to force static init function to be called at
  // initializtion time.
  matches["static_init_forcer"] = "static_init_forcer" + proto_path_name_;
  matches["static_init_func"] = "static_init" + proto_path_name_;
  matches["static_init_forcer_obj"] = "static_init_forcer" + proto_path_name_ + "_obj";
  printer.Print("// Force ${static_init_func}() to be called at initialization time.\n"
                "struct ${static_init_forcer} {\n"
                "  ${static_init_forcer}() {\n"
                "    ${static_init_func}();\n"
                "  }\n"
                "} ${static_init_forcer_obj}_;\n\n",
                matches);
}

void CppCodeGenerator::DefineClassMethods(Message* message) {
  // Check out name space.
  CheckoutNameSpace(pkg_stack_, message->pkg_stack());
  // Print constructors and destructor.
  printer.Print("// -------------------- " + message->name() + " --------------------- //\n");
  DefineConstructor(message);
  DefineCopyConstructor(message);
  DefineMoveConstructor(message);
  DefineCopyAssigner(message);
  DefineMoveAssigner(message);
  DefineNew(message);
  DefineInitAsDefaultInstance(message);
  DefineSwapper(message);
  DefineGetDefaultInstance(message);
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
    else if (field->IsRepeatedStringType() || field->IsRepeatedMessageType()) {
      printer.Print(
        "  for (const ${type_name}* p: other.${field_name}().GetElements()) {\n"
        "    ${field_name}_.AddAllocated(new ${type_name}(*p));\n"
        "  }\n",
        matches);
    }
    else {
      printer.Print("  ${field_name}_ = other.${field_name}();\n", matches);
    }
  }
}

void CppCodeGenerator::DefineConstructor(Message* message) {
  printer.Print("// constructor\n");
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print(
      "${msg_name}::${msg_name}() {\n"
      "  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {\n"
      "    has_bits_[i] = 0;\n"
      "  }\n"
      "  default_instance_ = nullptr;\n"
      "}\n\n",
      msg_match);
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
  printer.Print("}\n\n");
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
  printer.Print("  return *this;\n"
                "}\n\n");
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
}

void CppCodeGenerator::DefineMoveConstructor(Message* message) {
  printer.Print("// move constructor\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("${msg_name}::${msg_name}(${msg_name}&& other) {\n", msg_match);

  PrintMoveClassCode(message);
  printer.Print("}\n\n");
}

void CppCodeGenerator::DefineMoveAssigner(Message* message) {
  printer.Print("// move assignment\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("${msg_name}& ${msg_name}::operator=(${msg_name}&& other) {\n",
                msg_match);

  PrintMoveClassCode(message);
  printer.Print("  return *this;\n"
                "}\n\n");
}

void CppCodeGenerator::DefineNew(Message* message) {
  printer.Print("// New()\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("::proto::Message* ${msg_name}::New() {\n"
                "  return reinterpret_cast<::proto::Message*>(new ${msg_name}());\n"
                "}\n\n",
                msg_match);
}

void CppCodeGenerator::DefineInitAsDefaultInstance(Message* message) {
  printer.Print("// InitAsDefaultInstance()\n");

  std::map<std::string, std::string> matches{
     {"msg_name", message->name()},
  };
  printer.Print("void ${msg_name}::InitAsDefaultInstance() {\n",
                matches);
  for (const auto& field: message->fields_list()) {
    if (field->IsSingularMessageType()) {
      matches = GetFieldMatchMap(message, field.get());
      printer.Print("  ${field_name}_ = const_cast<${type_name}*>(&${type_name}::default_instance());\n",
                    matches);
    }
  }
  printer.Print("}\n\n");
}

void CppCodeGenerator::DefineGetDefaultInstance(Message* message) {
  printer.Print("// default_instance()\n");

  std::map<std::string, std::string> matches{
     {"msg_name", message->name()},
     {"proto_path_name", proto_path_name_}
  };
  printer.Print("const ${msg_name}& ${msg_name}::default_instance() {\n"
                "  if (default_instance_ == NULL) {\n"
                "    static_init_default_instances${proto_path_name}();\n"
                "  }\n"
                "  return *default_instance_;\n"
                "}\n\n",
                matches);
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
      if (field->IsPrimitiveType()) {
        matches["type_name"] =
            "::proto::RepeatedField<" + matches["type_name"] + ">";
      }
      else {
        matches["type_name"] =
            "::proto::RepeatedPtrField<" + matches["type_name"] + ">";
      }
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

  // Declare - bool has_foo() const;
  printer.Print("  bool has_${field_name}() const;\n", matches);

  // Declare - int32 foo() const;
  printer.Print("  ${type_name} ${field_name}() const;\n", matches);
  
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
  
  // Declare - bool has_foo() const;
  printer.Print("  bool has_${field_name}() const;\n", matches);

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

  // Declare - bool has_foo() const;
  printer.Print("  bool has_${field_name}() const;\n", matches);

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

  // Declare - const ::proto::RepeatedField<Bar>& foo() const;
  printer.Print("  const ::proto::RepeatedField<${type_name}>& ${field_name}() const;\n",
                matches);

  // Declare - ::proto::RepeatedField<Bar>& mutable_foo();
  printer.Print("  ::proto::RepeatedField<${type_name}>& mutable_${field_name}();\n",
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

  // String type can also have value passed by const char*
  if (field->IsRepeatedStringType()) {
    // Declare - void set_foo(int index, const string& value);
    printer.Print("  void set_${field_name}(int index, const std::string& value);\n",
                  matches);

    // Declare - void set_foo(int index, const char* value);
    printer.Print("  void set_${field_name}(int index, const char* value);\n",
                  matches);

    // Declare - void set_foo(int index, const char* value, int size);
    printer.Print("  void set_${field_name}(int index, const char* value, int size);\n",
                  matches);
  }

  // Declare - Bar* add_foo();
  printer.Print("  ${type_name}* add_${field_name}();\n",
                matches);

  // String type can also have value passed by const char*
  if (field->IsRepeatedStringType()) {
    // Declare - void add_foo(const string& value);
    printer.Print("  void add_${field_name}(const std::string& value);\n",
                  matches);

    // Declare - void add_foo(const char* value);
    printer.Print("  void add_${field_name}(const char* value);\n",
                  matches);

    // Declare - void add_foo(const char* value, int size);
    printer.Print("  void add_${field_name}(const char* value, int size);\n",
                  matches);
  }

  // Declare - Bar* mutable_foo(int index);
  printer.Print("  ${type_name}* mutable_${field_name}(int index);\n", matches);  

  // Declare - void clear_foo();
  printer.Print("  void clear_${field_name}();\n", matches);

  // Declare - const std::vector<Bar>& foo() const;
  printer.Print("  const ::proto::RepeatedPtrField<${type_name}>& ${field_name}() const;\n",
                matches);

  // Declare - std::vector<Bar>& mutable_foo();
  printer.Print("  ::proto::RepeatedPtrField<${type_name}>& mutable_${field_name}();\n",
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

  // Implement has_foo()
  printer.Print("inline bool ${msg_name}::has_${field_name}() const {\n"
                "  return (has_bits_[${tag_byte_index}] & ${tag_bit}) != 0;\n"
                "}\n\n",
                matches);

  // Implement - int32 foo() const;
  printer.Print("inline ${type_name} ${msg_name}::${field_name}() const {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);
  
  // Implement - void set_foo(int32 value);
  printer.Print("inline void ${msg_name}::set_${field_name}(${type_name} ${field_name}) {\n"
                "  ${field_name}_ = ${field_name};\n"
                "  has_bits_[${tag_byte_index}] |= ${tag_bit};\n"
                "}\n\n",
                matches);
  
  // Implement - void clear_foo(int32 value);
  printer.Print("inline void ${msg_name}::clear_${field_name}() {\n"
                "  ${field_name}_ = ${default_value};\n"
                "  has_bits_[${tag_byte_index}] &= (~${tag_bit});\n"
                "}\n\n",
                matches);
}

void CppCodeGenerator::DefineSingularStringTypeAccessors(
    Message* message,
    MessageField* field)
{
  std::map<std::string, std::string> matches = GetFieldMatchMap(message, field);

  printer.Print("// \"${field_name}\" = ${tag}\n", matches);

  // Implement has_foo()
  printer.Print("inline bool ${msg_name}::has_${field_name}() const {\n"
                "  return (has_bits_[${tag_byte_index}] & ${tag_bit}) != 0;\n"
                "}\n\n",
                matches);
  
  // Implement - std::string foo() const;
  printer.Print("inline const std::string& ${msg_name}::${field_name}() const {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);
  
  // Implement - void set_foo(std::string value);
  printer.Print("inline void ${msg_name}::set_${field_name}(const std::string& ${field_name}) {\n"
                "  ${field_name}_ = ${field_name};\n"
                "  has_bits_[${tag_byte_index}] |= ${tag_bit};\n"
                "}\n\n",
                matches);

  // Implement - void set_foo(const char* value);
  printer.Print("inline void ${msg_name}::set_${field_name}(const char* ${field_name}) {\n"
                "  ${field_name}_ = std::string(${field_name});\n"
                "  has_bits_[${tag_byte_index}] |= ${tag_bit};\n"
                "}\n\n",
                matches);

  // Implement - void set_foo(const char* value, int size);
  printer.Print("inline void ${msg_name}::set_${field_name}(const char* ${field_name}, int size) {\n"
                "  ${field_name}_ = std::string(${field_name}, size);\n"
                "  has_bits_[${tag_byte_index}] |= ${tag_bit};\n"
                "}\n\n",
                matches);

  // Implement - std::string& mutable_foo();
  printer.Print("inline std::string ${msg_name}::mutable_${field_name}() {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);

  // Implement - void clear_foo();
  printer.Print("inline void ${msg_name}::clear_${field_name}() {\n"
                "  ${field_name}_ = \"\";\n"
                "  has_bits_[${tag_byte_index}] &= (~${tag_bit});\n"
                "}\n\n",
                matches);
}

void CppCodeGenerator::DefineSingularMessageTypeAccessors(
    Message* message,
    MessageField* field)
{
  std::map<std::string, std::string> matches = GetFieldMatchMap(message, field);

  printer.Print("// \"${field_name}\" = ${tag}\n", matches);

  // Implement has_foo()
  printer.Print("inline bool ${msg_name}::has_${field_name}() const {\n"
                "  return (has_bits_[${tag_byte_index}] & ${tag_bit}) != 0;\n"
                "}\n\n",
                matches);
  
  // Implement - const Bar& foo() const;
  printer.Print("inline const ${type_name}& ${msg_name}::${field_name}() const {\n"
                "  return *${field_name}_;\n"
                "}\n\n",
                matches);

  // Implement - Bar* mutable_foo();
  printer.Print("inline ${type_name}* ${msg_name}::mutable_${field_name}() {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);

  // Implement - void set_alocated_foo(Bar* value);
  printer.Print("inline void ${msg_name}::set_allocated_${field_name}(${type_name}* ${field_name}) {\n"
                "  if (${field_name}_) {\n"
                "    delete ${field_name}_;\n"
                "  }\n"
                "  ${field_name}_ = ${field_name};\n"
                "  if (${field_name}_) {\n"
                "    has_bits_[${tag_byte_index}] |= ${tag_bit};\n"
                "  }\n"
                "  else {\n"
                "    has_bits_[${tag_byte_index}] &= (~${tag_bit});\n"
                "  }\n"
                "}\n\n",
                matches);

  // Implement - Bar* release_foo();
  printer.Print("inline ${type_name}* ${msg_name}::release_${field_name}() {\n"
                "  ${type_name}* ${field_name}_tmp__ = ${field_name}_;\n"
                "  ${field_name}_ = nullptr;\n"
                "  has_bits_[${tag_byte_index}] &= (~${tag_bit});\n"
                "  return ${field_name}_tmp__;\n"
                "}\n\n",
                matches);

  // Implement - void clear_foo();
  printer.Print("inline void ${msg_name}::clear_${field_name}() {\n"
                "  if (${field_name}_) {\n"
                "    delete ${field_name}_;\n"
                "  }\n"
                "  ${field_name}_ = nullptr;\n"
                "  has_bits_[${tag_byte_index}] &= (~${tag_bit});\n"
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
  printer.Print("inline int ${msg_name}::${field_name}_size() const {\n"
                "  return ${field_name}_.size();\n"
                "}\n\n",
                matches);

  // Implement - Bar foo(int index) const;
  printer.Print("inline ${type_name} ${msg_name}::${field_name}(int index) {\n"
                "  return ${field_name}_.Get(index);\n"
                "}\n\n",
                matches);

  // Implement - void set_foo(int index, Bar& value);
  printer.Print("inline void ${msg_name}::set_${field_name}(int index, ${type_name} value) {\n"
                "  if ((int)${field_name}_.size() > index) {\n"
                "    ${field_name}_.Set(index, value);\n"
                "  }\n"
                "}\n\n",
                matches);

  // Implement - void add_foo(Bar& value);
  printer.Print("inline void ${msg_name}::add_${field_name}(${type_name} value) {\n"
                "   ${field_name}_.Add(value);\n"
                "}\n\n",
                matches);

  // Implement - void clear_foo();
  printer.Print("inline void ${msg_name}::clear_${field_name}() {\n"
                "  ${field_name}_ .Clear();\n"
                "}\n\n",
                matches);

  // Implement - const ::proto::RepeatedField<Bar>& foo() const;
  printer.Print("inline const ::proto::RepeatedField<${type_name}>& ${msg_name}::${field_name}() const {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);

  // Implement - ::proto::RepeatedField<Bar>& mutable_foo();
  printer.Print("inline ::proto::RepeatedField<${type_name}>& ${msg_name}::mutable_${field_name}() {\n"
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
  printer.Print("inline int ${msg_name}::${field_name}_size() const {\n"
                "  return ${field_name}_.size();\n"
                "}\n\n",
                matches);

  // Implement - const ${type_name}& foo(int index) const;
  printer.Print("inline const ${type_name}& ${msg_name}::${field_name}(int index) {\n"
                "  return ${field_name}_.Get(index);\n"
                "}\n\n",
                matches);

  // String type can also have value passed by const char*
  if (field->IsRepeatedStringType()) {
    // Implement - void set_foo(int index, const char* value);
    printer.Print("inline void ${msg_name}::set_${field_name}(int index, const std::string& value) {\n"
                  "  if (index < (int)${field_name}_.size()) {\n"
                  "    ${field_name}_.Set(index, value);\n"
                  "  }\n"
                  "}\n\n",
                  matches);

    // Implement - void set_foo(int index, const char* value);
    printer.Print("inline void ${msg_name}::set_${field_name}(int index, const char* value) {\n"
                  "  if (index < (int)${field_name}_.size()) {\n"
                  "    ${field_name}_.Set(index, std::string(value));\n"
                  "  }\n"
                  "}\n\n",
                  matches);

    // Implement - void set_foo(int index, const char* value, int size);
    printer.Print("inline void ${msg_name}::set_${field_name}(int index, const char* value, int size) {\n"
                  "  if (index < (int)${field_name}_.size()) {\n"
                  "    ${field_name}_.Set(index, std::string(value, size));\n"
                  "  }\n"
                  "}\n\n",
                  matches);
  }

  // Implement - Bar* add_foo();
  printer.Print("inline ${type_name}* ${msg_name}::add_${field_name}() {\n"
                "  return ${field_name}_.Add();\n"
                "}\n\n",
                matches);

  // String type can also have value passed by const char*
  if (field->IsRepeatedStringType()) {
    // Implement - void add_foo(const std::string value);
    printer.Print("inline void ${msg_name}::add_${field_name}(const std::string& value) {\n"
                  "  ${field_name}_.AddAllocated(new std::string(value));\n"
                  "}\n\n",
                  matches);

    // Implement - void add_foo(const char* value);
    printer.Print("inline void ${msg_name}::add_${field_name}(const char* value) {\n"
                  "  ${field_name}_.AddAllocated(new std::string(value));\n"
                  "}\n\n",
                  matches);

    // Implement - void add_foo(const char* value, int size);
    printer.Print("inline void ${msg_name}::add_${field_name}(const char* value, int size) {\n"
                  "  ${field_name}_.AddAllocated(new std::string(value, size));\n"
                  "}\n\n",
                  matches);
  }

  // Implement - Bar* mutable_foo(int index);
  printer.Print("inline ${type_name}* ${msg_name}::mutable_${field_name}(int index) {\n"
                "  return ${field_name}_.GetMutable(index);\n"
                "}\n\n",
                matches);  

  // Implement - void clear_foo();
  printer.Print("inline void ${msg_name}::clear_${field_name}() {\n"
                "  ${field_name}_.Clear();\n"
                "}\n\n",
                matches);

  // Implement - const ::proto::RepeatedPtrField<Bar>& foo() const;
  printer.Print("inline const ::proto::RepeatedPtrField<${type_name}>& ${msg_name}::${field_name}() const {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);

  // Implement - ::proto::RepeatedPtrField<Bar>& mutable_foo();
  printer.Print("inline ::proto::RepeatedPtrField<${type_name}>& ${msg_name}::mutable_${field_name}() {\n"
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
    {"tag_byte_index", std::to_string(field->tag()/8)},
    {"tag_bit", StringUtils::IntToHexString(1<<(field->tag()%8))},
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
    // Start from global namespace.
    prefix = "::";
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
