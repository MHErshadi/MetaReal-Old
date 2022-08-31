// MetaReal Programming Language version 1.0.0

#include <ir/context.h>
#include <stddef.h>

context_t context_set1(cstr name, context_p parent, pos_t parent_pos)
{
    context_t context;

    context._name = name;
    context._parent = parent;
    context._parent_pos = parent_pos;

    return context;
}

context_t context_set2(cstr name)
{
    context_t context;

    context._name = name;
    context._parent = NULL;

    return context;
}
