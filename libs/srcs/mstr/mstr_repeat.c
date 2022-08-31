// MetaReal Programming Language version 1.0.0
// MetaReal String Library version 1.0.0

#include <mstr.h>

void mstr_repeat(heap_t heap, mstr_t str, uint64 count)
{
    if (!str->_size || !count)
        return;

    uint64 size = str->_size * count;
    str->_str = heap_expand(heap, str->_str, size + 1);

    uint64 i, j;
    for (i = str->_size; i < size;)
        for (j = 0; j < str->_size; j++, i++)
            str->_str[i] = str->_str[j];
    str->_str[i] = '\0';

    str->_size = size;
}
