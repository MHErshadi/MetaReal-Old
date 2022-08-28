// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#ifndef __M_BLOCK__
#define __M_BLOCK__

#include <memory.h>

fblock_p set_fblock(uint8p pos, uint64 size, fblock_p next);
void free_fblock(fblock_p block);

ablock_p set_ablock(uint8p pos, uint64 size, ablock_p prev, ablock_p next);
void free_ablock(ablock_p block);

#endif /* __M_BLOCK__ */
