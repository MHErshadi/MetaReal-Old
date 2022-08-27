// MetaReal Programming Language version 1.0.0

#ifndef __IDCT__
#define __IDCT__

#include <mrir/ival.h>

dct_i idct_concat(mem_t mem, dct_i op1, dct_i op2);

uint8 idct_contains(dct_i dct, iblk_t key, uint64p pos);
uint8 idct_equal(dct_i op1, dct_i op2);

#endif /* __IDCT__ */
