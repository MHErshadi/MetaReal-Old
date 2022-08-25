// MetaReal Programming Language version 1.0.0

#include <stdlib.h>
#include <stdio.h>

#include <mrir/ival.h>
#include <mstr.h>
#include <debug/crash.h>

#define UINT64_MAX "18446744073709551615"

#define ill_op_error(t1, t2, o)                                                                            \
    do                                                                                                     \
    {                                                                                                      \
        uint64 siz = 40 + vals_len[t1] + vals_len[t2];                                                     \
        str det = malloc(siz);                                                                             \
                                                                                                           \
        if (!det)                                                                                          \
            mem_error(siz);                                                                                \
                                                                                                           \
        snprintf(det, siz, "Illegal Operation (%s) between <%s> and <%s>", o, vals_nam[t1], vals_nam[t2]); \
                                                                                                           \
        igres_fail(res, run_tim_set(ILL_OP, det, pss, pse, ictx));                                         \
        return op1;                                                                                        \
    } while (0);

iblk_t ival_add(igres_tp res, iblk_t op1, iblk_t op2, mem_t igmem, ictx_t ictx, pos_t pss, pos_t pse)
{
    if (op1._dtyp == INT_V)
    {
        if (op2._dtyp == INT_V)
        {
            if (OPT_LVL && IS_CONSTEXPR(op1._prop) && IS_CONSTEXPR(op2._prop))
            {
                uint64 val = ((int_i)op1._blk)->_val + ((int_i)op2._blk)->_val;

                int_i blk = iint_set(igmem, val);

                return iblk_set1(INT_I, blk, INT_V, 0, SET_PROP(0, 0, 1, 0));
            }

            bop_i blk = ibop_set(igmem, op1, op2, "addII");

            if (IS_USEFUL(op1._prop) || IS_USEFUL(op2._prop))
                return iblk_set1(BOP_I, blk, INT_V, ++res->_data._int_cnt, SET_PROP(1, 1, 0, 0));
            return iblk_set1(BOP_I, blk, INT_V, ++res->_data._int_cnt, SET_PROP(0, 1, 0, 0));
        }

        if (op2._dtyp == FLT_V)
        {
            if (OPT_LVL && IS_CONSTEXPR(op1._prop) && IS_CONSTEXPR(op2._prop))
            {
                dec64 val = ((int_i)op1._blk)->_val + ((flt_i)op2._blk)->_val;

                flt_i blk = iflt_set(igmem, val);

                return iblk_set1(FLT_I, blk, FLT_V, 0, SET_PROP(0, 0, 1, 0));
            }

            bop_i blk = ibop_set(igmem, op1, op2, "addIF");

            if (IS_USEFUL(op1._prop) || IS_USEFUL(op2._prop))
                return iblk_set1(BOP_I, blk, FLT_V, ++res->_data._flt_cnt, SET_PROP(1, 1, 0, 0));
            return iblk_set1(BOP_I, blk, FLT_V, ++res->_data._flt_cnt, SET_PROP(0, 1, 0, 0));
        }

        ill_op_error(INT_V, op2._dtyp, "+");
    }

    if (op1._dtyp == FLT_V)
    {
        if (op2._dtyp == INT_V)
        {
            if (OPT_LVL && IS_CONSTEXPR(op1._prop) && IS_CONSTEXPR(op2._prop))
            {
                dec64 val = ((flt_i)op1._blk)->_val + ((int_i)op2._blk)->_val;

                flt_i blk = iflt_set(igmem, val);

                return iblk_set1(FLT_I, blk, FLT_V, 0, SET_PROP(0, 0, 1, 0));
            }

            bop_i blk = ibop_set(igmem, op1, op2, "addFI");

            if (IS_USEFUL(op1._prop) || IS_USEFUL(op2._prop))
                return iblk_set1(BOP_I, blk, FLT_V, ++res->_data._flt_cnt, SET_PROP(1, 1, 0, 0));
            return iblk_set1(BOP_I, blk, FLT_V, ++res->_data._flt_cnt, SET_PROP(0, 1, 0, 0));
        }

        if (op2._dtyp == FLT_V)
        {
            if (OPT_LVL && IS_CONSTEXPR(op1._prop) && IS_CONSTEXPR(op2._prop))
            {
                dec64 val = ((flt_i)op1._blk)->_val + ((flt_i)op2._blk)->_val;

                flt_i blk = iflt_set(igmem, val);

                return iblk_set1(FLT_I, blk, FLT_V, 0, SET_PROP(0, 0, 1, 0));
            }

            bop_i blk = ibop_set(igmem, op1, op2, "addFF");

            if (IS_USEFUL(op1._prop) || IS_USEFUL(op2._prop))
                return iblk_set1(BOP_I, blk, FLT_V, ++res->_data._flt_cnt, SET_PROP(1, 1, 0, 0));
            return iblk_set1(BOP_I, blk, FLT_V, ++res->_data._flt_cnt, SET_PROP(0, 1, 0, 0));
        }

        ill_op_error(FLT_V, op2._dtyp, "+");
    }

    if (op1._dtyp == STR_V)
    {
        if (op2._dtyp == STR_V)
        {
            if (OPT_LVL && IS_CONSTEXPR(op1._prop) && IS_CONSTEXPR(op2._prop))
            {
                if (!op1._blk)
                {
                    if (!op2._blk)
                        return iblk_set2(STR_I, STR_V, 0, SET_PROP(0, 0, 1, 0));

                    str_i op1b = op1._blk;
                    str_i blk = istr_set(igmem, op1b->_val, op1b->_siz);

                    return iblk_set1(STR_I, blk, STR_V, 0, SET_PROP(0, 0, 1, 0));
                }

                if (!op2._blk)
                {
                    str_i op2b = op2._blk;
                    str_i blk = istr_set(igmem, op2b->_val, op2b->_siz);

                    return iblk_set1(STR_I, blk, STR_V, 0, SET_PROP(0, 0, 1, 0));
                }

                str_i op1b = op1._blk;
                str_i op2b = op2._blk;

                str val;
                uint64 siz = mstr_concat(igmem, IGMEM_SIZ, &val, op1b->_val, op1b->_siz, op2b->_val, op2b->_siz);

                str_i blk = istr_set(igmem, val, siz);

                return iblk_set1(STR_I, blk, STR_V, 0, SET_PROP(0, 0, 1, 0));
            }

            bop_i blk = ibop_set(igmem, op1, op2, "addSS");

            if (IS_USEFUL(op1._prop) || IS_USEFUL(op2._prop))
                return iblk_set1(BOP_I, blk, STR_V, ++res->_data._str_cnt, SET_PROP(1, 1, 0, 0));
            return iblk_set1(BOP_I, blk, STR_V, ++res->_data._str_cnt, SET_PROP(0, 1, 0, 0));
        }

        ill_op_error(STR_V, op2._dtyp, "+");
    }
}

