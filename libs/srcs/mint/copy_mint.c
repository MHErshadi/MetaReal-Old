// MetaReal Programming Language version 1.0.0
// MetaReal Integer Library version 1.0.0

#include <mint.h>

void copy_mint(mint_t dst, mint_c src)
{
    dst->_value = src->_value;
}
