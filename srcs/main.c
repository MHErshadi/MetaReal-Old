// MetaReal Programming Language version 1.0.0

#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <lexer/lexer.h>
#include <parser/parser.h>
#include <mrir/irgen.h>
#include <debug/crash.h>
#include <info.h>

#define code_init(c, s, mem, stk, ic) \
    do                                \
    {                                 \
        c = malloc(s);                \
                                      \
        if (!c)                       \
            mem_error(s);             \
                                      \
        mem_alloc(mem, MEM_SIZ);      \
        stk_alloc(stk, STK_SIZ);      \
                                      \
        ic = ictx_set2(ROOT_ICTX);    \
    } while (0)

#define code_free(c, mem, stk) \
    do                         \
    {                          \
        free(c);               \
                               \
        free(mem->_mem);       \
        stk_free(stk);         \
    } while (0)

void terminate(int sig);

uint8 main(uint8 argc, strp argv)
{
    signal(SIGINT, terminate);

    if (argc == 1)
    {
        fprintf(STDOUT, "MetaReal version %s\n", LANG_VER);
        fprintf(STDOUT, "MetaReal Core verified as %s version %s\n", DEF_CORE, CORE_VER);
        fprintf(STDOUT, "MetaReal Port verified as %s version %s\n\n", DEF_PORT, PORT_VER);

        str code;

        mem_t mem;
        stk_t stk;

        ictx_t ictx;

        code_init(code, CMD_MAX_INP, mem, stk, ictx);

        while (1)
        {
            fprintf(STDOUT, ">>> ");
            fgets(code, CMD_MAX_INP, STDIN);

            uint64 siz = strlen(code) - 1;
            while (code[siz - 1] == '\\')
            {
                code[siz - 1] = '\n';

                fprintf(STDOUT, "... ");
                fgets(code + siz, CMD_MAX_INP - siz, STDIN);

                siz = strlen(code) - 1;
            }

            for (; *code == ' ' || *code == '\t' || *code == '\n'; code++);
            if (!*code)
                continue;
            code[strlen(code) - 1] = '\0';

            lres_t lres = lex(code, '\0', STDIN_NAM, mem);

            if (lres._herr)
            {
                ill_chr_print(lres._error, code, siz);
                goto clear;
            }

            pres_t pres = parse(lres._toks, mem, stk);

            if (pres._herr)
            {
                inv_syn_print(pres._error, code, siz);
                goto clear;
            }

            igres_t igres = irgen(pres._nodes, mem, ictx);

            if (igres._herr)
            {
                run_tim_print(igres._error, code, siz);
                goto clear;
            }

            putc('\n', STDOUT);
            mrir_print(&igres._data, igres._ir);
            putc('\n', STDOUT);
            mrir_free(igres._ir);

clear:
            mem_clear(mem, MEM_SIZ);
            stk_clear(stk, STK_SIZ);
        }

        code_free(code, mem, stk);
        return 0;
    }

    if (argc == 2)
    {
        FILE *file;
        file = fopen(argv[1], "r");

        if (!file)
            fil_error(argv[1]);

        str code;

        mem_t mem;
        stk_t stk;

        ictx_t ictx;

        code_init(code, FIL_MAX_INP, mem, stk, ictx);

        uint64 siz = 0;
        while (fgets(code + siz, FIL_MAX_INP - siz, file))
            siz = strlen(code);

        for (; *code == ' ' || *code == '\t' || *code == '\n'; code++);
        if (!*code)
            goto free;
        code[strlen(code) - 1] = '\0';

        lres_t lres = lex(code, '\0', STDIN_NAM, mem);

        if (lres._herr)
        {
            ill_chr_print(lres._error, code, siz);
            goto free;
        }

        pres_t pres = parse(lres._toks, mem, stk);

        if (pres._herr)
        {
            inv_syn_print(pres._error, code, siz);
            goto free;
        }

        igres_t igres = irgen(pres._nodes, mem, ictx);

        if (igres._herr)
        {
            run_tim_print(igres._error, code, siz);
            goto free;
        }

        mrir_print(&igres._data, igres._ir);
        mrir_free(igres._ir);

free:
        fclose(file);

        code_free(code, mem, stk);
        return 0;
    }

    return 0;
}

void terminate(int sig)
{
    fputs("\nterminating operation", STDOUT);
    exit(1);
}
