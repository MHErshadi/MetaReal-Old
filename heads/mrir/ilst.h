// MetaReal Programming Language version 1.0.0

#ifndef __ILST__
#define __ILST__

#include <mrir/ival.h>

lst_i ilst_append(mem_t mem, lst_i lst, iblk_t elm);
lst_i ilst_concat(mem_t mem, lst_i op1, iblk_tp op2e, uint64 op2s);

uint8 ilst_equal(lst_i op1, lst_i op2);

#endif /* __ILST__ */
