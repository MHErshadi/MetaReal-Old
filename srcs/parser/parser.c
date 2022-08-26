// MetaReal Programming Language version 1.0.0

#include <stdlib.h>

#include <parser/parser.h>
#include <lexer/lexer.h>
#include <mem/blk.h>
#include <debug/crash.h>

#define adv_nln1(t) if ((t)->_typ == NLN_T) (t)++
#define adv_nln2(t) for (; (t)->_typ == NLN_T || (t)->_typ == SEM_T; (t)++)

void pres_fail(pres_tp res, inv_syn_t error);

tok_tp mull(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // multi-line-support
tok_tp dlfc(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // dollar-function-support
tok_tp stmt(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // return continue break pass
tok_tp tupl(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // inline-list-support
tok_tp asgn(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // = += -= *= /= %= //= **= &= |= ^= <<= >>=
tok_tp tern(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // ternary-conditional-statement
tok_tp type(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // is are
tok_tp cont(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // in
tok_tp lior(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // || or
tok_tp lxor(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // ^^ xor
tok_tp land(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // && and
tok_tp bior(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // |
tok_tp bxor(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // ^
tok_tp band(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // &
tok_tp cmp1(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // == !=
tok_tp cmp2(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // < > <= >=
tok_tp shft(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // << >>
tok_tp expr(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // + -
tok_tp term(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // * / % //
tok_tp fact(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // +(un) -(un) ~ ! not ++ --
tok_tp expo(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // **
tok_tp post(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // array-subscripting ++(post) --(post) function-call access
tok_tp core(pres_tp res, tok_tp toks, mem_t mem, stk_t stk); // statements types ...

tok_tp hand_var(pres_tp res, tok_tp toks, mem_t mem, stk_t stk);
tok_tp hand_lst(pres_tp res, tok_tp toks, mem_t mem, stk_t stk);
tok_tp hand_dct(pres_tp res, tok_tp toks, mem_t mem, stk_t stk);
tok_tp hand_fdf(pres_tp res, tok_tp toks, mem_t mem, stk_t stk);
tok_tp hand_fcl(node_t func, pres_tp res, tok_tp toks, mem_t mem, stk_t stk);
tok_tp hand_cdf(pres_tp res, tok_tp toks, mem_t mem, stk_t stk);
tok_tp hand_sdf(pres_tp res, tok_tp toks, mem_t mem, stk_t stk);
tok_tp hand_iff(pres_tp res, tok_tp toks, mem_t mem, stk_t stk);
tok_tp hand_swh(pres_tp res, tok_tp toks, mem_t mem, stk_t stk);
tok_tp hand_for(pres_tp res, tok_tp toks, mem_t mem, stk_t stk);
tok_tp hand_fre(str var, pos_t pss, pres_tp res, tok_tp toks, mem_t mem, stk_t stk);
tok_tp hand_whl(pres_tp res, tok_tp toks, mem_t mem, stk_t stk);
tok_tp hand_dow(pres_tp res, tok_tp toks, mem_t mem, stk_t stk);
tok_tp hand_lop(pres_tp res, tok_tp toks, mem_t mem, stk_t stk);
tok_tp hand_try(pres_tp res, tok_tp toks, mem_t mem, stk_t stk);

tok_tp gen_prop(uint8p props, tok_tp toks);
tok_tp gen_body(body_tp body, uint64 alt, pres_tp res, tok_tp toks, mem_t mem, stk_t stk);
tok_tp gen_case(body_tp body, uint64 alt, pres_tp res, tok_tp toks, mem_t mem, stk_t stk);

pres_t parse(tok_tp toks, mem_t mem, stk_t stk)
{
    pres_t res;
    res._herr = 0;
    res._nodes = malloc(NODS_SIZ * sizeof(node_t));

    if (!res._nodes)
        mem_error(NODS_SIZ * sizeof(node_t));

    toks = mull(&res, toks, mem, stk);

    if (!res._herr && toks->_typ != EOF_T)
    {
        pres_fail(&res, inv_syn_set("Expected EOF", toks->_pss, toks->_pse));

        free(toks);
        return res;
    }

    free(toks);
    return res;
}

void pres_fail(pres_tp res, inv_syn_t error)
{
    res->_error = error;
    res->_herr = 1;
}

tok_tp mull(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    uint64 alc = NODS_SIZ;
    uint64 siz = 0;

    do
    {
        adv_nln2(toks);

        if (toks->_typ == EOF_T)
            break;

        if (siz == alc)
        {
            res->_nodes = realloc(res->_nodes, (alc += NODS_SIZ) * sizeof(node_t));

            if (!res->_nodes)
                mem_error(alc * sizeof(node_t));
        }

        toks = dlfc(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        siz++;
        res->_nodes++;
    } while ((toks - 1)->_typ == NLN_T || toks->_typ == SEM_T);

    res->_nodes -= siz;

    res->_nodes = realloc(res->_nodes, (siz + 1) * sizeof(node_t));

    if (!res->_nodes)
        mem_error((siz + 1) * sizeof(node_t));

    res->_nodes[siz] = node_set2(EOF_N, toks->_pss, toks->_pse);

    return toks;
}

tok_tp dlfc(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    if (toks->_typ == DLR_T)
    {
        pos_t pss = toks++->_pss;

        adv_nln1(toks);

        if (toks->_typ != IDN_T)
        {
            pres_fail(res, inv_syn_set("Expected identifier for dollar function", toks->_pss, toks->_pse));
            return toks;
        }

        str name = toks->_val;

        pos_t pse = toks++->_pse;

        adv_nln1(toks);

        toks = asgn(res, toks, mem, stk);
        if (res->_herr)
        {
            res->_herr = 0;

            dfc_n nod = dfc_set(mem, name, NULL, 0);

            *res->_nodes = node_set1(DFC_N, nod, pss, pse);
            return toks;
        }

        ptrp b_args = ptr_alloc(stk, DFC_ARG_SIZ * sizeof(node_t), STK_SIZ);
        node_tp args = *b_args;

        uint64 alc = DFC_ARG_SIZ;
        uint64 siz = 1;

        *args = *res->_nodes;

        while (toks->_typ == CMA_T)
        {
            toks++;
            adv_nln1(toks);

            toks = asgn(res, toks, mem, stk);
            if (res->_herr)
                return toks;

            if (siz == alc)
                args = ptr_realloc(b_args, (alc += DFC_ARG_SIZ) * sizeof(node_t));

            args[siz++] = *res->_nodes;
        }

        if (siz != alc)
            args = ptr_realloc(b_args, siz * sizeof(node_t));

        dfc_n nod = dfc_set(mem, name, args, siz);

        *res->_nodes = node_set1(DFC_N, nod, pss, res->_nodes->_pse);
        return toks;
    }

    return stmt(res, toks, mem, stk);
}

tok_tp stmt(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    if (toks->_typ == RET_K)
    {
        pos_t pss = toks->_pss;
        pos_t pse = toks->_pse;

        toks++;
        adv_nln1(toks);

        tok_tp tmp = tupl(res, toks, mem, stk);
        if (res->_herr)
        {
            res->_herr = 0;

            *res->_nodes = node_set2(RET_N, pss, pse);
            return toks;
        }
        toks = tmp;

        ret_n nod = ret_set(mem, *res->_nodes);

        *res->_nodes = node_set1(RET_N, nod, pss, res->_nodes->_pse);
        return toks;
    }

    if (toks->_typ == CNT_K)
    {
        *res->_nodes = node_set2(CNT_N, toks->_pss, toks->_pse);

        toks++;
        adv_nln1(toks);

        return toks;
    }
    if (toks->_typ == BRK_K)
    {
        *res->_nodes = node_set2(BRK_N, toks->_pss, toks->_pse);

        toks++;
        adv_nln1(toks);

        return toks;
    }

    if (toks->_typ == PSS_K)
    {
        *res->_nodes = node_set2(PSS_N, toks->_pss, toks->_pse);

        toks++;
        adv_nln1(toks);

        return toks;
    }

    return tupl(res, toks, mem, stk);
}

tok_tp tupl(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = asgn(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    if (toks->_typ == CMA_T)
    {
        ptrp b_elms = ptr_alloc(stk, TUPL_SIZ * sizeof(node_t), STK_SIZ);
        node_tp elms = *b_elms;

        uint64 alc = TUPL_SIZ;
        uint64 siz = 1;

        *elms = *res->_nodes;
        do
        {
            toks++;
            adv_nln1(toks);

            toks = asgn(res, toks, mem, stk);
            if (res->_herr)
            {
                res->_herr = 0;

                if (siz != alc)
                    elms = ptr_realloc(b_elms, siz * sizeof(node_t));

                tpl_n nod = tpl_set(mem, elms, siz);

                *res->_nodes = node_set1(TPL_N, nod, elms->_pss, res->_nodes->_pse);
                return toks;
            }

            if (siz == alc)
                elms = ptr_realloc(b_elms, (alc += TUPL_SIZ) * sizeof(node_t));

            elms[siz++] = *res->_nodes;
        } while (toks->_typ == CMA_T);

        if (siz != alc)
            elms = ptr_realloc(b_elms, siz * sizeof(node_t));

        tpl_n nod = tpl_set(mem, elms, siz);

        *res->_nodes = node_set1(TPL_N, nod, elms->_pss, res->_nodes->_pse);
        return toks;
    }

    return toks;
}

tok_tp asgn(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    if (toks->_typ == VAR_K)
        return hand_var(res, toks, mem, stk);

    toks = tern(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    if (toks->_typ == ASN_T || (toks->_typ >= ADE_T && toks->_typ <= RSE_T))
    {
        uint8 opr = toks++->_typ;

        node_t var = *res->_nodes;

        adv_nln1(toks);

        toks = tupl(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        vra_n nod = vra_set(mem, opr, var, *res->_nodes);

        *res->_nodes = node_set1(VRA_N, nod, var._pss, res->_nodes->_pse);
        return toks;
    }

    return toks;
}

tok_tp tern(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = type(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    if (toks->_typ == QUE_T)
    {
        node_t cond = *res->_nodes;

        toks++;
        adv_nln1(toks);

        toks = asgn(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        node_t op1 = *res->_nodes;

        if (toks->_typ != COL_T)
        {
            pres_fail(res, inv_syn_set("Expected ':' for ternary conditional statement", toks->_pss, toks->_pse));
            return toks;
        }

        toks++;
        adv_nln1(toks);

        toks = asgn(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        tco_n nod = tco_set(mem, cond, op1, *res->_nodes);

        *res->_nodes = node_set1(TCO_N, nod, cond._pss, res->_nodes->_pse);
        return toks;
    }

    return toks;
}

tok_tp type(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = cont(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t op1 = *res->_nodes;

    while (toks->_typ == ISS_K || toks->_typ == ARE_K)
    {
        uint8 op = toks++->_typ;

        adv_nln1(toks);

        toks = cont(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        bop_n nod = bop_set(mem, op, op1, *res->_nodes);
        op1 = node_set1(BOP_N, nod, op1._pss, res->_nodes->_pse);
    }

    *res->_nodes = op1;
    return toks;
}

tok_tp cont(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = lior(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t op1 = *res->_nodes;

    while (toks->_typ == INN_K)
    {
        toks++;
        adv_nln1(toks);

        toks = lior(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        bop_n nod = bop_set(mem, INN_K, op1, *res->_nodes);
        op1 = node_set1(BOP_N, nod, op1._pss, res->_nodes->_pse);
    }

    *res->_nodes = op1;
    return toks;
}

tok_tp lior(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = lxor(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t op1 = *res->_nodes;

    while (toks->_typ == IOR_T || toks->_typ == IOR_K)
    {
        toks++;
        adv_nln1(toks);

        toks = lxor(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        bop_n nod = bop_set(mem, IOR_T, op1, *res->_nodes);
        op1 = node_set1(BOP_N, nod, op1._pss, res->_nodes->_pse);
    }

    *res->_nodes = op1;
    return toks;
}

tok_tp lxor(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = land(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t op1 = *res->_nodes;

    while (toks->_typ == XOR_T || toks->_typ == XOR_K)
    {
        toks++;
        adv_nln1(toks);

        toks = land(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        bop_n nod = bop_set(mem, XOR_T, op1, *res->_nodes);
        op1 = node_set1(BOP_N, nod, op1._pss, res->_nodes->_pse);
    }

    *res->_nodes = op1;
    return toks;
}

tok_tp land(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = bior(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t op1 = *res->_nodes;

    while (toks->_typ == AND_T || toks->_typ == AND_K)
    {
        toks++;
        adv_nln1(toks);

        toks = bior(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        bop_n nod = bop_set(mem, AND_T, op1, *res->_nodes);
        op1 = node_set1(BOP_N, nod, op1._pss, res->_nodes->_pse);
    }

    *res->_nodes = op1;
    return toks;
}

tok_tp bior(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = bxor(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t op1 = *res->_nodes;

    while (toks->_typ == BIO_T)
    {
        toks++;
        adv_nln1(toks);

        toks = bxor(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        bop_n nod = bop_set(mem, BIO_T, op1, *res->_nodes);
        op1 = node_set1(BOP_N, nod, op1._pss, res->_nodes->_pse);
    }

    *res->_nodes = op1;
    return toks;
}

tok_tp bxor(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = band(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t op1 = *res->_nodes;

    while (toks->_typ == BXO_T)
    {
        toks++;
        adv_nln1(toks);

        toks = band(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        bop_n nod = bop_set(mem, BXO_T, op1, *res->_nodes);
        op1 = node_set1(BOP_N, nod, op1._pss, res->_nodes->_pse);
    }

    *res->_nodes = op1;
    return toks;
}

tok_tp band(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = cmp1(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t op1 = *res->_nodes;

    while (toks->_typ == BAN_T)
    {
        toks++;
        adv_nln1(toks);

        toks = cmp1(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        bop_n nod = bop_set(mem, BAN_T, op1, *res->_nodes);
        op1 = node_set1(BOP_N, nod, op1._pss, res->_nodes->_pse);
    }

    *res->_nodes = op1;
    return toks;
}

tok_tp cmp1(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = cmp2(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t op1 = *res->_nodes;

    while (toks->_typ == EQU_T || toks->_typ == NEQ_T)
    {
        uint8 op = toks++->_typ;

        adv_nln1(toks);

        toks = cmp2(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        bop_n nod = bop_set(mem, op, op1, *res->_nodes);
        op1 = node_set1(BOP_N, nod, op1._pss, res->_nodes->_pse);
    }

    *res->_nodes = op1;
    return toks;
}

tok_tp cmp2(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = shft(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t op1 = *res->_nodes;

    while (toks->_typ >= LST_T && toks->_typ <= GTE_T)
    {
        uint8 op = toks++->_typ;

        adv_nln1(toks);

        toks = shft(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        bop_n nod = bop_set(mem, op, op1, *res->_nodes);
        op1 = node_set1(BOP_N, nod, op1._pss, res->_nodes->_pse);
    }

    *res->_nodes = op1;
    return toks;
}

tok_tp shft(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = expr(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t op1 = *res->_nodes;

    while (toks->_typ == LSH_T || toks->_typ == RSH_T)
    {
        uint8 op = toks++->_typ;

        adv_nln1(toks);

        toks = expr(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        bop_n nod = bop_set(mem, op, op1, *res->_nodes);
        op1 = node_set1(BOP_N, nod, op1._pss, res->_nodes->_pse);
    }

    *res->_nodes = op1;
    return toks;
}

tok_tp expr(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = term(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t op1 = *res->_nodes;

    while (toks->_typ == ADD_T || toks->_typ == SUB_T)
    {
        uint8 op = toks++->_typ;

        adv_nln1(toks);

        toks = term(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        bop_n nod = bop_set(mem, op, op1, *res->_nodes);
        op1 = node_set1(BOP_N, nod, op1._pss, res->_nodes->_pse);
    }

    *res->_nodes = op1;
    return toks;
}

tok_tp term(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = fact(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t op1 = *res->_nodes;

    while (toks->_typ >= MUL_T && toks->_typ <= QOT_T)
    {
        uint8 op = toks++->_typ;

        adv_nln1(toks);

        toks = fact(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        bop_n nod = bop_set(mem, op, op1, *res->_nodes);
        op1 = node_set1(BOP_N, nod, op1._pss, res->_nodes->_pse);
    }

    *res->_nodes = op1;
    return toks;
}

tok_tp fact(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    if (toks->_typ == ADD_T || toks->_typ == SUB_T ||
        toks->_typ == BNT_T ||
        toks->_typ == NOT_T || toks->_typ == NOT_K)
    {
        pos_t pss = toks->_pss;
        uint8 opr = toks++->_typ;

        adv_nln1(toks);

        toks = fact(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        uop_n nod = uop_set(mem, opr, *res->_nodes);

        *res->_nodes = node_set1(UOP_N, nod, pss, res->_nodes->_pse);
        return toks;
    }

    if (toks->_typ == INC_T || toks->_typ == DEC_T)
    {
        pos_t pss = toks->_pss;
        uint8 opr = toks++->_typ == DEC_T ? 3 : 0;

        adv_nln1(toks);

        toks = fact(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        vfa_n nod = vfa_set(mem, opr, *res->_nodes);

        *res->_nodes = node_set1(VFA_N, nod, pss, res->_nodes->_pse);
        return toks;
    }

    return expo(res, toks, mem, stk);
}

tok_tp expo(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = post(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t op1 = *res->_nodes;

    while (toks->_typ == POW_T)
    {
        toks++;
        adv_nln1(toks);

        toks = fact(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        bop_n nod = bop_set(mem, POW_T, op1, *res->_nodes);
        op1 = node_set1(BOP_N, nod, op1._pss, res->_nodes->_pse);
    }

    *res->_nodes = op1;
    return toks;
}

tok_tp post(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks = core(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    if (toks->_typ == LSB_T)
    {
        node_t arr = *res->_nodes;

        toks++;
        adv_nln1(toks);

        toks = tupl(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        if (toks->_typ != RSB_T)
        {
            pres_fail(res, inv_syn_set("Expected ']' for array subscripting statement", toks->_pss, toks->_pse));
            return toks;
        }

        idx_n nod = idx_set(mem, arr, *res->_nodes);

        *res->_nodes = node_set1(IDX_N, nod, arr._pss, toks->_pse);

        toks++;
        adv_nln1(toks);

        return toks;
    }

    if (toks->_typ == INC_T || toks->_typ == DEC_T)
    {
        vfa_n nod = vfa_set(mem, toks->_typ == DEC_T ? 2 : 0, *res->_nodes);

        *res->_nodes = node_set1(VFA_N, nod, res->_nodes->_pss, toks->_pse);

        toks++;
        adv_nln1(toks);

        return toks;
    }

    if (toks->_typ == LPR_T)
        return hand_fcl(*res->_nodes, res, toks, mem, stk);

    if (toks->_typ == DOT_T)
    {
        node_t op1 = *res->_nodes;

        do
        {
            toks++;
            adv_nln1(toks);

            toks = post(res, toks, mem, stk);
            if (res->_herr)
                return toks;

            bop_n nod = bop_set(mem, DOT_T, op1, *res->_nodes);
            op1 = node_set1(BOP_N, nod, op1._pss, res->_nodes->_pse);
        } while (toks->_typ == POW_T);

        *res->_nodes = op1;
        return toks;
    }

    return toks;
}

tok_tp core(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    if (toks->_typ == LPR_T)
    {
        pos_t pss = toks++->_pss;

        adv_nln1(toks);

        toks = dlfc(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        if (toks->_typ != RPR_T)
        {
            pres_fail(res, inv_syn_set("Expected ')' for expression", toks->_pss, toks->_pse));
            return toks;
        }

        res->_nodes->_pss = pss;
        res->_nodes->_pse = toks++->_pse;

        adv_nln1(toks);

        return toks;
    }

    /* */

    if (toks->_typ == INT_T)
    {
        int_n nod = int_set(mem, toks->_val, toks->_len);

        *res->_nodes = node_set1(INT_N, nod, toks->_pss, toks->_pse);

        toks++;
        adv_nln1(toks);

        return toks;
    }
    if (toks->_typ == FLT_T)
    {
        flt_n nod = flt_set(mem, toks->_val, toks->_len);

        *res->_nodes = node_set1(FLT_N, nod, toks->_pss, toks->_pse);

        toks++;
        adv_nln1(toks);

        return toks;
    }

    if (toks->_typ == TRU_K)
    {
        bol_n nod = bol_set(mem, 1);

        *res->_nodes = node_set1(BOL_N, nod, toks->_pss, toks->_pse);

        toks++;
        adv_nln1(toks);

        return toks;
    }
    if (toks->_typ == FLS_K)
    {
        bol_n nod = bol_set(mem, 0);

        *res->_nodes = node_set1(BOL_N, nod, toks->_pss, toks->_pse);

        toks++;
        adv_nln1(toks);

        return toks;
    }

    if (toks->_typ == STR_T)
    {
        if (!toks->_val)
        {
            *res->_nodes = node_set2(STR_N, toks->_pss, toks->_pse);

            toks++;
            adv_nln1(toks);

            return toks;
        }

        str_n nod = str_set(mem, toks->_val, toks->_len);

        *res->_nodes = node_set1(STR_N, nod, toks->_pss, toks->_pse);

        toks++;
        adv_nln1(toks);

        return toks;
    }

    if (toks->_typ == LSB_T)
        return hand_lst(res, toks, mem, stk);
    if (toks->_typ == LCB_T)
        return hand_dct(res, toks, mem, stk);

    /* */

    if (toks->_typ == IDN_T)
    {
        vac_n nod = vac_set(mem, toks->_val);

        *res->_nodes = node_set1(VAC_N, nod, toks->_pss, toks->_pse);

        toks++;
        adv_nln1(toks);

        return toks;
    }

    if (toks->_typ == FUN_K)
        return hand_fdf(res, toks, mem, stk);

    if (toks->_typ == CLS_K)
        return hand_cdf(res, toks, mem, stk);

    if (toks->_typ == STT_K)
        return hand_sdf(res, toks, mem, stk);

    /* */

    if (toks->_typ == IFF_K)
        return hand_iff(res, toks, mem, stk);

    if (toks->_typ == SWH_K)
        return hand_swh(res, toks, mem, stk);

    /* */

    if (toks->_typ == FOR_K)
        return hand_for(res, toks, mem, stk);

    if (toks->_typ == WHL_K)
        return hand_whl(res, toks, mem, stk);
    if (toks->_typ == DOO_K)
        return hand_dow(res, toks, mem, stk);

    if (toks->_typ == LOP_K)
        return hand_lop(res, toks, mem, stk);

    /* */

    if (toks->_typ == TRY_K)
        return hand_try(res, toks, mem, stk);

    /* */

    if (toks->_typ == IMP_K)
    {
        pos_t pss = toks++->_pss;

        adv_nln1(toks);

        if (toks->_typ != IDN_T)
        {
            pres_fail(res, inv_syn_set("Expected identifier for import statement", toks->_pss, toks->_pse));
            return toks;
        }

        imp_n nod = imp_set(mem, toks->_val);

        *res->_nodes = node_set1(IMP_N, nod, pss, toks++->_pse);

        adv_nln1(toks);

        return toks;
    }

    pres_fail(res, inv_syn_set(NULL, toks->_pss, toks->_pse));
    return toks;
}

tok_tp hand_var(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    pos_t pss = toks++->_pss;

    adv_nln1(toks);

    uint8 prop = 0;
    toks = gen_prop(&prop, toks);

    if (toks->_typ != IDN_T)
    {
        pres_fail(res, inv_syn_set("Expected identifier for variable assign statement", toks->_pss, toks->_pse));
        return toks;
    }

    str name = toks++->_val;

    adv_nln1(toks);

    if (toks->_typ != ASN_T)
    {
        node_t val;
        val._typ = EOF_N;

        vas_n nod = vas_set(mem, name, val, prop);

        *res->_nodes = node_set1(VAS_N, nod, pss, (toks - 1)->_pse);
        return toks;
    }

    toks++;
    adv_nln1(toks);

    toks = tupl(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    vas_n nod = vas_set(mem, name, *res->_nodes, prop);

    *res->_nodes = node_set1(VAS_N, nod, pss, res->_nodes->_pse);
    return toks;
}

tok_tp hand_lst(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    pos_t pss = toks++->_pss;

    adv_nln1(toks);

    if (toks->_typ == RSB_T)
    {
        *res->_nodes = node_set2(LST_N, pss, toks++->_pse);

        adv_nln1(toks);

        return toks;
    }

    ptrp b_elms = ptr_alloc(stk, LIST_SIZ * sizeof(node_t), STK_SIZ);
    node_tp elms = *b_elms;

    uint64 alc = LIST_SIZ;
    uint64 siz = 1;

    toks = asgn(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    *elms = *res->_nodes;

    while (toks->_typ == CMA_T)
    {
        toks++;
        adv_nln1(toks);

        toks = asgn(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        if (siz == alc)
            elms = ptr_realloc(b_elms, (alc += LIST_SIZ) * sizeof(node_t));

        elms[siz++] = *res->_nodes;
    }

    if (toks->_typ != RSB_T)
    {
        pres_fail(res, inv_syn_set("Expected ']' for list", toks->_pss, toks->_pse));
        return toks;
    }

    if (siz != alc)
        elms = ptr_realloc(b_elms, siz * sizeof(node_t));

    lst_n nod = lst_set(mem, elms, siz);

    *res->_nodes = node_set1(LST_N, nod, pss, toks++->_pse);

    adv_nln1(toks);

    return toks;
}

tok_tp hand_dct(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    pos_t pss = toks++->_pss;

    adv_nln1(toks);

    if (toks->_typ == RCB_T)
    {
        *res->_nodes = node_set2(DCT_N, pss, toks++->_pse);

        adv_nln1(toks);

        return toks;
    }

    ptrp b_keys = ptr_alloc(stk, DICT_SIZ * sizeof(node_t), STK_SIZ);
    node_tp keys = *b_keys;

    ptrp b_vals = ptr_alloc(stk, DICT_SIZ * sizeof(node_t), STK_SIZ);
    node_tp vals = *b_vals;

    uint64 alc = LIST_SIZ;
    uint64 siz = 1;
    
    toks = asgn(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    *keys = *res->_nodes;

    if (toks->_typ != COL_T)
    {
        pres_fail(res, inv_syn_set("Expected ':' for dict", toks->_pss, toks->_pse));
        return toks;
    }

    toks++;
    adv_nln1(toks);

    toks = asgn(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    *vals = *res->_nodes;

    while (toks->_typ == CMA_T)
    {
        toks++;
        adv_nln1(toks);

        toks = asgn(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        if (siz == alc)
        {
            keys = ptr_realloc(b_keys, (alc += LIST_SIZ) * sizeof(node_t));

            vals = ptr_realloc(b_vals, alc * sizeof(node_t));
        }
        keys[siz] = *res->_nodes;

        if (toks->_typ != COL_T)
        {
            pres_fail(res, inv_syn_set("Expected ':' for dict", toks->_pss, toks->_pse));
            return toks;
        }

        toks++;
        adv_nln1(toks);

        toks = asgn(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        vals[siz++] = *res->_nodes;
    }

    if (toks->_typ != RCB_T)
    {
        pres_fail(res, inv_syn_set("Expected '}' for dict", toks->_pss, toks->_pse));
        return toks;
    }

    if (siz != alc)
    {
        keys = ptr_realloc(b_keys, siz * sizeof(node_t));

        vals = ptr_realloc(b_vals, siz * sizeof(node_t));
    }

    dct_n nod = dct_set(mem, keys, vals, siz);

    *res->_nodes = node_set1(DCT_N, nod, pss, toks++->_pse);

    adv_nln1(toks);

    return toks;
}

tok_tp hand_fdf(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    pos_t pss = toks++->_pss;
    pos_t pse;

    uint8 prop = 0;

    str name = NULL;

    adv_nln1(toks);

    if (toks->_typ != LPR_T)
    {
        toks = gen_prop(&prop, toks);

        if (toks->_typ != IDN_T)
        {
            pres_fail(res, inv_syn_set("Expected identifier for function definition statement", toks->_pss, toks->_pse));
            return toks;
        }

        name = toks++->_val;

        adv_nln1(toks);

        if (toks->_typ != LPR_T)
        {
            pres_fail(res, inv_syn_set("Expected '(' for function definition statement", toks->_pss, toks->_pse));
            return toks;
        }
    }

    arg_tp args = NULL;

    uint64 siz = 0;

    toks++;
    adv_nln1(toks);

    if (toks->_typ != RPR_T)
    {
        ptrp b_args = ptr_alloc(stk, FUN_ARG_SIZ * sizeof(arg_t), STK_SIZ);
        args = *b_args;

        uint64 alc = FUN_ARG_SIZ;
        siz = 1;

        if (toks->_typ != IDN_T)
        {
            pres_fail(res, inv_syn_set("Expected identifier for function definition statement", toks->_pss, toks->_pse));
            return toks;
        }

        if (siz == alc)
            args = ptr_realloc(b_args, (alc += FUN_ARG_SIZ) * sizeof(arg_t));

        args->_name = toks++->_val;

        adv_nln1(toks);

        if (toks->_typ == ASN_T)
        {
            toks++;
            adv_nln1(toks);

            toks = asgn(res, toks, mem, stk);
            if (res->_herr)
                return toks;

            args->_val = *res->_nodes;
        }
        else
            args->_val._typ = EOF_N;

        while (toks->_typ == CMA_T)
        {
            toks++;
            adv_nln1(toks);

            if (toks->_typ != IDN_T)
            {
                pres_fail(res, inv_syn_set("Expected identifier for function definition statement", toks->_pss, toks->_pse));
                return toks;
            }

            if (siz == alc)
                args = ptr_realloc(b_args, (alc += FUN_ARG_SIZ) * sizeof(arg_t));

            args[siz]._name = toks++->_val;

            adv_nln1(toks);

            if (toks->_typ == ASN_T)
            {
                toks++;
                adv_nln1(toks);

                toks = asgn(res, toks, mem, stk);
                if (res->_herr)
                    return toks;

                args[siz++]._val = *res->_nodes;
            }
            else
                args[siz++]._val._typ = EOF_N;
        }

        if (toks->_typ != RPR_T)
        {
            pres_fail(res, inv_syn_set("Expected ')' for function definition statement", toks->_pss, toks->_pse));
            return toks;
        }

        if (siz != alc)
            args = ptr_realloc(b_args, siz * sizeof(arg_t));
    }

    body_t body;

    toks++;
    adv_nln1(toks);

    if (toks->_typ == COL_T)
    {
        prop |= 16;

        toks++;
        adv_nln1(toks);

        toks = dlfc(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        body = body_set(mem, *res->_nodes);
        pse = res->_nodes->_pse;
    }
    else
    {
        if (toks->_typ != LCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '{' or ':' for function definition statement", toks->_pss, toks->_pse));
            return toks;
        }

        toks = gen_body(&body, FUN_BOD_SIZ, res, ++toks, mem, stk);
        if (res->_herr)
            return toks;

        if (toks->_typ != RCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '}' for function definition statement", toks->_pss, toks->_pse));
            return toks;
        }

        pse = toks++->_pse;

        adv_nln1(toks);
    }

    fdf_n nod = fdf_set(mem, name, args, siz, body, prop);

    *res->_nodes = node_set1(FDF_N, nod, pss, pse);
    return toks;
}

tok_tp hand_fcl(node_t func, pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    toks++;
    adv_nln1(toks);

    if (toks->_typ == RPR_T)
    {
        fcl_n nod = fcl_set(mem, func, NULL, 0);

        *res->_nodes = node_set1(FCL_N, nod, func._pss, toks++->_pse);

        adv_nln1(toks);

        return toks;
    }

    ptrp b_args = ptr_alloc(stk, FUN_ARG_SIZ * sizeof(arg_t), STK_SIZ);
    arg_tp args = *b_args;

    uint64 alc = FUN_ARG_SIZ;
    uint64 siz = 1;

    if (toks->_typ == IDN_T)
    {
        tok_tp tmp = toks;

        str name = toks++->_val;

        adv_nln1(toks);

        if (toks->_typ == ASN_T)
        {
            args->_name = name;

            toks++;
            adv_nln1(toks);
        }
        else
        {
            args->_name = NULL;
            toks = tmp;
        }
    }
    else
        args->_name = NULL;

    toks = asgn(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    args->_val = *res->_nodes;

    while (toks->_typ == CMA_T)
    {
        if (siz == alc)
            args = ptr_realloc(b_args, (alc += FUN_ARG_SIZ) * sizeof(arg_t));

        toks++;
        adv_nln1(toks);

        if (toks->_typ == IDN_T)
        {
            tok_tp tmp = toks;

            str name = toks++->_val;

            adv_nln1(toks);

            if (toks->_typ == ASN_T)
            {
                args[siz]._name = name;

                toks++;
                adv_nln1(toks);
            }
            else
            {
                args[siz]._name = NULL;
                toks = tmp;
            }
        }
        else
            args[siz]._name = NULL;

        toks = asgn(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        args[siz++]._val = *res->_nodes;
    }

    if (toks->_typ != RPR_T)
    {
        pres_fail(res, inv_syn_set("Expected ')' for function call statement", toks->_pss, toks->_pse));
        return toks;
    }

    if (siz != alc)
        args = ptr_realloc(b_args, siz * sizeof(arg_t));

    fcl_n nod = fcl_set(mem, func, args, siz);

    *res->_nodes = node_set1(FCL_N, nod, func._pss, toks++->_pse);

    adv_nln1(toks);

    return toks;
}

tok_tp hand_cdf(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    pos_t pss = toks++->_pss;

    uint8 prop = 0;

    str name = NULL;

    adv_nln1(toks);

    if (toks->_typ != LCB_T)
    {
        toks = gen_prop(&prop, toks);

        if (toks->_typ != IDN_T)
        {
            pres_fail(res, inv_syn_set("Expected identifier for class definition statement", toks->_pss, toks->_pse));
            return toks;
        }

        name = toks++->_val;

        adv_nln1(toks);

        if (toks->_typ != LCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '{' for class definition statement", toks->_pss, toks->_pse));
            return toks;
        }
    }

    body_t body;

    toks = gen_body(&body, CLS_BOD_SIZ, res, ++toks, mem, stk);
    if (res->_herr)
        return toks;

    if (toks->_typ != RCB_T)
    {
        pres_fail(res, inv_syn_set("Expected '}' for class definition statement", toks->_pss, toks->_pse));
        return toks;
    }

    cdf_n nod = cdf_set(mem, name, body, prop);

    *res->_nodes = node_set1(CDF_N, nod, pss, toks++->_pse);

    adv_nln1(toks);

    return toks;
}

tok_tp hand_sdf(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    pos_t pss = toks++->_pss;

    uint8 prop = 0;

    str name = NULL;

    adv_nln1(toks);

    if (toks->_typ != LCB_T)
    {
        toks = gen_prop(&prop, toks);

        if (toks->_typ != IDN_T)
        {
            pres_fail(res, inv_syn_set("Expected identifier for struct definition statement", toks->_pss, toks->_pse));
            return toks;
        }

        name = toks++->_val;

        adv_nln1(toks);

        if (toks->_typ != LCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '{' for struct definition statement", toks->_pss, toks->_pse));
            return toks;
        }
    }

    body_t body;

    toks = gen_body(&body, STT_BOD_SIZ, res, ++toks, mem, stk);
    if (res->_herr)
        return toks;

    if (toks->_typ != RCB_T)
    {
        pres_fail(res, inv_syn_set("Expected '}' for struct definition statement", toks->_pss, toks->_pse));
        return toks;
    }

    sdf_n nod = sdf_set(mem, name, body, prop);

    *res->_nodes = node_set1(SDF_N, nod, pss, toks++->_pse);

    adv_nln1(toks);

    return toks;
}

tok_tp hand_iff(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    pos_t pss = toks++->_pss;
    pos_t pse;

    ptrp b_stats = ptr_alloc(stk, IFF_CND_SIZ * sizeof(stat_t), STK_SIZ);
    stat_tp stats = *b_stats;

    uint64 alc = IFF_CND_SIZ;
    uint64 siz = 1;

    adv_nln1(toks);

    toks = tupl(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    stats->_cond = *res->_nodes;

    if (toks->_typ == COL_T)
    {
        toks++;
        adv_nln1(toks);

        toks = dlfc(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        stats->_body = body_set(mem, *res->_nodes);
        pse = res->_nodes->_pse;
    }
    else
    {
        if (toks->_typ != LCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '{' or ':' for if statement", toks->_pss, toks->_pse));
            return toks;
        }

        toks = gen_body(&stats->_body, IFF_BOD_SIZ, res, ++toks, mem, stk);
        if (res->_herr)
            return toks;

        if (toks->_typ != RCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '}' for if statement", toks->_pss, toks->_pse));
            return toks;
        }

        pse = (++toks)->_pse;

        adv_nln1(toks);
    }

    while (toks->_typ == ELF_K)
    {
        if (siz == alc)
            stats = ptr_realloc(b_stats, (alc += IFF_CND_SIZ) * sizeof(stat_t));

        toks++;
        adv_nln1(toks);

        toks = tupl(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        stats[siz]._cond = *res->_nodes;

        if (toks->_typ == COL_T)
        {
            toks++;
            adv_nln1(toks);

            toks = dlfc(res, toks, mem, stk);
            if (res->_herr)
                return toks;

            stats[siz++]._body = body_set(mem, *res->_nodes);
            pse = res->_nodes->_pse;
            continue;
        }

        if (toks->_typ != LCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '{' or ':' for if statement", toks->_pss, toks->_pse));
            return toks;
        }

        toks = gen_body(&stats[siz++]._body, IFF_BOD_SIZ, res, ++toks, mem, stk);
        if (res->_herr)
            return toks;

        if (toks->_typ != RCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '}' for if statement", toks->_pss, toks->_pse));
            return toks;
        }

        pse = (++toks)->_pse;

        adv_nln1(toks);
    }

    body_t ebody;

    if (toks->_typ == ELS_K)
    {
        toks++;
        adv_nln1(toks);

        if (toks->_typ == COL_T)
        {
            toks++;
            adv_nln1(toks);

            toks = dlfc(res, toks, mem, stk);
            if (res->_herr)
                return toks;

            ebody = body_set(mem, *res->_nodes);
            pse = res->_nodes->_pse;
        }
        else
        {
            if (toks->_typ != LCB_T)
            {
                pres_fail(res, inv_syn_set("Expected '{' or ':' for if statement", toks->_pss, toks->_pse));
                return toks;
            }

            toks = gen_body(&ebody, IFF_BOD_SIZ, res, ++toks, mem, stk);
            if (res->_herr)
                return toks;

            if (toks->_typ != RCB_T)
            {
                pres_fail(res, inv_syn_set("Expected '}' for if statement", toks->_pss, toks->_pse));
                return toks;
            }

            pse = (++toks)->_pse;

            adv_nln1(toks);
        }
    }
    else
        ebody._siz = EOF_N;

    if (siz != alc)
        stats = ptr_realloc(b_stats, siz * sizeof(stat_t));

    iff_n nod = iff_set(mem, stats, siz, ebody);

    *res->_nodes = node_set1(IFF_N, nod, pss, pse);
    return toks;
}

tok_tp hand_swh(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    pos_t pss = toks++->_pss;

    adv_nln1(toks);

    toks = tupl(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t val = *res->_nodes;

    if (toks->_typ != LCB_T)
    {
        pres_fail(res, inv_syn_set("Expected '{' for switch statement", toks->_pss, toks->_pse));
        return toks;
    }

    toks++;
    adv_nln1(toks);

    stat_tp stats = NULL;

    uint64 siz = 0;

    if (toks->_typ == CAS_K)
    {
        ptrp b_stats = ptr_alloc(stk, SWH_CAS_SIZ * sizeof(stat_t), STK_SIZ);
        stats = *b_stats;

        uint64 alc = SWH_CAS_SIZ;

        do
        {
            toks++;
            adv_nln1(toks);
            
            toks = tupl(res, toks, mem, stk);
            if (res->_herr)
                return toks;

            if (siz == alc)
                stats = ptr_realloc(b_stats, (alc += SWH_CAS_SIZ) * sizeof(stat_t));

            stats[siz]._cond = *res->_nodes;

            if (toks->_typ != COL_T)
            {
                pres_fail(res, inv_syn_set("Expected ':' for switch statement", toks->_pss, toks->_pse));
                return toks;
            }

            toks = gen_case(&stats[siz++]._body, SWH_BOD_SIZ, res, ++toks, mem, stk);
            if (res->_herr)
                return toks;
        } while (toks->_typ == CAS_K);

        if (siz != alc)
            stats = ptr_realloc(b_stats, siz * sizeof(stat_t));
    }

    body_t dbody;

    if (toks->_typ == DEF_K)
    {
        toks++;
        adv_nln1(toks);

        if (toks->_typ != COL_T)
        {
            pres_fail(res, inv_syn_set("Expected ':' for switch statement", toks->_pss, toks->_pse));
            return toks;
        }

        toks = gen_body(&dbody, SWH_BOD_SIZ, res, ++toks, mem, stk);
        if (res->_herr)
            return toks;
    }
    else
        dbody._siz = 0;

    if (toks->_typ != RCB_T)
    {
        pres_fail(res, inv_syn_set("Expected '}' for switch statement", toks->_pss, toks->_pse));
        return toks;
    }

    swh_n nod = swh_set(mem, val, stats, siz, dbody);

    *res->_nodes = node_set1(SWH_N, nod, pss, toks++->_pse);

    adv_nln1(toks);

    return toks;
}

tok_tp hand_for(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    pos_t pss = toks++->_pss;
    pos_t pse;

    adv_nln1(toks);

    if (toks->_typ != IDN_T)
    {
        pres_fail(res, inv_syn_set("Expected identifier for for statement", toks->_pss, toks->_pse));
        return toks;
    }

    str var = toks++->_val;

    adv_nln1(toks);

    if (toks->_typ == INN_K)
        return hand_fre(var, pss, res, ++toks, mem, stk);

    if (toks->_typ != ASN_T)
    {
        pres_fail(res, inv_syn_set("Expected '=' for for statement or 'in' for foreach statement", toks->_pss, toks->_pse));
        return toks;
    }

    toks++;
    adv_nln1(toks);

    node_t start;

    if (toks->_typ == TOO_K)
        start._typ = EOF_N;
    else
    {
        toks = tupl(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        start = *res->_nodes;

        if (toks->_typ != TOO_K)
        {
            pres_fail(res, inv_syn_set("Expected 'to' for for statement", toks->_pss, toks->_pse));
            return toks;
        }
    }

    toks++;
    adv_nln1(toks);

    toks = tupl(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t end = *res->_nodes;

    node_t step;

    if (toks->_typ == STP_K)
    {
        toks++;
        adv_nln1(toks);

        toks = tupl(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        step = *res->_nodes;
    }
    else
        step._typ = EOF_N;

    body_t body;

    if (toks->_typ == COL_T)
    {
        toks++;
        adv_nln1(toks);

        toks = dlfc(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        body = body_set(mem, *res->_nodes);
        pse = res->_nodes->_pse;
    }
    else
    {
        if (toks->_typ != LCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '{' or ':' for for statement", toks->_pss, toks->_pse));
            return toks;
        }

        toks = gen_body(&body, FOR_BOD_SIZ, res, ++toks, mem, stk);
        if (res->_herr)
            return toks;

        if (toks->_typ != RCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '}' for for statement", toks->_pss, toks->_pse));
            return toks;
        }

        pse = toks++->_pse;

        adv_nln1(toks);
    }

    for_n nod = for_set(mem, var, start, end, step, body);

    *res->_nodes = node_set1(FOR_N, nod, pss, pse);
    return toks;
}

tok_tp hand_fre(str var, pos_t pss, pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    pos_t pse;

    adv_nln1(toks);

    toks = tupl(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t iter = *res->_nodes;

    body_t body;

    if (toks->_typ == COL_T)
    {
        toks++;
        adv_nln1(toks);

        toks = dlfc(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        body = body_set(mem, *res->_nodes);
        pse = res->_nodes->_pse;
    }
    else
    {
        if (toks->_typ != LCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '{' or ':' for foreach statement", toks->_pss, toks->_pse));
            return toks;
        }

        toks = gen_body(&body, FRE_BOD_SIZ, res, ++toks, mem, stk);
        if (res->_herr)
            return toks;

        if (toks->_typ != RCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '}' for foreach statement", toks->_pss, toks->_pse));
            return toks;
        }

        pse = toks++->_pse;

        adv_nln1(toks);
    }

    fre_n nod = fre_set(mem, var, iter, body);

    *res->_nodes = node_set1(FRE_N, nod, pss, pse);
    return toks;
}

tok_tp hand_whl(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    pos_t pss = toks++->_pss;
    pos_t pse;

    adv_nln1(toks);

    toks = tupl(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    node_t cond = *res->_nodes;

    body_t body;

    if (toks->_typ == COL_T)
    {
        toks++;
        adv_nln1(toks);

        toks = dlfc(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        body = body_set(mem, *res->_nodes);
        pse = res->_nodes->_pse;
    }
    else
    {
        if (toks->_typ != LCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '{' or ':' for while statement", toks->_pss, toks->_pse));
            return toks;
        }

        toks = gen_body(&body, WHL_BOD_SIZ, res, ++toks, mem, stk);
        if (res->_herr)
            return toks;

        if (toks->_typ != RCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '}' for while statement", toks->_pss, toks->_pse));
            return toks;
        }

        pse = toks++->_pse;

        adv_nln1(toks);
    }

    whl_n nod = whl_set(mem, cond, body);

    *res->_nodes = node_set1(WHL_N, nod, pss, pse);
    return toks;
}

tok_tp hand_dow(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    pos_t pss = toks++->_pss;

    body_t body;

    adv_nln1(toks);

    if (toks->_typ == COL_T)
    {
        toks++;
        adv_nln1(toks);

        toks = dlfc(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        body = body_set(mem, *res->_nodes);
    }
    else
    {
        if (toks->_typ != LCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '{' or ':' for do-while statement", toks->_pss, toks->_pse));
            return toks;
        }

        toks = gen_body(&body, DOW_BOD_SIZ, res, ++toks, mem, stk);
        if (res->_herr)
            return toks;

        if (toks->_typ != RCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '}' for do-while statement", toks->_pss, toks->_pse));
            return toks;
        }

        toks++;
        adv_nln1(toks);
    }

    if (toks->_typ != WHL_K)
    {
        pres_fail(res, inv_syn_set("Expected 'while' for do-while statement", toks->_pss, toks->_pse));
        return toks;
    }

    toks++;
    adv_nln1(toks);

    toks = tupl(res, toks, mem, stk);
    if (res->_herr)
        return toks;

    dow_n nod = dow_set(mem, body, *res->_nodes);

    *res->_nodes = node_set1(DOW_N, nod, pss, res->_nodes->_pse);
    return toks;
}

tok_tp hand_lop(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    pos_t pss = toks++->_pss;
    pos_t pse;

    adv_nln1(toks);

    node_t init;

    if (toks->_typ == SEM_T)
        init._typ = EOF_N;
    else
    {
        toks = asgn(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        init = *res->_nodes;

        if (toks->_typ != SEM_T)
        {
            pres_fail(res, inv_syn_set("Expected ';' for loop statement", toks->_pss, toks->_pse));
            return toks;
        }
    }

    node_t cond;

    toks++;
    adv_nln1(toks);

    if (toks->_typ == SEM_T)
        cond._typ = EOF_N;
    else
    {
        toks = asgn(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        cond = *res->_nodes;

        if (toks->_typ != SEM_T)
        {
            pres_fail(res, inv_syn_set("Expected ';' for loop statement", toks->_pss, toks->_pse));
            return toks;
        }
    }

    node_t step;
    body_t body;

    toks++;
    adv_nln1(toks);

    if (toks->_typ == COL_T)
    {
        step._typ = EOF_N;

        toks++;
        adv_nln1(toks);

        toks = dlfc(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        body = body_set(mem, *res->_nodes);
        pse = res->_nodes->_pse;
    }
    else if (toks->_typ == LCB_T)
    {
        step._typ = EOF_N;

        toks = gen_body(&body, LOP_BOD_SIZ, res, ++toks, mem, stk);
        if (res->_herr)
            return toks;

        if (toks->_typ != RCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '}' for loop statement", toks->_pss, toks->_pse));
            return toks;
        }

        pse = toks++->_pse;

        adv_nln1(toks);
    }
    else
    {
        toks = asgn(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        step = *res->_nodes;

        if (toks->_typ == COL_T)
        {
            toks++;
            adv_nln1(toks);

            toks = dlfc(res, toks, mem, stk);
            if (res->_herr)
                return toks;

            body = body_set(mem, *res->_nodes);
            pse = res->_nodes->_pse;
        }
        else
        {
            if (toks->_typ != LCB_T)
            {
                pres_fail(res, inv_syn_set("Expected '{' or ':' for loop statement", toks->_pss, toks->_pse));
                return toks;
            }

            toks = gen_body(&body, LOP_BOD_SIZ, res, ++toks, mem, stk);
            if (res->_herr)
                return toks;

            if (toks->_typ != RCB_T)
            {
                pres_fail(res, inv_syn_set("Expected '}' for loop statement", toks->_pss, toks->_pse));
                return toks;
            }

            pse = toks++->_pse;

            adv_nln1(toks);
        }
    }

    lop_n nod = lop_set(mem, init, cond, step, body);

    *res->_nodes = node_set1(LOP_N, nod, pss, pse);
    return toks;
}

tok_tp hand_try(pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    pos_t pss = toks++->_pss;
    pos_t pse;

    adv_nln1(toks);

    body_t error;

    if (toks->_typ == COL_T)
    {
        toks++;
        adv_nln1(toks);

        toks = dlfc(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        error = body_set(mem, *res->_nodes);
        pse = res->_nodes->_pse;
    }
    else
    {
        if (toks->_typ != LCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '{' or ':' for try statement", toks->_pss, toks->_pse));
            return toks;
        }

        toks = gen_body(&error, TRY_BOD_SIZ, res, ++toks, mem, stk);
        if (res->_herr)
            return toks;

        if (toks->_typ != RCB_T)
        {
            pres_fail(res, inv_syn_set("Expected '}' for try statement", toks->_pss, toks->_pse));
            return toks;
        }

        pse = toks++->_pse;

        adv_nln1(toks);
    }

    stat_tp stats = NULL;

    uint64 siz = 0;

    if (toks->_typ == EXP_K)
    {
        ptrp b_stats = ptr_alloc(stk, TRY_EXP_SIZ * sizeof(stat_t), STK_SIZ);
        stats = *b_stats;

        uint64 alc = TRY_EXP_SIZ;

        do
        {
            toks++;
            adv_nln1(toks);

            toks = tupl(res, toks, mem, stk);
            if (res->_herr)
                return toks;

            if (siz == alc)
                stats = ptr_realloc(b_stats, (alc += TRY_EXP_SIZ) * sizeof(stat_t));

            stats[siz]._cond = *res->_nodes;

            if (toks->_typ == COL_T)
            {
                toks++;
                adv_nln1(toks);

                toks = dlfc(res, toks, mem, stk);
                if (res->_herr)
                    return toks;

                stats[siz++]._body = body_set(mem, *res->_nodes);

                pse = res->_nodes->_pse;
                continue;
            }

            if (toks->_typ != LCB_T)
            {
                pres_fail(res, inv_syn_set("Expected '{' or ':' for try statement", toks->_pss, toks->_pse));
                return toks;
            }

            toks = gen_body(&stats[siz++]._body, TRY_BOD_SIZ, res, ++toks, mem, stk);
            if (res->_herr)
                return toks;

            if (toks->_typ != RCB_T)
            {
                pres_fail(res, inv_syn_set("Expected '}' for try statement", toks->_pss, toks->_pse));
                return toks;
            }

            pse = toks++->_pse;

            adv_nln1(toks);
        } while (toks->_typ == EXP_K);

        if (siz != alc)
            stats = ptr_realloc(b_stats, siz * sizeof(stat_t));
    }

    body_t fbody;

    if (toks->_typ == FNL_K)
    {
        toks++;
        adv_nln1(toks);

        if (toks->_typ == COL_T)
        {
            toks++;
            adv_nln1(toks);

            toks = dlfc(res, toks, mem, stk);
            if (res->_herr)
                return toks;

            fbody = body_set(mem, *res->_nodes);
            pse = res->_nodes->_pse;
        }
        else
        {
            if (toks->_typ != LCB_T)
            {
                pres_fail(res, inv_syn_set("Expected '{' or ':' for try statement", toks->_pss, toks->_pse));
                return toks;
            }

            toks = gen_body(&fbody, TRY_BOD_SIZ, res, ++toks, mem, stk);
            if (res->_herr)

            if (toks->_typ != RCB_T)
            {
                pres_fail(res, inv_syn_set("Expected '}' for try statement", toks->_pss, toks->_pse));
                return toks;
            }

            pse = toks++->_pse;

            adv_nln1(toks);
        }
    }
    else
        fbody._siz = 0;

    try_n nod = try_set(mem, error, stats, siz, fbody);

    *res->_nodes = node_set1(TRY_N, nod, pss, pse);
    return toks;
}

tok_tp gen_prop(uint8p prop, tok_tp toks)
{
    uint8 acc = 0;
    uint8 ctx = 0;
    uint8 cst = 0;
    uint8 stc = 0;

    while (1)
    {
        if (toks->_typ == PBL_K && !acc)
        {
            acc = 1;
            *prop |= 8;

            toks++;
            adv_nln1(toks);

            continue;
        }
        else if (toks->_typ == PRV_K && !acc)
        {
            acc = 1;

            toks++;
            adv_nln1(toks);

            continue;
        }

        if (toks->_typ == GLB_K && !ctx)
        {
            ctx = 1;
            *prop |= 4;

            toks++;
            adv_nln1(toks);

            continue;
        }
        else if (toks->_typ == LCL_K && !ctx)
        {
            ctx = 0;

            toks++;
            adv_nln1(toks);

            continue;
        }

        if (toks->_typ == CST_K && !cst)
        {
            cst = 1;
            *prop |= 2;

            toks++;
            adv_nln1(toks);

            continue;
        }

        if (toks->_typ == STC_K && !stc)
        {
            stc = 1;
            *prop |= 1;

            toks++;
            adv_nln1(toks);

            continue;
        }

        adv_nln1(toks);

        return toks;
    }
}

tok_tp gen_body(body_tp body, uint64 alt, pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    ptrp b_nodes = ptr_alloc(stk, alt * sizeof(node_t), STK_SIZ);
    body->_nodes = *b_nodes;

    uint64 alc = alt;
    body->_siz = 0;

    do
    {
        adv_nln2(toks);

        if (toks->_typ == RCB_T || toks->_typ == EOF_T)
            break;

        if (body->_siz == alc)
            body->_nodes = ptr_realloc(b_nodes, (alc += alt) * sizeof(node_t));

        toks = dlfc(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        body->_nodes[body->_siz++] = *res->_nodes;
    } while ((toks - 1)->_typ == NLN_T || toks->_typ == SEM_T);

    if (!body->_siz)
    {
        free(body->_nodes);
        stk->_cnt--;

        return toks;
    }

    if (body->_siz != alc)
        body->_nodes = ptr_realloc(b_nodes, body->_siz * sizeof(node_t));

    return toks;
}

tok_tp gen_case(body_tp body, uint64 alt, pres_tp res, tok_tp toks, mem_t mem, stk_t stk)
{
    ptrp b_nodes = ptr_alloc(stk, alt * sizeof(node_t), STK_SIZ);
    body->_nodes = *b_nodes;

    uint64 alc = alt;
    body->_siz = 0;

    do
    {
        adv_nln2(toks);

        if (toks->_typ == CAS_K || toks->_typ == DEF_K || toks->_typ == RCB_T || toks->_typ == EOF_T)
            break;

        if (body->_siz == alc)
            body->_nodes = ptr_realloc(b_nodes, (alc += alt) * sizeof(node_t));

        toks = dlfc(res, toks, mem, stk);
        if (res->_herr)
            return toks;

        body->_nodes[body->_siz++] = *res->_nodes;
    } while ((toks - 1)->_typ == NLN_T || toks->_typ == SEM_T);

    if (!body->_siz)
    {
        free(body->_nodes);
        stk->_cnt--;

        return toks;
    }

    if (body->_siz != alc)
        body->_nodes = ptr_realloc(b_nodes, body->_siz * sizeof(node_t));

    return toks;
}
