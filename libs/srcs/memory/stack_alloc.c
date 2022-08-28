// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <memory.h>
#include <stddef.h>

ptr stack_alloc(stack_t stack, uint64 size, uint64 alt_size)
{
    while (size > stack->_free)
        increase_stack(stack, alt_size);

    ptr block = stack->_break;

    stack->_break += size;
    stack->_free -= size;

    return block;
}
