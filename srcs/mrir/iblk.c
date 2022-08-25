// MetaReal Programming Language version 1.0.0

#include <mrir/iblk.h>
#include <mem/blk.h>
#include <def.h>

iblk_t iblk_set1(uint8 typ, ptr blk, str inst, uint64 siz, uint8 prop)
{
    iblk_t iblk;

    iblk._typ = typ;
    iblk._blk = blk;
    iblk._inst = inst;
    iblk._siz = siz;
    iblk._prop = prop;

    return iblk;
}

iblk_t iblk_set2(uint8 typ, str inst, uint64 siz, uint8 prop)
{
    iblk_t iblk;

    iblk._typ = typ;
    iblk._inst = inst;
    iblk._siz = siz;
    iblk._prop = prop;

    return iblk;
}

iblk_t iblk_set3(uint8 typ, uint8 prop)
{
    iblk_t iblk;

    iblk._typ = typ;
    iblk._prop = prop;

    return iblk;
}

void iblk_print(iblk_t blk)
{
    switch (blk._typ)
    {
    case INT_I:
        fprintf(STDOUT, "%llu;\n", blk._siz);
        return;
    }
}
