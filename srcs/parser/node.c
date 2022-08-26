// MetaReal Programming Language version 1.0.0

#include <parser/node.h>
#include <mem/blk.h>

node_t node_set1(uint8 typ, ptr nod, pos_t pss, pos_t pse)
{
    node_t node;

    node._typ = typ;
    node._nod = nod;
    node._pss = pss;
    node._pse = pse;

    return node;
}

node_t node_set2(uint8 typ, pos_t pss, pos_t pse)
{
    node_t node;

    node._typ = typ;
    node._nod = NULL;
    node._pss = pss;
    node._pse = pse;

    return node;
}

body_t body_set(mem_t mem, node_t nodes)
{
    body_t body;

    body._nodes = blk_alloc(mem, sizeof(node_t), MEM_SIZ);

    *body._nodes = nodes;
    body._siz = 1;

    return body;
}

int_n int_set(mem_t mem, str val, uint64 len)
{
    int_n nod = blk_alloc(mem, sizeof(struct __int__), MEM_SIZ);

    nod->_val = val;
    nod->_len = len;

    return nod;
}

flt_n flt_set(mem_t mem, str val, uint64 len)
{
    flt_n nod = blk_alloc(mem, sizeof(struct __flt__), MEM_SIZ);

    nod->_val = val;
    nod->_len = len;

    return nod;
}

bol_n bol_set(mem_t mem, uint8 stat)
{
    bol_n nod = blk_alloc(mem, sizeof(struct __bol__), MEM_SIZ);

    nod->_stat = stat;

    return nod;
}

str_n str_set(mem_t mem, str val, uint64 len)
{
    str_n nod = blk_alloc(mem, sizeof(struct __str__), MEM_SIZ);

    nod->_val = val;
    nod->_len = len;

    return nod;
}

lst_n lst_set(mem_t mem, node_tp elms, uint64 siz)
{
    lst_n nod = blk_alloc(mem, sizeof(struct __lst__), MEM_SIZ);

    nod->_elms = elms;
    nod->_siz = siz;

    return nod;
}

tpl_n tpl_set(mem_t mem, node_tp elms, uint64 siz)
{
    tpl_n nod = blk_alloc(mem, sizeof(struct __tpl__), MEM_SIZ);

    nod->_elms = elms;
    nod->_siz = siz;

    return nod;
}

dct_n dct_set(mem_t mem, node_tp keys, node_tp vals, uint64 siz)
{
    dct_n nod = blk_alloc(mem, sizeof(struct __dct__), MEM_SIZ);

    nod->_keys = keys;
    nod->_vals = vals;
    nod->_siz = siz;

    return nod;
}

bop_n bop_set(mem_t mem, uint8 opr, node_t op1, node_t op2)
{
    bop_n nod = blk_alloc(mem, sizeof(struct __bop__), MEM_SIZ);

    nod->_opr = opr;
    nod->_op1 = op1;
    nod->_op2 = op2;

    return nod;
}

uop_n uop_set(mem_t mem, uint8 opr, node_t op)
{
    uop_n nod = blk_alloc(mem, sizeof(struct __uop__), MEM_SIZ);

    nod->_opr = opr;
    nod->_op = op;

    return nod;
}

tco_n tco_set(mem_t mem, node_t cond, node_t op1, node_t op2)
{
    tco_n nod = blk_alloc(mem, sizeof(struct __tco__), MEM_SIZ);

    nod->_cond = cond;
    nod->_op1 = op1;
    nod->_op2 = op2;

    return nod;
}

idx_n idx_set(mem_t mem, node_t arr, node_t idx)
{
    idx_n nod = blk_alloc(mem, sizeof(struct __idx__), MEM_SIZ);

    nod->_arr = arr;
    nod->_idx = idx;

    return nod;
}

vas_n vas_set(mem_t mem, str name, node_t val, uint8 prop)
{
    vas_n nod = blk_alloc(mem, sizeof(struct __vas__), MEM_SIZ);

    nod->_name = name;
    nod->_val = val;
    nod->_prop = prop;

    return nod;
}

vra_n vra_set(mem_t mem, uint8 opr, node_t var, node_t val)
{
    vra_n nod = blk_alloc(mem, sizeof(struct __vra__), MEM_SIZ);

    nod->_opr = opr;
    nod->_var = var;
    nod->_val = val;

    return nod;
}

vfa_n vfa_set(mem_t mem, uint8 opr, node_t var)
{
    vfa_n nod = blk_alloc(mem, sizeof(struct __vfa__), MEM_SIZ);

    nod->_opr = opr;
    nod->_var = var;

    return nod;
}

