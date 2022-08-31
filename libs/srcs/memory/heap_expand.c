// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <fblock.h>
#include <stdlib.h>
#include <crash.h>

ptr heap_expand(heap_t heap, ptr block, uint64 size)
{
    if (!heap->_fblock)
        heap_alloc_error(heap);

    uint64p cast = block;
    cast--;

    uint8p dcast = (uint8p)cast;

    fblock_p prev = NULL;
    fblock_p next = heap->_fblock;
    while (next->_pos < dcast && next->_next)
    {
        prev = next;
        next = next->_next;
    }

    if (dcast + *cast == next->_pos)
    {
        uint64 add = size - *cast;

        if (add == next->_size)
        {
            if (prev)
                prev->_next = next->_next;
            else
                heap->_fblock = next->_next;
            free(next);

            *cast = size;

            return ++cast;
        }

        if (add < next->_size)
        {
            next->_pos += add;
            next->_size -= add;

            *cast = size;

            return ++cast;
        }

        if (prev && prev->_pos + prev->_size == dcast)
        {
            prev->_size += *cast + next->_size;
            prev->_next = next->_next;

            free(next);
        }
        else
        {
            next->_pos -= *cast;
            next->_size += *cast;
        }
    }
    else if (prev && prev->_pos + prev->_size == dcast)
        prev->_size += *cast;
    else
    {
        if (next->_pos > dcast)
        {
            if (prev)
                prev->_next = set_fblock(dcast, *cast, next);
            else
                heap->_fblock = set_fblock(dcast, *cast, next);
        }
        else
            next->_next = set_fblock(dcast, *cast, NULL);
    }

    prev = NULL;
    fblock_p fit = heap->_fblock;
    while (fit->_next && fit->_size < size)
    {
        prev = fit;
        fit = fit->_next;
    }

    if (fit->_size < size)
        heap_alloc_error(heap);

    if (size == fit->_size)
    {
        uint64 i;
        for (i = sizeof(uint64); i < *cast; i++)
            fit->_pos[i] = dcast[i];

        cast = (uint64p)fit->_pos;
        *cast = size;

        if (prev)
            prev->_next = fit->_next;
        else
            heap->_fblock = fit->_next;
        free(fit);

        return ++cast;
    }

    uint64 i;
    for (i = sizeof(uint64); i < *cast; i++)
        fit->_pos[i] = dcast[i];

    cast = (uint64p)fit->_pos;
    *cast = size;

    fit->_pos += size;
    fit->_size -= size;

    return ++cast;
}
