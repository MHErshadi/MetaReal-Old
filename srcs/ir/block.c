// MetaReal Programming Language version 1.0.0

#include <ir/block.h>
#include <ir/value.h>
#include <def.h>

void block_label(data_p data, block_p block, cstr end);

uint64 block_id(data_p data, uint8 dtype);

block_t block_set1(uint8 type, ptr block, uint8 dtype, uint8 properties)
{
    block_t block_;

    block_._type = type;
    block_._block = block;
    block_._dtype = dtype;
    block_._id = 0;
    block_._properties = properties;

    return block_;
}

block_t block_set2(uint8 type, uint8 dtype, uint8 properties)
{
    block_t block;

    block._type = type;
    block._block = NULL;
    block._dtype = dtype;
    block._id = 0;
    block._properties = properties;

    return block;
}

block_t block_copy(block_t block, stack_t stack, heap_t heap)
{
    if (block._type == NULL_I)
        return block;

    if (block._type == INT_I)
    {
        mint_t value;
        copy_mint(value, ((int_i)block._block)->_value);

        int_i block_ = int_i_set(stack, value);

        return block_set1(INT_I, block_, INT_V, block._properties);
    }
    if (block._type == FLOAT_I)
    {
        mfloat_t value;
        copy_mfloat(value, ((float_i)block._block)->_value);

        float_i block_ = float_i_set(stack, value);

        return block_set1(FLOAT_I, block_, FLOAT_V, block._properties);
    }
    if (block._type == BOOL_I)
        return block;
    if (block._type == STR_I)
    {
        if (!block._block)
            return block;

        mstr_t value;
        copy_mstr(heap, value, ((str_i)block._block)->_value);

        str_i block_ = str_i_set(stack, value);

        return block_set1(STR_I, block_, STR_V, block._properties);
    }
    if (block._type == LIST_I)
    {
        if (!block._block)
            return block;

        mlist_t value;
        copy_mlist(stack, heap, value, ((list_i)block._block)->_value);

        list_i block_ = list_i_set(stack, value);

        return block_set1(LIST_I, block_, LIST_V, block._properties);
    }
    if (block._type == TUPLE_I)
    {
        if (!block._block)
            return block;

        mtuple_t value;
        copy_mtuple(stack, heap, value, ((tuple_i)block._block)->_value);

        tuple_i block_ = tuple_i_set(stack, value);

        return block_set1(TUPLE_I, block_, TUPLE_V, block._properties);
    }
}

void block_free(heap_t heap, block_t block)
{
    if (block._type == STR_I && block._block)
        free_mstr(heap, ((str_i)block._block)->_value);
    if (block._type == LIST_I && block._block)
        free_mlist(heap, ((list_i)block._block)->_value);
    if (block._type == TUPLE_I && block._block)
        free_mtuple(heap, ((tuple_i)block._block)->_value);
}

uint8 block_equal(block_t op1, block_t op2)
{
    if (op1._type != op2._type)
        return 0;

    switch (op1._type)
    {
    case NULL_I:
        return 1;
    case INT_I:
        return mint_equal(((int_i)op1._block)->_value, ((int_i)op2._block)->_value);
    case FLOAT_I:
        return mfloat_equal(((float_i)op1._block)->_value, ((float_i)op2._block)->_value);
    case BOOL_I:
        return IS_TRUE(op1._properties) == IS_TRUE(op2._properties);
    case STR_I:
        return mstr_equal(((str_i)op1._block)->_value, ((str_i)op2._block)->_value);
    case LIST_I:
        return mlist_equal(((list_i)op1._block)->_value, ((list_i)op2._block)->_value);
    case TUPLE_I:
        return mtuple_equal(((tuple_i)op1._block)->_value, ((tuple_i)op2._block)->_value);
    }
}

