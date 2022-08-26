// MetaReal Programming Language version 1.0.0

#include <stdlib.h>

#include <lexer/lexer.h>
#include <mem/blk.h>
#include <debug/crash.h>

#define tok_set(t)                            \
    do                                        \
    {                                         \
        pss = cpos;                           \
                                              \
        pos_adv(cpos);                        \
        code++;                               \
                                              \
        toks[siz++] = tok_set2(t, pss, cpos); \
    } while (0)

lres_t lres_succ(tok_tp toks);
lres_t lres_fail(ill_chr_t error);

cstr comment(cstr code, uint8 term, pos_tp cpos);

cstr gen_idn(tok_tp tok, mem_t mem, cstr code, pos_tp cpos);

cstr gen_num(tok_tp tok, mem_t mem, cstr code, pos_tp cpos);

cstr gen_str(tok_tp tok, mem_t mem, cstr code, uint8 term, pos_tp cpos);
cstr gen_etr(tok_tp tok, mem_t mem, cstr code, uint8 term, pos_tp cpos, uint8p flg);

cstr gen_double(tok_tp tok, cstr code, uint8 typ1, uint8 typ2, uint8 chr, pos_tp cpos);
cstr gen_triple(tok_tp tok, cstr code, uint8 typ1, uint8 typ2, uint8 typ3, uint8 chr1, uint8 chr2, pos_tp cpos);

cstr gen_mul(tok_tp tok, cstr code, pos_tp cpos);
cstr gen_div(tok_tp tok, cstr code, pos_tp cpos);
cstr gen_lst(tok_tp tok, cstr code, pos_tp cpos);
cstr gen_grt(tok_tp tok, cstr code, pos_tp cpos);

lres_t lex(cstr code, uint8 term, cstr fn, mem_t mem)
{
    tok_tp toks = malloc(TOKS_SIZ * sizeof(tok_t));

    if (!toks)
        mem_error(TOKS_SIZ * sizeof(tok_t));

    uint64 alc = TOKS_SIZ;
    uint64 siz = 0;

    pos_t pss;
    pos_t cpos = pos_set(0, 0, fn);

    while (*code != term)
    {
        if (siz == alc)
        {
            toks = realloc(toks, (alc += TOKS_SIZ) * sizeof(tok_t));

            if (!toks)
                mem_error(alc * sizeof(tok_t));
        }

        if (*code == ' ' || *code == '\t')
        {
            pos_adv(cpos);
            code++;
            continue;
        }

        if (*code == '#')
        {
            code = comment(code, term, &cpos);
            continue;
        }

        if (*code == '\n')
        {
            pss = cpos;

            pos_jmp(cpos);
            code++;

            if (toks[siz - 1]._typ == NLN_T)
                continue;

            toks[siz++] = tok_set2(NLN_T, pss, cpos);
            continue;
        }
        if (*code == ';')
        {
            pss = cpos;

            pos_adv(cpos);
            code++;

            toks[siz++] = tok_set2(SEM_T, pss, cpos);
            continue;
        }

        if (*code == '\\')
        {
            uint8 flg = 0;
            code = gen_etr(&toks[siz++], mem, code, term, &cpos, &flg);

            if (flg)
            {
                free(toks);
                return lres_fail(ill_chr_set(*code, cpos));
            }
            continue;
        }

        if ((*code >= 'a' && *code <= 'z') || (*code >= 'A' && *code <= 'Z') || *code == '_')
        {
            code = gen_idn(&toks[siz++], mem, code, &cpos);
            continue;
        }

        if (*code >= '0' && *code <= '9')
        {
            code = gen_num(&toks[siz++], mem, code, &cpos);
            continue;
        }

        switch (*code)
        {
        case '"':
        case '\'':
            code = gen_str(&toks[siz++], mem, code, term, &cpos);
            break;

        case '+':
            code = gen_triple(&toks[siz++], code, ADD_T, ADE_T, INC_T, '=', '+', &cpos);
            break;
        case '-':
            code = gen_triple(&toks[siz++], code, SUB_T, SBE_T, DEC_T, '=', '-', &cpos);
            break;
        case '*':
            code = gen_mul(&toks[siz++], code, &cpos);
            break;
        case '/':
            code = gen_div(&toks[siz++], code, &cpos);
            break;
        case '%':
            code = gen_double(&toks[siz++], code, MOD_T, MDE_T, '=', &cpos);
            break;

        case '&':
            code = gen_triple(&toks[siz++], code, BAN_T, AND_T, BAE_T, '&', '=', &cpos);
            break;
        case '|':
            code = gen_triple(&toks[siz++], code, BIO_T, IOR_T, BIE_T, '|', '=', &cpos);
            break;
        case '^':
            code = gen_triple(&toks[siz++], code, BXO_T, XOR_T, BXE_T, '^', '=', &cpos);
            break;

        case '~':
            tok_set(BNT_T);
            break;

        case '<':
            code = gen_lst(&toks[siz++], code, &cpos);
            break;
        case '>':
            code = gen_grt(&toks[siz++], code, &cpos);
            break;
        case '=':
            code = gen_double(&toks[siz++], code, ASN_T, EQU_T, '=', &cpos);
            break;

        case '!':
            code = gen_double(&toks[siz++], code, NOT_T, NEQ_T, '=', &cpos);
            break;

        case '(':
            tok_set(LPR_T);
            break;
        case ')':
            tok_set(RPR_T);
            break;

        case '{':
            tok_set(LCB_T);
            break;
        case '}':
            tok_set(RCB_T);
            break;

        case '[':
            tok_set(LSB_T);
            break;
        case ']':
            tok_set(RSB_T);
            break;

        case '.':
            tok_set(DOT_T);
            break;
        case ':':
            tok_set(COL_T);
            break;

        case '?':
            tok_set(QUE_T);
            break;
        case '$':
            tok_set(DLR_T);
            break;
        case ',':
            tok_set(CMA_T);
            break;
        default:
            free(toks);
            return lres_fail(ill_chr_set(*code, cpos));
        }
    }

    toks = realloc(toks, (siz + 1) * sizeof(tok_t));

    if (!toks)
        mem_error((siz + 1) * sizeof(tok_t));

    pss = cpos;
    pos_adv(cpos);

    toks[siz] = tok_set2(EOF_T, pss, cpos);

    return lres_succ(toks);
}

