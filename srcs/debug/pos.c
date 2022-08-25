// MetaReal Programming Language version 1.0.0

#include <debug/pos.h>

pos_t pos_set(uint64 idx, uint64 ln, cstr fn)
{
    pos_t pos;

    pos._idx = idx;
    pos._ln = ln;
    pos._fn = fn;

    return pos;
}
