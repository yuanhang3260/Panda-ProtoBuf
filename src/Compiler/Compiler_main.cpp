#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <memory>

#include "ProtoParser.h"
#include "CppCodeGenerator.h"

using namespace proto::ProtoParser;

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "usage: %s cpp/python proto_file\n", argv[0]);
    return -1;
  }

  LANGUAGE lang =
      ProtoParser::GetLanguageFromString(std::string(argv[1]));
  std::string proto = std::string(argv[2]);
  ProtoParser* compiler = NULL;
  switch (lang) {
    case CPP:
      compiler = new CppCodeGenerator(proto);
      break;
    case PYTHON:
      break;
    case JAVA:
      break;
    default: break;
  }
  if (!compiler) {
    return -1;
  }

  if (!compiler->ParseProto()) {
    return -1;
  }
  compiler->GenerateCode();

  delete compiler;
  return 0;
}
