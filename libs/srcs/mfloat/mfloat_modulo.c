// MetaReal Programming Language version 1.0.0
// MetaReal Float Library version 1.0.0

#include <mfloat.h>
#include <math.h>

void mfloat_modulo(mfloat_t op1, mfloat_t op2)
{
    op1->_value = fmod(op1->_value, op2->_value);
}
