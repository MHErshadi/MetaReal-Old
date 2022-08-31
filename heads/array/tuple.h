// MetaReal Programming Language version 1.0.0

#ifndef __M_TUPLE__
#define __M_TUPLE__

#include <ir/block.h>

struct __mtuple__
{
    block_p _elements;
    uint64 _size;
};
typedef struct __mtuple__ mtuple_t[1];
typedef const struct __mtuple__ mtuple_c[1];

void set_mtuple(mtuple_t dst, block_p elements, uint64 size);

void copy_mtuple(stack_t stack, heap_t heap, mtuple_t dst, mtuple_c src);

void free_mtuple(heap_t heap, mtuple_t mtuple);

/* */

uint8 mtuple_equal(mtuple_c op1, mtuple_c op2);
uint8 mtuple_nequal(mtuple_c op1, mtuple_c op2);

#endif /* __M_TUPLE__ */
