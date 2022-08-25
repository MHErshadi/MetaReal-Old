// MetaReal Programming Language version 1.0.0

#include <mrir/iblk.h>
#include <mem/blk.h>
#include <def.h>

void iblk_label(iblk_t blk, cstr end);

iblk_t iblk_set1(uint8 typ, ptr blk, uint8 prop)
{
    iblk_t iblk;

    iblk._typ = typ;
    iblk._blk = blk;
    iblk._prop = prop;

    return iblk;
}

iblk_t iblk_set2(uint8 typ, uint8 prop)
{
    iblk_t iblk;

    iblk._typ = typ;
    iblk._blk = NULL;
    iblk._prop = prop;

    return iblk;
}

void iblk_print(iblk_t blk, cstr end)
{
    if (blk._typ == NLL_I)
        return;

    if (blk._typ == INT_I)
    {
        fprintf(STDOUT, "%llu%s", ((int_i)blk._blk)->_val, end);
        return;
    }
    if (blk._typ == FLT_I)
    {
        fprintf(STDOUT, "%lf%s", ((flt_i)blk._blk)->_val, end);
        return;
    }

    if (blk._typ == BOL_I)
    {
        fprintf(STDOUT, "%u%s", BOOL_STAT(blk._prop), end);
        return;
    }

    if (blk._typ == STR_I)
    {
        fprintf(STDOUT, "\"%s\"%s", ((str_i)blk._blk)->_val, end);
        return;
    }

    if (blk._typ == LST_I)
    {
        if (!blk._blk)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        lst_i ilst = blk._blk;

        uint64 i;
        for (i = 0; i < ilst->_siz; i++)
            if (IS_COMPLEX(ilst->_elms[i]._prop))
                iblk_print(ilst->_elms[i], ";\n");

        fprintf(STDOUT, "$LST%llu |lst = {", ilst->_id);

        for (i = 0; i < ilst->_siz; i++)
            iblk_label(ilst->_elms[i], ", ");

        fprintf(STDOUT, "\b\b}%s", end);
        return;
    }
    if (blk._typ == TPL_I)
    {
        if (!blk._blk)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        tpl_i itpl = blk._blk;

        uint64 i;
        for (i = 0; i < itpl->_siz; i++)
            if (IS_COMPLEX(itpl->_elms[i]._prop))
                iblk_print(itpl->_elms[i], ";\n");

        fprintf(STDOUT, "$TPL%llu |tpl = {", itpl->_id);

        for (i = 0; i < itpl->_siz; i++)
            iblk_label(itpl->_elms[i], ", ");

        fprintf(STDOUT, "\b\b}%s", end);
        return;
    }
    if (blk._typ == DCT_I)
    {
        if (!blk._blk)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        dct_i idct = blk._blk;

        uint64 i;
        for (i = 0; i < idct->_siz; i++)
            if (IS_COMPLEX(idct->_keys[i]._prop))
                iblk_print(idct->_keys[i], ";\n");
        for (i = 0; i < idct->_siz; i++)
            if (IS_COMPLEX(idct->_vals[i]._prop))
                iblk_print(idct->_vals[i], ";\n");

        fprintf(STDOUT, "$KEYS |lst = {");

        for (i = 0; i < idct->_siz; i++)
            iblk_label(idct->_keys[i], ", ");

        fprintf(STDOUT, "\b\b};\n$VALS |lst = {");

        for (i = 0; i < idct->_siz; i++)
            iblk_label(idct->_vals[i], ", ");

        fprintf(STDOUT, "\b\b};\n$DCT%llu |dct = {$KEYS, $VALS}%s", idct->_id, end);
        return;
    }
}

int_i iint_set(mem_t igmem, uint64 val)
{
    int_i blk = blk_alloc(igmem, sizeof(struct __iint__), IGMEM_SIZ);

    blk->_val = val;

    return blk;
}

flt_i iflt_set(mem_t igmem, dec64 val)
{
    flt_i blk = blk_alloc(igmem, sizeof(struct __iflt__), IGMEM_SIZ);

    blk->_val = val;

    return blk;
}

str_i istr_set(mem_t igmem, str val, uint64 siz)
{
    str_i blk = blk_alloc(igmem, sizeof(struct __istr__), IGMEM_SIZ);

    blk->_val = val;
    blk->_siz = siz;

    return blk;
}

lst_i ilst_set(mem_t igmem, iblk_tp elms, uint64 siz, uint64 id)
{
    lst_i blk = blk_alloc(igmem, sizeof(struct __ilst__), IGMEM_SIZ);

    blk->_elms = elms;
    blk->_siz = siz;
    blk->_id = id;

    return blk;
}

tpl_i itpl_set(mem_t igmem, iblk_tp elms, uint64 siz, uint64 id)
{
    tpl_i blk = blk_alloc(igmem, sizeof(struct __itpl__), IGMEM_SIZ);

    blk->_elms = elms;
    blk->_siz = siz;
    blk->_id = id;

    return blk;
}

dct_i idct_set(mem_t igmem, iblk_tp keys, iblk_tp vals, uint64 siz, uint64 id)
{
    dct_i blk = blk_alloc(igmem, sizeof(struct __idct__), IGMEM_SIZ);

    blk->_keys = keys;
    blk->_vals = vals;
    blk->_siz = siz;
    blk->_id = id;

    return blk;
}

void iblk_label(iblk_t blk, cstr end)
{
    if (blk._typ == NLL_I)
        return;

    if (blk._typ == INT_I)
    {
        fprintf(STDOUT, "%llu%s", ((int_i)blk._blk)->_val, end);
        return;
    }
    if (blk._typ == FLT_I)
    {
        fprintf(STDOUT, "%lf%s", ((flt_i)blk._blk)->_val, end);
        return;
    }

    if (blk._typ == BOL_I)
    {
        fprintf(STDOUT, "%u%s", BOOL_STAT(blk._prop), end);
        return;
    }

    if (blk._typ == STR_I)
    {
        fprintf(STDOUT, "\"%s\"%s", ((str_i)blk._blk)->_val, end);
        return;
    }

    if (blk._typ == LST_I)
    {
        if (!blk._blk)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        fprintf(STDOUT, "$LST%llu%s", ((lst_i)blk._blk)->_id, end);
        return;
    }
    if (blk._typ == TPL_I)
    {
        if (!blk._blk)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        fprintf(STDOUT, "$TPL%llu%s", ((tpl_i)blk._blk)->_id, end);
        return;
    }
    if (blk._typ == DCT_I)
    {
        if (!blk._blk)
        {
            fprintf(STDOUT, "{}%s", end);
            return;
        }

        fprintf(STDOUT, "$DCT%llu%s", ((dct_i)blk._blk)->_id, end);
        return;
    }
}
