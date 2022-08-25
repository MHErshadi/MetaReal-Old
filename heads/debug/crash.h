// MetaReal Programming Language version 1.0.0

#ifndef __CRASH__
#define __CRASH__

#include <type.h>

void error1(cstr fmt, ...);
void error2(cstr msg);

#define fil_error(adr) error1("System Error: could not open \"%s\"", adr)
#define mem_error(siz) error1("System Error: could not allocate %llu bytes from memory", siz)

#endif /* __CRASH__ */
