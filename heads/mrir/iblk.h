// MetaReal Programming Language version 1.0.0

#ifndef __IBLK__
#define __IBLK__

#include <mem/mem.h>

enum _iblk_
{
    INT_I, // integer block
    FLT_I, // float block

    BOL_I, // boolean block

    STR_I, // string block
};

struct __iblk__
{
    uint8 _typ;
    ptr _blk;

    str _inst;
    uint64 _siz;

    uint8 _prop; // 0b00000321 (1 : is useful) (2 : is complex) (3 : is constant expression)
};
typedef struct __iblk__ iblk_t;
typedef struct __iblk__ *iblk_tp;

#define SET_PROP(is_useful, is_complex, is_constexpr) 0b ## is_constexpr ## is_complex ## is_useful

#define IS_USEFUL(x) ((x) & 1)
#define IS_COMPLEX(x) ((x) >> 1 & 1)
#define IS_CONSTEXPR(x) ((x) >> 2 & 1)

iblk_t iblk_set1(uint8 typ, ptr blk, str inst, uint64 siz, uint8 prop);
iblk_t iblk_set2(uint8 typ, str inst, uint64 siz, uint8 prop);
iblk_t iblk_set3(uint8 typ, uint8 prop);

void iblk_print(iblk_t blk);

#endif /* __IBLK__ */
