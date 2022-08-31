// MetaReal Programming Language version 1.0.0

#include <array/tuple.h>

void copy_mtuple(stack_t stack, heap_t heap, mtuple_t dst, mtuple_c src)
{
    dst->_elements = heap_alloc(heap, src->_size * sizeof(block_t));

    uint64 i;
    for (i = 0; i < src->_size; i++)
        dst->_elements[i] = block_copy(src->_elements[i], stack, heap);

    dst->_size = src->_size;
}
