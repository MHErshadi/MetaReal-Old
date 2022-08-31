// MetaReal Programming Language version 1.0.0
// MetaReal String Library version 1.0.0

#include <mstr.h>

void mstr_remove(heap_t heap, mstr_t str, uint64 pos)
{
    uint64 i, j;
    for (i = pos, j = pos + 1; j <= str->_size; i++, j++)
        str->_str[i] = str->_str[j];

    heap_shrink(heap, str->_str, str->_size);

    str->_size--;
}
