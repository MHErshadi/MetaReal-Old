// MetaReal Programming Language version 1.0.0

#include <array/tuple.h>

void set_mtuple(mtuple_t dst, block_p elements, uint64 size)
{
    dst->_elements = elements;
    dst->_size = size;
}
