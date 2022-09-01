// MetaReal Programming Language version 1.0.0

#ifndef __M_BLOCK__
#define __M_BLOCK__

#include <mfloat/mfloat.h>
#include <mstr/mstr.h>
#include <array/list.h>

enum _block_
{
    NULL_I, // null block

    /* */

    INT_I, // integer block
    FLOAT_I, // float block

    BOOL_I, // boolean block

    STR_I, // string block

    LIST_I, // list block
    TUPLE_I, // tuple block
    DICT_I, // dictionary block

    /* */

    BINARY_OPERATION_I, // binary operation block
};

struct __data__
{
    uint64 _int_count;
    uint64 _float_count;

    uint64 _bool_count;

    uint64 _str_count;

    uint64 _list_count;
    uint64 _tuple_count;
    uint64 _dict_count;
};
typedef struct __data__ data_t;
typedef struct __data__ *data_p;

struct __int_i__
{
    mint_t _value;
};
typedef struct __int_i__ *int_i;

struct __float_i__
{
    mfloat_t _value;
};
typedef struct __float_i__ *float_i;

struct __str_i__
{
    mstr_t _value;
};
typedef struct __str_i__ *str_i;

struct __list_i__
{
    mlist_t _value;
};
typedef struct __list_i__ *list_i;

struct __tuple_i__
{
    mtuple_t _value;
};
typedef struct __tuple_i__ *tuple_i;

struct __dict_i__
{
    block_p _keys;
    block_p _values;
    uint64 _size;
};
typedef struct __dict_i__ *dict_i;

struct __binary_operation_i__
{
    block_t _op1;
    block_t _op2;

    cstr _operator;
};
typedef struct __binary_operation_i__ *binary_operation_i;

block_t block_set1(uint8 type, ptr block, uint8 dtype, uint8 properties);
block_t block_set2(uint8 type, uint8 dtype, uint8 properties);

block_t block_copy(block_t block, stack_t stack, heap_t heap);
void block_free(heap_t heap, block_t block);

uint8 block_equal(block_t op1, block_t op2);

void block_print(data_p data, block_p block, cstr end);

int_i int_i_set(stack_t stack, mint_t value);
float_i float_i_set(stack_t stack, mfloat_t value);
str_i str_i_set(stack_t stack, mstr_t value);
list_i list_i_set(stack_t stack, mlist_t value);
tuple_i tuple_i_set(stack_t stack, mtuple_t value);
dict_i dict_i_set(stack_t stack, block_p keys, block_p values, uint64 size);
binary_operation_i binary_operation_i_set(stack_t stack, block_t op1, block_t op2, cstr operator);

#endif /* __M_BLOCK__ */
