#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"

#include "Log.h"

void Log::LogToStderr(const char* error_msg, ...) {
  va_list args;
  va_start(args, error_msg);
  vfprintf(stderr, error_msg, args);
  va_end(args);
  fprintf(stderr, ".\n");
}