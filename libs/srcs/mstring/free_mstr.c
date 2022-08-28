// MetaReal Programming Language version 1.0.0
// MetaReal String Library version 1.0.0

#include <mstring.h>
#include <stddef.h>

void free_mstr(heap_t heap, mstr_t str)
{
    str->_str = NULL;
    str->_size = 0;

    heap_free(heap, str->_ptr);
}
