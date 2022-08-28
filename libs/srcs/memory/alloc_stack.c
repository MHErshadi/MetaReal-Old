// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <memory.h>
#include <stdlib.h>
#include <crash.h>

void alloc_stack(stack_t stack, uint64 size)
{
    stack->_data = malloc(size);
    if (!stack->_data)
        alloc_error(size);

    stack->_size = size;

    stack->_break = stack->_data;
    stack->_free = size;
}
