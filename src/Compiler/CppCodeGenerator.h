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

  void DefineDestructor(Message* message);

  void PrintCopyClassCode(Message* message);
  void DefineCopyConstructor(Message* message);
  void DefineMoveConstructor(Message* message);

  void PrintMoveClassCode(Message* message);
  void DefineCopyAssigner(Message* message);
  void DefineMoveAssigner(Message* message);

  void DefineAccessors(Message* message);
  void DefineSingularNumericTypeAccessors(Message* message,
                                          MessageField* field);

  void DefineSwapper(Message* message);

  std::map<std::string, std::string>
  GetFieldMatchMap(Message* message, MessageField* field);
  
  void CheckoutNameSpace(std::vector<std::string>& context_stk,
                         const std::vector<std::string>& target_stk);
  
  std::string GetNameSpacePrefix(const std::vector<std::string>& context_stk,
                                 const std::vector<std::string>& target_stk);

  IO::TextPrinter printer;
};

}  // Compiler
}  // PandaProto

#endif  /* CPP_CODE_GENERATOR_ */