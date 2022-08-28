// MetaReal Programming Language version 1.0.0
// MetaReal Memory Library version 1.0.0

#ifndef __M_MEMORY__
#define __M_MEMORY__

#include <ctypes.h>

/* stack */

struct __stack__
{
    uint8p _data;
    uint64 _size;

    uint8p _break;
    uint64 _free;
};
typedef struct __stack__ stack_t[1];

void alloc_stack(stack_t stack, uint64 size);
void increase_stack(stack_t stack, uint64 add);

void clear_stack(stack_t stack, uint64 size);
void free_stack(stack_t stack);

/* */

ptr stack_alloc(stack_t stack, uint64 size, uint64 alt_size);

void stack_free(stack_t stack, ptr block);

/* heap */

struct __ablock__
{
    uint8p _pos;
    uint64 _size;

    struct __ablock__ *_prev;
    struct __ablock__ *_next;
};
typedef struct __ablock__ ablock_t;
typedef struct __ablock__ *ablock_p;

struct __fblock__
{
    uint8p _pos;
    uint64 _size;

    struct __fblock__ *_next;
};
typedef struct __fblock__ fblock_t;
typedef struct __fblock__ *fblock_p;

struct __heap__
{
    uint8p _data;
    uint64 _size;

    ablock_p _ablock;
    fblock_p _fblock;
};
typedef struct __heap__ heap_t[1];

void alloc_heap(heap_t heap, uint64 size);

void increase_heap(heap_t heap, uint64 add);
void clear_heap(heap_t heap, uint64 size);

void free_heap(heap_t heap);

/* */

ablock_p heap_alloc(heap_t heap, uint64 size, uint64 alt_size);

ptr heap_increase(heap_t heap, ablock_p block, uint64 add, uint64 alt_size);

ptr heap_expand(heap_t heap, ablock_p block, uint64 size, uint64 alt_size);
void heap_shrink(heap_t heap, ablock_p block, uint64 size);

void heap_free(heap_t heap, ablock_p block);

#endif /* __M_MEMORY__ */
