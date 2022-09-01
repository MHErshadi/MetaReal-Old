// MetaReal Programming Language version 1.0.0

#ifndef __M_LIST__
#define __M_LIST__

#include <array/tuple.h>

struct __mlist__
{
    block_p _elements;
    uint64 _size;
};
typedef struct __mlist__ mlist_t[1];
typedef const struct __mlist__ mlist_c[1];

void set_mlist(mlist_t dst, block_p elements, uint64 size);

void copy_mlist(stack_t stack, heap_t heap, mlist_t dst, mlist_c src);

void free_mlist(heap_t heap, mlist_t mlist);

/* */

void mlist_concat(heap_t heap, mlist_t op1, mlist_t op2);
void mlist_concat_mtuple(heap_t heap, mlist_t op1, mtuple_t op2);
void mlist_append(heap_t heap, mlist_t mlist, block_t element);
void mlist_remove(heap_t heap, mlist_t mlist, uint64 pos);
void mlist_repeat(stack_t stack, heap_t heap, mlist_t mlist, uint64 count);

uint8 mlist_equal(mlist_c op1, mlist_c op2);
uint8 mlist_nequal(mlist_c op1, mlist_c op2);

#endif /* __M_LIST__ */
