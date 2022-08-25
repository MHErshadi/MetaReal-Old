// MetaReal Programming Language version 1.0.0

#ifndef __TYPE__
#define __TYPE__

/* int */

typedef char int8;
typedef unsigned char uint8;

typedef short int16;
typedef unsigned short uint16;

typedef long int32;
typedef unsigned long uint32;

typedef long long int64;
typedef unsigned long long uint64;

/* decimal */

typedef float dec32;

typedef double dec64;

/* pointer */

typedef char *int8p;
typedef unsigned char *uint8p;

typedef short *int16p;
typedef unsigned short *uint16p;

typedef long *int32p;
typedef unsigned long *uint32p;

typedef long long *int64p;
typedef unsigned long long *uint64p;

typedef float *dec32p;

typedef double *dec64p;

typedef void *ptr;
typedef void **ptrp;

typedef char *str;
typedef const char *cstr;
typedef char **strp;

#endif /* __TYPE__ */
