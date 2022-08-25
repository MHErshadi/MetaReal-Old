// MetaReal Programming Language version 1.0.0

#ifndef __POS__
#define __POS__

#include <type.h>

struct __pos__
{
    uint64 _idx;
    uint64 _ln;

    cstr _fn;
};
typedef struct __pos__ pos_t;
typedef struct __pos__ *pos_tp;

pos_t pos_set(uint64 idx, uint64 ln, cstr fn);

#define pos_adv(p) p._idx++
#define pos_jmp(p) \
    do             \
    {              \
        p._idx++;  \
        p._ln++;   \
    } while (0)

#define posp_adv(p) p->_idx++
#define posp_jmp(p) \
    do              \
    {               \
        p->_idx++;  \
        p->_ln++;   \
    } while (0)

#endif /* __POS__ */
