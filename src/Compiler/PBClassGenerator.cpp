#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "PBClassGenerator.h"
#include "../IO/FileDescriptor.h"
#include "../Utility/BufferedDataReader.h"
#include "../Utility/BufferedDataWriter.h"
#include "../Utility/Strings.h"
#include "../Utility/Utils.h"

namespace PandaProto {
namespace Compiler {

PBClassGenerator::PBClassGenerator(LANGUAGE lang, std::string file) :
    lang_(lang),
    proto_file_(file) {
  if (!StringUtils::EndWith(proto_file_, ".proto")) {
    init_success_ = false;
    fprintf(stderr, "ERROR: proto file name must have \".proto\" postfix.\n");
    return;
  }
  init_success_ = true;
}

PBClassGenerator::~PBClassGenerator() {}

bool PBClassGenerator::ReadProtoFile() {
  if (!init_success_) {
    return false;
  }

  std::unique_ptr<IO::FileDescriptor> fd(
      new IO::FileDescriptor(proto_file_, IO::FileDescriptor::READ_ONLY));
  if (fd->closed()) {
    return false;
  }
  Utility::BufferedDataReader br(std::move(fd));

  // Read each line and parse in a finite state machine.
  std::string line;
  while (br.ReadLine(&line)) {
    line_number_++;
    line = StringUtils::Strip(line);
    std::cout << "--------------------------------------------" << std::endl;
    PrintParseState();
    std::cout << "Parsing: \"" << line << "\"\n" << std::endl;

    // Skip empty and comment lines.
    if (line.length() == 0 || StringUtils::StartWith(line, "//")) {
      continue;
    }
    // remove comment in the line.
    std::size_t pos;
    if ((pos = line.find("//")) != std::string::npos) {
      line = StringUtils::Strip(line.substr(0, pos));
    }

    // Parse in Global state: Only accept line start with "package ",
    // "message ", "enum ".
    if (state_ == GLOBAL) {
      if (StringUtils::StartWith(line, "package ")) {
        if (!ParsePackageName(line)) {
          return false;
        }
      }
      // Parse message.
      else if (StringUtils::StartWith(line, "message ")) {
        if (!ParseMessageName(line)) {
          return false;
        }
        state_ = PARSINGMSG;
      }
      else if (StringUtils::StartWith(line, "enum ")) {
        if (!ParseEnumName(line)) {
          return false;
        }
        state_ = PARSINGENUM;
      }
      else {
        LogError("Illegal line in global");
        return false;
      }
    }

    // Parse a message state.
    else if (state_ == PARSINGMSG) {
      // Parse message field.
      if (StringUtils::StartWith(line, "enum ")) {
        if (!ParseEnumName(line)) {
          return false;
        }
        state_ = PARSINGNESTEDENUM;
      }
      else if (line == "}") {
        state_ = GLOBAL;
      }
      else if (IsMessageFiledLine(line)) {
        if (!ParseMessageField(line)) {
          return false;
        }
      }
      else {
        LogError("Illegal line in Parsing a message");
        return false;
      }
    }

    // Parse a enum state.
    else if (state_ == PARSINGENUM) {
      if (line == "}") {
        state_ = GLOBAL;
      }
      else if (!ParseEnumValue(line)) {
        return false;
      }
    }

    // Parse a nested enum state.
    else if (state_ == PARSINGNESTEDENUM) {
      if (line == "}") {
        state_ = PARSINGMSG;
      }
      else if (!ParseEnumValue(line)) {
        return false;
      }
    }

    // Syntax Error
    else {
      LogError("Illegal line, can't parse");
      return false;
    }
  }
  return true;
}

bool PBClassGenerator::ParsePackageName(std::string line) {
  if (line[line.length()-1] != ';') {
    LogError("Expect \";\" at line end");
    return false;
  }
  line = StringUtils::Strip(line, ";");
  std::vector<std::string> result = StringUtils::SplitGreedy(line, ' ');
  if (result.size() != 2) {
    LogError("Expect 2 tokens, actual %d", result.size());
    return false;
  }
  if (result[0] != "package") {
    LogError("Unknown key word", result[0].c_str());
    return false;
  }
  current_package_ = result[1];
  result = StringUtils::Split(current_package_, '.');
  pkg_stack_.clear();
  for (auto& pkg: result) {
    if (!IsValidVariableName(pkg)) {
      LogError("Invalid package name %s in %s",
               pkg.c_str(), current_package_.c_str());
      return false;
    }
  }
  return true;
}

bool PBClassGenerator::ParseMessageName(std::string line) {
  std::vector<std::string> result = StringUtils::SplitGreedy(line, ' ');
  if (result.size() != 2 && result.size() != 3) {
    LogError("Expect 2 or 3 tokens, actual %d", result.size());
    return false;
  }
  if (result[0] != "message") {
    LogError("Unknown keyword \"%s\"\n", result[0].c_str());
    return false;
  }
  if (result.size() > 2 && result[2] != "{") {
    LogError("Syntax error, expect \"{\" at line end\n");
    return false;
  }
  if (result.size() == 2 && result[1][result[1].length()-1] != '{') {
    LogError("Syntax error, expect \"{\" at line end\n");
    return false;
  }

  // Get message name.
  std::string message_name = result[1];
  if (result.size() == 2) {
    message_name = message_name.substr(0, message_name.length() - 1);
  }
  if (!IsValidVariableName(message_name)) {
    LogError("invalid message name \"%s\"", message_name.c_str());
    return false;
  }

  // Add new message to message map.
  std::shared_ptr<Message> new_message(new Message(message_name,
                                                   current_package_));
  messages_list_.push_back(new_message);
  messages_map_[message_name] = new_message;
  current_message_ = new_message.get();
  return true;
}

bool PBClassGenerator::ParseMessageField(std::string line) {
  if (line[line.length()-1] != ';') {
    LogError("Expect \";\" at line end");
    return false;
  }
  line = StringUtils::Strip(line, ";");
  std::vector<std::string> result = StringUtils::SplitGreedy(line, ' ');

  if (result.size() < 3) {
    LogError("Syntax error");
    return false;
  }
  // Parse field modifier.
  MessageField::FIELD_MODIFIER modifier;
  if ((modifier = MessageField::GetMessageFieldModifier(result[0])) ==
      MessageField::UNKNOWN_MODIFIER) {
    LogError("Unknown modifier \"%s\"", result[0].c_str());
    return false;
  }
  // Parse field type.
  std::string type_name = result[1];
  MessageField::FIELD_TYPE type;
  if ((type = MessageField::GetMessageFieldType(type_name)) ==
      MessageField::UNDETERMINED) {
    if (messages_map_.find(type_name) != messages_map_.end() ||
        current_message_->FindEnumType(type_name)) {
      type = MessageField::MESSAGETYPE;
    }
    else if (enums_map_.find(type_name) != enums_map_.end()) {
      type = MessageField::ENUMTYPE;
    }
    else {
      LogError("Unknown field type \"%s\"", type_name.c_str());
      return false;
    }
  }

  // Parse name and tag.
  std::string remain;
  for (unsigned int i = 2; i < result.size(); i++) {
    remain += (result[i] + " ");
  }

  std::string nametag = remain, defaultblock = "";
  std::size_t pos = remain.find("[");
  if (pos != std::string::npos) {
    nametag = StringUtils::Strip(remain.substr(0, pos));
    defaultblock = StringUtils::Strip(remain.substr(pos));
    if (defaultblock[defaultblock.length()-1] != ']') {
      LogError("Expect \"]\" after default assignement");
      return false;
    }
    defaultblock = StringUtils::Strip(defaultblock, "[]");   
  }

  std::string name, tag;
  if (!ParseAssignExpression(nametag, &name, &tag)) {
    return false;
  }
  int tag_num = std::stoi(tag);

  // Parse default block.
  std::string default_name, default_value = "";
  if (defaultblock.length() > 0) {
    if (!ParseAssignExpression(defaultblock, &default_name, &default_value)) {
      return false;
    }
    if (default_name != "default") {
      LogError("Can't recognize \"%s\", should be \"default\"",
               default_name.c_str());
      return false;
    }
  }

  // Add new field to current message.
  std::shared_ptr<MessageField> new_field(
      new MessageField(modifier, type, type_name, name, tag_num,
                       default_value));
  if (!current_message_->AddField(new_field)) {
    LogError("Add field \"%s\" to message \"%s\" failed",
             name.c_str(), current_message_->name().c_str());
    return false;
  }
  return true;
}

bool PBClassGenerator::ParseEnumName(std::string line) {
  std::vector<std::string> result = StringUtils::SplitGreedy(line, ' ');
  if (result.size() != 2 && result.size() != 3) {
    LogError("Expect 2 or 3 least tokens, actual %d", result.size());
    return false;
  }
  if (result[0] != "enum") {
    LogError("Unknown keyword \"%s\"\n", result[0].c_str());
    return false;
  }
  if (result.size() > 2 && result[2] != "{") {
    LogError("Syntax error, expect \"{\" at line end\n");
    return false;
  }
  if (result.size() == 2 && result[1][result[1].length()-1] != '{') {
    LogError("Syntax error, expect \"{\" at line end\n");
    return false;
  }

  // Get enum name.
  std::string enum_name = result[1];
  if (result.size() == 2) {
    enum_name = enum_name.substr(0, enum_name.length() - 1);
  }
  if (!IsValidVariableName(enum_name)) {
    LogError("invalid enum name \"%s\"", enum_name.c_str());
    return false;
  }

  // Add new enum to the enum map
  std::shared_ptr<EnumType> new_enum;
  if (state_ == GLOBAL) {
    new_enum.reset(new EnumType(enum_name, current_package_));
    enums_map_[enum_name] = new_enum;
    currentEnum_ = new_enum.get();
  }
  else if (state_ == PARSINGMSG) {
    new_enum.reset(new EnumType(enum_name, current_package_,
                                current_message_->name()));
    current_message_->AddEnum(new_enum);
    currentEnum_ = new_enum.get();
  }

  return true;
}

bool PBClassGenerator::ParseEnumValue(std::string line) {
  if (line[line.length()-1] != ',') {
    LogError("Expect \",\" at line end");
    return false;
  }
  std::string name = line.substr(0, line.length() - 1);
  if (!IsValidVariableName(name)) {
    LogError("invalid enum field name \"%s\"", name.c_str());
    return false;
  }
  currentEnum_->AddEnumValue(name);
  return true;
}

bool PBClassGenerator::IsMessageFiledLine(std::string line) {
  return StringUtils::StartWith(line, "optional ") ||
         StringUtils::StartWith(line, "required ") ||
         StringUtils::StartWith(line, "repeated ");
}

bool PBClassGenerator::ParseAssignExpression(std::string line,
                                             std::string* left,
                                             std::string* right) const {
  line = StringUtils::Strip(line);
  std::size_t pos = line.find("=");
  if (pos == std::string::npos) {
    LogError("Expect \"variable = value\" but actual \"%s\"", line.c_str());
    return false;
  }
  *left = StringUtils::Strip(line.substr(0, pos));
  *right = StringUtils::Strip(line.substr(pos + 1));
  if ((*left).length() == 0 || !IsValidVariableName(*left)) {
    LogError("invalid variable name \"%s\"", (*left).c_str());
    return false;
  }
  if ((*right).length() == 0 || !IsValidVariableName(*right)) {
    LogError("invalid variable name \"%s\"", (*right).c_str());
    return false;
  }
  return true;
}

bool PBClassGenerator::GeneratePBClass() {
  if (!ReadProtoFile()) {
    fprintf(stderr, "ERROR: Can't parse proto %s\n", proto_file_.c_str());
    return false;
  }
  PrintParsedProto();
  
  switch (lang_) {
    case CPP:
      GenerateCppCode();
      break;
    default: break;
  }
  return true;
}

void PBClassGenerator::PrintParsedProto() const {
  for (auto& message : messages_list_) {
    message->Print();
    std::cout << std::endl << std::endl;
  }
  for (auto& e : enums_map_) {
    e.second->Print();
    std::cout << std::endl << std::endl;
  }
}

bool PBClassGenerator::IsValidVariableName(std::string str) {
  if (str.length() == 0) {
    return false;
  }
  for (unsigned int i = 0; i < str.length(); i++) {
    if (!StringUtils::IsLetterOrDigitOrUnderScore(str[i])) {
      return false;
    }
  }
  return true;
}

PBClassGenerator::LANGUAGE
PBClassGenerator::GetLanguageFromString(std::string lang) {
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

void PBClassGenerator::PrintToken(std::string description, std::string str) {
  std::cout << "[" << description << "] = \"" << str << "\"" << std::endl;
}

void PBClassGenerator::LogError(const char* error_msg, ...) const {
  fprintf(stderr, "%s:%d: ", proto_file_.c_str(), line_number_);
  va_list args;
  va_start(args, error_msg);
  vfprintf(stderr, error_msg, args);
  va_end(args);
  fprintf(stderr, "\n");
}

void PBClassGenerator::PrintParseState() const {
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

void PBClassGenerator::GenerateCppCode() {
  std::string outfile;
  outfile = proto_file_.substr(0, proto_file_.length() - 6) + "_pb";

  // Generate .h file
  if (!printer.Open(outfile + ".h")) {
    fprintf(stderr, "ERROR: Open output file %s.h failed\n", outfile.c_str());
  }

  std::vector<std::string> result = StringUtils::Split(outfile, '/');
  std::string filename = result[result.size() - 1];

  printer.Print("#ifndef _" + StringUtils::Upper(filename) + "_H\n");
  printer.Print("#define _" + StringUtils::Upper(filename) + "_H\n\n");
  printer.Print("#include <string>\n");
  printer.Print("#include <vector>\n\n");

  std::vector<std::string> pkg_stack;
  // Print global enums.
  for (auto& e: enums_map_) {
    EnumType* enum_p = e.second.get();
    CheckoutNameSpace(pkg_stack_, enum_p->pkg_stack());
    printer.Print("enum " + enum_p->name() + " {\n");
    for (auto& enumvalue: enum_p->enums()) {
      printer.Print("  " + enumvalue + ",\n");
    }
    printer.Print("};\n\n");
  }

  std::map<MessageField::FIELD_TYPE, std::string> pbCppTypeMap{
    {MessageField::INT32, "int"},
    {MessageField::INT64, "long long"},
    {MessageField::UINT32, "unsigned int"},
    {MessageField::UINT64, "unsigned long long"},
    {MessageField::DOUBLE, "dobule"},
    {MessageField::STRING, "std::string"},
    {MessageField::BOOL, "bool"},
  };
  // Print classes.
  for (auto& message: messages_list_) {
    CheckoutNameSpace(pkg_stack_, message->pkg_stack());
    printer.Print("class " + message->name() + " {\n");

    // Public fields.
    printer.Print(" public:\n");
    // Print enums declearation.
    for (auto& e: message->enums_map()) {
      EnumType* enum_p = e.second.get();
      printer.Print("  enum " + enum_p->name() + " {\n");
      for (auto& enumvalue: enum_p->enums()) {
        printer.Print("    " + enumvalue + ",\n");
      }
      printer.Print("  };\n\n");
    }

    // Print constructors and destructor.
    std::map<std::string, std::string> msg_match{
       {"msg_name", message->name()},
    };
    printer.Print("  // constructors and destructor\n");
    printer.Print("  " + message->name() + "() = default;\n");
    printer.Print("  ~" + message->name() + "();\n");
    printer.Print(
        "  {msg_name}(const {msg_name}& other);  // copy constructor\n",
        msg_match);
    printer.Print(
        "  {msg_name}({msg_name}&& other);  // move constructor\n",
        msg_match);
    printer.Print(
        "  {msg_name}& operator=(const {msg_name}& other);"
        "  // copy assignment\n",
        msg_match);
    printer.Print(
        "  {msg_name}& operator=({msg_name}&& other);  // move assignment\n",
        msg_match);
    printer.Print("  void Swap({msg_name}* other);\n", msg_match);
    printer.Print("\n");
    
    // Print public methods.
    for (auto& field: message->fields_list()) {
      std::string type_name = field->type_name();
      if (field->type() != MessageField::ENUMTYPE &&
          field->type() != MessageField::MESSAGETYPE) {
        type_name = pbCppTypeMap.at(field->type());
      }
      printer.Print("  // Access \"" + field->name() + "\"\n");
      
      // methods for required and optional fields
      if (field->modifier() != MessageField::REPEATED) {
        // get method and set method
        std::string get_method_line = "  {type} {field_name}() const;\n";
        std::string set_method_line =
              "  void set_{field_name}(const {type} {field_name});\n";
        if (field->type() == MessageField::STRING) {
          get_method_line = "  const {type}& {field_name}() const;\n";
          set_method_line =
              "  void set_{field_name}(const {type}& {field_name});\n";
        }
        std::map<std::string, std::string> matches{
           {"field_name", field->name()},
           {"type", type_name},
        };

        // define: int get_foo() const / const Bar& get_foo()
        printer.Print(get_method_line, matches);
        // define: void set_foo(int foo) / void set_foo(const Bar& foo)
        printer.Print(set_method_line, matches);
        
        // string type has other methods.
        if (field->type() == MessageField::STRING) {
          // define: const set_foo(char* value)
          printer.Print(
              "  void set_{field_name}(const char* {field_name});\n",
              matches);
          // define: const set_foo(char* value, int size)
          printer.Print(
              "  void set_{field_name}(const char* {field_name}, int size);\n",
              matches);
          // define: string* mutable_foo()
          printer.Print(
              "  std::string* mutable_{field_name}();\n", matches);
          // define: void clear_foo()
          printer.Print(
              "  void clear_{field_name}();\n", matches);
        }
        // message type has other methods.
        if (field->type() == MessageField::MESSAGETYPE &&
            field->modifier() != MessageField::REPEATED) {
          // define: Bar* mutable_foo()
          printer.Print("  {type}* mutable_{field_name}();\n", matches);
          // define: void set_allocated_foo(Bar* foo)
          printer.Print(
              "  void set_allocated_{field_name}({type}* {field_name});\n",
              matches);
          // define: Bar* release_foo()
          printer.Print("  {type}* release_{field_name}();\n", matches);
        }
      }
      // methods of repeated field.
      else {
        // get method and set method
        std::map<std::string, std::string> matches{
           {"field_name", field->name()},
           {"type", type_name},
        };
        // define: int foo_size() const
        printer.Print("  int {field_name}_size() const;\n", matches);
        if (field->type() != MessageField::MESSAGETYPE) {
          // define: Bar foo(int index) const
          printer.Print("  {type} {field_name}(int index) const;\n", matches);
          // define: void set_foo(int index, Bar& foo)
          printer.Print(
              "  void set_{field_name}(int index, {type} {field_name});\n",
              matches);
          // define: void add_foo(Bar& foo)
          printer.Print(
              "  void add_{field_name}({type} {field_name});\n", matches);
        }
        else {
          // define: const Bar& foo(int index) const
          printer.Print("  const {type}& {field_name}(int index) const;\n",
                        matches);
          // define: void set_foo(int index, Bar& foo)
          printer.Print(
            "  void set_{field_name}(int index, const {type}& {field_name});\n",
            matches);
          // define: void add_foo(Bar& foo)
          printer.Print(
              "  void add_{field_name}(const {type}& {field_name});\n",
              matches);
        } 
        // define: const std::vector<Bar> foo() const
        printer.Print(
            "  const std::vector<{type}>& {field_name}() const;\n", matches);
        // define: std::vector<Bar>* mutable_foo()
        printer.Print(
            "  std::vector<{type}>* mutable_{field_name}();\n", matches);
      }

      printer.Print("\n");
    }

    // Private fields.
    printer.Print(" private:\n");
    for (auto& field: message->fields_list()) {
      std::string type_name = field->type_name();
      if (field->type() != MessageField::ENUMTYPE &&
          field->type() != MessageField::MESSAGETYPE) {
        type_name = pbCppTypeMap.at(field->type());
      }
      if (field->modifier() == MessageField::REPEATED) {
        type_name = "std::vector<" + type_name + ">";
      }

      std::string declearation_line = "  $ $_";
      if (field->type() == MessageField::MESSAGETYPE &&
          field->modifier() != MessageField::REPEATED) {
        declearation_line = "  $* $_";
      }
      printer.Print(declearation_line,
                    std::vector<std::string>{type_name, field->name()});
      if (!field->default_value().empty()) {
        printer.Print(" = " + field->default_value());
      }
      printer.Print(";\n");
    }
    printer.Print("};\n\n");
  }

  CheckoutNameSpace(pkg_stack_, std::vector<std::string>());
  printer.Print("\n#endif  /* _" + StringUtils::Upper(filename) + "_H */\n");
  printer.Flush();
}

void PBClassGenerator::CheckoutNameSpace(
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

}  // Compiler
}  // PandaProto