#ifndef _TYPE_H_
#define _TYPE_H_

#define NULL (void *)0

/* unsigned variable type */
typedef unsigned long long u64;
typedef unsigned int       u32;
typedef unsigned short     u16;
typedef unsigned char      u8;

typedef u32 size_t;

/* signed variable */
typedef long long   s64;
typedef int         s32;
typedef short       s16;
typedef char        s8;

typedef s32 ssize_t;
typedef s32 bool_t;

/* error code */
typedef enum
{
    kerr_OK = 0,
    kerr_PERM,      /* not permit */
    kerr_NOENT,     /* no such file or directory */
    kerr_INTR,      /* interrupt system call */
    kerr_IO,        /* kernel io error */
    kerr_NXIO,      /* no device or address */
    kerr_OPINV,     /* operation invalid */
    kerr_OPNSUP,    /* operation not support */
    kerr_INVARG,    /* invalid argument */
} kerr_no_t;

/* This section is used to implement printf
 * to make a variable length */
typedef s8 * va_list;

/*
 * Storage alignment properties
 */
#define  _AUPBND                (sizeof (s32) - 1)
#define  _ADNBND                (sizeof (s32) - 1)

/*
 * Variable argument list macro definitions
 */
#define _bnd(X, bnd)            (((sizeof (X)) + (bnd)) & (~(bnd)))

#define va_arg(ap, T)           (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_end(ap)              (void) 0
#define va_start(ap, A)         (void) ((ap) = (((char *) &(A)) + (_bnd (A,_AUPBND))))

#endif