// MetaReal Programming Language version 1.0.0
// MetaReal Float Library version 1.0.0

#include <mfloat.h>

void copy_mfloat(mfloat_t dst, mfloat_c src)
{
    dst->_value = src->_value;
}
