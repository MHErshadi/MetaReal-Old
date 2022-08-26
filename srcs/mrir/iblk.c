// MetaReal Programming Language version 1.0.0

#include <mrir/ival.h>
#include <mem/blk.h>
#include <def.h>

void iblk_label(idata_tp data, iblk_tp blk, cstr end);

uint64 iblk_id(idata_tp data, uint8 dtyp);

iblk_t iblk_set1(uint8 typ, ptr blk, uint8 dtyp, uint8 prop)
{
    iblk_t iblk;

    iblk._typ = typ;
    iblk._blk = blk;
    iblk._dtyp = dtyp;
    iblk._id = 0;
    iblk._prop = prop;

    return iblk;
}

iblk_t iblk_set2(uint8 typ, uint8 dtyp, uint8 prop)
{
    iblk_t iblk;

    iblk._typ = typ;
    iblk._blk = NULL;
    iblk._dtyp = dtyp;
    iblk._id = 0;
    iblk._prop = prop;

    return iblk;
}

void iblk_print(idata_tp data, iblk_tp blk, cstr end)
{
    if (blk->_typ == NLL_I)
        return;

    /* */

    if (blk->_typ == INT_I)
    {
        fprintf(STDOUT, "%llu%s", ((int_i)blk->_blk)->_val, end);
        return;
    }
    if (blk->_typ == FLT_I)
    {
        fprintf(STDOUT, "%lf%s", ((flt_i)blk->_blk)->_val, end);
        return;
    }

    if (blk->_typ == BOL_I)
    {
        fprintf(STDOUT, "%u%s", IS_TRUE(blk->_prop), end);
        return;
    }

    if (blk->_typ == STR_I)
    {
        if (!blk->_blk)
        {
            fprintf(STDOUT, "\"\"%s", end);
            return;
        }

        fprintf(STDOUT, "\"%s\"%s", ((str_i)blk->_blk)->_val, end);
        return;
    }

    if (blk->_typ == LST_I)
    {
        if (!blk->_blk)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        lst_i ilst = blk->_blk;

        uint64 i;
        for (i = 0; i < ilst->_siz; i++)
            if (IS_COMPLEX(ilst->_elms[i]._prop))
                iblk_print(data, &ilst->_elms[i], ";\n");

        if (!blk->_id)
            blk->_id = ++data->_lst_cnt;
        fprintf(STDOUT, "$LST%llu |lst = {", blk->_id);

        for (i = 0; i < ilst->_siz; i++)
            iblk_label(data, &ilst->_elms[i], ", ");

        fprintf(STDOUT, "\b\b}%s", end);
        return;
    }
    if (blk->_typ == TPL_I)
    {
        if (!blk->_blk)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        tpl_i itpl = blk->_blk;

        uint64 i;
        for (i = 0; i < itpl->_siz; i++)
            if (IS_COMPLEX(itpl->_elms[i]._prop))
                iblk_print(data, &itpl->_elms[i], ";\n");

        if (!blk->_id)
            blk->_id = ++data->_tpl_cnt;
        fprintf(STDOUT, "$TPL%llu |tpl = {", blk->_id);

        for (i = 0; i < itpl->_siz; i++)
            iblk_label(data, &itpl->_elms[i], ", ");

        fprintf(STDOUT, "\b\b}%s", end);
        return;
    }
    if (blk->_typ == DCT_I)
    {
        if (!blk->_blk)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        dct_i idct = blk->_blk;

        uint64 i;
        for (i = 0; i < idct->_siz; i++)
            if (IS_COMPLEX(idct->_keys[i]._prop))
                iblk_print(data, &idct->_keys[i], ";\n");

        for (i = 0; i < idct->_siz; i++)
            if (IS_COMPLEX(idct->_vals[i]._prop))
                iblk_print(data, &idct->_vals[i], ";\n");

        fprintf(STDOUT, "$KEYS |lst = {");

        for (i = 0; i < idct->_siz; i++)
            iblk_label(data, &idct->_keys[i], ", ");

        fprintf(STDOUT, "\b\b};\n$VALS |lst = {");

        for (i = 0; i < idct->_siz; i++)
            iblk_label(data, &idct->_vals[i], ", ");

        if (!blk->_id)
            blk->_id = ++data->_dct_cnt;
        fprintf(STDOUT, "\b\b};\n$DCT%llu |dct = {$KEYS, $VALS}%s", blk->_id, end);
        return;
    }

    /* */

    if (blk->_typ == BOP_I)
    {
        bop_i ibop = blk->_blk;

        if (IS_COMPLEX(ibop->_op1._prop))
            iblk_print(data, &ibop->_op1, ";\n");
        if (IS_COMPLEX(ibop->_op2._prop))
            iblk_print(data, &ibop->_op2, ";\n");

        if (!blk->_id)
            blk->_id = iblk_id(data, blk->_dtyp);
        fprintf(STDOUT, "$%s%llu = ~%s ", vals_lbl[blk->_dtyp], blk->_id, ibop->_opr);

        iblk_label(data, &ibop->_op1, ", ");
        iblk_label(data, &ibop->_op2, ";\n");
        return;
    }
}

