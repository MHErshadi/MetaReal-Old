// MetaReal Programming Language version 1.0.0

#include <mrir/itpl.h>

uint8 itpl_equal(tpl_i op1, tpl_i op2)
{
    if (op1->_siz != op2->_siz)
        return 0;

    uint64 i;
    for (i = 0; i < op1->_siz; i++)
        if (!iblk_cmp(op1->_elms[i], op2->_elms[i]))
            return 0;
    return 1;
}
