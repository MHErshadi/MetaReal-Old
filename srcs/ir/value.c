// MetaReal Programming Language version 1.0.0

#include <ir/value.h>
#include <stdlib.h>
#include <stdio.h>
#include <crash.h>

#define UINT64_MAX "9223372036854775807"

#define illegal_operation_error(t1, t2, o)                                                                           \
    do                                                                                                               \
    {                                                                                                                \
        uint64 size = 40 + values_label_len[t1] + values_label_len[t2];                                              \
                                                                                                                     \
        str detail = malloc(size);                                                                                   \
        if (!detail)                                                                                                 \
            alloc_error(size);                                                                                       \
                                                                                                                     \
        snprintf(detail, size, "Illegal Operation (%s) between <%s> and <%s>", o, values_name[t1], values_name[t2]); \
                                                                                                                     \
        gres_fail(res, runtime_set(ILLEGAL_OPERATION, detail, poss, pose, context));                                 \
        return op1;                                                                                                  \
    } while (0)

block_t value_add(gres_p res, block_t op1, block_t op2, stack_t stack, heap_t heap, context_t context, pos_t poss, pos_t pose)
{
    if (op1._dtype == INT_V)
    {
        if (op2._dtype == INT_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._properties) && !IS_USEFUL(op2._properties))
            {
                int_i op1b = op1._block;
                mint_add(op1b->_value, ((int_i)op2._block)->_value);

                return op1;
            }

            binary_operation_i block = binary_operation_i_set(stack, op1, op2, "addII");

            return block_set1(BINARY_OPERATION_I, block, INT_V, SET_PROPERTIES(1, 1, 0));
        }

        if (op2._dtype == FLOAT_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._properties) && !IS_USEFUL(op2._properties))
            {
                mfloat_t op1v;
                set_mfloat_mint(op1v, ((int_i)op1._block)->_value);

                mfloat_add(op1v, ((float_i)op2._block)->_value);

                float_i block = float_i_set(stack, op1v);

                return block_set1(FLOAT_I, block, FLOAT_V, SET_PROPERTIES(0, 0, 0));
            }

            binary_operation_i block = binary_operation_i_set(stack, op1, op2, "addIF");

            return block_set1(BINARY_OPERATION_I, block, FLOAT_V, SET_PROPERTIES(1, 1, 0));
        }

        illegal_operation_error(INT_V, op2._dtype, "+");
    }

    if (op1._dtype == FLOAT_V)
    {
        if (op2._dtype == INT_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._properties) && !IS_USEFUL(op2._properties))
            {
                float_i op1b = op1._block;
                mfloat_t op2v;
                set_mfloat_mint(op2v, ((int_i)op2._block)->_value);

                mfloat_add(op1b->_value, op2v);

                return op1;
            }

            binary_operation_i block = binary_operation_i_set(stack, op1, op2, "addFI");

            return block_set1(BINARY_OPERATION_I, block, FLOAT_V, SET_PROPERTIES(1, 1, 0));
        }

        if (op2._dtype == FLOAT_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._properties) && !IS_USEFUL(op2._properties))
            {
                float_i op1b = op1._block;
                mfloat_add(op1b->_value, ((float_i)op2._block)->_value);

                return op1;
            }

            binary_operation_i block = binary_operation_i_set(stack, op1, op2, "addFF");

            return block_set1(BINARY_OPERATION_I, block, FLOAT_V, SET_PROPERTIES(1, 1, 0));
        }

        illegal_operation_error(FLOAT_V, op2._dtype, "+");
    }

    if (op1._dtype == STR_V)
    {
        if (op2._dtype == STR_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._properties) && !IS_USEFUL(op2._properties))
            {
                if (!op1._block)
                    return op2;

                if (!op2._block)
                    return op1;

                str_i op1b = op1._block;
                mstr_concat(heap, op1b->_value, ((str_i)op2._block)->_value);

                return op1;
            }

            binary_operation_i block = binary_operation_i_set(stack, op1, op2, "addSS");

            return block_set1(BINARY_OPERATION_I, block, STR_V, SET_PROPERTIES(1, 1, 0));
        }

        illegal_operation_error(STR_V, op2._dtype, "+");
    }

    if (op1._dtype == LIST_V)
    {
        if (op2._dtype == LIST_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._properties) && !IS_USEFUL(op2._properties))
            {
                if (!op1._block)
                    return op2;

                if (!op2._block)
                    return op1;

                list_i op1b = op1._block;
                mlist_concat(heap, op1b->_value, ((list_i)op2._block)->_value);

                return op1;
            }

            binary_operation_i block = binary_operation_i_set(stack, op1, op2, "addLL");

            return block_set1(BINARY_OPERATION_I, block, LIST_V, SET_PROPERTIES(1, 1, 0));
        }

        if (op2._dtype == TUPLE_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._properties) && !IS_USEFUL(op2._properties))
            {
                if (!op1._block)
                    return op2;

                if (!op2._block)
                    return op1;

                list_i op1b = op1._block;
                mlist_concat_mtuple(heap, op1b->_value, ((tuple_i)op2._block)->_value);

                return op1;
            }

            binary_operation_i block = binary_operation_i_set(stack, op1, op2, "addLT");

            return block_set1(BINARY_OPERATION_I, block, LIST_V, SET_PROPERTIES(1, 1, 0));
        }

        if (OPT_LVL && !IS_USEFUL(op1._properties) && !IS_USEFUL(op2._properties))
        {
            if (!op1._block)
            {
                block_p elements = heap_alloc(heap, sizeof(block_t));
                *elements = op2;

                mlist_t value;
                set_mlist(value, elements, 1);

                list_i block = list_i_set(stack, value);
                op1._block = block;

                return op1;
            }

            list_i op1b = op1._block;
            mlist_append(heap, op1b->_value, op2);

            return op1;
        }

        binary_operation_i block;

        switch (op2._dtype)
        {
        case INT_V:
            block = binary_operation_i_set(stack, op1, op2, "addLI");
        case FLOAT_V:
            block = binary_operation_i_set(stack, op1, op2, "addLF");
        case BOOL_V:
            block = binary_operation_i_set(stack, op1, op2, "addLB");
        case STR_V:
            block = binary_operation_i_set(stack, op1, op2, "addLS");
        }

        return block_set1(BINARY_OPERATION_I, block, LIST_V, SET_PROPERTIES(1, 1, 0));
    }

    illegal_operation_error(op1._dtype, op2._dtype, "+");
}
