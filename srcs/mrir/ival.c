// MetaReal Programming Language version 1.0.0

#include <stdlib.h>

#include <mrir/ival.h>

#define UINT64_MAX "18446744073709551615"

uint64 conv_int(cstr val, uint64 len, uint8p flg)
{
    for (; *val == '0'; val++, len--);
    if (!len)
        return 0;

    if (len > 20)
    {
        *flg = 1;
        return 0;
    }

    if (len < 20)
    {
        uint64 res = *val - '0';

        uint64 i;
        for (i = 1; i < len; i++)
            res = (res * 10) + val[i] - '0';

        return res;
    }

    if (*val > '1')
    {
        *flg = 1;
        return 0;
    }

    uint64 res = *val - '0';

    uint64 i;
    for (i = 1; i < 20; i++)
    {
        if (val[i] > UINT64_MAX[i])
        {
            *flg = 1;
            return 0;
        }

        res = (res * 10) + val[i] - '0';
    }

    return res;
}
