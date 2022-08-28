// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <memory.h>
#include <stdlib.h>
#include <crash.h>

void increase_stack(stack_t stack, uint64 add)
{
    stack->_size += add;
    stack->_data = realloc(stack->_data, stack->_size);
    if (!stack->_data)
        alloc_error(stack->_size);

    stack->_free += add;
    stack->_break = stack->_data + (stack->_size - stack->_free);
}
