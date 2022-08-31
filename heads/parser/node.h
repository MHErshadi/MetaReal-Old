// MetaReal Programming Language version 1.0.0

#ifndef __M_NODE__
#define __M_NODE__

#include <lexer/token.h>
#include <memory/memory.h>

enum _node_
{
    EOF_N, // end of file

    /* */

    INT_N, // integer
    FLOAT_N, // float

    BOOL_N, // boolean

    STR_N, // string

    LIST_N, // list
    TUPLE_N, // tuple
    DICT_N, // dictionary

    /* */

    BINARY_OPERATION_N, // binary operation
    UNARY_OPERATION_N, // unary operation

    TERNARY_CONDITION_N, // ternary conditional statement

    SUBSCRIPT_N, // array subscripting statement

    /* */

    VAR_ASSIGN_N, // variable assign statement
    VAR_REASSIGN_N, // variable re-assign statement
    VAR_FIXED_ASSIGN_N, // variable fixed-assign statement
    VAR_ACCESS_N, // variable access statement

    FUNC_DEF_N, // function definition statement
    FUNC_CALL_N, // function call statement

    CLASS_DEF_N, // class definition statement

    STRUCT_DEF_N, // structure definition statement

    DOLLAR_FUNC_CALL_N, // dollar-function call statement

    /* */

    IF_N, // if statement

    SWITCH_N, // switch statement

    /* */

    FOR_N, // for statement
    FOREACH_N, // foreach statement

    WHILE_N, // while statement
    DO_WHILE_N, // do-while statement

    LOOP_N, // loop statement

    /* */

    TRY_N, // try statement

    /* */

    RETURN_N, // return statement

    CONTINUE_N, // continue statement
    BREAK_N, // break statement

    PASS_N, // pass statement

    /* */

    IMPORT_N, // import statement
};

struct __node__
{
    uint8 _type;
    ptr _node;

    pos_t _poss;
    pos_t _pose;
};
typedef struct __node__ node_t;
typedef struct __node__ *node_p;

struct __body__
{
    node_p _nodes;
    uint64 _size;
};
typedef struct __body__ body_t;
typedef struct __body__ *body_p;

struct __stat__
{
    node_t _condition;
    body_t _body;
};
typedef struct __stat__ stat_t;
typedef struct __stat__ *stat_p;

struct __arg__
{
    str _name;
    node_t _value;
};
typedef struct __arg__ arg_t;
typedef struct __arg__ *arg_p;

struct __pair__
{
    node_t key;
    node_t value;
};
typedef struct __pair__ pair_t;
typedef struct __pair__ *pair_p;

struct __int__
{
    str _value;
    uint64 _len;
};
typedef struct __int__ *int_n;

struct __float__ 
{
    str _value;
    uint64 _len;
};
typedef struct __float__ *float_n;

struct __bool__
{
    uint8 _stat : 1;
};
typedef struct __bool__ *bool_n;

struct __str__
{
    str _value;
    uint64 _len;
};
typedef struct __str__ *str_n;

struct __list__
{
    node_p _elements;
    uint64 _size;
};
typedef struct __list__ *list_n;

struct __tuple__
{
    node_p _elements;
    uint64 _size;
};
typedef struct __tuple__ *tuple_n;

struct __dict__
{
    pair_p _elements;
    uint64 _size;
};
typedef struct __dict__ *dict_n;

struct __binary_operation__
{
    uint8 _operator;

    node_t _op1;
    node_t _op2;
};
typedef struct __binary_operation__ *binary_operation_n;

struct __unary_operation__
{
    uint8 _operator;

    node_t _op;
};
typedef struct __unary_operation__ *unary_operation_n;

struct __ternary_condition__
{
    node_t _condition;

    node_t _op1;
    node_t _op2;
};
typedef struct __ternary_condition__ *ternary_condition_n;

struct __subscript__
{
    node_t _array;

    node_t _index;
};
typedef struct __subscript__ *subscript_n;

struct __var_assign__
{
    str _name;
    node_t _value;

    uint8 _properties; // 0b00001234 (1 : access type) (2 : storage context) (3 : is constant) (4 : is static)
};
typedef struct __var_assign__ *var_assign_n;

struct __var_reassign__
{
    uint8 _operator;

    node_t _var;
    node_t _value;
};
typedef struct __var_reassign__ *var_reassign_n;

struct __var_fixed_assign__
{
    uint8 _operator; // 0b00000012 (1 : operator type) (2 : is prefix or not)

    node_t _var;
};
typedef struct __var_fixed_assign__ *var_fixed_assign_n;

struct __var_access__
{
    str _name;
};
typedef struct __var_access__ *var_access_n;

struct __func_def__
{
    str _name;

    arg_p _args;
    uint64 _size;

    body_t _body;

    uint8 _properties; // 0b0051234 (1 : access type) (2 : storage context) (3 : is constant) (4 : is static) (5 : is single-line)
};
typedef struct __func_def__ *func_def_n;

struct __func_call__
{
    node_t _func;