uint64 conv_int(cstr val, uint64 len, uint8p flg)
{
    for (; *val == '0'; val++, len--);
    if (!len)
        return 0;

    if (len > 20)
    {
        *flg = 1;
        return 0;
    }

    if (len < 20)
    {
        uint64 res = *val++ - '0';

        for (; *val; val++)
            res = (res * 10) + *val - '0';

        return res;
    }

    if (*val > '1')
    {
        *flg = 1;
        return 0;
    }

    uint64 res = *val - '0';

    uint64 i;
    for (i = 1; i < 20; i++)
    {
        if (val[i] > UINT64_MAX[i])
        {
            *flg = 1;
            return 0;
        }

        res = (res * 10) + val[i] - '0';
    }

    return res;
}

dec64 conv_dec(cstr val, uint64 len, uint8p flg)
{
    for (; *val == '0'; val++, len--);
    if (len == 1)
        return 0;

    len--;

    for (; val[len] == '0'; len--);
    if (!len)
        return 0;

    len++;

    dec64 res = 0;
    uint64 div = 1;

    uint8 ps = 0;

    uint64 i;
    for (i = 0; i < len; i++)
    {
        if (val[i] == '.')
        {
            ps = 1;
            continue;
        }

        if (ps)
            div *= 10;
        res = res * 10 + (val[i] - '0');
    }

    return res / div;
}
