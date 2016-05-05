#include <memory>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Base/Log.h"
#include "Proto/Common.h"
#include "Proto/RepeatedFields.h"
#include "Proto/MessageFactory.h"
#include "Proto/Descriptors_internal.h"
#include "Proto/MessageReflection.h"
#include "UnitTest/UnitTest.h"

namespace proto {

class MessageReflectionTest: public UnitTest {
 private:
   
 public:
  void Test_FieldAccessors() {
    
  }
};

}  // namespace proto

int main(int argc, char** argv) {
  proto::MessageReflectionTest test;
  test.setup();

  test.Test_FieldAccessors();

  test.teardown();
  std::cout << "\033[2;32mAll Passed ^_^\033[0m" << std::endl;
  return 0;
}