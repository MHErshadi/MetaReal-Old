// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <block.h>
#include <stdlib.h>
#include <crash.h>

void clear_heap(heap_t heap, uint64 size)
{
    if (heap->_size != size)
    {
        heap->_data = realloc(heap->_data, size);
        if (!heap->_data)
            alloc_error(size);

        heap->_size = size;
    }

    if (heap->_ablock)
        free_ablock(heap->_ablock);

    if (heap->_fblock)
        free_fblock(heap->_fblock);
    heap->_fblock = set_fblock(heap->_data, size, NULL);
}
