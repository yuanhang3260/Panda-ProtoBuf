#ifndef CPP_COMPILER
#define CPP_COMPILER

#include "PBClassGenerator.h"

namespace PandaProto {
namespace Compiler {

class CppCompiler: public PBClassGenerator {
 public:
  CppCompiler(LANGUAGE lang);
  virtual ~CppCompiler();
  virtual bool GeneratePBClass() override;

 private:
};

}  // Compiler
}  // PandaProto

#endif /* CPP_COMPILER */