int_i iint_set(mem_t mem, uint64 val)
{
    int_i blk = blk_alloc(mem, sizeof(struct __iint__), MEM_SIZ);

    blk->_val = val;

    return blk;
}

flt_i iflt_set(mem_t mem, dec64 val)
{
    flt_i blk = blk_alloc(mem, sizeof(struct __iflt__), MEM_SIZ);

    blk->_val = val;

    return blk;
}

str_i istr_set(mem_t mem, str val, uint64 siz)
{
    str_i blk = blk_alloc(mem, sizeof(struct __istr__), MEM_SIZ);

    blk->_val = val;
    blk->_siz = siz;

    return blk;
}

lst_i ilst_set(mem_t mem, iblk_tp elms, uint64 siz)
{
    lst_i blk = blk_alloc(mem, sizeof(struct __ilst__), MEM_SIZ);

    blk->_elms = elms;
    blk->_siz = siz;

    return blk;
}

tpl_i itpl_set(mem_t mem, iblk_tp elms, uint64 siz)
{
    tpl_i blk = blk_alloc(mem, sizeof(struct __itpl__), MEM_SIZ);

    blk->_elms = elms;
    blk->_siz = siz;

    return blk;
}

dct_i idct_set(mem_t mem, iblk_tp keys, iblk_tp vals, uint64 siz)
{
    dct_i blk = blk_alloc(mem, sizeof(struct __idct__), MEM_SIZ);

    blk->_keys = keys;
    blk->_vals = vals;
    blk->_siz = siz;

    return blk;
}

bop_i ibop_set(mem_t mem, iblk_t op1, iblk_t op2, cstr opr)
{
    bop_i blk = blk_alloc(mem, sizeof(struct __ibop__), MEM_SIZ);

    blk->_op1 = op1;
    blk->_op2 = op2;
    blk->_opr = opr;

    return blk;
}

void iblk_label(idata_tp data, iblk_tp blk, cstr end)
{
    if (blk->_typ == NLL_I)
        return;

    /* */

    if (blk->_typ == INT_I)
    {
        fprintf(STDOUT, "%llu%s", ((int_i)blk->_blk)->_val, end);
        return;
    }
    if (blk->_typ == FLT_I)
    {
        fprintf(STDOUT, "%lf%s", ((flt_i)blk->_blk)->_val, end);
        return;
    }

    if (blk->_typ == BOL_I)
    {
        fprintf(STDOUT, "%u%s", IS_TRUE(blk->_prop), end);
        return;
    }

    if (blk->_typ == STR_I)
    {
        if (!blk->_blk)
        {
            fprintf(STDOUT, "\"\"%s", end);
            return;
        }

        fprintf(STDOUT, "\"%s\"%s", ((str_i)blk->_blk)->_val, end);
        return;
    }

    if (blk->_typ == LST_I)
    {
        if (!blk->_blk)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        fprintf(STDOUT, "$LST%llu%s", blk->_id, end);
        return;
    }
    if (blk->_typ == TPL_I)
    {
        if (!blk->_blk)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        fprintf(STDOUT, "$TPL%llu%s", blk->_id, end);
        return;
    }
    if (blk->_typ == DCT_I)
    {
        if (!blk->_blk)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        fprintf(STDOUT, "$DCT%llu%s", blk->_id, end);
        return;
    }

    /* */

    if (blk->_typ == BOP_I)
    {
        fprintf(STDOUT, "$%s%llu%s", vals_lbl[blk->_dtyp], blk->_id, end);
        return;
    }
}

uint64 iblk_id(idata_tp data, uint8 dtyp)
{
    switch (dtyp)
    {
    case INT_V:
        return ++data->_int_cnt;
    case FLT_V:
        return ++data->_flt_cnt;
    case BOL_V:
        return ++data->_bol_cnt;
    case STR_V:
        return ++data->_str_cnt;
    case LST_V:
        return ++data->_lst_cnt;
    case TPL_V:
        return ++data->_tpl_cnt;
    case DCT_V:
        return ++data->_dct_cnt;
    }
}
