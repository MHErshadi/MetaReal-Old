// MetaReal Programming Language version 1.0.0
// MetaReal String Library version 1.0.0

#include <mstr.h>

void set_mstr(heap_t heap, mstr_t dst, cstr src, uint64 size)
{
    if (!size)
    {
        dst->_size = 0;
        return;
    }

    dst->_str = heap_alloc(heap, size);

    uint64 i;
    for (i = 0; i < size; i++)
        dst->_str[i] = src[i];

    dst->_size = size - 1;
}
