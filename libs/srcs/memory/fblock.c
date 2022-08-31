// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <fblock.h>
#include <stdlib.h>
#include <crash.h>

fblock_p set_fblock(uint8p pos, uint64 size, fblock_p next)
{
    fblock_p block = malloc(sizeof(fblock_t));
    if (!block)
        alloc_error(sizeof(fblock_t));

    block->_pos = pos;
    block->_size = size;
    block->_next = next;

    return block;
}

void free_fblock(fblock_p block)
{
    if (block->_next)
        free_fblock(block->_next);
    free(block);
}
