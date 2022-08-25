// MetaReal Programming Language version 1.0.0

#include <stdlib.h>

#include <mem/mem.h>
#include <debug/crash.h>

void mem_alloc(mem_t mem, uint64 siz)
{
    mem->_mem = malloc(siz);

    if (!mem->_mem)
        mem_error(siz);

    mem->_siz = siz;
    mem->_brk = mem->_mem;
}

void mem_clear(mem_t mem, uint64 siz)
{
    if (mem->_siz > siz)
    {
        mem->_mem = realloc(mem->_mem, siz);

        if (!mem->_mem)
            mem_error(siz);

        mem->_siz = siz;
    }

    mem->_brk = mem->_mem;
}

void stk_alloc(stk_t stk, uint64 siz)
{
    stk->_ptrs = malloc(siz * sizeof(ptr));

    if (!stk->_ptrs)
        mem_error(siz * sizeof(ptr));

    stk->_siz = siz;
    stk->_cnt = 0;
}

void stk_clear(stk_t stk, uint64 siz)
{
    uint64 i;
    for (i = 0; i < stk->_cnt; i++)
        free(stk->_ptrs[i]);

    if (stk->_siz > siz)
    {
        stk->_ptrs = realloc(stk->_ptrs, siz * sizeof(ptr));

        if (!stk->_ptrs)
            mem_error(siz * sizeof(ptr));

        stk->_siz = siz;
    }

    stk->_cnt = 0;
}

void stk_free(stk_t stk)
{
    uint64 i;
    for (i = 0; i < stk->_cnt; i++)
        free(stk->_ptrs[i]);
    free(stk->_ptrs);
}
