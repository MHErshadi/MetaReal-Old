// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <block.h>
#include <stdlib.h>
#include <crash.h>

void alloc_heap(heap_t heap, uint64 size)
{
    heap->_data = malloc(size);
    if (!heap->_data)
        alloc_error(size);

    heap->_size = size;

    heap->_ablock = NULL;
    heap->_fblock = set_fblock(heap->_data, size, NULL);
}
