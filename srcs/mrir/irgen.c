// MetaReal Programming Language version 1.0.0

#include <stdlib.h>

#include <mrir/irgen.h>
#include <mrir/ival.h>
#include <debug/crash.h>

mrir_t mrir_init();

void igres_fail(igres_tp res, run_tim_t error);

iblk_t visit_nod(igres_tp res, node_t node, mem_t igmem, ictx_t ictx);

iblk_t visit_int(igres_tp res, node_t node, mem_t igmem, ictx_t ictx);
iblk_t visit_flt(igres_tp res, node_t node, mem_t igmem, ictx_t ictx);
iblk_t visit_bol(igres_tp res, node_t node, mem_t igmem, ictx_t ictx);
iblk_t visit_str(igres_tp res, node_t node, mem_t igmem, ictx_t ictx);
iblk_t visit_lst(igres_tp res, node_t node, mem_t igmem, ictx_t ictx);
iblk_t visit_tpl(igres_tp res, node_t node, mem_t igmem, ictx_t ictx);
iblk_t visit_dct(igres_tp res, node_t node, mem_t igmem, ictx_t ictx);

igres_t irgen(node_tp nodes, mem_t igmem, ictx_t ictx)
{
    igres_t res;
    res._herr = 0;

    res._ir = mrir_init();

    uint64 m_alc = IMAIN_SIZ;

    while (nodes->_typ != EOF_N)
    {
        iblk_t blk = visit_nod(&res, *nodes++, igmem, ictx);

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

void mrir_print(mrir_t ir)
{
    fputs("@ Compiled by MetaReal version 1.0.0\n\n", STDOUT);

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
            iblk_print(ir._defs[i]);
            fputc('\n', STDOUT);
        }
    }

    fputs("&main($argc |int, $argv |lst) |int\n{\n", STDOUT);

    if (ir._msiz)
    {
        uint64 i;
        for (i = 0; i < ir._msiz; i++)
        {
            iblk_print(ir._main[i]);
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
            iblk_print(ir._funcs[i]);
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

void igres_fail(igres_tp res, run_tim_t error)
{
    res->_error = error;
    res->_herr = 1;
}

iblk_t visit_nod(igres_tp res, node_t node, mem_t igmem, ictx_t ictx)
{
    switch (node._typ)
    {
    case INT_N:
        return visit_int(res, node, igmem, ictx);
    }
}

iblk_t visit_int(igres_tp res, node_t node, mem_t igmem, ictx_t ictx)
{
    int_n nod = node._nod;

    uint8 flg = 0;
    uint64 val = conv_int(nod->_val, nod->_len - 1, &flg);

    return iblk_set2(INT_I, NULL, val, SET_PROP(0, 0, 1));
}
