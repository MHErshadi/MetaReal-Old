// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <block.h>
#include <stdlib.h>
#include <crash.h>

ptr heap_expand(heap_t heap, ablock_p block, uint64 size, uint64 alt_size)
{
    uint64 add = size - block->_size;

    if (!heap->_fblock)
    {
        increase_heap(heap, alt_size);

        while (size > heap->_fblock->_size)
            increase_heap(heap, alt_size);

        if (size == heap->_fblock->_size)
        {
            uint64 i;
            for (i = 0; i < block->_size; i++)
                heap->_fblock->_pos[i] = block->_pos[i];

            free(heap->_fblock);
            heap->_fblock = set_fblock(block->_pos, block->_size, NULL);

            block->_pos = heap->_fblock->_pos;
            block->_size = size;
            return block->_pos;
        }

        uint64 i;
        for (i = 0; i < block->_size; i++)
            heap->_fblock->_pos[i] = block->_pos[i];

        block->_pos = heap->_fblock->_pos;
        block->_size = size;

        heap->_fblock->_pos += add;
        heap->_fblock->_size -= add;
        heap->_fblock = set_fblock(block->_pos, block->_size, heap->_fblock);

        return block->_pos;
    }

    fblock_p prev = NULL;
    fblock_p next = heap->_fblock;
    while (next->_pos < block->_pos && next->_next)
    {
        prev = next;
        next = next->_next;
    }

    if (block->_pos + block->_size == next->_pos)
    {
        if (add == next->_size)
        {
            block->_size = size;

            if (prev)
                prev->_next = next->_next;
            else
                heap->_fblock = next->_next;
            free(next);

            return block->_pos;
        }

        if (add < next->_size)
        {
            next->_pos += add;
            next->_size -= add;

            block->_size = size;

            return block->_pos;
        }

        if (prev && prev->_pos + prev->_size == block->_pos)
        {
            prev->_size += block->_size + next->_size;
            prev->_next = next->_next;

            free(next);
        }
        else
        {
            next->_pos -= block->_size;
            next->_size += block->_size;
        }
    }
    else if (prev && prev->_pos + prev->_size == block->_pos)
        prev->_size += block->_size;
    else
    {
        if (next->_pos > block->_pos)
        {
            if (prev)
                prev->_next = set_fblock(block->_pos, block->_size, next);
            else
                heap->_fblock = set_fblock(block->_pos, block->_size, next);
        }
        else
            next->_next = set_fblock(block->_pos, block->_size, NULL);
    }

    prev = NULL;
    fblock_p fit = heap->_fblock;
    while (fit->_next && fit->_size < size)
    {
        prev = fit;
        fit = fit->_next;
    }

    if (fit->_size < size)
    {
        increase_heap(heap, alt_size);

        if (fit->_next)
        {
            prev = fit;
            fit = fit->_next;
        }

        while (size > fit->_size)
            increase_heap(heap, alt_size);

        if (size == fit->_size)
        {
            uint64 i;
            for (i = 0; i < block->_size; i++)
                fit->_pos[i] = block->_pos[i];

            block->_pos = fit->_pos;
            block->_size = size;

            if (prev)
                prev->_next = NULL;
            else
                heap->_fblock = NULL;
            free(fit);

            return block->_pos;
        }

        uint64 i;
        for (i = 0; i < block->_size; i++)
            fit->_pos[i] = block->_pos[i];

        block->_pos = fit->_pos;
        block->_size = size;

        fit->_pos += size;
        fit->_size -= size;

        return block->_pos;
    }

    if (size == fit->_size)
    {
        uint64 i;
        for (i = 0; i < block->_size; i++)
            fit->_pos[i] = block->_pos[i];

        block->_pos = fit->_pos;
        block->_size = size;

        if (prev)
            prev->_next = fit->_next;
        else
            heap->_fblock = fit->_next;
        free(fit);

        return block->_pos;
    }

    uint64 i;
    for (i = 0; i < block->_size; i++)
        fit->_pos[i] = block->_pos[i];

    block->_pos = fit->_pos;
    block->_size = size;

    fit->_pos += size;
    fit->_size -= size;

    return block->_pos;
}
