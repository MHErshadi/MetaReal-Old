// MetaReal Programming Language version 1.0.0

#ifndef __BLK__
#define __BLK__

#include <mem/mem.h>

ptr blk_alloc(mem_t mem, uint64 siz, uint64 alt);

void blk_add(mem_t mem, uint64 add, uint64 alt);
void blk_sub(mem_t mem, ptr blk, uint64 siz);

void blk_free(mem_t mem, ptr blk);

ptrp ptr_alloc(stk_t stk, uint64 siz, uint64 alt);
ptr ptr_realloc(ptrp pos, uint64 siz);

#endif /* __BLK__ */
