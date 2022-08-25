// MetaReal Programming Language version 1.0.0

#ifndef __IBLK__
#define __IBLK__

#include <mem/mem.h>

enum _iblk_
{
    NLL_I, // null block

    /* */

    INT_I, // integer block
    FLT_I, // float block

    BOL_I, // boolean block

    STR_I, // string block

    LST_I, // list block
    TPL_I, // tuple block
    DCT_I, // dictionary block

    /* */

    BOP_I, // binary operation block
};

struct __iblk__
{
    uint8 _typ;
    ptr _blk;

    uint8 _dtyp;
    uint64 _id;

    uint8 _prop; // 0b00000321 (1 : is useful) (2 : is complex) (3 : is constant expression)
};
typedef struct __iblk__ iblk_t;
typedef struct __iblk__ *iblk_tp;

#define SET_PROP(is_useful, is_complex, is_constexpr, tail) 0b ## tail ## is_constexpr ## is_complex ## is_useful

#define IS_USEFUL(x) ((x) & 1)
#define IS_COMPLEX(x) ((x) >> 1 & 1)
#define IS_CONSTEXPR(x) ((x) >> 2 & 1)

#define BOOL_STAT(x) ((x) >> 3)

struct __iint__
{
    uint64 _val;
};
typedef struct __iint__ *int_i;

struct __iflt__
{
    dec64 _val;
};
typedef struct __iflt__ *flt_i;

struct __istr__
{
    str _val;
    uint64 _siz;
};
typedef struct __istr__ *str_i;

struct __ilst__
{
    iblk_tp _elms;
    uint64 _siz;
};
typedef struct __ilst__ *lst_i;

struct __itpl__
{
    iblk_tp _elms;
    uint64 _siz;
};
typedef struct __itpl__ *tpl_i;

struct __idct__
{
    iblk_tp _keys;
    iblk_tp _vals;
    uint64 _siz;
};
typedef struct __idct__ *dct_i;

struct __ibop__
{
    iblk_t _op1;
    iblk_t _op2;

    cstr _opr;
};
typedef struct __ibop__ *bop_i;

iblk_t iblk_set1(uint8 typ, ptr blk, uint8 dtyp, uint64 id, uint8 prop);
iblk_t iblk_set2(uint8 typ, uint8 dtyp, uint64 id, uint8 prop);

void iblk_print(iblk_t blk, cstr end);

int_i iint_set(mem_t igmem, uint64 val);
flt_i iflt_set(mem_t igmem, dec64 val);
str_i istr_set(mem_t igmem, str val, uint64 siz);
lst_i ilst_set(mem_t igmem, iblk_tp elms, uint64 siz);
tpl_i itpl_set(mem_t igmem, iblk_tp elms, uint64 siz);
dct_i idct_set(mem_t igmem, iblk_tp keys, iblk_tp vals, uint64 siz);
bop_i ibop_set(mem_t igmem, iblk_t op1, iblk_t op2, cstr opr);

#endif /* __IBLK__ */