vac_n vac_set(mem_t mem, str name)
{
    vac_n nod = blk_alloc(mem, sizeof(struct __vac__), MEM_SIZ);

    nod->_name = name;

    return nod;
}

fdf_n fdf_set(mem_t mem, str name, arg_tp args, uint64 siz, body_t body, uint8 prop)
{
    fdf_n nod = blk_alloc(mem, sizeof(struct __fdf__), MEM_SIZ);

    nod->_name = name;
    nod->_args = args;
    nod->_siz = siz;
    nod->_body = body;
    nod->_prop = prop;

    return nod;
}

fcl_n fcl_set(mem_t mem, node_t func, arg_tp args, uint64 siz)
{
    fcl_n nod = blk_alloc(mem, sizeof(struct __fcl__), MEM_SIZ);

    nod->_func = func;
    nod->_args = args;
    nod->_siz = siz;

    return nod;
}

cdf_n cdf_set(mem_t mem, str name, body_t body, uint8 prop)
{
    cdf_n nod = blk_alloc(mem, sizeof(struct __cdf__), MEM_SIZ);

    nod->_name = name;
    nod->_body = body;
    nod->_prop = prop;

    return nod;
}

sdf_n sdf_set(mem_t mem, str name, body_t body, uint8 prop)
{
    sdf_n nod = blk_alloc(mem, sizeof(struct __sdf__), MEM_SIZ);

    nod->_name = name;
    nod->_body = body;
    nod->_prop = prop;

    return nod;
}

dfc_n dfc_set(mem_t mem, str name, node_tp args, uint64 siz)
{
    dfc_n nod = blk_alloc(mem, sizeof(struct __dfc__), MEM_SIZ);

    nod->_name = name;
    nod->_args = args;
    nod->_siz = siz;

    return nod;
}

iff_n iff_set(mem_t mem, stat_tp stats, uint64 siz, body_t ebody)
{
    iff_n nod = blk_alloc(mem, sizeof(struct __iff__), MEM_SIZ);

    nod->_stats = stats;
    nod->_siz = siz;
    nod->_ebody = ebody;

    return nod;
}

swh_n swh_set(mem_t mem, node_t val, stat_tp stats, uint64 siz, body_t dbody)
{
    swh_n nod = blk_alloc(mem, sizeof(struct __swh__), MEM_SIZ);

    nod->_val = val;
    nod->_stats = stats;
    nod->_siz = siz;
    nod->_dbody = dbody;

    return nod;
}

for_n for_set(mem_t mem, str var, node_t start, node_t end, node_t step, body_t body)
{
    for_n nod = blk_alloc(mem, sizeof(struct __for__), MEM_SIZ);

    nod->_var = var;
    nod->_start = start;
    nod->_end = end;
    nod->_step = step;
    nod->_body = body;

    return nod;
}

fre_n fre_set(mem_t mem, str var, node_t iter, body_t body)
{
    fre_n nod = blk_alloc(mem, sizeof(struct __fre__), MEM_SIZ);

    nod->_var = var;
    nod->_iter = iter;
    nod->_body = body;

    return nod;
}

whl_n whl_set(mem_t mem, node_t cond, body_t body)
{
    whl_n nod = blk_alloc(mem, sizeof(struct __whl__), MEM_SIZ);

    nod->_cond = cond;
    nod->_body = body;

    return nod;
}

dow_n dow_set(mem_t mem, body_t body, node_t cond)
{
    dow_n nod = blk_alloc(mem, sizeof(struct __dow__), MEM_SIZ);

    nod->_body = body;
    nod->_cond = cond;

    return nod;
}

lop_n lop_set(mem_t mem, node_t init, node_t cond, node_t step, body_t body)
{
    lop_n nod = blk_alloc(mem, sizeof(struct __lop__), MEM_SIZ);

    nod->_init = init;
    nod->_cond = cond;
    nod->_step = step;
    nod->_body = body;

    return nod;
}

try_n try_set(mem_t mem, body_t error, stat_tp stats, uint64 siz, body_t fbody)
{
    try_n nod = blk_alloc(mem, sizeof(struct __try__), MEM_SIZ);

    nod->_error = error;
    nod->_stats = stats;
    nod->_siz = siz;
    nod->_fbody = fbody;

    return nod;
}

ret_n ret_set(mem_t mem, node_t val)
{
    ret_n nod = blk_alloc(mem, sizeof(struct __ret__), MEM_SIZ);

    nod->_val = val;

    return nod;
}

imp_n imp_set(mem_t mem, str lib)
{
    imp_n nod = blk_alloc(mem, sizeof(struct __imp__), MEM_SIZ);

    nod->_lib = lib;

    return nod;
}
