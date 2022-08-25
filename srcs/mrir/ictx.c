// MetaReal Programming Language version 1.0.0

#include <mrir/ictx.h>

ictx_t ictx_set1(cstr name, ictx_tp prn, pos_t prn_pos)
{
    ictx_t ictx;

    ictx._name = name;
    ictx._prn = prn;
    ictx._prn_pos = prn_pos;

    return ictx;
}

ictx_t ictx_set2(cstr name)
{
    ictx_t ictx;

    ictx._name = name;

    return ictx;
}
