// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#include <memory.h>
#include <crash.h>

void stack_increase(stack_t stack, uint64 add)
{
    if (stack->_free < add)
        stack_alloc_error(add);

    stack->_break += add;
    stack->_free -= add;
}
