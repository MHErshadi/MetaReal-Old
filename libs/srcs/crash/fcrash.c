// MetaReal Programming Language version 1.0.0

#include <crash.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void fcrash(cstr format, ...)
{
    va_list args;

    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    abort();
}
