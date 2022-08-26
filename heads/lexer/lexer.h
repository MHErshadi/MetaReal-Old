// MetaReal Programming Language version 1.0.0

#ifndef __LEXER__
#define __LEXER__

#include <lexer/tok.h>
#include <mem/mem.h>
#include <debug/errlib.h>

struct __lres__
{
    tok_tp _toks;

    uint8 _herr : 1;
    ill_chr_t _error;
};
typedef struct __lres__ lres_t;

lres_t lex(cstr code, uint8 term, cstr fn, mem_t mem);

#endif /* __LEXER__ */
