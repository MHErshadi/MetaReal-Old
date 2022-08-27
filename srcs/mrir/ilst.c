// MetaReal Programming Language version 1.0.0

#include <mrir/ilst.h>
#include <mem/blk.h>

lst_i ilst_append(mem_t mem, lst_i lst, iblk_t elm)
{
    uint64 siz = lst->_siz + 1;
    iblk_tp elms = blk_alloc(mem, siz * sizeof(iblk_t), MEM_SIZ);

    uint64 i;
    for (i = 0; i < lst->_siz; i++)
        elms[i] = lst->_elms[i];
    elms[i] = elm;

    return ilst_set(mem, elms, siz);
}

lst_i ilst_concat(mem_t mem, lst_i op1, iblk_tp op2e, uint64 op2s)
{
    uint64 siz = op1->_siz + op2s;
    iblk_tp elms = blk_alloc(mem, siz * sizeof(iblk_t), MEM_SIZ);

    uint64 i, j;
    for (i = 0; i < op1->_siz; i++)
        elms[i] = op1->_elms[i];
    for (j = 0; j < op2s; j++)
        elms[i++] = op2e[j];

    return ilst_set(mem, elms, siz);
}

uint8 ilst_equal(lst_i op1, lst_i op2)
{
    if (op1->_siz != op2->_siz)
        return 0;

    uint64 i;
    for (i = 0; i < op1->_siz; i++)
        if (!iblk_cmp(op1->_elms[i], op2->_elms[i]))
            return 0;
    return 1;
}
