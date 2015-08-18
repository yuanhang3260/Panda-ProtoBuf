#ifndef PROTO_PARSER_
#define PROTO_PARSER_

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Type.h"
#include "Message.h"

namespace proto {
namespace ProtoParser {

class CppGeneratorImpl;

class ProtoParser {
 public:
  enum LANGUAGE {
    CPP,
    PYTHON,
    JAVA,
    UNKNOWN_LANGUAGE,
  };

  enum ParseState {
    GLOBAL,
    PARSINGMSG,
    PARSINGENUM,
    PARSINGNESTEDENUM,
  };

  ProtoParser(LANGUAGE lang, std::string file);
  virtual ~ProtoParser();
  ProtoParser(const ProtoParser&) = delete;
  ProtoParser& operator=(const ProtoParser&) = delete;

  bool ParseProto();
  void PrintParsedProto() const;
  virtual void GenerateCode() {}  // Need language-specific implementation.

  static LANGUAGE GetLanguageFromString(std::string lang);

  std::vector<std::shared_ptr<Message>>& messages_list();

 protected:
  bool ReadProtoFile();

  void PrintToken(std::string description, std::string str);

  void LogError(const char* error_msg, ...) const;
  void PrintParseState() const;

  LANGUAGE lang_;
  std::string proto_file_;

  std::map<std::string, std::shared_ptr<Message>> messages_map_;
  std::vector<std::shared_ptr<Message>> messages_list_;
  std::map<std::string, std::shared_ptr<EnumType>> enums_map_;

  int line_number_ = 0;
  ParseState state_ = GLOBAL;
  std::string current_package_;
  std::vector<std::string> pkg_stack_;
  Message* current_message_;
  EnumType* current_enum_;
 
 private:
  bool ParsePackageName(std::string line);
  bool ParseMessageName(std::string line);
  bool ParseMessageField(std::string line);
  bool ParseEnumName(std::string line);
  bool ParseEnumValue(std::string line);
  bool ParseAssignExpression(std::string line,
                             std::string* left,
                             std::string* right,
                             FIELD_TYPE type) const;

  static bool IsMessageFiledLine(std::string line);
  static bool IsValidVariableName(std::string str);

  bool init_success_ = false;
};

}  // namespace ProtoParser
}  // namespace Proto

#endif /* PROTO_PARSER_ */