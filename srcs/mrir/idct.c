// MetaReal Programming Language version 1.0.0

#include <mrir/idct.h>
#include <mem/blk.h>

dct_i idct_concat(mem_t mem, dct_i op1, dct_i op2)
{
    uint64 siz = op1->_siz + op2->_siz;
    iblk_tp keys = blk_alloc(mem, siz * sizeof(iblk_t), MEM_SIZ);
    iblk_tp vals = blk_alloc(mem, siz * sizeof(iblk_t), MEM_SIZ);

    uint64 i, j;
    for (i = 0; i < op1->_siz; i++)
    {
        keys[i] = op1->_keys[i];
        vals[i] = op1->_vals[i];
    }
    for (j = 0; j < op2->_siz; j++)
    {
        uint64 pos;
        if (!idct_contains(op1, op2->_keys[j], &pos))
        {
            keys[i] = op2->_keys[j];
            vals[i++] = op2->_vals[j];
            continue;
        }

        vals[pos] = op2->_vals[j];
    }

    return idct_set(mem, keys, vals, i);
}

uint8 idct_contains(dct_i dct, iblk_t key, uint64p pos)
{
    uint64 i;
    for (i = 0; i < dct->_siz; i++)
        if (iblk_cmp(dct->_keys[i], key))
        {
            if (pos)
                *pos = i;
            return 1;
        }
    return 0;
}

uint8 idct_equal(dct_i op1, dct_i op2)
{
    if (op1->_siz != op2->_siz)
        return 0;

    uint64 i;
    for (i = 0; i < op1->_siz; i++)
        if (!idct_contains(op2, op1->_keys[i], NULL))
            return 0;
    return 1;
}
