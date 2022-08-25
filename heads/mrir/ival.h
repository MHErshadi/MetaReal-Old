// MetaReal Programming Language version 1.0.0

#ifndef __IVAL__
#define __IVAL__

#include <mrir/irgen.h>
#include <type.h>

enum _ival_
{
    INT_V,
    FLT_V,
    
    BOL_V,

    STR_V,

    LST_V,
    TPL_V,
    DCT_V,
};

static cstr vals_lbl[7] =
{
    "INT",
    "FLT",
    "BOL",
    "STR",
    "LST",
    "TPL",
    "DCT",
};

static cstr vals_typ[7] =
{
    "int",
    "flt",
    "bol",
    "str",
    "lst",
    "tpl",
    "dct",
};

static cstr vals_nam[7] =
{
    "integer",
    "float",
    "boolean",
    "string",
    "list",
    "tuple",
    "dictionary"
};

static const uint8 vals_len[7] = {7, 5, 7, 6, 4, 5, 10};

iblk_t ival_add(igres_tp res, iblk_t op1, iblk_t op2, mem_t igmem, ictx_t ictx, pos_t pss, pos_t pse);

uint64 conv_int(cstr val, uint64 len, uint8p flg);
dec64 conv_dec(cstr val, uint64 len, uint8p flg);

#endif /* __IVAL__ */
