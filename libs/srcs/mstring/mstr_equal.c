// MetaReal Programming Language version 1.0.0
// MetaReal String Library version 1.0.0

#include <mstring.h>

uint8 mstr_equal(mstr_c op1, mstr_c op2)
{
    if (op1->_size != op2->_size)
        return 0;

    uint64 i;
    for (i = 0; i < op1->_size; i++)
        if (op1->_str[i] != op2->_str[i])
            return 0;
    return 1;
}
