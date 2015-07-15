#include "Strings.h"
#include "../Utility/Utils.h"

std::string StringUtils::Strip(std::string str) {
  if (str.length() <= 1) {
    return str;
  }
  unsigned int i, j;
  for (i = 0; i < str.length(); i++) {
    if (str[i] != ' ') {
      break;
    }
  }
  for (j = str.length() - 1; j >= 0; j--) {
    if (str[j] != ' ') {
      break;
    }
  }
  return str.substr(i, j + 1 - i);
}

std::string StringUtils::Strip(std::string str, std::string match) {
  std::string str1 = Strip(str);
  if (match.length() == 0) {
    return str1;
  }
  // strip head.
  int limit = Utils::Min(str.length(), match.length());
  int i = 0;
  for (; i < limit; i++) {
    if (str[i] != match[i]) {
      break;
    }
  }

  int j = str.length() - 1, k = match.length() - 1;
  for (; j >= 0 && k >= 0; j--, k--) {
    if (str[j] != match[k]) {
      break;
    }
  }
  // printf("i = %d, j = %d\n", i, j);
  return str.substr(i, j + 1 - i);
}

bool StringUtils::StartWith(std::string& str, std::string match) {
  if (match.length() > str.length()) {
    return false;
  }
  for (unsigned int i = 0; i < match.length(); i++) {
    if (str[i] != match[i]) {
      return false;
    }
  }
  return true;
}