// MetaReal Programming Language version 1.0.0

#ifndef __M_LEXER__
#define __M_LEXER__

#include <lexer/token.h>
#include <memory/memory.h>
#include <debug/errlib.h>

struct __lres__
{
    token_p _tokens;

    uint8 _has_error : 1;
    illegal_char_t _error;
};
typedef struct __lres__ lres_t;

lres_t lex(cstr code, uint8 terminator, cstr file_name, stack_t stack);

#endif /* __M_LEXER__ */
