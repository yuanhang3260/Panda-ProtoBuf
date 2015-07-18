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
  };

  PBClassGenerator(LANGUAGE lang) : lang_(lang) {}
  virtual ~PBClassGenerator();
  virtual bool GeneratePBClass() = 0;

 protected:
  bool ReadProtoFile(std::string proto_file);
 
 private:
  bool ParsePackageName(std::string line);
  bool ParseMessageName(std::string line);
  bool ParseMessageField(std::string line);
  bool ParseAssignExpression(std::string line,
                             std::string* left, std::string* right) const;

  static bool IsMessageFiledLine(std::string line);
  static bool IsValidVariableName(std::string str);
  Message* CurrentMessage() const;



  LANGUAGE lang_;
  std::string proto_file_;

  std::string current_package_ = "";
  std::map<std::string, std::shared_ptr<Message>> messages_map_;
  std::vector<std::shared_ptr<Message>> messages_list_;
};

}  // Compiler
}  // PandaProto

#endif /* PB_CLASS_GENERATOR_ */