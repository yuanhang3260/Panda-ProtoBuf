#ifndef _HY_LOG
#define _HY_LOG

#include <iostream>

class Log {
 public:
  static void LogToStderr(const char* error_msg, ...);

  static void debug0() { std::cout << "debug0()" << std::endl; }
  static void debug1() { std::cout << "debug1()" << std::endl; }
  static void debug2() { std::cout << "debug2()" << std::endl; }
  static void debug3() { std::cout << "debug3()" << std::endl; }
  static void debug4() { std::cout << "debug4()" << std::endl; }
  static void debug5() { std::cout << "debug5()" << std::endl; }
  static void debug6() { std::cout << "debug6()" << std::endl; }
};

#endif  /* _HY_LOG */