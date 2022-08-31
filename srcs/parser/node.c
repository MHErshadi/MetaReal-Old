// MetaReal Programming Language version 1.0.0

#include <parser/node.h>

node_t node_set1(uint8 type, ptr node, pos_t poss, pos_t pose)
{
    node_t node_;

    node_._type = type;
    node_._node = node;
    node_._poss = pose;
    node_._pose = pose;

    return node_;
}

node_t node_set2(uint8 type, pos_t poss, pos_t pose)
{
    node_t node;

    node._type = type;
    node._node = NULL;
    node._poss = poss;
    node._pose = pose;

    return node;
}

body_t body_set(stack_t stack, node_t nodes)
{
    body_t body;

    body._nodes = stack_alloc(stack, sizeof(node_t), STACK_SIZE);

    *body._nodes = nodes;
    body._size = 1;

    return body;
}

int_n int_n_set(stack_t stack, str value, uint64 len)
{
    int_n node = stack_alloc(stack, sizeof(struct __int__), STACK_SIZE);

    node->_value = value;
    node->_len = len;

    return node;
}

float_n float_n_set(stack_t stack, str value, uint64 len)
{
    float_n node = stack_alloc(stack, sizeof(struct __float__), STACK_SIZE);

    node->_value = value;
    node->_len = len;

    return node;
}

bool_n bool_n_set(stack_t stack, uint8 stat)
{
    bool_n node = stack_alloc(stack, sizeof(struct __bool__), STACK_SIZE);

    node->_stat = stat;

    return node;
}

str_n str_n_set(stack_t stack, str value, uint64 len)
{
    str_n node = stack_alloc(stack, sizeof(struct __str__), STACK_SIZE);

    node->_value = value;
    node->_len = len;

    return node;
}

list_n list_n_set(stack_t stack, node_p elements, uint64 size)
{
    list_n node = stack_alloc(stack, sizeof(struct __list__), STACK_SIZE);

    node->_elements = elements;
    node->_size = size;

    return node;
}

tuple_n tuple_n_set(stack_t stack, node_p elements, uint64 size)
{
    tuple_n node = stack_alloc(stack, sizeof(struct __tuple__), STACK_SIZE);

    node->_elements = elements;
    node->_size = size;

    return node;
}

dict_n dict_n_set(stack_t stack, pair_p elements, uint64 size)
{
    dict_n node = stack_alloc(stack, sizeof(struct __dict__), STACK_SIZE);

    node->_elements = elements;
    node->_size = size;

    return node;
}

binary_operation_n binary_operation_n_set(stack_t stack, uint8 operator, node_t op1, node_t op2)
{
    binary_operation_n node = stack_alloc(stack, sizeof(struct __binary_operation__), STACK_SIZE);

    node->_operator = operator;
    node->_op1 = op1;
    node->_op2 = op2;

    return node;
}

unary_operation_n unary_operation_n_set(stack_t stack, uint8 operator, node_t op)
{
    unary_operation_n node = stack_alloc(stack, sizeof(struct __unary_operation__), STACK_SIZE);

    node->_operator = operator;
    node->_op = op;

    return node;
}

ternary_condition_n ternary_condition_n_set(stack_t stack, node_t condition, node_t op1, node_t op2)
{
    ternary_condition_n node = stack_alloc(stack, sizeof(struct __ternary_condition__), STACK_SIZE);

    node->_condition = condition;
    node->_op1 = op1;
    node->_op2 = op2;

    return node;
}

subscript_n subscript_n_set(stack_t stack, node_t array, node_t index)
{
    subscript_n node = stack_alloc(stack, sizeof(struct __subscript__), STACK_SIZE);

    node->_array = array;
    node->_index = index;

    return node;
}

var_assign_n var_assign_n_set(stack_t stack, str name, node_t value, uint8 properties)
{
    var_assign_n node = stack_alloc(stack, sizeof(struct __var_assign__), STACK_SIZE);

    node->_name = name;
    node->_value = value;
    node->_properties = properties;

    return node;
}

var_reassign_n var_reassign_n_set(stack_t stack, uint8 operator, node_t var, node_t value)
{
    var_reassign_n node = stack_alloc(stack, sizeof(struct __var_reassign__), STACK_SIZE);

    node->_operator = operator;
    node->_var = var;
    node->_value = value;

    return node;
}

var_fixed_assign_n var_fixed_assign_n_set(stack_t stack, uint8 operator, node_t var)
{
    var_fixed_assign_n node = stack_alloc(stack, sizeof(struct __var_fixed_assign__), STACK_SIZE);

    node->_operator = operator;
    node->_var = var;

    return node;
}

