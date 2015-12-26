#include <iostream>
#include "../Utility/Strings.h"
#include "../Utility/Utils.h"
#include "../IO/FileUtils.h"
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
  pkg_stack_.clear();
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
  printer.Print("#include \"Proto/RepeatedFields.h\"\n");
  printer.Print("#include \"Proto/SerializedMessage.h\"\n");
  if (!services_map_.empty()) {
    printer.Print("#include \"RPC/Rpc.h\"\n");
    printer.Print("#include \"RPC/RpcService.h\"\n");
    printer.Print("#include \"RPC/RpcServer.h\"\n");
    printer.Print("#include \"RPC/RpcClientChannel.h\"\n");
    printer.Print("#include \"RPC/RpcCommon.h\"\n");
    printer.Print("#include \"Utility/CallBack.h\"\n");
  }
  printer.Print("\n");

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

  // Declare rpc service classes
  for (auto& service: services_map_) {
    DeclareRpcMethodClass(service.second.get());
  }

  CheckoutNameSpace(pkg_stack_, std::vector<std::string>());
  printer.Print("\n#endif  /* " + StringUtils::Upper(filename) + "_H_ */\n");
  printer.Flush();
}

void CppCodeGenerator::GenerateProtoPathName() {
  proto_file_ = IO::FileUtils::GetAbstractPath(proto_file_);
  std::string proto_name = proto_file_.substr(0, proto_file_.length() - 6);
  proto_path_name_ = StringUtils::replaceWith(proto_name, '/', '_');
  FormatPath(proto_path_name_);
}

