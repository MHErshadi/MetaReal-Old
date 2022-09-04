// MetaReal Programming Language version 1.0.0

#include <array/dict.h>

void free_mdict(heap_t heap, mdict_t mdict)
{
    free_dict_node(heap, mdict->_node);
}
