// MetaReal Programming Language version 1.0.0

#include <crash.h>
#include <stdio.h>
#include <stdlib.h>

void crash(cstr message)
{
    fputs(message, stderr);

    abort();
}
