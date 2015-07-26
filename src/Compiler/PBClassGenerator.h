#ifndef PB_CLASS_GENERATOR_
#define PB_CLASS_GENERATOR_

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "../IO/TextPrinter.h"
#include "Message.h"

namespace PandaProto {
namespace Compiler {

class CppGeneratorImpl;

class PBClassGenerator {
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

  PBClassGenerator(LANGUAGE lang, std::string file);
  ~PBClassGenerator();
  PBClassGenerator(const PBClassGenerator&) = delete;
  PBClassGenerator& operator=(const PBClassGenerator&) = delete;

  bool GeneratePBClass();
  void PrintParsedProto() const;

  static LANGUAGE GetLanguageFromString(std::string lang);

 protected:
  bool ReadProtoFile();

  void PrintToken(std::string description, std::string str);

  void LogError(const char* error_msg, ...) const;
  void PrintParseState() const;

  void GenerateCppCode();

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
  EnumType* currentEnum_;
 
 private:
  bool ParsePackageName(std::string line);
  bool ParseMessageName(std::string line);
  bool ParseMessageField(std::string line);
  bool ParseEnumName(std::string line);
  bool ParseEnumValue(std::string line);
  bool ParseAssignExpression(std::string line,
                             std::string* left, std::string* right) const;

  static bool IsMessageFiledLine(std::string line);
  static bool IsValidVariableName(std::string str);

  void CheckoutNameSpace(std::vector<std::string>& context_stk,
                         const std::vector<std::string>& target_stk);

  bool init_success_ = false;
  IO::TextPrinter printer;
};

}  // Compiler
}  // PandaProto

#endif /* PB_CLASS_GENERATOR_ */