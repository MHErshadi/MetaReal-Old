// MetaReal Programming Language version 1.0.0
// MetaReal Integer Library version 1.0.0

#ifndef __M_MINT__
#define __M_MINT__

#include <ctypes.h>

struct __mint__
{
    int64 _value;
};
typedef struct __mint__ mint_t[1];
typedef const struct __mint__ mint_c[1];

void set_mint_str(mint_t dst, cstr src);

void copy_mint(mint_t dst, mint_c src);

/* */

void mint_add(mint_t op1, mint_t op2);
void mint_subtract(mint_t op1, mint_t op2);
void mint_multiply(mint_t op1, mint_t op2);
void mint_modulo(mint_t op1, mint_t op2);
void mint_quotient(mint_t op1, mint_t op2);
void mint_power(mint_t op1, mint_t op2);

void mint_and(mint_t op1, mint_t op2);
void mint_ior(mint_t op1, mint_t op2);
void mint_xor(mint_t op1, mint_t op2);
void mint_lshift(mint_t op1, mint_t op2);
void mint_rshift(mint_t op1, mint_t op2);

void mint_neg(mint_t mint);
void mint_not(mint_t mint);

uint8 mint_equal(mint_c op1, mint_c op2);
uint8 mint_nequal(mint_c op1, mint_c op2);
uint8 mint_less(mint_c op1, mint_c op2);
uint8 mint_greater(mint_c op1, mint_c op2);
uint8 mint_lessequal(mint_c op1, mint_c op2);
uint8 mint_greaterequal(mint_c op1, mint_c op2);

#endif /* __M_MINT__ */
