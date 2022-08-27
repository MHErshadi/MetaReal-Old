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

struct __idata__
{
    uint64 _int_cnt;
    uint64 _flt_cnt;

    uint64 _bol_cnt;

    uint64 _str_cnt;

    uint64 _lst_cnt;
    uint64 _tpl_cnt;
    uint64 _dct_cnt;
};
typedef struct __idata__ idata_t;
typedef struct __idata__ *idata_tp;

struct __iblk__
{
    uint8 _typ;
    ptr _blk;

    uint8 _dtyp;
    uint64 _id;

    uint8 _prop; // 0b00000021 (1 : is useful) (2 : is complex)
};
typedef struct __iblk__ iblk_t;
typedef struct __iblk__ *iblk_tp;

#define SET_PROP(is_useful, is_complex, tail) 0b ## tail ## is_complex ## is_useful

#define IS_USEFUL(x) ((x) & 1)
#define IS_COMPLEX(x) ((x) >> 1 & 1)
#define IS_TRUE(x) ((x) >> 2)

struct __iint__
{
    int64 _val;
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

iblk_t iblk_set1(uint8 typ, ptr blk, uint8 dtyp, uint8 prop);
iblk_t iblk_set2(uint8 typ, uint8 dtyp, uint8 prop);

uint8 iblk_cmp(iblk_t op1, iblk_t op2);
uint8 iblkp_contains(iblk_tp blks, uint64 siz, iblk_t blk, uint64p pos);

void iblk_print(idata_tp data, iblk_tp blk, cstr end);

int_i iint_set(mem_t mem, int64 val);
flt_i iflt_set(mem_t mem, dec64 val);
str_i istr_set(mem_t mem, str val, uint64 siz);
lst_i ilst_set(mem_t mem, iblk_tp elms, uint64 siz);
tpl_i itpl_set(mem_t mem, iblk_tp elms, uint64 siz);
dct_i idct_set(mem_t mem, iblk_tp keys, iblk_tp vals, uint64 siz);
bop_i ibop_set(mem_t mem, iblk_t op1, iblk_t op2, cstr opr);

#endif /* __IBLK__ */
