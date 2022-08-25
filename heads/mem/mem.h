// MetaReal Programming Language version 1.0.0

#ifndef __MEM__
#define __MEM__

#include <type.h>

#define ptr_pos(s, p) ((p) - (s))

#define mem_cpy(d, s, n)          \
    {                             \
        uint64 i;                 \
        for (i = 0; i < (n); i++) \
            d[i] = s[i];          \
    }

struct __mem__
{
    uint8p _mem;
    uint64 _siz;

    uint8p _brk;
};
typedef struct __mem__ mem_t[1];

struct __stk__
{
    ptrp _ptrs;
    uint64 _siz;
    uint64 _cnt;
};
typedef struct __stk__ stk_t[1];

void mem_alloc(mem_t mem, uint64 siz);
void mem_clear(mem_t mem, uint64 siz);

void stk_alloc(stk_t stk, uint64 alc);
void stk_clear(stk_t stk, uint64 alc);
void stk_free(stk_t stk);

#endif /* __MEM__ */
