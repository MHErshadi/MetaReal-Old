// MetaReal Programming Language version 1.0.0

#ifndef __PARSER__
#define __PARSER__

#include <parser/node.h>
#include <debug/errlib.h>

struct __pres__
{
    node_tp _nodes;

    uint8 _herr : 1;
    inv_syn_t _error;
};
typedef struct __pres__ pres_t;
typedef struct __pres__ *pres_tp;

pres_t parse(tok_tp toks, mem_t pmem, stk_t pstk);

#endif /* __PARSER__ */
