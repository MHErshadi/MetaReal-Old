// MetaReal Programming Language version 1.0.0

#include <array/dict.h>

void mdict_concat(heap_t heap, mdict_t op1, mdict_t op2)
{
    op1->_size += dict_node_concat(heap, op1->_node, op2->_node);
}
