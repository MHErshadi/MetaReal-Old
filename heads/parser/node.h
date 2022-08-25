// MetaReal Programming Language version 1.0.0

#ifndef __NODE__
#define __NODE__

#include <mem/mem.h>
#include <lexer/tok.h>

enum _node_
{
    EOF_N, // end of file

    /* */

    INT_N, // integer
    FLT_N, // float

    BOL_N, // boolean

    STR_N, // string

    LST_N, // list
    TPL_N, // tuple
    DCT_N, // dictionary

    /* */

    BOP_N, // binary operation
    UOP_N, // unary operation

    TCO_N, // ternary conditional statement

    IDX_N, // array subscripting statement

    /* */

    VAS_N, // variable assign statement
    VRA_N, // variable re-assign statement
    VFA_N, // variable fixed-assign statement
    VAC_N, // variable access statement

    FDF_N, // function definition statement
    FCL_N, // function call statement

    CDF_N, // class definition statement

    SDF_N, // structure definition statement

    DFC_N, // dollar-function call statement

    /* */

    IFF_N, // if statement

    SWH_N, // switch statement

    /* */

    FOR_N, // for statement
    FRE_N, // foreach statement

    WHL_N, // while statement
    DOW_N, // do-while statement

    LOP_N, // loop statement

    /* */

    TRY_N, // try statement

    /* */

    RET_N, // return statement

    CNT_N, // continue statement
    BRK_N, // break statement

    PSS_N, // pass statement

    /* */

    IMP_N, // import statement
};

struct __node__
{
    uint8 _typ;
    ptr _nod;

    pos_t _pss;
    pos_t _pse;
};
typedef struct __node__ node_t;
typedef struct __node__ *node_tp;

struct __body__
{
    node_tp _nodes;
    uint64 _siz;
};
typedef struct __body__ body_t;
typedef struct __body__ *body_tp;

struct __stat__
{
    node_t _cond;
    body_t _body;
};
typedef struct __stat__ stat_t;
typedef struct __stat__ *stat_tp;

struct __arg__
{
    str _name;
    node_t _val;
};
typedef struct __arg__ arg_t;
typedef struct __arg__ *arg_tp;

struct __int__
{
    str _val;
    uint64 _len;
};
typedef struct __int__ *int_n;

struct __flt__
{
    str _val;
    uint64 _len;
};
typedef struct __flt__ *flt_n;

struct __bol__
{
    uint8 _stat : 1;
};
typedef struct __bol__ *bol_n;

struct __str__
{
    str _val;
    uint64 _len;
};
typedef struct __str__ *str_n;

struct __lst__
{
    node_tp _elms;
    uint64 _siz;
};
typedef struct __lst__ *lst_n;

struct __tpl__
{
    node_tp _elms;
    uint64 _siz;
};
typedef struct __tpl__ *tpl_n;

struct __dct__
{
    node_tp _keys;
    node_tp _vals;
    uint64 _siz;
};
typedef struct __dct__ *dct_n;

struct __bop__
{
    uint8 _opr;

    node_t _op1;
    node_t _op2;
};
typedef struct __bop__ *bop_n;

struct __uop__
{
    uint8 _opr;

    node_t _op;
};
typedef struct __uop__ *uop_n;

struct __tco__
{
    node_t _cond;

    node_t _op1;
    node_t _op2;
};
typedef struct __tco__ *tco_n;

struct __idx__
{
    node_t _arr;

    node_t _idx;
};
typedef struct __idx__ *idx_n;

struct __vas__
{
    str _name;
    node_t _val;

    uint8 _prop; // 0b00001234 (1 : access type) (2 : storage context) (3 : is constant) (4 : is static)
};
typedef struct __vas__ *vas_n;

struct __vra__
{
    uint8 _opr;

    node_t _var;
    node_t _val; 
};
typedef struct __vra__ *vra_n;

struct __vfa__
{
    uint8 _opr; // 0b00000012 (1 : operator type) (2 : is prefix or not)

    node_t _var;
};
typedef struct __vfa__ *vfa_n;

struct __vac__
{
    str _name;
};
typedef struct __vac__ *vac_n;

struct __fdf__
{
    str _name;

    arg_tp _args;
    uint64 _siz;

    body_t _body;

    uint8 _prop; // 0b0051234 (1 : access type) (2 : storage context) (3 : is constant) (4 : is static) (5 : is single-line)
};
typedef struct __fdf__ *fdf_n;

struct __fcl__
{
    node_t _func;

    arg_tp _args;
    uint64 _siz;
};
typedef struct __fcl__ *fcl_n;

struct __cdf__
{
    str _name;

    body_t _body;

