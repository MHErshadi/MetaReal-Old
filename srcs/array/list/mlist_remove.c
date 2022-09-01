// MetaReal Programming Language version 1.0.0

#include <ir/block.h>

void mlist_remove(heap_t heap, mlist_t mlist, uint64 pos)
{
    if (mlist->_size == 1)
    {
        free_mlist(heap, mlist);
        return;
    }

    block_free(heap, mlist->_elements[pos]);

    uint64 i, j;
    for (i = pos, j = pos + 1; j < mlist->_size; i++, j++)
        mlist->_elements[i] = mlist->_elements[j];

    mlist->_size--;

    heap_shrink(heap, mlist->_elements, mlist->_size * sizeof(block_t));
}
