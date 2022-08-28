// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <block.h>
#include <stdlib.h>

void free_heap(heap_t heap)
{
    free(heap->_data);
    heap->_size = 0;

    if (heap->_ablock)
        free_ablock(heap->_ablock);

    if (heap->_fblock)
        free_fblock(heap->_fblock);
}
