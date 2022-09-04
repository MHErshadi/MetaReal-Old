// MetaReal Programming Language version 1.0.0

#ifndef __M_MDICT__
#define __M_MDICT__

#include <ir/block_struct.h>

struct __dict_node__
{
    block_t _key;
    block_t _value;

    struct __dict_node__ *_left;
    struct __dict_node__ *_right;
};
typedef struct __dict_node__ dict_node_t;
typedef struct __dict_node__ *dict_node_p;

struct __mdict__
{
    dict_node_p _node;
    uint64 _size;
};
typedef struct __mdict__ mdict_t[1];
typedef const struct __mdict__ mdict_c[1];

void set_mdict(mdict_t dst, dict_node_p node, uint64 size);

void copy_mdict(stack_t stack, heap_t heap, mdict_t dst, mdict_c src);

void free_mdict(heap_t heap, mdict_t mdict);

/* */

void mdict_concat(heap_t heap, mdict_t op1, mdict_t op2);
void mdict_remove(heap_t heap, mdict_t mdict, block_t key);

uint8 mdict_contains(mdict_c mdict, block_t key);

uint8 mdict_equal(mdict_c op1, mdict_c op2);
uint8 mdict_nequal(mdict_c op1, mdict_c op2);

/* dict_node */

dict_node_p set_dict_node(heap_t heap, block_t key, block_t value, dict_node_p left, dict_node_p right);

dict_node_p copy_dict_node(stack_t stack, heap_t heap, dict_node_p src);

void free_dict_node(heap_t heap, dict_node_p node);

/* */

uint8 dict_node_new(heap_t heap, dict_node_p node, block_t key, block_t value);

uint8 dict_node_add(heap_t heap, dict_node_p node, dict_node_p add);
uint64 dict_node_concat(heap_t heap, dict_node_p dst, dict_node_p src);

uint8 dict_node_contains(dict_node_p node, block_t key);

uint8 dict_node_equal(dict_node_p op1, dict_node_p op2);

#endif /* __M_MDICT__ */
