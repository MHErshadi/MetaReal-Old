// MetaReal Programming Language version 1.0.0
// MetaReal String Library version 1.0.0

#include <mstring.h>

void copy_mstr(heap_t heap, uint64 alt_size, mstr_t dst, mstr_c src)
{
    dst->_size = src->_size;

    dst->_ptr = heap_alloc(heap, dst->_size + 1, alt_size);

    dst->_str = dst->_ptr->_pos;
    uint64 i;
    for (i = 0; i <= dst->_size; i++)
        dst->_str[i] = src->_str[i];
}
