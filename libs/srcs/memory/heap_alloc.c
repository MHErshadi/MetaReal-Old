// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <fblock.h>
#include <stdlib.h>
#include <crash.h>

ptr heap_alloc(heap_t heap, uint64 size)
{
    if (!heap->_fblock)
        heap_alloc_error(size);

    size += sizeof(uint64);

    fblock_p prev = NULL;
    fblock_p fit = heap->_fblock;
    while (fit->_size < size && fit->_next)
    {
        prev = fit;
        fit = fit->_next;
    }

    if (fit->_size < size)
        heap_alloc_error(size);

    if (size == fit->_size)
    {
        uint64p block = (uint64p)fit->_pos;
        *block = size;

        if (prev)
            prev->_next = fit->_next;
        else
            heap->_fblock = fit->_next;
        free(fit);

        return ++block;
    }

    uint64p block = (uint64p)fit->_pos;
    *block = size;

    fit->_pos += size;
    fit->_size -= size;

    return ++block;
}
