// MetaReal Programming Language version 1.0.0

#ifndef __M_TOKEN__
#define __M_TOKEN__

#include <debug/pos.h>
#include <def.h>

enum _token_
{
    EOF_T, // terminator
    NEWLINE_T, // new line

    /* */

    IDENTIFIER_T, // identifier

    /* */

    INT_T, // integer
    FLOAT_T, // float
    STR_T, // string

    /* */

    ADD_T, // +
    SUBTRACT_T, // -
    MULTIPLY_T, // *
    DIVIDE_T, // /
    MODULO_T, // %
    QUOTIENT_T, // //
    POWER_T, // **

    B_AND_T, // &
    B_IOR_T, // |
    B_XOR_T, // ^

    B_NOT_T, // ~

    LSHIFT_T, // <<
    RSHIFT_T, // >>

    /* */

    EQUAL_T, // ==
    NEQUAL_T, // !=
    LESS_T, // <
    GREATER_T, // >
    LESS_EQUAL_T, // <=
    GREATER_EQUAL_T, // >=

    AND_T, // &&
    IOR_T, // ||
    XOR_T, // ^^

    NOT_T, // !

    /* */

    ASSIGN_T, // =

    INCREMENT_T, // ++
    DECREMENT_T, // --

    ADD_EQ_T, // +=
    SUBTRACT_EQ_T, // -=
    MULTIPLY_EQ_T, // *=
    DIVIDE_EQ_T, // /=
    MODULO_EQ_T, // %=
    QUOTIENT_EQ_T, // //=
    POWER_EQ_T, // **=

    B_AND_EQ_T, // &=
    B_IOR_EQ_T, // |=
    B_XOR_EQ_T, // ^=

    LSHIFT_EQ_T, // <<=
    RSHIFT_EQ_T, // >>=

    /* */

    LPAREN_T, // (
    RPAREN_T, // )

    LCURLY_BRACE_T, // {
    RCURLY_BRACE_T, // }

    LSQUARE_BRACE_T, // [
    RSQUARE_BRACE_T, // ]

    /* */

    DOT_T, // .
    COLON_T, // :

    QUESTION_T, // ?
    DOLLAR_T, // $
    COMMA_T, // ,
    SEMICOLON_T, // ;
};

enum _keyword_
{
    VAR_K = KEYWORD_PAD, // var
    FUNC_K, // func
    CLASS_K, // class
    STRUCT_K, // struct

    PUBLIC_K, // public
    PRIVATE_K, // private

    GLOBAL_K, // global
    LOCAL_K, // local

    CONST_K, // const
    STATIC_K, // static

    /* */

    TRUE_K, // true
    FALSE_K, // false

    /* */

    IF_K, // if
    ELIF_K, // elif
    ELSE_K, // else

    SWITCH_K, // switch
    CASE_K, // case
    DEFAULT_K, // default

    /* */

    FOR_K, // for
    TO_K, // to
    STEP_K, // step

    WHILE_K, // while
    DO_K, // do

    LOOP_K, // loop

    /* */

    TRY_K, // try
    EXCEPT_K, // except
    FINALLY_K, // finally

    /* */

    RETURN_K, // return

    CONTINUE_K, // continue
    BREAK_K, // break

    PASS_K, // pass

    /* */

    IMPORT_K, // import

    /* */

    AND_K, // and
    IOR_K, // or
    XOR_K, // xor

    NOT_K, // not

    /* */

    IN_K, // in

    IS_K, // is
    ARE_K, // are
};

static cstr keywords[KEYWORD_COUNT] =
{
    "var", "func", "class", "struct",
    "public", "private",
    "global", "local",
    "const", "static",

    "true", "false",

    "if", "elif", "else",
    "switch", "case", "default",

    "for", "to", "step",
    "while", "do",
    "loop",

    "try", "except", "finally",

    "return",
    "continue", "break",
    "pass",

    "import",

    "and", "or", "xor",
    "not",

    "in",
    "is", "are",
};

struct __token__
{
    uint8 _type;

    str _value;
    uint64 _len;

    pos_t _poss;
    pos_t _pose;
};
typedef struct __token__ token_t;
typedef struct __token__ *token_p;

token_t token_set1(uint8 type, str value, uint64 len, pos_t poss, pos_t pose);
token_t token_set2(uint8 type, pos_t poss, pos_t pose);

uint8 is_keyword(cstr str);

#endif /* __M_TOKEN__ */
