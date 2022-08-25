// MetaReal Programming Language version 1.0.0

#include <string.h>

#include <lexer/tok.h>

tok_t tok_set1(uint8 typ, str val, uint64 len, pos_t pss, pos_t pse)
{
    tok_t tok;

    tok._typ = typ;
    tok._val = val;
    tok._len = len;
    tok._pss = pss;
    tok._pse = pse;

    return tok;
}

tok_t tok_set2(uint8 typ, pos_t pss, pos_t pse)
{
    tok_t tok;

    tok._typ = typ;
    tok._val = NULL;
    tok._pss = pss;
    tok._pse = pse;

    return tok;
}

uint8 is_kwd(cstr str)
{
    uint8 i;
    for (i = 0; i < KWD_CNT; i++)
        if (!strcmp(str, kwd[i]))
            return i + KWD_PAD;
    return 255;
}
