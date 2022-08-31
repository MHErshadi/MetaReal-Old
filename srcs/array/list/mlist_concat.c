// MetaReal Programming Language version 1.0.0

#include <array/list.h>

void mlist_concat(heap_t heap, mlist_t op1, mlist_t op2)
{
    if (!op2->_size)
        return;
    if (!op1->_size)
    {
        op1 = op2;
        return;
    }

    uint64 size = op1->_size + op2->_size;

    op1->_elements = heap_expand(heap, op1->_elements, size * sizeof(block_t));

    uint64 i, j;
    for (i = op1->_size, j = 0; i < size; i++, j++)
        op1->_elements[i] = op2->_elements[j];

    heap_free(heap, op2->_elements);
}
