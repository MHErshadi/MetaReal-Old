// MetaReal Programming Language version 1.0.0

#ifndef __M_CONTEXT__
#define __M_CONTEXT__

#include <debug/pos.h>

struct __context__
{
    cstr _name;

    struct __context__ *_parent;
    pos_t _parent_pos;
};
typedef struct __context__ context_t;
typedef struct __context__ *context_p;

context_t context_set1(cstr name, context_p parent, pos_t parent_pos);
context_t context_set2(cstr name);

#endif /* __M_CONTEXT__ */
