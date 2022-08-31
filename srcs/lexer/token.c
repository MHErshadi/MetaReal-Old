// MetaReal Programming Language version 1.0.0

#include <string.h>

#include <lexer/token.h>

token_t token_set1(uint8 type, str value, uint64 len, pos_t poss, pos_t pose)
{
    token_t token;

    token._type = type;
    token._value = value;
    token._len = len;
    token._poss = poss;
    token._pose = pose;

    return token;
}

token_t token_set2(uint8 type, pos_t poss, pos_t pose)
{
    token_t token;

    token._type = type;
    token._value = NULL;
    token._poss = poss;
    token._pose = pose;

    return token;
}

uint8 is_keyword(cstr str)
{
    uint8 i;
    for (i = 0; i < KEYWORD_COUNT; i++)
        if (!strcmp(str, keywords[i]))
            return i + KEYWORD_PAD;
    return 255;
}
