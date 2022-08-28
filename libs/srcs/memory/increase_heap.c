// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <block.h>
#include <stdlib.h>
#include <crash.h>

void increase_heap(heap_t heap, uint64 add)
{
    uint64 size = heap->_size + add;
    uint8p data = realloc(heap->_data, size);
    if (!data)
        alloc_error(size);

    uint64 shift = data - heap->_data;

    ablock_p alast = heap->_ablock;
    while (alast->_next)
    {
        alast->_pos += shift;
        alast = alast->_next;
    }
    alast->_pos += shift;

    fblock_p flast = heap->_fblock;
    if (!flast)
    {
        heap->_fblock = set_fblock(data + heap->_size, add, NULL);

        heap->_data = data;
        heap->_size = size;
        return;
    }
    while (flast->_next)
    {
        flast->_pos += shift;
        flast = flast->_next;
    }
    flast->_pos += shift;

    uint8p end = data + heap->_size;
    if (flast->_pos + flast->_size == end)
        flast->_size += add;
    else
        flast->_next = set_fblock(end, add, NULL);

    heap->_data = data;
    heap->_size = size;
}
