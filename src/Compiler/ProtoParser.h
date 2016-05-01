#ifndef COMPILER_PROTO_PARSER_
#define COMPILER_PROTO_PARSER_

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Type.h"
#include "Message.h"
#include "ServiceType.h"

namespace proto {
class DescriptorsBuilder;

namespace ProtoParser {
class CppGeneratorImpl;

class Parser {
 public:
  enum ParseState {
    GLOBAL,
    PARSINGMSG,
    PARSINGENUM,
    PARSINGNESTEDENUM,
    PARSESERVICE,
    PARSERPC,
  };

  Parser(LANGUAGE lang, std::string file);
  virtual ~Parser();
  Parser(const Parser&) = delete;
  Parser& operator=(const Parser&) = delete;

  bool ParseProto();
  void PrintParsedProto() const;
  virtual void GenerateCode() {}  // Need language-specific implementation.

  static LANGUAGE GetLanguageFromString(std::string lang);

  std::vector<std::shared_ptr<Message>>& mutable_messages_list();

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
  std::map<std::string, std::shared_ptr<ServiceType>> services_map_;

  int line_number_ = 0;
  ParseState state_ = GLOBAL;
  std::string current_package_;
  std::vector<std::string> pkg_stack_;
  Message* current_message_;
  EnumType* current_enum_;
  ServiceType* current_service_;
  RpcMethod* current_rpc_;
 
 private:
  bool ParsePackageName(std::string line);
  bool ParseMessageName(std::string line);
  bool ParseServiceName(std::string line);
  bool ParseMessageField(std::string line);
  bool ParseEnumName(std::string line);
  bool ParseEnumValue(std::string line);
  bool ParseRpcName(std::string line);
  bool ParseRpcOption(std::string line);
  bool ParseAssignExpression(std::string line,
                             std::string* left,
                             std::string* right,
                             FIELD_TYPE type) const;

  static bool IsMessageFiledLine(std::string line);
  static bool IsValidVariableName(std::string str);
  static bool IsValidPrimitiveTypeName(std::string str);
  PbType* FindParsedMessageOrEnumType(std::string type_name) const;

  bool init_success_ = false;

  friend class ::proto::DescriptorsBuilder;
};

}  // namespace ProtoParser
}  // namespace Proto

#endif  // COMPILER_PROTO_PARSER_
