// MetaReal Programming Language version 1.0.0
// MetaReal Integer Library version 1.0.0

#include <mint.h>

void mint_not(mint_t mint)
{
    mint->_value = ~mint->_value;
}
