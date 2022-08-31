// MetaReal Programming Language version 1.0.0

#include <lexer/lexer.h>
#include <stdlib.h>
#include <crash.h>

#define token_set(t)                                \
    do                                              \
    {                                               \
        poss = cpos;                                \
                                                    \
        cpos._index++;                              \
        code++;                                     \
                                                    \
        tokens[size++] = token_set2(t, poss, cpos); \
    } while (0)

lres_t lres_success(token_p tokens);
lres_t lres_fail(illegal_char_t error);

cstr comment(cstr code, uint8 terminator, pos_p cpos);

cstr gen_identifier(token_p token, stack_t stack, cstr code, pos_p cpos);

cstr gen_number(token_p token, stack_t stack, cstr code, pos_p cpos);

cstr gen_str(token_p token, stack_t stack, cstr code, uint8 terminator, pos_p cpos);
cstr gen_escape_str(token_p token, stack_t stack, cstr code, uint8 terminator, pos_p cpos, uint8p flag);

cstr gen_double(token_p token, cstr code, uint8 type1, uint8 type2, uint8 chr, pos_p cpos);
cstr gen_triple(token_p token, cstr code, uint8 type1, uint8 type2, uint8 type3, uint8 chr1, uint8 chr2, pos_p cpos);

cstr gen_multiply(token_p token, cstr code, pos_p cpos);
cstr gen_divide(token_p token, cstr code, pos_p cpos);
cstr gen_less(token_p token, cstr code, pos_p cpos);
cstr gen_greater(token_p token, cstr code, pos_p cpos);

lres_t lex(cstr code, uint8 terminator, cstr file_name, stack_t stack)
{
    token_p tokens = malloc(TOKENS_SIZE * sizeof(token_t));
    if (!tokens)
        alloc_error(TOKENS_SIZE * sizeof(token_t));

    uint64 alloc = TOKENS_SIZE;
    uint64 size = 0;

    pos_t poss;
    pos_t cpos = pos_set(0, 0, file_name);

    while (*code != terminator)
    {
        if (size == alloc)
        {
            tokens = realloc(tokens, (alloc += TOKENS_SIZE) * sizeof(token_t));
            if (!tokens)
                alloc_error(alloc * sizeof(token_t));
        }

        if (*code == ' ' || *code == '\t')
        {
            cpos._index++;
            code++;
            continue;
        }

        if (*code == '#')
        {
            code = comment(code, terminator, &cpos);
            continue;
        }

        if (*code == '\n')
        {
            poss = cpos;

            pos_jump(cpos);
            code++;

            if (tokens[size - 1]._type == NEWLINE_T)
                continue;

            tokens[size++] = token_set2(NEWLINE_T, poss, cpos);
            continue;
        }
        if (*code == ';')
        {
            poss = cpos;

            cpos._index++;
            code++;

            tokens[size++] = token_set2(SEMICOLON_T, poss, cpos);
            continue;
        }

        if (*code == '\\')
        {
            uint8 flag = 0;
            code = gen_escape_str(&tokens[size++], stack, code, terminator, &cpos, &flag);

            if (flag)
            {
                free(tokens);
                return lres_fail(illegal_char_set(*code, cpos));
            }
            continue;
        }

        if ((*code >= 'a' && *code <= 'z') || (*code >= 'A' && *code <= 'Z') || *code == '_')
        {
            code = gen_identifier(&tokens[size++], stack, code, &cpos);
            continue;
        }

        if (*code >= '0' && *code <= '9')
        {
            code = gen_number(&tokens[size++], stack, code, &cpos);
            continue;
        }

        switch (*code)
        {
        case '"':
        case '\'':
            code = gen_str(&tokens[size++], stack, code, terminator, &cpos);
            break;

        case '+':
            code = gen_triple(&tokens[size++], code, ADD_T, ADD_EQ_T, INCREMENT_T, '=', '+', &cpos);
            break;
        case '-':
            code = gen_triple(&tokens[size++], code, SUBTRACT_T, SUBTRACT_EQ_T, DECREMENT_T, '=', '-', &cpos);
            break;
        case '*':
            code = gen_multiply(&tokens[size++], code, &cpos);
            break;
        case '/':
            code = gen_divide(&tokens[size++], code, &cpos);
            break;
        case '%':
            code = gen_double(&tokens[size++], code, MODULO_T, MODULO_EQ_T, '=', &cpos);
            break;

        case '&':
            code = gen_triple(&tokens[size++], code, B_AND_T, AND_T, B_AND_EQ_T, '&', '=', &cpos);
            break;
        case '|':
            code = gen_triple(&tokens[size++], code, B_IOR_T, IOR_T, B_IOR_EQ_T, '|', '=', &cpos);
            break;
        case '^':
            code = gen_triple(&tokens[size++], code, B_XOR_T, XOR_T, B_XOR_EQ_T, '^', '=', &cpos);
            break;

        case '~':
            token_set(B_NOT_T);
            break;

        case '<':
            code = gen_less(&tokens[size++], code, &cpos);
            break;
        case '>':
            code = gen_greater(&tokens[size++], code, &cpos);
            break;
        case '=':
            code = gen_double(&tokens[size++], code, ASSIGN_T, EQUAL_T, '=', &cpos);
            break;

        case '!':
            code = gen_double(&tokens[size++], code, NOT_T, NEQUAL_T, '=', &cpos);
            break;

        case '(':
            token_set(LPAREN_T);
            break;
        case ')':
            token_set(RPAREN_T);
            break;

        case '{':
            token_set(LCURLY_BRACE_T);
            break;
        case '}':
            token_set(RCURLY_BRACE_T);
            break;

        case '[':
            token_set(LSQUARE_BRACE_T);
            break;
        case ']':
            token_set(RSQUARE_BRACE_T);
            break;

        case '.':
            token_set(DOT_T);
            break;
        case ':':
            token_set(COLON_T);
            break;

        case '?':
            token_set(QUESTION_T);
            break;
        case '$':
            token_set(DOLLAR_T);
            break;
        case ',':
            token_set(COMMA_T);
            break;
        default:
            free(tokens);
            return lres_fail(illegal_char_set(*code, cpos));
        }
    }

    if (alloc != size + 1)
    {
        tokens = realloc(tokens, (size + 1) * sizeof(token_t));
        if (!tokens)
            alloc_error((size + 1) * sizeof(token_t));
    }

    poss = cpos;
    cpos._index++;

    tokens[size] = token_set2(EOF_T, poss, cpos);

    return lres_success(tokens);
}

