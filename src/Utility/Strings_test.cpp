#include "stdlib.h"
#include "stdio.h"
#include "iostream"
#include "string"

#include "Strings.h"

void test_Strip() {
  std::cout << __FUNCTION__ << "()..." << std::endl;
  std::string result;
  
  result = StringUtils::Strip("  abcdefg ");
  if (result != "abcdefg") {
    fprintf(stderr, "ERROR in Strip(\"  abcdefg \"), returns \"%s\"\n",
                    result.c_str());
    exit(-1);
  }

  result = StringUtils::Strip("  abcdefg");
  if (result != "abcdefg") {
    fprintf(stderr, "ERROR in Strip(\"  abcdefg\"), returns \"%s\"\n",
                    result.c_str());
    exit(-1);
  }

  result = StringUtils::Strip("abcdefg ");
  if (result != "abcdefg") {
    fprintf(stderr, "ERROR in Strip(\"abcdefg \"), returns \"%s\"\n",
                    result.c_str());
    exit(-1);
  }

  result = StringUtils::Strip("abcdefg", "abfg");
  if (result != "cde") {
    fprintf(stderr,
            "ERROR in Strip(\"abcdefg\", \"abfg\"), returns \"%s\"\n",
            result.c_str());
    exit(-1);
  }

  result = StringUtils::Strip("{}abcdefg}", "{}");
  if (result != "abcdefg") {
    fprintf(stderr,
            "ERROR in Strip(\"{}abcdefg}\", \"{}\"), returns \"%s\"\n",
            result.c_str());
    exit(-1);
  }

  result = StringUtils::Strip("abcde", "abxyzde");
  if (result != "c") {
    fprintf(stderr,
            "ERROR in Strip(\"abcde\", \"abxyzde\"), returns \"%s\"\n",
            result.c_str());
    exit(-1);
  }

  result = StringUtils::Strip("abcdfe", "{abxyzde");
  if (result != "abcdf") {
    fprintf(stderr,
            "ERROR in Strip(\"abcdfe}\", \"abxyzde\"), returns \"%s\"\n",
            result.c_str());
    exit(-1);
  }

  std::cout << "Passed ^_^" << std::endl;
}

void test_StartWith() {
  std::cout << __FUNCTION__ << "()..." << std::endl;
  std::string str;

  str = "//abcdefg ";
  if (!StringUtils::StartWith(str, "//")) {
    fprintf(stderr,
            "ERROR in StartWith(\"//abcdefg \", \"//\"), should return true\n");
    exit(-1);
  }

  str = "//abcdefg";
  if (StringUtils::StartWith(str, " /")) {
    fprintf(stderr,
            "ERROR in StartWith(\" /abcdefg\", \" /\"), should return false\n");
    exit(-1);
  }

  str = "abc";
  if (!StringUtils::StartWith(str, "")) {
    fprintf(stderr, "ERROR in StartWith(\"abc\", \"\"), should return true\n");
    exit(-1);
  }

  str = "abc";
  if (StringUtils::StartWith(str, "abcdef")) {
    fprintf(stderr,
            "ERROR in StartWith(\"abc\", \"abcdefg\"), should return false\n");
    exit(-1);
  }

  str = "abc";
  if (!StringUtils::StartWith(str, "abc")) {
    fprintf(stderr,
            "ERROR in StartWith(\"abc\", \"abc\"), should return true\n");
    exit(-1);
  }

  str = "";
  if (!StringUtils::StartWith(str, "")) {
    fprintf(stderr,
            "ERROR in StartWith(\"\", \"\"), should return true\n");
    exit(-1);
  }

  str = "";
  if (StringUtils::StartWith(str, "a")) {
    fprintf(stderr,
            "ERROR in StartWith(\"\", \"a\"), should return false\n");
    exit(-1);
  }

  std::cout << "Passed ^_^" << std::endl;
}

int main(int argc, char** argv) {
  test_Strip();
  test_StartWith();
  return 0;
}
