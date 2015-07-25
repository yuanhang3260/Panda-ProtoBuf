#ifndef __STRINGS_H_
#define __STRINGS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <memory>

class StringUtils {
 public:
  static bool IsCapitalLetter(const char c) {
    return c >= 'A' && c <= 'Z';
  }

  static bool IsLowerCaseLetter(const char c) {
    return c >= 'a' && c <= 'z';
  }

  static std::string Upper(std::string str) {
    char c[str.length()];
    for (unsigned int i = 0; i < str.length(); i++) {
      if (str[i] >= 'a' && str[i] <= 'z') {
        c[i] = str[i] - 32;
      }
      else {
        c[i] = str[i];
      }
    }
    return std::string(c, str.length());
  }

  static std::string Lower(std::string str) {
    char c[str.length()];
    for (unsigned int i = 0; i < str.length(); i++) {
      if (str[i] >= 'A' && str[i] <= 'Z') {
        c[i] = str[i] + 32;
      }
      else {
        c[i] = str[i];
      }
    }
    return std::string(c, str.length());
  }

  static bool IsLetterOrDigitOrUnderScore(const char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           (c == '_');
  }

  static std::string Strip(std::string str);
  static std::string Strip(std::string str, std::string match);

  static bool StartWith(std::string& str, std::string match);
  static bool EndWith(std::string& str, std::string match);

  static std::vector<std::string> Split(std::string& str, const char c);
  static std::vector<std::string> Split(std::string& str, std::string match);

  static std::vector<std::string> SplitGreedy(std::string& str, const char c);
  static std::vector<std::string> SplitGreedy(std::string& str,
                                              std::string match);

  static bool IsSingleWord(std::string str);

  static std::string StrCat(std::vector<std::string>, unsigned int start = 0);
};

#endif /* __STRINGS_H_ */
