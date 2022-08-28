// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <memory.h>
#include <stdlib.h>

void free_stack(stack_t stack)
{
    free(stack->_data);
    stack->_size = 0;

    stack->_break = NULL;
    stack->_free = 0;
}
