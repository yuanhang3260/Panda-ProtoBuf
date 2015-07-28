#ifndef CPP_CODE_GENERATOR_
#define CPP_CODE_GENERATOR_

#include "ProtoParser.h"
#include "../IO/TextPrinter.h"

namespace PandaProto {
namespace Compiler {

class CppCodeGenerator : public ProtoParser {
 public:
  CppCodeGenerator(std::string file) : ProtoParser(CPP, file) {}
  ~CppCodeGenerator() {}
  CppCodeGenerator(const CppCodeGenerator&) = delete;
  CppCodeGenerator& operator=(const CppCodeGenerator&) = delete;

  void GenerateCode() override;
 
 private:
  void GenerateHeader();
  void GenerateCC();

  void CheckoutNameSpace(std::vector<std::string>& context_stk,
                         const std::vector<std::string>& target_stk);
  std::string GetNameSpacePrefix(const std::vector<std::string>& context_stk,
                                 const std::vector<std::string>& target_stk);

  IO::TextPrinter printer;
};

}  // Compiler
}  // PandaProto

#endif  /* CPP_CODE_GENERATOR_ */