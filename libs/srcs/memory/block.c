// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <block.h>
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

ablock_p set_ablock(uint8p pos, uint64 size, ablock_p prev, ablock_p next)
{
    ablock_p block = malloc(sizeof(ablock_t));
    if (!block)
        alloc_error(sizeof(ablock_t));

    block->_pos = pos;
    block->_size = size;
    block->_prev = prev;
    block->_next = next;

    return block;
}

void free_ablock(ablock_p block)
{
    while (block->_prev)
    {
        block = block->_prev;
        free(block->_next);
    }
    free(block);
}
