// MetaReal Programming Language version 1.0.0
// MetaReal String Library version 1.0.0

#include <stddef.h>

#include <mstr.h>
#include <mem/blk.h>

#include <stdio.h>

uint64 mstr_concat(mem_t mem, uint64 alt, strp res, str op1, uint64 siz1, str op2, uint64 siz2)
{
    siz1--;
    siz2--;

    uint64 siz = siz1 + siz2 + 1;
    *res = blk_alloc(mem, siz, alt);

    uint64 i, j;
    for (i = 0; i < siz1; i++)
        (*res)[i] = op1[i];
    for (j = 0; j <= siz2; j++)
        (*res)[i++] = op2[j];

    return siz;
}
