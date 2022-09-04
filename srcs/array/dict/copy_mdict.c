// MetaReal Programming Language version 1.0.0

#include <array/dict.h>

void copy_mdict(stack_t stack, heap_t heap, mdict_t dst, mdict_c src)
{
    dst->_node = copy_dict_node(stack, heap, src->_node);
    dst->_size = src->_size;
}
