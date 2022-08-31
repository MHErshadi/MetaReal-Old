// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <memory.h>
#include <crash.h>
#include <stddef.h>

ptr stack_alloc(stack_t stack, uint64 size)
{
    if (stack->_free < size)
       stack_alloc_error(size); 

    ptr block = stack->_break;

    stack->_break += size;
    stack->_free -= size;

    return block;
}
