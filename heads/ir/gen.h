// MetaReal Programming Language version 1.0.0

#ifndef __M_GEN__
#define __M_GEN__

#include <ir/block.h>
#include <parser/node.h>
#include <debug/errlib.h>

struct __ir__
{
    uint8p _incs;
    uint64 _isize;

    block_p _defs;
    uint64 _dsize;

    block_p _main;
    uint64 _msize;

    block_p _funcs;
    uint64 _fsize;
};
typedef struct __ir__ ir_t;

struct __gres__
{
    ir_t _ir;
    data_t _data;

    uint8 _has_error : 1;
    runtime_t _error;
};
typedef struct __gres__ gres_t;
typedef struct __gres__ *gres_p;

gres_t gen(node_p nodes, stack_t stack, heap_t heap, context_t context);

void gres_fail(gres_p res, runtime_t error);

void ir_print(data_p data, ir_t ir);
void ir_free(ir_t ir);

#endif /* __M_GEN__ */
