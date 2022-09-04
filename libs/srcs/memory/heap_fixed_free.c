// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <fblock.h>
#include <stdlib.h>

void heap_fixed_free(heap_t heap, ptr block, uint64 size)
{
    uint8p dcast = block;

    if (!heap->_fblock)
    {
        heap->_fblock = set_fblock(dcast, size, NULL);
        return;
    }

    fblock_p prev = NULL;
    fblock_p next = heap->_fblock;
    while (next->_pos < dcast && next->_next)
    {
        prev = next;
        next = next->_next;
    }

    if (!prev)
    {
        if (dcast + size == next->_pos)
        {
            next->_pos = dcast;
            next->_size += size;
        }
        else
            heap->_fblock = set_fblock(dcast, size, next);

        return;
    }
    if (next->_pos < dcast)
    {
        if (next->_pos + next->_size == dcast)
            next->_size += size;
        else
            next->_next = set_fblock(dcast, size, NULL);

        return;
    }

    if (prev->_pos + prev->_size == dcast)
    {
        prev->_size += size;

        if (dcast + size == next->_pos)
        {
            prev->_size += next->_size;
            prev->_next = next->_next;

            free(next);
        }

        return;
    }

    if (dcast + size == next->_pos)
    {
        next->_pos = dcast;
        next->_size += size;
        return;
    }

    prev->_next = set_fblock(dcast, size, next);
}
