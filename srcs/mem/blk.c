// MetaReal Programming Language version 1.0.0

#include <stdlib.h>

#include <mem/blk.h>
#include <debug/crash.h>

ptr blk_alloc(mem_t mem, uint64 siz, uint64 alt)
{
    uint64 fre = mem->_siz - ptr_pos(mem->_mem, mem->_brk);
    if (siz > fre)
    {
        mem->_siz += alt;
        fre += alt;

        mem->_mem = realloc(mem->_mem, mem->_siz);

        if (!mem->_mem)
            mem_error(mem->_siz);
    }

    if (fre == siz)
    {
        ptr ptr = mem->_brk;
        mem->_brk = NULL;

        return ptr;
    }

    ptr ptr = mem->_brk;
    mem->_brk += siz;

    return ptr;
}

void blk_realloc(mem_t mem, ptr blk, uint64 siz, uint64 alt)
{
    mem->_brk = blk;

    uint64 fre = mem->_siz - ptr_pos(mem->_mem, mem->_brk);
    if (siz > fre)
    {
        mem->_siz += alt;
        fre += alt;

        mem->_mem = realloc(mem->_mem, mem->_siz);

        if (!mem->_mem)
            mem_error(mem->_siz);
    }

    if (fre == siz)
        mem->_brk = NULL;

    mem->_brk += siz;
}

void blk_add(mem_t mem, uint64 add, uint64 alt)
{
    uint64 fre = mem->_siz - ptr_pos(mem->_mem, mem->_brk);
    if (add > fre)
    {
        mem->_siz += alt;
        fre += alt;

        mem->_mem = realloc(mem->_mem, mem->_siz);

        if (!mem->_mem)
            mem_error(mem->_siz);
    }

    if (fre == add)
        mem->_brk = NULL;

    mem->_brk += add;
}

void blk_sub(mem_t mem, ptr blk, uint64 siz)
{
    mem->_brk = blk;
    mem->_brk += siz;
}

void blk_free(mem_t mem, ptr blk)
{
    mem->_brk = blk;
}

ptrp ptr_alloc(stk_t stk, uint64 siz, uint64 alt)
{
    if (stk->_cnt == stk->_siz)
    {
        stk->_siz += alt;

        stk->_ptrs = realloc(stk->_ptrs, stk->_siz * sizeof(ptr));

        if (!stk->_ptrs)
            mem_error(stk->_siz * sizeof(ptr));
    }

    ptr blk = malloc(siz);

    if (!blk)
        mem_error(siz);

    stk->_ptrs[stk->_cnt] = blk;

    return stk->_ptrs + stk->_cnt++;
}

ptr ptr_realloc(ptrp pos, uint64 siz)
{
    *pos = realloc(*pos, siz);

    if (!*pos)
        mem_error(siz);

    return *pos;
}
