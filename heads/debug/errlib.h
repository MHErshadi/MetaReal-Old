// MetaReal Programming Language version 1.0.0

#ifndef __ERRLIB__
#define __ERRLIB__

#include <mrir/ictx.h>

enum _rte_typ_
{
    ILL_OP,
    DIV_BY_ZERO,
};

struct __ill_chr__
{
    uint8 _chr;
    pos_t _pos;
};
typedef struct __ill_chr__ ill_chr_t;

struct __inv_syn__
{
    cstr _det;

    pos_t _pss;
    pos_t _pse;
};
typedef struct __inv_syn__ inv_syn_t;

struct __run_tim__
{
    uint8 _typ;
    str _det;

    pos_t _pss;
    pos_t _pse;

    ictx_t _ictx;
};
typedef struct __run_tim__ run_tim_t;

ill_chr_t ill_chr_set(uint8 chr, pos_t pos);
void ill_chr_print(ill_chr_t error, cstr code, uint64 siz);

inv_syn_t inv_syn_set(cstr det, pos_t pss, pos_t pse);
void inv_syn_print(inv_syn_t error, cstr code, uint64 siz);

run_tim_t run_tim_set(uint8 typ, str det, pos_t pss, pos_t pse, ictx_t ictx);
void run_tim_print(run_tim_t error, cstr code, uint64 siz);

#endif /* __ERRLIB__ */
