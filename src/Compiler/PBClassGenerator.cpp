#include "PBClassGenerator.h"

#include "../IO/FileDescriptor.h"
#include "../Utility/BufferedDataReader.h"
#include "../Utility/Strings.h"

namespace PandaProto {
namespace Compiler {

bool PBClassGenerator::ParseProtoFile(std::string proto_file) {
  std::unique_ptr<IO::FileDescriptor> fd(
      new IO::FileDescriptor(proto_file, IO::FileDescriptor::READ_ONLY));
  Utility::BufferedDataReader br(std::move(fd));

  std::string line;
  while (br.ReadLine(&line)) {
    line = StringUtils::Strip(line);
    if (line.length() == 0 || StringUtils::StartWith(line, "//")) {
      continue;
    }
  }
  return true;
}

}  // Compiler
}  // PandaProto