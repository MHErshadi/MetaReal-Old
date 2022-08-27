// MetaReal Programming Language version 1.0.0

#include <stdlib.h>
#include <string.h>

#include <mrir/irgen.h>
#include <mrir/ival.h>
#include <mem/blk.h>
#include <debug/crash.h>

mrir_t mrir_init();

idata_t idata_init();

iblk_t visit_nod(igres_tp res, node_t node, mem_t mem, ictx_t ictx);

iblk_t visit_int(node_t node, mem_t mem);
iblk_t visit_flt(node_t node, mem_t mem);
iblk_t visit_bol(node_t node);
iblk_t visit_str(node_t node, mem_t mem);
iblk_t visit_lst(igres_tp res, node_t node, mem_t mem, ictx_t ictx);
iblk_t visit_tpl(igres_tp res, node_t node, mem_t mem, ictx_t ictx);
iblk_t visit_dct(igres_tp res, node_t node, mem_t mem, ictx_t ictx);
iblk_t visit_bop(igres_tp res, node_t node, mem_t mem, ictx_t ictx);

igres_t irgen(node_tp nodes, mem_t mem, ictx_t ictx)
{
    igres_t res;
    res._herr = 0;

    res._ir = mrir_init();
    res._data = idata_init();

    uint64 m_alc = IMAIN_SIZ;

    while (nodes->_typ != EOF_N)
    {
        iblk_t blk = visit_nod(&res, *nodes++, mem, ictx);

        if (res._herr)
        {
            free(nodes);
            mrir_free(res._ir);
            return res;
        }

        if (res._ir._msiz == m_alc)
        {
            res._ir._main = realloc(res._ir._main, (m_alc += IMAIN_SIZ) * sizeof(iblk_t));

            if (!res._ir._main)
                mem_error(m_alc * sizeof(iblk_t));
        }

        res._ir._main[res._ir._msiz++] = blk;
    }

    if (res._ir._msiz != m_alc)
    {
        res._ir._main = realloc(res._ir._main, res._ir._msiz * sizeof(iblk_t));

        if (!res._ir._main)
            mem_error(res._ir._msiz * sizeof(iblk_t));
    }

    free(nodes);
    return res;
}

void igres_fail(igres_tp res, run_tim_t error)
{
    res->_error = error;
    res->_herr = 1;
}

void mrir_print(idata_tp data, mrir_t ir)
{
    fputs("@ Generated by MetaReal version 1.0.0\n\n", STDOUT);

    if (ir._isiz)
    {
        uint64 i;
        for (i = 0; i < ir._isiz; i++)
            fprintf(STDOUT, "#inc <%s>\n", ir._incs[i]);

        fputc('\n', STDOUT);
    }

    if (ir._dsiz)
    {
        uint64 i;
        for (i = 0; i < ir._dsiz; i++)
        {
            iblk_print(data, &ir._defs[i], ";\n");
            fputc('\n', STDOUT);
        }
    }

    fputs("&main($argc |int, $argv |lst) |int\n{\n", STDOUT);

    if (ir._msiz)
    {
        uint64 i;
        for (i = 0; i < ir._msiz; i++)
        {
            if (!OPT_LVL)
            {
                if (IS_USEFUL(ir._main[i]._prop))
                {
                    iblk_print(data, &ir._main[i], ";\n");
                    fputc('\n', STDOUT);
                }

                continue;
            }

            iblk_print(data, &ir._main[i], ";\n");
            fputc('\n', STDOUT);
        }
    }

    fputs("~ret (0);\n}\n", STDOUT);

    if (ir._fsiz)
    {
        fputc('\n', STDOUT);

        uint64 i;
        for (i = 0; i < ir._fsiz; i++)
        {
            iblk_print(data, &ir._funcs[i], ";\n");
            fputc('\n', STDOUT);
        }
    }
}

void mrir_free(mrir_t ir)
{
    if (ir._isiz)
        free(ir._incs);

    if (ir._dsiz)
        free(ir._defs);

    if (ir._msiz)
        free(ir._main);

    if (ir._fsiz)
        free(ir._funcs);
}

mrir_t mrir_init()
{
    mrir_t ir;

    ir._isiz = 0;
    ir._dsiz = 0;
    ir._msiz = 0;
    ir._fsiz = 0;

    ir._main = malloc(IMAIN_SIZ * sizeof(iblk_t));

    if (!ir._main)
        mem_error(IMAIN_SIZ * sizeof(iblk_t));

    return ir;
}

idata_t idata_init()
{
    idata_t data;

    data._int_cnt = 0;
    data._flt_cnt = 0;
    data._bol_cnt = 0;
    data._str_cnt = 0;
    data._lst_cnt = 0;
    data._tpl_cnt = 0;
    data._dct_cnt = 0;

    return data;
}

iblk_t visit_nod(igres_tp res, node_t node, mem_t mem, ictx_t ictx)
{
    switch (node._typ)
    {
    case INT_N:
        return visit_int(node, mem);
    case FLT_N:
        return visit_flt(node, mem);
    case BOL_N:
        return visit_bol(node);
    case STR_N:
        return visit_str(node, mem);
    case LST_N:
        return visit_lst(res, node, mem, ictx);
    case TPL_N:
        return visit_tpl(res, node, mem, ictx);
    case DCT_N:
        return visit_dct(res, node, mem, ictx);
    case BOP_N:
        return visit_bop(res, node, mem, ictx);
    }
}

