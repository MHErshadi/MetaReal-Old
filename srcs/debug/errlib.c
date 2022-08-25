// MetaReal Programming Language version 1.0.0

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <debug/errlib.h>
#include <debug/crash.h>
#include <def.h>

#define num_cnt(n, s)  \
    do                 \
    {                  \
        uint64 cn = n; \
        do             \
        {              \
            cn /= 10;  \
            s++;       \
        } while (cn);  \
    } while (0)

ill_chr_t ill_chr_set(uint8 chr, pos_t pos)
{
    ill_chr_t error;

    error._chr = chr;
    error._pos = pos;

    return error;
}

void ill_chr_print(ill_chr_t error, cstr code, uint64 siz)
{
    fprintf(STDERR, "\nIllegal Character Error: '%c'\n", error._chr);
    fprintf(STDERR, "File \"%s\", line %llu\n\n", error._pos._fn, error._pos._ln + 1);

    uint64 s;
    for (s = error._pos._idx; code[s] != '\n' && s > 0; s--);
    if (s)
        s++;

    uint64 e;
    for (e = error._pos._idx; code[e] != '\n' && e < siz; e++);

    uint64 i;
    for (i = s; i < e; i++)
        putc(code[i], STDERR);
    putc('\n', STDERR);

    for (i = s; i < error._pos._idx; i++)
        putc(' ', STDERR);
    fprintf(STDERR, "^\n\n");
}

inv_syn_t inv_syn_set(cstr det, pos_t pss, pos_t pse)
{
    inv_syn_t error;

    error._det = det;
    error._pss = pss;
    error._pse = pse;

    return error;
}

void inv_syn_print(inv_syn_t error, cstr code, uint64 siz)
{
    if (error._det)
        fprintf(STDERR, "\nInvalid Syntax Error: %s\n", error._det);
    else
        fputs("\nInvalid Syntax Error\n", STDERR);
    fprintf(STDERR, "File \"%s\", line %llu\n\n", error._pss._fn, error._pss._ln + 1);

    if (error._pss._ln != error._pse._ln)
    {
        uint64 s;
        for (s = error._pss._idx; code[s] != '\n' && s > 0; s--);
        if (s)
            s++;

        uint64 e;
        for (e = error._pss._idx; code[e] != '\n' && e < siz; e++);

        uint64 i;
        for (i = s; i < e; i++)
            putc(code[i], STDERR);
        putc('\n', STDERR);

        for (i = s; i < error._pss._idx; i++)
            putc(' ', STDERR);
        for (; i < e; i++)
            putc('^', STDERR);
        fprintf(STDERR, "~\n\n");

        return;
    }

    uint64 s;
    for (s = error._pss._idx; code[s] != '\n' && s > 0; s--);
    if (s)
        s++;

    uint64 e;
    for (e = error._pse._idx; code[e] != '\n' && e < siz; e++);

    uint64 i;
    for (i = s; i < e; i++)
        putc(code[i], STDERR);
    putc('\n', STDERR);

    for (i = s; i < error._pss._idx; i++)
        putc(' ', STDERR);
    for (; i < error._pse._idx; i++)
        putc('^', STDERR);
    fprintf(STDERR, "\n\n");
}

run_tim_t run_tim_set(uint8 typ, str det, pos_t pss, pos_t pse, ictx_t ictx)
{
    run_tim_t error;

    error._typ = typ;
    error._det = det;
    error._pss = pss;
    error._pse = pse;
    error._ictx = ictx;

    return error;
}

static cstr rtyp[1] =
{
    "DivByZero",
};

void run_tim_print(run_tim_t error, cstr code, uint64 siz)
{
    fprintf(STDERR, "\nRuntime Error: %s\n", error._det);
    free(error._det);

    fprintf(STDERR, "Error Type: %s\n", rtyp[error._typ]);

    ictx_tp ictx = &error._ictx;
    pos_t pos = error._pss;

    uint64 len = strlen(pos._fn) + strlen(ictx->_name) + 23;
    num_cnt(pos._ln + 1, len);

    str trcb = malloc(len);

    snprintf(trcb, len, "  File \"%s\", line %llu, in %s\n", pos._fn, pos._ln + 1, ictx->_name);

    while (ictx->_prn)
    {
        pos = ictx->_prn_pos;
        ictx = ictx->_prn;

        len += strlen(pos._fn) + strlen(ictx->_name) + 23;
        num_cnt(pos._ln + 1, len);

        str trcn = malloc(len);

        snprintf(trcn, len, "  File \"%s\", line %llu, in %s\n%s", pos._fn, pos._ln + 1, ictx->_name, trcb);

        free(trcb);
        trcb = trcn;
    }

    fprintf(STDERR, "\nTroubleshoot (most recent call last):\n%s", trcb);
    free(trcb);

    if (error._pss._ln != error._pse._ln)
    {
        uint64 s;
        for (s = error._pss._idx; code[s] != '\n' && s > 0; s--);
        if (s)
            s++;

        uint64 e;
        for (e = error._pss._idx; code[e] != '\n' && e < siz; e++);

        uint64 i;
        for (i = s; i < e; i++)
            putc(code[i], STDERR);
        putc('\n', STDERR);

        for (i = s; i < error._pss._idx; i++)
            putc(' ', STDERR);
        for (; i < e; i++)
            putc('^', STDERR);
        fprintf(STDERR, "~\n\n");

        return;
    }

    uint64 s;
    for (s = error._pss._idx; code[s] != '\n' && s > 0; s--);
    if (s)
        s++;

    uint64 e;
    for (e = error._pse._idx; code[e] != '\n' && e < siz; e++);

    uint64 i;
    for (i = s; i < e; i++)
        putc(code[i], STDERR);
    putc('\n', STDERR);

    for (i = s; i < error._pss._idx; i++)
        putc(' ', STDERR);
    for (; i < error._pse._idx; i++)
        putc('^', STDERR);
    fprintf(STDERR, "\n\n");
}
