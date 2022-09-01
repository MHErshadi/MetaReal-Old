// MetaReal Programming Language version 1.0.0

#include <ir/block.h>

uint8 mlist_equal(mlist_c op1, mlist_c op2)
{
    if (op1->_size != op2->_size)
        return 0;

    uint64 i;
    for (i = 0; i < op1->_size; i++)
        if (!block_equal(op1->_elements[i], op2->_elements[i]))
            return 0;
    return 1;
}