iblk_t visit_int(node_t node, mem_t mem)
{
    int_n nod = node._nod;

    uint8 flg = 0;
    uint64 val = conv_int(nod->_val, nod->_len - 1, &flg);

    int_i blk = iint_set(mem, val);

    return iblk_set1(INT_I, blk, INT_V, SET_PROP(0, 0, 0));
}

iblk_t visit_flt(node_t node, mem_t mem)
{
    flt_n nod = node._nod;

    uint8 flg = 0;
    dec64 val = conv_dec(nod->_val, nod->_len - 1, &flg);

    flt_i blk = iflt_set(mem, val);

    return iblk_set1(FLT_I, blk, FLT_V, SET_PROP(0, 0, 0));
}

iblk_t visit_bol(node_t node)
{
    bol_n nod = node._nod;

    if (nod->_stat)
        return iblk_set2(BOL_I, BOL_V, SET_PROP(0, 0, 1));
    return iblk_set2(BOL_I, BOL_V, SET_PROP(0, 0, 0));
}

iblk_t visit_str(node_t node, mem_t mem)
{
    if (!node._nod)
        return iblk_set2(STR_I, STR_V, SET_PROP(0, 0, 0));

    str_n nod = node._nod;

    str_i blk = istr_set(mem, nod->_val, nod->_len);

    return iblk_set1(STR_I, blk, STR_V, SET_PROP(0, 0, 0));
}

iblk_t visit_lst(igres_tp res, node_t node, mem_t mem, ictx_t ictx)
{
    if (!node._nod)
        return iblk_set2(LST_I, LST_V, SET_PROP(0, 0, 0));

    lst_n nod = node._nod;

    iblk_tp elms = blk_alloc(mem, nod->_siz * sizeof(iblk_t), MEM_SIZ);

    uint8 is_useful = 0;
    uint64 i;
    for (i = 0; i < nod->_siz; i++)
    {
        iblk_t elem = visit_nod(res, nod->_elms[i], mem, ictx);

        if (res->_herr)
            return elem;

        elms[i] = elem;

        if (IS_USEFUL(elem._prop))
            is_useful = 1;
    }

    lst_i blk = ilst_set(mem, elms, nod->_siz);

    if (is_useful)
        return iblk_set1(LST_I, blk, LST_V, SET_PROP(1, 1, 0));
    return iblk_set1(LST_I, blk, LST_V, SET_PROP(0, 1, 0));
}

iblk_t visit_tpl(igres_tp res, node_t node, mem_t mem, ictx_t ictx)
{
    if (!node._nod)
        return iblk_set2(TPL_I, TPL_V, SET_PROP(0, 0, 0));

    tpl_n nod = node._nod;

    iblk_tp elms = blk_alloc(mem, nod->_siz * sizeof(iblk_t), MEM_SIZ);

    uint8 is_useful = 0;
    uint64 i;
    for (i = 0; i < nod->_siz; i++)
    {
        iblk_t elem = visit_nod(res, nod->_elms[i], mem, ictx);

        if (res->_herr)
            return elem;

        elms[i] = elem;

        if (IS_USEFUL(elem._prop))
            is_useful = 1;
    }

    tpl_i blk = itpl_set(mem, elms, nod->_siz);

    if (is_useful)
        return iblk_set1(TPL_I, blk, TPL_V, SET_PROP(1, 1, 0));
    return iblk_set1(TPL_I, blk, TPL_V, SET_PROP(0, 1, 0));
}

iblk_t visit_dct(igres_tp res, node_t node, mem_t mem, ictx_t ictx)
{
    if (!node._nod)
        return iblk_set2(DCT_I, DCT_V, SET_PROP(0, 0, 0));

    dct_n nod = node._nod;

    iblk_tp keys = blk_alloc(mem, nod->_siz * sizeof(iblk_t), MEM_SIZ);
    iblk_tp vals = blk_alloc(mem, nod->_siz * sizeof(iblk_t), MEM_SIZ);

    uint8 is_useful = 0;
    uint64 i;
    for (i = 0; i < nod->_siz; i++)
    {
        iblk_t key = visit_nod(res, nod->_keys[i], mem, ictx);

        if (res->_herr)
            return key;

        iblk_t val = visit_nod(res, nod->_vals[i], mem, ictx);

        if (res->_herr)
            return val;

        uint64 pos;
        if (!iblkp_contains(keys, i, key, &pos))
        {
            keys[i] = key;
            vals[i] = val;
        }
        else
            vals[pos] = val;

        if (IS_USEFUL(key._prop) || IS_USEFUL(val._prop))
            is_useful = 1;
    }

    dct_i blk = idct_set(mem, keys, vals, nod->_siz);

    if (is_useful)
        return iblk_set1(DCT_I, blk, DCT_V, SET_PROP(1, 1, 0));
    return iblk_set1(DCT_I, blk, DCT_V, SET_PROP(0, 1, 0));
}

iblk_t visit_bop(igres_tp res, node_t node, mem_t mem, ictx_t ictx)
{
    bop_n nod = node._nod;

    iblk_t op1 = visit_nod(res, nod->_op1, mem, ictx);

    if (res->_herr)
        return op1;

    iblk_t op2 = visit_nod(res, nod->_op2, mem, ictx);

    if (res->_herr)
        return op2;

    switch (nod->_opr)
    {
    case ADD_T:
        return ival_add(res, op1, op2, mem, ictx, node._pss, node._pse);
    }
}
