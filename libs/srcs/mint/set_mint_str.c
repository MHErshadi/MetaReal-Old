// MetaReal Programming Language version 1.0.0
// MetaReal Integer Library version 1.0.0

#include <mint.h>
#include <stdio.h>

void set_mint_str(mint_t dst, cstr value)
{
    sscanf(value, "%lld", &dst->_value);
}
