// MetaReal Programming Language version 1.0.0
// MetaReal Integer Library version 1.0.0

#include <mint.h>

void mint_add(mint_t op1, mint_t op2)
{
    op1->_value += op2->_value;
}
