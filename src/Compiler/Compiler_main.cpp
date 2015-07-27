#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <memory>

#include "ProtoParser.h"

using namespace PandaProto;

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "usage: %s cpp/python proto_file\n", argv[0]);
    return -1;
  }

  Compiler::ProtoParser::LANGUAGE lang =
      Compiler::ProtoParser::GetLanguageFromString(std::string(argv[1]));
  std::string proto = std::string(argv[2]);
  Compiler::ProtoParser compiler(lang, proto);
  compiler.GeneratePBClass();
  return 0;
}
