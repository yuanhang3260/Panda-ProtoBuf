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

  // Define copiers.
  void PrintCopyClassCode(Message* message);
  void DefineCopyConstructor(Message* message);
  void DefineMoveConstructor(Message* message);

  // Define movers.
  void PrintMoveClassCode(Message* message);
  void DefineCopyAssigner(Message* message);
  void DefineMoveAssigner(Message* message);

  // Swapper
  void DefineSwapper(Message* message);

  // Declare field accessors.
  void DeclareAccessors(Message* message, MessageField* field);
  void DeclareSingularNumericTypeAccessors(Message* message,
                                           MessageField* field);
  void DeclareSingularStringTypeAccessors(Message* message,
                                          MessageField* field);
  void DeclareSingularMessageTypeAccessors(Message* message,
                                           MessageField* field);
  void DeclareRepeatedNumericTypeAccessors(Message* message,
                                           MessageField* field);
  void DeclareRepeatedNonNumericTypeAccessors(Message* message,
                                              MessageField* field);

  // Define field accessors.
  void DefineAccessors(Message* message, MessageField* field);
  void DefineSingularNumericTypeAccessors(Message* message,
                                          MessageField* field);
  void DefineSingularStringTypeAccessors(Message* message,
                                         MessageField* field);
  void DefineSingularMessageTypeAccessors(Message* message,
                                          MessageField* field);
  void DefineRepeatedNumericTypeAccessors(Message* message,
                                          MessageField* field);
  void DefineRepeatedNonNumericTypeAccessors(Message* message,
                                             MessageField* field);



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