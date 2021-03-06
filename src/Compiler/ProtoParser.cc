#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "Base/Log.h"
#include "IO/File.h"
#include "Strings/Split.h"
#include "Strings/Utils.h"

#include "Compiler/ProtoParser.h"

namespace proto {
namespace ProtoParser {

Parser::Parser(LANGUAGE lang) :
  lang_(lang) {
}

Parser::Parser(LANGUAGE lang, const std::string& file) :
    lang_(lang),
    proto_file_(file) {
  if (!Strings::EndWith(proto_file_, ".proto")) {
    init_success_ = false;
    fprintf(stderr, "ERROR: proto file name must have \".proto\" postfix.\n");
    return;
  }

  if (!File::GetContent(proto_file_, &proto_content_)) {
    LogERROR("Failed to read proto file %s", proto_file_.c_str());
    return;
  }

  init_success_ = true;
}

Parser::~Parser() {}

void Parser::set_proto_content(const std::string& proto_content) {
  proto_content_ = proto_content;
  init_success_ = true;
}

bool Parser::Do_ParseProto() {
  if (!init_success_) {
    return false;
  }

  // Read each line and parse in a finite state machine.
  auto lines = Strings::Split(proto_content_, "\n", Strings::SkipWhiteSpace());
  for (std::string line : lines) {
    line_number_++;
    line = Strings::Strip(line);
    //std::cout << "--------------------------------------------" << std::endl;
    //PrintParseState();
    //std::cout << "Parsing: \"" << line << "\"\n" << std::endl;

    // Skip empty and comment lines.
    if (line.length() == 0 || Strings::StartWith(line, "//")) {
      continue;
    }
    // remove comment in the line.
    std::size_t pos;
    if ((pos = line.find("//")) != std::string::npos) {
      line = Strings::Strip(line.substr(0, pos));
    }

    // Parse in Global state: Only accept line start with "package ",
    // "message ", "enum ", "service".
    if (state_ == GLOBAL) {
      if (Strings::StartWith(line, "package ")) {
        if (!ParsePackageName(line)) {
          return false;
        }
      }
      // Parse message.
      else if (Strings::StartWith(line, "message ")) {
        if (!ParseMessageName(line)) {
          return false;
        }
        state_ = PARSINGMSG;
      }
      else if (Strings::StartWith(line, "enum ")) {
        if (!ParseEnumName(line)) {
          return false;
        }
        state_ = PARSINGENUM;
      }
      else if (Strings::StartWith(line, "service ")) {
        if (!ParseServiceName(line)) {
          return false;
        }
        state_ = PARSESERVICE;
      }
      else {
        LogERROR("Illegal line in global");
        return false;
      }
    }

    // Parse a message state.
    else if (state_ == PARSINGMSG) {
      // Parse message field.
      if (Strings::StartWith(line, "enum ")) {
        if (!ParseEnumName(line)) {
          return false;
        }
        state_ = PARSINGNESTEDENUM;
      }
      else if (line == "}") {
        if (current_message_->fields_list().empty() &&
            current_message_->enums_map().empty()) {
          LogERROR("Parsed %s contains nothing",
                   current_message_->name().c_str());
          return false;
        }
        // Check out namespace.
        pkg_stack_.pop_back();
        current_package_ =
            current_package_.substr(
                0, Strings::FindLastMatch(current_package_, "."));
        state_ = GLOBAL;
      }
      else if (IsMessageFiledLine(line)) {
        if (!ParseMessageField(line)) {
          return false;
        }
      }
      else {
        LogERROR("Illegal line in Parsing a message");
        return false;
      }
    }

    // Parse a enum state.
    else if (state_ == PARSINGENUM) {
      if (line == "}") {
        if (current_enum_->enums().empty()) {
          LogERROR("Enum %s contains nothing",
                   current_enum_->name().c_str());
          return false;
        }
        state_ = GLOBAL;
      }
      else if (!ParseEnumValue(line)) {
        return false;
      }
    }

    // Parse a nested enum state.
    else if (state_ == PARSINGNESTEDENUM) {
      if (line == "}") {
        if (current_enum_->enums().empty()) {
          LogERROR("Nested enum %s contains nothing",
                   current_enum_->name().c_str());
          return false;
        }
        state_ = PARSINGMSG;
      }
      else if (!ParseEnumValue(line)) {
        return false;
      }
    }

    // Parse a service
    else if (state_ == PARSESERVICE) {
      if (Strings::StartWith(line, "rpc ")) {
        if (!ParseRpcName(line)) {
          return false;
        }
        state_ = PARSERPC;
      }
      else if (Strings::StartWith(line, "option ")) {
        if (!ParseRpcOption(line)) {
          return false;
        }
      }
      else if (line == "}") {
        if (current_service_->RpcMethods().empty()) {
          LogERROR("service %s contains nothing",
                   current_service_->name().c_str());
          return false;
        }
        state_ = GLOBAL;
      }
      else {
        LogERROR("Illegal line in parsing a service");
        return false;
      }
    }

    // Parse a nested rpc in service
    else if (state_ == PARSERPC) {
      if (Strings::StartWith(line, "option ")) {
        if (!ParseRpcOption(line)) {
          return false;
        }
      }
      else if (line == "}") {
        state_ = PARSESERVICE;
      }
      else {
        LogERROR("Illegal line in parsing a rpc");
        return false;
      }
    }

    // Syntax Error
    else {
      LogERROR("Illegal global line, can't parse");
      return false;
    }
  }

  for (auto& message: messages_list_) {
    message->SortFieldsByTag();
  }
  return true;
}

bool Parser::ParsePackageName(std::string line) {
  if (line[line.length()-1] != ';') {
    LogERROR("Expect \";\" at line end");
    return false;
  }
  line = Strings::Strip(line, ";");
  auto result = Strings::Split(line, ' ', Strings::SkipWhiteSpace());
  if (result.size() != 2) {
    LogERROR("Expect 2 tokens, actual %d", result.size());
    return false;
  }
  if (result[0] != "package") {
    LogERROR("Unknown key word", result[0].c_str());
    return false;
  }
  current_package_ = result[1];
  result = Strings::Split(current_package_, '.');
  pkg_stack_.clear();
  for (auto& pkg: result) {
    if (!IsValidVariableName(pkg)) {
      LogERROR("Invalid package name %s in %s",
               pkg.c_str(), current_package_.c_str());
      return false;
    }
    pkg_stack_.push_back(pkg);
  }
  return true;
}

bool Parser::ParseMessageName(std::string line) {
  auto result = Strings::Split(line, ' ', Strings::SkipWhiteSpace());
  if (result.size() != 2 && result.size() != 3) {
    LogERROR("Expect 2 or 3 tokens, actual %d", result.size());
    return false;
  }
  if (result[0] != "message") {
    LogERROR("Unknown keyword \"%s\"\n", result[0].c_str());
    return false;
  }
  if (result.size() > 2 && result[2] != "{") {
    LogERROR("Syntax error, expect \"{\" at line end\n");
    return false;
  }
  if (result.size() == 2 && result[1][result[1].length()-1] != '{') {
    LogERROR("Syntax error, expect \"{\" at line end\n");
    return false;
  }

  // Get message name.
  std::string message_name = result[1];
  if (result.size() == 2) {
    message_name = message_name.substr(0, message_name.length() - 1);
  }
  if (!IsValidVariableName(message_name)) {
    LogERROR("invalid message name \"%s\"", message_name.c_str());
    return false;
  }
  // Check name duplication.
  const std::string& full_msg_name =
      PbType::GeneratePackagePrefix(PYTHON, pkg_stack_) + message_name;
  if (messages_map_.find(full_msg_name) != messages_map_.end()) {
    LogERROR("message name \"%s\" already exists", full_msg_name.c_str());
    return false;
  }

  // Add new message to message map.
  std::shared_ptr<Message> new_message(new Message(message_name,
                                                   current_package_));
  messages_list_.push_back(new_message);
  messages_map_[full_msg_name] = new_message;
  current_message_ = new_message.get();

  // update current namepsace
  pkg_stack_.push_back(message_name);
  const std::string& dot = current_package_.empty()? "" : ".";
  current_package_ += (dot + message_name);
  return true;
}

bool Parser::ParseMessageField(std::string line) {
  if (line[line.length()-1] != ';') {
    LogERROR("Expect \";\" at line end");
    return false;
  }
  line = Strings::Strip(line, ";");
  auto result = Strings::Split(line, ' ', Strings::SkipWhiteSpace());
  if (result.size() < 3) {
    LogERROR("Syntax error");
    return false;
  }
  // Parse field modifier.
  FieldLabel modifier;
  if ((modifier = MessageField::GetMessageFieldModifier(result[0])) ==
      UNKNOWN_MODIFIER) {
    LogERROR("Unknown modifier \"%s\"", result[0].c_str());
    return false;
  }
  // Parse field type.
  std::string type_name = result[1];
  FIELD_TYPE type;
  PbType* type_class = NULL;
  if ((type = PbCommon::GetMessageFieldType(type_name)) == UNDETERMINED) {
    type_class = FindParsedMessageOrEnumType(type_name);
    if (!type_class) {
      LogERROR("Unknown field type \"%s\"", type_name.c_str());
      return false;
    }
    type = type_class->type();
  }

  // Parse name and tag.
  std::string remain;
  for (unsigned int i = 2; i < result.size(); i++) {
    remain += (result[i] + " ");
  }

  std::string nametag = remain, defaultblock = "";
  std::size_t pos = remain.find("[");
  if (pos != std::string::npos) {
    nametag = Strings::Strip(remain.substr(0, pos));
    defaultblock = Strings::Strip(remain.substr(pos));
    if (defaultblock[defaultblock.length()-1] != ']') {
      LogERROR("Expect \"]\" after default assignement");
      return false;
    }
    defaultblock = Strings::Strip(defaultblock, "[]");   
  }

  std::string name, tag;
  if (!ParseAssignExpression(nametag, &name, &tag, INT32)) {
    return false;
  }
  int tag_num = std::stoi(tag);

  // Parse default block.
  std::string default_name, default_value = "";
  if (defaultblock.length() > 0) {
    if (type == MESSAGETYPE) {
      LogERROR("message type cannot have default value");
      return false;
    }
    if (modifier == REPEATED) {
      LogERROR("message type cannot have default value");
      return false;
    }

    // Value is validated inside ParseAssignExpression(), except for enum type.
    if (!ParseAssignExpression(defaultblock, &default_name, &default_value,
                               type)) {
      return false;
    }
    // Validate default value of enum type.
    if (type == ENUMTYPE) {
      EnumType* enumclass = static_cast<EnumType*>(type_class);
      if (!enumclass->ContainsEnum(default_value)) {
        LogERROR("Enum value \"%s\" is not a valid value in enum type %s",
                 default_value.c_str(), type_class->name().c_str());
        return false;
      }
    }
    if (default_name != "default") {
      LogERROR("Can't recognize \"%s\", should be \"default\"",
               default_name.c_str());
      return false;
    }
  }

  // Add new field to current message.
  std::shared_ptr<MessageField> new_field(
      new MessageField(modifier, type, type_class, name, tag_num,
                       default_value));
  if (!current_message_->AddField(new_field)) {
    LogERROR("Add field \"%s\" to message \"%s\" failed",
             name.c_str(), current_message_->name().c_str());
    return false;
  }

  return true;
}

bool Parser::ParseEnumName(std::string line) {
  auto result = Strings::Split(line, ' ', Strings::SkipWhiteSpace());
  if (result.size() != 2 && result.size() != 3) {
    LogERROR("Expect 2 or 3 least tokens, actual %d", result.size());
    return false;
  }
  if (result[0] != "enum") {
    LogERROR("Unknown keyword \"%s\"\n", result[0].c_str());
    return false;
  }
  if (result.size() > 2 && result[2] != "{") {
    LogERROR("Syntax error, expect \"{\" at line end\n");
    return false;
  }
  if (result.size() == 2 && result[1][result[1].length()-1] != '{') {
    LogERROR("Syntax error, expect \"{\" at line end\n");
    return false;
  }

  // Get enum name.
  std::string enum_name = result[1];
  if (result.size() == 2) {
    enum_name = enum_name.substr(0, enum_name.length() - 1);
  }
  if (!IsValidVariableName(enum_name)) {
    LogERROR("invalid enum name \"%s\"", enum_name.c_str());
    return false;
  }
  // Check name duplication
  const std::string& full_enum_name =
      PbType::GeneratePackagePrefix(PYTHON, pkg_stack_) + enum_name;
  if (state_ == PARSINGMSG && 
      current_message_->enums_map().find(full_enum_name) !=
          current_message_->enums_map().end()) {
    LogERROR("enum name \"%s\" already exists in message \"%s\"",
             full_enum_name.c_str(), current_message_->name().c_str());
    return false;
  }
  if (state_ == GLOBAL && 
      enums_map_.find(full_enum_name) != enums_map_.end()) {
    LogERROR("enum name \"%s\" already exists", full_enum_name.c_str());
    return false;
  }

  // Add new enum to the enum map
  std::shared_ptr<EnumType> new_enum;
  if (state_ == GLOBAL) {
    new_enum.reset(new EnumType(enum_name, current_package_));
    enums_map_[full_enum_name] = new_enum;
    current_enum_ = new_enum.get();
  }
  else if (state_ == PARSINGMSG) {
    new_enum.reset(new EnumType(enum_name, current_package_,
                                current_message_->name()));
    current_message_->AddEnum(new_enum);
    current_enum_ = new_enum.get();
  }

  return true;
}

bool Parser::ParseServiceName(std::string line) {
  auto result = Strings::Split(line, ' ', Strings::SkipWhiteSpace());
  if (result.size() != 2 && result.size() != 3) {
    LogERROR("Expect 2 or 3 least tokens, actual %d", result.size());
    return false;
  }
  if (result[0] != "service") {
    LogERROR("Unknown keyword \"%s\"\n", result[0].c_str());
    return false;
  }
  if (result.size() > 2 && result[2] != "{") {
    LogERROR("Syntax error, expect \"{\" at line end\n");
    return false;
  }
  if (result.size() == 2 && result[1][result[1].length()-1] != '{') {
    LogERROR("Syntax error, expect \"{\" at line end\n");
    return false;
  }

  // Get service name.
  std::string service_name = result[1];
  if (result.size() == 2) {
    service_name = service_name.substr(0, service_name.length() - 1);
  }
  if (!IsValidVariableName(service_name)) {
    LogERROR("invalid enum name \"%s\"", service_name.c_str());
    return false;
  }
  // Check name duplication
  const std::string& full_service_name =
      PbType::GeneratePackagePrefix(PYTHON, pkg_stack_) + service_name;
  if (services_map_.find(full_service_name) != services_map_.end()) {
    LogERROR("service name \"%s\" already exists", full_service_name.c_str());
    return false;
  }

  // Add new service to the services map.
  std::shared_ptr<ServiceType> new_service;
  new_service.reset(new ServiceType(service_name, current_package_));
  services_map_[full_service_name] = new_service;
  current_service_ = new_service.get();
  return true;
}

bool Parser::ParseRpcName(std::string line) {
  std::vector<std::string> rpc_params =
      Strings::ExtractTokens(&line, '(', ')');
  if (rpc_params.size() != 2) {
    LogERROR("Expect (rpc arg) and (rpc return) to be defined");
    return false;
  }

  auto result = Strings::Split(line, ' ', Strings::SkipWhiteSpace());
  if (result.size() != 4) {
    LogERROR("Expect 4 least tokens, actual %d", result.size());
    return false;
  }
  if (result[2] != "returns") {
    LogERROR("Unknown keyword \"%s\"\n", result[0].c_str());
    return false;
  }
  if (result[3] != "{") {
    LogERROR("Syntax error, expect \"{\" at line end\n");
    return false;
  }
  const std::string& rpc_name = result[1];
  std::shared_ptr<RpcMethod> new_rpc(new RpcMethod(rpc_name));
  // Parse and check all rpc arg type.
  auto args = Strings::Split(
      Strings::Strip(rpc_params[0], "()"), ',', Strings::SkipWhiteSpace());
  for (auto& token: args) {
    token = Strings::Strip(token);
    PbType* type_class = nullptr;
    FIELD_TYPE type;
    if ((type = PbCommon::GetMessageFieldType(token)) == UNDETERMINED) {
      type_class = FindParsedMessageOrEnumType(token);
      if (!type_class) {
        LogERROR("Unknown field type \"%s\"", token.c_str());
        return false;
      }
      new_rpc->AddArg(token, type_class);
    }
    else if (IsValidPrimitiveTypeName(token)) {
      new_rpc->AddArg(token);
    }
    else {
      LogERROR("Unknown rpc arg type %s\n", token.c_str());
      return false;
    }
  }

  // Parse and check all rpc return type.
  auto returns = Strings::Split(
      Strings::Strip(rpc_params[1], "()"), ',', Strings::SkipWhiteSpace());
  for (auto& token: returns) {
    token = Strings::Strip(token);
    PbType* type_class = nullptr;
    FIELD_TYPE type;
    if ((type = PbCommon::GetMessageFieldType(token)) == UNDETERMINED) {
      type_class = FindParsedMessageOrEnumType(token);
      if (!type_class) {
        LogERROR("Unknown field type \"%s\"", token.c_str());
        return false;
      }
      new_rpc->AddReturn(token, type_class);
    }
    else if (IsValidPrimitiveTypeName(token)) {
      new_rpc->AddReturn(token);
    }
    else {
      LogERROR("Unknown rpc arg type %s\n", token.c_str());
      return false;
    }
  }

  // Add new rpc to current service class.
  current_rpc_ = new_rpc.get();
  current_service_->AddRpcMethod(new_rpc);

  return true;
}

bool Parser::ParseRpcOption(std::string line) {
  if (line[line.length()-1] != ';') {
    LogERROR("Expect \";\" at line end");
    return false;
  }
  line = Strings::Strip(line, "option ;");
  int index = Strings::FindFirstMatch(line, "=");
  if (index < 0) {
    LogERROR("Invalid rpc option %s, expect assignement with \'=\'",
             line.c_str());
    return false;
  }
  std::string key = Strings::Strip(line.substr(0, index));
  std::string value = Strings::Strip(line.substr(index + 1));
  current_rpc_->AddOption(key, value);
  
  return true;
}

bool Parser::ParseEnumValue(std::string line) {
  if (line[line.length()-1] != ',') {
    LogERROR("Expect \",\" at line end");
    return false;
  }
  std::string name = line.substr(0, line.length() - 1);
  if (!IsValidVariableName(name)) {
    LogERROR("invalid enum field name \"%s\"", name.c_str());
    return false;
  }
  current_enum_->AddEnumValue(name);
  return true;
}

bool Parser::IsMessageFiledLine(std::string line) {
  return Strings::StartWith(line, "optional ") ||
         Strings::StartWith(line, "required ") ||
         Strings::StartWith(line, "repeated ");
}

bool Parser::ParseAssignExpression(std::string line,
                                   std::string* left,
                                   std::string* right,
                                   FIELD_TYPE type) const {
  line = Strings::Strip(line);
  std::size_t pos = line.find("=");
  if (pos == std::string::npos) {
    LogERROR("Expect \"variable = value\" but actual \"%s\"", line.c_str());
    return false;
  }
  *left = Strings::Strip(line.substr(0, pos));
  *right = Strings::Strip(line.substr(pos + 1));
  if ((*left).length() == 0 || !IsValidVariableName(*left)) {
    LogERROR("invalid variable name \"%s\"", (*left).c_str());
    return false;
  }
  if ((*right).length() == 0) {
    LogERROR("value of %s must not be empty", (*left).c_str());
    return false;
  }
  switch (type) {
    case INT32:
      try {
        int value = std::stoi(*right);
        (void)value;
      }
      catch (std::exception& err) {
        LogERROR("Can't parse \"%s\" as int32 value", (*right).c_str());
        return false;
      }
      break;
    case UINT32:
      try {
        unsigned long value = std::stoul(*right);
        (void)value;
      }
      catch (std::exception& err) {
        LogERROR("Can't parse \"%s\" as uint32 value", (*right).c_str());
        return false;
      }
      break;
    case INT64:
      try {
        long long value = std::stoll(*right);
        (void)value;
      }
      catch (std::exception& err) {
        LogERROR("Can't parse \"%s\" as int64 value", (*right).c_str());
        return false;
      }
      break;
    case UINT64:
      try {
        unsigned long long value = std::stoull(*right);
        (void)value;
      }
      catch (std::exception& err) {
        LogERROR("Can't parse %s as int64 value", (*right).c_str());
        return false;
      }
      break;
    case DOUBLE:
      try {
        double value = std::stod(*right);
        (void)value;
      }
      catch (std::exception& err) {
        LogERROR("Can't parse \"%s\" as double value", (*right).c_str());
        return false;
      }
      break;
    case BOOL:
      if (*right != "true" && *right != "false" &&
          *right != "True" && *right != "False") {
        LogERROR("Invalid boolean value \"%s\"", (*right).c_str());
        return false;
      }
      break;
    case CHAR:
      {
        std::string value = *right;
        if (!Strings::StartWith(value, "'") ||
            !Strings::EndWith(value, "'")) {
          LogERROR(
              "Invalid char value %s : must be quotated with ''",
              (*right).c_str());
          return false;
        }
        *right = Strings::Strip(*right, "'");
      }
      break;  
    case STRING:
      {
        std::string value = *right;
        if (!Strings::StartWith(value, "\"") ||
            !Strings::EndWith(value, "\"")) {
          LogERROR(
              "Invalid string value %s : must be double quotated",
              (*right).c_str());
          return false;
        }
        *right = Strings::Strip(*right, "\"");
      }
      break;
    default:
      break;
  }

  return true;
}

bool Parser::ParseProto() {
  if (!Do_ParseProto()) {
    LogERROR("Parse proto failed");
    return false;
  }
  //PrintParsedProto();
  return true;
}

void Parser::PrintParsedProto() const {
  for (auto& message : messages_list_) {
    message->Print();
    std::cout << std::endl << std::endl;
  }
  for (auto& e : enums_map_) {
    e.second->Print();
    std::cout << std::endl << std::endl;
  }
  for (auto& e : services_map_) {
    e.second->Print();
    std::cout << std::endl << std::endl;
  }
}

bool Parser::IsValidVariableName(std::string str) {
  if (str.length() == 0) {
    return false;
  }
  for (unsigned int i = 0; i < str.length(); i++) {
    if (!Strings::IsLetterOrDigitOrUnderScore(str[i])) {
      return false;
    }
  }
  return true;
}

bool Parser::IsValidPrimitiveTypeName(std::string str) {
  if (str.length() == 0) {
    return false;
  }
  return str == "int32" || str == "int64" ||
         str == "uint32" || str == "uint64" ||
         str == "bool" || str == "double" ||
         str == "string" || str == "char";
}

PbType* Parser::FindParsedMessageOrEnumType(std::string type_name) const {
  const std::string& as_global_name = type_name;
  const std::string& as_nested_name =
      current_package_ + "." + type_name;
  const std::string& as_parallel_name =
      current_message_->PackagePrefix(PYTHON) + type_name;

  // std::cout << as_nested_name << std::endl;
  // std::cout << as_parallel_name << std::endl;
  // std::cout << as_global_name << std::endl;
  // Search name as a nested type

  PbType* type_class = nullptr;
  if (current_message_->FindEnumType(as_nested_name)) {
    type_class = static_cast<PbType*>(
        current_message_->FindEnumType(as_nested_name));
  }
  else if (current_message_->FindEnumType(as_parallel_name)) {
    type_class = static_cast<PbType*>(
        current_message_->FindEnumType(as_parallel_name));
  }
  else if (current_message_->FindEnumType(as_global_name)) {
    type_class = static_cast<PbType*>(
        current_message_->FindEnumType(as_global_name));
  }
  // Search name in the same package this message belongs to.
  else if (enums_map_.find(as_nested_name) != enums_map_.end()) {
    type_class = static_cast<PbType*>(enums_map_.at(as_nested_name).get());
  }
  else if (enums_map_.find(as_parallel_name) != enums_map_.end()) {
    type_class = static_cast<PbType*>(enums_map_.at(as_parallel_name).get());
  }
  else if (enums_map_.find(as_global_name) != enums_map_.end()) {
    type_class = static_cast<PbType*>(enums_map_.at(as_global_name).get());
  }
  // Search in global namespace.
  else if (messages_map_.find(as_nested_name) != messages_map_.end()) {
    type_class = static_cast<PbType*>(
        messages_map_.at(as_nested_name).get());
  }
  else if (messages_map_.find(as_parallel_name) != messages_map_.end()) {
    type_class = static_cast<PbType*>(
        messages_map_.at(as_parallel_name).get());
  }
  else if (messages_map_.find(as_global_name) != messages_map_.end()) {
    type_class = static_cast<PbType*>(
        messages_map_.at(as_global_name).get());
  }

  else {
    return nullptr;
  }
  return type_class;
}

LANGUAGE Parser::GetLanguageFromString(std::string lang) {
  if (lang == "cpp") {
    return CPP;
  }
  if (lang == "python") {
    return PYTHON;
  }
  if (lang == "java") {
    return JAVA;
  }
  return UNKNOWN_LANGUAGE;
}

void Parser::PrintToken(std::string description, std::string str) {
  std::cout << "[" << description << "] = \"" << str << "\"" << std::endl;
}

void Parser::PrintParseState() const {
  switch (state_) {
    case GLOBAL:
      std::cout << "State: Global" << std::endl;
      break;
    case PARSINGMSG:
      std::cout << "State: Parsing Messsage" << std::endl;
      break;
    case PARSINGENUM:
      std::cout << "State: Parsing Enum" << std::endl;
      break;
    case PARSINGNESTEDENUM:
      std::cout << "State: Parsing Nested Enum" << std::endl;
      break;
    default:
      break;
  }
}

std::vector<std::shared_ptr<Message>>& Parser::mutable_messages_list() {
  return messages_list_;
}

}  // namespace ProtoParser
}  // namespace Proto
