// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <fblock.h>
#include <stdlib.h>

void heap_free(heap_t heap, ptr block)
{
    uint64p cast = block;
    cast--;

    uint8p dcast = (uint8p)cast;

    if (!heap->_fblock)
    {
        heap->_fblock = set_fblock(dcast, *cast, NULL);
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
        if (dcast + *cast == next->_pos)
        {
            next->_pos = dcast;
            next->_size += *cast;
        }
        else
            heap->_fblock = set_fblock(dcast, *cast, next);

        return;
    }
    if (next->_pos < dcast)
    {
        if (next->_pos + next->_size == dcast)
            next->_size += *cast;
        else
            next->_next = set_fblock(dcast, *cast, NULL);

        return;
    }

    if (prev->_pos + prev->_size == dcast)
    {
        prev->_size += *cast;

        if (dcast + *cast == next->_pos)
        {
            prev->_size += next->_size;
            prev->_next = next->_next;

            free(next);
        }

        return;
    }

    if (dcast + *cast == next->_pos)
    {
        next->_pos = dcast;
        next->_size += *cast;
        return;
    }

    prev->_next = set_fblock(dcast, *cast, next);
}