    arg_p _args;
    uint64 _size;
};
typedef struct __func_call__ *func_call_n;

struct __class_def__
{
    str _name;

    body_t _body;

    uint8 _properties; // 0b0001234 (1 : access type) (2 : storage context) (3 : is constant) (4 : is static)
};
typedef struct __class_def__ *class_def_n;

struct __struct_def__
{
    str _name;

    body_t _body;

    uint8 _properties; // 0b0001234 (1 : access type) (2 : storage context) (3 : is constant) (4 : is static)
};
typedef struct __struct_def__ *struct_def_n;

struct __dollar_func_call__
{
    str _name;

    node_p _args;
    uint64 _size;
};
typedef struct __dollar_func_call__ *dollar_func_call_n;

struct __if__
{
    stat_p _stats;
    uint64 _size;

    body_t _ebody;
};
typedef struct __if__ *if_n;

struct __switch__
{
    node_t _value;

    stat_p _stats;
    uint64 _size;

    body_t _dbody;
};
typedef struct __switch__ *switch_n;

struct __for__
{
    str _var;

    node_t _start;
    node_t _end;
    node_t _step;

    body_t _body;
};
typedef struct __for__ *for_n;

struct __foreach__
{
    str _var;

    node_t _iterable;

    body_t _body;
};
typedef struct __foreach__ *foreach_n;

struct __while__
{
    node_t _condition;
    body_t _body;
};
typedef struct __while__ *while_n;

struct __do_while__
{
    body_t _body;
    node_t _condition;
};
typedef struct __do_while__ *do_while_n;

struct __loop__
{
    node_t _init;
    node_t _condition;
    node_t _step;

    body_t _body;
};
typedef struct __loop__ *loop_n;

struct __try__
{
    body_t _error;

    stat_p _stats;
    uint64 _size;

    body_t _fbody;
};
typedef struct __try__ *try_n;

struct __return__
{
    node_t _value;
};
typedef struct __return__ *return_n;

struct __import__
{
    str _lib;
};
typedef struct __import__ *import_n;

node_t node_set1(uint8 type, ptr node, pos_t poss, pos_t pose);
node_t node_set2(uint8 type, pos_t poss, pos_t pose);

body_t body_set(stack_t stack, node_t nodes);

int_n int_n_set(stack_t stack, str value, uint64 len);
float_n float_n_set(stack_t stack, str value, uint64 len);

bool_n bool_n_set(stack_t stack, uint8 stat);

str_n str_n_set(stack_t stack, str value, uint64 len);

list_n list_n_set(stack_t stack, node_p elements, uint64 size);
tuple_n tuple_n_set(stack_t stack, node_p elements, uint64 size);
dict_n dict_n_set(stack_t stack, pair_p elements, uint64 size);

/* */

binary_operation_n binary_operation_n_set(stack_t stack, uint8 operator, node_t op1, node_t op2);
unary_operation_n unary_operation_n_set(stack_t stack, uint8 operator, node_t op);

ternary_condition_n ternary_condition_n_set(stack_t stack, node_t condition, node_t op1, node_t op2);

subscript_n subscript_n_set(stack_t stack, node_t array, node_t index);

/* */

var_assign_n var_assign_n_set(stack_t stack, str name, node_t value, uint8 properties);
var_reassign_n var_reassign_n_set(stack_t stack, uint8 operator, node_t var, node_t value);
var_fixed_assign_n var_fixed_assign_n_set(stack_t stack, uint8 operator, node_t var);
var_access_n var_access_n_set(stack_t stack, str name);

func_def_n func_def_n_set(stack_t stack, str name, arg_p args, uint64 size, body_t body, uint8 properties);
func_call_n func_call_n_set(stack_t stack, node_t func, arg_p args, uint64 size);

class_def_n class_def_n_set(stack_t stack, str name, body_t body, uint8 properties);

struct_def_n struct_def_n_set(stack_t stack, str name, body_t body, uint8 properties);

dollar_func_call_n dollar_func_call_n_set(stack_t stack, str name, node_p args, uint64 size);

/* */

if_n if_n_set(stack_t stack, stat_p stats, uint64 size, body_t ebody);

switch_n switch_n_set(stack_t stack, node_t value, stat_p stats, uint64 size, body_t dbody);

/* */

for_n for_n_set(stack_t stack, str var, node_t start, node_t end, node_t step, body_t body);
foreach_n foreach_n_set(stack_t stack, str var, node_t iterable, body_t body);

while_n while_n_set(stack_t stack, node_t condition, body_t body);
do_while_n do_while_n_set(stack_t stack, body_t body, node_t condition);

loop_n loop_n_set(stack_t stack, node_t init, node_t condition, node_t step, body_t body);

/* */

try_n try_n_set(stack_t stack, body_t error, stat_p stats, uint64 size, body_t fbody);

/* */

return_n return_n_set(stack_t stack, node_t value);

/* */

import_n import_n_set(stack_t stack, str lib);

#endif /* __M_NODE__ */
