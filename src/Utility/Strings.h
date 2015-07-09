#ifndef __STRINGS_H_
#define __STRINGS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <memory>

namespace Utility {

class Strings {
 public:
  static bool IsCapitalLetter(const char c) {
    return c >= 'A' && c <= 'Z';
  }

  static bool IsLowerCaseLetter(const char c) {
    return c >= 'a' && c <= 'z';
  }
};

}  // namespace Utility

#endif /* __STRINGS_H_ */
