// MetaReal Programming Language version 1.0.0

#ifndef __M_DEF__
#define __M_DEF__

#include <stdio.h>

/* */

#define STDIN stdin
#define STDOUT stdout
#define STDERR stderr

#define STDIN_NAME "<stdin>"
#define ROOT_CONTEXT "<program>"

#define CMD_MAX_INPUT 1025
#define FILE_MAX_INPUT 8193

#define OPT_LVL 0

#define STACK_SIZE 65536
#define HEAP_SIZE 65536

/* */

#define RUNTIME_ERROR_MAX_DETAIL 129

/* */

#define TOKENS_SIZE 2048

#define KEYWORD_COUNT 39
#define KEYWORD_PAD 128

#define IDENTIFIER_SIZE 16
#define NUMBER_SIZE 8
#define STRING_SIZE 32

/* */

#define NODES_SIZE 256

#define TUPLE_SIZE 8
#define LIST_SIZE 8
#define DICT_SIZE 8

#define FUNC_ARG_SIZE 6
#define FUNC_BODY_SIZE 32

#define CLASS_BODY_SIZE 64

#define STRUCT_BODY_SIZE 8

#define DOLLAR_FUNC_ARG_SIZE 4

#define IF_CONDITION_SIZE 4
#define IF_BODY_SIZE 16

#define SWITCH_CASE_SIZE 8
#define SWITCH_BODY_SIZE 12

#define FOR_BODY_SIZE 16
#define FOREACH_BODY_SIZE 16

#define WHILE_BODY_SIZE 16
#define DO_WHILE_BODY_SIZE 16

#define LOOP_BODY_SIZE 16

#define TRY_EXPR_SIZE 2
#define TRY_BODY_SIZE 12

/* */

#define IR_MAIN_SIZE 128

#define IR_LIST_SIZE 64

#endif /* __M_DEF__ */
