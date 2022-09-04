// MetaReal Programming Language version 1.0.0
// MetaReal String Library version 1.0.0

#include <mstr.h>
#include <stdio.h>

void mstr_concat(heap_t heap, mstr_t op1, mstr_t op2)
{
    uint64 size = op1->_size + op2->_size + 1;

    op1->_str = heap_expand(heap, op1->_str, size);

    uint64 i, j;
    for (i = op1->_size, j = 0; i < size; i++, j++)
        op1->_str[i] = op2->_str[j];

    op1->_size += op2->_size;

    free_mstr(heap, op2);
}
