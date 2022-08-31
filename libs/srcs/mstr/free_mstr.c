// MetaReal Programming Language version 1.0.0
// MetaReal String Library version 1.0.0

#include <mstr.h>
#include <stddef.h>

void free_mstr(heap_t heap, mstr_t str)
{
    if (str->_size)
    {
        heap_free(heap, str->_str);
        str->_size = 0;
    }
}