lres_t lres_success(token_p tokens)
{
    lres_t res;

    res._tokens = tokens;
    res._has_error = 0;

    return res;
}

lres_t lres_fail(illegal_char_t error)
{
    lres_t res;

    res._error = error;
    res._has_error = 1;

    return res;
}

cstr comment(cstr code, uint8 terminator, pos_p cpos)
{
    cpos->_index++;
    code++;

    if (*code == '*')
    {
        cpos->_index++;
        code++;

        while (*code != terminator)
        {
            if (*code == '*')
            {
                cpos->_index++;
                code++;

                if (*code == '#')
                {
                    cpos->_index++;
                    return ++code;
                }
            }

            if (*code++ == '\n')
                posp_jump(cpos);
            else
                cpos->_index++;
        }

        return code;
    }

    while (*code != terminator)
    {
        if (*code++ == '\n')
        {
            posp_jump(cpos);
            return code;
        }

        cpos->_index++;
    }

    return code;
}

cstr gen_identifier(token_p token, stack_t stack, cstr code, pos_p cpos)
{
    pos_t poss = *cpos;

    str identifier = stack_alloc(stack, IDENTIFIER_SIZE);

    uint64 alloc = IDENTIFIER_SIZE;
    uint64 size = 0;

    do
    {
        if (size == alloc)
        {
            stack_increase(stack, IDENTIFIER_SIZE);
            alloc += IDENTIFIER_SIZE;
        }

        identifier[size++] = *code++;
        cpos->_index++;
    } while ((*code >= 'a' && *code <= 'z') || (*code >= 'A' && *code <= 'Z') || *code == '_');

    if (size == alloc)
        stack_increase(stack, 1);

    identifier[size++] = '\0';

    uint8 keyword = is_keyword(identifier);
    if (keyword == 255)
    {
        *token = token_set1(IDENTIFIER_T, identifier, 0, poss, *cpos);

        if (size != alloc)
            stack_shrink(stack, identifier, size);
    }
    else
    {
        *token = token_set2(keyword, poss, *cpos);

        stack_free(stack, identifier);
    }

    return code;
}

