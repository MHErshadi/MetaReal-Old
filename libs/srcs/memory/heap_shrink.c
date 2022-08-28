// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <block.h>
#include <stddef.h>

void heap_shrink(heap_t heap, ablock_p block, uint64 size)
{
    if (!heap->_fblock)
    {
        heap->_fblock = set_fblock(block->_pos + size, block->_size - size, NULL);

        block->_size = size;
        return;
    }

    fblock_p prev = NULL;
    fblock_p next = heap->_fblock;
    while (next->_pos < block->_pos && next->_next)
    {
        prev = next;
        next = next->_next;
    }

    if (next->_pos < block->_pos)
    {
        next->_next = set_fblock(block->_pos + size, block->_size - size, NULL);

        block->_size = size;
        return;
    }

    if (block->_pos + block->_size == next->_pos)
    {
        uint64 diff = block->_size - size;

        next->_size += diff;
        next->_pos -= diff;

        block->_size = size;
        return;
    }

    prev->_next = set_fblock(block->_pos + size, block->_size - size, next);

    block->_size = size;
}
