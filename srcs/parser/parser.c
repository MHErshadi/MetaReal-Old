// MetaReal Programming Language version 1.0.0

#include <parser/parser.h>
#include <stdlib.h>
#include <crash.h>

#define advance_newline1(t) if ((t)->_type == NEWLINE_T) (t)++
#define advance_newline2(t) for (; (t)->_type == NEWLINE_T || (t)->_type == SEMICOLON_T; (t)++)

void pres_fail(pres_p res, invalid_syntax_t error);

token_p multiline(pres_p res, token_p tokens, stack_t stack, heap_t heap); // multi-line-support
token_p dollar_func(pres_p res, token_p tokens, stack_t stack, heap_t heap); // dollar-function-support
token_p statement(pres_p res, token_p tokens, stack_t stack, heap_t heap); // return continue break pass
token_p tuple(pres_p res, token_p tokens, stack_t stack, heap_t heap); // inline-list-support
token_p assign(pres_p res, token_p tokens, stack_t stack, heap_t heap); // = += -= *= /= %= //= **= &= |= ^= <<= >>=
token_p ternary(pres_p res, token_p tokens, stack_t stack, heap_t heap); // ternary-conditional-statement
token_p type(pres_p res, token_p tokens, stack_t stack, heap_t heap); // is are
token_p contain(pres_p res, token_p tokens, stack_t stack, heap_t heap); // in
token_p ior(pres_p res, token_p tokens, stack_t stack, heap_t heap); // || or
token_p xor(pres_p res, token_p tokens, stack_t stack, heap_t heap); // ^^ xor
token_p and(pres_p res, token_p tokens, stack_t stack, heap_t heap); // && and
token_p b_ior(pres_p res, token_p tokens, stack_t stack, heap_t heap); // |
token_p b_xor(pres_p res, token_p tokens, stack_t stack, heap_t heap); // ^
token_p b_and(pres_p res, token_p tokens, stack_t stack, heap_t heap); // &
token_p cmp1(pres_p res, token_p tokens, stack_t stack, heap_t heap); // == !=
token_p cmp2(pres_p res, token_p tokens, stack_t stack, heap_t heap); // < > <= >=
token_p shift(pres_p res, token_p tokens, stack_t stack, heap_t heap); // << >>
token_p expression(pres_p res, token_p tokens, stack_t stack, heap_t heap); // + -
token_p term(pres_p res, token_p tokens, stack_t stack, heap_t heap); // * / % //
token_p factor(pres_p res, token_p tokens, stack_t stack, heap_t heap); // +(un) -(un) ~ ! not ++ --
token_p exponent(pres_p res, token_p tokens, stack_t stack, heap_t heap); // **
token_p post(pres_p res, token_p tokens, stack_t stack, heap_t heap); // array-subscripting ++(post) --(post) function-call access
token_p core(pres_p res, token_p tokens, stack_t stack, heap_t heap); // statements types ...

token_p hand_var(pres_p res, token_p tokens, stack_t stack, heap_t heap);
token_p hand_list(pres_p res, token_p tokens, stack_t stack, heap_t heap);
token_p hand_dict(pres_p res, token_p tokens, stack_t stack, heap_t heap);
token_p hand_func_def(pres_p res, token_p tokens, stack_t stack, heap_t heap);
token_p hand_func_call(node_t func, pres_p res, token_p tokens, stack_t stack, heap_t heap);
token_p hand_class_def(pres_p res, token_p tokens, stack_t stack, heap_t heap);
token_p hand_struct_def(pres_p res, token_p tokens, stack_t stack, heap_t heap);
token_p hand_if(pres_p res, token_p tokens, stack_t stack, heap_t heap);
token_p hand_switch(pres_p res, token_p tokens, stack_t stack, heap_t heap);
token_p hand_for(pres_p res, token_p tokens, stack_t stack, heap_t heap);
token_p hand_foreach(str var, pos_t poss, pres_p res, token_p tokens, stack_t stack, heap_t heap);
token_p hand_while(pres_p res, token_p tokens, stack_t stack, heap_t heap);
token_p hand_do_while(pres_p res, token_p tokens, stack_t stack, heap_t heap);
token_p hand_loop(pres_p res, token_p tokens, stack_t stack, heap_t heap);
token_p hand_try(pres_p res, token_p tokens, stack_t stack, heap_t heap);

token_p gen_properties(uint8p properties, token_p tokens);
token_p gen_body(body_p body, uint64 alt, pres_p res, token_p tokens, stack_t stack, heap_t heap);
token_p gen_case(body_p body, uint64 alt, pres_p res, token_p tokens, stack_t stack, heap_t heap);

pres_t parse(token_p tokens, stack_t stack, heap_t heap)
{
    pres_t res;
    res._has_error = 0;

    res._nodes = malloc(NODES_SIZE * sizeof(node_t));
    if (!res._nodes)
        alloc_error(NODES_SIZE * sizeof(node_t));

    tokens = multiline(&res, tokens, stack, heap);

    if (!res._has_error && tokens->_type != EOF_T)
    {
        pres_fail(&res, invalid_syntax_set("Expected EOF", tokens->_poss, tokens->_pose));

        free(tokens);
        return res;
    }

    free(tokens);
    return res;
}

void pres_fail(pres_p res, invalid_syntax_t error)
{
    res->_error = error;
    res->_has_error = 1;
}