cstr gen_number(token_p token, stack_t stack, cstr code, pos_p cpos)
{
    pos_t poss = *cpos;

    str number = stack_alloc(stack, NUMBER_SIZE);

    uint64 alloc = NUMBER_SIZE;
    uint64 size = 0;

    uint8 is_float = 0;
    do
    {
        if (size == alloc)
        {
            stack_increase(stack, NUMBER_SIZE);
            alloc += NUMBER_SIZE;
        }

        if (*code == '.')
        {
            if (is_float)
                break;
            is_float = 1;
        }

        number[size++] = *code++;
        cpos->_index++;
    } while ((*code >= '0' && *code <= '9') || *code == '.');

    if (size == alloc)
    {
        stack_increase(stack, 1);

        number[size++] = '\0';
    }
    else
    {
        number[size++] = '\0';

        if (size != alloc)
            stack_shrink(stack, number, size);
    }

    if (is_float)
        *token = token_set1(FLOAT_T, number, size, poss, *cpos);
    else
        *token = token_set1(INT_T, number, size, poss, *cpos);

    return code;
}

cstr gen_str(token_p token, stack_t stack, cstr code, uint8 terminator, pos_p cpos)
{
    pos_t poss = *cpos;

    uint8 quote = *code++;
    cpos->_index++;

    if (*code == quote)
    {
        cpos->_index++;

        *token = token_set2(STR_T, poss, *cpos);

        return ++code;
    }

    str str = stack_alloc(stack, STRING_SIZE);

    uint64 alloc = STRING_SIZE;
    uint64 size = 0;

    uint8 escape_chr = 0;
    while ((*code != quote || escape_chr) && *code != terminator)
    {
        if (size == alloc)
        {
            stack_increase(stack, STRING_SIZE);
            alloc += STRING_SIZE;
        }

        if (*code == '\\' && !escape_chr)
        {
            escape_chr = 1;

            cpos->_index++;

            str[size++] = *code++;
            continue;
        }

        escape_chr = 0;

        if (*code == '\n')
            posp_jump(cpos);
        else
            cpos->_index++;

        str[size++] = *code++;
    }

    cpos->_index++;

    if (size == alloc)
    {
        stack_increase(stack, 1);

        str[size++] = '\0';
    }
    else
    {
        str[size++] = '\0';

        if (size != alloc)
            stack_shrink(stack, str, size);
    }

    *token = token_set1(STR_T, str, size, poss, *cpos);

    return ++code;
}

cstr gen_escape_str(token_p token, stack_t stack, cstr code, uint8 terminator, pos_p cpos, uint8p flag)
{
    if (*(code + 1) != '"' && *(code + 1) != '\'')
    {
        *flag = 1;
        return code;
    }

    pos_t poss = *cpos;
    cpos->_index++;
    code++;

    uint8 quote = *code++;
    cpos->_index++;

    if (*code == quote)
    {
        cpos->_index++;

        *token = token_set2(STR_T, poss, *cpos);

        return ++code;
    }

    str str = stack_alloc(stack, STRING_SIZE);

    uint64 alloc = STRING_SIZE;
    uint64 size = 0;

    while (*code != quote && *code != terminator)
    {
        if (size == alloc)
        {
            stack_increase(stack, STRING_SIZE);
            alloc += STRING_SIZE;
        }

        if (*code == '\n')
            posp_jump(cpos);
        else
            cpos->_index++;

        if (*code == '\\')
        {
            str[size++] = *code++;

            if (size == alloc)
            {
                stack_increase(stack, STRING_SIZE);
                alloc += STRING_SIZE;
            }

            str[size++] = '\\';
            continue;
        }

        str[size++] = *code++;
    }

    cpos->_index++;

    if (size == alloc)
    {
        stack_increase(stack, 1);

        str[size++] = '\0';
    }
    else
    {
        str[size++] = '\0';

        if (size != alloc)
            stack_shrink(stack, str, size);
    }

    *token = token_set1(STR_T, str, size, poss, *cpos);

    return ++code;
}

