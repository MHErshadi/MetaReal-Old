// MetaReal Programming Language version 1.0.0

#include <array/list.h>

void mlist_repeat(stack_t stack, heap_t heap, mlist_t mlist, uint64 count)
{
    if (!mlist->_size || !count)
        return;

    uint64 size = mlist->_size * count;
    mlist->_elements = heap_expand(heap, mlist->_elements, size * sizeof(block_t));

    uint64 i, j;
    for (i = mlist->_size; i < size;)
        for (j = 0; j < mlist->_size; j++, i++)
            mlist->_elements[i] = block_copy(mlist->_elements[j], stack, heap);

    mlist->_size = size;
}
