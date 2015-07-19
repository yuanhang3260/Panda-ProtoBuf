#ifndef PB_CLASS_GENERATOR_
#define PB_CLASS_GENERATOR_

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Message.h"

namespace PandaProto {
namespace Compiler {

class PBClassGenerator {
 public:
  enum LANGUAGE {
    CPP,
    PYTHON,
    JAVA,
    UNKNOWN_LANGUAGE,
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
 
 private:
  bool ParsePackageName(std::string line);
  bool ParseMessageName(std::string line);
  bool ParseMessageField(std::string line);
  bool ParseAssignExpression(std::string line,
                             std::string* left, std::string* right) const;
  Message* CurrentMessage() const;

  static bool IsMessageFiledLine(std::string line);
  static bool IsValidVariableName(std::string str);

  void PrintToken(std::string description, std::string str);

  LANGUAGE lang_;
  std::string proto_file_;

  std::string current_package_ = "";
  std::map<std::string, std::shared_ptr<Message>> messages_map_;
  std::vector<std::shared_ptr<Message>> messages_list_;
  int line_number_ = 0;
};

}  // Compiler
}  // PandaProto

#endif /* PB_CLASS_GENERATOR_ */