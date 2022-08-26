// MetaReal Programming Language version 1.0.0

#ifndef __IRGEN__
#define __IRGEN__

#include <mrir/iblk.h>
#include <parser/node.h>
#include <debug/errlib.h>

struct __mrir__
{
    uint8p _incs;
    uint64 _isiz;

    iblk_tp _defs;
    uint64 _dsiz;

    iblk_tp _main;
    uint64 _msiz;

    iblk_tp _funcs;
    uint64 _fsiz;
};
typedef struct __mrir__ mrir_t;

struct __igres__
{
    mrir_t _ir;
    idata_t _data;

    uint8 _herr : 1;
    run_tim_t _error;
};
typedef struct __igres__ igres_t;
typedef struct __igres__ *igres_tp;

igres_t irgen(node_tp nodes, mem_t mem, ictx_t ictx);

void igres_fail(igres_tp res, run_tim_t error);

void mrir_print(idata_tp data, mrir_t ir);
void mrir_free(mrir_t ir);

#endif /* __IRGEN__ */
