#ifndef __STRINGS_H_
#define __STRINGS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <memory>

class StringUtils {
 public:
  static bool IsCapitalLetter(const char c) {
    return c >= 'A' && c <= 'Z';
  }

  static bool IsLowerCaseLetter(const char c) {
    return c >= 'a' && c <= 'z';
  }

  static std::string Strip(std::string str);
  static std::string Strip(std::string str, std::string match);

  static bool StartWith(std::string& str, std::string match);
};

#endif /* __STRINGS_H_ */
