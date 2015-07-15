#include "stdlib.h"
#include "stdlio.h"
#include "iostream"
#include "string"

#include "../IO/FileDescriptor.h"
#include "../Utility/BufferedDataReader.h"
#include "CppCompiler.h"

namespace PandaProto {
namespace Compiler {

CppCompiler::CppCompiler(LANGUAGE lang) : PBClassGenerator(lang) {
  
}

CppCompiler::~CppCompiler() {

}

bool CppCompiler::GeneratePBClass() {
  
}

}  // Compiler
}  // PandaProto

#endif /* CPP_COMPILER */