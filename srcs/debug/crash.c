// MetaReal Programming Language version 1.0.0

#include <stdlib.h>
#include <stdarg.h>

#include <debug/crash.h>
#include <def.h>

void error1(cstr fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(STDERR, fmt, args);
    va_end(args);

    abort();
}

void error2(cstr msg)
{
    fputs(msg, STDERR);

    abort();
}
