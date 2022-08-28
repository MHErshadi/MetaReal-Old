// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <block.h>
#include <stdlib.h>

ablock_p heap_alloc(heap_t heap, uint64 size, uint64 alt_size)
{
    if (!heap->_fblock)
    {
        increase_heap(heap, alt_size);

        while (size > heap->_fblock->_size)
            increase_heap(heap, alt_size);

        if (size == heap->_fblock->_size)
        {
            heap->_ablock = set_ablock(heap->_fblock->_pos, size, NULL, heap->_ablock);

            free(heap->_fblock);
            return heap->_ablock;
        }

        heap->_ablock = set_ablock(heap->_fblock->_pos, size, NULL, heap->_ablock);

        heap->_fblock->_pos += size;
        heap->_fblock->_size -= size;

        return heap->_ablock;
    }

    fblock_p prev = NULL;
    fblock_p fit = heap->_fblock;
    while (fit->_size < size && fit->_next)
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
            heap->_ablock = set_ablock(fit->_pos, size, NULL, heap->_ablock);

            if (prev)
                prev->_next = NULL;
            else
                heap->_fblock = NULL;
            free(fit);

            return heap->_ablock;
        }

        heap->_ablock = set_ablock(fit->_pos, size, NULL, heap->_ablock);

        fit->_pos += size;
        fit->_size -= size;

        return heap->_ablock;
    }

    if (size == fit->_size)
    {
        heap->_ablock = set_ablock(fit->_pos, size, NULL, heap->_ablock);

        if (prev)
            prev->_next = fit->_next;
        else
            heap->_fblock = fit->_next;
        free(fit);

        return heap->_ablock;
    }

    heap->_ablock = set_ablock(fit->_pos, size, NULL, heap->_ablock);

    fit->_pos += size;
    fit->_size -= size;

    return heap->_ablock;
}