lres_t lres_succ(tok_tp toks)
{
    lres_t res;

    res._toks = toks;
    res._herr = 0;

    return res;
}

lres_t lres_fail(ill_chr_t error)
{
    lres_t res;

    res._error = error;
    res._herr = 1;

    return res;
}

cstr comment(cstr code, uint8 term, pos_tp cpos)
{
    posp_adv(cpos);
    code++;

    if (*code == '*')
    {
        posp_adv(cpos);
        code++;

        while (*code != term)
        {
            if (*code == '*')
            {
                posp_adv(cpos);
                code++;

                if (*code == '#')
                {
                    posp_adv(cpos);
                    return ++code;
                }
            }

            if (*code++ == '\n')
                posp_jmp(cpos);
            else
                posp_adv(cpos);
        }

        return code;
    }

    while (*code != term)
    {
        if (*code++ == '\n')
        {
            posp_jmp(cpos);
            return code;
        }

        posp_adv(cpos);
    }

    return code;
}

cstr gen_idn(tok_tp tok, mem_t mem, cstr code, pos_tp cpos)
{
    pos_t pss = *cpos;

    str idn = blk_alloc(mem, GIDN_SIZ, MEM_SIZ);
    uint64 alc = GIDN_SIZ;
    uint64 siz = 0;

    do
    {
        if (siz == alc)
        {
            blk_add(mem, GIDN_SIZ, MEM_SIZ);
            alc += GIDN_SIZ;
        }

        idn[siz++] = *code++;
        posp_adv(cpos);
    } while ((*code >= 'a' && *code <= 'z') || (*code >= 'A' && *code <= 'Z') || *code == '_');

    if (siz == alc)
        blk_add(mem, 1, MEM_SIZ);

    idn[siz++] = '\0';

    uint8 kwd = is_kwd(idn);
    if (kwd == 255)
    {
        *tok = tok_set1(IDN_T, idn, 0, pss, *cpos);

        blk_sub(mem, idn, siz);
    }
    else
    {
        *tok = tok_set2(kwd, pss, *cpos);

        blk_free(mem, idn);
    }

    return code;
}

