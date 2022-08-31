// MetaReal Programming Language version 1.0.0

#include <stdlib.h>
#include <stdio.h>

#include <mrir/ival.h>
#include <mstr.h>
#include <mrir/ilst.h>
#include <mrir/idct.h>
#include <mem/blk.h>
#include <debug/crash.h>

#define UINT64_MAX "9223372036854775807"

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
    } while (0)

block_t value_add(gres_p res, block_t op1, block_t op2, stack_t stack, heap_t heap, context_t context, pos_t poss, pos_t pose)
{
    if (op1._dtyp == INT_V)
    {
        if (op2._dtyp == INT_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._prop) && !IS_USEFUL(op2._prop))
            {
                int64 val = ((int_i)op1._blk)->_val + ((int_i)op2._blk)->_val;

                int_i blk = iint_set(mem, val);

                return iblk_set1(INT_I, blk, INT_V, SET_PROP(0, 0, 0));
            }

            bop_i blk = ibop_set(mem, op1, op2, "addII");

            return iblk_set1(BOP_I, blk, INT_V, SET_PROP(1, 1, 0));
        }

        if (op2._dtyp == FLT_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._prop) && !IS_USEFUL(op2._prop))
            {
                dec64 val = ((int_i)op1._blk)->_val + ((flt_i)op2._blk)->_val;

                flt_i blk = iflt_set(mem, val);

                return iblk_set1(FLT_I, blk, FLT_V, SET_PROP(0, 0, 0));
            }

            bop_i blk = ibop_set(mem, op1, op2, "addIF");

            return iblk_set1(BOP_I, blk, FLT_V, SET_PROP(1, 1, 0));
        }

        ill_op_error(INT_V, op2._dtyp, "+");
    }

    if (op1._dtyp == FLT_V)
    {
        if (op2._dtyp == INT_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._prop) && !IS_USEFUL(op2._prop))
            {
                dec64 val = ((flt_i)op1._blk)->_val + ((int_i)op2._blk)->_val;

                flt_i blk = iflt_set(mem, val);

                return iblk_set1(FLT_I, blk, FLT_V, SET_PROP(0, 0, 0));
            }

            bop_i blk = ibop_set(mem, op1, op2, "addFI");

            return iblk_set1(BOP_I, blk, FLT_V, SET_PROP(1, 1, 0));
        }

        if (op2._dtyp == FLT_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._prop) && !IS_USEFUL(op2._prop))
            {
                dec64 val = ((flt_i)op1._blk)->_val + ((flt_i)op2._blk)->_val;

                flt_i blk = iflt_set(mem, val);

                return iblk_set1(FLT_I, blk, FLT_V, SET_PROP(0, 0, 0));
            }

            bop_i blk = ibop_set(mem, op1, op2, "addFF");

            return iblk_set1(BOP_I, blk, FLT_V, SET_PROP(1, 1, 0));
        }

        ill_op_error(FLT_V, op2._dtyp, "+");
    }

    if (op1._dtyp == STR_V)
    {
        if (op2._dtyp == STR_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._prop) && !IS_USEFUL(op2._prop))
            {
                if (!op1._blk)
                {
                    if (!op2._blk)
                        return iblk_set2(STR_I, STR_V, SET_PROP(0, 0, 0));

                    str_i op1b = op1._blk;
                    str_i blk = istr_set(mem, op1b->_val, op1b->_siz);

                    return iblk_set1(STR_I, blk, STR_V, SET_PROP(0, 0, 0));
                }

                if (!op2._blk)
                {
                    str_i op2b = op2._blk;
                    str_i blk = istr_set(mem, op2b->_val, op2b->_siz);

                    return iblk_set1(STR_I, blk, STR_V, SET_PROP(0, 0, 0));
                }

                str_i op1b = op1._blk;
                str_i op2b = op2._blk;

                str val;
                uint64 siz = mstr_concat(mem, MEM_SIZ, &val, op1b->_val, op1b->_siz, op2b->_val, op2b->_siz);

                str_i blk = istr_set(mem, val, siz);

                return iblk_set1(STR_I, blk, STR_V, SET_PROP(0, 0, 0));
            }

            bop_i blk = ibop_set(mem, op1, op2, "addSS");

            return iblk_set1(BOP_I, blk, STR_V, SET_PROP(1, 1, 0));
        }

        ill_op_error(STR_V, op2._dtyp, "+");
    }

    if (op1._dtyp == LST_V)
    {
        if (op2._dtyp == LST_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._prop) && !IS_USEFUL(op2._prop))
            {
                if (!op1._blk)
                {
                    if (!op2._blk)
                        return iblk_set2(LST_I, LST_V, SET_PROP(0, 0, 0));

                    return op2;
                }

                if (!op2._blk)
                    return op1;

                lst_i op2b = op2._blk;

                lst_i blk = ilst_concat(mem, op1._blk, op2b->_elms, op2b->_siz);

                return iblk_set1(LST_I, blk, LST_V, SET_PROP(0, 1, 0));
            }

            bop_i blk = ibop_set(mem, op1, op2, "addLL");

            return iblk_set1(BOP_I, blk, LST_V, SET_PROP(1, 1, 0));
        }

        if (op2._dtyp == TPL_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._prop) && !IS_USEFUL(op2._prop))
            {
                if (!op1._blk)
                {
                    if (!op2._blk)
                        return iblk_set2(LST_I, LST_V, SET_PROP(0, 0, 0));

                    tpl_i op2b = op2._blk;

                    lst_i blk = ilst_set(mem, op2b->_elms, op2b->_siz);

                    return iblk_set1(LST_I, blk, LST_V, SET_PROP(0, 1, 0));
                }

                if (!op2._blk)
                    return op1;

                tpl_i op2b = op2._blk;

                lst_i blk = ilst_concat(mem, op1._blk, op2b->_elms, op2b->_siz);

                return iblk_set1(LST_I, blk, LST_V, SET_PROP(0, 1, 0));
            }

            bop_i blk = ibop_set(mem, op1, op2, "addLT");

            return iblk_set1(BOP_I, blk, LST_V, SET_PROP(1, 1, 0));
        }

        if (op2._dtyp == DCT_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._prop) && !IS_USEFUL(op2._prop))
            {
                if (!op1._blk)
                {
                    if (!op2._blk)
                        return iblk_set2(LST_I, LST_V, SET_PROP(0, 0, 0));

                    dct_i op2b = op2._blk;

                    lst_i blk = ilst_set(mem, op2b->_vals, op2b->_siz);

                    return iblk_set1(LST_I, blk, LST_V, SET_PROP(0, 1, 0));
                }

                if (!op2._blk)
                    return op1;

                dct_i op2b = op2._blk;

                lst_i blk = ilst_concat(mem, op1._blk, op2b->_vals, op2b->_siz);

                return iblk_set1(LST_I, blk, LST_V, SET_PROP(0, 1, 0));
            }

            bop_i blk = ibop_set(mem, op1, op2, "addLD");

            return iblk_set1(BOP_I, blk, LST_V, SET_PROP(1, 1, 0));
        }

        if (OPT_LVL && !IS_USEFUL(op1._prop) && !IS_USEFUL(op2._prop))
        {
            if (!op1._blk)
            {
                lst_i blk = ilst_set(mem, &op2, 1);

                return iblk_set2(LST_I, LST_V, SET_PROP(0, 1, 0));
            }

            lst_i blk = ilst_append(mem, op1._blk, op2);

            return iblk_set1(LST_I, blk, LST_V, SET_PROP(0, 1, 0));
        }

        bop_i blk;

        switch (op2._dtyp)
        {
        case INT_V:
            blk = ibop_set(mem, op1, op2, "addLI");
        case FLT_V:
            blk = ibop_set(mem, op1, op2, "addLF");
        case BOL_V:
            blk = ibop_set(mem, op1, op2, "addLB");
        case STR_V:
            blk = ibop_set(mem, op1, op2, "addLS");
        }

        return iblk_set1(BOP_I, blk, LST_V, SET_PROP(1, 1, 0));
    }

    if (op1._dtyp == DCT_V)
    {
        if (op2._dtyp == DCT_V)
        {
            if (OPT_LVL && !IS_USEFUL(op1._prop) && !IS_USEFUL(op2._prop))
            {
                if (!op1._blk)
                    return op2;

                if (!op2._blk)
                    return op1;

                dct_i blk = idct_concat(mem, op1._blk, op2._blk);

                return iblk_set1(DCT_I, blk, DCT_V, SET_PROP(0, 1, 0));
            }

            bop_i blk = ibop_set(mem, op1, op2, "addDD");

            return iblk_set1(BOP_I, blk, DCT_V, SET_PROP(1, 1, 0));
        }

        ill_op_error(DCT_V, op2._dtyp, "+");
    }

    ill_op_error(op1._dtyp, op2._dtyp, "+");
}
