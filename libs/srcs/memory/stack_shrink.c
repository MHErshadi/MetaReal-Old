// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <memory.h>
#include <crash.h>

void stack_shrink(stack_t stack, ptr block, uint64 size)
{
    stack->_break = (uint8p)block + size;
    stack->_free = stack->_size - (stack->_break - stack->_data);
}
