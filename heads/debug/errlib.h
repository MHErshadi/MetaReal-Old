// MetaReal Programming Language version 1.0.0

#ifndef __M_ERRLIB__
#define __M_ERRLIB__

#include <ir/context.h>

enum _runtime_error_type_
{
    ILLEGAL_OPERATION,
    DIVISION_BY_ZERO,
};

struct __illegal_char__
{
    uint8 _chr;
    pos_t _pos;
};
typedef struct __illegal_char__ illegal_char_t;

struct __invalid_syntax__
{
    cstr _detail;

    pos_t _poss;
    pos_t _pose;
};
typedef struct __invalid_syntax__ invalid_syntax_t;

struct __runtime__
{
    uint8 _type;
    str _detail;

    pos_t _poss;
    pos_t _pose;

    context_t _context;
};
typedef struct __runtime__ runtime_t;

illegal_char_t illegal_char_set(uint8 chr, pos_t pos);
void illegal_char_print(illegal_char_t error, cstr code, uint64 size);

invalid_syntax_t invalid_syntax_set(cstr detail, pos_t poss, pos_t pose);
void invalid_syntax_print(invalid_syntax_t error, cstr code, uint64 size);

runtime_t runtime_set(uint8 type, str detail, pos_t poss, pos_t pose, context_t context);
void runtime_print(runtime_t error, cstr code, uint64 size);

#endif /* __M_ERRLIB__ */
