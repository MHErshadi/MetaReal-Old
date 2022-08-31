// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <fblock.h>
#include <stddef.h>

void heap_shrink(heap_t heap, ptr block, uint64 size)
{
    size += sizeof(uint64);

    uint64p cast = block;
    cast--;

    uint8p dcast = (uint8p)cast;

    if (!heap->_fblock)
    {
        heap->_fblock = set_fblock(dcast + size, *cast - size, NULL);

        *cast = size;
        return;
    }

    fblock_p prev = NULL;
    fblock_p next = heap->_fblock;
    while (next->_pos < dcast && next->_next)
    {
        prev = next;
        next = next->_next;
    }

    if (next->_pos < dcast)
    {
        next->_next = set_fblock(dcast + size, *cast - size, NULL);

        *cast = size;
        return;
    }

    if (dcast + *cast == next->_pos)
    {
        uint64 diff = *cast - size;

        next->_size += diff;
        next->_pos -= diff;

        *cast = size;
        return;
    }

    prev->_next = set_fblock(dcast + size, *cast - size, next);

    *cast = size;
}
