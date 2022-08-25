// MetaReal Programming Language version 1.0.0

#ifndef __TOK__
#define __TOK__

#include <debug/pos.h>
#include <def.h>

enum _tok_
{
    EOF_T, // terminator
    NLN_T, // new line

    /* */

    IDN_T, // identifier

    /* */

    INT_T, // integer
    FLT_T, // float
    STR_T, // string

    /* */

    ADD_T, // +
    SUB_T, // -
    MUL_T, // *
    DIV_T, // /
    MOD_T, // %
    QOT_T, // //
    POW_T, // **

    BAN_T, // &
    BIO_T, // |
    BXO_T, // ^

    BNT_T, // ~

    LSH_T, // <<
    RSH_T, // >>

    /* */

    EQU_T, // ==
    NEQ_T, // !=
    LST_T, // <
    GRT_T, // >
    LTE_T, // <=
    GTE_T, // >=

    AND_T, // &&
    IOR_T, // ||
    XOR_T, // ^^

    NOT_T, // !

    /* */

    ASN_T, // =

    INC_T, // ++
    DEC_T, // --

    ADE_T, // +=
    SBE_T, // -=
    MLE_T, // *=
    DVE_T, // /=
    MDE_T, // %=
    QTE_T, // //=
    PWE_T, // **=

    BAE_T, // &=
    BIE_T, // |=
    BXE_T, // ^=

    LSE_T, // <<=
    RSE_T, // >>=

    /* */

    LPR_T, // (
    RPR_T, // )

    LCB_T, // {
    RCB_T, // }

    LSB_T, // [
    RSB_T, // ]

    /* */

    DOT_T, // .
    COL_T, // :

    QUE_T, // ?
    DLR_T, // $
    CMA_T, // ,
    SEM_T, // ;
};

enum _kwd_
{
    VAR_K = KWD_PAD, // var
    FUN_K, // func
    CLS_K, // class
    STT_K, // struct

    PBL_K, // public
    PRV_K, // private
    
    GLB_K, // global
    LCL_K, // local

    CST_K, // const
    STC_K, // static

    /* */

    TRU_K, // true
    FLS_K, // false

    /* */

    IFF_K, // if
    ELF_K, // elif
    ELS_K, // else

    SWH_K, // switch
    CAS_K, // case
    DEF_K, // default

    /* */

    FOR_K, // for
    TOO_K, // to
    STP_K, // step

    WHL_K, // while
    DOO_K, // do

    LOP_K, // loop

    /* */

    TRY_K, // try
    EXP_K, // except
    FNL_K, // finally

    /* */

    RET_K, // return

    CNT_K, // continue
    BRK_K, // break

    PSS_K, // pass

    /* */

    IMP_K, // import

    /* */

    AND_K, // and
    IOR_K, // or
    XOR_K, // xor

    NOT_K, // not

    /* */

    INN_K, // in

    ISS_K, // is
    ARE_K, // are
};

static cstr kwd[KWD_CNT] =
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

struct __tok__
{
    uint8 _typ;

    str _val;
    uint64 _len;

    pos_t _pss;
    pos_t _pse;
};
typedef struct __tok__ tok_t;
typedef struct __tok__ *tok_tp;

tok_t tok_set1(uint8 typ, str val, uint64 len, pos_t pss, pos_t pse);
tok_t tok_set2(uint8 typ, pos_t pss, pos_t pse);

uint8 is_kwd(cstr str);

#endif /* __TOK__ */