cstr gen_num(tok_tp tok, mem_t mem, cstr code, pos_tp cpos)
{
    pos_t pss = *cpos;

    str num = blk_alloc(mem, GNUM_SIZ, MEM_SIZ);
    uint64 alc = GNUM_SIZ;
    uint64 siz = 0;

    uint8 isf = 0;

    do
    {
        if (siz == alc)
        {
            blk_add(mem, GNUM_SIZ, MEM_SIZ);
            alc += GNUM_SIZ;
        }

        if (*code == '.')
        {
            if (isf)
                break;
            isf = 1;
        }

        num[siz++] = *code++;
        posp_adv(cpos);
    } while ((*code >= '0' && *code <= '9') || *code == '.');

    if (*code == 'e')
    {
        isf = 1;

        if (siz == alc)
        {
            blk_add(mem, GEXP_SIZ, MEM_SIZ);
            alc += GEXP_SIZ;
        }

        num[siz++] = *code++;
        posp_adv(cpos);

        if (*code == '+' || *code == '-')
        {
            if (siz == alc)
            {
                blk_add(mem, GEXP_SIZ, MEM_SIZ);
                alc += GEXP_SIZ;
            }

            num[siz++] = *code++;
            posp_adv(cpos);
        }

        while (*code >= '0' && *code <= '9')
        {
            if (siz == alc)
            {
                blk_add(mem, GEXP_SIZ, MEM_SIZ);
                alc += GEXP_SIZ;
            }

            num[siz++] = *code++;
            posp_adv(cpos);
        }
    }

    if (siz == alc)
    {
        blk_add(mem, 1, MEM_SIZ);

        num[siz++] = '\0';
    }
    else
    {
        num[siz++] = '\0';

        blk_sub(mem, num, siz);
    }

    if (isf)
        *tok = tok_set1(FLT_T, num, siz, pss, *cpos);
    else
        *tok = tok_set1(INT_T, num, siz, pss, *cpos);

    return code;
}

cstr gen_str(tok_tp tok, mem_t mem, cstr code, uint8 term, pos_tp cpos)
{
    pos_t pss = *cpos;
    uint8 quote = *code++;
    posp_adv(cpos);

    if (*code == quote)
    {
        posp_adv(cpos);

        *tok = tok_set2(STR_T, pss, *cpos);

        return ++code;
    }

    str str = blk_alloc(mem, GSTR_SIZ, MEM_SIZ);
    uint64 alc = GSTR_SIZ;
    uint64 siz = 0;

    uint8 spc_chr = 0;
    while ((*code != quote || spc_chr) && *code != term)
    {
        if (siz == alc)
        {
            blk_add(mem, GSTR_SIZ, MEM_SIZ);
            alc += GSTR_SIZ;
        }

        if (*code == '\\' && !spc_chr)
        {
            spc_chr = 1;

            posp_adv(cpos);

            str[siz++] = *code++;
            continue;
        }

        spc_chr = 0;

        if (*code == '\n')
            posp_jmp(cpos);
        else
            posp_adv(cpos);

        str[siz++] = *code++;
    }

    posp_adv(cpos);

    if (siz == alc)
    {
        blk_add(mem, 1, MEM_SIZ);

        str[siz++] = '\0';
    }
    else
    {
        str[siz++] = '\0';

        blk_sub(mem, str, siz);
    }

    *tok = tok_set1(STR_T, str, siz, pss, *cpos);

    return ++code;
}

cstr gen_etr(tok_tp tok, mem_t mem, cstr code, uint8 term, pos_tp cpos, uint8p flg)
{
    if (*(code + 1) != '"' && *(code + 1) != '\'')
    {
        *flg = 1;
        return code;
    }

    pos_t pss = *cpos;
    posp_adv(cpos);
    code++;

    uint8 quote = *code++;
    posp_adv(cpos);

    if (*code == quote)
    {
        posp_adv(cpos);

        *tok = tok_set2(STR_T, pss, *cpos);

        return ++code;
    }

    str str = blk_alloc(mem, GSTR_SIZ, MEM_SIZ);
    uint64 alc = GSTR_SIZ;
    uint64 siz = 0;

    while (*code != quote && *code != term)
    {
        if (siz == alc)
        {
            blk_add(mem, GSTR_SIZ, MEM_SIZ);
            alc += GSTR_SIZ;
        }

        if (*code == '\n')
            posp_jmp(cpos);
        else
            posp_adv(cpos);

        if (*code == '\\')
        {
            str[siz++] = *code++;

            if (siz == alc)
            {
                blk_add(mem, GSTR_SIZ, MEM_SIZ);
                alc += GSTR_SIZ;
            }

            str[siz++] = '\\';
            continue;
        }

        str[siz++] = *code++;
    }

    posp_adv(cpos);

    if (siz == alc)
    {
        blk_add(mem, 1, MEM_SIZ);

        str[siz++] = '\0';
    }
    else
    {
        str[siz++] = '\0';

        blk_sub(mem, str, siz);
    }

    *tok = tok_set1(STR_T, str, siz, pss, *cpos);

    return ++code;
}