var_access_n var_access_n_set(stack_t stack, str name)
{
    var_access_n node = stack_alloc(stack, sizeof(struct __var_access__), STACK_SIZE);

    node->_name = name;

    return node;
}

func_def_n func_def_n_set(stack_t stack, str name, arg_p args, uint64 size, body_t body, uint8 properties)
{
    func_def_n node = stack_alloc(stack, sizeof(struct __func_def__), STACK_SIZE);

    node->_name = name;
    node->_args = args;
    node->_size = size;
    node->_body = body;
    node->_properties = properties;

    return node;
}

func_call_n func_call_n_set(stack_t stack, node_t func, arg_p args, uint64 size)
{
    func_call_n node = stack_alloc(stack, sizeof(struct __func_call__), STACK_SIZE);

    node->_func = func;
    node->_args = args;
    node->_size = size;

    return node;
}

class_def_n class_def_n_set(stack_t stack, str name, body_t body, uint8 properties)
{
    class_def_n node = stack_alloc(stack, sizeof(struct __class_def__), STACK_SIZE);

    node->_name = name;
    node->_body = body;
    node->_properties = properties;

    return node;
}

struct_def_n struct_def_n_set(stack_t stack, str name, body_t body, uint8 properties)
{
    struct_def_n node = stack_alloc(stack, sizeof(struct __struct_def__), STACK_SIZE);

    node->_name = name;
    node->_body = body;
    node->_properties = properties;

    return node;
}

dollar_func_call_n dollar_func_call_n_set(stack_t stack, str name, node_p args, uint64 size)
{
    dollar_func_call_n node = stack_alloc(stack, sizeof(struct __dollar_func_call__), STACK_SIZE);

    node->_name = name;
    node->_args = args;
    node->_size = size;

    return node;
}

if_n if_n_set(stack_t stack, stat_p stats, uint64 size, body_t ebody)
{
    if_n node = stack_alloc(stack, sizeof(struct __if__), STACK_SIZE);

    node->_stats = stats;
    node->_size = size;
    node->_ebody = ebody;

    return node;
}

switch_n switch_n_set(stack_t stack, node_t value, stat_p stats, uint64 size, body_t dbody)
{
    switch_n node = stack_alloc(stack, sizeof(struct __switch__), STACK_SIZE);

    node->_value = value;
    node->_stats = stats;
    node->_size = size;
    node->_dbody = dbody;

    return node;
}

for_n for_n_set(stack_t stack, str var, node_t start, node_t end, node_t step, body_t body)
{
    for_n node = stack_alloc(stack, sizeof(struct __for__), STACK_SIZE);

    node->_var = var;
    node->_start = start;
    node->_end = end;
    node->_step = step;
    node->_body = body;

    return node;
}

foreach_n foreach_n_set(stack_t stack, str var, node_t iterable, body_t body)
{
    foreach_n node = stack_alloc(stack, sizeof(struct __foreach__), STACK_SIZE);

    node->_var = var;
    node->_iterable = iterable;
    node->_body = body;

    return node;
}

while_n while_n_set(stack_t stack, node_t condition, body_t body)
{
    while_n node = stack_alloc(stack, sizeof(struct __while__), STACK_SIZE);

    node->_condition = condition;
    node->_body = body;

    return node;
}

do_while_n do_while_n_set(stack_t stack, body_t body, node_t condition)
{
    do_while_n node = stack_alloc(stack, sizeof(struct __do_while__), STACK_SIZE);

    node->_body = body;
    node->_condition = condition;

    return node;
}

loop_n loop_n_set(stack_t stack, node_t init, node_t condition, node_t step, body_t body)
{
    loop_n node = stack_alloc(stack, sizeof(struct __loop__), STACK_SIZE);

    node->_init = init;
    node->_condition = condition;
    node->_step = step;
    node->_body = body;

    return node;
}

try_n try_n_set(stack_t stack, body_t error, stat_p stats, uint64 size, body_t fbody)
{
    try_n node = stack_alloc(stack, sizeof(struct __try__), STACK_SIZE);

    node->_error = error;
    node->_stats = stats;
    node->_size = size;
    node->_fbody = fbody;

    return node;
}

return_n return_n_set(stack_t stack, node_t value)
{
    return_n node = stack_alloc(stack, sizeof(struct __return__), STACK_SIZE);

    node->_value = value;

    return node;
}

import_n import_n_set(stack_t stack, str lib)
{
    import_n node = stack_alloc(stack, sizeof(struct __import__), STACK_SIZE);

    node->_lib = lib;

    return node;
}
