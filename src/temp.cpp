#include "temp.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void __attribute__((__noreturn__)) panic(const char *__restrict fmt, ...) {
  va_list arg_list;
  va_start(arg_list, fmt);
  vfprintf(stderr, fmt, arg_list);
  va_end(arg_list);
  exit(1);
}
