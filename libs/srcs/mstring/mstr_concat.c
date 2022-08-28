// MetaReal Programming Language version 1.0.0
// MetaReal String Library version 1.0.0

#include <mstring.h>
#include <stdio.h>

void mstr_concat(heap_t heap, uint64 alt_size, mstr_t op1, mstr_c op2)
{
    op1->_str = heap_increase(heap, op1->_ptr, op2->_size + 1, alt_size);

    uint64 i, j;
    for (i = op1->_size, j = 0; i < op1->_ptr->_size; i++, j++)
        op1->_str[i] = op2->_str[j];

    op1->_size += op2->_size;
}
