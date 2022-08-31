// MetaReal Programming Language version 1.0.0
// MetaReal String Library version 1.0.0

#include <mstr.h>

void copy_mstr(heap_t heap, mstr_t dst, mstr_c src)
{
    if (!src->_size)
    {
        dst->_size = 0;
        return;
    }

    dst->_str = heap_alloc(heap, src->_size + 1);

    uint64 i;
    for (i = 0; i <= src->_size; i++)
        dst->_str[i] = src->_str[i];

    dst->_size = src->_size;
}
