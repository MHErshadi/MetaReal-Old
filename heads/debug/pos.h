// MetaReal Programming Language version 1.0.0

#ifndef __M_POS__
#define __M_POS__

#include <ctypes.h>

struct __pos__
{
    uint64 _index;
    uint64 _line;

    cstr _file_name;
};
typedef struct __pos__ pos_t;
typedef struct __pos__ *pos_p;

pos_t pos_set(uint64 index, uint64 line, cstr file_name);

#define pos_jump(p)  \
    do              \
    {               \
        p._index++; \
        p._line++;  \
    } while (0)

#define posp_jump(p)  \
    do               \
    {                \
        p->_index++; \
        p->_line++;  \
    } while (0)

#endif /* __M_POS__ */
