// MetaReal Programming Language version 1.0.0
// MetaReal Float Library version 1.0.0

#include <mfloat.h>
#include <stdio.h>

void set_mfloat_str(mfloat_t dst, cstr value)
{
    sscanf(value, "%lf", &dst->_value);
}
