// MetaReal Programming Language version 1.0.0
// MetaReal String Library version 1.0.0

#include <mstring.h>

void set_mstr(heap_t heap, uint64 alt_size, mstr_t dst, cstr src, uint64 size)
{
    dst->_size = size;

    dst->_ptr = heap_alloc(heap, size + 1, alt_size);

    dst->_str = dst->_ptr->_pos;

    uint64 i;
    for (i = 0; i < size; i++)
        dst->_str[i] = src[i];
    dst->_str[i] = '\0';
}
