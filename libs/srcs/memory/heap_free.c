// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <block.h>
#include <stdlib.h>

void heap_free(heap_t heap, ablock_p block)
{
    if (!heap->_fblock)
    {
        heap->_fblock = set_fblock(block->_pos, block->_size, NULL);

        if (block->_prev)
            block->_prev->_next = block->_next;
        if (block->_next)
            block->_next->_prev = block->_prev;

        free(block);
        return;
    }

    fblock_p prev = NULL;
    fblock_p next = heap->_fblock;
    while (next->_pos < block->_pos && next->_next)
    {
        prev = next;
        next = next->_next;
    }

    if (!prev)
    {
        if (block->_pos + block->_size == next->_pos)
        {
            next->_pos = block->_pos;
            next->_size += block->_size;
        }
        else
            heap->_fblock = set_fblock(block->_pos, block->_size, next);

        if (block->_prev)
            block->_prev->_next = block->_next;
        if (block->_next)
            block->_next->_prev = block->_prev;

        free(block);
        return;
    }
    if (next->_pos < block->_pos)
    {
        if (next->_pos + next->_size == block->_pos)
            next->_size += block->_size;
        else
            next->_next = set_fblock(block->_pos, block->_size, NULL);

        if (block->_prev)
            block->_prev->_next = block->_next;
        if (block->_next)
            block->_next->_prev = block->_prev;

        free(block);
        return;
    }

    if (prev->_pos + prev->_size == block->_pos)
    {
        prev->_size += block->_size;

        if (block->_pos + block->_size == next->_pos)
        {
            prev->_size += next->_size;
            prev->_next = next->_next;

            free(next);
        }

        if (block->_prev)
            block->_prev->_next = block->_next;
        if (block->_next)
            block->_next->_prev = block->_prev;

        free(block);
        return;
    }

    if (block->_pos + block->_size == next->_pos)
    {
        next->_pos = block->_pos;
        next->_size += block->_size;

        if (block->_prev)
            block->_prev->_next = block->_next;
        if (block->_next)
            block->_next->_prev = block->_prev;

        free(block);
        return;
    }

    prev->_next = set_fblock(block->_pos, block->_size, next);

    if (block->_prev)
        block->_prev->_next = block->_next;
    if (block->_next)
        block->_next->_prev = block->_prev;

    free(block);
}
