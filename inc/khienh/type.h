#ifndef _TYPE_H_
#define _TYPE_H_

/* unsigned variable type */
typedef unsigned long long u64;
typedef unsigned int       u32;
typedef unsigned short     u16;
typedef unsigned char      u8;

typedef u32 size_t;

/* signed variable */
typedef long long   i64;
typedef int         i32;
typedef short       i16;
typedef char        c8;

typedef i32 ssize_t;
typedef i32 bool_t;

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

#endif