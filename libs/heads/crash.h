// MetaReal Programming Language version 1.0.0

#ifndef __M_CRASH__
#define __M_CRASH__

#include <ctypes.h>

void fcrash(cstr format, ...);
void crash(cstr message);

#define file_open_error(address) fcrash("System Error: could not open \"%s\"", address)
#define alloc_error(size) fcrash("System Error: could not allocate %llu bytes from memory", size)

#endif /* __M_CRASH__ */
