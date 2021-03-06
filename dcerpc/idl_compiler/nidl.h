/*
 *
 * (c) Copyright 1989 OPEN SOFTWARE FOUNDATION, INC.
 * (c) Copyright 1989 HEWLETT-PACKARD COMPANY
 * (c) Copyright 1989 DIGITAL EQUIPMENT CORPORATION
 * To anyone who acknowledges that this file is provided "AS IS"
 * without any express or implied warranty:
 *                 permission to use, copy, modify, and distribute this
 * file for any purpose is hereby granted without fee, provided that
 * the above copyright notices and this notice appears in all source
 * code copies, and that none of the names of Open Software
 * Foundation, Inc., Hewlett-Packard Company, or Digital Equipment
 * Corporation be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission.  Neither Open Software Foundation, Inc., Hewlett-
 * Packard Company, nor Digital Equipment Corporation makes any
 * representations about the suitability of this software for any
 * purpose.
 *
 */
/*
**
**  NAME
**
**      NIDL.H
**
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**      Mandatory header file containing all system dependent
**      includes and common macros used by the IDL compiler.
**
**  VERSION: DCE 1.0
*/

#ifndef NIDLH_INCL
#define NIDLH_INCL

#define NIDLBASE_H

#ifdef _WIN32

#ifndef inline
#define inline __inline
#endif

#define YYMALLOC
#define YYFREE

#define __attribute__(x)
#define snprintf _snprintf
#define fileno _fileno

#endif /* ifdef _WIN32 */

#ifndef HAVE_CONFIG_H
#define HAVE_CONFIG_H
#endif
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Base include files needed by all IDL compiler modules */

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#else
typedef enum { false = 0, true = 1 } bool;
#ifndef true
#define true true
#endif
#ifndef false
#define false false
#endif
#endif

#ifdef DUMPERS
# define DEBUG_VERBOSE 1
#endif

#if defined(__STDC__) || defined(_WIN32)
#   include <stdlib.h>
#   ifndef CHAR_BIT
#       include <limits.h>  /* Bring in limits.h if not cacaded in yet */
#   endif
#else /* prototypes that normally come from stdlib.h */
    extern void *malloc();
    extern void free();
    extern char *getenv();
    extern int atoi();
    extern double atof();
    extern long atol();
#endif
#ifdef __STDC__
#  include <assert.h>
#else
#  define assert(ex) if (ex) ;
#endif
#include <sysdep.h>

/*
 * some generally useful types and macros
 */

typedef unsigned char       unsigned8;
typedef unsigned short int  unsigned16;
typedef unsigned long int   unsigned32;

typedef unsigned8 boolean;
#ifndef TRUE
#define TRUE true
#define FALSE false
#endif

/*
 * IDL's model of the info in a UUID (see dce_uuid_t in nbase.idl)
 */

typedef struct
{
    unsigned32      time_low;
    unsigned16      time_mid;
    unsigned16      time_hi_and_version;
    unsigned8       clock_seq_hi_and_reserved;
    unsigned8       clock_seq_low;
    unsigned8       node[6];
} nidl_uuid_t;

/*
 * Include files needed by the remaining supplied definitions in this file.
 * These need to be here, since they depend on the above definitions.
 */

#include <errors.h>
#include <nidlmsg.h>

/* Language enum.  Here for lack of any place else. */
typedef enum {
    lang_ada_k,
    lang_basic_k,
    lang_c_k,
    lang_cobol_k,
    lang_fortran_k,
    lang_pascal_k
} language_k_t;

/*
 * Macro jackets for each of the C memory management routines.
 * The macros guarantee that control will not return to the caller without
 * memory; therefore the call site doesn't have to test.
 */

/**
 * Returns pointer to a new allocated object of the specified type.
 * It behaves like C++ new. The returned pointer is already correctly typed to
 * type *. So you should not cast it. Let the compiler detect any errors
 * instead of casting.
 *
 * The the returned memory is cleared.
 *
 * @param type of the object that should be allocated
 * @return a valid pointer correctly typed
 */
static inline void *__NEW(size_t len)
{
    void *retptr = calloc(1, len);
    if (!retptr) error(NIDL_OUTOFMEM);
    return retptr;
}
#define NEW(type)	(type *) __NEW(sizeof(type))


/**
 * Allocates and returns pointer to a vector of objects.
 * It behaves like C++ new. The returned pointer is already correctly typed to
 * type *. So you should not cast it. Let the compiler detect any errors
 * instead of casting.
 *
 * The the returned memory is cleared.
 *
 * @notice size is the _number_ of objects to be allocated
 *
 * @param type of the object that should be allocated
 * @param size number of objects to be allocated
 * @return a valid pointer correctly typed
 */
static inline void *__NEW_VEC(size_t size, size_t len)
{
    void *retptr = calloc(size, len);
    if (!retptr) error(NIDL_OUTOFMEM);
    return retptr;
}
#define NEW_VEC(type, size) (type *)  __NEW_VEC(sizeof(type), size)


/**
 * Reallocates prevoiusly allocated memory area and returns the pointer to it.
 * It behaves like C++ new and C realloc. The returned pointer is already
 * correctly typed to typeof(pointer). So you should not cast it. Let the compiler
 * detect any errors instead of casting.
 *
 * The the returned memory is _not_ cleared.
 *
 * @notice size is the _number_ of objects to be allocated
 *
 * @param pointer points to previously allocated vector
 * @param size number of objects to be allocated
 * @return a valid pointer correctly typed
 */
static inline void *__RENEW(void *ptr, size_t size)
{
    void *retptr = realloc(ptr, size);
    if (!retptr) error(NIDL_OUTOFMEM);
    return retptr;

}
#define RENEW(pointer, type_size, size) __RENEW(pointer, (type_size) * (size))


/**
 * Allocates some memory area.
 * The returned pointer is always valid. Do not use this function. The better
 * sollution is to use one of the above *NEW* function which return already
 * typed pointers.
 *
 * @param size of the area to be allocated
 * @return a valid pointer to the allocated memory
 */
static inline void *__MALLOC(size_t size)
{
    void *retptr = calloc(1, size);
    if (!retptr) error(NIDL_OUTOFMEM);
    return retptr;
}
#define MALLOC(size) __MALLOC(size)

/**
 * Frees memory allocated with one of the above functions
 *
 * @param pointer to the memory to be freed
 */
#define FREE(pointer) free (pointer);


/*
 * Enable YYDEBUG, and ASSERTION checking, if DUMPERS is defined
 */
#ifdef DUMPERS
#  define YYDEBUG 1
   /* If ASSERTION expression is FALSE, then issue warning */
#  define ASSERTION(x) if (!(x)) warning(NIDL_INTERNAL_ERROR, __FILE__, __LINE__)
#else
#  define ASSERTION(x) do {;} while (0);
#endif

#endif
