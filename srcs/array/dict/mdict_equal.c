// MetaReal Programming Language version 1.0.0

#include <array/dict.h>

uint8 mdict_equal(mdict_c op1, mdict_c op2)
{
    if (op1->_size != op2->_size)
        return 0;

    return dict_node_equal(op1->_node, op2->_node);
}
