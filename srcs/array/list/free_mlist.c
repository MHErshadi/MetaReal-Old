// MetaReal Programming Language version 1.0.0

#include <ir/block.h>

void free_mlist(heap_t heap, mlist_t mlist)
{
    uint64 i;
    for (i = 0; i < mlist->_size; i++)
        block_free(heap, mlist->_elements[i]);

    heap_free(heap, mlist->_elements);
}
