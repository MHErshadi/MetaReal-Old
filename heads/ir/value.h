// MetaReal Programming Language version 1.0.0

#ifndef __M_VALUE__
#define __M_VALUE__

#include <ir/gen.h>

enum _value_
{
    INT_V,
    FLOAT_V,
    
    BOOL_V,

    STR_V,

    LIST_V,
    TUPLE_V,
    DICT_V,
};

static cstr values_label[7] =
{
    "INT",
    "FLT",
    "BOL",
    "STR",
    "LST",
    "TPL",
    "DCT",
};

static cstr values_type[7] =
{
    "int",
    "flt",
    "bol",
    "str",
    "lst",
    "tpl",
    "dct",
};

static cstr values_name[7] =
{
    "integer",
    "float",
    "boolean",
    "string",
    "list",
    "tuple",
    "dictionary"
};

static const uint8 values_label_len[7] = {7, 5, 7, 6, 4, 5, 10};

block_t value_add(gres_p res, block_t op1, block_t op2, stack_t stack, heap_t heap, context_t context, pos_t poss, pos_t pose);

#endif /* __M_VALUE__ */
