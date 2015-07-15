#ifndef PB_CLASS_GENERATOR_
#define PB_CLASS_GENERATOR_

#include <string>

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
  bool ParseProtoFile(std::string proto_file);

  LANGUAGE lang_;
  std::string proto_file_;
};

}  // Compiler
}  // PandaProto

#endif /* PB_CLASS_GENERATOR_ */