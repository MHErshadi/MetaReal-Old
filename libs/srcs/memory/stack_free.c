// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <memory.h>

void stack_free(stack_t stack, ptr block)
{
    stack->_free += stack->_break - (uint8p)block;
    stack->_break = block;
}
