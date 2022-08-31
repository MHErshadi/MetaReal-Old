// MetaReal Programming Language version 1.0.0
// MetaReal Float Library version 1.0.0

#include <mfloat.h>

void set_mfloat_mint(mfloat_t dst, mint_c src)
{
    dst->_value = src->_value;
}
