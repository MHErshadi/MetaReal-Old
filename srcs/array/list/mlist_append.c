// MetaReal Programming Language version 1.0.0

#include <array/list.h>

void mlist_append(heap_t heap, mlist_t mlist, block_t element)
{
    if (!mlist->_size)
    {
        mlist->_elements = heap_alloc(heap, sizeof(block_t));
        *mlist->_elements = element;

        mlist->_size = 1;
    }

    mlist->_elements = heap_increase(heap, mlist->_elements, sizeof(block_t));
    mlist->_elements[mlist->_size++] = element;
}
