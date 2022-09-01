// MetaReal Programming Language version 1.0.0

#ifndef __M_BLOCK_STRUCT__
#define __M_BLOCK_STRUCT__

#include <ctypes.h>

struct __block__
{
    uint8 _type;
    ptr _block;

    uint8 _dtype;
    uint64 _id;

    uint8 _properties; // 0b00000021 (1 : is useful) (2 : is complex)
};
typedef struct __block__ block_t;
typedef struct __block__ *block_p;

#define SET_PROPERTIES(is_useful, is_complex, tail) 0b ## tail ## is_complex ## is_useful

#define IS_USEFUL(x) ((x) & 1)
#define IS_COMPLEX(x) ((x) >> 1 & 1)
#define IS_TRUE(x) ((x) >> 2)

#endif /* __M_BLOCK_STRUCT__ */