cstr gen_double(token_p token, cstr code, uint8 type1, uint8 type2, uint8 chr, pos_p cpos)
{
    pos_t poss = *cpos;
    cpos->_index++;

    if (*++code == chr)
    {
        cpos->_index++;

        *token = token_set2(type2, poss, *cpos);
        return ++code;
    }

    *token = token_set2(type1, poss, *cpos);
    return code;
}

cstr gen_triple(token_p token, cstr code, uint8 type1, uint8 type2, uint8 type3, uint8 chr1, uint8 chr2, pos_p cpos)
{
    pos_t poss = *cpos;
    cpos->_index++;

    if (*++code == chr1)
    {
        cpos->_index++;

        *token = token_set2(type2, poss, *cpos);
        return ++code;
    }
    if (*code == chr2)
    {
        cpos->_index++;

        *token = token_set2(type3, poss, *cpos);
        return ++code;
    }

    *token = token_set2(type1, poss, *cpos);
    return code;
}

cstr gen_multiply(token_p token, cstr code, pos_p cpos)
{
    pos_t poss = *cpos;
    cpos->_index++;

    if (*++code == '=')
    {
        cpos->_index++;

        *token = token_set2(MULTIPLY_EQ_T, poss, *cpos);
        return ++code;
    }
    if (*code == '*')
    {
        cpos->_index++;

        if (*++code == '=')
        {
            cpos->_index++;

            *token = token_set2(POWER_EQ_T, poss, *cpos);
            return ++code;
        }

        *token = token_set2(POWER_T, poss, *cpos);
        return code;
    }

    *token = token_set2(MULTIPLY_T, poss, *cpos);
    return code;
}

cstr gen_divide(token_p token, cstr code, pos_p cpos)
{
    pos_t poss = *cpos;
    cpos->_index++;

    if (*++code == '=')
    {
        cpos->_index++;

        *token = token_set2(DIVIDE_EQ_T, poss, *cpos);
        return ++code;
    }
    if (*code == '/')
    {
        cpos->_index++;

        if (*++code == '=')
        {
            cpos->_index++;

            *token = token_set2(QUOTIENT_EQ_T, poss, *cpos);
            return ++code;
        }

        *token = token_set2(QUOTIENT_T, poss, *cpos);
        return code;
    }

    *token = token_set2(DIVIDE_T, poss, *cpos);
    return code;
}

cstr gen_less(token_p token, cstr code, pos_p cpos)
{
    pos_t poss = *cpos;
    cpos->_index++;

    if (*++code == '=')
    {
        cpos->_index++;

        *token = token_set2(LESS_EQUAL_T, poss, *cpos);
        return ++code;
    }
    if (*code == '<')
    {
        cpos->_index++;

        if (*++code == '=')
        {
            cpos->_index++;

            *token = token_set2(LSHIFT_EQ_T, poss, *cpos);
            return ++code;
        }

        *token = token_set2(LSHIFT_T, poss, *cpos);
        return code;
    }

    *token = token_set2(LESS_T, poss, *cpos);
    return code;
}

cstr gen_greater(token_p token, cstr code, pos_p cpos)
{
    pos_t poss = *cpos;
    cpos->_index++;

    if (*++code == '=')
    {
        cpos->_index++;

        *token = token_set2(GREATER_EQUAL_T, poss, *cpos);
        return ++code;
    }
    if (*code == '>')
    {
        cpos->_index++;

        if (*++code == '=')
        {
            cpos->_index++;

            *token = token_set2(RSHIFT_EQ_T, poss, *cpos);
            return ++code;
        }

        *token = token_set2(RSHIFT_T, poss, *cpos);
        return code;
    }

    *token = token_set2(GREATER_T, poss, *cpos);
    return code;
}
