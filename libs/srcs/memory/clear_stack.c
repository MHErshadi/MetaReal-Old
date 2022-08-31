// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <memory.h>
#include <stdlib.h>
#include <crash.h>

void clear_stack(stack_t stack)
{
    stack->_break = stack->_data;
    stack->_free = stack->_size;
}
