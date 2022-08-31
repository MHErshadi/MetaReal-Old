// MetaReal Programming Language version 1.0.0

#include <debug/pos.h>

pos_t pos_set(uint64 index, uint64 line, cstr file_name)
{
    pos_t pos;

    pos._index = index;
    pos._line = line;
    pos._file_name = file_name;

    return pos;
}
