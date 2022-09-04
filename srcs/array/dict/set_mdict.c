// MetaReal Programming Language version 1.0.0

#include <array/dict.h>

void set_mdict(mdict_t dst, dict_node_p node, uint64 size)
{
    dst->_node = node;
    dst->_size = size;
}
