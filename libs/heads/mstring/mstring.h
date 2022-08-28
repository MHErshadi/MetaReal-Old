// MetaReal Programming Language version 1.0.0
// MetaReal String Library version 1.0.0

#ifndef __M_MSTR__
#define __M_MSTR__

#include <memory/memory.h>

/* string */

struct __mstr__
{
    str _str;
    uint64 _size;

    ablock_p _ptr;
};
typedef struct __mstr__ mstr_t[1];
typedef const struct __mstr__ mstr_c[1];

void set_mstr(heap_t heap, uint64 alt_size, mstr_t dst, cstr src, uint64 size);
void copy_mstr(heap_t heap, uint64 alt_size, mstr_t dst, mstr_c src);
void free_mstr(heap_t heap, mstr_t str);

void mstr_concat(heap_t heap, uint64 alt_size, mstr_t op1, mstr_c op2);
void mstr_repeat(heap_t heap, uint64 alt_size, mstr_t str, uint64 count);
void mstr_remove(heap_t heap, mstr_t str, uint64 pos);

uint8 mstr_equal(mstr_c op1, mstr_c op2);
uint8 mstr_nequal(mstr_c op1, mstr_c op2);

#endif /* __M_MSTR__ */
