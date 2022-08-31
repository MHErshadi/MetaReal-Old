// MetaReal Programming Language version 1.0.0
// MetaReal Float Library version 1.0.0

#include <mfloat.h>

uint8 mfloat_lessequal(mfloat_c op1, mfloat_c op2)
{
    return op1->_value <= op2->_value;
}
