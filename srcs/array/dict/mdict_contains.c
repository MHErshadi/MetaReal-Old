// MetaReal Programming Language version 1.0.0

#include <ir/block.h>

uint8 mdict_contains(mdict_c mdict, block_t key)
{
    return dict_node_contains(mdict->_node, key);
}