void block_print(data_p data, block_p block, cstr end)
{
    if (block->_type == NULL_I)
        return;

    /* */

    if (block->_type == INT_I)
    {
        fprintf(STDOUT, "%llu%s", ((int_i)block->_block)->_value->_value, end);
        return;
    }
    if (block->_type == FLOAT_I)
    {
        fprintf(STDOUT, "%lf%s", ((float_i)block->_block)->_value->_value, end);
        return;
    }

    if (block->_type == BOOL_I)
    {
        fprintf(STDOUT, "%u%s", IS_TRUE(block->_properties), end);
        return;
    }

    if (block->_type == STR_I)
    {
        if (!block->_block)
        {
            fprintf(STDOUT, "\"\"%s", end);
            return;
        }

        fprintf(STDOUT, "\"%s\"%s", ((str_i)block->_block)->_value->_str, end);
        return;
    }

    if (block->_type == LIST_I)
    {
        if (!block->_block)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        list_i list = block->_block;

        uint64 i;
        for (i = 0; i < list->_value->_size; i++)
            if (IS_COMPLEX(list->_value->_elements[i]._properties))
                block_print(data, &list->_value->_elements[i], ";\n");

        if (!block->_id)
            block->_id = ++data->_list_count;
        fprintf(STDOUT, "$LST%llu |lst = {", block->_id);

        for (i = 0; i < list->_value->_size; i++)
            block_print(data, &list->_value->_elements[i], ", ");

        fprintf(STDOUT, "\b\b}%s", end);
        return;
    }
    if (block->_type == TUPLE_I)
    {
        if (!block->_block)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        tuple_i tuple = block->_block;

        uint64 i;
        for (i = 0; i < tuple->_value->_size; i++)
            if (IS_COMPLEX(tuple->_value->_elements[i]._properties))
                block_print(data, &tuple->_value->_elements[i], ";\n");

        if (!block->_id)
            block->_id = ++data->_tuple_count;
        fprintf(STDOUT, "$TPL%llu |tpl = {", block->_id);

        for (i = 0; i < tuple->_value->_size; i++)
            block_label(data, &tuple->_value->_elements[i], ", ");

        fprintf(STDOUT, "\b\b}%s", end);
        return;
    }
    if (block->_type == DICT_I)
    {
        if (!block->_block)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        dict_i dict = block->_block;

        uint64 i;
        for (i = 0; i < dict->_size; i++)
            if (IS_COMPLEX(dict->_keys[i]._properties))
                block_print(data, &dict->_keys[i], ";\n");

        for (i = 0; i < dict->_size; i++)
            if (IS_COMPLEX(dict->_values[i]._properties))
                block_print(data, &dict->_values[i], ";\n");

        fprintf(STDOUT, "$KEYS |lst = {");

        for (i = 0; i < dict->_size; i++)
            block_label(data, &dict->_keys[i], ", ");

        fprintf(STDOUT, "\b\b};\n$VALS |lst = {");

        for (i = 0; i < dict->_size; i++)
            block_label(data, &dict->_values[i], ", ");

        if (!block->_id)
            block->_id = ++data->_dict_count;
        fprintf(STDOUT, "\b\b};\n$DCT%llu |dct = {$KEYS, $VALS}%s", block->_id, end);
        return;
    }

    /* */

    if (block->_type == BINARY_OPERATION_I)
    {
        binary_operation_i binary_operation = block->_block;

        if (IS_COMPLEX(binary_operation->_op1._properties))
            block_print(data, &binary_operation->_op1, ";\n");
        if (IS_COMPLEX(binary_operation->_op2._properties))
            block_print(data, &binary_operation->_op2, ";\n");

        if (!block->_id)
            block->_id = block_id(data, block->_dtype);
        fprintf(STDOUT, "$%s%llu = ~%s ", values_label[block->_dtype], block->_id, binary_operation->_operator);

        block_label(data, &binary_operation->_op1, ", ");
        block_label(data, &binary_operation->_op2, ";\n");
        return;
    }
}

int_i int_i_set(stack_t stack, mint_t value)
{
    int_i block = stack_alloc(stack, sizeof(struct __int__));

    block->_value->_value = value->_value;

    return block;
}

float_i float_i_set(stack_t stack, mfloat_t value)
{
    float_i block = stack_alloc(stack, sizeof(struct __float__));

    block->_value->_value = value->_value;

    return block;
}

str_i str_i_set(stack_t stack, mstr_t value)
{
    str_i block = stack_alloc(stack, sizeof(struct __str__));

    block->_value->_str = value->_str;
    block->_value->_size = value->_size;

    return block;
}

list_i list_i_set(stack_t stack, mlist_t value)
{
    list_i block = stack_alloc(stack, sizeof(struct __list__));

    block->_value->_elements = value->_elements;
    block->_value->_size = value->_size;

    return block;
}

tuple_i tuple_i_set(stack_t stack, mtuple_t value)
{
    tuple_i block = stack_alloc(stack, sizeof(struct __tuple__));

    block->_value->_elements = value->_elements;
    block->_value->_size = value->_size;

    return block;
}

dict_i dict_i_set(stack_t stack, block_p keys, block_p values, uint64 size)
{
    dict_i block = stack_alloc(stack, sizeof(struct __dict__));

    block->_keys = keys;
    block->_values = values;
    block->_size = size;

    return block;
}

binary_operation_i binary_operation_i_set(stack_t stack, block_t op1, block_t op2, cstr operator)
{
    binary_operation_i block = stack_alloc(stack, sizeof(struct __binary_operation__));

    block->_op1 = op1;
    block->_op2 = op2;
    block->_operator = operator;

    return block;
}

void block_label(data_p data, block_p block, cstr end)
{
    if (block->_type == NULL_I)
        return;

    /* */

    if (block->_type == INT_I)
    {
        fprintf(STDOUT, "%llu%s", ((int_i)block->_block)->_value->_value, end);
        return;
    }
    if (block->_type == FLOAT_I)
    {
        fprintf(STDOUT, "%lf%s", ((float_i)block->_block)->_value->_value, end);
        return;
    }

    if (block->_type == BOOL_I)
    {
        fprintf(STDOUT, "%u%s", IS_TRUE(block->_properties), end);
        return;
    }

    if (block->_type == STR_I)
    {
        if (!block->_block)
        {
            fprintf(STDOUT, "\"\"%s", end);
            return;
        }

        fprintf(STDOUT, "\"%s\"%s", ((str_i)block->_block)->_value->_str, end);
        return;
    }

    if (block->_type == LIST_I)
    {
        if (!block->_block)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        fprintf(STDOUT, "$LST%llu%s", block->_id, end);
        return;
    }
    if (block->_type == TUPLE_I)
    {
        if (!block->_block)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        fprintf(STDOUT, "$TPL%llu%s", block->_id, end);
        return;
    }
    if (block->_type == DICT_I)
    {
        if (!block->_block)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        fprintf(STDOUT, "$DCT%llu%s", block->_id, end);
        return;
    }

    /* */

    if (block->_type == BINARY_OPERATION_I)
    {
        fprintf(STDOUT, "$%s%llu%s", values_label[block->_dtype], block->_id, end);
        return;
    }
}

uint64 block_id(data_p data, uint8 dtype)
{
    switch (dtype)
    {
    case INT_V:
        return ++data->_int_count;
    case FLOAT_V:
        return ++data->_float_count;
    case BOOL_V:
        return ++data->_bool_count;
    case STR_V:
        return ++data->_str_count;
    case LIST_V:
        return ++data->_list_count;
    case TUPLE_V:
        return ++data->_tuple_count;
    case DICT_V:
        return ++data->_dict_count;
    }
}
