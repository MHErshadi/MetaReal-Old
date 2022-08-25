// MetaReal Programming Language version 1.0.0

#ifndef __ICTX__
#define __ICTX__

#include <debug/pos.h>

struct __ictx__
{
    cstr _name;

    struct __ictx__ *_prn;
    pos_t _prn_pos;
};
typedef struct __ictx__ ictx_t;
typedef struct __ictx__ *ictx_tp;

ictx_t ictx_set1(cstr name, ictx_tp prn, pos_t prn_pos);
ictx_t ictx_set2(cstr name);

#endif /* __ICTX__ */
