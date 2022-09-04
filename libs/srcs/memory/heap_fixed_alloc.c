// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <fblock.h>
#include <stdlib.h>
#include <crash.h>

ptr heap_fixed_alloc(heap_t heap, uint64 size)
{
    if (!heap->_fblock)
        heap_alloc_error(size);

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
        uint8p block = (uint8p)fit->_pos;

        if (prev)
            prev->_next = fit->_next;
        else
            heap->_fblock = fit->_next;
        free(fit);

        return block;
    }

    uint8p block = (uint8p)fit->_pos;

    fit->_pos += size;
    fit->_size -= size;

    return block;
}
