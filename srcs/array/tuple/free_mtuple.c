// MetaReal Programming Language version 1.0.0

#include <ir/block.h>

void free_mtuple(heap_t heap, mtuple_t mtuple)
{
    uint64 i;
    for (i = 0; i < mtuple->_size; i++)
        block_free(heap, mtuple->_elements[i]);

    heap_free(heap, mtuple->_elements);
}
