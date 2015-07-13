#ifndef PB_CLASS_GENERATOR_
#define PB_CLASS_GENERATOR_

#include <string>

namespace Compiler {

class PBClassGenerator {
 public:
  enum LANGUAGE {
    CPP,
    PYTHON,
  };

  PBClassGenerator(LANGUAGE lang) : lang_(lang) {}
  ~PBClassGenerator();
  virtual void GeneratePBClass() = 0;

 private:
  int ParseProtoFile(std::string proto_file);

  LANGUAGE lang_;
  std::string proto_file_;
};

}

#endif /* PB_CLASS_GENERATOR_ */