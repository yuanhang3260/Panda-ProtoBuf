#include "PBClassGenerator.h"

#include "../IO/FileDescriptor.h"
#include "../Utility/BufferedDataReader.h"
#include "../Utility/Strings.h"

namespace PandaProto {
namespace Compiler {

bool PBClassGenerator::ReadProtoFile(std::string proto_file) {
  std::unique_ptr<IO::FileDescriptor> fd(
      new IO::FileDescriptor(proto_file, IO::FileDescriptor::READ_ONLY));
  Utility::BufferedDataReader br(std::move(fd));

  std::string line;
  while (br.ReadLine(&line)) {
    line = StringUtils::Strip(line);
    // Skip empty and comment lines.
    if (line.length() == 0 || StringUtils::StartWith(line, "//")) {
      continue;
    }
    // remove comment in the line.
    unsigned int pos;
    if ((pos = line.find("//")) != std::string::npos) {
      line = StringUtils::Strip(line.substr(0, pos));
    }

    // Parse package name.
    if (StringUtils::StartWith(line, "package") && !ParsePackageName(line)) {
      return false;
    }
    // Parse message.
    if (StringUtils::StartWith(line, "message") && !ParseMessageName(line)) {
      return false;
    }
    // Parse message field.
    if (IsMessageFiledLine(line) && !ParseMessageField(line)) {
      return false;
    }
  }
  return true;
}

bool PBClassGenerator::ParsePackageName(std::string line) {
  std::vector<std::string> result = StringUtils::SplitGreedy(line, ' ');
  if (result.size() != 2) {
    fprintf(stderr,
            "ERROR: Syntax error in package line - "
            "more than one packet name\n");
    return false;
  }
  if (result[0] != "package") {
    fprintf(stderr,
            "ERROR: Syntax error in package line, unknown keyword %s\n",
            result[0].c_str());
    return false;
  }
  current_package_ = result[1];
  return true;
}

bool PBClassGenerator::ParseMessageName(std::string line) {
  std::vector<std::string> result = StringUtils::SplitGreedy(line, ' ');
  if (result.size() != 3) {
    fprintf(stderr, "ERROR: Syntax error in message name line\n");
    return false;
  }
  if (result[0] != "message") {
    fprintf(stderr, "ERROR: Syntax error in message name line - "
                    "unknown keyword \"%s\"\n", result[0].c_str());
    return false;
  }
  if (result[2] != "{") {
    fprintf(stderr, "ERROR: Syntax error in message name line - "
                    "Expect \"{\" at line end\n");
    return false;
  }
  std::string message_name = result[1];
  std::shared_ptr<Message> new_message(new Message(message_name,
                                                   current_package_));
  messages_list_.push_back(new_message);
  messages_map_[message_name] = new_message; 
  return true;
}

bool PBClassGenerator::ParseMessageField(std::string line) {
  if (line[line.length()-1] != ';') {
    fprintf(stderr, "ERROR: Syntax error in message field line - "
                    "Expect \";\" at end\n.");
  }
  line = StringUtils::Strip(line, ";");
  std::vector<std::string> result = StringUtils::SplitGreedy(line, ' ');

  if (result.size() < 3) {
    fprintf(stderr, "ERROR: Syntax error in message field line\n");
    return false;
  }
  // Parse field modifier.
  MessageField::FIELD_MODIFIER modifier;
  if ((modifier = MessageField::GetMessageFieldModifier(result[0])) ==
      MessageField::UNKNOWN_MODIFIER) {
    fprintf(stderr, "ERROR: Syntax error in message field line - "
                    "unknown modifier \"%s\"\n", result[0].c_str());
    return false;
  }
  // Parse field type.
  MessageField::FIELD_TYPE type;
  if ((type = MessageField::GetMessageFieldType(result[1])) ==
      MessageField::UNDETERMINED) {
    fprintf(stderr, "ERROR: Syntax error in message field line - "
                    "unknown field type \"%s\"\n", result[1].c_str());
    return false;
  }

  // Parse name and tag.
  std::string nametag = result[2], defaultblock = "";
  unsigned int pos = nametag.find("[");
  if (pos != std::string::npos) {
    nametag = StringUtils::Strip(nametag.substr(0, pos));
    defaultblock = StringUtils::Strip(nametag.substr(pos));
    if (defaultblock[defaultblock.length()-1] != ']') {
      fprintf(stderr, "ERROR: Syntax error in message field line - "
                      "illegal default assign: \"%s\"\n", defaultblock.c_str());
      return false;
    }
  }
  std::string name, tag;
  if (!ParseAssignExpression(nametag, &name, &tag)) {
    return false;
  }
  int tag_num = std::stoi(tag);
  std::string default_name, default_value = "";
  if (defaultblock.length() > 0 &&
      !ParseAssignExpression(defaultblock, &default_name, &default_value) &&
      default_name != "default") {
    return false;
  }

  std::shared_ptr<MessageField> new_field(
      new MessageField(modifier, type, name, tag_num, default_value));
  if (!CurrentMessage()->AddField(new_field)) {
    return false;
  }
  return true;
}

bool PBClassGenerator::IsMessageFiledLine(std::string line) {
  return StringUtils::StartWith(line, "optional") ||
         StringUtils::StartWith(line, "required") ||
         StringUtils::StartWith(line, "repeated");
}

bool PBClassGenerator::ParseAssignExpression(std::string line,
                                             std::string* left,
                                             std::string* right) const {
  unsigned int pos = line.find("=");
  if (pos == std::string::npos) {
    fprintf(stderr, "ERROR: Syntax error in \"name = tag\" assignement - "
                    "Expect a \"=\" but actual \"%s\"\n",
                    line.c_str());
    return false;
  }
  *left = StringUtils::Strip(line.substr(0, pos));
  *right = StringUtils::Strip(line.substr(pos + 1));
  if (!IsValidVariableName(*left) || !IsValidVariableName(*right)) {
    return false;
  }
  return true;
}

Message* PBClassGenerator::CurrentMessage() const {
  return messages_list_[messages_list_.size()-1].get();
}

bool PBClassGenerator::IsValidVariableName(std::string str) {
  for (unsigned int i = 0; i < str.length(); i++) {
    if (!StringUtils::IsLetterOrDigitOrUnderScore(str[i])) {
      return false;
    }
  }
  return true;
}

}  // Compiler
}  // PandaProto