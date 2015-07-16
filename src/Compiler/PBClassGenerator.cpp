#include "PBClassGenerator.h"

#include "../IO/FileDescriptor.h"
#include "../Utility/BufferedDataReader.h"
#include "../Utility/Strings.h"

namespace PandaProto {
namespace Compiler {

bool PBClassGenerator::ReadProtoFile(std::string proto_file) {
  std::unique_ptr<IO::FileDescriptor> fd(
      new IO::FileDescriptor(proto_file, IO::FileDescriptor::READ_ONLY));
  Utility::BufferedDataReader br(std::move(fd));

  std::string line;
  while (br.ReadLine(&line)) {
    line = StringUtils::Strip(line);
    // Skip empty and comment lines.
    if (line.length() == 0 || StringUtils::StartWith(line, "//")) {
      continue;
    }
    // Parse package name.
    if (StringUtils::StartWith(line, "package") && 
        (package_ = ParsePackageName(line)) == "") {
      return false;
    }
    // Parse message.
    // TODO.
  }
  return true;
}

std::string PBClassGenerator::ParsePackageName(std::string line) {
  if (StringUtils::StartWith(line, "package")) {
    std::vector<std::string> result = StringUtils::Split(line, ' ');
    if (result.size() != 2) {
      fprintf(stderr,
              "ERROR: Syntax error in package line, "
              "more than one packet name\n");
      return "";
    }
    return result[1];
  }
  return "";
}

}  // Compiler
}  // PandaProto