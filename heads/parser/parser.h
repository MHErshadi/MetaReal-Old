// MetaReal Programming Language version 1.0.0

#ifndef __M_PARSER__
#define __M_PARSER__

#include <parser/node.h>
#include <debug/errlib.h>

struct __pres__
{
    node_p _nodes;

    uint8 _has_error : 1;
    invalid_syntax_t _error;
};
typedef struct __pres__ pres_t;
typedef struct __pres__ *pres_p;

pres_t parse(token_p tokens, stack_t stack, heap_t heap);

#endif /* __M_PARSER__ */
