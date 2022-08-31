// MetaReal Programming Language version 1.0.0

#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <lexer/lexer.h>
#include <parser/parser.h>
#include <ir/gen.h>
#include <crash.h>
#include <info.h>

void terminate(int sig);

uint8 main(uint8 argc, strp argv)
{
    signal(SIGINT, terminate);

    if (argc == 1)
    {
        fprintf(STDOUT, "MetaReal version %s\n", LANG_VER);
        fprintf(STDOUT, "MetaReal Core verified as %s version %s\n", DEF_CORE, CORE_VER);
        fprintf(STDOUT, "MetaReal Port verified as %s version %s\n\n", DEF_PORT, PORT_VER);

        str code = malloc(CMD_MAX_INPUT);
        if (!code)
            alloc_error(CMD_MAX_INPUT);

        stack_t stack;
        alloc_stack(stack, STACK_SIZE);

        heap_t heap;
        alloc_heap(heap, HEAP_SIZE);

        context_t context;
        context_set2(ROOT_CONTEXT);

        while (1)
        {
            fprintf(STDOUT, ">>> ");
            fgets(code, CMD_MAX_INPUT, STDIN);

            uint64 size = strlen(code) - 1;
            while (code[size - 1] == '\\')
            {
                code[size - 1] = '\n';

                fprintf(STDOUT, "... ");
                fgets(code + size, CMD_MAX_INPUT - size, STDIN);

                size = strlen(code) - 1;
            }

            for (; *code == ' ' || *code == '\t' || *code == '\n'; code++);
            if (!*code)
                continue;
            code[strlen(code) - 1] = '\0';

            lres_t lres = lex(code, '\0', STDIN_NAME, stack);

            if (lres._has_error)
            {
                illegal_char_print(lres._error, code, size);
                goto clear;
            }

            pres_t pres = parse(lres._tokens, stack, heap);

            if (pres._has_error)
            {
                invalid_syntax_print(pres._error, code, size);
                goto clear;
            }

            gres_t gres = gen(pres._nodes, stack, heap, context);

            if (gres._has_error)
            {
                runtime_print(gres._error, code, size);
                goto clear;
            }

            putc('\n', STDOUT);
            ir_print(&gres._data, gres._ir);
            putc('\n', STDOUT);

            ir_free(gres._ir);

clear:
            clear_stack(stack, STACK_SIZE);
            clear_heap(heap, HEAP_SIZE);
        }

        free_stack(stack);
        free_heap(heap);
        return 0;
    }

    if (argc == 2)
    {
        FILE *file;
        file = fopen(argv[1], "r");

        if (!file)
            file_open_error(argv[1]);

        str code = malloc(FILE_MAX_INPUT);
        if (!code)
            alloc_error(FILE_MAX_INPUT);

        stack_t stack;
        alloc_stack(stack, STACK_SIZE);

        heap_t heap;
        alloc_heap(heap, HEAP_SIZE);

        context_t context = context_set2(ROOT_CONTEXT);

        uint64 size = 0;
        while (fgets(code + size, FILE_MAX_INPUT - size, file))
            size = strlen(code);

        for (; *code == ' ' || *code == '\t' || *code == '\n'; code++);
        if (!*code)
            goto free;
        code[strlen(code) - 1] = '\0';

        lres_t lres = lex(code, '\0', STDIN_NAME, stack);

        if (lres._has_error)
        {
            illegal_char_print(lres._error, code, size);
            goto free;
        }

        pres_t pres = parse(lres._tokens, stack, heap);

        if (pres._has_error)
        {
            invalid_syntax_print(pres._error, code, size);
            goto free;
        }

        gres_t gres = gen(pres._nodes, stack, heap, context);

        if (gres._has_error)
        {
            runtime_print(gres._error, code, size);
            goto free;
        }

        ir_print(&gres._data, gres._ir);
        ir_free(gres._ir);

free:
        fclose(file);

        free(code);

        free_stack(stack);
        free_heap(heap);
        return 0;
    }

    return 0;
}

void terminate(int sig)
{
    fputs("\nterminating operation", STDOUT);
    exit(1);
}
