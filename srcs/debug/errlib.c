// MetaReal Programming Language version 1.0.0

#include <debug/errlib.h>
#include <stdlib.h>
#include <string.h>
#include <crash.h>
#include <def.h>

#define num_count(n, s) \
    do                  \
    {                   \
        uint64 cn = n;  \
        do              \
        {               \
            cn /= 10;   \
            s++;        \
        } while (cn);   \
    } while (0)

illegal_char_t illegal_char_set(uint8 chr, pos_t pos)
{
    illegal_char_t error;

    error._chr = chr;
    error._pos = pos;

    return error;
}

void illegal_char_print(illegal_char_t error, cstr code, uint64 size)
{
    fprintf(STDERR, "\nIllegal Character Error: '%c'\n", error._chr);
    fprintf(STDERR, "File \"%s\", line %llu\n\n", error._pos._file_name, error._pos._line + 1);

    uint64 start;
    for (start = error._pos._index; code[start] != '\n' && start > 0; start--);
    if (start)
        start++;

    uint64 end;
    for (end = error._pos._index; code[end] != '\n' && end < size; end++);

    uint64 i;
    for (i = start; i < end; i++)
        putc(code[i], STDERR);
    putc('\n', STDERR);

    for (i = start; i < error._pos._index; i++)
        putc(' ', STDERR);
    fprintf(STDERR, "^\n\n");
}

invalid_syntax_t invalid_syntax_set(cstr detail, pos_t poss, pos_t pose)
{
    invalid_syntax_t error;

    error._detail = detail;
    error._poss = poss;
    error._pose = pose;

    return error;
}

void invalid_syntax_print(invalid_syntax_t error, cstr code, uint64 size)
{
    if (error._detail)
        fprintf(STDERR, "\nInvalid Syntax Error: %s\n", error._detail);
    else
        fputs("\nInvalid Syntax Error\n", STDERR);
    fprintf(STDERR, "File \"%s\", line %llu\n\n", error._poss._file_name, error._poss._line + 1);

    if (error._poss._line != error._pose._line)
    {
        uint64 start;
        for (start = error._poss._index; code[start] != '\n' && start > 0; start--);
        if (start)
            start++;

        uint64 end;
        for (end = error._poss._index; code[end] != '\n' && end < size; end++);

        uint64 i;
        for (i = start; i < end; i++)
            putc(code[i], STDERR);
        putc('\n', STDERR);

        for (i = start; i < error._poss._index; i++)
            putc(' ', STDERR);
        for (; i < end; i++)
            putc('^', STDERR);
        fprintf(STDERR, "~\n\n");

        return;
    }

    uint64 start;
    for (start = error._poss._index; code[start] != '\n' && start > 0; start--);
    if (start)
        start++;

    uint64 end;
    for (end = error._pose._index; code[end] != '\n' && end < size; end++);

    uint64 i;
    for (i = start; i < end; i++)
        putc(code[i], STDERR);
    putc('\n', STDERR);

    for (i = start; i < error._poss._index; i++)
        putc(' ', STDERR);
    for (; i < error._pose._index; i++)
        putc('^', STDERR);
    fprintf(STDERR, "\n\n");
}

runtime_t runtime_set(uint8 type, str detail, pos_t poss, pos_t pose, context_t context)
{
    runtime_t error;

    error._type = type;
    error._detail = detail;
    error._poss = poss;
    error._pose = pose;
    error._context = context;

    return error;
}

static cstr runtime_error_types_label[2] =
{
    "IllegalOperation",
    "DivisionByZero",
};

void runtime_print(runtime_t error, cstr code, uint64 size)
{
    fprintf(STDERR, "\nRuntime Error: %s\n", error._detail);
    free(error._detail);

    fprintf(STDERR, "Error Type: %s\n", runtime_error_types_label[error._type]);

    context_p context = &error._context;
    pos_t pos = error._poss;

    uint64 len = strlen(pos._file_name) + strlen(context->_name) + 23;
    num_count(pos._line + 1, len);

    str troubleshoot = malloc(len);

    snprintf(troubleshoot, len, "  File \"%s\", line %llu, in %s\n", pos._file_name, pos._line + 1, context->_name);

    while (context->_parent)
    {
        pos = context->_parent_pos;
        context = context->_parent;

        len += strlen(pos._file_name) + strlen(context->_name) + 23;
        num_count(pos._line + 1, len);

        str trace = malloc(len);

        snprintf(trace, len, "  File \"%s\", line %llu, in %s\n%s", pos._file_name, pos._line + 1, context->_name, troubleshoot);

        free(troubleshoot);
        troubleshoot = trace;
    }

    fprintf(STDERR, "\nTroubleshoot (most recent call last):\n%s", troubleshoot);
    free(troubleshoot);

    if (error._poss._line != error._pose._line)
    {
        uint64 start;
        for (start = error._poss._index; code[start] != '\n' && start > 0; start--);
        if (start)
            start++;

        uint64 end;
        for (end = error._poss._index; code[end] != '\n' && end < size; end++);

        uint64 i;
        for (i = start; i < end; i++)
            putc(code[i], STDERR);
        putc('\n', STDERR);

        for (i = start; i < error._poss._index; i++)
            putc(' ', STDERR);
        for (; i < end; i++)
            putc('^', STDERR);
        fprintf(STDERR, "~\n\n");

        return;
    }

    uint64 start;
    for (start = error._poss._index; code[start] != '\n' && start > 0; start--);
    if (start)
        start++;

    uint64 end;
    for (end = error._pose._index; code[end] != '\n' && end < size; end++);

    uint64 i;
    for (i = start; i < end; i++)
        putc(code[i], STDERR);
    putc('\n', STDERR);

    for (i = start; i < error._poss._index; i++)
        putc(' ', STDERR);
    for (; i < error._pose._index; i++)
        putc('^', STDERR);
    fprintf(STDERR, "\n\n");
}
