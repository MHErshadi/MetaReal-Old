// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <fblock.h>
#include <crash.h>
#include <stddef.h>

void clear_heap(heap_t heap)
{
    if (heap->_fblock)
        free_fblock(heap->_fblock);
    heap->_fblock = set_fblock(heap->_data, heap->_size, NULL);
}