token_p multiline(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    uint64 alloc = NODES_SIZE;
    uint64 size = 0;

    do
    {
        advance_newline2(tokens);

        if (tokens->_type == EOF_T)
            break;

        if (size == alloc)
        {
            res->_nodes = realloc(res->_nodes, (alloc += NODES_SIZE) * sizeof(node_t));
            if (!res->_nodes)
                alloc_error(alloc * sizeof(node_t));
        }

        tokens = dollar_func(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        size++;
        res->_nodes++;
    } while ((tokens - 1)->_type == NEWLINE_T || tokens->_type == SEMICOLON_T);

    res->_nodes -= size;

    if (size + 1 == alloc)
    {
        res->_nodes = realloc(res->_nodes, (size + 1) * sizeof(node_t));
        if (!res->_nodes)
            alloc_error((size + 1) * sizeof(node_t));
    }

    res->_nodes[size] = node_set2(EOF_N, tokens->_poss, tokens->_pose);

    return tokens;
}

token_p dollar_func(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    if (tokens->_type == DOLLAR_T)
    {
        pos_t poss = tokens++->_poss;

        advance_newline1(tokens);

        if (tokens->_type != IDENTIFIER_T)
        {
            pres_fail(res, invalid_syntax_set("Expected identifier for dollar function", tokens->_poss, tokens->_pose));
            return tokens;
        }

        str name = tokens->_value;

        pos_t pose = tokens++->_pose;

        advance_newline1(tokens);

        tokens = assign(res, tokens, stack, heap);
        if (res->_has_error)
        {
            res->_has_error = 0;

            dollar_func_call_n node = dollar_func_call_n_set(stack, name, NULL, 0);

            *res->_nodes = node_set1(DOLLAR_FUNC_CALL_N, node, poss, pose);
            return tokens;
        }

        node_p args = heap_alloc(heap, DOLLAR_FUNC_ARG_SIZE * sizeof(node_t));
        *args = *res->_nodes;

        uint64 alloc = DOLLAR_FUNC_ARG_SIZE;
        uint64 size = 1;

        while (tokens->_type == COMMA_T)
        {
            tokens++;
            advance_newline1(tokens);

            tokens = assign(res, tokens, stack, heap);
            if (res->_has_error)
                return tokens;

            if (size == alloc)
                args = heap_expand(heap, args, (alloc += DOLLAR_FUNC_ARG_SIZE) * sizeof(node_t));

            args[size++] = *res->_nodes;
        }

        if (size != alloc)
            heap_shrink(heap, args, size * sizeof(node_t));

        dollar_func_call_n node = dollar_func_call_n_set(stack, name, args, size);

        *res->_nodes = node_set1(DOLLAR_FUNC_CALL_N, node, poss, res->_nodes->_pose);
        return tokens;
    }

    return statement(res, tokens, stack, heap);
}

token_p statement(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    if (tokens->_type == RETURN_K)
    {
        pos_t poss = tokens->_poss;
        pos_t pose = tokens->_pose;

        tokens++;
        advance_newline1(tokens);

        token_p temp = tuple(res, tokens, stack, heap);
        if (res->_has_error)
        {
            res->_has_error = 0;

            *res->_nodes = node_set2(RETURN_N, poss, pose);
            return tokens;
        }
        tokens = temp;

        return_n node = return_n_set(stack, *res->_nodes);

        *res->_nodes = node_set1(RETURN_N, node, poss, res->_nodes->_pose);
        return tokens;
    }

    if (tokens->_type == CONTINUE_K)
    {
        *res->_nodes = node_set2(CONTINUE_N, tokens->_poss, tokens->_pose);

        tokens++;
        advance_newline1(tokens);

        return tokens;
    }
    if (tokens->_type == BREAK_K)
    {
        *res->_nodes = node_set2(BREAK_N, tokens->_poss, tokens->_pose);

        tokens++;
        advance_newline1(tokens);

        return tokens;
    }

    if (tokens->_type == PASS_K)
    {
        *res->_nodes = node_set2(PASS_N, tokens->_poss, tokens->_pose);

        tokens++;
        advance_newline1(tokens);

        return tokens;
    }

    return tuple(res, tokens, stack, heap);
}

token_p tuple(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = assign(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    if (tokens->_type == COMMA_T)
    {
        node_p elements = heap_alloc(heap, TUPLE_SIZE * sizeof(node_t));
        *elements = *res->_nodes;

        uint64 alloc = TUPLE_SIZE;
        uint64 size = 1;

        do
        {
            tokens++;
            advance_newline1(tokens);

            tokens = assign(res, tokens, stack, heap);
            if (res->_has_error)
            {
                res->_has_error = 0;

                if (size != alloc)
                    heap_shrink(heap, elements, size * sizeof(node_t));

                tuple_n node = tuple_n_set(stack, elements, size);

                *res->_nodes = node_set1(TUPLE_N, node, elements->_poss, res->_nodes->_pose);
                return tokens;
            }

            if (size == alloc)
                elements = heap_increase(heap, elements, (alloc += TUPLE_SIZE) * sizeof(node_t));

            elements[size++] = *res->_nodes;
        } while (tokens->_type == COMMA_T);

        if (size != alloc)
            heap_shrink(heap, elements, size * sizeof(node_t));

        tuple_n node = tuple_set(stack, elements, size);

        *res->_nodes = node_set1(TUPLE_N, node, elements->_poss, res->_nodes->_pose);
        return tokens;
    }

    return tokens;
}

token_p assign(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    if (tokens->_type == VAR_K)
        return hand_var(res, tokens, stack, heap);

    tokens = ternary(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    if (tokens->_type == ASSIGN_T || (tokens->_type >= ADD_EQ_T && tokens->_type <= RSHIFT_EQ_T))
    {
        uint8 operator = tokens++->_type;

        node_t var = *res->_nodes;

        advance_newline1(tokens);

        tokens = tuple(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        var_reassign_n node = var_reassign_n_set(stack, operator, var, *res->_nodes);

        *res->_nodes = node_set1(VAR_REASSIGN_N, node, var._poss, res->_nodes->_pose);
        return tokens;
    }

    return tokens;
}

token_p ternary(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = type(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    if (tokens->_type == QUESTION_T)
    {
        node_t condition = *res->_nodes;

        tokens++;
        advance_newline1(tokens);

        tokens = assign(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        node_t op1 = *res->_nodes;

        if (tokens->_type != COLON_T)
        {
            pres_fail(res, invalid_syntax_set("Expected ':' for ternary conditional statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        tokens++;
        advance_newline1(tokens);

        tokens = assign(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        ternary_condition_n node = ternary_condition_n_set(stack, condition, op1, *res->_nodes);

        *res->_nodes = node_set1(TERNARY_CONDITION_N, node, condition._poss, res->_nodes->_pose);
        return tokens;
    }

    return tokens;
}

token_p type(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = contain(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t op1 = *res->_nodes;

    while (tokens->_type == IS_K || tokens->_type == ARE_K)
    {
        uint8 operator = tokens++->_type;

        advance_newline1(tokens);

        tokens = contain(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        binary_operation_n node = binary_operation_n_set(stack, operator, op1, *res->_nodes);
        op1 = node_set1(BINARY_OPERATION_N, node, op1._poss, res->_nodes->_pose);
    }

    *res->_nodes = op1;
    return tokens;
}

token_p contain(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = ior(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t op1 = *res->_nodes;

    while (tokens->_type == IN_K)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = ior(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        binary_operation_n node = binary_operation_n_set(stack, IN_K, op1, *res->_nodes);
        op1 = node_set1(BINARY_OPERATION_N, node, op1._poss, res->_nodes->_pose);
    }

    *res->_nodes = op1;
    return tokens;
}

token_p ior(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = xor(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t op1 = *res->_nodes;

    while (tokens->_type == IOR_T || tokens->_type == IOR_K)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = xor(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        binary_operation_n node = binary_operation_n_set(stack, IOR_T, op1, *res->_nodes);
        op1 = node_set1(BINARY_OPERATION_N, node, op1._poss, res->_nodes->_pose);
    }

    *res->_nodes = op1;
    return tokens;
}

token_p xor(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = and(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t op1 = *res->_nodes;

    while (tokens->_type == XOR_T || tokens->_type == XOR_K)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = and(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        binary_operation_n node = binary_operation_n_set(stack, XOR_T, op1, *res->_nodes);
        op1 = node_set1(BINARY_OPERATION_N, node, op1._poss, res->_nodes->_pose);
    }

    *res->_nodes = op1;
    return tokens;
}

token_p and(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = b_ior(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t op1 = *res->_nodes;

    while (tokens->_type == AND_T || tokens->_type == AND_K)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = b_ior(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        binary_operation_n node = binary_operation_n_set(stack, AND_T, op1, *res->_nodes);
        op1 = node_set1(BINARY_OPERATION_N, node, op1._poss, res->_nodes->_pose);
    }

    *res->_nodes = op1;
    return tokens;
}

token_p b_ior(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = b_xor(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t op1 = *res->_nodes;

    while (tokens->_type == B_IOR_T)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = b_xor(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        binary_operation_n node = binary_operation_n_set(stack, B_IOR_T, op1, *res->_nodes);
        op1 = node_set1(BINARY_OPERATION_N, node, op1._poss, res->_nodes->_pose);
    }

    *res->_nodes = op1;
    return tokens;
}

token_p b_xor(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = b_and(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t op1 = *res->_nodes;

    while (tokens->_type == B_XOR_T)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = b_and(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        binary_operation_n node = binary_operation_n_set(stack, B_XOR_T, op1, *res->_nodes);
        op1 = node_set1(BINARY_OPERATION_N, node, op1._poss, res->_nodes->_pose);
    }

    *res->_nodes = op1;
    return tokens;
}

token_p b_and(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = cmp1(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t op1 = *res->_nodes;

    while (tokens->_type == B_AND_T)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = cmp1(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        binary_operation_n node = binary_operation_n_set(stack, B_AND_T, op1, *res->_nodes);
        op1 = node_set1(BINARY_OPERATION_N, node, op1._poss, res->_nodes->_pose);
    }

    *res->_nodes = op1;
    return tokens;
}

token_p cmp1(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = cmp2(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t op1 = *res->_nodes;

    while (tokens->_type == EQUAL_T || tokens->_type == NEQUAL_T)
    {
        uint8 operator = tokens++->_type;

        advance_newline1(tokens);

        tokens = cmp2(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        binary_operation_n node = binary_operation_n_set(stack, operator, op1, *res->_nodes);
        op1 = node_set1(BINARY_OPERATION_N, node, op1._poss, res->_nodes->_pose);
    }

    *res->_nodes = op1;
    return tokens;
}

token_p cmp2(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = shift(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t op1 = *res->_nodes;

    while (tokens->_type >= LESS_T && tokens->_type <= GREATER_EQUAL_T)
    {
        uint8 operator = tokens++->_type;

        advance_newline1(tokens);

        tokens = shift(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        binary_operation_n node = binary_operation_n_set(stack, operator, op1, *res->_nodes);
        op1 = node_set1(BINARY_OPERATION_N, node, op1._poss, res->_nodes->_pose);
    }

    *res->_nodes = op1;
    return tokens;
}

token_p shift(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = expression(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t op1 = *res->_nodes;

    while (tokens->_type == LSHIFT_T || tokens->_type == RSHIFT_T)
    {
        uint8 operator = tokens++->_type;

        advance_newline1(tokens);

        tokens = expression(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        binary_operation_n node = binary_operation_n_set(stack, operator, op1, *res->_nodes);
        op1 = node_set1(BINARY_OPERATION_N, node, op1._poss, res->_nodes->_pose);
    }

    *res->_nodes = op1;
    return tokens;
}

token_p expression(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = term(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t op1 = *res->_nodes;

    while (tokens->_type == ADD_T || tokens->_type == SUBTRACT_T)
    {
        uint8 operator = tokens++->_type;

        advance_newline1(tokens);

        tokens = term(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        binary_operation_n node = binary_operation_n_set(stack, operator, op1, *res->_nodes);
        op1 = node_set1(BINARY_OPERATION_N, node, op1._poss, res->_nodes->_pose);
    }

    *res->_nodes = op1;
    return tokens;
}

token_p term(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = factor(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t op1 = *res->_nodes;

    while (tokens->_type >= MULTIPLY_T && tokens->_type <= QUOTIENT_T)
    {
        uint8 operator = tokens++->_type;

        advance_newline1(tokens);

        tokens = factor(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        binary_operation_n node = binary_operation_n_set(stack, operator, op1, *res->_nodes);
        op1 = node_set1(BINARY_OPERATION_N, node, op1._poss, res->_nodes->_pose);
    }

    *res->_nodes = op1;
    return tokens;
}

token_p factor(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    if (tokens->_type == ADD_T || tokens->_type == SUBTRACT_T ||
        tokens->_type == B_NOT_T ||
        tokens->_type == NOT_T || tokens->_type == NOT_K)
    {
        pos_t poss = tokens->_poss;
        uint8 operator = tokens++->_type;

        advance_newline1(tokens);

        tokens = factor(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        unary_operation_n node = unary_operation_n_set(stack, operator, *res->_nodes);

        *res->_nodes = node_set1(UNARY_OPERATION_N, node, poss, res->_nodes->_pose);
        return tokens;
    }

    if (tokens->_type == INCREMENT_T || tokens->_type == DECREMENT_T)
    {
        pos_t poss = tokens->_poss;
        uint8 operator = tokens++->_type == DECREMENT_T ? 3 : 0;

        advance_newline1(tokens);

        tokens = factor(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        var_fixed_assign_n node = var_fixed_assign_n_set(stack, operator, *res->_nodes);

        *res->_nodes = node_set1(VAR_FIXED_ASSIGN_N, node, poss, res->_nodes->_pose);
        return tokens;
    }

    return exponent(res, tokens, stack, heap);
}

token_p exponent(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = post(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t op1 = *res->_nodes;

    while (tokens->_type == POWER_T)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = factor(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        binary_operation_n node = binary_operation_n_set(stack, POWER_T, op1, *res->_nodes);
        op1 = node_set1(BINARY_OPERATION_N, node, op1._poss, res->_nodes->_pose);
    }

    *res->_nodes = op1;
    return tokens;
}

token_p post(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens = core(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    if (tokens->_type == LSQUARE_BRACE_T)
    {
        node_t array = *res->_nodes;

        tokens++;
        advance_newline1(tokens);

        tokens = tuple(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        if (tokens->_type != RSQUARE_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected ']' for array subscripting statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        subscript_n node = subscript_n_set(stack, array, *res->_nodes);

        *res->_nodes = node_set1(SUBSCRIPT_N, node, array._poss, tokens->_pose);

        tokens++;
        advance_newline1(tokens);

        return tokens;
    }

    if (tokens->_type == INCREMENT_T || tokens->_type == DECREMENT_T)
    {
        var_fixed_assign_n node = var_fixed_assign_n_set(stack, tokens->_type == DECREMENT_T ? 2 : 0, *res->_nodes);

        *res->_nodes = node_set1(VAR_FIXED_ASSIGN_N, node, res->_nodes->_poss, tokens->_pose);

        tokens++;
        advance_newline1(tokens);

        return tokens;
    }

    if (tokens->_type == LPAREN_T)
        return hand_func_call(*res->_nodes, res, tokens, stack, heap);

    if (tokens->_type == DOT_T)
    {
        node_t op1 = *res->_nodes;

        do
        {
            tokens++;
            advance_newline1(tokens);

            tokens = post(res, tokens, stack, heap);
            if (res->_has_error)
                return tokens;

            binary_operation_n node = binary_operation_n_set(stack, DOT_T, op1, *res->_nodes);

            op1 = node_set1(BINARY_OPERATION_N, node, op1._poss, res->_nodes->_pose);
        } while (tokens->_type == DOT_T);

        *res->_nodes = op1;
        return tokens;
    }

    return tokens;
}

token_p core(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    if (tokens->_type == LPAREN_T)
    {
        pos_t poss = tokens++->_poss;

        advance_newline1(tokens);

        tokens = dollar_func(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        if (tokens->_type != RPAREN_T)
        {
            pres_fail(res, invalid_syntax_set("Expected ')' for expression", tokens->_poss, tokens->_pose));
            return tokens;
        }

        res->_nodes->_poss = poss;
        res->_nodes->_pose = tokens++->_pose;

        advance_newline1(tokens);

        return tokens;
    }

    /* */

    if (tokens->_type == INT_T)
    {
        int_n node = int_n_set(stack, tokens->_value, tokens->_len);

        *res->_nodes = node_set1(INT_N, node, tokens->_poss, tokens->_pose);

        tokens++;
        advance_newline1(tokens);

        return tokens;
    }
    if (tokens->_type == FLOAT_T)
    {
        float_n node = float_n_set(stack, tokens->_value, tokens->_len);

        *res->_nodes = node_set1(FLOAT_N, node, tokens->_poss, tokens->_pose);

        tokens++;
        advance_newline1(tokens);

        return tokens;
    }

    if (tokens->_type == TRUE_K)
    {
        bool_n node = bool_n_set(stack, 1);

        *res->_nodes = node_set1(BOOL_N, node, tokens->_poss, tokens->_pose);

        tokens++;
        advance_newline1(tokens);

        return tokens;
    }
    if (tokens->_type == FALSE_K)
    {
        bool_n node = bool_n_set(stack, 0);

        *res->_nodes = node_set1(BOOL_N, node, tokens->_poss, tokens->_pose);

        tokens++;
        advance_newline1(tokens);

        return tokens;
    }

    if (tokens->_type == STR_T)
    {
        if (!tokens->_value)
        {
            *res->_nodes = node_set2(STR_N, tokens->_poss, tokens->_pose);

            tokens++;
            advance_newline1(tokens);

            return tokens;
        }

        str_n node = str_n_set(stack, tokens->_value, tokens->_len);

        *res->_nodes = node_set1(STR_N, node, tokens->_poss, tokens->_pose);

        tokens++;
        advance_newline1(tokens);

        return tokens;
    }

    if (tokens->_type == LSQUARE_BRACE_T)
        return hand_list(res, tokens, stack, heap);
    if (tokens->_type == LCURLY_BRACE_T)
        return hand_dict(res, tokens, stack, heap);

    /* */

    if (tokens->_type == IDENTIFIER_T)
    {
        var_access_n node = var_access_n_set(stack, tokens->_value);

        *res->_nodes = node_set1(VAR_ACCESS_N, node, tokens->_poss, tokens->_pose);

        tokens++;
        advance_newline1(tokens);

        return tokens;
    }

    if (tokens->_type == FUNC_K)
        return hand_func_def(res, tokens, stack, heap);

    if (tokens->_type == CLASS_K)
        return hand_class_def(res, tokens, stack, heap);

    if (tokens->_type == STRUCT_K)
        return hand_struct_def(res, tokens, stack, heap);

    /* */

    if (tokens->_type == IF_K)
        return hand_if(res, tokens, stack, heap);

    if (tokens->_type == SWITCH_K)
        return hand_switch(res, tokens, stack, heap);

    /* */

    if (tokens->_type == FOR_K)
        return hand_for(res, tokens, stack, heap);

    if (tokens->_type == WHILE_K)
        return hand_while(res, tokens, stack, heap);
    if (tokens->_type == DO_K)
        return hand_do_while(res, tokens, stack, heap);

    if (tokens->_type == LOOP_K)
        return hand_loop(res, tokens, stack, heap);

    /* */

    if (tokens->_type == TRY_K)
        return hand_try(res, tokens, stack, heap);

    /* */

    if (tokens->_type == IMPORT_K)
    {
        pos_t poss = tokens++->_poss;

        advance_newline1(tokens);

        if (tokens->_type != IDENTIFIER_T)
        {
            pres_fail(res, invalid_syntax_set("Expected identifier for import statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        import_n node = import_n_set(stack, tokens->_value);

        *res->_nodes = node_set1(IMPORT_N, node, poss, tokens++->_pose);

        advance_newline1(tokens);
        return tokens;
    }

    pres_fail(res, invalid_syntax_set(NULL, tokens->_poss, tokens->_pose));
    return tokens;
}

token_p hand_var(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    pos_t poss = tokens++->_poss;

    advance_newline1(tokens);

    uint8 properties = 0;
    tokens = gen_properties(&properties, tokens);

    if (tokens->_type != IDENTIFIER_T)
    {
        pres_fail(res, invalid_syntax_set("Expected identifier for variable assign statement", tokens->_poss, tokens->_pose));
        return tokens;
    }

    str name = tokens++->_value;

    advance_newline1(tokens);

    if (tokens->_type != ASSIGN_T)
    {
        node_t value;
        value._type = EOF_N;

        var_assign_n node = var_assign_n_set(stack, name, value, properties);

        *res->_nodes = node_set1(VAR_ASSIGN_N, node, poss, (tokens - 1)->_pose);
        return tokens;
    }

    tokens++;
    advance_newline1(tokens);

    tokens = tuple(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    var_assign_n node = var_assign_n_set(stack, name, *res->_nodes, properties);

    *res->_nodes = node_set1(VAR_ASSIGN_N, node, poss, res->_nodes->_pose);
    return tokens;
}

token_p hand_list(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    pos_t poss = tokens++->_poss;

    advance_newline1(tokens);

    if (tokens->_type == RSQUARE_BRACE_T)
    {
        *res->_nodes = node_set2(LIST_N, poss, tokens++->_pose);

        advance_newline1(tokens);

        return tokens;
    }

    node_p elements = heap_alloc(heap, LIST_SIZE * sizeof(node_t));

    uint64 alloc = LIST_SIZE;
    uint64 size = 1;

    tokens = assign(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    *elements = *res->_nodes;

    while (tokens->_type == COMMA_T)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = assign(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        if (size == alloc)
            elements = heap_expand(heap, elements, (alloc += LIST_SIZE) * sizeof(node_t));

        elements[size++] = *res->_nodes;
    }

    if (tokens->_type != RSQUARE_BRACE_T)
    {
        pres_fail(res, invalid_syntax_set("Expected ']' for list", tokens->_poss, tokens->_pose));
        return tokens;
    }

    if (size != alloc)
        heap_shrink(heap, elements, size * sizeof(node_t));

    list_n node = list_n_set(stack, elements, size);

    *res->_nodes = node_set1(LIST_N, node, poss, tokens++->_pose);

    advance_newline1(tokens);

    return tokens;
}

token_p hand_dict(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    pos_t poss = tokens++->_poss;

    advance_newline1(tokens);

    if (tokens->_type == RCURLY_BRACE_T)
    {
        *res->_nodes = node_set2(DICT_N, poss, tokens++->_pose);

        advance_newline1(tokens);

        return tokens;
    }

    pair_p elements = heap_alloc(heap, DICT_SIZE * sizeof(pair_t));

    uint64 alloc = DICT_SIZE;
    uint64 size = 1;
    
    tokens = assign(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    elements->key = *res->_nodes;

    if (tokens->_type != COLON_T)
    {
        pres_fail(res, invalid_syntax_set("Expected ':' for dict", tokens->_poss, tokens->_pose));
        return tokens;
    }

    tokens++;
    advance_newline1(tokens);

    tokens = assign(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    elements->value = *res->_nodes;

    while (tokens->_type == COMMA_T)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = assign(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        if (size == alloc)
            elements = heap_expand(heap, elements, (alloc += DICT_SIZE) * sizeof(pair_t));

        elements[size].key = *res->_nodes;

        if (tokens->_type != COLON_T)
        {
            pres_fail(res, invalid_syntax_set("Expected ':' for dict", tokens->_poss, tokens->_pose));
            return tokens;
        }

        tokens++;
        advance_newline1(tokens);

        tokens = assign(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        elements[size++].value = *res->_nodes;
    }

    if (tokens->_type != RCURLY_BRACE_T)
    {
        pres_fail(res, invalid_syntax_set("Expected '}' for dict", tokens->_poss, tokens->_pose));
        return tokens;
    }

    if (size != alloc)
        heap_shrink(heap, elements, size * sizeof(pair_t));

    dict_n node = dict_n_set(stack, elements, size);

    *res->_nodes = node_set1(DICT_N, node, poss, tokens++->_pose);

    advance_newline1(tokens);

    return tokens;
}

token_p hand_func_def(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    pos_t poss = tokens++->_poss;
    pos_t pose;

    uint8 properties = 0;

    str name = NULL;

    advance_newline1(tokens);

    if (tokens->_type != LPAREN_T)
    {
        tokens = gen_properties(&properties, tokens);

        if (tokens->_type != IDENTIFIER_T)
        {
            pres_fail(res, invalid_syntax_set("Expected identifier for function definition statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        name = tokens++->_value;

        advance_newline1(tokens);

        if (tokens->_type != LPAREN_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '(' for function definition statement", tokens->_poss, tokens->_pose));
            return tokens;
        }
    }

    arg_p args = NULL;

    uint64 size = 0;

    tokens++;
    advance_newline1(tokens);

    if (tokens->_type != RPAREN_T)
    {
        args = heap_alloc(heap, FUNC_ARG_SIZE * sizeof(arg_t));

        uint64 alloc = FUNC_ARG_SIZE;
        size = 1;

        if (tokens->_type != IDENTIFIER_T)
        {
            pres_fail(res, invalid_syntax_set("Expected identifier for function definition statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        if (size == alloc)
            args = heap_expand(heap, args, (alloc += FUNC_ARG_SIZE) * sizeof(arg_t));

        args->_name = tokens++->_value;

        advance_newline1(tokens);

        if (tokens->_type == ASSIGN_T)
        {
            tokens++;
            advance_newline1(tokens);

            tokens = assign(res, tokens, stack, heap);
            if (res->_has_error)
                return tokens;

            args->_value = *res->_nodes;
        }
        else
            args->_value._type = EOF_N;

        while (tokens->_type == COMMA_T)
        {
            tokens++;
            advance_newline1(tokens);

            if (tokens->_type != IDENTIFIER_T)
            {
                pres_fail(res, invalid_syntax_set("Expected identifier for function definition statement", tokens->_poss, tokens->_pose));
                return tokens;
            }

            if (size == alloc)
                args = heap_expand(heap, args, (alloc += FUNC_ARG_SIZE) * sizeof(arg_t));

            args[size]._name = tokens++->_value;

            advance_newline1(tokens);

            if (tokens->_type == ASSIGN_T)
            {
                tokens++;
                advance_newline1(tokens);

                tokens = assign(res, tokens, stack, heap);
                if (res->_has_error)
                    return tokens;

                args[size++]._value = *res->_nodes;
            }
            else
                args[size++]._value._type = EOF_N;
        }

        if (tokens->_type != RPAREN_T)
        {
            pres_fail(res, invalid_syntax_set("Expected ')' for function definition statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        if (size != alloc)
            heap_shrink(heap, args, size * sizeof(arg_t));
    }

    body_t body;

    tokens++;
    advance_newline1(tokens);

    if (tokens->_type == COLON_T)
    {
        properties |= 16;

        tokens++;
        advance_newline1(tokens);

        tokens = dollar_func(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        body = body_set(stack, *res->_nodes);
        pose = res->_nodes->_pose;
    }
    else
    {
        if (tokens->_type != LCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '{' or ':' for function definition statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        tokens = gen_body(&body, FUNC_BODY_SIZE, res, ++tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        if (tokens->_type != RCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '}' for function definition statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        pose = tokens++->_pose;

        advance_newline1(tokens);
    }

    func_def_n node = func_def_n_set(stack, name, args, size, body, properties);

    *res->_nodes = node_set1(FUNC_DEF_N, node, poss, pose);
    return tokens;
}

token_p hand_func_call(node_t func, pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    tokens++;
    advance_newline1(tokens);

    if (tokens->_type == RPAREN_T)
    {
        func_call_n node = func_call_n_set(stack, func, NULL, 0);

        *res->_nodes = node_set1(FUNC_CALL_N, node, func._poss, tokens++->_pose);

        advance_newline1(tokens);

        return tokens;
    }

    arg_p args = heap_alloc(heap, FUNC_ARG_SIZE * sizeof(arg_t));

    uint64 alloc = FUNC_ARG_SIZE;
    uint64 size = 1;

    if (tokens->_type == IDENTIFIER_T)
    {
        token_p temp = tokens;

        str name = tokens++->_value;

        advance_newline1(tokens);

        if (tokens->_type == ASSIGN_T)
        {
            args->_name = name;

            tokens++;
            advance_newline1(tokens);
        }
        else
        {
            args->_name = NULL;
            tokens = temp;
        }
    }
    else
        args->_name = NULL;

    tokens = assign(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    args->_value = *res->_nodes;

    while (tokens->_type == COMMA_T)
    {
        if (size == alloc)
            args = heap_expand(heap, args, (alloc += FUNC_ARG_SIZE) * sizeof(arg_t));

        tokens++;
        advance_newline1(tokens);

        if (tokens->_type == IDENTIFIER_T)
        {
            token_p temp = tokens;

            str name = tokens++->_value;

            advance_newline1(tokens);

            if (tokens->_type == ASSIGN_T)
            {
                args[size]._name = name;

                tokens++;
                advance_newline1(tokens);
            }
            else
            {
                args[size]._name = NULL;
                tokens = temp;
            }
        }
        else
            args[size]._name = NULL;

        tokens = assign(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        args[size++]._value = *res->_nodes;
    }

    if (tokens->_type != RPAREN_T)
    {
        pres_fail(res, invalid_syntax_set("Expected ')' for function call statement", tokens->_poss, tokens->_pose));
        return tokens;
    }

    if (size != alloc)
        heap_shrink(heap, args, size * sizeof(arg_t));

    func_call_n node = func_call_n_set(stack, func, args, size);

    *res->_nodes = node_set1(FUNC_CALL_N, node, func._poss, tokens++->_pose);

    advance_newline1(tokens);

    return tokens;
}

token_p hand_class_def(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    pos_t poss = tokens++->_poss;

    uint8 properties = 0;

    str name = NULL;

    advance_newline1(tokens);

    if (tokens->_type != LCURLY_BRACE_T)
    {
        tokens = gen_properties(&properties, tokens);

        if (tokens->_type != IDENTIFIER_T)
        {
            pres_fail(res, invalid_syntax_set("Expected identifier for class definition statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        name = tokens++->_value;

        advance_newline1(tokens);

        if (tokens->_type != LCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '{' for class definition statement", tokens->_poss, tokens->_pose));
            return tokens;
        }
    }

    body_t body;

    tokens = gen_body(&body, CLASS_BODY_SIZE, res, ++tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    if (tokens->_type != RCURLY_BRACE_T)
    {
        pres_fail(res, invalid_syntax_set("Expected '}' for class definition statement", tokens->_poss, tokens->_pose));
        return tokens;
    }

    class_def_n node = class_def_n_set(stack, name, body, properties);

    *res->_nodes = node_set1(CLASS_DEF_N, node, poss, tokens++->_pose);

    advance_newline1(tokens);

    return tokens;
}

token_p hand_struct_def(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    pos_t poss = tokens++->_poss;

    uint8 properties = 0;

    str name = NULL;

    advance_newline1(tokens);

    if (tokens->_type != LCURLY_BRACE_T)
    {
        tokens = gen_prop(&properties, tokens);

        if (tokens->_type != IDENTIFIER_T)
        {
            pres_fail(res, invalid_syntax_set("Expected identifier for struct definition statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        name = tokens++->_value;

        advance_newline1(tokens);

        if (tokens->_type != LCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '{' for struct definition statement", tokens->_poss, tokens->_pose));
            return tokens;
        }
    }

    body_t body;

    tokens = gen_body(&body, STRUCT_BODY_SIZE, res, ++tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    if (tokens->_type != RCURLY_BRACE_T)
    {
        pres_fail(res, invalid_syntax_set("Expected '}' for struct definition statement", tokens->_poss, tokens->_pose));
        return tokens;
    }

    struct_def_n node = struct_def_n_set(stack, name, body, properties);

    *res->_nodes = node_set1(STRUCT_DEF_N, node, poss, tokens++->_pose);

    advance_newline1(tokens);

    return tokens;
}

token_p hand_if(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    pos_t poss = tokens++->_poss;
    pos_t pose;

    stat_p stats = heap_alloc(heap, IF_CONDITION_SIZE * sizeof(stat_t));

    uint64 alloc = IF_CONDITION_SIZE;
    uint64 size = 1;

    advance_newline1(tokens);

    tokens = tuple(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    stats->_condition = *res->_nodes;

    if (tokens->_type == COLON_T)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = dollar_func(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        stats->_body = body_set(stack, *res->_nodes);
        pose = res->_nodes->_pose;
    }
    else
    {
        if (tokens->_type != LCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '{' or ':' for if statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        tokens = gen_body(&stats->_body, IF_BODY_SIZE, res, ++tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        if (tokens->_type != RCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '}' for if statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        pose = (++tokens)->_pose;

        advance_newline1(tokens);
    }

    while (tokens->_type == ELIF_K)
    {
        if (size == alloc)
            stats = heap_expand(heap, stats, (alloc += IF_CONDITION_SIZE) * sizeof(stat_t));

        tokens++;
        advance_newline1(tokens);

        tokens = tuple(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        stats[size]._condition = *res->_nodes;

        if (tokens->_type == COLON_T)
        {
            tokens++;
            advance_newline1(tokens);

            tokens = dollar_func(res, tokens, stack, heap);
            if (res->_has_error)
                return tokens;

            stats[size++]._body = body_set(stack, *res->_nodes);
            pose = res->_nodes->_pose;
            continue;
        }

        if (tokens->_type != LCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '{' or ':' for if statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        tokens = gen_body(&stats[size++]._body, IF_BODY_SIZE, res, ++tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        if (tokens->_type != RCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '}' for if statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        pose = (++tokens)->_pose;

        advance_newline1(tokens);
    }

    body_t ebody;

    if (tokens->_type == ELSE_K)
    {
        tokens++;
        advance_newline1(tokens);

        if (tokens->_type == COLON_T)
        {
            tokens++;
            advance_newline1(tokens);

            tokens = dollar_func(res, tokens, stack, heap);
            if (res->_has_error)
                return tokens;

            ebody = body_set(stack, *res->_nodes);
            pose = res->_nodes->_pose;
        }
        else
        {
            if (tokens->_type != LCURLY_BRACE_T)
            {
                pres_fail(res, invalid_syntax_set("Expected '{' or ':' for if statement", tokens->_poss, tokens->_pose));
                return tokens;
            }

            tokens = gen_body(&ebody, IF_BODY_SIZE, res, ++tokens, stack, heap);
            if (res->_has_error)
                return tokens;

            if (tokens->_type != RCURLY_BRACE_T)
            {
                pres_fail(res, invalid_syntax_set("Expected '}' for if statement", tokens->_poss, tokens->_pose));
                return tokens;
            }

            pose = (++tokens)->_pose;

            advance_newline1(tokens);
        }
    }
    else
        ebody._size = EOF_N;

    if (size != alloc)
        heap_shrink(heap, stats, size * sizeof(stat_t));

    if_n node = if_n_set(stack, stats, size, ebody);

    *res->_nodes = node_set1(IF_N, node, poss, pose);
    return tokens;
}

token_p hand_switch(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    pos_t poss = tokens++->_poss;

    advance_newline1(tokens);

    tokens = tuple(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t value = *res->_nodes;

    if (tokens->_type != LCURLY_BRACE_T)
    {
        pres_fail(res, invalid_syntax_set("Expected '{' for switch statement", tokens->_poss, tokens->_pose));
        return tokens;
    }

    tokens++;
    advance_newline1(tokens);

    stat_p stats = NULL;

    uint64 size = 0;

    if (tokens->_type == CASE_K)
    {
        stats = heap_alloc(heap, SWITCH_CASE_SIZE * sizeof(stat_t));

        uint64 alloc = SWITCH_CASE_SIZE;

        do
        {
            tokens++;
            advance_newline1(tokens);
            
            tokens = tuple(res, tokens, stack, heap);
            if (res->_has_error)
                return tokens;

            if (size == alloc)
                stats = heap_expand(heap, stats, (alloc += SWITCH_CASE_SIZE) * sizeof(stat_t));

            stats[size]._condition = *res->_nodes;

            if (tokens->_type != COLON_T)
            {
                pres_fail(res, invalid_syntax_set("Expected ':' for switch statement", tokens->_poss, tokens->_pose));
                return tokens;
            }

            tokens = gen_case(&stats[size++]._body, SWITCH_BODY_SIZE, res, ++tokens, stack, heap);
            if (res->_has_error)
                return tokens;
        } while (tokens->_type == CASE_K);

        if (size != alloc)
            heap_shrink(heap, stats, size * sizeof(stat_t));
    }

    body_t dbody;

    if (tokens->_type == DEFAULT_K)
    {
        tokens++;
        advance_newline1(tokens);

        if (tokens->_type != COLON_T)
        {
            pres_fail(res, invalid_syntax_set("Expected ':' for switch statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        tokens = gen_body(&dbody, SWITCH_BODY_SIZE, res, ++tokens, stack, heap);
        if (res->_has_error)
            return tokens;
    }
    else
        dbody._size = 0;

    if (tokens->_type != RCURLY_BRACE_T)
    {
        pres_fail(res, invalid_syntax_set("Expected '}' for switch statement", tokens->_poss, tokens->_pose));
        return tokens;
    }

    switch_n node = switch_n_set(stack, value, stats, size, dbody);

    *res->_nodes = node_set1(SWITCH_N, node, poss, tokens++->_pose);

    advance_newline1(tokens);

    return tokens;
}

token_p hand_for(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    pos_t poss = tokens++->_poss;
    pos_t pose;

    advance_newline1(tokens);

    if (tokens->_type != IDENTIFIER_T)
    {
        pres_fail(res, invalid_syntax_set("Expected identifier for for statement", tokens->_poss, tokens->_pose));
        return tokens;
    }

    str var = tokens++->_value;

    advance_newline1(tokens);

    if (tokens->_type == IN_K)
        return hand_foreach(var, poss, res, ++tokens, stack, heap);

    if (tokens->_type != ASSIGN_T)
    {
        pres_fail(res, invalid_syntax_set("Expected '=' for for statement or 'in' for foreach statement", tokens->_poss, tokens->_pose));
        return tokens;
    }

    tokens++;
    advance_newline1(tokens);

    node_t start;

    if (tokens->_type == TO_K)
        start._type = EOF_N;
    else
    {
        tokens = tuple(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        start = *res->_nodes;

        if (tokens->_type != TO_K)
        {
            pres_fail(res, invalid_syntax_set("Expected 'to' for for statement", tokens->_poss, tokens->_pose));
            return tokens;
        }
    }

    tokens++;
    advance_newline1(tokens);

    tokens = tuple(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t end = *res->_nodes;

    node_t step;

    if (tokens->_type == STEP_K)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = tuple(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        step = *res->_nodes;
    }
    else
        step._type = EOF_N;

    body_t body;

    if (tokens->_type == COLON_T)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = dollar_func(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        body = body_set(stack, *res->_nodes);
        pose = res->_nodes->_pose;
    }
    else
    {
        if (tokens->_type != LCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '{' or ':' for for statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        tokens = gen_body(&body, FOR_BODY_SIZE, res, ++tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        if (tokens->_type != RCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '}' for for statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        pose = tokens++->_pose;

        advance_newline1(tokens);
    }

    for_n node = for_n_set(stack, var, start, end, step, body);

    *res->_nodes = node_set1(FOR_N, node, poss, pose);
    return tokens;
}

token_p hand_foreach(str var, pos_t poss, pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    pos_t pose;

    advance_newline1(tokens);

    tokens = tuple(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t iterable = *res->_nodes;

    body_t body;

    if (tokens->_type == COLON_T)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = dollar_func(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        body = body_set(stack, *res->_nodes);
        pose = res->_nodes->_pose;
    }
    else
    {
        if (tokens->_type != LCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '{' or ':' for foreach statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        tokens = gen_body(&body, FOREACH_BODY_SIZE, res, ++tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        if (tokens->_type != RCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '}' for foreach statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        pose = tokens++->_pose;

        advance_newline1(tokens);
    }

    foreach_n node = foreach_n_set(stack, var, iterable, body);

    *res->_nodes = node_set1(FOREACH_N, node, poss, pose);
    return tokens;
}

token_p hand_while(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    pos_t poss = tokens++->_poss;
    pos_t pose;

    advance_newline1(tokens);

    tokens = tuple(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    node_t condition = *res->_nodes;

    body_t body;

    if (tokens->_type == COLON_T)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = dollar_func(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        body = body_set(stack, *res->_nodes);
        pose = res->_nodes->_pose;
    }
    else
    {
        if (tokens->_type != LCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '{' or ':' for while statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        tokens = gen_body(&body, WHILE_BODY_SIZE, res, ++tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        if (tokens->_type != RCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '}' for while statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        pose = tokens++->_pose;

        advance_newline1(tokens);
    }

    while_n node = while_n_set(stack, condition, body);

    *res->_nodes = node_set1(WHILE_N, node, poss, pose);
    return tokens;
}

token_p hand_do_while(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    pos_t poss = tokens++->_poss;

    body_t body;

    advance_newline1(tokens);

    if (tokens->_type == COLON_T)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = dollar_func(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        body = body_set(stack, *res->_nodes);
    }
    else
    {
        if (tokens->_type != LCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '{' or ':' for do-while statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        tokens = gen_body(&body, DO_WHILE_BODY_SIZE, res, ++tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        if (tokens->_type != RCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '}' for do-while statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        tokens++;
        advance_newline1(tokens);
    }

    if (tokens->_type != WHILE_K)
    {
        pres_fail(res, invalid_syntax_set("Expected 'while' for do-while statement", tokens->_poss, tokens->_pose));
        return tokens;
    }

    tokens++;
    advance_newline1(tokens);

    tokens = tuple(res, tokens, stack, heap);
    if (res->_has_error)
        return tokens;

    do_while_n node = do_while_n_set(stack, body, *res->_nodes);

    *res->_nodes = node_set1(DO_WHILE_N, node, poss, res->_nodes->_pose);
    return tokens;
}

token_p hand_loop(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    pos_t poss = tokens++->_poss;
    pos_t pose;

    advance_newline1(tokens);

    node_t init;

    if (tokens->_type == SEMICOLON_T)
        init._type = EOF_N;
    else
    {
        tokens = tuple(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        init = *res->_nodes;

        if (tokens->_type != SEMICOLON_T)
        {
            pres_fail(res, invalid_syntax_set("Expected ';' for loop statement", tokens->_poss, tokens->_pose));
            return tokens;
        }
    }

    node_t condition;

    tokens++;
    advance_newline1(tokens);

    if (tokens->_type == SEMICOLON_T)
        condition._type = EOF_N;
    else
    {
        tokens = tuple(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        condition = *res->_nodes;

        if (tokens->_type != SEMICOLON_T)
        {
            pres_fail(res, invalid_syntax_set("Expected ';' for loop statement", tokens->_poss, tokens->_pose));
            return tokens;
        }
    }

    node_t step;
    body_t body;

    tokens++;
    advance_newline1(tokens);

    if (tokens->_type == COLON_T)
    {
        step._type = EOF_N;

        tokens++;
        advance_newline1(tokens);

        tokens = dollar_func(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        body = body_set(stack, *res->_nodes);
        pose = res->_nodes->_pose;
    }
    else if (tokens->_type == LCURLY_BRACE_T)
    {
        step._type = EOF_N;

        tokens = gen_body(&body, LOOP_BODY_SIZE, res, ++tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        if (tokens->_type != RCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '}' for loop statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        pose = tokens++->_pose;

        advance_newline1(tokens);
    }
    else
    {
        tokens = tuple(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        step = *res->_nodes;

        if (tokens->_type == COLON_T)
        {
            tokens++;
            advance_newline1(tokens);

            tokens = dollar_func(res, tokens, stack, heap);
            if (res->_has_error)
                return tokens;

            body = body_set(stack, *res->_nodes);
            pose = res->_nodes->_pose;
        }
        else
        {
            if (tokens->_type != LCURLY_BRACE_T)
            {
                pres_fail(res, invalid_syntax_set("Expected '{' or ':' for loop statement", tokens->_poss, tokens->_pose));
                return tokens;
            }

            tokens = gen_body(&body, LOOP_BODY_SIZE, res, ++tokens, stack, heap);
            if (res->_has_error)
                return tokens;

            if (tokens->_type != RCURLY_BRACE_T)
            {
                pres_fail(res, invalid_syntax_set("Expected '}' for loop statement", tokens->_poss, tokens->_pose));
                return tokens;
            }

            pose = tokens++->_pose;

            advance_newline1(tokens);
        }
    }

    loop_n node = loop_n_set(stack, init, condition, step, body);

    *res->_nodes = node_set1(LOOP_N, node, poss, pose);
    return tokens;
}

token_p hand_try(pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    pos_t poss = tokens++->_poss;
    pos_t pose;

    advance_newline1(tokens);

    body_t error;

    if (tokens->_type == COLON_T)
    {
        tokens++;
        advance_newline1(tokens);

        tokens = dollar_func(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        error = body_set(stack, *res->_nodes);
        pose = res->_nodes->_pose;
    }
    else
    {
        if (tokens->_type != LCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '{' or ':' for try statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        tokens = gen_body(&error, TRY_BODY_SIZE, res, ++tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        if (tokens->_type != RCURLY_BRACE_T)
        {
            pres_fail(res, invalid_syntax_set("Expected '}' for try statement", tokens->_poss, tokens->_pose));
            return tokens;
        }

        pose = tokens++->_pose;

        advance_newline1(tokens);
    }

    stat_p stats = NULL;

    uint64 size = 0;

    if (tokens->_type == EXCEPT_K)
    {
        stats = heap_alloc(heap, TRY_EXPR_SIZE * sizeof(stat_t));

        uint64 alloc = TRY_EXPR_SIZE;

        do
        {
            tokens++;
            advance_newline1(tokens);

            tokens = tuple(res, tokens, stack, heap);
            if (res->_has_error)
                return tokens;

            if (size == alloc)
                stats = heap_expand(heap, stats, (alloc += TRY_EXPR_SIZE) * sizeof(stat_t));

            stats[size]._condition = *res->_nodes;

            if (tokens->_type == COLON_T)
            {
                tokens++;
                advance_newline1(tokens);

                tokens = dollar_func(res, tokens, stack, heap);
                if (res->_has_error)
                    return tokens;

                stats[size++]._body = body_set(stack, *res->_nodes);

                pose = res->_nodes->_pose;
                continue;
            }

            if (tokens->_type != LCURLY_BRACE_T)
            {
                pres_fail(res, invalid_syntax_set("Expected '{' or ':' for try statement", tokens->_poss, tokens->_pose));
                return tokens;
            }

            tokens = gen_body(&stats[size++]._body, TRY_BODY_SIZE, res, ++tokens, stack, heap);
            if (res->_has_error)
                return tokens;

            if (tokens->_type != RCURLY_BRACE_T)
            {
                pres_fail(res, invalid_syntax_set("Expected '}' for try statement", tokens->_poss, tokens->_pose));
                return tokens;
            }

            pose = tokens++->_pose;

            advance_newline1(tokens);
        } while (tokens->_type == EXCEPT_K);

        if (size != alloc)
            heap_shrink(heap, stats, size * sizeof(stat_t));
    }

    body_t fbody;

    if (tokens->_type == FINALLY_K)
    {
        tokens++;
        advance_newline1(tokens);

        if (tokens->_type == COLON_T)
        {
            tokens++;
            advance_newline1(tokens);

            tokens = dollar_func(res, tokens, stack, heap);
            if (res->_has_error)
                return tokens;

            fbody = body_set(stack, *res->_nodes);
            pose = res->_nodes->_pose;
        }
        else
        {
            if (tokens->_type != LCURLY_BRACE_T)
            {
                pres_fail(res, invalid_syntax_set("Expected '{' or ':' for try statement", tokens->_poss, tokens->_pose));
                return tokens;
            }

            tokens = gen_body(&fbody, TRY_BODY_SIZE, res, ++tokens, stack, heap);
            if (res->_has_error)

            if (tokens->_type != RCURLY_BRACE_T)
            {
                pres_fail(res, invalid_syntax_set("Expected '}' for try statement", tokens->_poss, tokens->_pose));
                return tokens;
            }

            pose = tokens++->_pose;

            advance_newline1(tokens);
        }
    }
    else
        fbody._size = 0;

    try_n node = try_n_set(stack, error, stats, size, fbody);

    *res->_nodes = node_set1(TRY_N, node, poss, pose);
    return tokens;
}

token_p gen_properties(uint8p properties, token_p tokens)
{
    uint8 access = 0;
    uint8 context = 0;
    uint8 is_const = 0;
    uint8 is_static = 0;

    while (1)
    {
        if (tokens->_type == PUBLIC_K && !access)
        {
            access = 1;
            *properties |= 8;

            tokens++;
            advance_newline1(tokens);

            continue;
        }
        else if (tokens->_type == PRIVATE_K && !access)
        {
            access = 1;

            tokens++;
            advance_newline1(tokens);

            continue;
        }

        if (tokens->_type == GLOBAL_K && !context)
        {
            context = 1;
            *properties |= 4;

            tokens++;
            advance_newline1(tokens);

            continue;
        }
        else if (tokens->_type == LOCAL_K && !context)
        {
            context = 1;

            tokens++;
            advance_newline1(tokens);

            continue;
        }

        if (tokens->_type == CONST_K && !is_const)
        {
            is_const = 1;
            *properties |= 2;

            tokens++;
            advance_newline1(tokens);

            continue;
        }

        if (tokens->_type == STATIC_K && !is_static)
        {
            is_static = 1;
            *properties |= 1;

            tokens++;
            advance_newline1(tokens);

            continue;
        }

        advance_newline1(tokens);

        return tokens;
    }
}

token_p gen_body(body_p body, uint64 alt, pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    body->_nodes = heap_alloc(heap, alt * sizeof(node_t));

    uint64 alloc = alt;
    body->_size = 0;

    do
    {
        advance_newline2(tokens);

        if (tokens->_type == RCURLY_BRACE_T || tokens->_type == EOF_T)
            break;

        if (body->_size == alloc)
            body->_nodes = heap_expand(heap, body->_nodes, (alloc += alt) * sizeof(node_t));

        tokens = dollar_func(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        body->_nodes[body->_size++] = *res->_nodes;
    } while ((tokens - 1)->_type == NEWLINE_T || tokens->_type == SEMICOLON_T);

    if (!body->_size)
    {
        heap_free(heap, body->_nodes);

        return tokens;
    }

    if (body->_size != alloc)
        heap_shrink(heap, body->_nodes, body->_size * sizeof(node_t));

    return tokens;
}

token_p gen_case(body_p body, uint64 alt, pres_p res, token_p tokens, stack_t stack, heap_t heap)
{
    body->_nodes = heap_alloc(heap, alt * sizeof(node_t));

    uint64 alloc = alt;
    body->_size = 0;

    do
    {
        adv_nln2(tokens);

        if (tokens->_type == CASE_K || tokens->_type == DEFAULT_K || tokens->_type == RCURLY_BRACE_T || tokens->_type == EOF_T)
            break;

        if (body->_size == alloc)
            body->_nodes = heap_expand(heap, body->_nodes, (alloc += alt) * sizeof(node_t));

        tokens = dollar_func(res, tokens, stack, heap);
        if (res->_has_error)
            return tokens;

        body->_nodes[body->_size++] = *res->_nodes;
    } while ((tokens - 1)->_type == NEWLINE_T || tokens->_type == SEMICOLON_T);

    if (!body->_size)
    {
        heap_free(heap, body->_nodes);

        return tokens;
    }

    if (body->_size != alloc)
        heap_shrink(heap, body->_nodes, body->_size * sizeof(node_t));

    return tokens;
}