cstr gen_double(tok_tp tok, cstr code, uint8 typ1, uint8 typ2, uint8 chr, pos_tp cpos)
{
    pos_t pss = *cpos;
    posp_adv(cpos);

    if (*++code == chr)
    {
        posp_adv(cpos);

        *tok = tok_set2(typ2, pss, *cpos);
        return ++code;
    }

    *tok = tok_set2(typ1, pss, *cpos);
    return code;
}

cstr gen_triple(tok_tp tok, cstr code, uint8 typ1, uint8 typ2, uint8 typ3, uint8 chr1, uint8 chr2, pos_tp cpos)
{
    pos_t pss = *cpos;
    posp_adv(cpos);

    if (*++code == chr1)
    {
        posp_adv(cpos);

        *tok = tok_set2(typ2, pss, *cpos);
        return ++code;
    }
    if (*code == chr2)
    {
        posp_adv(cpos);

        *tok = tok_set2(typ3, pss, *cpos);
        return ++code;
    }

    *tok = tok_set2(typ1, pss, *cpos);
    return code;
}

cstr gen_mul(tok_tp tok, cstr code, pos_tp cpos)
{
    pos_t pss = *cpos;
    posp_adv(cpos);

    if (*++code == '=')
    {
        posp_adv(cpos);

        *tok = tok_set2(MLE_T, pss, *cpos);
        return ++code;
    }
    if (*code == '*')
    {
        posp_adv(cpos);

        if (*++code == '=')
        {
            posp_adv(cpos);

            *tok = tok_set2(PWE_T, pss, *cpos);
            return ++code;
        }

        *tok = tok_set2(POW_T, pss, *cpos);
        return code;
    }

    *tok = tok_set2(MUL_T, pss, *cpos);
    return code;
}

cstr gen_div(tok_tp tok, cstr code, pos_tp cpos)
{
    pos_t pss = *cpos;
    posp_adv(cpos);

    if (*++code == '=')
    {
        posp_adv(cpos);

        *tok = tok_set2(DVE_T, pss, *cpos);
        return ++code;
    }
    if (*code == '/')
    {
        posp_adv(cpos);

        if (*++code == '=')
        {
            posp_adv(cpos);

            *tok = tok_set2(QTE_T, pss, *cpos);
            return ++code;
        }

        *tok = tok_set2(QOT_T, pss, *cpos);
        return code;
    }

    *tok = tok_set2(DIV_T, pss, *cpos);
    return code;
}

cstr gen_lst(tok_tp tok, cstr code, pos_tp cpos)
{
    pos_t pss = *cpos;
    posp_adv(cpos);

    if (*++code == '=')
    {
        posp_adv(cpos);

        *tok = tok_set2(LTE_T, pss, *cpos);
        return ++code;
    }
    if (*code == '<')
    {
        posp_adv(cpos);

        if (*++code == '=')
        {
            posp_adv(cpos);

            *tok = tok_set2(LSE_T, pss, *cpos);
            return ++code;
        }

        *tok = tok_set2(LSH_T, pss, *cpos);
        return code;
    }

    *tok = tok_set2(LST_T, pss, *cpos);
    return code;
}

cstr gen_grt(tok_tp tok, cstr code, pos_tp cpos)
{
    pos_t pss = *cpos;
    posp_adv(cpos);

    if (*++code == '=')
    {
        posp_adv(cpos);

        *tok = tok_set2(GTE_T, pss, *cpos);
        return ++code;
    }
    if (*code == '>')
    {
        posp_adv(cpos);

        if (*++code == '=')
        {
            posp_adv(cpos);

            *tok = tok_set2(RSE_T, pss, *cpos);
            return ++code;
        }

        *tok = tok_set2(RSH_T, pss, *cpos);
        return code;
    }

    *tok = tok_set2(GRT_T, pss, *cpos);
    return code;
}
