// MetaReal Programming Language version 1.0.0
// MetaReal Integer Library version 1.0.0

#include <mint.h>
#include <math.h>

void mint_power(mint_t op1, mint_t op2)
{
    op1->_value = pow(op1->_value, op2->_value);
}
