// MetaReal Programming Language version 1.0.0
// MetaReal Float Library version 1.0.0

#ifndef __M_MFLOAT__
#define __M_MFLOAT__

#include <mint/mint.h>

struct __mfloat__
{
    dec64 _value;
};
typedef struct __mfloat__ mfloat_t[1];
typedef const struct __mfloat__ mfloat_c[1];

void set_mfloat_str(mfloat_t dst, cstr src);
void set_mfloat_mint(mfloat_t dst, mint_c src);

void copy_mfloat(mfloat_t dst, mfloat_c src);

/* */

void mfloat_add(mfloat_t op1, mfloat_t op2);
void mfloat_subtract(mfloat_t op1, mfloat_t op2);
void mfloat_multiply(mfloat_t op1, mfloat_t op2);
void mfloat_divide(mfloat_t op1, mfloat_t op2);
void mfloat_modulo(mfloat_t op1, mfloat_t op2);
void mfloat_quotient(mfloat_t op1, mfloat_t op2);
void mfloat_power(mfloat_t op1, mfloat_t op2);

void mfloat_neg(mfloat_t mfloat);

uint8 mfloat_equal(mfloat_c op1, mfloat_c op2);
uint8 mfloat_nequal(mfloat_c op1, mfloat_c op2);
uint8 mfloat_less(mfloat_c op1, mfloat_c op2);
uint8 mfloat_greater(mfloat_c op1, mfloat_c op2);
uint8 mfloat_lessequal(mfloat_c op1, mfloat_c op2);
uint8 mfloat_greaterequal(mfloat_c op1, mfloat_c op2);

#endif /* __M_MFLOAT__ */
