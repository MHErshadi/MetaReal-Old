// MetaReal Programming Language version 1.0.0
// MetaReal Integer Library version 1.0.0

#include <mint.h>

uint8 mint_greater(mint_c op1, mint_c op2)
{
    return op1->_value > op2->_value;
}
