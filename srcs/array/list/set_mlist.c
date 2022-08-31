// MetaReal Programming Language version 1.0.0

#include <array/list.h>

void set_mlist(mlist_t dst, block_p elements, uint64 size)
{
    dst->_elements = elements;
    dst->_size = size;
}