    uint8 _prop; // 0b0001234 (1 : access type) (2 : storage context) (3 : is constant) (4 : is static)
};
typedef struct __cdf__ *cdf_n;

struct __sdf__
{
    str _name;

    body_t _body;

    uint8 _prop; // 0b0001234 (1 : access type) (2 : storage context) (3 : is constant) (4 : is static)
};
typedef struct __sdf__ *sdf_n;

struct __dfc__
{
    str _name;

    node_tp _args;
    uint64 _siz;
};
typedef struct __dfc__ *dfc_n;

struct __iff__
{
    stat_tp _stats;
    uint64 _siz;

    body_t _ebody;
};
typedef struct __iff__ *iff_n;

struct __swh__
{
    node_t _val;

    stat_tp _stats;
    uint64 _siz;

    body_t _dbody;
};
typedef struct __swh__ *swh_n;

struct __for__
{
    str _var;

    node_t _start;
    node_t _end;
    node_t _step;

    body_t _body;
};
typedef struct __for__ *for_n;

struct __fre__
{
    str _var;

    node_t _iter;

    body_t _body;
};
typedef struct __fre__ *fre_n;

struct __whl__
{
    node_t _cond;
    body_t _body;
};
typedef struct __whl__ *whl_n;

struct __dow__
{
    body_t _body;
    node_t _cond;
};
typedef struct __dow__ *dow_n;

struct __lop__
{
    node_t _init;
    node_t _cond;
    node_t _step;

    body_t _body;
};
typedef struct __lop__ *lop_n;

struct __try__
{
    body_t _error;

    stat_tp _stats;
    uint64 _siz;

    body_t _fbody;
};
typedef struct __try__ *try_n;

struct __ret__
{
    node_t _val;
};
typedef struct __ret__ *ret_n;

struct __imp__
{
    str _lib;
};
typedef struct __imp__ *imp_n;

node_t node_set1(uint8 typ, ptr nod, pos_t pss, pos_t pse);
node_t node_set2(uint8 typ, pos_t pss, pos_t pse);

body_t body_set(mem_t pmem, node_t nodes);

int_n int_set(mem_t pmem, str val, uint64 len);
flt_n flt_set(mem_t pmem, str val, uint64 len);

bol_n bol_set(mem_t pmem, uint8 stat);

str_n str_set(mem_t pmem, str val, uint64 len);

lst_n lst_set(mem_t pmem, node_tp elms, uint64 siz);
tpl_n tpl_set(mem_t pmem, node_tp elms, uint64 siz);
dct_n dct_set(mem_t pmem, node_tp keys, node_tp vals, uint64 siz);

/* */

bop_n bop_set(mem_t pmem, uint8 opr, node_t op1, node_t op2);
uop_n uop_set(mem_t pmem, uint8 opr, node_t op);

tco_n tco_set(mem_t pmem, node_t cond, node_t op1, node_t op2);

idx_n idx_set(mem_t pmem, node_t arr, node_t idx);

/* */

vas_n vas_set(mem_t pmem, str name, node_t val, uint8 prop);
vra_n vra_set(mem_t pmem, uint8 opr, node_t var, node_t val);
vfa_n vfa_set(mem_t pmem, uint8 opr, node_t var);
vac_n vac_set(mem_t pmem, str name);

fdf_n fdf_set(mem_t pmem, str name, arg_tp args, uint64 siz, body_t body, uint8 prop);
fcl_n fcl_set(mem_t pmem, node_t func, arg_tp args, uint64 siz);

cdf_n cdf_set(mem_t pmem, str name, body_t body, uint8 prop);

sdf_n sdf_set(mem_t pmem, str name, body_t body, uint8 prop);

dfc_n dfc_set(mem_t pmem, str name, node_tp args, uint64 siz);

/* */

iff_n iff_set(mem_t pmem, stat_tp stats, uint64 siz, body_t ebody);

swh_n swh_set(mem_t pmem, node_t val, stat_tp stats, uint64 siz, body_t dbody);

/* */

for_n for_set(mem_t pmem, str var, node_t start, node_t end, node_t step, body_t body);
fre_n fre_set(mem_t pmem, str var, node_t iter, body_t body);

whl_n whl_set(mem_t pmem, node_t cond, body_t body);
dow_n dow_set(mem_t pmem, body_t body, node_t cond);

lop_n lop_set(mem_t pmem, node_t init, node_t cond, node_t step, body_t body);

/* */

try_n try_set(mem_t pmem, body_t error, stat_tp stats, uint64 siz, body_t fbody);

/* */

ret_n ret_set(mem_t pmem, node_t val);

/* */

imp_n imp_set(mem_t pmem, str lib);

#endif /* __NODE__ */