void CppCodeGenerator::FormatPath(std::string& path) {
  for (unsigned int i = 0; i < path.length(); i++) {
    if (!StringUtils::IsLetterOrDigitOrUnderScore(path[i])) {
      path[i] = '_';
    }
  }
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

void CppCodeGenerator::DeclareRpcMethodClass(ServiceType* service) {
  CheckoutNameSpace(pkg_stack_, service->pkg_stack());
  printer.Print("class " + service->name() + ": public ::RPC::RpcService {\n");

  // Public:
  std::map<std::string, std::string> matches{
    {"service_name", service->name()},
  };
  printer.Print(" public:\n"
                "  virtual ~${service_name}() {}\n"
                "  virtual const ::RPC::RpcDescriptor* descriptor();\n"
                "\n"
                "  static ${service_name}* NewStub(::RPC::RpcClientChannel* channel);\n"
                "  static ${service_name}* NewStub(::RPC::RpcClientChannel* channel, const ::RPC::RpcStubOptions options);\n"
                "\n"
                "  virtual void RegisterToServer(::RPC::RpcServer* server);\n"
                "  virtual void DeRegisterFromServer(::RPC::RpcServer* server);\n"
                "\n"
                "  virtual void InternalRegisterHandlers(::RPC::RpcHandlerMap* handler_map);\n"
                "  virtual void InternalDeRegisterHandlers(::RPC::RpcHandlerMap* handler_map);\n"
                "\n", matches);
  for (const auto& rpc_service: service->RpcMethods()) {
    matches["rpc_method_name"] = rpc_service->name();
    matches["arg_type"] =
        (rpc_service->args_list())[0].type_class()->FullNameWithPackagePrefix(CPP);
    matches["return_type"] =
        (rpc_service->returns_list())[0].type_class()->FullNameWithPackagePrefix(CPP);
    printer.Print("  // ${rpc_method_name}() to be Implemented by user.\n"
                  "  virtual void ${rpc_method_name}(\n"
                  "      ::RPC::Rpc* rpc,\n"
                  "      const ${arg_type}* arg,\n"
                  "      ${return_type}* result,\n"
                  "      ::Base::Closure* cb);\n\n",
                  matches);
  }

  // Protected:
  printer.Print(" protected:\n"
                "  ${service_name}();\n"
                "  class Stub;\n"
                "  static const ::RPC::RpcDescriptor* descriptor_;\n\n",
                matches);

  // Private:
  printer.Print(" private:\n");
  for (const auto& rpc_service: service->RpcMethods()) {
    matches["rpc_method_name"] = rpc_service->name();
    printer.Print("  void internal_${rpc_method_name}(::RPC::Rpc* rpc);\n", matches);
  }

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
      "  ${msg_name}(const ${msg_name}& other);  // copy constructor\n"
      "  ${msg_name}(${msg_name}&& other);  // move constructor\n"
      "  ${msg_name}& operator=(const ${msg_name}& other); // copy assignment\n"
      "  ${msg_name}& operator=(${msg_name}&& other);  // move assignment\n"
      "  void Swap(${msg_name}* other);  // Swap\n"
      "  ::proto::Message* New() const override;  // New()\n"
      "  void CopyFrom(const ${msg_name}& other);  // CopyFrom()\n"
      "  void MoveFrom(${msg_name}&& other);  // MoveFrom()\n"
      "  bool Equals(const ${msg_name}& other) const;  // Compare\n"
      "  // Serialize() and DeSerialize().\n"
      "  ::proto::SerializedMessage* Serialize() const override;\n"
      "  void DeSerialize(const char* buf, unsigned int size) override;\n"
      "  static const ${msg_name}& default_instance();\n"
      "  void Print(int indent_num=0) const override;\n\n",
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
    if (!field->IsRepeatedType()) {
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
  if (!printer.Open(outfile + ".cc")) {
    fprintf(stderr, "ERROR: Open output file %s.p failed\n", outfile.c_str());
  }

  // Include proto_name.pb.h file
  std::vector<std::string> result = StringUtils::Split(outfile, '/');
  std::string filename = result[result.size() - 1];
  printer.Print("#include <memory>\n");
  printer.Print("#include <mutex>\n");
  printer.Print("#include <map>\n\n");
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

  for (auto& service: services_map_) {
    DefineServiceClass(service.second.get());
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

  printer.Print("  static bool already_called = false;\n"
                "  if (already_called) return;\n"
                "  already_called = true;\n"
                "\n"
                "  ::proto::ProtoParser::ProtoParser parser(\n"
                "      ::proto::ProtoParser::CPP,\n"
                "      \"${proto_file_}\");\n"
                "  CHECK(parser.ParseProto(),\n"
                "        \"static class initialization for ${proto_file_} failed\");\n"
                "\n",
                matches);
  printer.Print("  static_init_default_instances${proto_path_name}();\n\n",
                matches);
  if (!messages_list_.empty()) {
    printer.Print("  int i = 0;\n");
  }
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
                  "          ${whole_msg_name}::default_instance_,\n"
                  "          PROTO_MESSAGE_FIELD_OFFSET(${whole_msg_name}, has_bits_))\n"
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
  printer.Print("// ******************** " + message->name() + " ******************** //\n");
  DefineConstructor(message);
  DefineCopyConstructor(message);
  DefineMoveConstructor(message);
  DefineCopyAssigner(message);
  DefineMoveAssigner(message);
  DefineNew(message);
  DefineCopyFrom(message);
  DefineMoveFrom(message);
  DefineEquals(message);
  DefineSerialize(message);
  DefineDeSerialize(message);
  DefinePrint(message);
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
  std::map<std::string, std::string> matches{
     {"msg_name", message->name()},
  };

  printer.Print("// destructor\n");
  printer.Print("${msg_name}::~${msg_name}() {\n", matches);
  if (message->has_message_field()) {
    for (auto& field : message->fields_list()) {
      matches["field_name"] = field->name();
      if (field->type() == MESSAGETYPE &&
          field->modifier() != MessageField::REPEATED) {
        printer.Print("  if (${field_name}_) {\n"
                      "    delete ${field_name}_;\n"
                      "  }\n", matches);
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
        "  if (other.${field_name}_) {\n"
        "    if (!${field_name}_) {\n"
        "      ${field_name}_ = new ${type_name}();\n"
        "    }\n"
        "    *${field_name}_ = other.${field_name}();\n"
        "  }\n", matches);
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
  printer.Print("  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {\n"
                "    has_bits_[i] = other.has_bits_[i];\n"
                "  }\n");
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
      "${msg_name}::${msg_name}(const ${msg_name}& other) {\n"
      "  CopyFrom(other);\n"
      "}\n\n",
      msg_match);
}

void CppCodeGenerator::DefineCopyAssigner(Message* message) {
  printer.Print("// copy assignment\n");
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print(
      "${msg_name}& ${msg_name}::operator=(const ${msg_name}& other) {\n"
      "  CopyFrom(other);\n"
      "  return *this;\n"
      "}\n",
      msg_match);
}

void CppCodeGenerator::PrintMoveClassCode(Message* message) {
  printer.Print("  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {\n"
                "    has_bits_[i] = other.has_bits_[i];\n"
                "  }\n");

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
        "  if (other.${field_name}_) {\n"
        "    if (${field_name}_) {\n"
        "      delete ${field_name}_;\n"
        "    }\n"
        "    ${field_name}_ = other.${field_name}_;\n"
        "    other.${field_name}_ = nullptr;\n"
        "  }\n", matches);
    }
    else {
      printer.Print("  ${field_name}_ = other.${field_name}();\n", matches);
    }
  }
  printer.Print("  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {\n"
                "    other.has_bits_[i] = 0;\n"
                "  }\n");
}

void CppCodeGenerator::DefineEquals(Message* message) {
  printer.Print("// Equals()\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("bool ${msg_name}::Equals(const ${msg_name}& other) const {\n",
                msg_match);

  printer.Print("  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {\n"
                "    if (has_bits_[i] != other.has_bits_[i]) {\n"
                "      return false;\n"
                "    }\n"
                "  }\n");

  for (auto& field : message->fields_list()) {
    std::map<std::string, std::string> matches =
        GetFieldMatchMap(message, field.get());

    if (field->IsSingularMessageType()) {
      printer.Print(
        "  if (${field_name}_ && other.${field_name}_ &&\n"
        "      !${field_name}_->Equals(*other.${field_name}_)) {\n"
        "    return false;\n"
        "  }\n",
        matches);
    }
    else if (field->IsRepeatedMessageType()) {
      printer.Print(
        "  for (unsigned int i = 0; i < ${field_name}_.size(); i++) {\n"
        "    if (!${field_name}_.at(i).Equals(other.${field_name}_.at(i))) {\n"
        "      return false;\n"
        "    }\n"
        "  }\n",
        matches);
    }
    else if (field->IsSingularNumericType() || field->IsSingularStringType()) {
      printer.Print(
        "  if (${field_name}_ != other.${field_name}_) {\n"
        "    return false;\n"
        "  }\n",
        matches);
    }
    else if (field->IsRepeatedNumericType() || field->IsRepeatedStringType()) {
      printer.Print(
        "  for (unsigned int i = 0; i < ${field_name}_.size(); i++) {\n"
        "    if (${field_name}_.at(i) != other.${field_name}_.at(i)) {\n"
        "      return false;\n"
        "    }\n"
        "  }\n",
        matches);
    }
  }
  printer.Print("  return true;\n"
                "}\n\n");
}

void CppCodeGenerator::DefineMoveConstructor(Message* message) {
  printer.Print("// move constructor\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("${msg_name}::${msg_name}(${msg_name}&& other) {\n"
                "  MoveFrom(std::move(other));\n"
                "}\n\n",
                msg_match);
}

void CppCodeGenerator::DefineMoveAssigner(Message* message) {
  printer.Print("// move assignment\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("${msg_name}& ${msg_name}::operator=(${msg_name}&& other) {\n"
                "  MoveFrom(std::move(other));\n"
                "  return *this;\n"
                "}\n\n",
                msg_match);
}

void CppCodeGenerator::DefineNew(Message* message) {
  printer.Print("// New()\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("::proto::Message* ${msg_name}::New() const {\n"
                "  return reinterpret_cast<::proto::Message*>(new ${msg_name}());\n"
                "}\n\n",
                msg_match);
}

void CppCodeGenerator::DefineCopyFrom(Message* message) {
  printer.Print("// CopyFrom()\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("void ${msg_name}::CopyFrom(const ${msg_name}& other) {\n",
                msg_match);
  PrintCopyClassCode(message);
  printer.Print("}\n\n");
}

void CppCodeGenerator::DefineMoveFrom(Message* message) {
  printer.Print("// MoveFrom()\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("void ${msg_name}::MoveFrom(${msg_name}&& other) {\n",
                msg_match);
  PrintMoveClassCode(message);
  printer.Print("}\n\n");
}

void CppCodeGenerator::DefineSerialize(Message* message) {
  printer.Print("// Serialize()\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("::proto::SerializedMessage* ${msg_name}::Serialize() const {\n"
                "  return ${msg_name}_reflection_->Serialize(this);\n"
                "}\n\n",
                msg_match);
}

void CppCodeGenerator::DefineDeSerialize(Message* message) {
  printer.Print("// DeSerialize()\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("void ${msg_name}::DeSerialize(const char* buf, unsigned int size) {\n"
                "  ${msg_name}_reflection_->DeSerialize(this, buf, size);\n"
                "}\n\n",
                msg_match);
}

void CppCodeGenerator::DefinePrint(Message* message) {
  printer.Print("// Print()\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("void ${msg_name}::Print(int indent_num) const {\n"
                "  PrintIndent(indent_num);\n"
                "  std::cout << \"${msg_name} \" << \"{\" << std::endl;\n",
                msg_match);

  for (auto& field : message->fields_list()) {
    std::map<std::string, std::string> matches =
        GetFieldMatchMap(message, field.get());
    if (field->IsSingularNumericType() || field->IsSingularStringType()) {
      printer.Print("  if (has_${field_name}()) {\n"
                    "    PrintIndent(indent_num + 1);\n",
                    matches);
      if (field->type() != ENUMTYPE) {
        printer.Print("    std::cout << \"${field_name}: \" << ${field_name}_ << std::endl;\n"
                      "  }\n",
                      matches);
      }
      else {
        // For EnumType we need to print enum value as string.
        printer.Print("    std::string enum_value =\n"
                      "        (reinterpret_cast<const proto::ProtoParser::EnumType*>(\n"
                      "            ${msg_name}_descriptor_->FindFieldByName(\"${field_name}\")->type_class()))\n"
                      "                 ->EnumValueAsString(${field_name}_);\n"
                      "    std::cout << \"${field_name}: \" << enum_value << std::endl;\n"
                      "  }\n",
                      matches); 
      }
    }
    else if (field->IsSingularMessageType()) {
      printer.Print("  if (has_${field_name}()) {\n"
                    "    PrintIndent(indent_num + 1);\n"
                    "    std::cout << \"${field_name}: \" << \"*\" << std::endl;\n"
                    "    ${field_name}_->Print(indent_num + 1);\n"
                    "  }\n",
                    matches);
    }
    else if (field->IsRepeatedStringType() || field->IsRepeatedNumericType()) {
      printer.Print("  if (${field_name}_size() > 0) {\n"
                    "    PrintIndent(indent_num + 1);\n"
                    "    std::cout << \"${field_name}: \" << \"[\";\n",
                    matches);
      if (field->type() != ENUMTYPE) {
        printer.Print("    for (const auto& ele: ${field_name}_) {\n"
                      "        std::cout << ele << \", \";\n"
                      "    }\n"
                      "    std::cout << \"]\" << std::endl;\n"
                      "  }\n",
                      matches);
      }
      else {
        printer.Print("    const auto* enum_type_descriptor =\n"
                      "        (reinterpret_cast<const proto::ProtoParser::EnumType*>(\n"
                      "            ${msg_name}_descriptor_->FindFieldByName(\"${field_name}\")->type_class()));\n"
                      "    for (const auto& ele: ${field_name}_) {\n"
                      "        std::cout << enum_type_descriptor->EnumValueAsString(ele) << \", \";\n"
                      "    }\n"
                      "    std::cout << \"]\" << std::endl;\n"
                      "  }\n",
                      matches);
      }
    }
    else if (field->IsRepeatedMessageType()) {
      printer.Print("  if (${field_name}_size() > 0) {\n"
                    "    PrintIndent(indent_num + 1);\n"
                    "    std::cout << \"${field_name}: \" << \"[***]\" << std::endl;\n"
                    "    for (const auto& ele: ${field_name}_) {\n"
                    "        ele.Print(indent_num + 1);\n"
                    "    }\n"
                    "  }\n",
                    matches);
    }
  }
  printer.Print("  PrintIndent(indent_num);\n"
                "  std::cout << \"}\" << std::endl;\n"
                "}\n\n");
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
                "}\n\n"
                "${msg_name}* ${msg_name}::default_instance_ = NULL;\n\n",
                matches);
}

void CppCodeGenerator::DefineSwapper(Message* message) {
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };

  printer.Print("// swapper\n");
  printer.Print("void ${msg_name}::Swap(${msg_name}* other) {\n", msg_match);
  // swap has bits first
  printer.Print("  // store has_bits\n"
                "  char* buf = new char[2 * sizeof(has_bits_)];\n"
                "  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {\n"
                "    buf[i] = has_bits_[i];\n"
                "    buf[i + sizeof(has_bits_)] = other->has_bits_[i];\n"
                "  }\n");
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
  printer.Print("\n  // swap has_bits\n"
                "  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {\n"
                "    has_bits_[i] = buf[i + sizeof(has_bits_)];\n"
                "    other->has_bits_[i] = buf[i];\n"
                "  }\n"
                "  delete buf;\n");
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
  printer.Print("  ${type_name} ${field_name}(int index) const;\n",
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
  printer.Print("  const ${type_name}& ${field_name}(int index) const;\n", matches);

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
  printer.Print("bool ${msg_name}::has_${field_name}() const {\n"
                "  return (has_bits_[${tag_byte_index}] & ${tag_bit}) != 0;\n"
                "}\n\n",
                matches);

  // Implement - int32 foo() const;
  printer.Print("${type_name} ${msg_name}::${field_name}() const {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);
  
  // Implement - void set_foo(int32 value);
  printer.Print("void ${msg_name}::set_${field_name}(${type_name} ${field_name}) {\n"
                "  ${field_name}_ = ${field_name};\n"
                "  has_bits_[${tag_byte_index}] |= ${tag_bit};\n"
                "}\n\n",
                matches);
  
  // Implement - void clear_foo(int32 value);
  printer.Print("void ${msg_name}::clear_${field_name}() {\n"
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
  printer.Print("bool ${msg_name}::has_${field_name}() const {\n"
                "  return (has_bits_[${tag_byte_index}] & ${tag_bit}) != 0;\n"
                "}\n\n",
                matches);
  
  // Implement - std::string foo() const;
  printer.Print("const std::string& ${msg_name}::${field_name}() const {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);
  
  // Implement - void set_foo(std::string value);
  printer.Print("void ${msg_name}::set_${field_name}(const std::string& ${field_name}) {\n"
                "  ${field_name}_ = ${field_name};\n"
                "  has_bits_[${tag_byte_index}] |= ${tag_bit};\n"
                "}\n\n",
                matches);

  // Implement - void set_foo(const char* value);
  printer.Print("void ${msg_name}::set_${field_name}(const char* ${field_name}) {\n"
                "  ${field_name}_ = std::string(${field_name});\n"
                "  has_bits_[${tag_byte_index}] |= ${tag_bit};\n"
                "}\n\n",
                matches);

  // Implement - void set_foo(const char* value, int size);
  printer.Print("void ${msg_name}::set_${field_name}(const char* ${field_name}, int size) {\n"
                "  ${field_name}_ = std::string(${field_name}, size);\n"
                "  has_bits_[${tag_byte_index}] |= ${tag_bit};\n"
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
  printer.Print("bool ${msg_name}::has_${field_name}() const {\n"
                "  return (has_bits_[${tag_byte_index}] & ${tag_bit}) != 0;\n"
                "}\n\n",
                matches);
  
  // Implement - const Bar& foo() const;
  printer.Print("const ${type_name}& ${msg_name}::${field_name}() const {\n"
                "  if (has_${field_name}() && ${field_name}_) {\n"
                "    return *${field_name}_;\n"
                "  }\n"
                "  else {\n"
                "    return ${type_name}::default_instance();\n"
                "  }\n"
                "}\n\n",
                matches);

  // Implement - Bar* mutable_foo();
  printer.Print("${type_name}* ${msg_name}::mutable_${field_name}() {\n"
                "  if (has_${field_name}() && ${field_name}_) {\n"
                "    return ${field_name}_;\n"
                "  }\n"
                "  else {\n"
                "    ${field_name}_ = new ${type_name}();\n"
                "    has_bits_[${tag_byte_index}] |= ${tag_bit};\n"
                "    return ${field_name}_;\n"
                "  }\n"
                "}\n\n",
                matches);

  // Implement - void set_alocated_foo(Bar* value);
  printer.Print("void ${msg_name}::set_allocated_${field_name}(${type_name}* ${field_name}) {\n"
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
  printer.Print("${type_name}* ${msg_name}::release_${field_name}() {\n"
                "  ${type_name}* ${field_name}_tmp__ = ${field_name}_;\n"
                "  ${field_name}_ = nullptr;\n"
                "  has_bits_[${tag_byte_index}] &= (~${tag_bit});\n"
                "  return ${field_name}_tmp__;\n"
                "}\n\n",
                matches);

  // Implement - void clear_foo();
  printer.Print("void ${msg_name}::clear_${field_name}() {\n"
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
  printer.Print("int ${msg_name}::${field_name}_size() const {\n"
                "  return ${field_name}_.size();\n"
                "}\n\n",
                matches);

  // Implement - Bar foo(int index) const;
  printer.Print("${type_name} ${msg_name}::${field_name}(int index) const {\n"
                "  return ${field_name}_.Get(index);\n"
                "}\n\n",
                matches);

  // Implement - void set_foo(int index, Bar& value);
  printer.Print("void ${msg_name}::set_${field_name}(int index, ${type_name} value) {\n"
                "  if ((int)${field_name}_.size() > index) {\n"
                "    ${field_name}_.Set(index, value);\n"
                "  }\n"
                "}\n\n",
                matches);

  // Implement - void add_foo(Bar& value);
  printer.Print("void ${msg_name}::add_${field_name}(${type_name} value) {\n"
                "   ${field_name}_.Add(value);\n"
                "}\n\n",
                matches);

  // Implement - void clear_foo();
  printer.Print("void ${msg_name}::clear_${field_name}() {\n"
                "  ${field_name}_ .Clear();\n"
                "}\n\n",
                matches);

  // Implement - const ::proto::RepeatedField<Bar>& foo() const;
  printer.Print("const ::proto::RepeatedField<${type_name}>& ${msg_name}::${field_name}() const {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);

  // Implement - ::proto::RepeatedField<Bar>& mutable_foo();
  printer.Print("::proto::RepeatedField<${type_name}>& ${msg_name}::mutable_${field_name}() {\n"
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
  printer.Print("const ${type_name}& ${msg_name}::${field_name}(int index) const {\n"
                "  return ${field_name}_.Get(index);\n"
                "}\n\n",
                matches);

  // String type can also have value passed by const char*
  if (field->IsRepeatedStringType()) {
    // Implement - void set_foo(int index, const char* value);
    printer.Print("void ${msg_name}::set_${field_name}(int index, const std::string& value) {\n"
                  "  if (index < (int)${field_name}_.size()) {\n"
                  "    ${field_name}_.Set(index, value);\n"
                  "  }\n"
                  "}\n\n",
                  matches);

    // Implement - void set_foo(int index, const char* value);
    printer.Print("void ${msg_name}::set_${field_name}(int index, const char* value) {\n"
                  "  if (index < (int)${field_name}_.size()) {\n"
                  "    ${field_name}_.Set(index, std::string(value));\n"
                  "  }\n"
                  "}\n\n",
                  matches);

    // Implement - void set_foo(int index, const char* value, int size);
    printer.Print("void ${msg_name}::set_${field_name}(int index, const char* value, int size) {\n"
                  "  if (index < (int)${field_name}_.size()) {\n"
                  "    ${field_name}_.Set(index, std::string(value, size));\n"
                  "  }\n"
                  "}\n\n",
                  matches);
  }

  // Implement - Bar* add_foo();
  printer.Print("${type_name}* ${msg_name}::add_${field_name}() {\n"
                "  return ${field_name}_.Add();\n"
                "}\n\n",
                matches);

  // String type can also have value passed by const char*
  if (field->IsRepeatedStringType()) {
    // Implement - void add_foo(const std::string value);
    printer.Print("void ${msg_name}::add_${field_name}(const std::string& value) {\n"
                  "  ${field_name}_.AddAllocated(new std::string(value));\n"
                  "}\n\n",
                  matches);

    // Implement - void add_foo(const char* value);
    printer.Print("void ${msg_name}::add_${field_name}(const char* value) {\n"
                  "  ${field_name}_.AddAllocated(new std::string(value));\n"
                  "}\n\n",
                  matches);

    // Implement - void add_foo(const char* value, int size);
    printer.Print("void ${msg_name}::add_${field_name}(const char* value, int size) {\n"
                  "  ${field_name}_.AddAllocated(new std::string(value, size));\n"
                  "}\n\n",
                  matches);
  }

  // Implement - Bar* mutable_foo(int index);
  printer.Print("${type_name}* ${msg_name}::mutable_${field_name}(int index) {\n"
                "  return ${field_name}_.GetMutable(index);\n"
                "}\n\n",
                matches);  

  // Implement - void clear_foo();
  printer.Print("void ${msg_name}::clear_${field_name}() {\n"
                "  ${field_name}_.Clear();\n"
                "}\n\n",
                matches);

  // Implement - const ::proto::RepeatedPtrField<Bar>& foo() const;
  printer.Print("const ::proto::RepeatedPtrField<${type_name}>& ${msg_name}::${field_name}() const {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);

  // Implement - ::proto::RepeatedPtrField<Bar>& mutable_foo();
  printer.Print("::proto::RepeatedPtrField<${type_name}>& ${msg_name}::mutable_${field_name}() {\n"
                "  return ${field_name}_;\n"
                "}\n\n",
                matches);
}

void CppCodeGenerator::DefineServiceClass(ServiceType* service) {
  CheckoutNameSpace(pkg_stack_, service->pkg_stack());
  DefineStubClass(service);
  DefineServiceClassMethods(service);
}

void CppCodeGenerator::DefineServiceClassMethods(ServiceType* service) {
  std::map<std::string, std::string> matches{
    {"service_name", service->name()},
    {"full_service_name", service->FullNameWithPackagePrefix()},
  };
  printer.Print("// *********************** ${service_name} *********************** //\n",
                matches);
  // Constructor
  printer.Print("${service_name}::${service_name}() : ::RPC::RpcService(\"${full_service_name}\") {\n"
                "}\n\n", matches);
  // Descriptor
  printer.Print("const ::RPC::RpcDescriptor* ${service_name}::descriptor() { return descriptor_; }\n\n",
                matches);
  // NewStub
  printer.Print("StudentManagement* ${service_name}::NewStub(::RPC::RpcClientChannel* channel) {\n"
                "  return new ${service_name}::Stub(\"${service_name}\", channel, ::RPC::RpcStubOptions());\n"
                "}\n\n"
                "StudentManagement* ${service_name}::NewStub(\n"
                "    ::RPC::RpcClientChannel* channel, const ::RPC::RpcStubOptions options) {\n"
                "  return new ${service_name}::Stub(\"${service_name}\", channel, options);\n"
                "}\n\n", matches);
  // Register and De-Register service.
  printer.Print("void ${service_name}::RegisterToServer(::RPC::RpcServer* server) {\n"
                "  server->RegisterService(\"${service_name}\", this);\n"
                "  InternalRegisterHandlers(server->handler_map());\n"
                "}\n\n", matches);
  printer.Print("void ${service_name}::DeRegisterFromServer(::RPC::RpcServer* server) {\n"
                "  server->DeRegisterService(\"${service_name}\");\n"
                "  InternalDeRegisterHandlers(server->handler_map());\n"
                "}\n\n", matches);
  // Internal register handler to server's handler_map
  printer.Print("void ${service_name}::InternalRegisterHandlers(::RPC::RpcHandlerMap* handler_map) {\n",
                matches);
  for (auto rpc_method: service->RpcMethods()) {
    matches = GetRpcMatchMap(service, rpc_method.get());
    printer.Print("  (*handler_map)[\"${service_full_name}.${rpc_method_name}\"] =\n"
                  "      std::shared_ptr<::RPC::RpcHandler>(new RPC::RpcHandler(\n"
                  "          \"${service_full_name}.${rpc_method_name}\",  // full rpc name\n"
                  "          \"${rpc_method_name}\",  // method name\n"
                  "          new ${arg_type}(),  // request proto type\n"
                  "          new ${return_type}(),  // response proto type\n"
                  "          nullptr,  // TODO: stream prototype\n"
                  "          new RPC::InternalRpcMethod(\n"
                  "              std::bind(&${service_name}::internal_${rpc_method_name},\n"
                  "                        this, std::placeholders::_1))\n"
                  "      ));\n",
                  matches);
  }
  printer.Print("}\n\n");

  // Internal de-register handler from server's handler_map
  printer.Print("void ${service_name}::InternalDeRegisterHandlers(::RPC::RpcHandlerMap* handler_map) {\n"
                "  auto it = handler_map->end();\n",
                matches);
  for (const auto rpc_method: service->RpcMethods()) {
    matches = GetRpcMatchMap(service, rpc_method.get());
    printer.Print("  if ((it = handler_map->find(\"${service_full_name}.${rpc_method_name}\")) != handler_map->end()) {\n"
                  "    handler_map->erase(it);\n"
                  "  }\n",
                  matches);
  }
  printer.Print("}\n\n");

  // internal rpc handler method.
  for (const auto rpc_method: service->RpcMethods()) {
    matches = GetRpcMatchMap(service, rpc_method.get());
    printer.Print("void ${service_name}::internal_${rpc_method_name}(::RPC::Rpc* rpc) {\n"
                  "  ${rpc_method_name}(\n"
                  "      rpc,\n"
                  "      (${arg_type}*)rpc->internal_request(),\n"
                  "      (${return_type}*)rpc->internal_response(),\n"
                  "      rpc->cb_final()\n"
                  "  );\n"
                  "}\n\n", matches);
  }

  // Rpc method to be implemented by user.
  for (const auto rpc_method: service->RpcMethods()) {
    matches = GetRpcMatchMap(service, rpc_method.get());
    printer.Print("void ${service_name}::${rpc_method_name}(\n"
                  "    ::RPC::Rpc* rpc, const ${arg_type}*,\n"
                  "    ${return_type}*, ::Base::Closure* done) {\n"
                  "  UnInplemented(rpc, done);\n"
                  "}\n\n", matches);
  }

  // Init descriptor.
  matches["full_service_name_underscore"] =
      StringUtils::replaceWith(service->FullNameWithPackagePrefix(), ".", "_");
  matches["package"] = service->package();
  printer.Print("static const RPC::RpcDescriptor* Init_${full_service_name_underscore}_Descriptor() {\n"
                "  ::RPC::RpcDescriptor* descriptor =\n"
                "      new ::RPC::RpcDescriptor(\"${package}\", \"${service_name}\");\n",
                matches);
  for (const auto& rpc_method: service->RpcMethods()) {
    matches = GetRpcMatchMap(service, rpc_method.get());
    printer.Print("  // Add method ${rpc_method_name}\n"
                  "  {\n"
                  "    ::RPC::RpcMethodDescriptor* method_decpt = descriptor->AddMethod(\"${rpc_method_name}\");\n"
                  "    method_decpt->set_arg_type(\"${arg_type_py}\");\n"
                  "    method_decpt->set_return_type(\"${return_type_py}\");\n",
                  matches);
    for (const auto& it: rpc_method->options_map()) {
      matches["option_key"] = it.first;
      matches["option_value"] = it.second;
      printer.Print("    method_decpt->add_option(\"${option_key}\", \"${option_value}\");\n",
                    matches);
    }
    printer.Print("  }\n");
  }
  printer.Print("\n"
                "  return descriptor;\n"
                "}\n\n");

  matches["full_service_name_underscore"] =
      StringUtils::replaceWith(service->FullNameWithPackagePrefix(), ".", "_");
  printer.Print("const RPC::RpcDescriptor* ${service_name}::descriptor_ =\n"
                "    Init_${full_service_name_underscore}_Descriptor();\n\n",
                matches);
}

void CppCodeGenerator::DefineStubClass(ServiceType* service) {
  std::map<std::string, std::string> matches{
    {"service_name", service->name()},
    {"full_service_name", service->FullNameWithPackagePrefix()},
  };
  printer.Print("// *********************** ${service_name}_Stub *********************** //\n"
                "class ${service_name}::Stub : public ${service_name} {\n"
                " public:\n"
                "  Stub(const char* name, ::RPC::RpcClientChannel* channel, const ::RPC::RpcStubOptions options):\n"
                "      ${service_name}() {\n"
                "    ::RPC::RpcService::InitStub(name, channel, options);\n"
                "  }\n",
                matches);
  for (auto& rpc_service: service->RpcMethods()) {
    matches = GetRpcMatchMap(service, rpc_service.get());
    printer.Print("\n"
                  "  virtual void ${rpc_method_name}(\n"
                  "      RPC::Rpc* rpc, const ${arg_type}* request,\n"
                  "      ${return_type}* response,\n"
                  "      Base::Closure* cb) {\n"
                  "    StartClientRpcCall(rpc, descriptor_, \"${rpc_method_name}\", request, response, cb);\n"
                  "  }\n", matches);
  }
  printer.Print("};\n\n");
}

std::map<std::string, std::string>
CppCodeGenerator::GetRpcMatchMap(ServiceType* service, RpcMethod* rpc) {
  std::map<std::string, std::string> matches;
  matches["service_name"] = service->name();
  matches["service_full_name"] = service->FullNameWithPackagePrefix();
  matches["rpc_method_name"] = rpc->name();
  matches["arg_type"] =
      (rpc->args_list()).at(0).type_class()->FullNameWithPackagePrefix(CPP);
  matches["return_type"] =
      (rpc->returns_list()).at(0).type_class()->FullNameWithPackagePrefix(CPP);
  matches["arg_type_py"] =
      (rpc->args_list()).at(0).type_class()->FullNameWithPackagePrefix();
  matches["return_type_py"] =
      (rpc->returns_list()).at(0).type_class()->FullNameWithPackagePrefix();
  return matches;
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
  // std::cout << "context_stk package: ";
  // for (const auto& str: context_stk) {
  //   std::cout << str << ".";
  // }
  // std::cout << std::endl;
  // std::cout << "target package: ";
  // for (const auto& str: context_stk) {
  //   std::cout << str << ".";
  // }
  // std::cout << std::endl;

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
  // std::cout << "prefix = " << prefix << std::endl;
  return prefix;
}

}  // Compiler
}  // PandaProto
