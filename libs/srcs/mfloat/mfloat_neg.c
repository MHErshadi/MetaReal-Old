// MetaReal Programming Language version 1.0.0
// MetaReal Float Library version 1.0.0

#include <mfloat.h>

void mfloat_neg(mfloat_t mfloat)
{
    mfloat->_value = -mfloat->_value;
}
