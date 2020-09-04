/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
********************************************************************************

********************************************************************************
NOTE: this file is auto-generated from: https://github.com/facil-io/cstl
***************************************************************************** */

/** ****************************************************************************
# facil.io's C STL - Simple (type) Template Library

This file contains macros that create generic / common core types, such as:

* Linked Lists - defined by `FIO_LIST_NAME`

* Dynamic Arrays - defined by `FIO_ARRAY_NAME`

* Hash Maps / Sets - defined by `FIO_MAP_NAME`

* Binary Safe Dynamic Strings - defined by `FIO_STR_NAME` or `FIO_STR_SMALL`

* Reference counting / Type wrapper - defined by `FIO_REF_NAME` (adds atomic)

* Pointer Tagging for Types - defined by `FIO_PTR_TAG(p)`/`FIO_PTR_UNTAG(p)`

* Soft / Dynamic Types (FIOBJ) - defined by `FIO_FIOBJ`


This file also contains common helper macros / primitives, such as:

* Macro Stringifier - `FIO_MACRO2STR(macro)`

* Version Macros - i.e., `FIO_VERSION_MAJOR` / `FIO_VERSION_STRING`

* Pointer Math - i.e., `FIO_PTR_MATH_ADD` / `FIO_PTR_FROM_FIELD`

* Memory Allocation Macros - i.e., `FIO_MEM_CALLOC`

* Security Related macros - i.e., `FIO_MEM_STACK_WIPE`

* String Information Helper Type - `fio_str_info_s` / `FIO_STR_INFO_IS_EQ`

* Naming Macros - i.e., `FIO_NAME` / `FIO_NAME2` / `FIO_NAME_BL`

* Sleep / Thread Scheduling Macros - i.e., `FIO_THREAD_RESCHEDULE`

* Logging and Assertion (no heap allocation) - defined by `FIO_LOG`

* Atomic add/subtract/replace - defined by `FIO_ATOMIC`

* Bit-Byte Operations - defined by `FIO_BITWISE` and `FIO_BITMAP` (adds atomic)

* Data Hashing (using Risky Hash) - defined by `FIO_RISKY_HASH`

* Psedo Random Generation - defined by `FIO_RAND`

* String / Number conversion - defined by `FIO_ATOL`

* Time Helpers - defined by `FIO_TIME`

* Task / Timer Queues (Event Loop Engine) - defined by `FIO_QUEUE`

* Command Line Interface helpers - defined by `FIO_CLI`

* Custom Memory Allocation - defined by `FIO_MALLOC`, updates `FIO_MEM_CALLOC`

* Custom JSON Parser - defined by `FIO_JSON`

However, this file does very little unless specifically requested.

To make sure this file defines a specific macro or type, it's macro should be
set.

In addition, if the `FIO_TEST_CSTL` macro is defined, the self-testing function
`fio_test_dynamic_types()` will be defined. the `fio_test_dynamic_types`
function will test the functionality of this file and, as consequence, will
define all available macros.

**Notes**:

- To make this file usable for kernel authoring, the `include` statements should
be reviewed.

- To make these functions safe for kernel authoring, the `FIO_MEM_CALLOC` /
`FIO_MEM_FREE` / `FIO_MEM_REALLOC` macros should be (re)-defined.

  These macros default to using the `calloc` and `free` functions calls. If
  `FIO_MALLOC` was defined, these macros will default to the custom memory
  allocator.

- To make the custom memory allocator safe for kernel authoring, the
  `FIO_MEM_PAGE_ALLOC`, `FIO_MEM_PAGE_REALLOC` and `FIO_MEM_PAGE_FREE` macros
  should be redefined. These macros default to using `mmap` and `munmap` (on
  linux, also `mremap`).

- The functions defined using this file default to `static` or `static
  inline`.

  To create an externally visible API, define the `FIO_EXTERN`. Define the
  `FIO_EXTERN_COMPLETE` macro to include the API's implementation as well.

- To implement a library style version guard, define the `FIO_VERSION_GUARD`
macro in a single translation unit (.c file) **before** including this STL
library for the first time.

***************************************************************************** */

/* *****************************************************************************
C++ extern start
***************************************************************************** */
/* support C++ */
#ifdef __cplusplus
extern "C" {
/* C++ keyword was deprecated */
#ifndef register
#define register
#endif
/* C keyword - unavailable in C++ */
#ifndef restrict
#define restrict
#endif
#endif

/* *****************************************************************************










                            Constants (included once)










***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H
#define H___FIO_CSTL_INCLUDE_ONCE_H

/* *****************************************************************************
Basic macros and included files
***************************************************************************** */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#if !defined(__GNUC__) && !defined(__clang__) && !defined(GNUC_BYPASS)
#define __attribute__(...)
#define __has_include(...) 0
#define __has_builtin(...) 0
#define GNUC_BYPASS 1
#elif !defined(__clang__) && !defined(__has_builtin)
/* E.g: GCC < 6.0 doesn't support __has_builtin */
#define __has_builtin(...) 0
#define GNUC_BYPASS 1
#endif

#ifndef __has_include
#define __has_include(...) 0
#define GNUC_BYPASS 1
#endif

#if defined(__GNUC__) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 5))
/* GCC < 4.5 doesn't support deprecation reason string */
#define DEPRECATED(reason) __attribute__((deprecated))
#else
#define DEPRECATED(reason) __attribute__((deprecated(reason)))
#endif

#if !defined(__clang__) && !defined(__GNUC__)
#define __thread _Thread_value
#endif

#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(__unix__) || defined(__linux__) || defined(__APPLE__) ||           \
    defined(__CYGWIN__)
#define FIO_HAVE_UNIX_TOOLS 1
#include <sys/param.h>
#endif

#if FIO_UNALIGNED_ACCESS && (__amd64 || __amd64__ || __x86_64 || __x86_64__)
#define FIO_UNALIGNED_MEMORY_ACCESS_ENABLED 1
#else
#define FIO_UNALIGNED_MEMORY_ACCESS_ENABLED 0
#endif

/* *****************************************************************************
Macro Stringifier
***************************************************************************** */

#ifndef FIO_MACRO2STR
#define FIO_MACRO2STR_STEP2(macro) #macro
/** Converts a macro's content to a string literal. */
#define FIO_MACRO2STR(macro) FIO_MACRO2STR_STEP2(macro)
#endif

/* *****************************************************************************
Version Macros

The facil.io C STL library follows [semantic versioning](https://semver.org) and
supports macros that will help detect and validate it's version.
***************************************************************************** */

/** MAJOR version: API/ABI breaking changes. */
#define FIO_VERSION_MAJOR 0
/** MINOR version: Deprecation, or significant features added. May break ABI. */
#define FIO_VERSION_MINOR 8
/** PATCH version: Bug fixes, minor features may be added. */
#define FIO_VERSION_PATCH 0
/** BETA version: pre-version development marker. Nothing is stable. */
#define FIO_VERSION_BETA 1

#if FIO_VERSION_BETA
/** Version as a String literal (MACRO). */
#define FIO_VERSION_STRING                                                     \
  FIO_MACRO2STR(FIO_VERSION_MAJOR)                                             \
  "." FIO_MACRO2STR(FIO_VERSION_MINOR) "." FIO_MACRO2STR(                      \
      FIO_VERSION_PATCH) ".beta" FIO_MACRO2STR(FIO_VERSION_BETA)
#else
/** Version as a String literal (MACRO). */
#define FIO_VERSION_STRING                                                     \
  FIO_MACRO2STR(FIO_VERSION_MAJOR)                                             \
  "." FIO_MACRO2STR(FIO_VERSION_MINOR) "." FIO_MACRO2STR(FIO_VERSION_PATCH)
#endif

/** If implemented, returns the major version number. */
size_t fio_version_major(void);
/** If implemented, returns the minor version number. */
size_t fio_version_minor(void);
/** If implemented, returns the patch version number. */
size_t fio_version_patch(void);
/** If implemented, returns the beta version number. */
size_t fio_version_beta(void);
/** If implemented, returns the version number as a string. */
char *fio_version_string(void);

#define FIO_VERSION_VALIDATE()                                                 \
  FIO_ASSERT(fio_version_major() == FIO_VERSION_MAJOR &&                       \
                 fio_version_minor() == FIO_VERSION_MINOR &&                   \
                 fio_version_patch() == FIO_VERSION_PATCH &&                   \
                 fio_version_beta() == FIO_VERSION_BETA,                       \
             "facil.io version mismatch, not %s",                              \
             fio_version_string())

/**
 * To implement the fio_version_* functions and FIO_VERSION_VALIDATE guard, the
 * `FIO_VERSION_GUARD` must be defined (only) once per application / library.
 */
#ifdef FIO_VERSION_GUARD
size_t __attribute__((weak)) fio_version_major(void) {
  return FIO_VERSION_MAJOR;
}
size_t __attribute__((weak)) fio_version_minor(void) {
  return FIO_VERSION_MINOR;
}
size_t __attribute__((weak)) fio_version_patch(void) {
  return FIO_VERSION_PATCH;
}
size_t __attribute__((weak)) fio_version_beta(void) { return FIO_VERSION_BETA; }
char *__attribute__((weak)) fio_version_string(void) {
  return FIO_VERSION_STRING;
}
#undef FIO_VERSION_GUARD
#endif /* FIO_VERSION_GUARD */

#if !defined(FIO_NO_COOKIE)
/** If implemented, does stuff. */
void __attribute__((weak)) fio___(void) {
  volatile uint8_t tmp[] =
      "\xA8\x94\x9A\x10\x99\x92\x93\x96\x9C\x1D\x96\x9F\x10\x9C\x96\x91\xB1\x92"
      "\xB1\xB6\x10\xBB\x92\xB3\x10\x92\xBA\xB8\x94\x9F\xB1\x9A\x98\x10\x91\xB6"
      "\x10\x81\x9F\x92\xB5\x10\xA3\x9A\x9B\x9A\xB9\x1D\x05\x10\x10\x10\x10\x8C"
      "\x96\xB9\x9A\x10\x9C\x9F\x9D\x9B\x10\x92\x9D\x98\x10\xB0\xB1\x9F\xB3\xB0"
      "\x9A\xB1\x1D";
  for (size_t i = 0; tmp[i]; ++i) {
    tmp[i] = ((tmp[i] & 0x55) << 1) | ((tmp[i] & 0xaa) >> 1);
  }
  fprintf(stderr, "%s\n", tmp);
}
#endif

/* *****************************************************************************
Pointer Math
***************************************************************************** */

/** Masks a pointer's left-most bits, returning the right bits. */
#define FIO_PTR_MATH_LMASK(T_type, ptr, bits)                                  \
  ((T_type *)((uintptr_t)(ptr) & (((uintptr_t)1 << (bits)) - 1)))

/** Masks a pointer's right-most bits, returning the left bits. */
#define FIO_PTR_MATH_RMASK(T_type, ptr, bits)                                  \
  ((T_type *)((uintptr_t)(ptr) & ((~(uintptr_t)0) << (bits))))

/** Add offset bytes to pointer, updating the pointer's type. */
#define FIO_PTR_MATH_ADD(T_type, ptr, offset)                                  \
  ((T_type *)((uintptr_t)(ptr) + (uintptr_t)(offset)))

/** Subtract X bytes from pointer, updating the pointer's type. */
#define FIO_PTR_MATH_SUB(T_type, ptr, offset)                                  \
  ((T_type *)((uintptr_t)(ptr) - (uintptr_t)(offset)))

/** Find the root object (of a struct) from it's field. */
#define FIO_PTR_FROM_FIELD(T_type, field, ptr)                                 \
  FIO_PTR_MATH_SUB(T_type, ptr, (&((T_type *)0)->field))

/* *****************************************************************************
Memory allocation macros
***************************************************************************** */
#ifndef FIO_MEM_CALLOC
/** Allocates size X units of bytes, where all bytes equal zero. */
#define FIO_MEM_CALLOC(size, units) calloc((size), (units))
#endif

#ifndef FIO_MEM_REALLOC
/** Reallocates memory, copying (at least) `copy_len` if necessary. */
#define FIO_MEM_REALLOC(ptr, old_size, new_size, copy_len)                     \
  realloc((ptr), (new_size))
#endif

#ifndef FIO_MEM_FREE
/** Frees allocated memory. */
#define FIO_MEM_FREE(ptr, size) free((ptr))
#endif

#ifndef FIO_MEM_INTERNAL_MALLOC
#define FIO_MEM_INTERNAL_MALLOC 0
#endif

/* *****************************************************************************
Security Related macros
***************************************************************************** */
#define FIO_MEM_STACK_WIPE(pages)                                              \
  do {                                                                         \
    volatile char stack_mem[(pages) << 12] = {0};                              \
    (void)stack_mem;                                                           \
  } while (0)

/* *****************************************************************************
String Information Helper Type
***************************************************************************** */

/** An information type for reporting the string's state. */
typedef struct fio_str_info_s {
  /** The string's buffer (pointer to first byte) or NULL on error. */
  char *buf;
  /** The string's length, if any. */
  size_t len;
  /** The buffer's capacity. Zero (0) indicates the buffer is read-only. */
  size_t capa;
} fio_str_info_s;

/** Compares two `fio_str_info_s` objects for content equality. */
#define FIO_STR_INFO_IS_EQ(s1, s2)                                             \
  ((s1).len == (s2).len && (!(s1).len || (s1).buf == (s2).buf ||               \
                            !memcmp((s1).buf, (s2).buf, (s1).len)))

/* *****************************************************************************
Linked Lists Persistent Macros and Types
***************************************************************************** */

/** A common linked list node type. */
typedef struct fio___list_node_s {
  struct fio___list_node_s *next;
  struct fio___list_node_s *prev;
} fio___list_node_s;

/** A linked list node type */
#define FIO_LIST_NODE fio___list_node_s
/** A linked list head type */
#define FIO_LIST_HEAD fio___list_node_s

/** Allows initialization of FIO_LIST_HEAD objects. */
#define FIO_LIST_INIT(obj)                                                     \
  { .next = &(obj), .prev = &(obj) }

#ifndef FIO_LIST_EACH
/** Loops through every node in the linked list except the head. */
#define FIO_LIST_EACH(type, node_name, head, pos)                              \
  for (type *pos = FIO_PTR_FROM_FIELD(type, node_name, (head)->next),          \
            *next____p_ls =                                                    \
                FIO_PTR_FROM_FIELD(type, node_name, (head)->next->next);       \
       pos != FIO_PTR_FROM_FIELD(type, node_name, (head));                     \
       (pos = next____p_ls),                                                   \
            (next____p_ls = FIO_PTR_FROM_FIELD(                                \
                 type, node_name, next____p_ls->node_name.next)))
#endif

/* *****************************************************************************
Naming Macros
***************************************************************************** */

/* Used for naming functions and types */
#define FIO_NAME_FROM_MACRO_STEP2(prefix, postfix, div) prefix##div##postfix
#define FIO_NAME_FROM_MACRO_STEP1(prefix, postfix, div)                        \
  FIO_NAME_FROM_MACRO_STEP2(prefix, postfix, div)

/** Used for naming functions and variables resulting in: prefix_postfix */
#define FIO_NAME(prefix, postfix) FIO_NAME_FROM_MACRO_STEP1(prefix, postfix, _)

/** Sets naming convention for conversion functions, i.e.: foo2bar */
#define FIO_NAME2(prefix, postfix) FIO_NAME_FROM_MACRO_STEP1(prefix, postfix, 2)

/** Sets naming convention for boolean testing functions, i.e.: foo_is_true */
#define FIO_NAME_BL(prefix, postfix)                                           \
  FIO_NAME_FROM_MACRO_STEP1(prefix, postfix, _is_)

/** Used internally to name test functions. */
#define FIO_NAME_TEST(prefix, postfix)                                         \
  FIO_NAME(fio___test, FIO_NAME(prefix, postfix))

/* *****************************************************************************
Sleep / Thread Scheduling Macros
***************************************************************************** */

#ifndef FIO_THREAD_RESCHEDULE
/**
 * Reschedules the thread by calling nanosleeps for a sinlge nano-second.
 *
 * In practice, the thread will probably sleep for 60ns or more.
 */
#define FIO_THREAD_RESCHEDULE()                                                \
  do {                                                                         \
    const struct timespec tm = {.tv_sec = 0, .tv_nsec = (long)1L};             \
    nanosleep(&tm, (struct timespec *)NULL);                                   \
  } while (0)
#endif

#ifndef FIO_THREAD_WAIT
/**
 * Calls nonsleep with the requested nano-second count.
 */
#define FIO_THREAD_WAIT(nano_sec)                                              \
  do {                                                                         \
    const struct timespec tm = {.tv_sec = (time_t)((nano_sec) / 1000000000),   \
                                .tv_nsec = ((long)(nano_sec) % 1000000000)};   \
    nanosleep(&tm, (struct timespec *)NULL);                                   \
  } while (0)
#endif

/* *****************************************************************************
Miscellaneous helper macros
***************************************************************************** */

/* avoid printing a full / nested path when __FILE_NAME__ is available */
#ifdef __FILE_NAME__
#define FIO__FILE__ __FILE_NAME__
#else
#define FIO__FILE__ __FILE__
#endif

/** An empty macro, adding white space. Used to avoid function like macros. */
#define FIO_NOOP
/* allow logging to quitely fail unless enabled */
#define FIO_LOG_DEBUG(...)
#define FIO_LOG_DEBUG2(...)
#define FIO_LOG_INFO(...)
#define FIO_LOG_WARNING(...)
#define FIO_LOG_ERROR(...)
#define FIO_LOG_SECURITY(...)
#define FIO_LOG_FATAL(...)
#define FIO_LOG2STDERR(...)
#define FIO_LOG2STDERR2(...)

#ifndef FIO_LOG_LENGTH_LIMIT
/** Defines a point at which logging truncates (limited by stack memory) */
#define FIO_LOG_LENGTH_LIMIT 1024
#endif

// clang-format off
/* Asserts a condition is true, or kills the application using SIGINT. */
#define FIO_ASSERT(cond, ...)                                                  \
  if (!(cond)) {                                                               \
    FIO_LOG_FATAL("(" FIO__FILE__ ":" FIO_MACRO2STR(__LINE__) ") " __VA_ARGS__);  \
    perror("     errno");                                                      \
    kill(0, SIGINT);                                                           \
    exit(-1);                                                                  \
  }

#ifndef FIO_ASSERT_ALLOC
/** Tests for an allocation failure. The behavior can be overridden. */
#define FIO_ASSERT_ALLOC(ptr)                                                  \
  if (!(ptr)) {                                                                \
    FIO_LOG_FATAL("memory allocation error " FIO__FILE__                          \
                  ":" FIO_MACRO2STR(__LINE__));                                \
    kill(0, SIGINT);                                                           \
    exit(-1);                                                                  \
  }
#endif
// clang-format on

#ifdef DEBUG
/** If `DEBUG` is defined, acts as `FIO_ASSERT`, otherwise a NOOP. */
#define FIO_ASSERT_DEBUG(cond, ...) FIO_ASSERT(cond, __VA_ARGS__)
#else
#define FIO_ASSERT_DEBUG(...)
#endif

/** Marks a function as `static`, `inline` and possibly unused. */
#define FIO_IFUNC static inline __attribute__((unused))

/** Marks a function as `static` and possibly unused. */
#define FIO_SFUNC static __attribute__((unused))

/** Marks a function as weak */
#define FIO_WEAK __attribute__((weak))

/* *****************************************************************************
End persistent segment (end include-once guard)
***************************************************************************** */
#endif /* H___FIO_CSTL_INCLUDE_ONCE_H */

/* *****************************************************************************










                          Common internal Macros










***************************************************************************** */

/* *****************************************************************************
Common macros
***************************************************************************** */
#ifndef SFUNC_ /* if we aren't in a recursive #include statement */

#ifdef FIO_EXTERN
#define SFUNC_
#define IFUNC_

#else /* !FIO_EXTERN */
#undef SFUNC
#undef IFUNC
#define SFUNC_ static __attribute__((unused))
#define IFUNC_ static inline __attribute__((unused))
#ifndef FIO_EXTERN_COMPLETE /* force implementation, emitting static data */
#define FIO_EXTERN_COMPLETE 2
#endif /* FIO_EXTERN_COMPLETE */
#endif /* FIO_EXTERN */

#undef SFUNC
#undef IFUNC
#define SFUNC SFUNC_
#define IFUNC IFUNC_

#ifndef FIO_PTR_TAG
/**
 * Supports embedded pointer tagging / untagging for the included types.
 *
 * Should resolve to a tagged pointer value. i.e.: ((uintptr_t)(p) | 1)
 */
#define FIO_PTR_TAG(p) (p)
#endif

#ifndef FIO_PTR_UNTAG
/**
 * Supports embedded pointer tagging / untagging for the included types.
 *
 * Should resolve to an untagged pointer value. i.e.: ((uintptr_t)(p) | ~1UL)
 */
#define FIO_PTR_UNTAG(p) (p)
#endif

/**
 * If FIO_PTR_TAG_TYPE is defined, then functions returning a type's pointer
 * will return a pointer of the specified type instead.
 */
#ifndef FIO_PTR_TAG_TYPE
#endif

/**
 * If FIO_PTR_TAG_VALIDATE is defined, tagging will be verified before executing
 * any code.
 */
#ifdef FIO_PTR_TAG_VALIDATE
#define FIO_PTR_TAG_VALID_OR_RETURN(tagged_ptr, value)                         \
  do {                                                                         \
    if (!(FIO_PTR_TAG_VALIDATE(tagged_ptr))) {                                 \
      FIO_LOG_DEBUG("pointer tag (type) mismatch in function call.");          \
      return (value);                                                          \
    }                                                                          \
  } while (0)
#define FIO_PTR_TAG_VALID_OR_RETURN_VOID(tagged_ptr)                           \
  do {                                                                         \
    if (!(FIO_PTR_TAG_VALIDATE(tagged_ptr))) {                                 \
      FIO_LOG_DEBUG("pointer tag (type) mismatch in function call.");          \
      return;                                                                  \
    }                                                                          \
  } while (0)
#define FIO_PTR_TAG_VALID_OR_GOTO(tagged_ptr, lable)                           \
  do {                                                                         \
    if (!(FIO_PTR_TAG_VALIDATE(tagged_ptr))) {                                 \
      /* Log error since GOTO indicates cleanup or other side-effects. */      \
      FIO_LOG_ERROR("(" FIO__FILE__ ":" FIO_MACRO2STR(                         \
          __LINE__) ") pointer tag (type) mismatch in function call.");        \
      goto lable;                                                              \
    }                                                                          \
  } while (0)
#else
#define FIO_PTR_TAG_VALIDATE(tagged_ptr) 1
#define FIO_PTR_TAG_VALID_OR_RETURN(tagged_ptr, value)
#define FIO_PTR_TAG_VALID_OR_RETURN_VOID(tagged_ptr)
#define FIO_PTR_TAG_VALID_OR_GOTO(tagged_ptr, lable)                           \
  while (0) {                                                                  \
    goto lable;                                                                \
  }
#endif

#else /* SFUNC_ - internal helper types are `static` */
#undef SFUNC
#undef IFUNC
#define SFUNC FIO_SFUNC
#define IFUNC FIO_IFUNC
#endif /* SFUNC_ vs FIO_STL_KEEP__*/
/* *****************************************************************************






                          Internal Dependencies






***************************************************************************** */

/* FIO_LOCK2 dependencies */
#ifdef FIO_LOCK2
#ifndef FIO_ATOMIC
#define FIO_ATOMIC
#endif
#endif /* FIO_LOCK2 */

/* FIO_MALLOC dependencies */
#ifdef FIO_MALLOC
#ifndef FIO_LOG
#define FIO_LOG
#endif
#ifndef FIO_ATOMIC
#define FIO_ATOMIC
#endif
#endif /* FIO_MALLOC */

/* FIO_BITMAP dependencies */
#ifdef FIO_BITMAP
#ifndef FIO_ATOMIC
#define FIO_ATOMIC
#endif
#endif /* FIO_BITMAP */

/* FIO_REF_NAME dependencies */
#ifdef FIO_REF_NAME
#ifndef FIO_ATOMIC
#define FIO_ATOMIC
#endif
#endif /* FIO_REF_NAME */

/* FIO_RAND dependencies */
#ifdef FIO_RAND
#ifndef FIO_BITWISE
#define FIO_BITWISE
#endif
#ifndef FIO_RISKY_HASH
#define FIO_RISKY_HASH
#endif
#ifndef FIO_TIME
#define FIO_TIME
#endif
#endif /* FIO_RAND */

/* FIO_STR_NAME / FIO_STR_SMALL dependencies */
#if defined(FIO_STR_NAME) || defined(FIO_STR_SMALL)
#ifndef FIO_ATOL
#define FIO_ATOL
#endif
#ifndef FIO_BITWISE
#define FIO_BITWISE
#endif
#ifndef FIO_RISKY_HASH
#define FIO_RISKY_HASH
#endif
#endif /* FIO_STR_NAME */

/* FIO_QUEUE dependencies */
#ifdef FIO_QUEUE
#ifndef FIO_TIME
#define FIO_TIME
#endif
#ifndef FIO_ATOL
#define FIO_ATOL
#endif
#ifndef FIO_ATOMIC
#define FIO_ATOMIC
#endif
#endif /* FIO_QUEUE */

/* FIO_TIME dependencies */
#ifdef FIO_TIME
#ifndef FIO_ATOL
#define FIO_ATOL
#endif
#endif /* FIO_TIME */

/* FIO_CLI dependencies */
#ifdef FIO_CLI
#ifndef FIO_ATOL
#define FIO_ATOL
#endif
#ifndef FIO_RISKY_HASH
#define FIO_RISKY_HASH
#endif
#ifndef FIO_BITWISE
#define FIO_BITWISE
#endif
#endif /* FIO_CLI */

/* FIO_JSON dependencies */
#ifdef FIO_JSON
#ifndef FIO_ATOL
#define FIO_ATOL
#endif
#ifndef FIO_BITMAP
#define FIO_BITMAP
#endif
#ifndef FIO_ATOMIC
#define FIO_ATOMIC
#endif
#endif /* FIO_JSON */

/* FIO_RISKY_HASH dependencies */
#ifdef FIO_RISKY_HASH
#ifndef FIO_BITWISE
#define FIO_BITWISE
#endif
#endif /* FIO_RISKY_HASH */
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************










                                  Logging





Use:

```c
FIO_LOG2STDERR("message.") // => message.
FIO_LOG_LEVEL = FIO_LOG_LEVEL_WARNING; // set dynamic logging level
FIO_LOG_INFO("message"); // => [no output, exceeds logging level]
int i = 3;
FIO_LOG_WARNING("number invalid: %d", i); // => WARNING: number invalid: 3
```

***************************************************************************** */

/**
 * Enables logging macros that avoid heap memory allocations
 */
#if !defined(FIO_LOG_PRINT__) && defined(FIO_LOG)

#if FIO_LOG_LENGTH_LIMIT > 128
#define FIO_LOG____LENGTH_ON_STACK FIO_LOG_LENGTH_LIMIT
#define FIO_LOG____LENGTH_BORDER (FIO_LOG_LENGTH_LIMIT - 32)
#else
#define FIO_LOG____LENGTH_ON_STACK (FIO_LOG_LENGTH_LIMIT + 32)
#define FIO_LOG____LENGTH_BORDER FIO_LOG_LENGTH_LIMIT
#endif

#undef FIO_LOG2STDERR

#pragma weak FIO_LOG2STDERR
__attribute__((format(printf, 1, 0), weak)) void
FIO_LOG2STDERR(const char *format, ...) {
  char tmp___log[FIO_LOG____LENGTH_ON_STACK];
  va_list argv;
  va_start(argv, format);
  int len___log = vsnprintf(tmp___log, FIO_LOG_LENGTH_LIMIT - 2, format, argv);
  va_end(argv);
  if (len___log <= 0 || len___log >= FIO_LOG_LENGTH_LIMIT - 2) {
    if (len___log >= FIO_LOG_LENGTH_LIMIT - 2) {
      memcpy(tmp___log + FIO_LOG____LENGTH_BORDER,
             "...\n\tWARNING: TRUNCATED!",
             24);
      len___log = FIO_LOG____LENGTH_BORDER + 24;
    } else {
      fwrite("\x1B[1mERROR\x1B[0m: log output error (can't write).\n",
             39,
             1,
             stderr);
      return;
    }
  }
  tmp___log[len___log++] = '\n';
  tmp___log[len___log] = '0';
  fwrite(tmp___log, len___log, 1, stderr);
}
#undef FIO_LOG____LENGTH_ON_STACK
#undef FIO_LOG____LENGTH_BORDER

// clang-format off
#undef FIO_LOG2STDERR2
#define FIO_LOG2STDERR2(...) FIO_LOG2STDERR("(" FIO__FILE__ ":" FIO_MACRO2STR(__LINE__) "): " __VA_ARGS__)
// clang-format on

/** Logging level of zero (no logging). */
#define FIO_LOG_LEVEL_NONE 0
/** Log fatal errors. */
#define FIO_LOG_LEVEL_FATAL 1
/** Log errors and fatal errors. */
#define FIO_LOG_LEVEL_ERROR 2
/** Log warnings, errors and fatal errors. */
#define FIO_LOG_LEVEL_WARNING 3
/** Log every message (info, warnings, errors and fatal errors). */
#define FIO_LOG_LEVEL_INFO 4
/** Log everything, including debug messages. */
#define FIO_LOG_LEVEL_DEBUG 5

/** The logging level */
#ifndef FIO_LOG_LEVEL_DEFAULT
#if DEBUG
#define FIO_LOG_LEVEL_DEFAULT FIO_LOG_LEVEL_DEBUG
#else
#define FIO_LOG_LEVEL_DEFAULT FIO_LOG_LEVEL_INFO
#endif
#endif
int __attribute__((weak)) FIO_LOG_LEVEL = FIO_LOG_LEVEL_DEFAULT;

#define FIO_LOG_PRINT__(level, ...)                                            \
  do {                                                                         \
    if (level <= FIO_LOG_LEVEL)                                                \
      FIO_LOG2STDERR(__VA_ARGS__);                                             \
  } while (0)

// clang-format off
#undef FIO_LOG_DEBUG
#define FIO_LOG_DEBUG(...)   FIO_LOG_PRINT__(FIO_LOG_LEVEL_DEBUG,"DEBUG:    (" FIO__FILE__ ":" FIO_MACRO2STR(__LINE__) ") " __VA_ARGS__)
#undef FIO_LOG_DEBUG2
#define FIO_LOG_DEBUG2(...)  FIO_LOG_PRINT__(FIO_LOG_LEVEL_DEBUG, "DEBUG:    " __VA_ARGS__)
#undef FIO_LOG_INFO
#define FIO_LOG_INFO(...)    FIO_LOG_PRINT__(FIO_LOG_LEVEL_INFO, "INFO:     " __VA_ARGS__)
#undef FIO_LOG_WARNING
#define FIO_LOG_WARNING(...) FIO_LOG_PRINT__(FIO_LOG_LEVEL_WARNING, "\x1B[2mWARNING:\x1B[0m  " __VA_ARGS__)
#undef FIO_LOG_SECURITY
#define FIO_LOG_SECURITY(...)   FIO_LOG_PRINT__(FIO_LOG_LEVEL_ERROR, "\x1B[1mSECURITY:\x1B[0m " __VA_ARGS__)
#undef FIO_LOG_ERROR
#define FIO_LOG_ERROR(...)   FIO_LOG_PRINT__(FIO_LOG_LEVEL_ERROR, "\x1B[1mERROR:\x1B[0m    " __VA_ARGS__)
#undef FIO_LOG_FATAL
#define FIO_LOG_FATAL(...)   FIO_LOG_PRINT__(FIO_LOG_LEVEL_FATAL, "\x1B[1;7mFATAL:\x1B[0m    " __VA_ARGS__)
// clang-format on

#endif /* FIO_LOG */
#undef FIO_LOG
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************










                            Atomic Operations










***************************************************************************** */

#if defined(FIO_ATOMIC) && !defined(H___FIO_ATOMIC___H)
#define H___FIO_ATOMIC___H 1
/* C11 Atomics are defined? */
#if defined(__ATOMIC_RELAXED)
/** An atomic load operation, returns value in pointer. */
#define fio_atomic_load(dest, p_obj)                                           \
  do {                                                                         \
    dest = __atomic_load_n((p_obj), __ATOMIC_SEQ_CST);                         \
  } while (0)

// clang-format off

/** An atomic compare and exchange operation, returns true if an exchange occured. `p_expected` MAY be overwritten with the existing value (system specific). */
#define fio_atomic_compare_exchange_p(p_obj, p_expected, p_desired) __atomic_compare_exchange((p_obj), (p_expected), (p_desired), 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)
/** An atomic exchange operation, returns previous value */
#define fio_atomic_exchange(p_obj, value) __atomic_exchange_n((p_obj), (value), __ATOMIC_SEQ_CST)
/** An atomic addition operation, returns previous value */
#define fio_atomic_add(p_obj, value) __atomic_fetch_add((p_obj), (value), __ATOMIC_SEQ_CST)
/** An atomic subtraction operation, returns previous value */
#define fio_atomic_sub(p_obj, value) __atomic_fetch_sub((p_obj), (value), __ATOMIC_SEQ_CST)
/** An atomic AND (&) operation, returns previous value */
#define fio_atomic_and(p_obj, value) __atomic_fetch_and((p_obj), (value), __ATOMIC_SEQ_CST)
/** An atomic XOR (^) operation, returns previous value */
#define fio_atomic_xor(p_obj, value) __atomic_fetch_xor((p_obj), (value), __ATOMIC_SEQ_CST)
/** An atomic OR (|) operation, returns previous value */
#define fio_atomic_or(p_obj, value) __atomic_fetch_or((p_obj), (value), __ATOMIC_SEQ_CST)
/** An atomic NOT AND ((~)&) operation, returns previous value */
#define fio_atomic_nand(p_obj, value) __atomic_fetch_nand((p_obj), (value), __ATOMIC_SEQ_CST)
/** An atomic addition operation, returns new value */
#define fio_atomic_add_fetch(p_obj, value) __atomic_add_fetch((p_obj), (value), __ATOMIC_SEQ_CST)
/** An atomic subtraction operation, returns new value */
#define fio_atomic_sub_fetch(p_obj, value) __atomic_sub_fetch((p_obj), (value), __ATOMIC_SEQ_CST)
/** An atomic AND (&) operation, returns new value */
#define fio_atomic_and_fetch(p_obj, value) __atomic_and_fetch((p_obj), (value), __ATOMIC_SEQ_CST)
/** An atomic XOR (^) operation, returns new value */
#define fio_atomic_xor_fetch(p_obj, value) __atomic_xor_fetch((p_obj), (value), __ATOMIC_SEQ_CST)
/** An atomic OR (|) operation, returns new value */
#define fio_atomic_or_fetch(p_obj, value) __atomic_or_fetch((p_obj), (value), __ATOMIC_SEQ_CST)
/** An atomic NOT AND ((~)&) operation, returns new value */
#define fio_atomic_nand_fetch(p_obj, value) __atomic_nand_fetch((p_obj), (value), __ATOMIC_SEQ_CST)
/* note: __ATOMIC_SEQ_CST may be safer and __ATOMIC_ACQ_REL may be faster */

/* Select the correct compiler builtin method. */
#elif __has_builtin(__sync_add_and_fetch) || (__GNUC__ > 3)
/** An atomic load operation, returns value in pointer. */
#define fio_atomic_load(dest, p_obj)                                           \
  do {                                                                         \
    dest = *(p_obj);                                                           \
  } while (!__sync_bool_compare_and_swap((p_obj), dest, dest))


/** An atomic compare and exchange operation, returns true if an exchange occured. `p_expected` MAY be overwritten with the existing value (system specific). */
#define fio_atomic_compare_exchange_p(p_obj, p_expected, p_desired) __sync_bool_compare_and_swap((p_obj), (p_expected), *(p_desired))
/** An atomic exchange operation, ruturns previous value */
#define fio_atomic_exchange(p_obj, value) __sync_val_compare_and_swap((p_obj), *(p_obj), (value))
/** An atomic addition operation, returns new value */
#define fio_atomic_add(p_obj, value) __sync_fetch_and_add((p_obj), (value))
/** An atomic subtraction operation, returns new value */
#define fio_atomic_sub(p_obj, value) __sync_fetch_and_sub((p_obj), (value))
/** An atomic AND (&) operation, returns new value */
#define fio_atomic_and(p_obj, value) __sync_fetch_and_and((p_obj), (value))
/** An atomic XOR (^) operation, returns new value */
#define fio_atomic_xor(p_obj, value) __sync_fetch_and_xor((p_obj), (value))
/** An atomic OR (|) operation, returns new value */
#define fio_atomic_or(p_obj, value) __sync_fetch_and_or((p_obj), (value))
/** An atomic NOT AND ((~)&) operation, returns new value */
#define fio_atomic_nand(p_obj, value) __sync_fetch_and_nand((p_obj), (value))
/** An atomic addition operation, returns previous value */
#define fio_atomic_add_fetch(p_obj, value) __sync_add_and_fetch((p_obj), (value))
/** An atomic subtraction operation, returns previous value */
#define fio_atomic_sub_fetch(p_obj, value) __sync_sub_and_fetch((p_obj), (value))
/** An atomic AND (&) operation, returns previous value */
#define fio_atomic_and_fetch(p_obj, value) __sync_and_and_fetch((p_obj), (value))
/** An atomic XOR (^) operation, returns previous value */
#define fio_atomic_xor_fetch(p_obj, value) __sync_xor_and_fetch((p_obj), (value))
/** An atomic OR (|) operation, returns previous value */
#define fio_atomic_or_fetch(p_obj, value) __sync_or_and_fetch((p_obj), (value))
/** An atomic NOT AND ((~)&) operation, returns previous value */
#define fio_atomic_nand_fetch(p_obj, value) __sync_nand_and_fetch((p_obj), (value))

// clang-format on

#else
#error Required builtin "__sync_add_and_fetch" not found.
#endif

#define FIO_LOCK_INIT 0
#define FIO_LOCK_SUBLOCK(sub) ((uint8_t)(1U) << ((sub)&7))
typedef volatile unsigned char fio_lock_i;

/** Tries to lock a specific sublock. Returns 0 on success and 1 on failure. */
FIO_IFUNC uint8_t fio_trylock_sublock(fio_lock_i *lock, uint8_t sub) {
  __asm__ volatile("" ::: "memory"); /* clobber CPU registers */
  sub &= 7;
  uint8_t sub_ = 1U << sub;
  return ((fio_atomic_or(lock, sub_) & sub_) >> sub);
}

/** Busy waits for a specific sublock to become available - not recommended. */
FIO_IFUNC void fio_lock_sublock(fio_lock_i *lock, uint8_t sub) {
  while (fio_trylock_sublock(lock, sub)) {
    FIO_THREAD_RESCHEDULE();
  }
}

/** Unlocks the specific sublock, no matter which thread owns the lock. */
FIO_IFUNC void fio_unlock_sublock(fio_lock_i *lock, uint8_t sub) {
  sub = 1U << (sub & 7);
  fio_atomic_and(lock, (~(fio_lock_i)sub));
}

/**
 * Tries to lock a group of sublocks.
 *
 * Combine a number of sublocks using OR (`|`) and the FIO_LOCK_SUBLOCK(i)
 * macro. i.e.:
 *
 *      if(!fio_trylock_group(&lock,
 *                            FIO_LOCK_SUBLOCK(1) | FIO_LOCK_SUBLOCK(2))) {
 *         // act in lock
 *      }
 *
 * Returns 0 on success and non-zero on failure.
 */
FIO_IFUNC uint8_t fio_trylock_group(fio_lock_i *lock, uint8_t group) {
  if (!group)
    group = 1;
  __asm__ volatile("" ::: "memory"); /* clobber CPU registers */
  uint8_t state = fio_atomic_or(lock, group);
  if (!(state & group))
    return 0;
  fio_atomic_and(lock, (state | (~group)));
  return 1;
}

/**
 * Busy waits for a group lock to become available - not recommended.
 *
 * See `fio_trylock_group` for details.
 */
FIO_IFUNC void fio_lock_group(fio_lock_i *lock, uint8_t group) {
  while (fio_trylock_group(lock, group)) {
    FIO_THREAD_RESCHEDULE();
  }
}

/** Unlocks a sublock group, no matter which thread owns which sublock. */
FIO_IFUNC void fio_unlock_group(fio_lock_i *lock, uint8_t group) {
  if (!group)
    group = 1;
  fio_atomic_and(lock, (~group));
}

/** Tries to lock all sublocks. Returns 0 on success and 1 on failure. */
FIO_IFUNC uint8_t fio_trylock_full(fio_lock_i *lock) {
  __asm__ volatile("" ::: "memory"); /* clobber CPU registers */
  fio_lock_i old = fio_atomic_or(lock, ~(fio_lock_i)0);
  if (!old)
    return 0;
  fio_atomic_and(lock, old);
  return 1;
}

/** Busy waits for all sub lock to become available - not recommended. */
FIO_IFUNC void fio_lock_full(fio_lock_i *lock) {
  while (fio_trylock_full(lock)) {
    FIO_THREAD_RESCHEDULE();
  }
}

/** Unlocks all sub locks, no matter which thread owns the lock. */
FIO_IFUNC void fio_unlock_full(fio_lock_i *lock) { fio_atomic_and(lock, 0); }

/**
 * Tries to acquire the default lock (sublock 0).
 *
 * Returns 0 on success and 1 on failure.
 */
FIO_IFUNC uint8_t fio_trylock(fio_lock_i *lock) {
  return fio_trylock_sublock(lock, 0);
}

/** Busy waits for the default lock to become available - not recommended. */
FIO_IFUNC void fio_lock(fio_lock_i *lock) {
  while (fio_trylock(lock)) {
    FIO_THREAD_RESCHEDULE();
  }
}

/** Unlocks the default lock, no matter which thread owns the lock. */
FIO_IFUNC void fio_unlock(fio_lock_i *lock) { fio_unlock_sublock(lock, 0); }

/** Returns 1 if the lock is locked, 0 otherwise. */
FIO_IFUNC uint8_t FIO_NAME_BL(fio, locked)(fio_lock_i *lock) {
  return *lock & 1;
}

/** Returns 1 if the lock is locked, 0 otherwise. */
FIO_IFUNC uint8_t FIO_NAME_BL(fio, sublocked)(fio_lock_i *lock, uint8_t sub) {
  uint8_t bit = 1U << (sub & 7);
  return (((*lock) & bit) >> (sub & 7));
}

/* *****************************************************************************
Atomic operations - test
***************************************************************************** */
#if defined(FIO_TEST_CSTL)

FIO_SFUNC void FIO_NAME_TEST(stl, atomics)(void) {
  fprintf(stderr, "* Testing atomic operation macros.\n");
  struct fio___atomic_test_s {
    size_t w;
    unsigned long l;
    unsigned short s;
    unsigned char c;
  } s = {0}, r1 = {0}, r2 = {0};
  fio_lock_i lock = FIO_LOCK_INIT;

  r1.c = fio_atomic_add(&s.c, 1);
  r1.s = fio_atomic_add(&s.s, 1);
  r1.l = fio_atomic_add(&s.l, 1);
  r1.w = fio_atomic_add(&s.w, 1);
  FIO_ASSERT(r1.c == 0 && s.c == 1, "fio_atomic_add failed for c");
  FIO_ASSERT(r1.s == 0 && s.s == 1, "fio_atomic_add failed for s");
  FIO_ASSERT(r1.l == 0 && s.l == 1, "fio_atomic_add failed for l");
  FIO_ASSERT(r1.w == 0 && s.w == 1, "fio_atomic_add failed for w");
  r2.c = fio_atomic_add_fetch(&s.c, 1);
  r2.s = fio_atomic_add_fetch(&s.s, 1);
  r2.l = fio_atomic_add_fetch(&s.l, 1);
  r2.w = fio_atomic_add_fetch(&s.w, 1);
  FIO_ASSERT(r2.c == 2 && s.c == 2, "fio_atomic_add_fetch failed for c");
  FIO_ASSERT(r2.s == 2 && s.s == 2, "fio_atomic_add_fetch failed for s");
  FIO_ASSERT(r2.l == 2 && s.l == 2, "fio_atomic_add_fetch failed for l");
  FIO_ASSERT(r2.w == 2 && s.w == 2, "fio_atomic_add_fetch failed for w");
  r1.c = fio_atomic_sub(&s.c, 1);
  r1.s = fio_atomic_sub(&s.s, 1);
  r1.l = fio_atomic_sub(&s.l, 1);
  r1.w = fio_atomic_sub(&s.w, 1);
  FIO_ASSERT(r1.c == 2 && s.c == 1, "fio_atomic_sub failed for c");
  FIO_ASSERT(r1.s == 2 && s.s == 1, "fio_atomic_sub failed for s");
  FIO_ASSERT(r1.l == 2 && s.l == 1, "fio_atomic_sub failed for l");
  FIO_ASSERT(r1.w == 2 && s.w == 1, "fio_atomic_sub failed for w");
  r2.c = fio_atomic_sub_fetch(&s.c, 1);
  r2.s = fio_atomic_sub_fetch(&s.s, 1);
  r2.l = fio_atomic_sub_fetch(&s.l, 1);
  r2.w = fio_atomic_sub_fetch(&s.w, 1);
  FIO_ASSERT(r2.c == 0 && s.c == 0, "fio_atomic_sub_fetch failed for c");
  FIO_ASSERT(r2.s == 0 && s.s == 0, "fio_atomic_sub_fetch failed for s");
  FIO_ASSERT(r2.l == 0 && s.l == 0, "fio_atomic_sub_fetch failed for l");
  FIO_ASSERT(r2.w == 0 && s.w == 0, "fio_atomic_sub_fetch failed for w");
  fio_atomic_add(&s.c, 1);
  fio_atomic_add(&s.s, 1);
  fio_atomic_add(&s.l, 1);
  fio_atomic_add(&s.w, 1);
  r1.c = fio_atomic_exchange(&s.c, 99);
  r1.s = fio_atomic_exchange(&s.s, 99);
  r1.l = fio_atomic_exchange(&s.l, 99);
  r1.w = fio_atomic_exchange(&s.w, 99);
  FIO_ASSERT(r1.c == 1 && s.c == 99, "fio_atomic_exchange failed for c");
  FIO_ASSERT(r1.s == 1 && s.s == 99, "fio_atomic_exchange failed for s");
  FIO_ASSERT(r1.l == 1 && s.l == 99, "fio_atomic_exchange failed for l");
  FIO_ASSERT(r1.w == 1 && s.w == 99, "fio_atomic_exchange failed for w");
  // clang-format off
  FIO_ASSERT(!fio_atomic_compare_exchange_p(&s.c, &r1.c, &r1.c), "fio_atomic_compare_exchange_p didn't fail for c");
  FIO_ASSERT(!fio_atomic_compare_exchange_p(&s.s, &r1.s, &r1.s), "fio_atomic_compare_exchange_p didn't fail for s");
  FIO_ASSERT(!fio_atomic_compare_exchange_p(&s.l, &r1.l, &r1.l), "fio_atomic_compare_exchange_p didn't fail for l");
  FIO_ASSERT(!fio_atomic_compare_exchange_p(&s.w, &r1.w, &r1.w), "fio_atomic_compare_exchange_p didn't fail for w");
  r1.c = 1;s.c = 99; r1.s = 1;s.s = 99; r1.l = 1;s.l = 99; r1.w = 1;s.w = 99; /* ignore system spefcific behavior. */
  r1.c = fio_atomic_compare_exchange_p(&s.c,&s.c, &r1.c);
  r1.s = fio_atomic_compare_exchange_p(&s.s,&s.s, &r1.s);
  r1.l = fio_atomic_compare_exchange_p(&s.l,&s.l, &r1.l);
  r1.w = fio_atomic_compare_exchange_p(&s.w,&s.w, &r1.w);
  FIO_ASSERT(r1.c == 1 && s.c == 1, "fio_atomic_compare_exchange_p failed for c");
  FIO_ASSERT(r1.s == 1 && s.s == 1, "fio_atomic_compare_exchange_p failed for s");
  FIO_ASSERT(r1.l == 1 && s.l == 1, "fio_atomic_compare_exchange_p failed for l");
  FIO_ASSERT(r1.w == 1 && s.w == 1, "fio_atomic_compare_exchange_p failed for w");
  // clang-format on

  uint64_t val = 1;
  FIO_ASSERT(fio_atomic_and(&val, 2) == 1,
             "fio_atomic_and should return old value");
  FIO_ASSERT(val == 0, "fio_atomic_and should update value");
  FIO_ASSERT(fio_atomic_xor(&val, 1) == 0,
             "fio_atomic_xor should return old value");
  FIO_ASSERT(val == 1, "fio_atomic_xor_fetch should update value");
  FIO_ASSERT(fio_atomic_xor_fetch(&val, 1) == 0,
             "fio_atomic_xor_fetch should return new value");
  FIO_ASSERT(val == 0, "fio_atomic_xor should update value");
  FIO_ASSERT(fio_atomic_or(&val, 2) == 0,
             "fio_atomic_or should return old value");
  FIO_ASSERT(val == 2, "fio_atomic_or should update value");
  FIO_ASSERT(fio_atomic_or_fetch(&val, 1) == 3,
             "fio_atomic_or_fetch should return new value");
  FIO_ASSERT(val == 3, "fio_atomic_or_fetch should update value");
  FIO_ASSERT(fio_atomic_nand_fetch(&val, 4) == ~0ULL,
             "fio_atomic_nand_fetch should return new value");
  FIO_ASSERT(val == ~0ULL, "fio_atomic_nand_fetch should update value");
  val = 3ULL;
  FIO_ASSERT(fio_atomic_nand(&val, 4) == 3ULL,
             "fio_atomic_nand should return old value");
  FIO_ASSERT(val == ~0ULL, "fio_atomic_nand_fetch should update value");

  FIO_ASSERT(!fio_is_locked(&lock),
             "lock should be initialized in unlocked state");
  FIO_ASSERT(!fio_trylock(&lock), "fio_trylock should succeed");
  FIO_ASSERT(fio_trylock(&lock), "fio_trylock should fail");
  FIO_ASSERT(fio_is_locked(&lock), "lock should be engaged");
  fio_unlock(&lock);
  FIO_ASSERT(!fio_is_locked(&lock), "lock should be released");
  fio_lock(&lock);
  FIO_ASSERT(fio_is_locked(&lock), "lock should be engaged (fio_lock)");
  for (uint8_t i = 1; i < 8; ++i) {
    FIO_ASSERT(!fio_is_sublocked(&lock, i),
               "sublock flagged, but wasn't engaged (%u - %p)",
               (unsigned int)i,
               (void *)(uintptr_t)lock);
  }
  fio_unlock(&lock);
  FIO_ASSERT(!fio_is_locked(&lock), "lock should be released");
  lock = FIO_LOCK_INIT;
  for (size_t i = 0; i < 8; ++i) {
    FIO_ASSERT(!fio_is_sublocked(&lock, i),
               "sublock should be initialized in unlocked state");
    FIO_ASSERT(!fio_trylock_sublock(&lock, i),
               "fio_trylock_sublock should succeed");
    FIO_ASSERT(fio_trylock_sublock(&lock, i), "fio_trylock should fail");
    FIO_ASSERT(fio_trylock_full(&lock), "fio_trylock_full should fail");
    FIO_ASSERT(fio_is_sublocked(&lock, i), "lock should be engaged");
    {
      uint8_t g =
          fio_trylock_group(&lock, FIO_LOCK_SUBLOCK(1) | FIO_LOCK_SUBLOCK(3));
      FIO_ASSERT((i != 1 && i != 3 && !g) || ((i == 1 || i == 3) && g),
                 "fio_trylock_group should succeed / fail");
      if (!g)
        fio_unlock_group(&lock, FIO_LOCK_SUBLOCK(1) | FIO_LOCK_SUBLOCK(3));
    }
    for (uint8_t j = 1; j < 8; ++j) {
      FIO_ASSERT(i == j || !fio_is_sublocked(&lock, j),
                 "another sublock was flagged, though it wasn't engaged");
    }
    FIO_ASSERT(fio_is_sublocked(&lock, i), "lock should remain engaged");
    fio_unlock_sublock(&lock, i);
    FIO_ASSERT(!fio_is_sublocked(&lock, i), "sublock should be released");
    FIO_ASSERT(!fio_trylock_full(&lock), "fio_trylock_full should succeed");
    fio_unlock_full(&lock);
    FIO_ASSERT(!lock, "fio_unlock_full should unlock all");
  }
}

#endif /* FIO_TEST_CSTL */

/* *****************************************************************************
Atomics - cleanup
***************************************************************************** */
#endif /* FIO_ATOMIC */
#undef FIO_ATOMIC

/* *****************************************************************************










                      Multi-Lock with Mutex Emulation










***************************************************************************** */
#if defined(FIO_LOCK2) && !defined(H___FIO_LOCK2___H)
#define H___FIO_LOCK2___H 1

#ifndef FIO_THREAD_T
#include <pthread.h>
#define FIO_THREAD_T pthread_t
#endif

#ifndef FIO_THREAD_ID
#define FIO_THREAD_ID() pthread_self()
#endif

#ifndef FIO_THREAD_PAUSE
#define FIO_THREAD_PAUSE(id)                                                   \
  do {                                                                         \
    sigset_t set___;                                                           \
    int got___sig;                                                             \
    sigemptyset(&set___);                                                      \
    sigaddset(&set___, SIGINT);                                                \
    sigaddset(&set___, SIGTERM);                                               \
    sigaddset(&set___, SIGCONT);                                               \
    sigwait(&set___, &got___sig);                                              \
  } while (0)
#endif

#ifndef FIO_THREAD_RESUME
#define FIO_THREAD_RESUME(id) pthread_kill((id), SIGCONT)
#endif

typedef struct fio___lock2_wait_s fio___lock2_wait_s;

/* *****************************************************************************
Public API
***************************************************************************** */

/**
 * The fio_lock2 variation is a Mutex style multi-lock.
 *
 * Thread functions and types are managed by the following macros:
 * * the `FIO_THREAD_T` macro should return a thread type, default: `pthread_t`
 * * the `FIO_THREAD_ID()` macro should return this thread's FIO_THREAD_T.
 * * the `FIO_THREAD_PAUSE(id)` macro should temporarily pause thread execution.
 * * the `FIO_THREAD_RESUME(id)` macro should resume thread execution.
 */
typedef struct {
  volatile size_t lock;
  fio___lock2_wait_s *volatile waiting;
} fio_lock2_s;

/**
 * Tries to lock a multilock.
 *
 * Combine a number of sublocks using OR (`|`) and the FIO_LOCK_SUBLOCK(i)
 * macro. i.e.:
 *
 *      if(!fio_trylock2(&lock,
 *                            FIO_LOCK_SUBLOCK(1) | FIO_LOCK_SUBLOCK(2))) {
 *         // act in lock
 *      }
 *
 * Returns 0 on success and non-zero on failure.
 */
FIO_IFUNC uint8_t fio_trylock2(fio_lock2_s *lock, size_t group);

/**
 * Locks a multilock, waiting as needed.
 *
 * Combine a number of sublocks using OR (`|`) and the FIO_LOCK_SUBLOCK(i)
 * macro. i.e.:
 *
 *      fio_lock2(&lock, FIO_LOCK_SUBLOCK(1) | FIO_LOCK_SUBLOCK(2)));
 *
 * Doesn't return until a successful lock was acquired.
 */
SFUNC void fio_lock2(fio_lock2_s *lock, size_t group);

/**
 * Unlocks a multilock, regardless of who owns the locked group.
 *
 * Combine a number of sublocks using OR (`|`) and the FIO_LOCK_SUBLOCK(i)
 * macro. i.e.:
 *
 *      fio_unlock2(&lock, FIO_LOCK_SUBLOCK(1) | FIO_LOCK_SUBLOCK(2));
 *
 */
SFUNC void fio_unlock2(fio_lock2_s *lock, size_t group);

/* *****************************************************************************
Implementation - Inline
***************************************************************************** */

/**
 * Tries to lock a multilock.
 *
 * Combine a number of sublocks using OR (`|`) and the FIO_LOCK_SUBLOCK(i)
 * macro. i.e.:
 *
 *      if(!fio_trylock2(&lock,
 *                            FIO_LOCK_SUBLOCK(1) | FIO_LOCK_SUBLOCK(2))) {
 *         // act in lock
 *      }
 *
 * Returns 0 on success and non-zero on failure.
 */
FIO_IFUNC uint8_t fio_trylock2(fio_lock2_s *lock, size_t group) {
  if (!group)
    group = 1;
  __asm__ volatile("" ::: "memory"); /* clobber CPU registers */
  size_t state = fio_atomic_or(&lock->lock, group);
  if (!(state & group))
    return 0;
  fio_atomic_and(&lock->lock, (state | (~group)));
  return 1;
}

/* *****************************************************************************
Implementation - Extern
***************************************************************************** */
#if defined(FIO_EXTERN_COMPLETE)

struct fio___lock2_wait_s {
  FIO_THREAD_T t;
  fio___lock2_wait_s *volatile next;
};

/**
 * Locks a multilock, waiting as needed.
 *
 * Combine a number of sublocks using OR (`|`) and the FIO_LOCK_SUBLOCK(i)
 * macro. i.e.:
 *
 *      fio_lock2(&lock, FIO_LOCK_SUBLOCK(1) | FIO_LOCK_SUBLOCK(2)));
 *
 * Doesn't return until a successful lock was acquired.
 */
SFUNC void fio_lock2(fio_lock2_s *lock, size_t group) {
  const size_t inner_lock = (sizeof(inner_lock) >= 8)
                                ? ((size_t)1UL << 63)
                                : (sizeof(inner_lock) >= 4)
                                      ? ((size_t)1UL << 31)
                                      : (sizeof(inner_lock) >= 2)
                                            ? ((size_t)1UL << 15)
                                            : ((size_t)1UL << 7);
  fio___lock2_wait_s self_thread;
  if (!group)
    group = 1;
  __asm__ volatile("" ::: "memory"); /* clobber CPU registers */
  size_t state = fio_atomic_or(&lock->lock, group);
  if (!(state & group))
    return;

  /* initialize self-waiting node memory (using stack memory) */
  self_thread.t = FIO_THREAD_ID();
  self_thread.next = NULL; // lock->waiting;

  /* enter waitlist lock */
  while ((fio_atomic_or(&lock->lock, inner_lock) & inner_lock)) {
    FIO_THREAD_RESCHEDULE();
  }

  /* add self-thread to end of waitlist */
  {
    fio___lock2_wait_s *volatile *i = &(lock->waiting);
    while (i[0]) {
      i = &(i[0]->next);
    }
    (*i) = &self_thread;
  }

  /* release waitlist lock and return lock's state */
  state = (state | (~group)) & (~inner_lock);
  fio_atomic_and(&lock->lock, (state & (~inner_lock)));

  for (;;) {
    state = fio_atomic_or(&lock->lock, group);
    if (!(state & group))
      break;
    // `next` may have been added while we didn't look
    if (self_thread.next) {
      /* resume next thread if this isn't for us (possibly different group) */
      fio_atomic_and(&lock->lock, (state | (~group)));
      FIO_THREAD_RESUME(self_thread.next->t);
    }
    FIO_THREAD_PAUSE(self_thread.t);
  }

  /* lock waitlist */
  while ((fio_atomic_or(&lock->lock, inner_lock) & inner_lock)) {
    FIO_THREAD_RESCHEDULE();
  }
  /* remove self from waiting list */
  for (fio___lock2_wait_s *volatile *i = &lock->waiting; *i; i = &(*i)->next) {
    if (*i != &self_thread)
      continue;
    *i = (*i)->next;
    break;
  }
  /* unlock waitlist */
  fio_atomic_and(&lock->lock, ~inner_lock);
}

/**
 * Unlocks a multilock, regardless of who owns the locked group.
 *
 * Combine a number of sublocks using OR (`|`) and the FIO_LOCK_SUBLOCK(i)
 * macro. i.e.:
 *
 *      fio_unlock2(&lock, FIO_LOCK_SUBLOCK(1) | FIO_LOCK_SUBLOCK(2));
 *
 */
SFUNC void fio_unlock2(fio_lock2_s *lock, size_t group) {
  size_t inner_lock;
  if (sizeof(inner_lock) >= 8)
    inner_lock = (size_t)1UL << 63;
  else if (sizeof(inner_lock) >= 4)
    inner_lock = (size_t)1UL << 31;
  else if (sizeof(inner_lock) >= 2)
    inner_lock = (size_t)1UL << 15;
  else
    inner_lock = (size_t)1UL << 7;
  fio___lock2_wait_s *waiting;
  if (!group)
    group = 1;
  /* spinlock for waitlist */
  while ((fio_atomic_or(&lock->lock, inner_lock) & inner_lock)) {
    FIO_THREAD_RESCHEDULE();
  }
  /* unlock group */
  waiting = lock->waiting;
  fio_atomic_and(&lock->lock, ~group);
  if (waiting) {
    FIO_THREAD_RESUME(waiting->t);
  }
  /* unlock waitlist */
  fio_atomic_and(&lock->lock, ~inner_lock);
}
#endif /* FIO_EXTERN_COMPLETE */

#endif /* FIO_LOCK2 */
#undef FIO_LOCK2
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#include "003 atomics.h"            /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************










                            Bit-Byte Operations










***************************************************************************** */

#if defined(FIO_BITWISE) && !defined(H___BITWISE___H)
#define H___BITWISE___H
/* *****************************************************************************
Swapping byte's order (`bswap` variations)
***************************************************************************** */

/** Byte swap a 16 bit integer, inlined. */
#if __has_builtin(__builtin_bswap16)
#define fio_bswap16(i) __builtin_bswap16((uint16_t)(i))
#else
FIO_IFUNC uint16_t fio_bswap16(uint16_t i) {
  return ((((i)&0xFFU) << 8) | (((i)&0xFF00U) >> 8));
}
#endif

/** Byte swap a 32 bit integer, inlined. */
#if __has_builtin(__builtin_bswap32)
#define fio_bswap32(i) __builtin_bswap32((uint32_t)(i))
#else
FIO_IFUNC uint32_t fio_bswap32(uint32_t i) {
  return ((((i)&0xFFUL) << 24) | (((i)&0xFF00UL) << 8) |
          (((i)&0xFF0000UL) >> 8) | (((i)&0xFF000000UL) >> 24));
}
#endif

/** Byte swap a 64 bit integer, inlined. */
#if __has_builtin(__builtin_bswap64)
#define fio_bswap64(i) __builtin_bswap64((uint64_t)(i))
#else
FIO_IFUNC uint64_t fio_bswap64(uint64_t i) {
  return ((((i)&0xFFULL) << 56) | (((i)&0xFF00ULL) << 40) |
          (((i)&0xFF0000ULL) << 24) | (((i)&0xFF000000ULL) << 8) |
          (((i)&0xFF00000000ULL) >> 8) | (((i)&0xFF0000000000ULL) >> 24) |
          (((i)&0xFF000000000000ULL) >> 40) |
          (((i)&0xFF00000000000000ULL) >> 56));
}
#endif

/* *****************************************************************************
Big Endian / Small Endian
***************************************************************************** */
#if (defined(__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__) ||                       \
    (defined(__BIG_ENDIAN__) && !__BIG_ENDIAN__) ||                            \
    (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__))
#ifndef __BIG_ENDIAN__
#define __BIG_ENDIAN__ 0
#endif
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__ 1
#endif
#elif (defined(__BIG_ENDIAN__) && __BIG_ENDIAN__) ||                           \
    (defined(__LITTLE_ENDIAN__) && !__LITTLE_ENDIAN__) ||                      \
    (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__))
#ifndef __BIG_ENDIAN__
#define __BIG_ENDIAN__ 1
#endif
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__ 0
#endif
#elif !defined(__BIG_ENDIAN__) && !defined(__BYTE_ORDER__) &&                  \
    !defined(__LITTLE_ENDIAN__)
#error Could not detect byte order on this system.
#endif

#if __BIG_ENDIAN__

/** Local byte order to Network byte order, 16 bit integer */
#define fio_lton16(i) (i)
/** Local byte order to Network byte order, 32 bit integer */
#define fio_lton32(i) (i)
/** Local byte order to Network byte order, 62 bit integer */
#define fio_lton64(i) (i)

/** Network byte order to Local byte order, 16 bit integer */
#define fio_ntol16(i) (i)
/** Network byte order to Local byte order, 32 bit integer */
#define fio_ntol32(i) (i)
/** Network byte order to Local byte order, 62 bit integer */
#define fio_ntol64(i) (i)

#else /* Little Endian */

/** Local byte order to Network byte order, 16 bit integer */
#define fio_lton16(i) fio_bswap16((i))
/** Local byte order to Network byte order, 32 bit integer */
#define fio_lton32(i) fio_bswap32((i))
/** Local byte order to Network byte order, 62 bit integer */
#define fio_lton64(i) fio_bswap64((i))

/** Network byte order to Local byte order, 16 bit integer */
#define fio_ntol16(i) fio_bswap16((i))
/** Network byte order to Local byte order, 32 bit integer */
#define fio_ntol32(i) fio_bswap32((i))
/** Network byte order to Local byte order, 62 bit integer */
#define fio_ntol64(i) fio_bswap64((i))

#endif /* __BIG_ENDIAN__ */

/* *****************************************************************************
Bit rotation
***************************************************************************** */

/** Left rotation for an unknown size element, inlined. */
#define FIO_LROT(i, bits)                                                      \
  (((i) << ((bits) & ((sizeof((i)) << 3) - 1))) |                              \
   ((i) >> ((-(bits)) & ((sizeof((i)) << 3) - 1))))

/** Right rotation for an unknown size element, inlined. */
#define FIO_RROT(i, bits)                                                      \
  (((i) >> ((bits) & ((sizeof((i)) << 3) - 1))) |                              \
   ((i) << ((-(bits)) & ((sizeof((i)) << 3) - 1))))

#if __has_builtin(__builtin_rotateleft8)
/** 8Bit left rotation, inlined. */
#define fio_lrot8(i, bits) __builtin_rotateleft8(i, bits)
#else
/** 8Bit left rotation, inlined. */
FIO_IFUNC uint8_t fio_lrot8(uint8_t i, uint8_t bits) {
  return ((i << (bits & 7UL)) | (i >> ((-(bits)) & 7UL)));
}
#endif

#if __has_builtin(__builtin_rotateleft16)
/** 16Bit left rotation, inlined. */
#define fio_lrot16(i, bits) __builtin_rotateleft16(i, bits)
#else
/** 16Bit left rotation, inlined. */
FIO_IFUNC uint16_t fio_lrot16(uint16_t i, uint8_t bits) {
  return ((i << (bits & 15UL)) | (i >> ((-(bits)) & 15UL)));
}
#endif

#if __has_builtin(__builtin_rotateleft32)
/** 32Bit left rotation, inlined. */
#define fio_lrot32(i, bits) __builtin_rotateleft32(i, bits)
#else
/** 32Bit left rotation, inlined. */
FIO_IFUNC uint32_t fio_lrot32(uint32_t i, uint8_t bits) {
  return ((i << (bits & 31UL)) | (i >> ((-(bits)) & 31UL)));
}
#endif

#if __has_builtin(__builtin_rotateleft64)
/** 64Bit left rotation, inlined. */
#define fio_lrot64(i, bits) __builtin_rotateleft64(i, bits)
#else
/** 64Bit left rotation, inlined. */
FIO_IFUNC uint64_t fio_lrot64(uint64_t i, uint8_t bits) {
  return ((i << ((bits)&63UL)) | (i >> ((-(bits)) & 63UL)));
}
#endif

#if __has_builtin(__builtin_rotatrightt8)
/** 8Bit left rotation, inlined. */
#define fio_rrot8(i, bits) __builtin_rotateright8(i, bits)
#else
/** 8Bit left rotation, inlined. */
FIO_IFUNC uint8_t fio_rrot8(uint8_t i, uint8_t bits) {
  return ((i >> (bits & 7UL)) | (i << ((-(bits)) & 7UL)));
}
#endif

#if __has_builtin(__builtin_rotateright16)
/** 16Bit left rotation, inlined. */
#define fio_rrot16(i, bits) __builtin_rotateright16(i, bits)
#else
/** 16Bit left rotation, inlined. */
FIO_IFUNC uint16_t fio_rrot16(uint16_t i, uint8_t bits) {
  return ((i >> (bits & 15UL)) | (i << ((-(bits)) & 15UL)));
}
#endif

#if __has_builtin(__builtin_rotateright32)
/** 32Bit left rotation, inlined. */
#define fio_rrot32(i, bits) __builtin_rotateright32(i, bits)
#else
/** 32Bit left rotation, inlined. */
FIO_IFUNC uint32_t fio_rrot32(uint32_t i, uint8_t bits) {
  return ((i >> (bits & 31UL)) | (i << ((-(bits)) & 31UL)));
}
#endif

#if __has_builtin(__builtin_rotateright64)
/** 64Bit left rotation, inlined. */
#define fio_rrot64(i, bits) __builtin_rotateright64(i, bits)
#else
/** 64Bit left rotation, inlined. */
FIO_IFUNC uint64_t fio_rrot64(uint64_t i, uint8_t bits) {
  return ((i >> ((bits)&63UL)) | (i << ((-(bits)) & 63UL)));
}
#endif

/* *****************************************************************************
Unaligned memory read / write operations
***************************************************************************** */

#if FIO_UNALIGNED_MEMORY_ACCESS_ENABLED
/** Converts an unaligned byte stream to a 16 bit number (local byte order). */
FIO_IFUNC uint16_t FIO_NAME2(fio_buf, u16_local)(const void *c) {
  const uint16_t *tmp = (const uint16_t *)c; /* fio_buf2u16 */
  return *tmp;
}
/** Converts an unaligned byte stream to a 32 bit number (local byte order). */
FIO_IFUNC uint32_t FIO_NAME2(fio_buf, u32_local)(const void *c) {
  const uint32_t *tmp = (const uint32_t *)c; /* fio_buf2u32 */
  return *tmp;
}
/** Converts an unaligned byte stream to a 64 bit number (local byte order). */
FIO_IFUNC uint64_t FIO_NAME2(fio_buf, u64_local)(const void *c) {
  const uint64_t *tmp = (const uint64_t *)c; /* fio_buf2u64 */
  return *tmp;
}

/** Writes a local 16 bit number to an unaligned buffer. */
FIO_IFUNC void FIO_NAME2(fio_u, buf16_local)(void *buf, uint16_t i) {
  *((uint16_t *)buf) = i; /* fio_u2buf16 */
}
/** Writes a local 32 bit number to an unaligned buffer. */
FIO_IFUNC void FIO_NAME2(fio_u, buf32_local)(void *buf, uint32_t i) {
  *((uint32_t *)buf) = i; /* fio_u2buf32 */
}
/** Writes a local 64 bit number to an unaligned buffer. */
FIO_IFUNC void FIO_NAME2(fio_u, buf64_local)(void *buf, uint64_t i) {
  *((uint64_t *)buf) = i; /* fio_u2buf64 */
}

#else /* FIO_UNALIGNED_MEMORY_ACCESS_ENABLED */

#if __has_builtin(__builtin_memcpy)
#define FIO___MEMCPY __builtin_memcpy
#else
#define FIO___MEMCPY memcpy
#endif

/** Converts an unaligned byte stream to a 16 bit number (local byte order). */
FIO_IFUNC uint16_t FIO_NAME2(fio_buf, u16_local)(const void *c) {
  uint16_t tmp; /* fio_buf2u16 */
  FIO___MEMCPY(&tmp, c, sizeof(tmp));
  return tmp;
}
/** Converts an unaligned byte stream to a 32 bit number (local byte order). */
FIO_IFUNC uint32_t FIO_NAME2(fio_buf, u32_local)(const void *c) {
  uint32_t tmp; /* fio_buf2u32 */
  FIO___MEMCPY(&tmp, c, sizeof(tmp));
  return tmp;
}
/** Converts an unaligned byte stream to a 64 bit number (local byte order). */
FIO_IFUNC uint64_t FIO_NAME2(fio_buf, u64_local)(const void *c) {
  uint64_t tmp; /* fio_buf2u64 */
  memcpy(&tmp, c, sizeof(tmp));
  return tmp;
}

/** Writes a local 16 bit number to an unaligned buffer. */
FIO_IFUNC void FIO_NAME2(fio_u, buf16_local)(void *buf, uint16_t i) {
  FIO___MEMCPY(buf, &i, sizeof(i)); /* fio_u2buf16 */
}
/** Writes a local 32 bit number to an unaligned buffer. */
FIO_IFUNC void FIO_NAME2(fio_u, buf32_local)(void *buf, uint32_t i) {
  FIO___MEMCPY(buf, &i, sizeof(i)); /* fio_u2buf32 */
}
/** Writes a local 64 bit number to an unaligned buffer. */
FIO_IFUNC void FIO_NAME2(fio_u, buf64_local)(void *buf, uint64_t i) {
  FIO___MEMCPY(buf, &i, sizeof(i)); /* fio_u2buf64 */
}
#undef FIO___MEMCPY

#endif /* FIO_UNALIGNED_MEMORY_ACCESS_ENABLED */

/** Converts an unaligned byte stream to a 16 bit number (reversed order). */
FIO_IFUNC uint16_t FIO_NAME2(fio_buf, u16_bswap)(const void *c) {
  return fio_bswap16(FIO_NAME2(fio_buf, u16_local)(c)); /* fio_buf2u16 */
}
/** Converts an unaligned byte stream to a 32 bit number (reversed order). */
FIO_IFUNC uint32_t FIO_NAME2(fio_buf, u32_bswap)(const void *c) {
  return fio_bswap32(FIO_NAME2(fio_buf, u32_local)(c)); /* fio_buf2u32 */
}
/** Converts an unaligned byte stream to a 64 bit number (reversed order). */
FIO_IFUNC uint64_t FIO_NAME2(fio_buf, u64_bswap)(const void *c) {
  return fio_bswap64(FIO_NAME2(fio_buf, u64_local)(c)); /* fio_buf2u64 */
}

/** Writes a local 16 bit number to an unaligned buffer in reversed order. */
FIO_IFUNC void FIO_NAME2(fio_u, buf16_bswap)(void *buf, uint16_t i) {
  FIO_NAME2(fio_u, buf16_local)(buf, fio_bswap16(i));
}
/** Writes a local 32 bit number to an unaligned buffer in reversed order. */
FIO_IFUNC void FIO_NAME2(fio_u, buf32_bswap)(void *buf, uint32_t i) {
  FIO_NAME2(fio_u, buf32_local)(buf, fio_bswap32(i));
}
/** Writes a local 64 bit number to an unaligned buffer in reversed order. */
FIO_IFUNC void FIO_NAME2(fio_u, buf64_bswap)(void *buf, uint64_t i) {
  FIO_NAME2(fio_u, buf64_local)(buf, fio_bswap64(i));
}

#if __LITTLE_ENDIAN__
/** Converts an unaligned byte stream to a 16 bit number (Big Endian). */
FIO_IFUNC uint16_t FIO_NAME2(fio_buf, u16)(const void *c) { /* fio_buf2u16 */
  return FIO_NAME2(fio_buf, u16_bswap)(c);
}
/** Converts an unaligned byte stream to a 32 bit number (Big Endian). */
FIO_IFUNC uint32_t FIO_NAME2(fio_buf, u32)(const void *c) { /* fio_buf2u32 */
  return FIO_NAME2(fio_buf, u32_bswap)(c);
}
/** Converts an unaligned byte stream to a 64 bit number (Big Endian). */
FIO_IFUNC uint64_t FIO_NAME2(fio_buf, u64)(const void *c) { /* fio_buf2u64 */
  return FIO_NAME2(fio_buf, u64_bswap)(c);
}
/** Converts an unaligned byte stream to a 16 bit number (Little Endian). */
FIO_IFUNC uint16_t FIO_NAME2(fio_buf, u16_little)(const void *c) {
  return FIO_NAME2(fio_buf, u16_local)(c); /* fio_buf2u16 */
}
/** Converts an unaligned byte stream to a 32 bit number (Little Endian). */
FIO_IFUNC uint32_t FIO_NAME2(fio_buf, u32_little)(const void *c) {
  return FIO_NAME2(fio_buf, u32_local)(c); /* fio_buf2u32 */
}
/** Converts an unaligned byte stream to a 64 bit number (Little Endian). */
FIO_IFUNC uint64_t FIO_NAME2(fio_buf, u64_little)(const void *c) {
  return FIO_NAME2(fio_buf, u64_local)(c); /* fio_buf2u64 */
}

/** Writes a local 16 bit number to an unaligned buffer in Big Endian. */
FIO_IFUNC void FIO_NAME2(fio_u, buf16)(void *buf, uint16_t i) {
  FIO_NAME2(fio_u, buf16_bswap)(buf, i);
}
/** Writes a local 32 bit number to an unaligned buffer in Big Endian. */
FIO_IFUNC void FIO_NAME2(fio_u, buf32)(void *buf, uint32_t i) {
  FIO_NAME2(fio_u, buf32_bswap)(buf, i);
}
/** Writes a local 64 bit number to an unaligned buffer in Big Endian. */
FIO_IFUNC void FIO_NAME2(fio_u, buf64)(void *buf, uint64_t i) {
  FIO_NAME2(fio_u, buf64_bswap)(buf, i);
}
/** Writes a local 16 bit number to an unaligned buffer in Little Endian. */
FIO_IFUNC void FIO_NAME2(fio_u, buf16_little)(void *buf, uint16_t i) {
  FIO_NAME2(fio_u, buf16_local)(buf, i);
}
/** Writes a local 32 bit number to an unaligned buffer in Little Endian. */
FIO_IFUNC void FIO_NAME2(fio_u, buf32_little)(void *buf, uint32_t i) {
  FIO_NAME2(fio_u, buf32_local)(buf, i);
}
/** Writes a local 64 bit number to an unaligned buffer in Little Endian. */
FIO_IFUNC void FIO_NAME2(fio_u, buf64_little)(void *buf, uint64_t i) {
  FIO_NAME2(fio_u, buf64_local)(buf, i);
}
#else
/** Converts an unaligned byte stream to a 16 bit number (Big Endian). */
FIO_IFUNC uint16_t FIO_NAME2(fio_buf, u16)(const void *c) { /* fio_buf2u16 */
  return FIO_NAME2(fio_buf, u16_local)(c);
}
/** Converts an unaligned byte stream to a 32 bit number (Big Endian). */
FIO_IFUNC uint32_t FIO_NAME2(fio_buf, u32)(const void *c) { /* fio_buf2u32 */
  return FIO_NAME2(fio_buf, u32_local)(c);
}
/** Converts an unaligned byte stream to a 64 bit number (Big Endian). */
FIO_IFUNC uint64_t FIO_NAME2(fio_buf, u64)(const void *c) { /* fio_buf2u64 */
  return FIO_NAME2(fio_buf, u64_local)(c);
}
/** Converts an unaligned byte stream to a 16 bit number (Little Endian). */
FIO_IFUNC uint16_t FIO_NAME2(fio_buf, u16_little)(const void *c) {
  return FIO_NAME2(fio_buf, u16_bswap)(c); /* fio_buf2u16 */
}
/** Converts an unaligned byte stream to a 32 bit number (Little Endian). */
FIO_IFUNC uint32_t FIO_NAME2(fio_buf, u32_little)(const void *c) {
  return FIO_NAME2(fio_buf, u32_bswap)(c); /* fio_buf2u32 */
}
/** Converts an unaligned byte stream to a 64 bit number (Little Endian). */
FIO_IFUNC uint64_t FIO_NAME2(fio_buf, u64_little)(const void *c) {
  return FIO_NAME2(fio_buf, u64_bswap)(c); /* fio_buf2u64 */
}

/** Writes a local 16 bit number to an unaligned buffer in Big Endian. */
FIO_IFUNC void FIO_NAME2(fio_u, buf16)(void *buf, uint16_t i) {
  FIO_NAME2(fio_u, buf16_local)(buf, i);
}
/** Writes a local 32 bit number to an unaligned buffer in Big Endian. */
FIO_IFUNC void FIO_NAME2(fio_u, buf32)(void *buf, uint32_t i) {
  FIO_NAME2(fio_u, buf32_local)(buf, i);
}
/** Writes a local 64 bit number to an unaligned buffer in Big Endian. */
FIO_IFUNC void FIO_NAME2(fio_u, buf64)(void *buf, uint64_t i) {
  FIO_NAME2(fio_u, buf64_local)(buf, i);
}
/** Writes a local 16 bit number to an unaligned buffer in Little Endian. */
FIO_IFUNC void FIO_NAME2(fio_u, buf16_little)(void *buf, uint16_t i) {
  FIO_NAME2(fio_u, buf16_bswap)(buf, i);
}
/** Writes a local 32 bit number to an unaligned buffer in Little Endian. */
FIO_IFUNC void FIO_NAME2(fio_u, buf32_little)(void *buf, uint32_t i) {
  FIO_NAME2(fio_u, buf32_bswap)(buf, i);
}
/** Writes a local 64 bit number to an unaligned buffer in Little Endian. */
FIO_IFUNC void FIO_NAME2(fio_u, buf64_little)(void *buf, uint64_t i) {
  FIO_NAME2(fio_u, buf64_bswap)(buf, i);
}

#endif

/** Convinience function for reading 1 byte (8 bit) from a buffer. */
FIO_IFUNC uint8_t FIO_NAME2(fio_buf, u8_local)(const void *c) {
  const uint8_t *tmp = (const uint8_t *)c; /* fio_buf2u16 */
  return *tmp;
}

/** Convinience function for writing 1 byte (8 bit) to a buffer. */
FIO_IFUNC void FIO_NAME2(fio_u, buf8_local)(void *buf, uint8_t i) {
  *((uint8_t *)buf) = i; /* fio_u2buf16 */
}

/** Convinience function for reading 1 byte (8 bit) from a buffer. */
FIO_IFUNC uint8_t FIO_NAME2(fio_buf, u8_bswap)(const void *c) {
  const uint8_t *tmp = (const uint8_t *)c; /* fio_buf2u16 */
  return *tmp;
}

/** Convinience function for writing 1 byte (8 bit) to a buffer. */
FIO_IFUNC void FIO_NAME2(fio_u, buf8_bswap)(void *buf, uint8_t i) {
  *((uint8_t *)buf) = i; /* fio_u2buf16 */
}

/** Convinience function for reading 1 byte (8 bit) from a buffer. */
FIO_IFUNC uint8_t FIO_NAME2(fio_buf, u8_little)(const void *c) {
  const uint8_t *tmp = (const uint8_t *)c; /* fio_buf2u16 */
  return *tmp;
}

/** Convinience function for writing 1 byte (8 bit) to a buffer. */
FIO_IFUNC void FIO_NAME2(fio_u, buf8_little)(void *buf, uint8_t i) {
  *((uint8_t *)buf) = i; /* fio_u2buf16 */
}

/** Convinience function for reading 1 byte (8 bit) from a buffer. */
FIO_IFUNC uint8_t FIO_NAME2(fio_buf, u8)(const void *c) {
  const uint8_t *tmp = (const uint8_t *)c; /* fio_buf2u16 */
  return *tmp;
}

/** Convinience function for writing 1 byte (8 bit) to a buffer. */
FIO_IFUNC void FIO_NAME2(fio_u, buf8)(void *buf, uint8_t i) {
  *((uint8_t *)buf) = i; /* fio_u2buf16 */
}

/* *****************************************************************************
Constant-Time Selectors
***************************************************************************** */

/** Returns 1 if the expression is true (input isn't zero). */
FIO_IFUNC uintptr_t fio_ct_true(uintptr_t cond) {
  // promise that the highest bit is set if any bits are set, than shift.
  return ((cond | (0 - cond)) >> ((sizeof(cond) << 3) - 1));
}

/** Returns 1 if the expression is false (input is zero). */
FIO_IFUNC uintptr_t fio_ct_false(uintptr_t cond) {
  // fio_ct_true returns only one bit, XOR will inverse that bit.
  return fio_ct_true(cond) ^ 1;
}

/** Returns `a` if `cond` is boolean and true, returns b otherwise. */
FIO_IFUNC uintptr_t fio_ct_if_bool(uint8_t cond, uintptr_t a, uintptr_t b) {
  // b^(a^b) cancels b out. 0-1 => sets all bits.
  return (b ^ ((0 - (cond & 1)) & (a ^ b)));
}

/** Returns `a` if `cond` isn't zero (uses fio_ct_true), returns b otherwise. */
FIO_IFUNC uintptr_t fio_ct_if(uintptr_t cond, uintptr_t a, uintptr_t b) {
  // b^(a^b) cancels b out. 0-1 => sets all bits.
  return fio_ct_if_bool(fio_ct_true(cond), a, b);
}

/* *****************************************************************************
Byte masking (XOR)
***************************************************************************** */

/**
 * Masks 64 bit memory aligned data using a 64 bit mask and a counter mode
 * nonce.
 *
 * Returns the end state of the mask.
 */
FIO_IFUNC uint64_t fio___xmask_aligned64(uint64_t buf[],
                                         size_t byte_len,
                                         uint64_t mask,
                                         uint64_t nonce) {

  register uint64_t m = mask;
  for (size_t i = byte_len >> 3; i; --i) {
    *buf ^= m;
    m += nonce;
    ++buf;
  }
  mask = m;
  union { /* type punning */
    char *p8;
    uint64_t *p64;
  } pn, mpn;
  pn.p64 = buf;
  mpn.p64 = &mask;

  switch ((byte_len & 7)) {
  case 0:
    return mask;
  case 7:
    pn.p8[6] ^= mpn.p8[6];
  /* fallthrough */
  case 6:
    pn.p8[5] ^= mpn.p8[5];
  /* fallthrough */
  case 5:
    pn.p8[4] ^= mpn.p8[4];
  /* fallthrough */
  case 4:
    pn.p8[3] ^= mpn.p8[3];
  /* fallthrough */
  case 3:
    pn.p8[2] ^= mpn.p8[2];
  /* fallthrough */
  case 2:
    pn.p8[1] ^= mpn.p8[1];
  /* fallthrough */
  case 1:
    pn.p8[0] ^= mpn.p8[0];
    /* fallthrough */
  }
  return mask;
}

/**
 * Masks unaligned memory data using a 64 bit mask and a counter mode nonce.
 *
 * Returns the end state of the mask.
 */
FIO_IFUNC uint64_t fio___xmask_unaligned_words(void *buf_,
                                               size_t len,
                                               uint64_t mask,
                                               const uint64_t nonce) {
  register uint8_t *buf = (uint8_t *)buf_;
  register uint64_t m = mask;
  for (size_t i = len >> 3; i; --i) {
    uint64_t tmp;
    tmp = FIO_NAME2(fio_buf, u64_local)(buf);
    tmp ^= m;
    FIO_NAME2(fio_u, buf64_local)(buf, tmp);
    m += nonce;
    buf += 8;
  }
  mask = m;
  switch ((len & 7)) {
  case 0:
    return mask;
  case 7:
    buf[6] ^= ((uint8_t *)(&mask))[6];
  /* fallthrough */
  case 6:
    buf[5] ^= ((uint8_t *)(&mask))[5];
  /* fallthrough */
  case 5:
    buf[4] ^= ((uint8_t *)(&mask))[4];
  /* fallthrough */
  case 4:
    buf[3] ^= ((uint8_t *)(&mask))[3];
  /* fallthrough */
  case 3:
    buf[2] ^= ((uint8_t *)(&mask))[2];
  /* fallthrough */
  case 2:
    buf[1] ^= ((uint8_t *)(&mask))[1];
  /* fallthrough */
  case 1:
    buf[0] ^= ((uint8_t *)(&mask))[0];
    /* fallthrough */
  }
  return mask;
}

/**
 * Masks data using a 64 bit mask and a counter mode nonce. When the buffer's
 * memory is aligned, the function may perform significantly better.
 *
 * Returns the end state of the mask.
 */
FIO_IFUNC uint64_t fio_xmask(char *buf,
                             size_t len,
                             uint64_t mask,
                             uint64_t nonce) {
  if (!((uintptr_t)buf & 7)) {
    union {
      char *p8;
      uint64_t *p64;
    } pn;
    pn.p8 = buf;
    return fio___xmask_aligned64(pn.p64, len, mask, nonce);
  }
  return fio___xmask_unaligned_words(buf, len, mask, nonce);
}

/* *****************************************************************************
Hemming Distance and bit counting
***************************************************************************** */

#if __has_builtin(__builtin_popcountll)
#define fio_popcount(n) __builtin_popcountll(n)
#else
FIO_IFUNC int fio_popcount(uint64_t n) {
  int c = 0;
  while (n) {
    ++c;
    n &= n - 1;
  }
  return c;
}
#endif

#define fio_hemming_dist(n1, n2) fio_popcount(((uint64_t)(n1) ^ (uint64_t)(n2)))

/* *****************************************************************************
Bitewise helpers cleanup
***************************************************************************** */
#endif /* FIO_BITWISE */
#undef FIO_BITWISE

/* *****************************************************************************










                                Bitmap Helpers










***************************************************************************** */
#if defined(FIO_BITMAP) && !defined(H___FIO_BITMAP_H)
#define H___FIO_BITMAP_H
/* *****************************************************************************
Bitmap access / manipulation
***************************************************************************** */

/** Gets the state of a bit in a bitmap. */
FIO_IFUNC uint8_t fio_bitmap_get(void *map, size_t bit) {
  return ((((uint8_t *)(map))[(bit) >> 3] >> ((bit)&7)) & 1);
}

/** Sets the a bit in a bitmap (sets to 1). */
FIO_IFUNC void fio_bitmap_set(void *map, size_t bit) {
  fio_atomic_or((uint8_t *)(map) + ((bit) >> 3), (1UL << ((bit)&7)));
}

/** Unsets the a bit in a bitmap (sets to 0). */
FIO_IFUNC void fio_bitmap_unset(void *map, size_t bit) {
  fio_atomic_and((uint8_t *)(map) + ((bit) >> 3),
                 (uint8_t)(~(1UL << ((bit)&7))));
}

/** Flips the a bit in a bitmap (sets to 0 if 1, sets to 1 if 0). */
FIO_IFUNC void fio_bitmap_flip(void *map, size_t bit) {
  fio_atomic_xor((uint8_t *)(map) + ((bit) >> 3), (1UL << ((bit)&7)));
}

/* *****************************************************************************
Bit-Byte operations - testing
***************************************************************************** */
#ifdef FIO_TEST_CSTL

FIO_SFUNC void FIO_NAME_TEST(stl, bitwise)(void) {
  fprintf(stderr, "* Testing fio_bswapX macros.\n");
  FIO_ASSERT(fio_bswap16(0x0102) == (uint16_t)0x0201, "fio_bswap16 failed");
  FIO_ASSERT(fio_bswap32(0x01020304) == (uint32_t)0x04030201,
             "fio_bswap32 failed");
  FIO_ASSERT(fio_bswap64(0x0102030405060708ULL) == 0x0807060504030201ULL,
             "fio_bswap64 failed");

  fprintf(stderr, "* Testing fio_lrotX and fio_rrotX macros.\n");
  {
    uint64_t tmp = 1;
    tmp = FIO_RROT(tmp, 1);
    __asm__ volatile("" ::: "memory");
    FIO_ASSERT(tmp == ((uint64_t)1 << ((sizeof(uint64_t) << 3) - 1)),
               "fio_rrot failed");
    tmp = FIO_LROT(tmp, 3);
    __asm__ volatile("" ::: "memory");
    FIO_ASSERT(tmp == ((uint64_t)1 << 2), "fio_lrot failed");
    tmp = 1;
    tmp = fio_rrot32(tmp, 1);
    __asm__ volatile("" ::: "memory");
    FIO_ASSERT(tmp == ((uint64_t)1 << 31), "fio_rrot32 failed");
    tmp = fio_lrot32(tmp, 3);
    __asm__ volatile("" ::: "memory");
    FIO_ASSERT(tmp == ((uint64_t)1 << 2), "fio_lrot32 failed");
    tmp = 1;
    tmp = fio_rrot64(tmp, 1);
    __asm__ volatile("" ::: "memory");
    FIO_ASSERT(tmp == ((uint64_t)1 << 63), "fio_rrot64 failed");
    tmp = fio_lrot64(tmp, 3);
    __asm__ volatile("" ::: "memory");
    FIO_ASSERT(tmp == ((uint64_t)1 << 2), "fio_lrot64 failed");
  }

  fprintf(stderr, "* Testing fio_buf2uX and fio_u2bufX helpers.\n");
#define FIO___BITMAP_TEST_BITS(bits)                                           \
  for (size_t i = 0; i <= (bits); ++i) {                                       \
    char tmp_buf[16];                                                          \
    int##bits##_t n = ((uint##bits##_t)1 << i);                                \
    FIO_NAME2(fio_u, buf##bits)(tmp_buf, n);                                   \
    int##bits##_t r = FIO_NAME2(fio_buf, u##bits)(tmp_buf);                    \
    FIO_ASSERT(r == n,                                                         \
               "roundtrip failed for U" #bits " at bit %zu\n\t%zu != %zu",     \
               i,                                                              \
               (size_t)n,                                                      \
               (size_t)r);                                                     \
  }
  FIO___BITMAP_TEST_BITS(8);
  FIO___BITMAP_TEST_BITS(16);
  FIO___BITMAP_TEST_BITS(32);
  FIO___BITMAP_TEST_BITS(64);
#undef FIO___BITMAP_TEST_BITS

  fprintf(stderr, "* Testing constant-time helpers.\n");
  FIO_ASSERT(fio_ct_true(0) == 0, "fio_ct_true(0) should be zero!");
  for (uintptr_t i = 1; i; i <<= 1) {
    FIO_ASSERT(
        fio_ct_true(i) == 1, "fio_ct_true(%p) should be true!", (void *)i);
  }
  for (uintptr_t i = 1; i + 1 != 0; i = (i << 1) | 1) {
    FIO_ASSERT(
        fio_ct_true(i) == 1, "fio_ct_true(%p) should be true!", (void *)i);
  }
  FIO_ASSERT(fio_ct_true((~0ULL)) == 1,
             "fio_ct_true(%p) should be true!",
             (void *)(~0ULL));

  FIO_ASSERT(fio_ct_false(0) == 1, "fio_ct_false(0) should be true!");
  for (uintptr_t i = 1; i; i <<= 1) {
    FIO_ASSERT(
        fio_ct_false(i) == 0, "fio_ct_false(%p) should be zero!", (void *)i);
  }
  for (uintptr_t i = 1; i + 1 != 0; i = (i << 1) | 1) {
    FIO_ASSERT(
        fio_ct_false(i) == 0, "fio_ct_false(%p) should be zero!", (void *)i);
  }
  FIO_ASSERT(fio_ct_false((~0ULL)) == 0,
             "fio_ct_false(%p) should be zero!",
             (void *)(~0ULL));
  FIO_ASSERT(fio_ct_true(8), "fio_ct_true should be true.");
  FIO_ASSERT(!fio_ct_true(0), "fio_ct_true should be false.");
  FIO_ASSERT(!fio_ct_false(8), "fio_ct_false should be false.");
  FIO_ASSERT(fio_ct_false(0), "fio_ct_false should be true.");
  FIO_ASSERT(fio_ct_if_bool(0, 1, 2) == 2,
             "fio_ct_if_bool selection error (false).");
  FIO_ASSERT(fio_ct_if_bool(1, 1, 2) == 1,
             "fio_ct_if_bool selection error (true).");
  FIO_ASSERT(fio_ct_if(0, 1, 2) == 2, "fio_ct_if selection error (false).");
  FIO_ASSERT(fio_ct_if(8, 1, 2) == 1, "fio_ct_if selection error (true).");
  {
    uint8_t bitmap[1024];
    memset(bitmap, 0, 1024);
    fprintf(stderr, "* Testing bitmap helpers.\n");
    FIO_ASSERT(!fio_bitmap_get(bitmap, 97), "fio_bitmap_get should be 0.");
    fio_bitmap_set(bitmap, 97);
    FIO_ASSERT(fio_bitmap_get(bitmap, 97) == 1,
               "fio_bitmap_get should be 1 after being set");
    FIO_ASSERT(!fio_bitmap_get(bitmap, 96),
               "other bits shouldn't be effected by set.");
    FIO_ASSERT(!fio_bitmap_get(bitmap, 98),
               "other bits shouldn't be effected by set.");
    fio_bitmap_flip(bitmap, 96);
    fio_bitmap_flip(bitmap, 97);
    FIO_ASSERT(!fio_bitmap_get(bitmap, 97),
               "fio_bitmap_get should be 0 after flip.");
    FIO_ASSERT(fio_bitmap_get(bitmap, 96) == 1,
               "other bits shouldn't be effected by flip");
    fio_bitmap_unset(bitmap, 96);
    fio_bitmap_flip(bitmap, 97);
    FIO_ASSERT(!fio_bitmap_get(bitmap, 96),
               "fio_bitmap_get should be 0 after unset.");
    FIO_ASSERT(fio_bitmap_get(bitmap, 97) == 1,
               "other bits shouldn't be effected by unset");
    fio_bitmap_unset(bitmap, 96);
  }
  {
    fprintf(stderr, "* Testing popcount and hemming distance calculation.\n");
    for (int i = 0; i < 64; ++i) {
      FIO_ASSERT(fio_popcount((uint64_t)1 << i) == 1,
                 "fio_popcount error for 1 bit");
    }
    for (int i = 0; i < 63; ++i) {
      FIO_ASSERT(fio_popcount((uint64_t)3 << i) == 2,
                 "fio_popcount error for 2 bits");
    }
    for (int i = 0; i < 62; ++i) {
      FIO_ASSERT(fio_popcount((uint64_t)7 << i) == 3,
                 "fio_popcount error for 3 bits");
    }
    for (int i = 0; i < 59; ++i) {
      FIO_ASSERT(fio_popcount((uint64_t)21 << i) == 3,
                 "fio_popcount error for 3 alternating bits");
    }
    for (int i = 0; i < 64; ++i) {
      FIO_ASSERT(fio_hemming_dist(((uint64_t)1 << i) - 1, 0) == i,
                 "fio_hemming_dist error at %d",
                 i);
    }
  }
  {
    struct test_s {
      int a;
      char force_padding;
      int b;
    } stst = {.a = 1};
    struct test_s *stst_p = FIO_PTR_FROM_FIELD(struct test_s, b, &stst.b);
    FIO_ASSERT(stst_p == &stst, "FIO_PTR_FROM_FIELD failed to retrace pointer");
  }
}
#endif /* FIO_TEST_CSTL */
/* *****************************************************************************
Bit-Byte operations - cleanup
***************************************************************************** */
#endif /* FIO_BITMAP */
#undef FIO_BITMAP
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "004 bitwise.h"            /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************










                        Risky Hash - a fast and simple hash










***************************************************************************** */

#if defined(FIO_RISKY_HASH) && !defined(H___FIO_RISKY_HASH_H)
#define H___FIO_RISKY_HASH_H

/* *****************************************************************************
Risky Hash - API
***************************************************************************** */

/**  Computes a facil.io Risky Hash (Risky v.3). */
SFUNC uint64_t fio_risky_hash(const void *buf, size_t len, uint64_t seed);

/**
 * Masks data using a Risky Hash and a counter mode nonce.
 *
 * Used for mitigating memory access attacks when storing "secret" information
 * in memory.
 *
 * Keep the nonce information in a different memory address then the secret. For
 * example, if the secret is on the stack, store the nonce on the heap or using
 * a static variable.
 *
 * Don't use the same nonce-secret combination for other data.
 *
 * This is NOT a cryptographically secure encryption. Even if the algorithm was
 * secure, it would provide no more then a 32bit level encryption, which isn't
 * strong enough for any cryptographic use-case.
 *
 * However, this could be used to mitigate memory probing attacks. Secrets
 * stored in the memory might remain accessible after the program exists or
 * through core dump information. By storing "secret" information masked in this
 * way, it mitigates the risk of secret information being recognized or
 * deciphered.
 */
IFUNC void fio_risky_mask(char *buf, size_t len, uint64_t key, uint64_t nonce);

/* *****************************************************************************
Risky Hash - Implementation

Note: I don't remember what information I used when designining this, but Risky
Hash is probably NOT cryptographically safe (though I wanted it to be).

Here's a few resources about hashes that might explain more:
- https://komodoplatform.com/cryptographic-hash-function/
- https://en.wikipedia.org/wiki/Avalanche_effect
- http://ticki.github.io/blog/designing-a-good-non-cryptographic-hash-function/

***************************************************************************** */

#ifdef FIO_EXTERN_COMPLETE

/* Risky Hash primes */
#define FIO_RISKY3_PRIME0 0xCAEF89D1E9A5EB21ULL
#define FIO_RISKY3_PRIME1 0xAB137439982B86C9ULL
#define FIO_RISKY3_PRIME2 0xD9FDC73ABE9EDECDULL
#define FIO_RISKY3_PRIME3 0x3532D520F9511B13ULL
#define FIO_RISKY3_PRIME4 0x038720DDEB5A8415ULL
/* Risky Hash initialization constants */
#define FIO_RISKY3_IV0 0x0000001000000001ULL
#define FIO_RISKY3_IV1 0x0000010000000010ULL
#define FIO_RISKY3_IV2 0x0000100000000100ULL
#define FIO_RISKY3_IV3 0x0001000000001000ULL
/* read u64 in little endian */
#define FIO_RISKY_BUF2U64 fio_buf2u64_little

#if 1 /* switch to 0 if the compiler's optimizer prefers arrays... */
/*  Computes a facil.io Risky Hash. */
SFUNC uint64_t fio_risky_hash(const void *data_, size_t len, uint64_t seed) {
  register uint64_t v0 = FIO_RISKY3_IV0;
  register uint64_t v1 = FIO_RISKY3_IV1;
  register uint64_t v2 = FIO_RISKY3_IV2;
  register uint64_t v3 = FIO_RISKY3_IV3;
  register uint64_t w0;
  register uint64_t w1;
  register uint64_t w2;
  register uint64_t w3;
  register const uint8_t *data = (const uint8_t *)data_;

#define FIO_RISKY3_ROUND64(vi, w_)                                             \
  w##vi = w_;                                                                  \
  v##vi += w##vi;                                                              \
  v##vi = fio_lrot64(v##vi, 29);                                               \
  v##vi += w##vi;                                                              \
  v##vi *= FIO_RISKY3_PRIME##vi;

#define FIO_RISKY3_ROUND256(w0, w1, w2, w3)                                    \
  FIO_RISKY3_ROUND64(0, w0);                                                   \
  FIO_RISKY3_ROUND64(1, w1);                                                   \
  FIO_RISKY3_ROUND64(2, w2);                                                   \
  FIO_RISKY3_ROUND64(3, w3);

  if (seed) {
    /* process the seed as if it was a prepended 8 Byte string. */
    v0 *= seed;
    v1 *= seed;
    v2 *= seed;
    v3 *= seed;
    v1 ^= seed;
    v2 ^= seed;
    v3 ^= seed;
  }

  for (size_t i = len >> 5; i; --i) {
    /* vectorized 32 bytes / 256 bit access */
    FIO_RISKY3_ROUND256(FIO_RISKY_BUF2U64(data),
                        FIO_RISKY_BUF2U64(data + 8),
                        FIO_RISKY_BUF2U64(data + 16),
                        FIO_RISKY_BUF2U64(data + 24));
    data += 32;
  }
  switch (len & 24) {
  case 24:
    FIO_RISKY3_ROUND64(2, FIO_RISKY_BUF2U64(data + 16));
    /* fallthrough */
  case 16:
    FIO_RISKY3_ROUND64(1, FIO_RISKY_BUF2U64(data + 8));
    /* fallthrough */
  case 8:
    FIO_RISKY3_ROUND64(0, FIO_RISKY_BUF2U64(data + 0));
    data += len & 24;
  }

  uint64_t tmp = (len & 0xFF) << 56; /* add offset information to padding */
  /* leftover bytes */
  switch ((len & 7)) {
  case 7:
    tmp |= ((uint64_t)data[6]) << 48; /* fallthrough */
  case 6:
    tmp |= ((uint64_t)data[5]) << 40; /* fallthrough */
  case 5:
    tmp |= ((uint64_t)data[4]) << 32; /* fallthrough */
  case 4:
    tmp |= ((uint64_t)data[3]) << 24; /* fallthrough */
  case 3:
    tmp |= ((uint64_t)data[2]) << 16; /* fallthrough */
  case 2:
    tmp |= ((uint64_t)data[1]) << 8; /* fallthrough */
  case 1:
    tmp |= ((uint64_t)data[0]);
    /* the last (now padded) byte's position */
    switch ((len & 24)) {
    case 24: /* offset 24 in 32 byte segment */
      FIO_RISKY3_ROUND64(3, tmp);
      break;
    case 16: /* offset 16 in 32 byte segment */
      FIO_RISKY3_ROUND64(2, tmp);
      break;
    case 8: /* offset 8 in 32 byte segment */
      FIO_RISKY3_ROUND64(1, tmp);
      break;
    case 0: /* offset 0 in 32 byte segment */
      FIO_RISKY3_ROUND64(0, tmp);
      break;
    }
  }

  /* irreversible avalanche... I think */
  uint64_t r = (len) ^ ((uint64_t)len << 36);
  r += fio_lrot64(v0, 17) + fio_lrot64(v1, 13) + fio_lrot64(v2, 47) +
       fio_lrot64(v3, 57);
  r += v0 ^ v1;
  r ^= fio_lrot64(r, 13);
  r += v1 ^ v2;
  r ^= fio_lrot64(r, 29);
  r += v2 ^ v3;
  r += fio_lrot64(r, 33);
  r += v3 ^ v0;
  r ^= fio_lrot64(r, 51);
  r ^= (r >> 29) * FIO_RISKY3_PRIME4;
  return r;
}
#else
/*  Computes a facil.io Risky Hash. */
SFUNC uint64_t fio_risky_hash(const void *data_, size_t len, uint64_t seed) {
  uint64_t v[4] = {
      FIO_RISKY3_IV0, FIO_RISKY3_IV1, FIO_RISKY3_IV2, FIO_RISKY3_IV3};
  uint64_t w[4];
  const uint8_t *data = (const uint8_t *)data_;

#define FIO_RISKY3_ROUND64(vi, w_)                                             \
  w[vi] = w_;                                                                  \
  v[vi] += w[vi];                                                              \
  v[vi] = fio_lrot64(v[vi], 29);                                               \
  v[vi] += w[vi];                                                              \
  v[vi] *= FIO_RISKY3_PRIME##vi;

#define FIO_RISKY3_ROUND256(w0, w1, w2, w3)                                    \
  FIO_RISKY3_ROUND64(0, w0);                                                   \
  FIO_RISKY3_ROUND64(1, w1);                                                   \
  FIO_RISKY3_ROUND64(2, w2);                                                   \
  FIO_RISKY3_ROUND64(3, w3);

  if (seed) {
    /* process the seed as if it was a prepended 8 Byte string. */
    v[0] *= seed;
    v[1] *= seed;
    v[2] *= seed;
    v[3] *= seed;
    v[1] ^= seed;
    v[2] ^= seed;
    v[3] ^= seed;
  }

  for (size_t i = len >> 5; i; --i) {
    /* vectorized 32 bytes / 256 bit access */
    FIO_RISKY3_ROUND256(FIO_RISKY_BUF2U64(data),
                        FIO_RISKY_BUF2U64(data + 8),
                        FIO_RISKY_BUF2U64(data + 16),
                        FIO_RISKY_BUF2U64(data + 24));
    data += 32;
  }
  switch (len & 24) {
  case 24:
    FIO_RISKY3_ROUND64(2, FIO_RISKY_BUF2U64(data + 16));
    /* fallthrough */
  case 16:
    FIO_RISKY3_ROUND64(1, FIO_RISKY_BUF2U64(data + 8));
    /* fallthrough */
  case 8:
    FIO_RISKY3_ROUND64(0, FIO_RISKY_BUF2U64(data + 0));
    data += len & 24;
  }

  uint64_t tmp = (len & 0xFF) << 56; /* add offset information to padding */
  /* leftover bytes */
  switch ((len & 7)) {
  case 7:
    tmp |= ((uint64_t)data[6]) << 48; /* fallthrough */
  case 6:
    tmp |= ((uint64_t)data[5]) << 40; /* fallthrough */
  case 5:
    tmp |= ((uint64_t)data[4]) << 32; /* fallthrough */
  case 4:
    tmp |= ((uint64_t)data[3]) << 24; /* fallthrough */
  case 3:
    tmp |= ((uint64_t)data[2]) << 16; /* fallthrough */
  case 2:
    tmp |= ((uint64_t)data[1]) << 8; /* fallthrough */
  case 1:
    tmp |= ((uint64_t)data[0]);
    /* the last (now padded) byte's position */
    switch ((len & 24)) {
    case 24: /* offset 24 in 32 byte segment */
      FIO_RISKY3_ROUND64(3, tmp);
      break;
    case 16: /* offset 16 in 32 byte segment */
      FIO_RISKY3_ROUND64(2, tmp);
      break;
    case 8: /* offset 8 in 32 byte segment */
      FIO_RISKY3_ROUND64(1, tmp);
      break;
    case 0: /* offset 0 in 32 byte segment */
      FIO_RISKY3_ROUND64(0, tmp);
      break;
    }
  }

  /* irreversible avalanche... I think */
  uint64_t r = (len) ^ ((uint64_t)len << 36);
  r += fio_lrot64(v[0], 17) + fio_lrot64(v[1], 13) + fio_lrot64(v[2], 47) +
       fio_lrot64(v[3], 57);
  r += v[0] ^ v[1];
  r ^= fio_lrot64(r, 13);
  r += v[1] ^ v[2];
  r ^= fio_lrot64(r, 29);
  r += v[2] ^ v[3];
  r += fio_lrot64(r, 33);
  r += v[3] ^ v[0];
  r ^= fio_lrot64(r, 51);
  r ^= (r >> 29) * FIO_RISKY3_PRIME4;
  return r;
}
#endif

/**
 * Masks data using a Risky Hash and a counter mode nonce.
 */
IFUNC void fio_risky_mask(char *buf, size_t len, uint64_t key, uint64_t nonce) {
  { /* avoid zero nonce, make sure nonce is effective and odd */
    nonce |= 1;
    nonce *= 0xDB1DD478B9E93B1ULL;
    nonce ^= ((nonce << 24) | (nonce >> 40));
    nonce |= 1;
  }
  uint64_t hash = fio_risky_hash(&key, sizeof(key), nonce);
  fio_xmask(buf, len, hash, nonce);
}
/* *****************************************************************************
Risky Hash - Cleanup
***************************************************************************** */
#undef FIO_RISKY3_ROUND64
#undef FIO_RISKY3_ROUND256
#undef FIO_RISKY_BUF2U64

#endif /* FIO_EXTERN_COMPLETE */
#endif
#undef FIO_RISKY_HASH

/* *****************************************************************************










                      Psedo-Random Generator Functions










***************************************************************************** */
#if defined(FIO_RAND) && !defined(H___FIO_RAND_H)
#define H___FIO_RAND_H
/* *****************************************************************************
Random - API
***************************************************************************** */

/** Returns 64 psedo-random bits. Probably not cryptographically safe. */
SFUNC uint64_t fio_rand64(void);

/** Writes `len` bytes of psedo-random bits to the target buffer. */
SFUNC void fio_rand_bytes(void *target, size_t len);

/** Feeds up to 1023 bytes of entropy to the random state. */
IFUNC void fio_rand_feed2seed(void *buf_, size_t len);

/** Reseeds the random engin using system state (rusage / jitter). */
IFUNC void fio_rand_reseed(void);

/* *****************************************************************************
Random - Implementation
***************************************************************************** */

#ifdef FIO_EXTERN_COMPLETE

#if FIO_HAVE_UNIX_TOOLS ||                                                     \
    (__has_include("sys/resource.h") && __has_include("sys/time.h"))
#include <sys/resource.h>
#include <sys/time.h>
#endif

static __thread uint64_t fio___rand_state[4]; /* random state */
static __thread size_t fio___rand_counter;    /* seed counter */
/* feeds random data to the algorithm through this 256 bit feed. */
static __thread uint64_t fio___rand_buffer[4] = {0x9c65875be1fce7b9ULL,
                                                 0x7cc568e838f6a40d,
                                                 0x4bb8d885a0fe47d5,
                                                 0x95561f0927ad7ecd};

IFUNC void fio_rand_feed2seed(void *buf_, size_t len) {
  len &= 1023;
  uint8_t *buf = (uint8_t *)buf_;
  uint8_t offset = (fio___rand_counter & 3);
  uint64_t tmp = 0;
  for (size_t i = 0; i < (len >> 3); ++i) {
    tmp = FIO_NAME2(fio_buf, u64_local)(buf);
    fio___rand_buffer[(offset++ & 3)] ^= tmp;
    buf += 8;
  }
  switch (len & 7) {
  case 7:
    tmp <<= 8;
    tmp |= buf[6];
    /* fallthrough */
  case 6:
    tmp <<= 8;
    tmp |= buf[5];
  /* fallthrough */
  case 5:
    tmp <<= 8;
    tmp |= buf[4];
  /* fallthrough */
  case 4:
    tmp <<= 8;
    tmp |= buf[3];
  /* fallthrough */
  case 3:
    tmp <<= 8;
    tmp |= buf[2];
  /* fallthrough */
  case 2:
    tmp <<= 8;
    tmp |= buf[1];
  /* fallthrough */
  case 1:
    tmp <<= 8;
    tmp |= buf[1];
    fio___rand_buffer[(offset & 3)] ^= tmp;
    break;
  }
}

/* used here, defined later */
FIO_IFUNC uint64_t fio_time_nano();

IFUNC void fio_rand_reseed(void) {
  const size_t jitter_samples = 16 | (fio___rand_state[0] & 15);
#if defined(RUSAGE_SELF)
  {
    struct rusage rusage;
    getrusage(RUSAGE_SELF, &rusage);
    fio___rand_state[0] =
        fio_risky_hash(&rusage, sizeof(rusage), fio___rand_state[0]);
  }
#endif
  for (size_t i = 0; i < jitter_samples; ++i) {
    uint64_t clk = fio_time_nano();
    fio___rand_state[0] =
        fio_risky_hash(&clk, sizeof(clk), fio___rand_state[0] + i);
    clk = fio_time_nano();
    fio___rand_state[1] = fio_risky_hash(
        &clk, sizeof(clk), fio___rand_state[1] + fio___rand_counter);
  }
  fio___rand_state[2] =
      fio_risky_hash(fio___rand_buffer,
                     sizeof(fio___rand_buffer),
                     fio___rand_counter + fio___rand_state[0]);
  fio___rand_state[3] = fio_risky_hash(fio___rand_state,
                                       sizeof(fio___rand_state),
                                       fio___rand_state[1] + jitter_samples);
  fio___rand_buffer[0] = fio_lrot64(fio___rand_buffer[0], 31);
  fio___rand_buffer[1] = fio_lrot64(fio___rand_buffer[1], 29);
  fio___rand_buffer[2] ^= fio___rand_buffer[0];
  fio___rand_buffer[3] ^= fio___rand_buffer[1];
  fio___rand_counter += jitter_samples;
}

/* tested for randomness using code from: http://xoshiro.di.unimi.it/hwd.php */
SFUNC uint64_t fio_rand64(void) {
  /* modeled after xoroshiro128+, by David Blackman and Sebastiano Vigna */
  const uint64_t P[] = {0x37701261ED6C16C7ULL, 0x764DBBB75F3B3E0DULL};
  if (((fio___rand_counter++) & (((size_t)1 << 19) - 1)) == 0) {
    /* re-seed state every 524,288 requests / 2^19-1 attempts  */
    fio_rand_reseed();
  }
  fio___rand_state[0] +=
      (fio_lrot64(fio___rand_state[0], 33) + fio___rand_counter) * P[0];
  fio___rand_state[1] += fio_lrot64(fio___rand_state[1], 33) * P[1];
  fio___rand_state[2] +=
      (fio_lrot64(fio___rand_state[2], 33) + fio___rand_counter) * (~P[0]);
  fio___rand_state[3] += fio_lrot64(fio___rand_state[3], 33) * (~P[1]);
  return fio_lrot64(fio___rand_state[0], 31) +
         fio_lrot64(fio___rand_state[1], 29) +
         fio_lrot64(fio___rand_state[2], 27) +
         fio_lrot64(fio___rand_state[3], 30);
}

/* copies 64 bits of randomness (8 bytes) repeatedly. */
SFUNC void fio_rand_bytes(void *data_, size_t len) {
  if (!data_ || !len)
    return;
  uint8_t *data = (uint8_t *)data_;

  if (len < 8)
    goto small_random;

  if ((uintptr_t)data & 7) {
    /* align pointer to 64 bit word */
    size_t offset = 8 - ((uintptr_t)data & 7);
    fio_rand_bytes(data_, offset); /* perform small_random */
    data += offset;
    len -= offset;
  }

  /* 128 random bits at a time */
  for (size_t i = (len >> 4); i; --i) {
    uint64_t t0 = fio_rand64();
    uint64_t t1 = fio_rand64();
    FIO_NAME2(fio_u, buf64_local)(data, t0);
    FIO_NAME2(fio_u, buf64_local)(data + 8, t1);
    data += 16;
  }
  /* 64 random bits at tail */
  if ((len & 8)) {
    uint64_t t0 = fio_rand64();
    FIO_NAME2(fio_u, buf64_local)(data, t0);
  }

small_random:
  if ((len & 7)) {
    /* leftover bits */
    uint64_t tmp = fio_rand64();
    /* leftover bytes */
    switch ((len & 7)) {
    case 7:
      data[6] = (tmp >> 8) & 0xFF;
      /* fallthrough */
    case 6:
      data[5] = (tmp >> 16) & 0xFF;
      /* fallthrough */
    case 5:
      data[4] = (tmp >> 24) & 0xFF;
      /* fallthrough */
    case 4:
      data[3] = (tmp >> 32) & 0xFF;
      /* fallthrough */
    case 3:
      data[2] = (tmp >> 40) & 0xFF;
      /* fallthrough */
    case 2:
      data[1] = (tmp >> 48) & 0xFF;
      /* fallthrough */
    case 1:
      data[0] = (tmp >> 56) & 0xFF;
    }
  }
}

/* *****************************************************************************
Hashing speed test
***************************************************************************** */
#ifdef FIO_TEST_CSTL
#include <math.h>

typedef uintptr_t (*fio__hashing_func_fn)(char *, size_t);

SFUNC void fio_test_hash_function(fio__hashing_func_fn h,
                                  char *name,
                                  uint8_t mem_alignment_ofset) {
#ifdef DEBUG
  fprintf(stderr,
          "* Testing %s speed "
          "(DEBUG mode detected - speed may be affected).\n",
          name);
  uint64_t cycles_start_at = (8192 << 4);
#else
  fprintf(stderr, "* Testing %s speed.\n", name);
  uint64_t cycles_start_at = (8192 << 8);
#endif
  /* test based on code from BearSSL with credit to Thomas Pornin */
  size_t const buffer_len = 8192;
  uint8_t buffer_[8200];
  uint8_t *buffer = buffer_ + (mem_alignment_ofset & 7);
  // uint64_t buffer[1024];
  memset(buffer, 'T', buffer_len);
  /* warmup */
  uint64_t hash = 0;
  for (size_t i = 0; i < 4; i++) {
    hash += h((char *)buffer, buffer_len);
    memcpy(buffer, &hash, sizeof(hash));
  }
  /* loop until test runs for more than 2 seconds */
  for (uint64_t cycles = cycles_start_at;;) {
    clock_t start, end;
    start = clock();
    for (size_t i = cycles; i > 0; i--) {
      hash += h((char *)buffer, buffer_len);
      __asm__ volatile("" ::: "memory");
    }
    end = clock();
    memcpy(buffer, &hash, sizeof(hash));
    if ((end - start) >= (2 * CLOCKS_PER_SEC) ||
        cycles >= ((uint64_t)1 << 62)) {
      fprintf(stderr,
              "\t%-40s %8.2f MB/s\n",
              name,
              (double)(buffer_len * cycles) /
                  (((end - start) * (1000000.0 / CLOCKS_PER_SEC))));
      break;
    }
    cycles <<= 1;
  }
}

FIO_SFUNC uintptr_t FIO_NAME_TEST(stl, risky_wrapper)(char *buf, size_t len) {
  return fio_risky_hash(buf, len, 1);
}

FIO_SFUNC uintptr_t FIO_NAME_TEST(stl, risky_mask_wrapper)(char *buf,
                                                           size_t len) {
  fio_risky_mask(buf, len, 0, 0);
  return len;
}

FIO_SFUNC void FIO_NAME_TEST(stl, risky)(void) {
  for (int i = 0; i < 8; ++i) {
    char buf[128];
    uint64_t nonce = fio_rand64();
    const char *str = "this is a short text, to test risky masking";
    char *tmp = buf + i;
    memcpy(tmp, str, strlen(str));
    fio_risky_mask(tmp, strlen(str), (uint64_t)tmp, nonce);
    FIO_ASSERT(memcmp(tmp, str, strlen(str)), "Risky Hash masking failed");
    size_t err = 0;
    for (size_t b = 0; b < strlen(str); ++b) {
      FIO_ASSERT(tmp[b] != str[b] || (err < 2),
                 "Risky Hash masking didn't mask buf[%zu] on offset "
                 "%d (statistical deviation?)",
                 b,
                 i);
      err += (tmp[b] == str[b]);
    }
    fio_risky_mask(tmp, strlen(str), (uint64_t)tmp, nonce);
    FIO_ASSERT(!memcmp(tmp, str, strlen(str)), "Risky Hash masking RT failed");
  }
  const uint8_t alignment_test_offset = 0;
  if (alignment_test_offset)
    fprintf(stderr,
            "The following speed tests use a memory alignment offset of %d "
            "bytes.\n",
            (int)(alignment_test_offset & 7));
  fio_test_hash_function(FIO_NAME_TEST(stl, risky_wrapper),
                         (char *)"fio_risky_hash",
                         alignment_test_offset);
  fio_test_hash_function(FIO_NAME_TEST(stl, risky_mask_wrapper),
                         (char *)"fio_risky_mask (Risky XOR + counter)",
                         alignment_test_offset);
  fio_test_hash_function(FIO_NAME_TEST(stl, risky_mask_wrapper),
                         (char *)"fio_risky_mask (unaligned)",
                         1);
}

FIO_SFUNC void FIO_NAME_TEST(stl, random_buffer)(uint64_t *stream,
                                                 size_t len,
                                                 const char *name,
                                                 size_t clk) {
  size_t totals[2] = {0};
  size_t freq[256] = {0};
  const size_t total_bits = (len * sizeof(*stream) * 8);
  uint64_t hemming = 0;
  /* collect data */
  for (size_t i = 1; i < len; i += 2) {
    hemming += fio_hemming_dist(stream[i], stream[i - 1]);
    for (size_t byte = 0; byte < (sizeof(*stream) << 1); ++byte) {
      uint8_t val = ((uint8_t *)(stream + (i - 1)))[byte];
      ++freq[val];
      for (int bit = 0; bit < 8; ++bit) {
        ++totals[(val >> bit) & 1];
      }
    }
  }
  hemming /= len;
  fprintf(stderr, "\n");
#if DEBUG
  fprintf(stderr,
          "\t- \x1B[1m%s\x1B[0m (%zu CPU cycles NOT OPTIMIZED):\n",
          name,
          clk);
#else
  fprintf(stderr, "\t- \x1B[1m%s\x1B[0m (%zu CPU cycles):\n", name, clk);
#endif
  fprintf(stderr,
          "\t  zeros / ones (bit frequency)\t%.05f\n",
          ((float)1.0 * totals[0]) / totals[1]);
  FIO_ASSERT(totals[0] < totals[1] + (total_bits / 20) &&
                 totals[1] < totals[0] + (total_bits / 20),
             "randomness isn't random?");
  fprintf(stderr, "\t  avarage hemming distance\t%zu\n", (size_t)hemming);
  /* expect avarage hemming distance of 25% == 16 bits */
  FIO_ASSERT(hemming >= 14 && hemming <= 18,
             "randomness isn't random (hemming distance failed)?");
  /* test chi-square ... I think */
  if (len * sizeof(*stream) > 2560) {
    double n_r = (double)1.0 * ((len * sizeof(*stream)) / 256);
    double chi_square = 0;
    for (unsigned int i = 0; i < 256; ++i) {
      double f = freq[i] - n_r;
      chi_square += (f * f);
    }
    chi_square /= n_r;
    double chi_square_r_abs =
        (chi_square - 256 >= 0) ? chi_square - 256 : (256 - chi_square);
    fprintf(
        stderr,
        "\t  chi-sq. variation\t\t%.02lf - %s (expect <= %0.2lf)\n",
        chi_square_r_abs,
        ((chi_square_r_abs <= 2 * (sqrt(n_r)))
             ? "good"
             : ((chi_square_r_abs <= 3 * (sqrt(n_r))) ? "not amazing"
                                                      : "\x1B[1mBAD\x1B[0m")),
        2 * (sqrt(n_r)));
  }
}

FIO_SFUNC void FIO_NAME_TEST(stl, random)(void) {
  fprintf(stderr,
          "* Testing randomness "
          "- bit frequency / hemming distance / chi-square.\n");
  const size_t test_len = (TEST_REPEAT << 7);
  uint64_t *rs = (uint64_t *)FIO_MEM_CALLOC(sizeof(*rs), test_len);
  clock_t start, end;
  FIO_ASSERT_ALLOC(rs);

  rand(); /* warmup */
  if (sizeof(int) < sizeof(uint64_t)) {
    start = clock();
    for (size_t i = 0; i < test_len; ++i) {
      rs[i] = ((uint64_t)rand() << 32) | (uint64_t)rand();
    }
    end = clock();
  } else {
    start = clock();
    for (size_t i = 0; i < test_len; ++i) {
      rs[i] = (uint64_t)rand();
    }
    end = clock();
  }
  FIO_NAME_TEST(stl, random_buffer)
  (rs, test_len, "rand (system - naive, ignoring missing bits)", end - start);

  memset(rs, 0, sizeof(*rs) * test_len);
  {
    if (RAND_MAX == ~(uint64_t)0ULL) {
      /* RAND_MAX fills all bits */
      start = clock();
      for (size_t i = 0; i < test_len; ++i) {
        rs[i] = (uint64_t)rand();
      }
      end = clock();
    } else if (RAND_MAX >= (~(uint32_t)0UL)) {
      /* RAND_MAX fill at least 32 bits per call */
      uint32_t *rs_adjusted = (uint32_t *)rs;
      start = clock();
      for (size_t i = 0; i < (test_len << 1); ++i) {
        rs_adjusted[i] = (uint32_t)rand();
      }
      end = clock();
    } else if (RAND_MAX >= (~(uint16_t)0U)) {
      /* RAND_MAX fill at least 16 bits per call */
      uint16_t *rs_adjusted = (uint16_t *)rs;
      start = clock();
      for (size_t i = 0; i < (test_len << 2); ++i) {
        rs_adjusted[i] = (uint16_t)rand();
      }
      end = clock();
    } else {
      /* assume RAND_MAX fill at least 8 bits per call */
      uint8_t *rs_adjusted = (uint8_t *)rs;
      start = clock();
      for (size_t i = 0; i < (test_len << 2); ++i) {
        rs_adjusted[i] = (uint8_t)rand();
      }
      end = clock();
    }
    /* test RAND_MAX value */
    uint8_t rand_bits = 63;
    while (rand_bits) {
      if (RAND_MAX <= (~(0ULL)) >> rand_bits)
        break;
      --rand_bits;
    }
    rand_bits = 64 - rand_bits;
    char buffer[128] = {0};
    snprintf(buffer,
             128 - 14,
             "rand (system - fixed, testing %d random bits)",
             (int)rand_bits);
    FIO_NAME_TEST(stl, random_buffer)(rs, test_len, buffer, end - start);
  }

  memset(rs, 0, sizeof(*rs) * test_len);
  fio_rand64(); /* warmup */
  start = clock();
  for (size_t i = 0; i < test_len; ++i) {
    rs[i] = fio_rand64();
  }
  end = clock();
  FIO_NAME_TEST(stl, random_buffer)(rs, test_len, "fio_rand64", end - start);
  memset(rs, 0, sizeof(*rs) * test_len);
  start = clock();
  fio_rand_bytes(rs, test_len * sizeof(*rs));
  end = clock();
  FIO_NAME_TEST(stl, random_buffer)
  (rs, test_len, "fio_rand_bytes", end - start);

  fio_rand_feed2seed(rs, sizeof(*rs) * test_len);
  FIO_MEM_FREE(rs, sizeof(*rs) * test_len);
  fprintf(stderr, "\n");
#if DEBUG
  fprintf(stderr,
          "\t- to compare CPU cycles, test randomness with optimization.\n\n");
#endif /* DEBUG */
}
#endif /* FIO_TEST_CSTL */
/* *****************************************************************************
Random - Cleanup
***************************************************************************** */
#endif /* FIO_EXTERN_COMPLETE */
#endif /* FIO_RAND */
#undef FIO_RAND
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************










                            String <=> Number helpers










***************************************************************************** */
#if defined(FIO_ATOL) && !defined(H___FIO_ATOL_H)
#define H___FIO_ATOL_H
#include <math.h>
/* *****************************************************************************
Strings to Numbers - API
***************************************************************************** */
/**
 * A helper function that converts between String data to a signed int64_t.
 *
 * Numbers are assumed to be in base 10. Octal (`0###`), Hex (`0x##`/`x##`) and
 * binary (`0b##`/ `b##`) are recognized as well. For binary Most Significant
 * Bit must come first.
 *
 * The most significant difference between this function and `strtol` (aside of
 * API design), is the added support for binary representations.
 */
SFUNC int64_t fio_atol(char **pstr);

/** A helper function that converts between String data to a signed double. */
SFUNC double fio_atof(char **pstr);

/* *****************************************************************************
Numbers to Strings - API
***************************************************************************** */

/**
 * A helper function that writes a signed int64_t to a string.
 *
 * No overflow guard is provided, make sure there's at least 68 bytes
 * available (for base 2).
 *
 * Offers special support for base 2 (binary), base 8 (octal), base 10 and base
 * 16 (hex). An unsupported base will silently default to base 10. Prefixes
 * are automatically added (i.e., "0x" for hex and "0b" for base 2).
 *
 * Returns the number of bytes actually written (excluding the NUL
 * terminator).
 */
SFUNC size_t fio_ltoa(char *dest, int64_t num, uint8_t base);

/**
 * A helper function that converts between a double to a string.
 *
 * No overflow guard is provided, make sure there's at least 130 bytes
 * available (for base 2).
 *
 * Supports base 2, base 10 and base 16. An unsupported base will silently
 * default to base 10. Prefixes aren't added (i.e., no "0x" or "0b" at the
 * beginning of the string).
 *
 * Returns the number of bytes actually written (excluding the NUL
 * terminator).
 */
SFUNC size_t fio_ftoa(char *dest, double num, uint8_t base);
/* *****************************************************************************
Strings to Numbers - Implementation
***************************************************************************** */
#ifdef FIO_EXTERN_COMPLETE

typedef struct {
  uint64_t val;
  int64_t expo;
  uint8_t sign;
} fio___number_s;

/** Reads number information in base 2. Returned expo in base 2. */
FIO_IFUNC fio___number_s fio___aton_read_b2_b2(char **pstr) {
  fio___number_s r = (fio___number_s){0};
  const uint64_t mask = ((1ULL) << ((sizeof(mask) << 3) - 1));
  while (**pstr >= '0' && **pstr <= '1' && !(r.val & mask)) {
    r.val = (r.val << 1) | (**pstr - '0');
    ++(*pstr);
  }
  while (**pstr >= '0' && **pstr <= '1') {
    ++r.expo;
    ++(*pstr);
  }
  return r;
}

/** Reads number information, up to base 10 numbers. Returned expo in `base`. */
FIO_IFUNC fio___number_s fio___aton_read_b2_b10(char **pstr, uint8_t base) {
  fio___number_s r = (fio___number_s){0};
  const uint64_t limit = ((~0ULL) / base) - (base - 1);
  while (**pstr >= '0' && **pstr < ('0' + base) && r.val <= (limit)) {
    r.val = (r.val * base) + (**pstr - '0');
    ++(*pstr);
  }
  while (**pstr >= '0' && **pstr < ('0' + base)) {
    ++r.expo;
    ++(*pstr);
  }
  return r;
}

/** Reads number information for base 16 (hex). Returned expo in base 4. */
FIO_IFUNC fio___number_s fio___aton_read_b2_b16(char **pstr) {
  fio___number_s r = (fio___number_s){0};
  const uint64_t mask = (~0ULL) << ((sizeof(mask) << 3) - 4);
  for (; !(r.val & mask);) {
    uint8_t tmp;
    if (**pstr >= '0' && **pstr <= '9')
      tmp = **pstr - '0';
    else if ((**pstr >= 'A' && **pstr <= 'F') ||
             (**pstr >= 'a' && **pstr <= 'f'))
      tmp = (**pstr | 32) - ('a' - 10);
    else
      return r;
    r.val = (r.val << 4) | tmp;
    ++(*pstr);
  }
  for (;;) {
    if ((**pstr >= '0' && **pstr <= '9') || (**pstr >= 'A' && **pstr <= 'F') ||
        (**pstr >= 'a' && **pstr <= 'f'))
      ++r.expo;
    else
      return r;
  }
  return r;
}

SFUNC int64_t fio_atol(char **pstr) {
  if (!pstr || !(*pstr))
    return 0;
  char *p = *pstr;
  unsigned char invert = 0;
  fio___number_s n = (fio___number_s){0};
  while ((int)(unsigned char)isspace(*p))
    ++p;
  if (*p == '-') {
    invert = 1;
    ++p;
  } else if (*p == '+') {
    ++p;
  }
  switch (*p) {
  case 'x': /* fallthrough */
  case 'X':
    goto is_hex;
  case 'b': /* fallthrough */
  case 'B':
    goto is_binary;
  case '0':
    ++p;
    switch (*p) {
    case 'x': /* fallthrough */
    case 'X':
      goto is_hex;
    case 'b': /* fallthrough */
    case 'B':
      goto is_binary;
    }
    goto is_base8;
  }

  /* is_base10: */
  *pstr = p;
  n = fio___aton_read_b2_b10(pstr, 10);

  /* sign can't be embeded */
#define CALC_N_VAL()                                                           \
  if (invert) {                                                                \
    if (n.expo || ((n.val << 1) && (n.val >> ((sizeof(n.val) << 3) - 1)))) {   \
      errno = E2BIG;                                                           \
      return (int64_t)(1ULL << ((sizeof(n.val) << 3) - 1));                    \
    }                                                                          \
    n.val = 0 - n.val;                                                         \
  } else {                                                                     \
    if (n.expo || (n.val >> ((sizeof(n.val) << 3) - 1))) {                     \
      errno = E2BIG;                                                           \
      return (int64_t)((~0ULL) >> 1);                                          \
    }                                                                          \
  }

  CALC_N_VAL();
  return n.val;

is_hex:
  ++p;
  while (*p == '0') {
    ++p;
  }
  *pstr = p;
  n = fio___aton_read_b2_b16(pstr);

  /* sign can be embeded */
#define CALC_N_VAL_EMBEDABLE()                                                 \
  if (invert) {                                                                \
    if (n.expo) {                                                              \
      errno = E2BIG;                                                           \
      return (int64_t)(1ULL << ((sizeof(n.val) << 3) - 1));                    \
    }                                                                          \
    n.val = 0 - n.val;                                                         \
  } else {                                                                     \
    if (n.expo) {                                                              \
      errno = E2BIG;                                                           \
      return (int64_t)((~0ULL) >> 1);                                          \
    }                                                                          \
  }

  CALC_N_VAL_EMBEDABLE();
  return n.val;

is_binary:
  ++p;
  while (*p == '0') {
    ++p;
  }
  *pstr = p;
  n = fio___aton_read_b2_b2(pstr);
  CALC_N_VAL_EMBEDABLE()
  return n.val;

is_base8:
  while (*p == '0') {
    ++p;
  }
  *pstr = p;
  n = fio___aton_read_b2_b10(pstr, 8);
  CALC_N_VAL();
  return n.val;
}

SFUNC double fio_atof(char **pstr) {
  if (!pstr || !(*pstr))
    return 0;
  if ((*pstr)[1] == 'b' || ((*pstr)[1] == '0' && (*pstr)[1] == 'b'))
    goto binary_raw;
  return strtod(*pstr, pstr);
binary_raw:
  /* binary representation is assumed to spell an exact double */
  (void)0;
  union {
    uint64_t i;
    double d;
  } punned = {.i = (uint64_t)fio_atol(pstr)};
  return punned.d;
}

/* *****************************************************************************
Numbers to Strings - Implementation
***************************************************************************** */

SFUNC size_t fio_ltoa(char *dest, int64_t num, uint8_t base) {
  // clang-format off
  const char notation[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  // clang-format on
  size_t len = 0;
  char buf[48]; /* we only need up to 20 for base 10, but base 3 needs 41... */

  if (!num)
    goto zero;

  switch (base) {
  case 1: /* fallthrough */
  case 2:
    /* Base 2 */
    {
      uint64_t n = num; /* avoid bit shifting inconsistencies with signed bit */
      uint8_t i = 0;    /* counting bits */
      dest[len++] = '0';
      dest[len++] = 'b';
#if __has_builtin(__builtin_clzll)
      i = __builtin_clzll(n);
      /* make sure the Binary representation doesn't appear signed */
      if (i) {
        --i;
        /*  keep it even */
        if ((i & 1))
          --i;
        n <<= i;
      }
#else
      while ((i < 64) && (n & 0x8000000000000000) == 0) {
        n = n << 1;
        i++;
      }
      /* make sure the Binary representation doesn't appear signed */
      if (i) {
        --i;
        n = n >> 1;
        /*  keep it even */
        if ((i & 1)) {
          --i;
          n = n >> 1;
        }
      }
#endif
      /* write to dest. */
      while (i < 64) {
        dest[len++] = ((n & 0x8000000000000000) ? '1' : '0');
        n = n << 1;
        i++;
      }
      dest[len] = 0;
      return len;
    }
  case 8:
    /* Base 8 */
    {
      uint64_t l = 0;
      if (num < 0) {
        dest[len++] = '-';
        num = 0 - num;
      }
      dest[len++] = '0';

      while (num) {
        buf[l++] = '0' + (num & 7);
        num = num >> 3;
      }
      while (l) {
        --l;
        dest[len++] = buf[l];
      }
      dest[len] = 0;
      return len;
    }

  case 16:
    /* Base 16 */
    {
      uint64_t n = num; /* avoid bit shifting inconsistencies with signed bit */
      uint8_t i = 0;    /* counting bits */
      dest[len++] = '0';
      dest[len++] = 'x';
      while ((n & 0xFF00000000000000) == 0) { // since n != 0, then i < 8
        n = n << 8;
        i++;
      }
      /* make sure the Hex representation doesn't appear misleadingly signed. */
      if (i && (n & 0x8000000000000000) && (n & 0x00FFFFFFFFFFFFFF)) {
        dest[len++] = '0';
        dest[len++] = '0';
      }
      /* write the damn thing, high to low */
      while (i < 8) {
        uint8_t tmp = (n & 0xF000000000000000) >> 60;
        uint8_t tmp2 = (n & 0x0F00000000000000) >> 56;
        dest[len++] = notation[tmp];
        dest[len++] = notation[tmp2];
        i++;
        n = n << 8;
      }
      dest[len] = 0;
      return len;
    }
  case 3: /* fallthrough */
  case 4: /* fallthrough */
  case 5: /* fallthrough */
  case 6: /* fallthrough */
  case 7: /* fallthrough */
  case 9: /* fallthrough */
    /* rare bases */
    {
      int64_t t = num / base;
      uint64_t l = 0;
      if (num < 0) {
        num = 0 - num; /* might fail due to overflow, but fixed with tail (t) */
        t = (int64_t)0 - t;
        dest[len++] = '-';
      }
      while (num) {
        buf[l++] = '0' + (num - (t * base));
        num = t;
        t = num / base;
      }
      while (l) {
        --l;
        dest[len++] = buf[l];
      }
      dest[len] = 0;
      return len;
    }

  default:
    break;
  }
  /* Base 10, the default base */
  {
    int64_t t = num / 10;
    uint64_t l = 0;
    if (num < 0) {
      num = 0 - num; /* might fail due to overflow, but fixed with tail (t) */
      t = (int64_t)0 - t;
      dest[len++] = '-';
    }
    while (num) {
      buf[l++] = '0' + (num - (t * 10));
      num = t;
      t = num / 10;
    }
    while (l) {
      --l;
      dest[len++] = buf[l];
    }
    dest[len] = 0;
    return len;
  }

zero:
  switch (base) {
  case 1:
  case 2:
    dest[len++] = '0';
    dest[len++] = 'b';
    break;
  case 8:
    dest[len++] = '0';
    break;
  case 16:
    dest[len++] = '0';
    dest[len++] = 'x';
    dest[len++] = '0';
    break;
  }
  dest[len++] = '0';
  dest[len] = 0;
  return len;
}

SFUNC size_t fio_ftoa(char *dest, double num, uint8_t base) {
  if (base == 2 || base == 16) {
    /* handle binary / Hex representation the same as an int64_t */
    /* FIXME: Hex representation should use floating-point hex instead */
    union {
      int64_t i;
      double d;
    } p;
    p.d = num;
    return fio_ltoa(dest, p.i, base);
  }
  size_t written = 0;
  uint8_t need_zero = 1;
  char *start = dest;

  if (isinf(num))
    goto is_inifinity;
  if (isnan(num))
    goto is_nan;

  written = sprintf(dest, "%g", num);
  while (*start) {
    if (*start == 'e')
      goto finish;
    if (*start == ',') // locale issues?
      *start = '.';
    if (*start == '.') {
      need_zero = 0;
    }
    start++;
  }
  if (need_zero) {
    dest[written++] = '.';
    dest[written++] = '0';
  }

finish:
  dest[written] = 0;
  return written;

is_inifinity:
  if (num < 0)
    dest[written++] = '-';
  memcpy(dest + written, "Infinity", 9);
  return written + 8;
is_nan:
  memcpy(dest, "NaN", 4);
  return 3;
}

/* *****************************************************************************
Numbers <=> Strings - Testing
***************************************************************************** */

#ifdef FIO_TEST_CSTL

FIO_SFUNC void FIO_NAME_TEST(stl, atol)(void) {
  fprintf(stderr, "* Testing fio_atol and fio_ltoa.\n");
  char buffer[1024];
  for (int i = 0 - TEST_REPEAT; i < TEST_REPEAT; ++i) {
    size_t tmp = fio_ltoa(buffer, i, 0);
    FIO_ASSERT(tmp > 0, "fio_ltoa returned length error");
    buffer[tmp++] = 0;
    char *tmp2 = buffer;
    int i2 = fio_atol(&tmp2);
    FIO_ASSERT(tmp2 > buffer, "fio_atol pointer motion error");
    FIO_ASSERT(
        i == i2, "fio_ltoa-fio_atol roundtrip error %lld != %lld", i, i2);
  }
  for (size_t bit = 0; bit < sizeof(int64_t) * 8; ++bit) {
    uint64_t i = (uint64_t)1 << bit;
    size_t tmp = fio_ltoa(buffer, (int64_t)i, 0);
    FIO_ASSERT(tmp > 0, "fio_ltoa return length error");
    buffer[tmp] = 0;
    char *tmp2 = buffer;
    int64_t i2 = fio_atol(&tmp2);
    FIO_ASSERT(tmp2 > buffer, "fio_atol pointer motion error");
    FIO_ASSERT((int64_t)i == i2,
               "fio_ltoa-fio_atol roundtrip error %lld != %lld",
               i,
               i2);
  }
  fprintf(stderr, "* Testing fio_atol samples.\n");
#define TEST_ATOL(s, n)                                                        \
  do {                                                                         \
    char *p = (char *)(s);                                                     \
    int64_t r = fio_atol(&p);                                                  \
    FIO_ASSERT(r == (n),                                                       \
               "fio_atol test error! %s => %zd (not %zd)",                     \
               ((char *)(s)),                                                  \
               (size_t)r,                                                      \
               (size_t)n);                                                     \
    FIO_ASSERT((s) + strlen((s)) == p,                                         \
               "fio_atol test error! %s reading position not at end (%zu)",    \
               (s),                                                            \
               (size_t)(p - (s)));                                             \
    char buf[72];                                                              \
    buf[fio_ltoa(buf, n, 2)] = 0;                                              \
    p = buf;                                                                   \
    FIO_ASSERT(fio_atol(&p) == (n),                                            \
               "fio_ltoa base 2 test error! "                                  \
               "%s != %s (%zd)",                                               \
               buf,                                                            \
               ((char *)(s)),                                                  \
               (size_t)((p = buf), fio_atol(&p)));                             \
    buf[fio_ltoa(buf, n, 8)] = 0;                                              \
    p = buf;                                                                   \
    FIO_ASSERT(fio_atol(&p) == (n),                                            \
               "fio_ltoa base 8 test error! "                                  \
               "%s != %s (%zd)",                                               \
               buf,                                                            \
               ((char *)(s)),                                                  \
               (size_t)((p = buf), fio_atol(&p)));                             \
    buf[fio_ltoa(buf, n, 10)] = 0;                                             \
    p = buf;                                                                   \
    FIO_ASSERT(fio_atol(&p) == (n),                                            \
               "fio_ltoa base 10 test error! "                                 \
               "%s != %s (%zd)",                                               \
               buf,                                                            \
               ((char *)(s)),                                                  \
               (size_t)((p = buf), fio_atol(&p)));                             \
    buf[fio_ltoa(buf, n, 16)] = 0;                                             \
    p = buf;                                                                   \
    FIO_ASSERT(fio_atol(&p) == (n),                                            \
               "fio_ltoa base 16 test error! "                                 \
               "%s != %s (%zd)",                                               \
               buf,                                                            \
               ((char *)(s)),                                                  \
               (size_t)((p = buf), fio_atol(&p)));                             \
  } while (0)

  TEST_ATOL("0x1", 1);
  TEST_ATOL("-0x1", -1);
  TEST_ATOL("-0xa", -10);                                /* sign before hex */
  TEST_ATOL("0xe5d4c3b2a1908770", -1885667171979196560); /* sign within hex */
  TEST_ATOL("0b00000000000011", 3);
  TEST_ATOL("-0b00000000000011", -3);
  TEST_ATOL("0b0000000000000000000000000000000000000000000000000", 0);
  TEST_ATOL("0", 0);
  TEST_ATOL("1", 1);
  TEST_ATOL("2", 2);
  TEST_ATOL("-2", -2);
  TEST_ATOL("0000000000000000000000000000000000000000000000042", 34); /* oct */
  TEST_ATOL("9223372036854775807", 9223372036854775807LL); /* INT64_MAX */
  TEST_ATOL("9223372036854775808",
            9223372036854775807LL); /* INT64_MAX overflow protection */
  TEST_ATOL("9223372036854775999",
            9223372036854775807LL); /* INT64_MAX overflow protection */
#undef TEST_ATOL

#ifdef FIO_ATOF_ALT
#define TEST_DOUBLE(s, d, stop)                                                \
  do {                                                                         \
    union {                                                                    \
      double d_;                                                               \
      uint64_t as_i;                                                           \
    } pn, pn2;                                                                 \
    pn2.d_ = d;                                                                \
    char *p = (char *)(s);                                                     \
    char *p2 = (char *)(s);                                                    \
    double r = fio_atof(&p);                                                   \
    double std = strtod(p2, &p2);                                              \
    (void)std;                                                                 \
    pn.d_ = r;                                                                 \
    FIO_ASSERT(*p == stop || p == p2,                                          \
               "float parsing didn't stop at correct possition! %x != %x",     \
               *p,                                                             \
               stop);                                                          \
    if ((double)d == r || r == std) {                                          \
      /** fprintf(stderr, "Okay for %s\n", s); */                              \
    } else if ((pn2.as_i + 1) == (pn.as_i) || (pn.as_i + 1) == pn2.as_i) {     \
      fprintf(                                                                 \
          stderr, "* WARNING: Single bit rounding error detected: %s\n", s);   \
    } else if (r == 0.0 && d != 0.0) {                                         \
      fprintf(stderr, "* WARNING: float range limit marked before: %s\n", s);  \
    } else {                                                                   \
      char f_buf[164];                                                         \
      pn.d_ = std;                                                             \
      pn2.d_ = r;                                                              \
      size_t tmp_pos = fio_ltoa(f_buf, pn.as_i, 2);                            \
      f_buf[tmp_pos] = '\n';                                                   \
      fio_ltoa(f_buf + tmp_pos + 1, pn2.as_i, 2);                              \
      FIO_ASSERT(0,                                                            \
                 "Float error bigger than a single bit rounding error. exp. "  \
                 "vs. act.:\n%.19g\n%.19g\nBinary:\n%s",                       \
                 std,                                                          \
                 r,                                                            \
                 f_buf);                                                       \
    }                                                                          \
  } while (0)

  fprintf(stderr, "* Testing fio_atof samples.\n");

  /* A few hex-float examples  */
  TEST_DOUBLE("0x10.1p0", 0x10.1p0, 0);
  TEST_DOUBLE("0x1.8p1", 0x1.8p1, 0);
  TEST_DOUBLE("0x1.8p5", 0x1.8p5, 0);
  TEST_DOUBLE("0x4.0p5", 0x4.0p5, 0);
  TEST_DOUBLE("0x1.0p50a", 0x1.0p50, 'a');
  TEST_DOUBLE("0x1.0p500", 0x1.0p500, 0);
  TEST_DOUBLE("0x1.0P-1074", 0x1.0P-1074, 0);
  TEST_DOUBLE("0x3a.0P-1074", 0x3a.0P-1074, 0);

  /* These numbers were copied from https://gist.github.com/mattn/1890186 */
  TEST_DOUBLE(".1", 0.1, 0);
  TEST_DOUBLE("  .", 0, 0);
  TEST_DOUBLE("  1.2e3", 1.2e3, 0);
  TEST_DOUBLE(" +1.2e3", 1.2e3, 0);
  TEST_DOUBLE("1.2e3", 1.2e3, 0);
  TEST_DOUBLE("+1.2e3", 1.2e3, 0);
  TEST_DOUBLE("+1.e3", 1000, 0);
  TEST_DOUBLE("-1.2e3", -1200, 0);
  TEST_DOUBLE("-1.2e3.5", -1200, '.');
  TEST_DOUBLE("-1.2e", -1.2, 0);
  TEST_DOUBLE("--1.2e3.5", 0, '-');
  TEST_DOUBLE("--1-.2e3.5", 0, '-');
  TEST_DOUBLE("-a", 0, 'a');
  TEST_DOUBLE("a", 0, 'a');
  TEST_DOUBLE(".1e", 0.1, 0);
  TEST_DOUBLE(".1e3", 100, 0);
  TEST_DOUBLE(".1e-3", 0.1e-3, 0);
  TEST_DOUBLE(".1e-", 0.1, 0);
  TEST_DOUBLE(" .e-", 0, 0);
  TEST_DOUBLE(" .e", 0, 0);
  TEST_DOUBLE(" e", 0, 0);
  TEST_DOUBLE(" e0", 0, 0);
  TEST_DOUBLE(" ee", 0, 'e');
  TEST_DOUBLE(" -e", 0, 0);
  TEST_DOUBLE(" .9", 0.9, 0);
  TEST_DOUBLE(" ..9", 0, '.');
  TEST_DOUBLE("009", 9, 0);
  TEST_DOUBLE("0.09e02", 9, 0);
  /* http://thread.gmane.org/gmane.editors.vim.devel/19268/ */
  TEST_DOUBLE("0.9999999999999999999999999999999999", 1, 0);
  TEST_DOUBLE("2.2250738585072010e-308", 2.225073858507200889e-308, 0);
  TEST_DOUBLE("2.2250738585072013e-308", 2.225073858507201383e-308, 0);
  TEST_DOUBLE("9214843084008499", 9214843084008499, 0);
  TEST_DOUBLE("30078505129381147446200", 3.007850512938114954e+22, 0);

  /* These numbers were copied from https://github.com/miloyip/rapidjson */
  TEST_DOUBLE("0.0", 0.0, 0);
  TEST_DOUBLE("-0.0", -0.0, 0);
  TEST_DOUBLE("1.0", 1.0, 0);
  TEST_DOUBLE("-1.0", -1.0, 0);
  TEST_DOUBLE("1.5", 1.5, 0);
  TEST_DOUBLE("-1.5", -1.5, 0);
  TEST_DOUBLE("3.1416", 3.1416, 0);
  TEST_DOUBLE("1E10", 1E10, 0);
  TEST_DOUBLE("1e10", 1e10, 0);
  TEST_DOUBLE("100000000000000000000000000000000000000000000000000000000000"
              "000000000000000000000",
              1E80,
              0);
  TEST_DOUBLE("1E+10", 1E+10, 0);
  TEST_DOUBLE("1E-10", 1E-10, 0);
  TEST_DOUBLE("-1E10", -1E10, 0);
  TEST_DOUBLE("-1e10", -1e10, 0);
  TEST_DOUBLE("-1E+10", -1E+10, 0);
  TEST_DOUBLE("-1E-10", -1E-10, 0);
  TEST_DOUBLE("1.234E+10", 1.234E+10, 0);
  TEST_DOUBLE("1.234E-10", 1.234E-10, 0);
  TEST_DOUBLE("1.79769e+308", 1.79769e+308, 0);
  TEST_DOUBLE("2.22507e-308", 2.22507e-308, 0);
  TEST_DOUBLE("-1.79769e+308", -1.79769e+308, 0);
  TEST_DOUBLE("-2.22507e-308", -2.22507e-308, 0);
  TEST_DOUBLE("4.9406564584124654e-324", 4.9406564584124654e-324, 0);
  TEST_DOUBLE("2.2250738585072009e-308", 2.2250738585072009e-308, 0);
  TEST_DOUBLE("2.2250738585072014e-308", 2.2250738585072014e-308, 0);
  TEST_DOUBLE("1.7976931348623157e+308", 1.7976931348623157e+308, 0);
  TEST_DOUBLE("1e-10000", 0.0, 0);
  TEST_DOUBLE("18446744073709551616", 18446744073709551616.0, 0);

  TEST_DOUBLE("-9223372036854775809", -9223372036854775809.0, 0);

  TEST_DOUBLE("0.9868011474609375", 0.9868011474609375, 0);
  TEST_DOUBLE("123e34", 123e34, 0);
  TEST_DOUBLE("45913141877270640000.0", 45913141877270640000.0, 0);
  TEST_DOUBLE("2.2250738585072011e-308", 2.2250738585072011e-308, 0);
  TEST_DOUBLE("1e-214748363", 0.0, 0);
  TEST_DOUBLE("1e-214748364", 0.0, 0);
  TEST_DOUBLE("0.017976931348623157e+310, 1", 1.7976931348623157e+308, ',');

  TEST_DOUBLE("2.2250738585072012e-308", 2.2250738585072014e-308, 0);
  TEST_DOUBLE("2.22507385850720113605740979670913197593481954635164565e-308",
              2.2250738585072014e-308,
              0);

  TEST_DOUBLE(
      "0.999999999999999944488848768742172978818416595458984375", 1.0, 0);
  TEST_DOUBLE(
      "0.999999999999999944488848768742172978818416595458984376", 1.0, 0);
  TEST_DOUBLE(
      "1.00000000000000011102230246251565404236316680908203125", 1.0, 0);
  TEST_DOUBLE(
      "1.00000000000000011102230246251565404236316680908203124", 1.0, 0);

  TEST_DOUBLE("72057594037927928.0", 72057594037927928.0, 0);
  TEST_DOUBLE("72057594037927936.0", 72057594037927936.0, 0);
  TEST_DOUBLE("72057594037927932.0", 72057594037927936.0, 0);
  TEST_DOUBLE("7205759403792793200001e-5", 72057594037927936.0, 0);

  TEST_DOUBLE("9223372036854774784.0", 9223372036854774784.0, 0);
  TEST_DOUBLE("9223372036854775808.0", 9223372036854775808.0, 0);
  TEST_DOUBLE("9223372036854775296.0", 9223372036854775808.0, 0);
  TEST_DOUBLE("922337203685477529600001e-5", 9223372036854775808.0, 0);

  TEST_DOUBLE("10141204801825834086073718800384",
              10141204801825834086073718800384.0,
              0);
  TEST_DOUBLE("10141204801825835211973625643008",
              10141204801825835211973625643008.0,
              0);
  TEST_DOUBLE("10141204801825834649023672221696",
              10141204801825835211973625643008.0,
              0);
  TEST_DOUBLE("1014120480182583464902367222169600001e-5",
              10141204801825835211973625643008.0,
              0);

  TEST_DOUBLE("5708990770823838890407843763683279797179383808",
              5708990770823838890407843763683279797179383808.0,
              0);
  TEST_DOUBLE("5708990770823839524233143877797980545530986496",
              5708990770823839524233143877797980545530986496.0,
              0);
  TEST_DOUBLE("5708990770823839207320493820740630171355185152",
              5708990770823839524233143877797980545530986496.0,
              0);
  TEST_DOUBLE("5708990770823839207320493820740630171355185152001e-3",
              5708990770823839524233143877797980545530986496.0,
              0);
#undef TEST_DOUBLE
#if !DEBUG
  {
    clock_t start, stop;
    memcpy(buffer, "1234567890.123", 14);
    buffer[14] = 0;
    size_t r = 0;
    start = clock();
    for (int i = 0; i < (TEST_REPEAT << 3); ++i) {
      char *pos = buffer;
      r += fio_atol(&pos);
      __asm__ volatile("" ::: "memory");
      // FIO_ASSERT(r == exp, "fio_atol failed during speed test");
    }
    stop = clock();
    fprintf(stderr,
            "* fio_atol speed test completed in %zu cycles\n",
            stop - start);
    r = 0;
    start = clock();
    for (int i = 0; i < (TEST_REPEAT << 3); ++i) {
      char *pos = buffer;
      r += strtol(pos, NULL, 10);
      __asm__ volatile("" ::: "memory");
      // FIO_ASSERT(r == exp, "system strtol failed during speed test");
    }
    stop = clock();
    fprintf(stderr,
            "* system atol speed test completed in %zu cycles\n",
            stop - start);
  }
#endif /* !DEBUG */
#endif /* FIO_ATOF_ALT */
}
#endif /* FIO_TEST_CSTL */

/* *****************************************************************************
Numbers <=> Strings - Cleanup
***************************************************************************** */
#endif /* FIO_EXTERN_COMPLETE */
#endif /* FIO_ATOL */
#undef FIO_ATOL
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************













                                  URI Parsing













***************************************************************************** */
#if (defined(FIO_URL) || defined(FIO_URI)) && !defined(H___FIO_URL___H)
#define H___FIO_URL___H
/** the result returned by `fio_url_parse` */
typedef struct {
  fio_str_info_s scheme;
  fio_str_info_s user;
  fio_str_info_s password;
  fio_str_info_s host;
  fio_str_info_s port;
  fio_str_info_s path;
  fio_str_info_s query;
  fio_str_info_s target;
} fio_url_s;

/**
 * Parses the URI returning it's components and their lengths (no decoding
 * performed, doesn't accept decoded URIs).
 *
 * The returned string are NOT NUL terminated, they are merely locations within
 * the original string.
 *
 * This function attempts to accept many different formats, including any of the
 * following:
 *
 * * `/complete_path?query#target`
 *
 *   i.e.: /index.html?page=1#list
 *
 * * `host:port/complete_path?query#target`
 *
 *   i.e.:
 *      example.com
 *      example.com:8080
 *      example.com/index.html
 *      example.com:8080/index.html
 *      example.com:8080/index.html?key=val#target
 *
 * * `user:password@host:port/path?query#target`
 *
 *   i.e.: user:1234@example.com:8080/index.html
 *
 * * `username[:password]@host[:port][...]`
 *
 *   i.e.: john:1234@example.com
 *
 * * `schema://user:password@host:port/path?query#target`
 *
 *   i.e.: http://example.com/index.html?page=1#list
 *
 * Invalid formats might produce unexpected results. No error testing performed.
 */
SFUNC fio_url_s fio_url_parse(const char *url, size_t len);

/* *****************************************************************************
FIO_URL - Implementation
***************************************************************************** */
#if defined(FIO_EXTERN_COMPLETE)

/**
 * Parses the URI returning it's components and their lengths (no decoding
 * performed, doesn't accept decoded URIs).
 *
 * The returned string are NOT NUL terminated, they are merely locations within
 * the original string.
 *
 * This function expects any of the following formats:
 *
 * * `/complete_path?query#target`
 *
 *   i.e.: /index.html?page=1#list
 *
 * * `host:port/complete_path?query#target`
 *
 *   i.e.:
 *      example.com/index.html
 *      example.com:8080/index.html
 *
 * * `schema://user:password@host:port/path?query#target`
 *
 *   i.e.: http://example.com/index.html?page=1#list
 *
 * Invalid formats might produce unexpected results. No error testing performed.
 */
SFUNC fio_url_s fio_url_parse(const char *url, size_t len) {
  /*
  Intention:
  [schema://][user[:]][password[@]][host.com[:/]][:port/][/path][?quary][#target]
  */
  const char *end = url + len;
  const char *pos = url;
  fio_url_s r = {.scheme = {.buf = (char *)url}};
  if (len == 0) {
    goto finish;
  }

  if (pos[0] == '/') {
    /* start at path */
    goto start_path;
  }

  while (pos < end && pos[0] != ':' && pos[0] != '/' && pos[0] != '@' &&
         pos[0] != '#' && pos[0] != '?')
    ++pos;

  if (pos == end) {
    /* was only host (path starts with '/') */
    r.host = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};
    goto finish;
  }
  switch (pos[0]) {
  case '@':
    /* username@[host] */
    r.user = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};
    ++pos;
    goto start_host;
  case '/':
    /* host[/path] */
    r.host = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};
    goto start_path;
  case '?':
    /* host?[query] */
    r.host = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};
    ++pos;
    goto start_query;
  case '#':
    /* host#[target] */
    r.host = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};
    ++pos;
    goto start_target;
  case ':':
    if (pos + 2 <= end && pos[1] == '/' && pos[2] == '/') {
      /* scheme:// */
      r.scheme.len = pos - url;
      pos += 3;
    } else {
      /* username:[password] OR */
      /* host:[port] */
      r.user = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};
      ++pos;
      goto start_password;
    }
    break;
  }

  // start_username:
  url = pos;
  while (pos < end && pos[0] != ':' && pos[0] != '/' && pos[0] != '@'
         /* && pos[0] != '#' && pos[0] != '?' */)
    ++pos;

  if (pos >= end) { /* scheme://host */
    r.host = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};
    goto finish;
  }

  switch (pos[0]) {
  case '/':
    /* scheme://host[/path] */
    r.host = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};
    goto start_path;
  case '@':
    /* scheme://username@[host]... */
    r.user = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};
    ++pos;
    goto start_host;
  case ':':
    /* scheme://username:[password]@[host]... OR */
    /* scheme://host:[port][/...] */
    r.user = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};
    ++pos;
    break;
  }

start_password:
  url = pos;
  while (pos < end && pos[0] != '/' && pos[0] != '@')
    ++pos;

  if (pos >= end) {
    /* was host:port */
    r.port = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};
    r.host = r.user;
    r.user.len = 0;
    goto finish;
    ;
  }

  switch (pos[0]) {
  case '/':
    r.port = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};
    r.host = r.user;
    r.user.len = 0;
    goto start_path;
  case '@':
    r.password =
        (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};
    ++pos;
    break;
  }

start_host:
  url = pos;
  while (pos < end && pos[0] != '/' && pos[0] != ':' && pos[0] != '#' &&
         pos[0] != '?')
    ++pos;

  r.host = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};
  if (pos >= end) {
    goto finish;
  }
  switch (pos[0]) {
  case '/':
    /* scheme://[...@]host[/path] */
    goto start_path;
  case '?':
    /* scheme://[...@]host?[query] (bad)*/
    ++pos;
    goto start_query;
  case '#':
    /* scheme://[...@]host#[target] (bad)*/
    ++pos;
    goto start_target;
    // case ':':
    /* scheme://[...@]host:[port] */
  }
  ++pos;

  // start_port:
  url = pos;
  while (pos < end && pos[0] != '/' && pos[0] != '#' && pos[0] != '?')
    ++pos;

  r.port = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};

  if (pos >= end) {
    /* scheme://[...@]host:port */
    goto finish;
  }
  switch (pos[0]) {
  case '?':
    /* scheme://[...@]host:port?[query] (bad)*/
    ++pos;
    goto start_query;
  case '#':
    /* scheme://[...@]host:port#[target] (bad)*/
    ++pos;
    goto start_target;
    // case '/':
    /* scheme://[...@]host:port[/path] */
  }

start_path:
  url = pos;
  while (pos < end && pos[0] != '#' && pos[0] != '?')
    ++pos;

  r.path = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};

  if (pos >= end) {
    goto finish;
  }
  ++pos;
  if (pos[-1] == '#')
    goto start_target;

start_query:
  url = pos;
  while (pos < end && pos[0] != '#')
    ++pos;

  r.query = (fio_str_info_s){.buf = (char *)url, .len = (size_t)(pos - url)};
  ++pos;

  if (pos >= end)
    goto finish;

start_target:
  r.target = (fio_str_info_s){.buf = (char *)pos, .len = (size_t)(end - pos)};

finish:

  if (r.scheme.len == 4 && r.host.buf &&
      (((r.scheme.buf[0] | 32) == 'f' && (r.scheme.buf[1] | 32) == 'i' &&
        (r.scheme.buf[2] | 32) == 'l' && (r.scheme.buf[3] | 32) == 'e') ||
       ((r.scheme.buf[0] | 32) == 'u' && (r.scheme.buf[1] | 32) == 'n' &&
        (r.scheme.buf[2] | 32) == 'i' && (r.scheme.buf[3] | 32) == 'x'))) {
    r.path.len += (r.path.buf - (r.scheme.buf + 7));
    r.path.buf = r.scheme.buf + 7;
    r.user.len = r.password.len = r.port.len = r.host.len = 0;
  }

  /* set any empty values to NULL */
  if (!r.scheme.len)
    r.scheme.buf = NULL;
  if (!r.user.len)
    r.user.buf = NULL;
  if (!r.password.len)
    r.password.buf = NULL;
  if (!r.host.len)
    r.host.buf = NULL;
  if (!r.port.len)
    r.port.buf = NULL;
  if (!r.path.len)
    r.path.buf = NULL;
  if (!r.query.len)
    r.query.buf = NULL;
  if (!r.target.len)
    r.target.buf = NULL;

  return r;
}

/* *****************************************************************************
URL parsing - Test
***************************************************************************** */
#ifdef FIO_TEST_CSTL

/* Test for URI variations:
 *
 * * `/complete_path?query#target`
 *
 *   i.e.: /index.html?page=1#list
 *
 * * `host:port/complete_path?query#target`
 *
 *   i.e.:
 *      example.com
 *      example.com:8080
 *      example.com/index.html
 *      example.com:8080/index.html
 *      example.com:8080/index.html?key=val#target
 *
 * * `user:password@host:port/path?query#target`
 *
 *   i.e.: user:1234@example.com:8080/index.html
 *
 * * `username[:password]@host[:port][...]`
 *
 *   i.e.: john:1234@example.com
 *
 * * `schema://user:password@host:port/path?query#target`
 *
 *   i.e.: http://example.com/index.html?page=1#list
 */
TEST_FUNC void FIO_NAME_TEST(stl, url)(void) {
  fprintf(stderr, "* Testing URL (URI) parser.\n");
  struct {
    char *url;
    size_t len;
    fio_url_s expected;
  } tests[] = {
      {
          .url = (char *)"file://go/home/",
          .len = 15,
          .expected =
              {
                  .scheme = {.buf = (char *)"file", .len = 4},
                  .path = {.buf = (char *)"go/home/", .len = 8},
              },
      },
      {
          .url = (char *)"unix:///go/home/",
          .len = 16,
          .expected =
              {
                  .scheme = {.buf = (char *)"unix", .len = 4},
                  .path = {.buf = (char *)"/go/home/", .len = 9},
              },
      },
      {
          .url = (char *)"schema://user:password@host:port/path?query#target",
          .len = 50,
          .expected =
              {
                  .scheme = {.buf = (char *)"schema", .len = 6},
                  .user = {.buf = (char *)"user", .len = 4},
                  .password = {.buf = (char *)"password", .len = 8},
                  .host = {.buf = (char *)"host", .len = 4},
                  .port = {.buf = (char *)"port", .len = 4},
                  .path = {.buf = (char *)"/path", .len = 5},
                  .query = {.buf = (char *)"query", .len = 5},
                  .target = {.buf = (char *)"target", .len = 6},
              },
      },
      {
          .url = (char *)"schema://user@host:port/path?query#target",
          .len = 41,
          .expected =
              {
                  .scheme = {.buf = (char *)"schema", .len = 6},
                  .user = {.buf = (char *)"user", .len = 4},
                  .host = {.buf = (char *)"host", .len = 4},
                  .port = {.buf = (char *)"port", .len = 4},
                  .path = {.buf = (char *)"/path", .len = 5},
                  .query = {.buf = (char *)"query", .len = 5},
                  .target = {.buf = (char *)"target", .len = 6},
              },
      },
      {
          .url = (char *)"http://localhost.com:3000/home?is=1",
          .len = 35,
          .expected =
              {
                  .scheme = {.buf = (char *)"http", .len = 4},
                  .host = {.buf = (char *)"localhost.com", .len = 13},
                  .port = {.buf = (char *)"3000", .len = 4},
                  .path = {.buf = (char *)"/home", .len = 5},
                  .query = {.buf = (char *)"is=1", .len = 4},
              },
      },
      {
          .url = (char *)"/complete_path?query#target",
          .len = 27,
          .expected =
              {
                  .path = {.buf = (char *)"/complete_path", .len = 14},
                  .query = {.buf = (char *)"query", .len = 5},
                  .target = {.buf = (char *)"target", .len = 6},
              },
      },
      {
          .url = (char *)"/index.html?page=1#list",
          .len = 23,
          .expected =
              {
                  .path = {.buf = (char *)"/index.html", .len = 11},
                  .query = {.buf = (char *)"page=1", .len = 6},
                  .target = {.buf = (char *)"list", .len = 4},
              },
      },
      {
          .url = (char *)"example.com",
          .len = 11,
          .expected =
              {
                  .host = {.buf = (char *)"example.com", .len = 11},
              },
      },

      {
          .url = (char *)"example.com:8080",
          .len = 16,
          .expected =
              {
                  .host = {.buf = (char *)"example.com", .len = 11},
                  .port = {.buf = (char *)"8080", .len = 4},
              },
      },
      {
          .url = (char *)"example.com/index.html",
          .len = 22,
          .expected =
              {
                  .host = {.buf = (char *)"example.com", .len = 11},
                  .path = {.buf = (char *)"/index.html", .len = 11},
              },
      },
      {
          .url = (char *)"example.com:8080/index.html",
          .len = 27,
          .expected =
              {
                  .host = {.buf = (char *)"example.com", .len = 11},
                  .port = {.buf = (char *)"8080", .len = 4},
                  .path = {.buf = (char *)"/index.html", .len = 11},
              },
      },
      {
          .url = (char *)"example.com:8080/index.html?key=val#target",
          .len = 42,
          .expected =
              {
                  .host = {.buf = (char *)"example.com", .len = 11},
                  .port = {.buf = (char *)"8080", .len = 4},
                  .path = {.buf = (char *)"/index.html", .len = 11},
                  .query = {.buf = (char *)"key=val", .len = 7},
                  .target = {.buf = (char *)"target", .len = 6},
              },
      },
      {
          .url = (char *)"user:1234@example.com:8080/index.html",
          .len = 37,
          .expected =
              {
                  .user = {.buf = (char *)"user", .len = 4},
                  .password = {.buf = (char *)"1234", .len = 4},
                  .host = {.buf = (char *)"example.com", .len = 11},
                  .port = {.buf = (char *)"8080", .len = 4},
                  .path = {.buf = (char *)"/index.html", .len = 11},
              },
      },
      {
          .url = (char *)"user@example.com:8080/index.html",
          .len = 32,
          .expected =
              {
                  .user = {.buf = (char *)"user", .len = 4},
                  .host = {.buf = (char *)"example.com", .len = 11},
                  .port = {.buf = (char *)"8080", .len = 4},
                  .path = {.buf = (char *)"/index.html", .len = 11},
              },
      },
      {.url = NULL},
  };
  for (size_t i = 0; tests[i].url; ++i) {
    fio_url_s result = fio_url_parse(tests[i].url, tests[i].len);
    FIO_LOG_DEBUG2("Result for: %s"
                   "\n\t     scheme   (%zu bytes):  %.*s"
                   "\n\t     user     (%zu bytes):  %.*s"
                   "\n\t     password (%zu bytes):  %.*s"
                   "\n\t     host     (%zu bytes):  %.*s"
                   "\n\t     port     (%zu bytes):  %.*s"
                   "\n\t     path     (%zu bytes):  %.*s"
                   "\n\t     query    (%zu bytes):  %.*s"
                   "\n\t     target   (%zu bytes):  %.*s\n",
                   tests[i].url,
                   result.scheme.len,
                   (int)result.scheme.len,
                   result.scheme.buf,
                   result.user.len,
                   (int)result.user.len,
                   result.user.buf,
                   result.password.len,
                   (int)result.password.len,
                   result.password.buf,
                   result.host.len,
                   (int)result.host.len,
                   result.host.buf,
                   result.port.len,
                   (int)result.port.len,
                   result.port.buf,
                   result.path.len,
                   (int)result.path.len,
                   result.path.buf,
                   result.query.len,
                   (int)result.query.len,
                   result.query.buf,
                   result.target.len,
                   (int)result.target.len,
                   result.target.buf);
    FIO_ASSERT(
        result.scheme.len == tests[i].expected.scheme.len &&
            (!result.scheme.len || !memcmp(result.scheme.buf,
                                           tests[i].expected.scheme.buf,
                                           tests[i].expected.scheme.len)),
        "scheme result failed for:\n\ttest[%zu]: %s\n\texpected: "
        "%s\n\tgot: %.*s",
        i,
        tests[i].url,
        tests[i].expected.scheme.buf,
        (int)result.scheme.len,
        result.scheme.buf);
    FIO_ASSERT(
        result.user.len == tests[i].expected.user.len &&
            (!result.user.len || !memcmp(result.user.buf,
                                         tests[i].expected.user.buf,
                                         tests[i].expected.user.len)),
        "user result failed for:\n\ttest[%zu]: %s\n\texpected: %s\n\tgot: %.*s",
        i,
        tests[i].url,
        tests[i].expected.user.buf,
        (int)result.user.len,
        result.user.buf);
    FIO_ASSERT(
        result.password.len == tests[i].expected.password.len &&
            (!result.password.len || !memcmp(result.password.buf,
                                             tests[i].expected.password.buf,
                                             tests[i].expected.password.len)),
        "password result failed for:\n\ttest[%zu]: %s\n\texpected: %s\n\tgot: "
        "%.*s",
        i,
        tests[i].url,
        tests[i].expected.password.buf,
        (int)result.password.len,
        result.password.buf);
    FIO_ASSERT(
        result.host.len == tests[i].expected.host.len &&
            (!result.host.len || !memcmp(result.host.buf,
                                         tests[i].expected.host.buf,
                                         tests[i].expected.host.len)),
        "host result failed for:\n\ttest[%zu]: %s\n\texpected: %s\n\tgot: %.*s",
        i,
        tests[i].url,
        tests[i].expected.host.buf,
        (int)result.host.len,
        result.host.buf);
    FIO_ASSERT(
        result.port.len == tests[i].expected.port.len &&
            (!result.port.len || !memcmp(result.port.buf,
                                         tests[i].expected.port.buf,
                                         tests[i].expected.port.len)),
        "port result failed for:\n\ttest[%zu]: %s\n\texpected: %s\n\tgot: %.*s",
        i,
        tests[i].url,
        tests[i].expected.port.buf,
        (int)result.port.len,
        result.port.buf);
    FIO_ASSERT(
        result.path.len == tests[i].expected.path.len &&
            (!result.path.len || !memcmp(result.path.buf,
                                         tests[i].expected.path.buf,
                                         tests[i].expected.path.len)),
        "path result failed for:\n\ttest[%zu]: %s\n\texpected: %s\n\tgot: %.*s",
        i,
        tests[i].url,
        tests[i].expected.path.buf,
        (int)result.path.len,
        result.path.buf);
    FIO_ASSERT(result.query.len == tests[i].expected.query.len &&
                   (!result.query.len || !memcmp(result.query.buf,
                                                 tests[i].expected.query.buf,
                                                 tests[i].expected.query.len)),
               "query result failed for:\n\ttest[%zu]: %s\n\texpected: "
               "%s\n\tgot: %.*s",
               i,
               tests[i].url,
               tests[i].expected.query.buf,
               (int)result.query.len,
               result.query.buf);
    FIO_ASSERT(
        result.target.len == tests[i].expected.target.len &&
            (!result.target.len || !memcmp(result.target.buf,
                                           tests[i].expected.target.buf,
                                           tests[i].expected.target.len)),
        "target result failed for:\n\ttest[%zu]: %s\n\texpected: "
        "%s\n\tgot: %.*s",
        i,
        tests[i].url,
        tests[i].expected.target.buf,
        (int)result.target.len,
        result.target.buf);
  }
}
#endif /* FIO_TEST_CSTL */

/* *****************************************************************************
FIO_URL - Cleanup
***************************************************************************** */
#endif /* FIO_EXTERN_COMPLETE */
#endif /* FIO_URL || FIO_URI */
#undef FIO_URL
#undef FIO_URI
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#include "006 atol.h"               /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************










                                JSON Parsing









***************************************************************************** */
#if defined(FIO_JSON) && !defined(H___FIO_JSON_H)
#define H___FIO_JSON_H

#ifndef JSON_MAX_DEPTH
/** Maximum allowed JSON nesting level. Values above 64K might fail. */
#define JSON_MAX_DEPTH 512
#endif

/** The JSON parser type. Memory must be initialized to 0 before first uses. */
typedef struct {
  /** level of nesting. */
  uint32_t depth;
  /** expectation bit flag: 0=key, 1=colon, 2=value, 4=comma/closure . */
  uint8_t expect;
  /** nesting bit flags - dictionary bit = 0, array bit = 1. */
  uint8_t nesting[(JSON_MAX_DEPTH + 7) >> 3];
} fio_json_parser_s;

#define FIO_JSON_INIT                                                          \
  { .depth = 0 }

/**
 * The facil.io JSON parser is a non-strict parser, with support for trailing
 * commas in collections, new-lines in strings, extended escape characters and
 * octal, hex and binary numbers.
 *
 * The parser allows for streaming data and decouples the parsing process from
 * the resulting data-structure by calling static callbacks for JSON related
 * events.
 *
 * Returns the number of bytes consumed before parsing stopped (due to either
 * error or end of data). Stops as close as possible to the end of the buffer or
 * once an object parsing was completed.
 */
SFUNC size_t fio_json_parse(fio_json_parser_s *parser,
                            const char *buffer,
                            const size_t len);

/* *****************************************************************************
JSON Parsing - Implementation - Helpers and Callbacks


Note: static Callacks must be implemented in the C file that uses the parser

Note: a Helper API is provided for the parsing implementation.
***************************************************************************** */
#ifdef FIO_EXTERN_COMPLETE

/** common FIO_JSON callback function properties */
#define FIO_JSON_CB static inline __attribute__((unused))

/* *****************************************************************************
JSON Parsing - Helpers API
***************************************************************************** */

/** Tests the state of the JSON parser. Returns 1 for true and 0 for false. */
FIO_JSON_CB uint8_t fio_json_parser_is_in_array(fio_json_parser_s *parser);

/** Tests the state of the JSON parser. Returns 1 for true and 0 for false. */
FIO_JSON_CB uint8_t fio_json_parser_is_in_object(fio_json_parser_s *parser);

/** Tests the state of the JSON parser. Returns 1 for true and 0 for false. */
FIO_JSON_CB uint8_t fio_json_parser_is_key(fio_json_parser_s *parser);

/** Tests the state of the JSON parser. Returns 1 for true and 0 for false. */
FIO_JSON_CB uint8_t fio_json_parser_is_value(fio_json_parser_s *parser);

/* *****************************************************************************
JSON Parsing - Implementation - Callbacks
***************************************************************************** */

/** a NULL object was detected */
FIO_JSON_CB void fio_json_on_null(fio_json_parser_s *p);
/** a TRUE object was detected */
static inline void fio_json_on_true(fio_json_parser_s *p);
/** a FALSE object was detected */
FIO_JSON_CB void fio_json_on_false(fio_json_parser_s *p);
/** a Number was detected (long long). */
FIO_JSON_CB void fio_json_on_number(fio_json_parser_s *p, long long i);
/** a Float was detected (double). */
FIO_JSON_CB void fio_json_on_float(fio_json_parser_s *p, double f);
/** a String was detected (int / float). update `pos` to point at ending */
FIO_JSON_CB void
fio_json_on_string(fio_json_parser_s *p, const void *start, size_t len);
/** a dictionary object was detected, should return 0 unless error occurred. */
FIO_JSON_CB int fio_json_on_start_object(fio_json_parser_s *p);
/** a dictionary object closure detected */
FIO_JSON_CB void fio_json_on_end_object(fio_json_parser_s *p);
/** an array object was detected, should return 0 unless error occurred. */
FIO_JSON_CB int fio_json_on_start_array(fio_json_parser_s *p);
/** an array closure was detected */
FIO_JSON_CB void fio_json_on_end_array(fio_json_parser_s *p);
/** the JSON parsing is complete */
FIO_JSON_CB void fio_json_on_json(fio_json_parser_s *p);
/** the JSON parsing encountered an error */
FIO_JSON_CB void fio_json_on_error(fio_json_parser_s *p);

/* *****************************************************************************
JSON Parsing - Implementation - Helpers and Parsing


Note: static Callacks must be implemented in the C file that uses the parser
***************************************************************************** */

/** Tests the state of the JSON parser. Returns 1 for true and 0 for false. */
FIO_JSON_CB uint8_t fio_json_parser_is_in_array(fio_json_parser_s *p) {
  return p->depth && fio_bitmap_get(p->nesting, p->depth);
}

/** Tests the state of the JSON parser. Returns 1 for true and 0 for false. */
FIO_JSON_CB uint8_t fio_json_parser_is_in_object(fio_json_parser_s *p) {
  return p->depth && !fio_bitmap_get(p->nesting, p->depth);
}

/** Tests the state of the JSON parser. Returns 1 for true and 0 for false. */
FIO_JSON_CB uint8_t fio_json_parser_is_key(fio_json_parser_s *p) {
  return fio_json_parser_is_in_object(p) && !p->expect;
}

/** Tests the state of the JSON parser. Returns 1 for true and 0 for false. */
FIO_JSON_CB uint8_t fio_json_parser_is_value(fio_json_parser_s *p) {
  return !fio_json_parser_is_key(p);
}

FIO_IFUNC const char *fio___json_skip_comments(const char *buffer,
                                               const char *stop) {
  if (*buffer == '#' ||
      ((stop - buffer) > 2 && buffer[0] == '/' && buffer[1] == '/')) {
    /* EOL style comment, C style or Bash/Ruby style*/
    buffer = (const char *)memchr(buffer + 1, '\n', stop - (buffer + 1));
    return buffer;
  }
  if (((stop - buffer) > 3 && buffer[0] == '/' && buffer[1] == '*')) {
    while ((buffer = (const char *)memchr(buffer, '/', stop - buffer)) &&
           buffer && ++buffer && buffer[-2] != '*')
      ;
    return buffer;
  }
  return NULL;
}

FIO_IFUNC const char *fio___json_consume_string(fio_json_parser_s *p,
                                                const char *buffer,
                                                const char *stop) {
  const char *start = ++buffer;
  for (;;) {
    buffer = (const char *)memchr(buffer, '\"', stop - buffer);
    if (!buffer)
      return NULL;
    size_t escaped = 1;
    while (buffer[0 - escaped] == '\\')
      ++escaped;
    if (escaped & 1)
      break;
    ++buffer;
  }
  fio_json_on_string(p, start, buffer - start);
  return buffer + 1;
}

FIO_IFUNC const char *fio___json_consume_number(fio_json_parser_s *p,
                                                const char *buffer,
                                                const char *stop) {

  const char *const was = buffer;
  errno = 0; /* testo for E2BIG on number parsing */
  long long i = fio_atol((char **)&buffer);
  if (buffer < stop &&
      ((*buffer) == '.' || (*buffer | 32) == 'e' || (*buffer | 32) == 'x' ||
       (*buffer | 32) == 'p' || (*buffer | 32) == 'i' || errno)) {
    buffer = was;
    double f = fio_atof((char **)&buffer);
    fio_json_on_float(p, f);
  } else {
    fio_json_on_number(p, i);
  }
  return buffer;
}

FIO_IFUNC const char *fio___json_identify(fio_json_parser_s *p,
                                          const char *buffer,
                                          const char *stop) {
  /* Use `break` to change separator requirement status.
   * Use `continue` to keep separator requirement the same.
   */
  switch (*buffer) {
  case 0x09: /* fallthrough */
  case 0x0A: /* fallthrough */
  case 0x0D: /* fallthrough */
  case 0x20:
    /* consume whitespace */
    ++buffer;
    if (!((uintptr_t)buffer & 7)) {
      while (buffer + 8 < stop) {
        const uint64_t w1 = 0x0101010101010101 * 0x09;
        const uint64_t w2 = 0x0101010101010101 * 0x0A;
        const uint64_t w3 = 0x0101010101010101 * 0x0D;
        const uint64_t w4 = 0x0101010101010101 * 0x20;
        const uint64_t t1 = ~(w1 ^ (*(uint64_t *)(buffer)));
        const uint64_t t2 = ~(w2 ^ (*(uint64_t *)(buffer)));
        const uint64_t t3 = ~(w3 ^ (*(uint64_t *)(buffer)));
        const uint64_t t4 = ~(w4 ^ (*(uint64_t *)(buffer)));
        const uint64_t b1 =
            (((t1 & 0x7f7f7f7f7f7f7f7fULL) + 0x0101010101010101ULL) &
             (t1 & 0x8080808080808080ULL));
        const uint64_t b2 =
            (((t2 & 0x7f7f7f7f7f7f7f7fULL) + 0x0101010101010101ULL) &
             (t2 & 0x8080808080808080ULL));
        const uint64_t b3 =
            (((t3 & 0x7f7f7f7f7f7f7f7fULL) + 0x0101010101010101ULL) &
             (t3 & 0x8080808080808080ULL));
        const uint64_t b4 =
            (((t4 & 0x7f7f7f7f7f7f7f7fULL) + 0x0101010101010101ULL) &
             (t4 & 0x8080808080808080ULL));
        if ((b1 | b2 | b3 | b4) != 0x8080808080808080ULL)
          break;
        buffer += 8;
      }
    }
    return buffer;
  case ',': /* comma separator */
    if (!p->depth || !(p->expect & 4))
      goto unexpected_separator;
    ++buffer;
    p->expect = (fio_bitmap_get(p->nesting, p->depth) << 1);
    return buffer;
  case ':': /* colon separator */
    if (!p->depth || !(p->expect & 1))
      goto unexpected_separator;
    ++buffer;
    p->expect = 2;
    return buffer;
    /*
     *
     * JSON Strings
     *
     */
  case '"':
    if (p->depth && (p->expect & ((uint8_t)5)))
      goto missing_separator;
    buffer = fio___json_consume_string(p, buffer, stop);
    if (!buffer)
      goto unterminated_string;
    break;
    /*
     *
     * JSON Objects
     *
     */
  case '{':
    if (p->depth && !(p->expect & 2))
      goto missing_separator;
    p->expect = 0;
    if (p->depth == JSON_MAX_DEPTH)
      goto too_deep;
    ++p->depth;
    fio_bitmap_unset(p->nesting, p->depth);
    fio_json_on_start_object(p);
    return buffer + 1;
  case '}':
    if (fio_bitmap_get(p->nesting, p->depth) || !p->depth || (p->expect & 3))
      goto object_closure_unexpected;
    fio_bitmap_unset(p->nesting, p->depth);
    p->expect = 4; /* expect comma */
    --p->depth;
    fio_json_on_end_object(p);
    return buffer + 1;
    /*
     *
     * JSON Arrays
     *
     */
  case '[':
    if (p->depth && !(p->expect & 2))
      goto missing_separator;
    fio_json_on_start_array(p);
    p->expect = 2;
    if (p->depth == JSON_MAX_DEPTH)
      goto too_deep;
    ++p->depth;
    fio_bitmap_set(p->nesting, p->depth);
    return buffer + 1;
  case ']':
    if (!fio_bitmap_get(p->nesting, p->depth) || !p->depth)
      goto array_closure_unexpected;
    fio_bitmap_unset(p->nesting, p->depth);
    p->expect = 4; /* expect comma */
    --p->depth;
    fio_json_on_end_array(p);
    return buffer + 1;
    /*
     *
     * JSON Primitives (true / false / null (NaN))
     *
     */
  case 'N': /* NaN or null? - fallthrough */
  case 'n':
    if (p->depth && !(p->expect & 2))
      goto missing_separator;
    if (buffer + 4 > stop || buffer[1] != 'u' || buffer[2] != 'l' ||
        buffer[3] != 'l') {
      if (buffer + 3 > stop || (buffer[1] | 32) != 'a' ||
          (buffer[2] | 32) != 'n')
        return NULL;
      char *nan_str = (char *)"NaN";
      fio_json_on_float(p, fio_atof(&nan_str));
      buffer += 3;
      break;
    }
    fio_json_on_null(p);
    buffer += 4;
    break;
  case 't': /* true */
    if (p->depth && !(p->expect & 2))
      goto missing_separator;
    if (buffer + 4 > stop || buffer[1] != 'r' || buffer[2] != 'u' ||
        buffer[3] != 'e')
      return NULL;
    fio_json_on_true(p);
    buffer += 4;
    break;
  case 'f': /* false */
    if (p->depth && !(p->expect & 2))
      goto missing_separator;
    if (buffer + 5 > stop || buffer[1] != 'a' || buffer[2] != 'l' ||
        buffer[3] != 's' || buffer[4] != 'e')
      return NULL;
    fio_json_on_false(p);
    buffer += 5;
    break;
    /*
     *
     * JSON Numbers (Integers / Floats)
     *
     */
  case '+': /* fallthrough */
  case '-': /* fallthrough */
  case '0': /* fallthrough */
  case '1': /* fallthrough */
  case '2': /* fallthrough */
  case '3': /* fallthrough */
  case '4': /* fallthrough */
  case '5': /* fallthrough */
  case '6': /* fallthrough */
  case '7': /* fallthrough */
  case '8': /* fallthrough */
  case '9': /* fallthrough */
  case 'x': /* fallthrough */
  case '.': /* fallthrough */
  case 'e': /* fallthrough */
  case 'E': /* fallthrough */
  case 'i': /* fallthrough */
  case 'I':
    if (p->depth && !(p->expect & 2))
      goto missing_separator;
    buffer = fio___json_consume_number(p, buffer, stop);
    if (!buffer)
      goto bad_number_format;
    break;
    /*
     *
     * Comments
     *
     */
  case '#': /* fallthrough */
  case '/': /* fallthrough */
    return fio___json_skip_comments(buffer, stop);
    /*
     *
     * Unrecognized Data Handling
     *
     */
  default:
    FIO_LOG_DEBUG("unrecognized JSON identifier at:\n%.*s",
                  ((stop - buffer > 48) ? (int)48 : ((int)(stop - buffer))),
                  buffer);
    return NULL;
  }
  /* p->expect should be either 0 (key) or 2 (value) */
  p->expect = (p->expect << 1) + ((p->expect ^ 2) >> 1);
  return buffer;

missing_separator:
  FIO_LOG_DEBUG("missing JSON separator '%c' at (%d):\n%.*s",
                (p->expect == 2 ? ':' : ','),
                p->expect,
                ((stop - buffer > 48) ? 48 : ((int)(stop - buffer))),
                buffer);
  fio_json_on_error(p);
  return NULL;
unexpected_separator:
  FIO_LOG_DEBUG("unexpected JSON separator at:\n%.*s",
                ((stop - buffer > 48) ? 48 : ((int)(stop - buffer))),
                buffer);
  fio_json_on_error(p);
  return NULL;
unterminated_string:
  FIO_LOG_DEBUG("unterminated JSON string at:\n%.*s",
                ((stop - buffer > 48) ? 48 : ((int)(stop - buffer))),
                buffer);
  fio_json_on_error(p);
  return NULL;
bad_number_format:
  FIO_LOG_DEBUG("bad JSON numeral format at:\n%.*s",
                ((stop - buffer > 48) ? 48 : ((int)(stop - buffer))),
                buffer);
  fio_json_on_error(p);
  return NULL;
array_closure_unexpected:
  FIO_LOG_DEBUG("JSON array closure unexpected at:\n%.*s",
                ((stop - buffer > 48) ? 48 : ((int)(stop - buffer))),
                buffer);
  fio_json_on_error(p);
  return NULL;
object_closure_unexpected:
  FIO_LOG_DEBUG("JSON object closure unexpected at (%d):\n%.*s",
                p->expect,
                ((stop - buffer > 48) ? 48 : ((int)(stop - buffer))),
                buffer);
  fio_json_on_error(p);
  return NULL;
too_deep:
  FIO_LOG_DEBUG("JSON object nesting too deep at:\n%.*s",
                p->expect,
                ((stop - buffer > 48) ? 48 : ((int)(stop - buffer))),
                buffer);
  fio_json_on_error(p);
  return NULL;
}

/**
 * Returns the number of bytes consumed. Stops as close as possible to the end
 * of the buffer or once an object parsing was completed.
 */
SFUNC size_t fio_json_parse(fio_json_parser_s *p,
                            const char *buffer,
                            const size_t len) {
  const char *start = buffer;
  const char *stop = buffer + len;
  const char *last;
  /* skip BOM, if exists */
  if (len >= 3 && buffer[0] == (char)0xEF && buffer[1] == (char)0xBB &&
      buffer[2] == (char)0xBF) {
    buffer += 3;
    if (len == 3)
      goto finish;
  }
  /* loop until the first JSON data was read */
  do {
    last = buffer;
    buffer = fio___json_identify(p, buffer, stop);
    if (!buffer)
      goto failed;
  } while (!p->expect && buffer < stop);
  /* loop until the JSON object (nesting) is closed */
  while (p->depth && buffer < stop) {
    last = buffer;
    buffer = fio___json_identify(p, buffer, stop);
    if (!buffer)
      goto failed;
  }
  if (!p->depth) {
    p->expect = 0;
    fio_json_on_json(p);
  }
finish:
  return buffer - start;
failed:
  FIO_LOG_DEBUG("JSON parsing failed after:\n%.*s",
                ((stop - last > 48) ? 48 : ((int)(stop - last))),
                last);
  return last - start;
}

#endif /* FIO_EXTERN_COMPLETE */
#undef FIO_JSON
#endif /* FIO_JSON */
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#define FIO_ATOMIC                  /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#include "003 atomics.h"            /* Development inclusion - ignore line */
#define FIO_MALLOC                  /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************










                          Custom Memory Allocation
                  Memory allocator for short lived objects






The allocator has multiple allocation "arenas" that elevate thread contention.

Arenas are "sticky", which means that each thread will first try to access it's
previoiusly accessed arena. Only if the previously accessed arena is busy will
the thread attempt to access a different arena.

Each Arena has both  a "block" and a "big-block" of memory which are used for
memory allocation.

Once the user frees all of a block's memory, the blocks will be placed in an
"available" list until the memory can be returned to the system (or cached).

Allocations
==============

Memory "chunks" are allocated from the system (by default through `mmap`).

Chunks are divided into "blocks" that are used for actual memory allocation.

Big blocks are used for larger allocations and they use the whole chunk.

Both chunks and blocks are always allocated on a specific memory boundary where
a specific amount of least significant bits are always zero.

Allocations are performed by "slicing" the block and passing the pointer to the
user. No metadata is attached to the allocated "slices" since both the block and
the chunk (where metadata is stored) can be easily inferred using a bit mask.

Once a block was fully utiliized, the allocator will no longer keep a record of
that block until the user frees all it's memory.

It should be noted that the first block header on every chunk and the chunk
header occupy the same 16 bytes (interleaved).

Deallocations
==============

Deallocations are performed by atomically marking a deallocation in the block.

Once all the block slices were deallocated, and assuming the block isn't the
active allocation block, that block is marked as free and placed in an
"available" list.

If all the blocks in a chunk were marked as available, all it's blocks are
removed from the list and the chunk will be returned to the system (or cached).

Cons
==============

1. By avoiding a detailed "free list", there is no way to re-use parts of a
"dirty" block. This increases the price of fragmentation may be the whole block
that contains withheld memory.

2. Large overall memory allocation-deallocation cycles (bigger than (cache +
arena) * sys_alloc) will result in a lot of system calls, as the allocator will
return memory to the system before requesting it back and using it.

This approach shouldn't be used for objects with mixed lifetime, as it will
result in fragmentation where the cost of a single allocation (i.e., a
persistent 16 byte allocation) could be a whole block that can't be utilized.

Pros
==============

1. Consecative allocations in the same thread are likely to be from the same
block, resulting in improved memory locality.

2. The use of multiple arenas for allocation minimizes thread contention during
allocation.

3. The use of atomic operations for deallocations minimizes thread contention
until a whole block is placed in the available list or a chunk is ready to be
freed.

4. The allocator's overrhead for even the smallest of allocations is fairly low,
in addition to a set memory consumption (~1 page for core data + 16 bytes for
`fio_malloc(0)` + 8 bytes per thread), only 16 bytes per block are
used by the allocator (%0.006), the rest is available for the application.

***************************************************************************** */
#if defined(FIO_MALLOC) && !defined(H___FIO_MALLOC_H)
#define H___FIO_MALLOC_H

/* *****************************************************************************
Memory Allocation - API
***************************************************************************** */

/* inform the compiler that the returned value is aligned on 16 byte marker */
#if __clang__ || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 8)
#define FIO_ALIGN __attribute__((assume_aligned(16)))
#define FIO_ALIGN_NEW __attribute__((malloc, assume_aligned(16)))
#else
#define FIO_ALIGN
#define FIO_ALIGN_NEW
#endif /* (__clang__ || __GNUC__)... */

/**
 * Allocates memory using a per-CPU core block memory pool.
 * Memory is zeroed out.
 *
 * Allocations above FIO_MEMORY_BLOCK_ALLOC_LIMIT (16Kb when using 32Kb blocks)
 * will be redirected to `mmap`, as if `fio_mmap` was called.
 *
 * `fio_malloc` promises a best attempt at providing locality between
 * consecutive calls, but locality can't be guaranteed.
 */
SFUNC void *FIO_ALIGN_NEW fio_malloc(size_t size);

/**
 * same as calling `fio_malloc(size_per_unit * unit_count)`;
 *
 * Allocations above FIO_MEMORY_BLOCK_ALLOC_LIMIT (16Kb when using 32Kb blocks)
 * will be redirected to `mmap`, as if `fio_mmap` was called.
 */
SFUNC void *FIO_ALIGN_NEW fio_calloc(size_t size_per_unit, size_t unit_count);

/** Frees memory that was allocated using this library. */
SFUNC void fio_free(void *ptr);

/**
 * Re-allocates memory. An attempt to avoid copying the data is made only for
 * big memory allocations (larger than FIO_MEMORY_BLOCK_ALLOC_LIMIT).
 */
SFUNC void *FIO_ALIGN fio_realloc(void *ptr, size_t new_size);

/**
 * Re-allocates memory. An attempt to avoid copying the data is made only for
 * big memory allocations (larger than FIO_MEMORY_BLOCK_ALLOC_LIMIT).
 *
 * This variation is slightly faster as it might copy less data.
 */
SFUNC void *FIO_ALIGN fio_realloc2(void *ptr, size_t new_size, size_t copy_len);

/**
 * Allocates memory directly using `mmap`, this is preferred for objects that
 * both require almost a page of memory (or more) and expect a long lifetime.
 *
 * However, since this allocation will invoke the system call (`mmap`), it will
 * be inherently slower.
 *
 * `fio_free` can be used for deallocating the memory.
 */
SFUNC void *FIO_ALIGN_NEW fio_mmap(size_t size);

/**
 * When forking is called manually, call this function to reset the facil.io
 * memory allocator's locks.
 */
SFUNC void fio_malloc_after_fork(void);

/* *****************************************************************************
Memory Allocation - configuration macros

NOTE: most configuration values should be a power of 2 or a logarithmic value.
***************************************************************************** */

#ifndef FIO_MEMORY_ARENA_COUNT_MAX
/**
 * The maximum number of memory arenas to initialize.
 *
 * When 0 - maximum detected cores X 2.
 */
#define FIO_MEMORY_ARENA_COUNT_MAX 64
#endif

#ifndef FIO_MEMORY_ARENA_COUNT_DEFAULT
/**
 * The default number of memory arenas to initialize when CPU core detection
 * fails.
 *
 * Normally, fio_malloc tries to initialize as many memory allocation arenas as
 * the number of CPU cores. This value will only be used if core detection isn't
 * available or fails.
 */
#define FIO_MEMORY_ARENA_COUNT_DEFAULT 5
#endif

#ifndef FIO_MEMORY_SYS_ALLOCATION_SIZE_LOG
/**
 * The logarithmic size of a single allocatiion "chunk" (16 blocks).
 *
 * Limited to >=17 and <=24.
 *
 * By default 22, whch is a ~4Mb allocation per system call.
 *
 * A block is always 1/16 of this value. A default block size is ~256Kb.
 */
#define FIO_MEMORY_SYS_ALLOCATION_SIZE_LOG 23
#endif

#ifndef FIO_MEMORY_BLOCK_SIZE_LOG
/** The each system allocation is divided into blocks, by default: 256Kb */
#define FIO_MEMORY_BLOCK_SIZE_LOG 18
#endif

#ifndef FIO_MEMORY_BLOCK_ALLOC_LIMIT
/**
 * The maximum allocation size, after which a "big block" allocation will be
 * used.
 *
 * Defaults to 6.25% of the block (16KB), after which big-blocks are used.
 */
#define FIO_MEMORY_BLOCK_ALLOC_LIMIT (1UL << (FIO_MEMORY_BLOCK_SIZE_LOG - 4))
#endif

#ifndef FIO_MEMORY_BIG_BLOCK_ALLOC_LIMIT
/**
 * The maximum allocation size, after which `mmap` will be called instead of the
 * facil.io allocator.
 *
 * Defaults to 6.25% of the big-block/chunk (0.5Mb), after which `mmap` is used
 * instead.
 */
// #define FIO_MEMORY_BIG_BLOCK_ALLOC_LIMIT FIO_MEMORY_BLOCK_ALLOC_LIMIT
#define FIO_MEMORY_BIG_BLOCK_ALLOC_LIMIT                                       \
  (1UL << (FIO_MEMORY_SYS_ALLOCATION_SIZE_LOG - 4))
#endif

#ifndef FIO_MEMORY_CACHE_SLOTS
/**
 * The number of memory "chunks" / "blocks" to cache even if they are not in
 * use.
 *
 * This is the number of chunks that won't be returned to the system even though
 * they aren't assigned to any arena.
 *
 * This is ALSO the number of blocks that will be cached for lockless access
 * after being partially freed - their chunks not marked as free until they are
 * pushed out of the lockless queue.
 */
#define FIO_MEMORY_CACHE_SLOTS 8
#endif

#ifndef FIO_MEMORY_USE_PTHREAD_MUTEX
/** Uses a pthread mutex instead of a spinlock. */
#define FIO_MEMORY_USE_PTHREAD_MUTEX 1
#endif

#ifndef FIO_MEMORY_USE_FIO_MEMSET
/** Prefer a custom implementation of memset over the library implementation */
#define FIO_MEMORY_USE_FIO_MEMSET 0
#endif

/* *****************************************************************************
Memory Allocation - configuration value - results and constants
***************************************************************************** */

/* Helper macro, don't change this */
#undef FIO_MEMORY_BLOCKS_PER_ALLOCATION
#define FIO_MEMORY_BLOCKS_PER_ALLOCATION                                       \
  (1UL << (FIO_MEMORY_SYS_ALLOCATION_SIZE_LOG - FIO_MEMORY_BLOCK_SIZE_LOG))

/* Helper macro, don't change this */
#undef FIO_MEMORY_SYS_ALLOCATION_SIZE
#define FIO_MEMORY_SYS_ALLOCATION_SIZE                                         \
  (1UL << FIO_MEMORY_SYS_ALLOCATION_SIZE_LOG)

#undef FIO_MEMORY_BLOCK_SIZE
/** The resulting memory block size = chunk size / 16, defaults to 256Kb */
#define FIO_MEMORY_BLOCK_SIZE (1UL << FIO_MEMORY_BLOCK_SIZE_LOG)

/* *****************************************************************************
Memory Allocation - redefine default allocation macros
***************************************************************************** */
#undef FIO_MEM_CALLOC
/** Allocates size X units of bytes, where all bytes equal zero. */
#define FIO_MEM_CALLOC(size, units) fio_calloc((size), (units))

#undef FIO_MEM_REALLOC
/** Reallocates memory, copying (at least) `copy_len` if necessary. */
#define FIO_MEM_REALLOC(ptr, old_size, new_size, copy_len)                     \
  fio_realloc2((ptr), (new_size), (copy_len))

#undef FIO_MEM_FREE
/** Frees allocated memory. */
#define FIO_MEM_FREE(ptr, size) fio_free((ptr))

#undef FIO_MEM_INTERNAL_MALLOC
#define FIO_MEM_INTERNAL_MALLOC 1

/* *****************************************************************************





Memory Allocation - Implementation





***************************************************************************** */

/* *****************************************************************************
Memory Allocation - forced bypass
***************************************************************************** */
#if defined(FIO_EXTERN_COMPLETE) && defined(FIO_MALLOC_FORCE_SYSTEM)

SFUNC void *FIO_ALIGN_NEW fio_malloc(size_t size) { return calloc(size, 1); }

SFUNC void *FIO_ALIGN_NEW fio_calloc(size_t size_per_unit, size_t unit_count) {
  return calloc(size_per_unit, unit_count);
}

SFUNC void fio_free(void *ptr) { free(ptr); }

SFUNC void *FIO_ALIGN fio_realloc(void *ptr, size_t new_size) {
  return realloc(ptr, new_size);
}

SFUNC void *FIO_ALIGN fio_realloc2(void *ptr,
                                   size_t new_size,
                                   size_t copy_len) {
  return realloc(ptr, new_size);
  (void)copy_len;
}

SFUNC void *FIO_ALIGN_NEW fio_mmap(size_t size) { return calloc(size, 1); }

SFUNC void fio_malloc_after_fork(void) {}

/* *****************************************************************************
Memory Allocation - custom implementation
***************************************************************************** */
#elif defined(FIO_EXTERN_COMPLETE)

#if FIO_HAVE_UNIX_TOOLS
#include <unistd.h>
#endif /* H___FIO_UNIX_TOOLS4STR_INCLUDED_H */

/* *****************************************************************************
Aligned memory copying
***************************************************************************** */
#define FIO_MEMCOPY_FIO_IFUNC_ALIGNED(type, size)                              \
  FIO_IFUNC void fio___memcpy_##size##b(                                       \
      void *restrict dest_, const void *restrict src_, size_t units) {         \
    type *dest = (type *)dest_;                                                \
    type *src = (type *)src_;                                                  \
    while (units >= 16) {                                                      \
      dest[0] = src[0];                                                        \
      dest[1] = src[1];                                                        \
      dest[2] = src[2];                                                        \
      dest[3] = src[3];                                                        \
      dest[4] = src[4];                                                        \
      dest[5] = src[5];                                                        \
      dest[6] = src[6];                                                        \
      dest[7] = src[7];                                                        \
      dest[8] = src[8];                                                        \
      dest[9] = src[9];                                                        \
      dest[10] = src[10];                                                      \
      dest[11] = src[11];                                                      \
      dest[12] = src[12];                                                      \
      dest[13] = src[13];                                                      \
      dest[14] = src[14];                                                      \
      dest[15] = src[15];                                                      \
      dest += 16;                                                              \
      src += 16;                                                               \
      units -= 16;                                                             \
    }                                                                          \
    switch (units) {                                                           \
    case 15:                                                                   \
      *(dest++) = *(src++); /* fallthrough */                                  \
    case 14:                                                                   \
      *(dest++) = *(src++); /* fallthrough */                                  \
    case 13:                                                                   \
      *(dest++) = *(src++); /* fallthrough */                                  \
    case 12:                                                                   \
      *(dest++) = *(src++); /* fallthrough */                                  \
    case 11:                                                                   \
      *(dest++) = *(src++); /* fallthrough */                                  \
    case 10:                                                                   \
      *(dest++) = *(src++); /* fallthrough */                                  \
    case 9:                                                                    \
      *(dest++) = *(src++); /* fallthrough */                                  \
    case 8:                                                                    \
      *(dest++) = *(src++); /* fallthrough */                                  \
    case 7:                                                                    \
      *(dest++) = *(src++); /* fallthrough */                                  \
    case 6:                                                                    \
      *(dest++) = *(src++); /* fallthrough */                                  \
    case 5:                                                                    \
      *(dest++) = *(src++); /* fallthrough */                                  \
    case 4:                                                                    \
      *(dest++) = *(src++); /* fallthrough */                                  \
    case 3:                                                                    \
      *(dest++) = *(src++); /* fallthrough */                                  \
    case 2:                                                                    \
      *(dest++) = *(src++); /* fallthrough */                                  \
    case 1:                                                                    \
      *(dest++) = *(src++);                                                    \
    }                                                                          \
  }
FIO_MEMCOPY_FIO_IFUNC_ALIGNED(uint16_t, 2)
FIO_MEMCOPY_FIO_IFUNC_ALIGNED(uint32_t, 4)
FIO_MEMCOPY_FIO_IFUNC_ALIGNED(uint64_t, 8)
#undef FIO_MEMCOPY_FIO_IFUNC_ALIGNED

/** Copies 16 byte `units` of size_t aligned memory blocks */
FIO_IFUNC void
fio___memcpy_16byte(void *dest_, const void *src_, size_t units) {
#if SIZE_MAX == 0xFFFFFFFFFFFFFFFF /* 64 bit size_t */
  fio___memcpy_8b(dest_, src_, units << 1);
#elif SIZE_MAX == 0xFFFFFFFF       /* 32 bit size_t */
  fio___memcpy_4b(dest_, src_, units << 2);
#else                              /* unknown... assume 16 bit? */
  fio___memcpy_2b(dest_, src_, units << 3);
#endif                             /* SIZE_MAX */
}

/** a 16 byte aligned memset implementation. */
FIO_SFUNC void
fio___memset_aligned(void *restrict dest_, uint64_t data, size_t bytes) {
  uint64_t *dest = (uint64_t *)dest_;
  bytes >>= 3;
  while (bytes >= 16) {
    dest[0] = data;
    dest[1] = data;
    dest[2] = data;
    dest[3] = data;
    dest[4] = data;
    dest[5] = data;
    dest[6] = data;
    dest[7] = data;
    dest[8] = data;
    dest[9] = data;
    dest[10] = data;
    dest[11] = data;
    dest[12] = data;
    dest[13] = data;
    dest[14] = data;
    dest[15] = data;
    dest += 16;
    bytes -= 16;
  }
  switch (bytes) {
  case 15:
    *(dest++) = data; /* fallthrough */
  case 14:
    *(dest++) = data; /* fallthrough */
  case 13:
    *(dest++) = data; /* fallthrough */
  case 12:
    *(dest++) = data; /* fallthrough */
  case 11:
    *(dest++) = data; /* fallthrough */
  case 10:
    *(dest++) = data; /* fallthrough */
  case 9:
    *(dest++) = data; /* fallthrough */
  case 8:
    *(dest++) = data; /* fallthrough */
  case 7:
    *(dest++) = data; /* fallthrough */
  case 6:
    *(dest++) = data; /* fallthrough */
  case 5:
    *(dest++) = data; /* fallthrough */
  case 4:
    *(dest++) = data; /* fallthrough */
  case 3:
    *(dest++) = data; /* fallthrough */
  case 2:
    *(dest++) = data; /* fallthrough */
  case 1:
    *(dest++) = data;
  }
}

#if FIO_MEMORY_USE_FIO_MEMSET
#define FIO___MEMSET fio___memset_aligned
#else
#define FIO___MEMSET memset
#endif
/* *****************************************************************************
Override the system's malloc functions if required
***************************************************************************** */
#ifdef FIO_MALLOC_OVERRIDE_SYSTEM
void *malloc(size_t size) { return fio_malloc(size); }
void *calloc(size_t size, size_t count) { return fio_calloc(size, count); }
void free(void *ptr) { fio_free(ptr); }
void *realloc(void *ptr, size_t new_size) { return fio_realloc(ptr, new_size); }
#endif /* FIO_MALLOC_OVERRIDE_SYSTEM */

/* *****************************************************************************
Big memory allocation macros and helpers (page allocation / mmap)
***************************************************************************** */
#ifndef FIO_MEM_PAGE_ALLOC

#ifndef FIO_MEM_PAGE_SIZE_LOG
#define FIO_MEM_PAGE_SIZE_LOG 12 /* 4096 bytes per page */
#endif                           /* FIO_MEM_PAGE_SIZE_LOG */

#if FIO_HAVE_UNIX_TOOLS || __has_include("sys/mman.h")
#include <sys/mman.h>

/* Mitigates MAP_ANONYMOUS not being defined on older versions of MacOS */
#if !defined(MAP_ANONYMOUS)
#if defined(MAP_ANON)
#define MAP_ANONYMOUS MAP_ANON
#else
#define MAP_ANONYMOUS 0
#endif /* defined(MAP_ANONYMOUS) */
#endif /* FIO_MEM_PAGE_ALLOC */

/*
 * allocates memory using `mmap`, but enforces alignment.
 */
FIO_SFUNC void *FIO_MEM_PAGE_ALLOC_def_func(size_t pages,
                                            uint8_t alignment_log) {
  void *result;
  static void *next_alloc = (void *)0x01;
  const size_t alignment_mask = (1ULL << alignment_log) - 1;
  const size_t alignment_size = (1ULL << alignment_log);
  pages <<= FIO_MEM_PAGE_SIZE_LOG;
  next_alloc =
      (void *)(((uintptr_t)next_alloc + alignment_mask) & alignment_mask);
/* hope for the best? */
#ifdef MAP_ALIGNED
  result = mmap(next_alloc,
                pages,
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS | MAP_ALIGNED(alignment_log),
                -1,
                0);
#else
  result = mmap(next_alloc,
                pages,
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS,
                -1,
                0);
#endif /* MAP_ALIGNED */
  if (result == MAP_FAILED)
    return (void *)NULL;
  if (((uintptr_t)result & alignment_mask)) {
    munmap(result, pages);
    result = mmap(NULL,
                  pages + alignment_size,
                  PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANONYMOUS,
                  -1,
                  0);
    if (result == MAP_FAILED) {
      return (void *)NULL;
    }
    const uintptr_t offset =
        (alignment_size - ((uintptr_t)result & alignment_mask));
    if (offset) {
      munmap(result, offset);
      result = (void *)((uintptr_t)result + offset);
    }
    munmap((void *)((uintptr_t)result + pages), alignment_size - offset);
  }
  next_alloc = (void *)((uintptr_t)result + (pages << 2));
  return result;
}

/*
 * Re-allocates memory using `mmap`, enforcing alignment.
 */
FIO_SFUNC void *FIO_MEM_PAGE_REALLOC_def_func(void *mem,
                                              size_t prev_pages,
                                              size_t new_pages,
                                              uint8_t alignment_log) {
  const size_t prev_len = prev_pages << FIO_MEM_PAGE_SIZE_LOG;
  const size_t new_len = new_pages << FIO_MEM_PAGE_SIZE_LOG;
  if (new_len > prev_len) {
    void *result;
#if defined(__linux__)
    result = mremap(mem, prev_len, new_len, 0);
    if (result != MAP_FAILED)
      return result;
#endif
    result = mmap((void *)((uintptr_t)mem + prev_len),
                  new_len - prev_len,
                  PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANONYMOUS,
                  -1,
                  0);
    if (result == (void *)((uintptr_t)mem + prev_len)) {
      result = mem;
    } else {
      /* copy and free */
      munmap(result, new_len - prev_len); /* free the failed attempt */
      result = FIO_MEM_PAGE_ALLOC_def_func(
          new_pages, alignment_log); /* allocate new memory */
      if (!result) {
        return (void *)NULL;
      }
      fio___memcpy_16byte(result, mem, prev_len >> 4); /* copy data */
      // memcpy(result, mem, prev_len);
      munmap(mem, prev_len); /* free original memory */
    }
    return result;
  }
  if (prev_len != new_len) /* remove dangling pages */
    munmap((void *)((uintptr_t)mem + new_len), prev_len - new_len);
  return mem;
}

/* frees memory using `munmap`. */
FIO_IFUNC void FIO_MEM_PAGE_FREE_def_func(void *mem, size_t pages) {
  munmap(mem, (pages << FIO_MEM_PAGE_SIZE_LOG));
}

#else /* FIO_HAVE_UNIX_TOOLS */

FIO_IFUNC void *FIO_MEM_PAGE_ALLOC_def_func(size_t pages,
                                            uint8_t alignment_log) {
  // return aligned_alloc((pages << 12), (1UL << alignment_log));
  exit(-1);
  (void)pages;
  (void)alignment_log;
}

FIO_IFUNC void *FIO_MEM_PAGE_REALLOC_def_func(void *mem,
                                              size_t prev_pages,
                                              size_t new_pages,
                                              uint8_t alignment_log) {
  (void)prev_pages;
  (void)alignment_log;
  return realloc(mem, (new_pages << 12));
}

FIO_IFUNC void FIO_MEM_PAGE_FREE_def_func(void *mem, size_t pages) {
  free(mem);
  (void)pages;
}

#endif /* FIO_HAVE_UNIX_TOOLS */

#define FIO_MEM_PAGE_ALLOC(pages, alignment_log)                               \
  FIO_MEM_PAGE_ALLOC_def_func((pages), (alignment_log))
#define FIO_MEM_PAGE_REALLOC(ptr, old_pages, new_pages, alignment_log)         \
  FIO_MEM_PAGE_REALLOC_def_func(                                               \
      (ptr), (old_pages), (new_pages), (alignment_log))
#define FIO_MEM_PAGE_FREE(ptr, pages) FIO_MEM_PAGE_FREE_def_func((ptr), (pages))

#define FIO_MEM_BYTES2PAGES(size)                                              \
  (((size) + ((1UL << FIO_MEM_PAGE_SIZE_LOG) - 1)) >> (FIO_MEM_PAGE_SIZE_LOG))

#endif /* FIO_MEM_PAGE_ALLOC */

/* *****************************************************************************
Allocator debugging helpers
***************************************************************************** */

#if DEBUG
/* maximum block allocation count. */
static size_t fio___mem_chunk_count_max;
/* current block allocation count. */
static size_t fio___mem_chunk_count;

#define FIO_MEMORY_ON_CHUNK_ALLOC(ptr)                                         \
  do {                                                                         \
    FIO_LOG_DEBUG2("MEMORY SYS-ALLOC - retrieved %p from system", ptr);        \
    fio_atomic_add(&fio___mem_chunk_count, 1);                                 \
    if (fio___mem_chunk_count > fio___mem_chunk_count_max)                     \
      fio___mem_chunk_count_max = fio___mem_chunk_count;                       \
  } while (0)
#define FIO_MEMORY_ON_CHUNK_FREE(ptr)                                          \
  do {                                                                         \
    FIO_LOG_DEBUG2("MEMORY SYS-DEALLOC- returned %p to system", ptr);          \
    fio_atomic_sub_fetch(&fio___mem_chunk_count, 1);                           \
  } while (0)
#define FIO_MEMORY_ON_CHUNK_CACHE(ptr)                                         \
  do {                                                                         \
    FIO_LOG_DEBUG2("MEMORY CACHE-DEALLOC placed %p in cache", ptr);            \
  } while (0);
#define FIO_MEMORY_ON_CHUNK_UNCACHE(ptr)                                       \
  do {                                                                         \
    FIO_LOG_DEBUG2("MEMORY CACHE-ALLOC retrieved %p from cache", ptr);         \
  } while (0);
#define FIO_MEMORY_PRINT_STATS()                                               \
  FIO_LOG_INFO(                                                                \
      "(fio) Total memory chunks allocated before cleanup %zu\n"               \
      "       Maximum memory blocks allocated at a single time %zu\n",         \
      fio___mem_chunk_count,                                                   \
      fio___mem_chunk_count_max)
#define FIO_MEMORY_PRINT_STATS_END()                                           \
  do {                                                                         \
    if (fio___mem_chunk_count) {                                               \
      FIO_LOG_ERROR("(fio) Total memory chunks allocated "                     \
                    "after cleanup (POSSIBLE LEAKS): %zu\n",                   \
                    fio___mem_chunk_count);                                    \
    } else {                                                                   \
      FIO_LOG_INFO("(fio) Total memory chunks allocated after cleanup: %zu\n", \
                   fio___mem_chunk_count);                                     \
    }                                                                          \
  } while (0)
#else /* DEBUG */
#define FIO_MEMORY_ON_CHUNK_ALLOC(ptr)
#define FIO_MEMORY_ON_CHUNK_FREE(ptr)
#define FIO_MEMORY_ON_CHUNK_CACHE(ptr)
#define FIO_MEMORY_ON_CHUNK_UNCACHE(ptr)
#define FIO_MEMORY_PRINT_STATS()
#define FIO_MEMORY_PRINT_STATS_END()
#endif /* DEBUG */

/* *****************************************************************************
Lock type choice
***************************************************************************** */
#if defined(FIO_MEMORY_USE_PTHREAD_MUTEX) && FIO_MEMORY_USE_PTHREAD_MUTEX
#include "pthread.h"
#define FIO_MEMORY_LOCK_TYPE pthread_mutex_t
#define FIO_MEMORY_LOCK_TYPE_INIT (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER
#define FIO_MEMORY_TRYLOCK(lock) pthread_mutex_trylock(&(lock))
#define FIO_MEMORY_LOCK(lock) pthread_mutex_lock(&(lock))
#define FIO_MEMORY_UNLOCK(lock) pthread_mutex_unlock(&(lock))
#else
#define FIO_MEMORY_LOCK_TYPE fio_lock_i
#define FIO_MEMORY_LOCK_TYPE_INIT FIO_LOCK_INIT
#define FIO_MEMORY_TRYLOCK(lock) fio_trylock(&(lock))
#define FIO_MEMORY_LOCK(lock) fio_lock(&(lock))
#define FIO_MEMORY_UNLOCK(lock) fio_unlock(&(lock))
#endif
/* *****************************************************************************
Memory Allocator Types
***************************************************************************** */

#ifndef FIO_LOCK_INIT
#define FIO_ATOMIC
#include __FILE__
#endif

typedef struct fio___mem_block_s fio___mem_block_s;
typedef struct fio___mem_big_block_s fio___mem_big_block_s;
typedef struct fio___mem_chunk_s fio___mem_chunk_s;
typedef struct fio___mem_arena_s fio___mem_arena_s;

/* must consume exactly 16 bytes for allocation alignment - overlays chunk */
struct fio___mem_block_s {
  uint32_t marker;
  uint32_t ref;
  uint32_t pos;
  uint32_t reserved;
};

/* must consume exactly 16 bytes for allocation alignment - overlays chunk */
struct fio___mem_big_block_s {
  uint32_t marker;
  uint32_t ref;
  uint32_t pos;
  uint32_t reserved;
};

#undef FIO_MEMORY_BIG_BLOCK_MARKER
#define FIO_MEMORY_BIG_BLOCK_MARKER (~(uint32_t)1)

/* must consume exactly 16 bytes for allocation alignment - overlays block */
struct fio___mem_chunk_s {
  /** if marker is non-zero, it contains the number of pages allocated */
  uint32_t marker;
  int32_t reserved[2];
  uint32_t ref;
};

/* must consume exactly 16 bytes for allocation alignment - overlays block */
struct fio___mem_arena_s {
  fio___mem_big_block_s *big;
  fio___mem_block_s *block;
  FIO_MEMORY_LOCK_TYPE lock;
};

/* *****************************************************************************
Memory Allocator State Containers
***************************************************************************** */

struct fio___mem_state_s {
  size_t cores;
#if defined(FIO_MEMORY_CACHE_SLOTS) && FIO_MEMORY_CACHE_SLOTS
  struct {
    fio___mem_chunk_s *store[FIO_MEMORY_CACHE_SLOTS];
    size_t pos;
  } cache;
  fio___mem_block_s *bstore[FIO_MEMORY_CACHE_SLOTS];
#endif
  fio___list_node_s available;
  FIO_MEMORY_LOCK_TYPE
  lock; /* locks the cache and the available block linked list. */
  fio___mem_arena_s a[];
} *fio___mem_state = NULL;

#undef FIO_MEMORY___STATE_SIZE
#define FIO_MEMORY___STATE_SIZE(cores)                                         \
  FIO_MEM_BYTES2PAGES(                                                         \
      (sizeof(*fio___mem_state) + (sizeof(fio___mem_arena_s *) * cores)))

/* *****************************************************************************
Helpers for system allocation / deallocation of chunks.
***************************************************************************** */

FIO_IFUNC void fio___mem_chunk_dealloc(fio___mem_chunk_s *c) {
  FIO_MEM_PAGE_FREE(
      c, (1UL << (FIO_MEMORY_SYS_ALLOCATION_SIZE_LOG - FIO_MEM_PAGE_SIZE_LOG)));
  FIO_MEMORY_ON_CHUNK_FREE(c);
}

FIO_IFUNC fio___mem_chunk_s *fio___mem_chunk_alloc(void) {
  fio___mem_chunk_s *c = (fio___mem_chunk_s *)FIO_MEM_PAGE_ALLOC(
      (1UL << (FIO_MEMORY_SYS_ALLOCATION_SIZE_LOG - FIO_MEM_PAGE_SIZE_LOG)),
      FIO_MEMORY_SYS_ALLOCATION_SIZE_LOG);
  FIO_MEMORY_ON_CHUNK_ALLOC(c);
  return c;
}

/* *****************************************************************************
Helpers for pointer mathematics
***************************************************************************** */

FIO_IFUNC fio___mem_chunk_s *fio___mem_ptr2chunk(void *p) {
  /* use a bit mask to find the original chunk, adjust for header offset */
  fio___mem_chunk_s *c = FIO_PTR_MATH_RMASK(
      fio___mem_chunk_s, ((uintptr_t)p), FIO_MEMORY_SYS_ALLOCATION_SIZE_LOG);
  return c;
}

FIO_IFUNC fio___mem_block_s *fio___mem_ptr2block(void *p) {
  /* use a bit mask to find the offset of the block
   *  + adjust for the 16 byte chunk header offset
   */
  fio___mem_block_s *b = FIO_PTR_MATH_RMASK(
      fio___mem_block_s, ((uintptr_t)p), (FIO_MEMORY_BLOCK_SIZE_LOG));
  return b;
}

/* *****************************************************************************
Dirty list push / pop / remove
***************************************************************************** */

FIO_IFUNC void fio___mem_list___push_node(fio___list_node_s *restrict head,
                                          fio___list_node_s *restrict n) {
  // FIO_LOG_DEBUG("available block list pushing node %p", (void *)n);
  if (!n || n->next || n->prev)
    return; /* if non-zero, the  node is in use */
  n->prev = head->prev;
  n->next = head;
  head->prev->next = n;
  head->prev = n;
}
FIO_IFUNC void fio___mem_list_remove_node(fio___list_node_s *n) {
  // FIO_LOG_DEBUG("available block list popping node %p", (void *)n);
  if (!n || !n->next || !n->prev) {
    FIO_LOG_FATAL("memory allocator corruption? refused to remove node at %p "
                  "where next = %p prev = %p",
                  (void *)n,
                  (void *)(n ? n->next : n),
                  (void *)(n ? n->prev : n));
    abort();
    return; /* node isn't in list */
  }
  n->prev->next = n->next;
  n->next->prev = n->prev;
  n->next = n->prev = NULL;
}

FIO_IFUNC fio___list_node_s *fio___mem_block2node(fio___mem_block_s *b) {
  return (fio___list_node_s *)(b + 1);
}

FIO_IFUNC fio___mem_block_s *fio___mem_node2block(fio___list_node_s *n) {
  return ((fio___mem_block_s *)n) - 1;
}

FIO_IFUNC void fio___mem_list_push(fio___list_node_s *head,
                                   fio___mem_block_s *b) {
  fio___mem_list___push_node(head, fio___mem_block2node(b));
}

FIO_IFUNC fio___mem_block_s *fio___mem_list_pop(fio___list_node_s *head) {
  fio___mem_block_s *b = NULL;
  if (!head || head->next == head->prev)
    return b;
  fio___list_node_s *n = head->prev;
  fio___mem_list_remove_node(n);
  b = fio___mem_node2block(n);
  return b;
}

/** removes a chunk from the dirty list, returns -1 on failure. */
FIO_IFUNC void fio___mem_list_remove(fio___list_node_s *head,
                                     fio___mem_chunk_s *c) {
  if (!c || !head)
    return;
  if (head->next == head)
    return;
  for (size_t i = 0; i < FIO_MEMORY_BLOCKS_PER_ALLOCATION; ++i) {
    fio___list_node_s *n = fio___mem_block2node(
        (fio___mem_block_s *)((uintptr_t)c + (FIO_MEMORY_BLOCK_SIZE * i)));
    fio___mem_list_remove_node(n);
  }
}

/* *****************************************************************************
Arena selection / locking
***************************************************************************** */

static __thread size_t fio___mem_arena_index = 0;

fio___mem_arena_s *fio___mem_arena_lock(void) {
  size_t a = fio___mem_arena_index;
  for (;;) {
    for (size_t i = 0; i < fio___mem_state->cores; ++i) {
      if (a >= fio___mem_state->cores)
        a = 0;
      if (!FIO_MEMORY_TRYLOCK(fio___mem_state->a[a].lock)) {
        fio___mem_arena_index = a;
        return &fio___mem_state->a[a];
      }
      ++a;
    }
    FIO_THREAD_RESCHEDULE();
  }
}

void fio___mem_arena_unlock(fio___mem_arena_s *a) {
  FIO_MEMORY_UNLOCK(a->lock);
}

/* *****************************************************************************
Chunk allocation / deallocation
***************************************************************************** */

/* seeks the cache first, then allocates from the system*/
FIO_SFUNC fio___mem_chunk_s *fio___mem_chunk_new(void) {
  fio___mem_chunk_s *c = NULL;

#if defined(FIO_MEMORY_CACHE_SLOTS) && FIO_MEMORY_CACHE_SLOTS
  if (fio___mem_state->cache.pos) {
    c = fio___mem_state->cache.store[--fio___mem_state->cache.pos];
    FIO_MEMORY_ON_CHUNK_UNCACHE(c);
  }
#endif
  if (!c) {
    c = fio___mem_chunk_alloc();
  }
  return c;
}

FIO_SFUNC void fio___mem_chunk_free(fio___mem_chunk_s *c, int has_blocks) {
  FIO_ASSERT(!c->marker,
             "memory allocator corruption, block header overwritten?\n\t%p "
             "marked with %p",
             (void *)c,
             (void *)(*(uintptr_t *)c));
  if (!c || fio_atomic_sub_fetch(&c->ref, 1))
    return;
  if (fio___mem_state) {
    if (has_blocks) {
      fio___mem_list_remove(&fio___mem_state->available, c);
    }
#if defined(FIO_MEMORY_CACHE_SLOTS) && FIO_MEMORY_CACHE_SLOTS
    if (fio___mem_state->cache.pos < FIO_MEMORY_CACHE_SLOTS) {
      fio___mem_state->cache.store[fio___mem_state->cache.pos++] = c;
      FIO_MEMORY_ON_CHUNK_CACHE(c);
      return;
    }
#endif
  }
  fio___mem_chunk_dealloc(c);
}

/* *****************************************************************************
Block allocation / deallocation
***************************************************************************** */

/* seeks the cache first, then allocates from the system*/
FIO_SFUNC fio___mem_block_s *fio___mem_block_new(void) {
  fio___mem_block_s *b = NULL;
#if defined(FIO_MEMORY_CACHE_SLOTS) && FIO_MEMORY_CACHE_SLOTS
  for (size_t i = 0; i < FIO_MEMORY_CACHE_SLOTS; ++i) {
    b = fio_atomic_exchange(fio___mem_state->bstore + i, b);
    if (b) {
      b->pos = 1; /* 1st unit is the block header */
      b->ref = 1; /* block reference */
      return b;
    }
  }
#endif

  FIO_MEMORY_LOCK(fio___mem_state->lock);
  b = fio___mem_list_pop(&fio___mem_state->available);
  if (!b) {
    fio___mem_chunk_s *c = fio___mem_chunk_new();
    if (!c)
      goto finish;

    for (size_t i = 0; i < (FIO_MEMORY_BLOCKS_PER_ALLOCATION - 1); ++i) {
      fio___mem_list_push(
          &fio___mem_state->available,
          (fio___mem_block_s *)((uintptr_t)c + (FIO_MEMORY_BLOCK_SIZE * i)));
    }
    b = (fio___mem_block_s *)((uintptr_t)c +
                              (FIO_MEMORY_BLOCK_SIZE *
                               (FIO_MEMORY_BLOCKS_PER_ALLOCATION - 1)));
  }

finish:
  if (b) {
    b->pos = 1; /* 1st unit is the block header */
    b->ref = 1; /* block reference */
    fio_atomic_add(&fio___mem_ptr2chunk(b)->ref, 1); /* mark chunk reference */
  }
  FIO_MEMORY_UNLOCK(fio___mem_state->lock);
  return b;
}

FIO_SFUNC void fio___mem_block_free(fio___mem_block_s *b) {
  if (!b)
    return;
  FIO_ASSERT(!b->marker,
             "memory allocator corruption, block header overwritten?\n\t%p "
             "marked with %p",
             (void *)b,
             (void *)(*(uintptr_t *)b));
  if (fio_atomic_sub_fetch(&b->ref, 1))
    return;
  FIO___MEMSET(b + 1, 0, FIO_MEMORY_BLOCK_SIZE - 16);

#if defined(FIO_MEMORY_CACHE_SLOTS) && FIO_MEMORY_CACHE_SLOTS
  for (size_t i = 0; i < FIO_MEMORY_CACHE_SLOTS; ++i) {
    b = fio_atomic_exchange(fio___mem_state->bstore + i, b);
    if (!b)
      return;
  }
#endif
  fio___mem_chunk_s *c = fio___mem_ptr2chunk(b);

  FIO_MEMORY_LOCK(fio___mem_state->lock);
  fio___mem_list_push(&fio___mem_state->available, b);
  fio___mem_chunk_free(c, 1);
  FIO_MEMORY_UNLOCK(fio___mem_state->lock);
}

/* *****************************************************************************
Slice allocation / deallocation (slicing the block)
***************************************************************************** */

FIO_IFUNC void *fio___mem_slice(fio___mem_block_s *b, size_t units) {
  /* Note: each unit is 16 bytes long */
  void *ret = NULL;
  if (!b)
    return ret;

  /* if we hold the only reference, all slices were freed... */
  if (b->ref == 1 && b->pos > 1) {
    /* reset used memory to 0 and reset slice counter */
    FIO___MEMSET(b + 1, 0, (size_t)(b->pos - 1) << 4);
    b->pos = 1;
  }

  /* enough room? */
  if (b->pos + units >= (FIO_MEMORY_BLOCK_SIZE >> 4))
    return ret;

  ret = (void *)(b + b->pos);
  b->pos += units;
  fio_atomic_add(&b->ref, 1);
  return ret;
}

FIO_IFUNC void fio___mem_slice_free(void *p) {
  fio___mem_block_s *b = fio___mem_ptr2block(p);
  fio___mem_block_free(b);
}

/* *****************************************************************************
Big Block allocation / deallocation (slicing the big block)
***************************************************************************** */

FIO_IFUNC fio___mem_big_block_s *fio___mem_big_block_new(void) {
  FIO_MEMORY_LOCK(fio___mem_state->lock);
  fio___mem_big_block_s *b = (fio___mem_big_block_s *)fio___mem_chunk_new();
  FIO_MEMORY_UNLOCK(fio___mem_state->lock);
  b->ref = 1;
  b->pos = 1;
  b->marker = FIO_MEMORY_BIG_BLOCK_MARKER;
  return b;
}
FIO_IFUNC void *fio___mem_big_slice(fio___mem_big_block_s *b, size_t units) {
  /* Note: each unit is 16 bytes long */
  void *ret = NULL;
  if (!b)
    return ret;

  /* if we hold the only reference, all slices were freed... */
  if (b->ref == 1 && b->pos > 1) {
    /* reset used memory to 0 and reset slice counter */
    FIO___MEMSET(b + 1, 0, (size_t)b->pos << FIO_MEM_PAGE_SIZE_LOG);
    b->pos = 1;
  }

  /* enough room? */
  if (b->pos + units >=
      (FIO_MEMORY_SYS_ALLOCATION_SIZE >> FIO_MEM_PAGE_SIZE_LOG))
    return ret;

  ret = (void *)((uintptr_t)(b + 1) +
                 ((uintptr_t)b->pos << FIO_MEM_PAGE_SIZE_LOG));
  b->pos += units;
  fio_atomic_add(&b->ref, 1);
  return ret;
}

FIO_IFUNC void fio___mem_big_slice_free(void *p) {
  fio___mem_big_block_s *b = (fio___mem_big_block_s *)fio___mem_ptr2chunk(p);
  if (!b)
    return;
  if (fio_atomic_sub_fetch(&b->ref, 1))
    return;
  FIO___MEMSET(b, 0, FIO_MEMORY_SYS_ALLOCATION_SIZE);
  fio___mem_chunk_s *c = (fio___mem_chunk_s *)b;
  c->ref = 1;
  FIO_MEMORY_LOCK(fio___mem_state->lock);
  fio___mem_chunk_free(c, 0);
  FIO_MEMORY_UNLOCK(fio___mem_state->lock);
}

/* *****************************************************************************
Arena based allocations
***************************************************************************** */

FIO_IFUNC void *fio___mem_alloc_normal(size_t size) {
  void *r = NULL;
  /* convert bytes to units */
  size = (size + 15) >> 4;
  fio___mem_arena_s *a = fio___mem_arena_lock();
  if (!a->block)
    a->block = fio___mem_block_new();
  r = fio___mem_slice(a->block, size);
  if (!r)
    goto rotate;
  fio___mem_arena_unlock(a);
  return r;
rotate:
  // FIO_LOG_DEBUG2("ROTATING memory block / chunk");
  fio___mem_block_free(a->block);
  a->block = fio___mem_block_new();
  r = fio___mem_slice(a->block, size);
  fio___mem_arena_unlock(a);
  return r;
}

FIO_IFUNC void *fio___mem_alloc_big(size_t size) {
  void *r = NULL;
  /* convert bytes to units */
  size = FIO_MEM_BYTES2PAGES(size);
  fio___mem_arena_s *a = fio___mem_arena_lock();
  if (!a->big)
    a->big = fio___mem_big_block_new();
  r = fio___mem_big_slice(a->big, size);
  if (!r)
    goto rotate;
  fio___mem_arena_unlock(a);
  return r;
rotate:
  // FIO_LOG_DEBUG2("ROTATING memory block / chunk");
  fio___mem_big_slice_free(a->big);
  a->big = fio___mem_big_block_new();
  r = fio___mem_big_slice(a->big, size);
  fio___mem_arena_unlock(a);
  return r;
}

/* *****************************************************************************
Core allocator state Construction
***************************************************************************** */

FIO_SFUNC void __attribute__((constructor)) fio___mem_state_allocate(void) {
  if (fio___mem_state)
    return;

  FIO_ASSERT_DEBUG(sizeof(fio___mem_block_s) == 16,
                   "fio___mem_block_s size error");
  FIO_ASSERT_DEBUG(sizeof(fio___mem_chunk_s) == 16,
                   "fio___mem_chunk_s size error");

#ifdef _SC_NPROCESSORS_ONLN
  size_t cores = sysconf(_SC_NPROCESSORS_ONLN);
  if ((intptr_t)cores <= 0)
    cores = FIO_MEMORY_ARENA_COUNT_DEFAULT;
#else
#warning Dynamic CPU core count is unavailable - assuming FIO_MEMORY_ARENA_COUNT_DEFAULT cores.
  size_t cores = FIO_MEMORY_ARENA_COUNT_DEFAULT;
  if ((intptr_t)cores <= 0)
    cores = 1;
#endif /* _SC_NPROCESSORS_ONLN */
#if FIO_MEMORY_ARENA_COUNT_MAX
  if (cores >= FIO_MEMORY_ARENA_COUNT_MAX)
    cores = FIO_MEMORY_ARENA_COUNT_MAX;
#endif /* FIO_MEMORY_ARENA_COUNT_MAX */
  if ((intptr_t)cores <= 0)
    cores = 1;

  const size_t pages = FIO_MEMORY___STATE_SIZE(cores);
  fio___mem_state = (struct fio___mem_state_s *)FIO_MEM_PAGE_ALLOC(pages, 1);
  FIO_ASSERT_ALLOC(fio___mem_state);
  *fio___mem_state = (struct fio___mem_state_s){
      .cores = cores,
      .available.next = &fio___mem_state->available,
      .available.prev = &fio___mem_state->available,
      .lock = FIO_MEMORY_LOCK_TYPE_INIT,
  };
  for (size_t i = 0; i < cores; ++i) {
    fio___mem_state->a[i].lock = FIO_MEMORY_LOCK_TYPE_INIT;
  }

#if DEBUG && defined(FIO_LOG_INFO)
  FIO_LOG_INFO(
      "facil.io memory allocation initialized:\n"
      "\t* %zu concurrent arenas (@%p).\n"
      "\t* %zu pages required for arenas (~%zu bytes).\n"
      "\t* system allocation size:                     %zu bytes\n"
      "\t* system allocation overhead:                 %zu bytes\n"
      "\t* memory block size (allocation slice / bin): %zu bytes\n"
      "\t* memory blocks per system allocation:        %zu blocks\n"
      "\t* memory block overhead:                      %zu bytes\n"
      "\t* allocator small allocation limit:           %zu bytes\n"
      "\t* allocator limit (revert to mmap):           %zu bytes\n",
      cores,
      (void *)fio___mem_state,
      (size_t)pages,
      (size_t)(pages << FIO_MEM_PAGE_SIZE_LOG),
      (size_t)(FIO_MEMORY_BLOCKS_PER_ALLOCATION * FIO_MEMORY_BLOCK_SIZE),
      sizeof(fio___mem_chunk_s),
      (size_t)FIO_MEMORY_BLOCK_SIZE,
      (size_t)FIO_MEMORY_BLOCKS_PER_ALLOCATION,
      sizeof(fio___mem_block_s),
      (size_t)FIO_MEMORY_BLOCK_ALLOC_LIMIT,
      (size_t)FIO_MEMORY_BIG_BLOCK_ALLOC_LIMIT);
#endif /* DEBUG */
}

FIO_SFUNC void fio___mem_state_deallocate(void) {
  if (!fio___mem_state)
    return;
  const size_t pages = FIO_MEMORY___STATE_SIZE(fio___mem_state->cores);
  FIO_MEM_PAGE_FREE(fio___mem_state, pages);
  fio___mem_state = (struct fio___mem_state_s *)NULL;
}

/* *****************************************************************************
Allocator Destruction
***************************************************************************** */

FIO_SFUNC void __attribute__((destructor)) fio___mem_destroy(void) {
  if (!fio___mem_state)
    return;
  FIO_MEMORY_PRINT_STATS();
  for (size_t i = 0; i < fio___mem_state->cores; ++i) {
    /* free all blocks in the arenas */
    /* this may place chunks in the cache */
    fio___mem_block_s *b = fio___mem_state->a[i].block;
    fio___mem_big_block_s *big = fio___mem_state->a[i].big;
    fio___mem_state->a[i].block = NULL;
    fio___mem_block_free(b);
    fio___mem_big_slice_free(big);
  }
#if defined(FIO_MEMORY_CACHE_SLOTS) && FIO_MEMORY_CACHE_SLOTS
  for (size_t i = 0; i < FIO_MEMORY_CACHE_SLOTS; ++i) {
    fio___mem_block_s *b =
        fio_atomic_exchange(fio___mem_state->bstore + i, NULL);
    if (b) {
      fio___mem_list_push(&fio___mem_state->available, b);
      fio___mem_chunk_free(fio___mem_ptr2chunk(b), 1);
    }
  }
  while (fio___mem_state->cache.pos) {
    fio___mem_chunk_s *c =
        fio___mem_state->cache.store[--fio___mem_state->cache.pos];
    fio___mem_chunk_dealloc(c);
  }
#endif
  fio___mem_state_deallocate();
  FIO_MEMORY_PRINT_STATS_END();
#if DEBUG && defined(FIO_LOG_INFO)
  FIO_LOG_INFO("facil.io memory allocation cleanup complete.");
#endif /* DEBUG */
}

/* *****************************************************************************
Public facing API
***************************************************************************** */

/* Address returned when allocating 0 bytes ( fio_malloc(0) ) */
static long double fio___mem_on_malloc_zero;

/**
 * Allocates memory using a per-CPU core block memory pool.
 * Memory is zeroed out.
 *
 * Allocations above FIO_MEMORY_BLOCK_ALLOC_LIMIT (16Kb when using 32Kb blocks)
 * will be redirected to `mmap`, as if `fio_mmap` was called.
 *
 * `fio_malloc` promises a best attempt at providing locality between
 * consecutive calls, but locality can't be guaranteed.
 */
SFUNC void *FIO_ALIGN_NEW fio_malloc(size_t size) {
  if (!size)
    return &fio___mem_on_malloc_zero;
  if (!fio___mem_state)
    fio___mem_state_allocate();
  if (size <= FIO_MEMORY_BLOCK_ALLOC_LIMIT) {
    return fio___mem_alloc_normal(size);
  }
  if (size <= FIO_MEMORY_BIG_BLOCK_ALLOC_LIMIT) {
    return fio___mem_alloc_big(size);
  }
  return fio_mmap(size);
}

/**
 * same as calling `fio_malloc(size_per_unit * unit_count)`;
 *
 * Allocations above FIO_MEMORY_BLOCK_ALLOC_LIMIT (16Kb when using 32Kb blocks)
 * will be redirected to `mmap`, as if `fio_mmap` was called.
 */
SFUNC void *FIO_ALIGN_NEW fio_calloc(size_t size_per_unit, size_t unit_count) {
  return fio_malloc(size_per_unit * unit_count);
}

/** Frees memory that was allocated using this library. */
SFUNC void fio_free(void *ptr) {
  if (!ptr || ptr == &fio___mem_on_malloc_zero)
    return;
  fio___mem_chunk_s *c = fio___mem_ptr2chunk(ptr);
  /* big slice allocation? */
  if (c->marker == FIO_MEMORY_BIG_BLOCK_MARKER)
    goto big_free;
  /* if the allocation is after a marked chunk header it was fio_mmap */
  if ((uintptr_t)(c + 1) == (uintptr_t)ptr && c->marker)
    goto mmap_free;
  fio___mem_slice_free(ptr);
  return;
big_free:
  fio___mem_big_slice_free(ptr);
  return;
mmap_free:
  /* zero out memory before returning it to the system */
  FIO___MEMSET(
      ptr, 0, ((size_t)c->marker << FIO_MEM_PAGE_SIZE_LOG) - sizeof(*c));
  FIO_MEM_PAGE_FREE(c, c->marker);
  FIO_MEMORY_ON_CHUNK_FREE(c);
}

/**
 * Re-allocates memory. An attempt to avoid copying the data is made only for
 * big memory allocations (larger than FIO_MEMORY_BLOCK_ALLOC_LIMIT).
 */
SFUNC void *FIO_ALIGN fio_realloc(void *ptr, size_t new_size) {
  return fio_realloc2(ptr, new_size, new_size);
}

/**
 * Uses system page maps for reallocation.
 */
FIO_SFUNC void *fio_realloc2_big(fio___mem_chunk_s *c, size_t new_size) {
  const size_t new_page_len = FIO_MEM_BYTES2PAGES(new_size + sizeof(*c));
  c = (fio___mem_chunk_s *)FIO_MEM_PAGE_REALLOC(
      c,
      c->marker >> FIO_MEM_PAGE_SIZE_LOG,
      new_page_len,
      FIO_MEMORY_SYS_ALLOCATION_SIZE_LOG);
  if (!c)
    return NULL;
  c->marker = new_page_len;
  return (void *)(c + 1);
}

/**
 * Re-allocates memory. An attempt to avoid copying the data is made only for
 * big memory allocations (larger than FIO_MEMORY_BLOCK_ALLOC_LIMIT).
 *
 * This variation is slightly faster as it might copy less data.
 */
SFUNC void *FIO_ALIGN fio_realloc2(void *ptr,
                                   size_t new_size,
                                   size_t copy_len) {
  void *mem = NULL;
  if (!new_size) {
    fio_free(ptr);
    mem = (void *)&fio___mem_on_malloc_zero;
    return mem;
  }
  if (!ptr || ptr == (void *)&fio___mem_on_malloc_zero) {
    mem = fio_malloc(new_size);
    return mem;
  }

  { /* test for big-paged malloc and limit copy_len */
    register fio___mem_chunk_s *c = fio___mem_ptr2chunk(ptr);
    register fio___mem_block_s *const b = fio___mem_ptr2block(ptr);
    register size_t max_len =
        ((uintptr_t)b + FIO_MEMORY_BLOCK_SIZE) - ((uintptr_t)ptr);
    if (c->marker == FIO_MEMORY_BIG_BLOCK_MARKER) {
      /* big-slice reallocation */
      max_len =
          ((uintptr_t)c + FIO_MEMORY_SYS_ALLOCATION_SIZE) - ((uintptr_t)ptr);
    } else if ((uintptr_t)(c + 1) == (uintptr_t)ptr && c->marker) {
      if (new_size > FIO_MEMORY_BIG_BLOCK_ALLOC_LIMIT)
        return (mem = fio_realloc2_big(c, new_size));
      max_len = new_size; /* shrinking from mmap to allocator */
    }

    if (copy_len > max_len)
      copy_len = max_len;
    if (copy_len > new_size)
      copy_len = new_size;
  }

  mem = fio_malloc(new_size);
  if (!mem) {
    return mem;
  }
  fio___memcpy_16byte(mem, ptr, ((copy_len + 15) >> 4));
  fio_free(ptr);
  // zero out leftover bytes, if any.
  while (copy_len & 15) {
    ((uint8_t *)mem)[(copy_len++) & 15] = 0;
  }

  return mem;
}

/**
 * Allocates memory directly using `mmap`, this is preferred for objects that
 * both require almost a page of memory (or more) and expect a long lifetime.
 *
 * However, since this allocation will invoke the system call (`mmap`), it will
 * be inherently slower.
 *
 * `fio_free` can be used for deallocating the memory.
 */
SFUNC void *FIO_ALIGN_NEW fio_mmap(size_t size) {
  if (!size)
    return &fio___mem_on_malloc_zero;
  size_t pages = FIO_MEM_BYTES2PAGES(size + sizeof(fio___mem_chunk_s));
  if (((uint64_t)pages >> 32))
    return NULL;
  fio___mem_chunk_s *c = (fio___mem_chunk_s *)FIO_MEM_PAGE_ALLOC(
      pages, FIO_MEMORY_SYS_ALLOCATION_SIZE_LOG);
  if (!c)
    return NULL;
  FIO_MEMORY_ON_CHUNK_ALLOC(c);
  c->marker = (uint32_t)pages;
  return (void *)(c + 1);
}

/**
 * When forking is called manually, call this function to reset the facil.io
 * memory allocator's locks.
 */
SFUNC void fio_malloc_after_fork(void) {
  if (!fio___mem_state)
    return;
  for (size_t i = 0; i < fio___mem_state->cores; ++i) {
    fio___mem_state->a[i].lock = FIO_MEMORY_LOCK_TYPE_INIT;
  }
  fio___mem_state->lock = FIO_MEMORY_LOCK_TYPE_INIT;
}

/* *****************************************************************************
Print Memory Allocator State (for debugging)
***************************************************************************** */

SFUNC void fio_malloc_print_state(void) {
  if (!fio___mem_state) {
    FIO_LOG_INFO("Printing facil.io allocator memory state: UNINITIALIZED!");
  } else {
    char buffer[FIO_LOG_LENGTH_LIMIT];
    size_t pos = 0;
    memcpy(buffer + pos, "Printing facil.io allocator memory state:\n", 42);
    pos += 42;
    pos += snprintf(buffer + pos,
                    FIO_LOG_LENGTH_LIMIT - pos,
                    "\tCores: %zu",
                    fio___mem_state->cores);
    for (size_t i = 0; i < fio___mem_state->cores; ++i) {
      size_t w;
      w = snprintf(buffer + pos,
                   FIO_LOG_LENGTH_LIMIT - pos,
                   "\n\t[%zu] block: %p",
                   i,
                   (void *)fio___mem_state->a[i].block);
      if (w + pos + 32 > FIO_LOG_LENGTH_LIMIT)
        goto flush;
      if (fio___mem_state->a[i].block) {
        w += snprintf(buffer + pos + w,
                      FIO_LOG_LENGTH_LIMIT - (pos + w),
                      "\n\t[%zu] block ref: %u"
                      "\n\t[%zu] block pos: %u",
                      i,
                      fio___mem_state->a[i].block->ref,
                      i,
                      fio___mem_state->a[i].block->pos);
        if (w + pos + 32 > FIO_LOG_LENGTH_LIMIT)
          goto flush;
      }
      pos += w;
      continue;
    flush:
      buffer[pos] = 0;
      FIO_LOG_INFO("%s", buffer);
      pos = 0;
      --i;
    }
    buffer[pos] = 0;
    FIO_LOG_INFO("%s", buffer);
  }
}
/* *****************************************************************************
Memory Allocation - test
***************************************************************************** */
#ifdef FIO_TEST_CSTL
#ifdef FIO_MALLOC_FORCE_SYSTEM
FIO_SFUNC void FIO_NAME_TEST(stl, mem)(void) {
  fprintf(stderr, "* Custom memory allocator bypassed.\n");
}

#else /* FIO_MALLOC_FORCE_SYSTEM */

#include "pthread.h"

FIO_IFUNC void fio___memset_test_aligned(void *restrict dest_,
                                         uint64_t data,
                                         size_t bytes,
                                         const char *msg) {
  uint64_t *dest = (uint64_t *)dest_;
  size_t units = bytes >> 3;
  FIO_ASSERT(
      *(dest) = data, "%s memory data was overwritten (first 8 bytes)", msg);
  while (units >= 16) {
    FIO_ASSERT(dest[0] == data && dest[1] == data && dest[2] == data &&
                   dest[3] == data && dest[4] == data && dest[5] == data &&
                   dest[6] == data && dest[7] == data && dest[8] == data &&
                   dest[9] == data && dest[10] == data && dest[11] == data &&
                   dest[12] == data && dest[13] == data && dest[14] == data &&
                   dest[15] == data,
               "%s memory data was overwritten",
               msg);
    dest += 16;
    units -= 16;
  }
  switch (units) {
  case 15:
    FIO_ASSERT(*(dest++) = data,
               "%s memory data was overwritten",
               msg); /* fallthrough */
  case 14:
    FIO_ASSERT(*(dest++) = data,
               "%s memory data was overwritten",
               msg); /* fallthrough */
  case 13:
    FIO_ASSERT(*(dest++) = data,
               "%s memory data was overwritten",
               msg); /* fallthrough */
  case 12:
    FIO_ASSERT(*(dest++) = data,
               "%s memory data was overwritten",
               msg); /* fallthrough */
  case 11:
    FIO_ASSERT(*(dest++) = data,
               "%s memory data was overwritten",
               msg); /* fallthrough */
  case 10:
    FIO_ASSERT(*(dest++) = data,
               "%s memory data was overwritten",
               msg); /* fallthrough */
  case 9:
    FIO_ASSERT(*(dest++) = data,
               "%s memory data was overwritten",
               msg); /* fallthrough */
  case 8:
    FIO_ASSERT(*(dest++) = data,
               "%s memory data was overwritten",
               msg); /* fallthrough */
  case 7:
    FIO_ASSERT(*(dest++) = data,
               "%s memory data was overwritten",
               msg); /* fallthrough */
  case 6:
    FIO_ASSERT(*(dest++) = data,
               "%s memory data was overwritten",
               msg); /* fallthrough */
  case 5:
    FIO_ASSERT(*(dest++) = data,
               "%s memory data was overwritten",
               msg); /* fallthrough */
  case 4:
    FIO_ASSERT(*(dest++) = data,
               "%s memory data was overwritten",
               msg); /* fallthrough */
  case 3:
    FIO_ASSERT(*(dest++) = data,
               "%s memory data was overwritten",
               msg); /* fallthrough */
  case 2:
    FIO_ASSERT(*(dest++) = data,
               "%s memory data was overwritten",
               msg); /* fallthrough */
  case 1:
    FIO_ASSERT(
        *(dest++) = data, "%s memory data was overwritten (last 8 bytes)", msg);
  }
}

FIO_IFUNC void *FIO_NAME_TEST(stl, mem_tsk)(void *i_) {
  uintptr_t cycles = (uintptr_t)i_;
  const size_t test_block_count = (2UL << FIO_MEMORY_SYS_ALLOCATION_SIZE_LOG);
  uint64_t marker;
  do {
    marker = fio_rand64();
  } while (!marker);

  const size_t limit = (test_block_count / cycles);
  char **ary = (char **)fio_calloc(sizeof(*ary), limit);
  FIO_ASSERT(ary, "allocation failed for test container");
  for (size_t i = 0; i < limit; ++i) {
    ary[i] = (char *)fio_malloc(cycles);
    FIO_ASSERT(ary[i], "allocation failed!");
    {
      char *tmp;
      fio_free((tmp = (char *)fio_malloc(16))); /* add some fragmentation */
      FIO_ASSERT(tmp, "allocation failed!")
    }
    FIO_ASSERT(!ary[i][0], "allocated memory not zero");
    FIO_ASSERT(!ary[i][(cycles - 1)], "allocated memory not zero (end)");
    fio___memset_aligned(ary[i], marker, (cycles));
  }
  for (size_t i = 0; i < limit; ++i) {
    char *tmp = (char *)fio_realloc2(ary[i], (cycles << 1), (cycles));
    FIO_ASSERT(tmp, "re-allocation failed!")
    ary[i] = tmp;
    FIO_ASSERT(!ary[i][(cycles)], "fio_realloc2 copy overflow!");
    fio___memset_test_aligned(ary[i], marker, (cycles), "realloc grow");
    tmp = (char *)fio_realloc2(ary[i], (cycles), (cycles));
    FIO_ASSERT(tmp, "re-allocation (shrinking) failed!")
    ary[i] = tmp;
    fio___memset_test_aligned(ary[i], marker, (cycles), "realloc shrink");
  }
  for (size_t i = 0; i < limit; ++i) {
    fio___memset_test_aligned(ary[i], marker, (cycles), "mem review");
    fio_free(ary[i]);
  }
  fio_free(ary);
  return NULL;
}

FIO_SFUNC void FIO_NAME_TEST(stl, mem)(void) {
  fprintf(stderr, "* Testing core memory allocator (fio_malloc).\n");
  FIO_ASSERT(fio___mem_state, "memory state machine uninitialized");

#if 0 /* help in bug tracking  */
  fprintf(stderr, "* State before allocating 1 unit (16 bytes).\n");
  fio_malloc_print_state();
  void *ptr = fio_malloc(16);
  fprintf(stderr, "* allocated 1 unit (16 bytes) at %p.\n", ptr);
  fprintf(
      stderr, "* Should be from block %p\n", (void *)fio___mem_ptr2block(ptr));
  fio_malloc_print_state();
  fio_free(ptr);
  fprintf(stderr, "* deallocated 1 unit (16 bytes) at %p.\n", ptr);
  fprintf(
      stderr, "* Should be from block %p\n", (void *)fio___mem_ptr2block(ptr));
  fio_malloc_print_state();
  ptr = NULL;
  for (size_t i = 1; i < ((FIO_MEMORY_BLOCK_SIZE >> 4) - 2); ++i) {
    fio_free(ptr);
    ptr = fio_malloc(16);
  }
  fprintf(stderr,
          "* allocated and deallocated a block worth of units (%zu units) "
          "(last: %p).\n",
          (size_t)((FIO_MEMORY_BLOCK_SIZE >> 4) - 1),
          ptr);
  fprintf(
      stderr, "* Should be from block %p\n", (void *)fio___mem_ptr2block(ptr));
  fio_malloc_print_state();
  {
    void *tmp2 = fio_malloc(16);
    fio_free(tmp2);
    fio_free(ptr);
    ptr = tmp2;
  }
  fprintf(stderr,
          "* deallocated block's last unit after 1 unit allocation at %p.\n",
          ptr);
  fprintf(
      stderr, "* Should be from block %p\n", (void *)fio___mem_ptr2block(ptr));
  fio_malloc_print_state();

#endif

  for (uintptr_t cycles = 16; cycles <= (FIO_MEMORY_BIG_BLOCK_ALLOC_LIMIT);
       cycles *= 2) {
    const size_t thread_count = ((fio___mem_state->cores >> 1) | 1);
    pthread_t threads[thread_count];
    fprintf(stderr,
            "* Testing %zu byte allocation blocks with %zu threads.\n",
            (size_t)(cycles),
            (thread_count + 1));
    for (size_t i = 1; i < thread_count; ++i) {
      if (pthread_create(
              threads + i, NULL, FIO_NAME_TEST(stl, mem_tsk), (void *)cycles)) {
        abort();
      }
    }
    FIO_NAME_TEST(stl, mem_tsk)((void *)cycles);
    for (size_t i = 1; i < thread_count; ++i) {
      pthread_join(threads[i], NULL);
    }
  }
#if DEBUG && FIO_EXTERN_COMPLETE
  // fio_malloc_print_state();
  fio___mem_destroy();
  FIO_ASSERT(!fio___mem_chunk_count, "memory leaks?");
#endif /* DEBUG && FIO_EXTERN_COMPLETE */
}
#endif /* FIO_MALLOC_FORCE_SYSTEM */

#endif /* FIO_TEST_CSTL */

/* *****************************************************************************
Memory Allocation - cleanup
***************************************************************************** */
#undef FIO_MEMORY_ON_CHUNK_ALLOC
#undef FIO_MEMORY_ON_CHUNK_FREE
#undef FIO_MEMORY_ON_CHUNK_CACHE
#undef FIO_MEMORY_ON_CHUNK_UNCACHE
#undef FIO_MEMORY_PRINT_STATS
#undef FIO_MEMORY_PRINT_STATS_END
#undef FIO_MEMORY_BIG_BLOCK_MARKER
#undef FIO_MEMORY_USE_PTHREAD_MUTEX
#undef FIO_MEMORY_LOCK_TYPE
#undef FIO_MEMORY_LOCK_TYPE_INIT
#undef FIO_MEMORY_TRYLOCK
#undef FIO_MEMORY_LOCK
#undef FIO_MEMORY_UNLOCK
#undef FIO_MEMORY_USE_FIO_MEMSET
#undef FIO___MEMSET

#endif /* FIO_EXTERN_COMPLETE */
#undef FIO_ALIGN
#undef FIO_ALIGN_NEW
#undef FIO_MALLOC
#endif /* FIO_MALLOC */

/* *****************************************************************************










                          Memory Management MACROs
                    (used internally, for dynamic types)









***************************************************************************** */

/* *****************************************************************************
Memory management macros
***************************************************************************** */

#ifdef FIO_MALLOC_TMP_USE_SYSTEM /* force malloc */
#define FIO_MEM_CALLOC_(size, units) calloc((size), (units))
#define FIO_MEM_REALLOC_(ptr, old_size, new_size, copy_len)                    \
  realloc((ptr), (new_size))
#define FIO_MEM_FREE_(ptr, size) free((ptr))
#define FIO_MEM_INTERNAL_MALLOC_ 0
#else /* FIO_MALLOC_TMP_USE_SYSTEM */
#define FIO_MEM_CALLOC_ FIO_MEM_CALLOC
#define FIO_MEM_REALLOC_ FIO_MEM_REALLOC
#define FIO_MEM_FREE_ FIO_MEM_FREE
#define FIO_MEM_INTERNAL_MALLOC_ FIO_MEM_INTERNAL_MALLOC
#endif /* FIO_MALLOC_TMP_USE_SYSTEM */
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#include "003 atomics.h"            /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************










                                  Time Helpers










***************************************************************************** */
#if defined(FIO_TIME) && !defined(H___FIO_TIME___H)
#define H___FIO_TIME___H

/* *****************************************************************************
Collecting Monotonic / Real Time
***************************************************************************** */

/** Returns human (watch) time... this value isn't as safe for measurements. */
FIO_IFUNC struct timespec fio_time_real();

/** Returns monotonic time. */
FIO_IFUNC struct timespec fio_time_mono();

/** Returns monotonic time in nano-seconds (now in 1 billionth of a second). */
FIO_IFUNC uint64_t fio_time_nano();

/** Returns monotonic time in micro-seconds (now in 1 millionth of a second). */
FIO_IFUNC uint64_t fio_time_micro();

/** Returns monotonic time in milliseconds. */
FIO_IFUNC uint64_t fio_time_milli();

/**
 * A faster (yet less localized) alternative to `gmtime_r`.
 *
 * See the libc `gmtime_r` documentation for details.
 *
 * Falls back to `gmtime_r` for dates before epoch.
 */
SFUNC struct tm fio_time2gm(time_t time);

/** Converts a `struct tm` to time in seconds (assuming UTC). */
SFUNC time_t fio_gm2time(struct tm tm);

/**
 * Writes an RFC 7231 date representation (HTTP date format) to target.
 *
 * Usually requires 29 characters, although this may vary.
 */
SFUNC size_t fio_time2rfc7231(char *target, time_t time);

/**
 * Writes an RFC 2109 date representation to target.
 *
 * Usually requires 31 characters, although this may vary.
 */
SFUNC size_t fio_time2rfc2109(char *target, time_t time);

/**
 * Writes an RFC 2822 date representation to target.
 *
 * Usually requires 28 to 29 characters, although this may vary.
 */
SFUNC size_t fio_time2rfc2822(char *target, time_t time);

/* *****************************************************************************
Patch for OSX version < 10.12 from https://stackoverflow.com/a/9781275/4025095
***************************************************************************** */
#if defined(__MACH__) && !defined(CLOCK_REALTIME)
#include <sys/time.h>
#define CLOCK_REALTIME 0
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 0
#endif
#define clock_gettime fio___patch_clock_gettime
// clock_gettime is not implemented on older versions of OS X (< 10.12).
// If implemented, CLOCK_MONOTONIC will have already been defined.
FIO_IFUNC int fio___patch_clock_gettime(int clk_id, struct timespec *t) {
  struct timeval now;
  int rv = gettimeofday(&now, NULL);
  if (rv)
    return rv;
  t->tv_sec = now.tv_sec;
  t->tv_nsec = now.tv_usec * 1000;
  return 0;
  (void)clk_id;
}
#warning fio_time functions defined using gettimeofday patch.
#endif

/* *****************************************************************************
Time Inline Helpers
***************************************************************************** */

/** Returns human (watch) time... this value isn't as safe for measurements. */
FIO_IFUNC struct timespec fio_time_real() {
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);
  return t;
}

/** Returns monotonic time. */
FIO_IFUNC struct timespec fio_time_mono() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return t;
}

/** Returns monotonic time in nano-seconds (now in 1 micro of a second). */
FIO_IFUNC uint64_t fio_time_nano() {
  struct timespec t = fio_time_mono();
  return ((uint64_t)t.tv_sec * 1000000000) + (uint64_t)t.tv_nsec;
}

/** Returns monotonic time in micro-seconds (now in 1 millionth of a second). */
FIO_IFUNC uint64_t fio_time_micro() {
  struct timespec t = fio_time_mono();
  return ((uint64_t)t.tv_sec * 1000000) + (uint64_t)t.tv_nsec / 1000;
}

/** Returns monotonic time in milliseconds. */
FIO_IFUNC uint64_t fio_time_milli() {
  struct timespec t = fio_time_mono();
  return ((uint64_t)t.tv_sec * 1000) + (uint64_t)t.tv_nsec / 1000000;
}

/* *****************************************************************************
Time Implementation
***************************************************************************** */
#if defined(FIO_EXTERN_COMPLETE)

/**
 * A faster (yet less localized) alternative to `gmtime_r`.
 *
 * See the libc `gmtime_r` documentation for details.
 *
 * Falls back to `gmtime_r` for dates before epoch.
 */
SFUNC struct tm fio_time2gm(time_t timer) {
  struct tm tm;
  ssize_t a, b;
#if HAVE_TM_TM_ZONE || defined(BSD)
  tm = (struct tm){
      .tm_isdst = 0,
      .tm_zone = (char *)"UTC",
  };
#else
  tm = (struct tm){
      .tm_isdst = 0,
  };
#endif

  // convert seconds from epoch to days from epoch + extract data
  if (timer >= 0) {
    // for seconds up to weekdays, we reduce the reminder every step.
    a = (ssize_t)timer;
    b = a / 60; // b == time in minutes
    tm.tm_sec = a - (b * 60);
    a = b / 60; // b == time in hours
    tm.tm_min = b - (a * 60);
    b = a / 24; // b == time in days since epoch
    tm.tm_hour = a - (b * 24);
    // b == number of days since epoch
    // day of epoch was a thursday. Add + 4 so sunday == 0...
    tm.tm_wday = (b + 4) % 7;
  } else {
    // for seconds up to weekdays, we reduce the reminder every step.
    a = (ssize_t)timer;
    b = a / 60; // b == time in minutes
    if (b * 60 != a) {
      /* seconds passed */
      tm.tm_sec = (a - (b * 60)) + 60;
      --b;
    } else {
      /* no seconds */
      tm.tm_sec = 0;
    }
    a = b / 60; // b == time in hours
    if (a * 60 != b) {
      /* minutes passed */
      tm.tm_min = (b - (a * 60)) + 60;
      --a;
    } else {
      /* no minutes */
      tm.tm_min = 0;
    }
    b = a / 24; // b == time in days since epoch?
    if (b * 24 != a) {
      /* hours passed */
      tm.tm_hour = (a - (b * 24)) + 24;
      --b;
    } else {
      /* no hours */
      tm.tm_hour = 0;
    }
    // day of epoch was a thursday. Add + 4 so sunday == 0...
    tm.tm_wday = ((b - 3) % 7);
    if (tm.tm_wday)
      tm.tm_wday += 7;
    /* b == days from epoch */
  }

  // at this point we can apply the algorithm described here:
  // http://howardhinnant.github.io/date_algorithms.html#civil_from_days
  // Credit to Howard Hinnant.
  {
    b += 719468L; // adjust to March 1st, 2000 (post leap of 400 year era)
    // 146,097 = days in era (400 years)
    const size_t era = (b >= 0 ? b : b - 146096) / 146097;
    const uint32_t doe = (b - (era * 146097)); // day of era
    const uint16_t yoe =
        (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365; // year of era
    a = yoe;
    a += era * 400; // a == year number, assuming year starts on March 1st...
    const uint16_t doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    const uint16_t mp = (5U * doy + 2) / 153;
    const uint16_t d = doy - (153U * mp + 2) / 5 + 1;
    const uint8_t m = mp + (mp < 10 ? 2 : -10);
    a += (m <= 1);
    tm.tm_year = a - 1900; // tm_year == years since 1900
    tm.tm_mon = m;
    tm.tm_mday = d;
    const uint8_t is_leap = (a % 4 == 0 && (a % 100 != 0 || a % 400 == 0));
    tm.tm_yday = (doy + (is_leap) + 28 + 31) % (365 + is_leap);
  }

  return tm;
}

/** Converts a `struct tm` to time in seconds (assuming UTC). */
SFUNC time_t fio_gm2time(struct tm tm) {
  time_t time = 0;
  // we start with the algorithm described here:
  // http://howardhinnant.github.io/date_algorithms.html#days_from_civil
  // Credit to Howard Hinnant.
  {
    const int32_t y = (tm.tm_year + 1900) - (tm.tm_mon < 2);
    const int32_t era = (y >= 0 ? y : y - 399) / 400;
    const uint16_t yoe = (y - era * 400L); // 0-399
    const uint32_t doy =
        (153L * (tm.tm_mon + (tm.tm_mon > 1 ? -2 : 10)) + 2) / 5 + tm.tm_mday -
        1;                                                       // 0-365
    const uint32_t doe = yoe * 365L + yoe / 4 - yoe / 100 + doy; // 0-146096
    time = era * 146097L + doe - 719468L; // time == days from epoch
  }

  /* Adjust for hour, minute and second */
  time = time * 24L + tm.tm_hour;
  time = time * 60L + tm.tm_min;
  time = time * 60L + tm.tm_sec;

  if (tm.tm_isdst > 0) {
    time -= 60 * 60;
  }
#if HAVE_TM_TM_ZONE || defined(BSD)
  if (tm.tm_gmtoff) {
    time += tm.tm_gmtoff;
  }
#endif
  return time;
}

static const char *FIO___DAY_NAMES[] =
    {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
// clang-format off
static const char *FIO___MONTH_NAMES[] =
    {"Jan ", "Feb ", "Mar ", "Apr ", "May ", "Jun ",
     "Jul ", "Aug ", "Sep ", "Oct ", "Nov ", "Dec "};
// clang-format on
static const char *FIO___GMT_STR = "GMT";

/** Writes an RFC 7231 date representation (HTTP date format) to target. */
SFUNC size_t fio_time2rfc7231(char *target, time_t time) {
  const struct tm tm = fio_time2gm(time);
  /* note: day of month is always 2 digits */
  char *pos = target;
  uint16_t tmp;
  pos[0] = FIO___DAY_NAMES[tm.tm_wday][0];
  pos[1] = FIO___DAY_NAMES[tm.tm_wday][1];
  pos[2] = FIO___DAY_NAMES[tm.tm_wday][2];
  pos[3] = ',';
  pos[4] = ' ';
  pos += 5;
  tmp = tm.tm_mday / 10;
  pos[0] = '0' + tmp;
  pos[1] = '0' + (tm.tm_mday - (tmp * 10));
  pos += 2;
  *(pos++) = ' ';
  pos[0] = FIO___MONTH_NAMES[tm.tm_mon][0];
  pos[1] = FIO___MONTH_NAMES[tm.tm_mon][1];
  pos[2] = FIO___MONTH_NAMES[tm.tm_mon][2];
  pos[3] = ' ';
  pos += 4;
  // write year.
  pos += fio_ltoa(pos, tm.tm_year + 1900, 10);
  *(pos++) = ' ';
  tmp = tm.tm_hour / 10;
  pos[0] = '0' + tmp;
  pos[1] = '0' + (tm.tm_hour - (tmp * 10));
  pos[2] = ':';
  tmp = tm.tm_min / 10;
  pos[3] = '0' + tmp;
  pos[4] = '0' + (tm.tm_min - (tmp * 10));
  pos[5] = ':';
  tmp = tm.tm_sec / 10;
  pos[6] = '0' + tmp;
  pos[7] = '0' + (tm.tm_sec - (tmp * 10));
  pos += 8;
  pos[0] = ' ';
  pos[1] = FIO___GMT_STR[0];
  pos[2] = FIO___GMT_STR[1];
  pos[3] = FIO___GMT_STR[2];
  pos[4] = 0;
  pos += 4;
  return pos - target;
}
/** Writes an RFC 2109 date representation to target. */
SFUNC size_t fio_time2rfc2109(char *target, time_t time) {
  const struct tm tm = fio_time2gm(time);
  /* note: day of month is always 2 digits */
  char *pos = target;
  uint16_t tmp;
  pos[0] = FIO___DAY_NAMES[tm.tm_wday][0];
  pos[1] = FIO___DAY_NAMES[tm.tm_wday][1];
  pos[2] = FIO___DAY_NAMES[tm.tm_wday][2];
  pos[3] = ',';
  pos[4] = ' ';
  pos += 5;
  tmp = tm.tm_mday / 10;
  pos[0] = '0' + tmp;
  pos[1] = '0' + (tm.tm_mday - (tmp * 10));
  pos += 2;
  *(pos++) = ' ';
  pos[0] = FIO___MONTH_NAMES[tm.tm_mon][0];
  pos[1] = FIO___MONTH_NAMES[tm.tm_mon][1];
  pos[2] = FIO___MONTH_NAMES[tm.tm_mon][2];
  pos[3] = ' ';
  pos += 4;
  // write year.
  pos += fio_ltoa(pos, tm.tm_year + 1900, 10);
  *(pos++) = ' ';
  tmp = tm.tm_hour / 10;
  pos[0] = '0' + tmp;
  pos[1] = '0' + (tm.tm_hour - (tmp * 10));
  pos[2] = ':';
  tmp = tm.tm_min / 10;
  pos[3] = '0' + tmp;
  pos[4] = '0' + (tm.tm_min - (tmp * 10));
  pos[5] = ':';
  tmp = tm.tm_sec / 10;
  pos[6] = '0' + tmp;
  pos[7] = '0' + (tm.tm_sec - (tmp * 10));
  pos += 8;
  *pos++ = ' ';
  *pos++ = '-';
  *pos++ = '0';
  *pos++ = '0';
  *pos++ = '0';
  *pos++ = '0';
  *pos = 0;
  return pos - target;
}

/** Writes an RFC 2822 date representation to target. */
SFUNC size_t fio_time2rfc2822(char *target, time_t time) {
  const struct tm tm = fio_time2gm(time);
  /* note: day of month is either 1 or 2 digits */
  char *pos = target;
  uint16_t tmp;
  pos[0] = FIO___DAY_NAMES[tm.tm_wday][0];
  pos[1] = FIO___DAY_NAMES[tm.tm_wday][1];
  pos[2] = FIO___DAY_NAMES[tm.tm_wday][2];
  pos[3] = ',';
  pos[4] = ' ';
  pos += 5;
  if (tm.tm_mday < 10) {
    *pos = '0' + tm.tm_mday;
    ++pos;
  } else {
    tmp = tm.tm_mday / 10;
    pos[0] = '0' + tmp;
    pos[1] = '0' + (tm.tm_mday - (tmp * 10));
    pos += 2;
  }
  *(pos++) = '-';
  pos[0] = FIO___MONTH_NAMES[tm.tm_mon][0];
  pos[1] = FIO___MONTH_NAMES[tm.tm_mon][1];
  pos[2] = FIO___MONTH_NAMES[tm.tm_mon][2];
  pos += 3;
  *(pos++) = '-';
  // write year.
  pos += fio_ltoa(pos, tm.tm_year + 1900, 10);
  *(pos++) = ' ';
  tmp = tm.tm_hour / 10;
  pos[0] = '0' + tmp;
  pos[1] = '0' + (tm.tm_hour - (tmp * 10));
  pos[2] = ':';
  tmp = tm.tm_min / 10;
  pos[3] = '0' + tmp;
  pos[4] = '0' + (tm.tm_min - (tmp * 10));
  pos[5] = ':';
  tmp = tm.tm_sec / 10;
  pos[6] = '0' + tmp;
  pos[7] = '0' + (tm.tm_sec - (tmp * 10));
  pos += 8;
  pos[0] = ' ';
  pos[1] = FIO___GMT_STR[0];
  pos[2] = FIO___GMT_STR[1];
  pos[3] = FIO___GMT_STR[2];
  pos[4] = 0;
  pos += 4;
  return pos - target;
}

/* *****************************************************************************
Time - test
***************************************************************************** */
#ifdef FIO_TEST_CSTL

#define FIO___GMTIME_TEST_INTERVAL ((60L * 60 * 24) - 7) /* 1day - 7seconds */
#define FIO___GMTIME_TEST_RANGE (4093L * 365) /* test ~4 millenium  */

FIO_SFUNC void FIO_NAME_TEST(stl, time)(void) {
  fprintf(stderr, "* Testing facil.io fio_time2gm vs gmtime_r\n");
  struct tm tm1, tm2;
  const time_t now = fio_time_real().tv_sec;
  const time_t end =
      now + (FIO___GMTIME_TEST_RANGE * FIO___GMTIME_TEST_INTERVAL);
  time_t t = now - (FIO___GMTIME_TEST_RANGE * FIO___GMTIME_TEST_INTERVAL);
  while (t < end) {
    time_t tmp = t;
    t += FIO___GMTIME_TEST_INTERVAL;
    tm2 = fio_time2gm(tmp);
    FIO_ASSERT(fio_gm2time(tm2) == tmp,
               "fio_gm2time roundtrip error (%ld != %ld)",
               (long)fio_gm2time(tm2),
               (long)tmp);
    gmtime_r(&tmp, &tm1);
    if (tm1.tm_year != tm2.tm_year || tm1.tm_mon != tm2.tm_mon ||
        tm1.tm_mday != tm2.tm_mday || tm1.tm_yday != tm2.tm_yday ||
        tm1.tm_hour != tm2.tm_hour || tm1.tm_min != tm2.tm_min ||
        tm1.tm_sec != tm2.tm_sec || tm1.tm_wday != tm2.tm_wday) {
      char buf[256];
      fio_time2rfc7231(buf, tmp);
      FIO_ASSERT(0,
                 "system gmtime_r != fio_time2gm for %ld!\n"
                 "-- System:\n"
                 "\ttm_year: %d\n"
                 "\ttm_mon: %d\n"
                 "\ttm_mday: %d\n"
                 "\ttm_yday: %d\n"
                 "\ttm_hour: %d\n"
                 "\ttm_min: %d\n"
                 "\ttm_sec: %d\n"
                 "\ttm_wday: %d\n"
                 "-- facil.io:\n"
                 "\ttm_year: %d\n"
                 "\ttm_mon: %d\n"
                 "\ttm_mday: %d\n"
                 "\ttm_yday: %d\n"
                 "\ttm_hour: %d\n"
                 "\ttm_min: %d\n"
                 "\ttm_sec: %d\n"
                 "\ttm_wday: %d\n"
                 "-- As String:\n"
                 "\t%s",
                 (long)t,
                 tm1.tm_year,
                 tm1.tm_mon,
                 tm1.tm_mday,
                 tm1.tm_yday,
                 tm1.tm_hour,
                 tm1.tm_min,
                 tm1.tm_sec,
                 tm1.tm_wday,
                 tm2.tm_year,
                 tm2.tm_mon,
                 tm2.tm_mday,
                 tm2.tm_yday,
                 tm2.tm_hour,
                 tm2.tm_min,
                 tm2.tm_sec,
                 tm2.tm_wday,
                 buf);
    }
  }
  {
    uint64_t start, stop;
#if DEBUG
    fprintf(stderr, "PERFOMEANCE TESTS IN DEBUG MODE ARE BIASED\n");
#endif
    start = fio_time_micro();
    for (size_t i = 0; i < (1 << 17); ++i) {
      volatile struct tm tm = fio_time2gm(now);
      __asm__ volatile("" ::: "memory"); /* clobber CPU registers */
      (void)tm;
    }
    stop = fio_time_micro();
    fprintf(stderr,
            "\t- fio_time2gm speed test took:\t%zuus\n",
            (size_t)(stop - start));
    start = fio_time_micro();
    for (size_t i = 0; i < (1 << 17); ++i) {
      volatile struct tm tm;
      time_t tmp = now;
      gmtime_r(&tmp, (struct tm *)&tm);
      __asm__ volatile("" ::: "memory"); /* clobber CPU registers */
    }
    stop = fio_time_micro();
    fprintf(stderr,
            "\t- gmtime_r speed test took:  \t%zuus\n",
            (size_t)(stop - start));
    fprintf(stderr, "\n");
    struct tm tm_now = fio_time2gm(now);
    start = fio_time_micro();
    for (size_t i = 0; i < (1 << 17); ++i) {
      tm_now = fio_time2gm(now + i);
      time_t t_tmp = fio_gm2time(tm_now);
      __asm__ volatile("" ::: "memory"); /* clobber CPU registers */
      (void)t_tmp;
    }
    stop = fio_time_micro();
    fprintf(stderr,
            "\t- fio_gm2time speed test took:\t%zuus\n",
            (size_t)(stop - start));
    start = fio_time_micro();
    for (size_t i = 0; i < (1 << 17); ++i) {
      tm_now = fio_time2gm(now + i);
      volatile time_t t_tmp = mktime((struct tm *)&tm_now);
      __asm__ volatile("" ::: "memory"); /* clobber CPU registers */
      (void)t_tmp;
    }
    stop = fio_time_micro();
    fprintf(stderr,
            "\t- mktime speed test took:    \t%zuus\n",
            (size_t)(stop - start));
    fprintf(stderr, "\n");
  }
}
#undef FIO___GMTIME_TEST_INTERVAL
#undef FIO___GMTIME_TEST_RANGE
#endif /* FIO_TEST_CSTL */

/* *****************************************************************************
Time Cleanup
***************************************************************************** */
#endif /* FIO_EXTERN_COMPLETE */
#undef FIO_TIME
#endif /* FIO_TIME */
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#include "003 atomics.h"            /* Development inclusion - ignore line */
#include "101 time.h"               /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************










                                Task / Timer Queues
                                (Event Loop Engine)










***************************************************************************** */
#if defined(FIO_QUEUE) && !defined(H___FIO_QUEUE___H)
#define H___FIO_QUEUE___H

/* *****************************************************************************
Queue Type(s)
***************************************************************************** */

/* Note: FIO_QUEUE_TASKS_PER_ALLOC can't be more than 65535 */
#ifndef FIO_QUEUE_TASKS_PER_ALLOC
#if UINTPTR_MAX <= 0xFFFFFFFF
/* fits fio_queue_s in one page on most 32 bit machines */
#define FIO_QUEUE_TASKS_PER_ALLOC 338
#else
/* fits fio_queue_s in one page on most 64 bit machines */
#define FIO_QUEUE_TASKS_PER_ALLOC 168
#endif
#endif

/** Task information */
typedef struct {
  /** The function to call */
  void (*fn)(void *, void *);
  /** User opaque data */
  void *udata1;
  /** User opaque data */
  void *udata2;
} fio_queue_task_s;

/* internal use */
typedef struct fio___task_ring_s {
  uint16_t r;   /* reader position */
  uint16_t w;   /* writer position */
  uint16_t dir; /* direction */
  struct fio___task_ring_s *next;
  fio_queue_task_s buf[FIO_QUEUE_TASKS_PER_ALLOC];
} fio___task_ring_s;

/** The queue object - should be considered opaque (or, at least, read only). */
typedef struct {
  fio___task_ring_s *r;
  fio___task_ring_s *w;
  /** the number of tasks waiting to be performed. */
  size_t count;
  fio_lock_i lock;
  fio___task_ring_s mem;
} fio_queue_s;

/* *****************************************************************************
Queue API
***************************************************************************** */

/** Used to initialize a fio_queue_s object. */
#define FIO_QUEUE_INIT(name)                                                   \
  { .r = &(name).mem, .w = &(name).mem, .lock = FIO_LOCK_INIT }

/** Destroys a queue and reinitializes it, after freeing any used resources. */
SFUNC void fio_queue_destroy(fio_queue_s *q);

/** Creates a new queue object (allocated on the heap). */
FIO_IFUNC fio_queue_s *fio_queue_new(void);

/** Frees a queue object after calling fio_queue_destroy. */
SFUNC void fio_queue_free(fio_queue_s *q);

/** Pushes a task to the queue. Returns -1 on error. */
SFUNC int fio_queue_push(fio_queue_s *q, fio_queue_task_s task);

/**
 * Pushes a task to the queue, offering named arguments for the task.
 * Returns -1 on error.
 */
#define fio_queue_push(q, ...)                                                 \
  fio_queue_push((q), (fio_queue_task_s){__VA_ARGS__})

/** Pushes a task to the head of the queue. Returns -1 on error (no memory). */
SFUNC int fio_queue_push_urgent(fio_queue_s *q, fio_queue_task_s task);

/**
 * Pushes a task to the queue, offering named arguments for the task.
 * Returns -1 on error.
 */
#define fio_queue_push_urgent(q, ...)                                          \
  fio_queue_push_urgent((q), (fio_queue_task_s){__VA_ARGS__})

/** Pops a task from the queue (FIFO). Returns a NULL task on error. */
SFUNC fio_queue_task_s fio_queue_pop(fio_queue_s *q);

/** Performs a task from the queue. Returns -1 on error (queue empty). */
SFUNC int fio_queue_perform(fio_queue_s *q);

/** Performs all tasks in the queue. */
SFUNC void fio_queue_perform_all(fio_queue_s *q);

/** returns the number of tasks in the queue. */
FIO_IFUNC size_t fio_queue_count(fio_queue_s *q);

/* *****************************************************************************
Timer Queue Types and API
***************************************************************************** */

typedef struct fio___timer_event_s fio___timer_event_s;

typedef struct {
  fio___timer_event_s *next;
  fio_lock_i lock;
} fio_timer_queue_s;

#define FIO_TIMER_QUEUE_INIT                                                   \
  { .lock = FIO_LOCK_INIT }

typedef struct {
  /** The timer function. If it returns a non-zero value, the timer stops. */
  int (*fn)(void *, void *);
  /** Opaque user data. */
  void *udata1;
  /** Opaque user data. */
  void *udata2;
  /** Called when the timer is done (finished). */
  void (*on_finish)(void *, void *);
  /** Timer interval, in milliseconds. */
  uint32_t every;
  /** The number of times the timer should be performed. -1 == infinity. */
  int32_t repetitions;
  /** Millisecond at which to start. If missing, filled automatically. */
  uint64_t start_at;
} fio_timer_schedule_args_s;

/** Adds a time-bound event to the timer queue. */
SFUNC void fio_timer_schedule(fio_timer_queue_s *timer_queue,
                              fio_timer_schedule_args_s args);

/** A MACRO allowing named arguments to be used. See fio_timer_schedule_args_s.
 */
#define fio_timer_schedule(timer_queue, ...)                                   \
  fio_timer_schedule((timer_queue), (fio_timer_schedule_args_s){__VA_ARGS__})

/** Pushes due events from the timer queue to an event queue. */
SFUNC size_t fio_timer_push2queue(fio_queue_s *queue,
                                  fio_timer_queue_s *timer_queue,
                                  uint64_t now_in_milliseconds);

/*
 * Returns the millisecond at which the next event should occur.
 *
 * If no timer is due (list is empty), returns `(uint64_t)-1`.
 *
 * NOTE: unless manually specified, millisecond timers are relative to
 * `fio_time_milli()`.
 */
FIO_IFUNC uint64_t fio_timer_next_at(fio_timer_queue_s *timer_queue);

/**
 * Clears any waiting timer bound tasks.
 *
 * NOTE:
 *
 * The timer queue must NEVER be freed when there's a chance that timer tasks
 * are waiting to be performed in a `fio_queue_s`.
 *
 * This is due to the fact that the tasks may try to reschedule themselves (if
 * they repeat).
 */
SFUNC void fio_timer_clear(fio_timer_queue_s *timer_queue);

/* *****************************************************************************
Queue Inline Helpers
***************************************************************************** */

/** Creates a new queue object (allocated on the heap). */
FIO_IFUNC fio_queue_s *fio_queue_new(void) {
  fio_queue_s *q = (fio_queue_s *)FIO_MEM_CALLOC_(sizeof(*q), 1);
  if (!q)
    return NULL;
  *q = (fio_queue_s)FIO_QUEUE_INIT(*q);
  return q;
}

/** returns the number of tasks in the queue. */
FIO_IFUNC size_t fio_queue_count(fio_queue_s *q) { return q->count; }

/* *****************************************************************************
Timer Queue Inline Helpers
***************************************************************************** */

struct fio___timer_event_s {
  int (*fn)(void *, void *);
  void *udata1;
  void *udata2;
  void (*on_finish)(void *udata1, void *udata2);
  uint64_t due;
  uint32_t every;
  int32_t repetitions;
  struct fio___timer_event_s *next;
};

/*
 * Returns the millisecond at which the next event should occur.
 *
 * If no timer is due (list is empty), returns `(uint64_t)-1`.
 *
 * NOTE: unless manually specified, millisecond timers are relative to
 * `fio_time_milli()`.
 */
FIO_IFUNC uint64_t fio_timer_next_at(fio_timer_queue_s *tq) {
  uint64_t v = (uint64_t)-1;
  if (!tq)
    goto missing_tq;
  if (!tq || !tq->next)
    return v;
  fio_lock(&tq->lock);
  if (tq->next)
    v = tq->next->due;
  fio_unlock(&tq->lock);
  return v;

missing_tq:
  FIO_LOG_ERROR("`fio_timer_next_at` called with a NULL timer queue!");
  return v;
}

/* *****************************************************************************
Queue Implementation
***************************************************************************** */
#if defined(FIO_EXTERN_COMPLETE)

/** Destroys a queue and reinitializes it, after freeing any used resources. */
SFUNC void fio_queue_destroy(fio_queue_s *q) {
  fio_lock(&q->lock);
  while (q->r) {
    fio___task_ring_s *tmp = q->r;
    q->r = q->r->next;
    if (tmp != &q->mem)
      FIO_MEM_FREE_(tmp, sizeof(*tmp));
  }
  *q = (fio_queue_s)FIO_QUEUE_INIT(*q);
}

/** Frees a queue object after calling fio_queue_destroy. */
SFUNC void fio_queue_free(fio_queue_s *q) {
  fio_queue_destroy(q);
  FIO_MEM_FREE_(q, sizeof(*q));
}

FIO_IFUNC int fio___task_ring_push(fio___task_ring_s *r,
                                   fio_queue_task_s task) {
  if (r->dir && r->r == r->w)
    return -1;
  r->buf[r->w] = task;
  ++r->w;
  if (r->w == FIO_QUEUE_TASKS_PER_ALLOC) {
    r->w = 0;
    r->dir = ~r->dir;
  }
  return 0;
}

FIO_IFUNC int fio___task_ring_unpop(fio___task_ring_s *r,
                                    fio_queue_task_s task) {
  if (r->dir && r->r == r->w)
    return -1;
  if (!r->r) {
    r->r = FIO_QUEUE_TASKS_PER_ALLOC;
    r->dir = ~r->dir;
  }
  --r->r;
  r->buf[r->r] = task;
  return 0;
}

FIO_IFUNC fio_queue_task_s fio___task_ring_pop(fio___task_ring_s *r) {
  fio_queue_task_s t = {.fn = NULL};
  if (!r->dir && r->r == r->w) {
    return t;
  }
  t = r->buf[r->r];
  ++r->r;
  if (r->r == FIO_QUEUE_TASKS_PER_ALLOC) {
    r->r = 0;
    r->dir = ~r->dir;
  }
  return t;
}

int fio_queue_push___(void); /* sublimetext marker */
/** Pushes a task to the queue. Returns -1 on error. */
SFUNC int fio_queue_push FIO_NOOP(fio_queue_s *q, fio_queue_task_s task) {
  if (!task.fn)
    return 0;
  fio_lock(&q->lock);
  if (fio___task_ring_push(q->w, task)) {
    if (q->w != &q->mem && q->mem.next == NULL) {
      q->w->next = &q->mem;
      q->mem.w = q->mem.r = q->mem.dir = 0;
    } else {
      q->w->next = (fio___task_ring_s *)FIO_MEM_CALLOC_(sizeof(*q->w->next), 1);
      if (!q->w->next)
        goto no_mem;
    }
    q->w = q->w->next;
    fio___task_ring_push(q->w, task);
  }
  ++q->count;
  fio_unlock(&q->lock);
  return 0;
no_mem:
  fio_unlock(&q->lock);
  return -1;
}

int fio_queue_push_urgent___(void); /* sublimetext marker */
/** Pushes a task to the head of the queue. Returns -1 on error (no memory). */
SFUNC int fio_queue_push_urgent FIO_NOOP(fio_queue_s *q,
                                         fio_queue_task_s task) {
  if (!task.fn)
    return 0;
  fio_lock(&q->lock);
  if (fio___task_ring_unpop(q->r, task)) {
    /* such a shame... but we must allocate a while task block for one task */
    fio___task_ring_s *tmp =
        (fio___task_ring_s *)FIO_MEM_CALLOC_(sizeof(*q->w->next), 1);
    if (!tmp)
      goto no_mem;
    tmp->next = q->r;
    q->r = tmp;
    tmp->w = 1;
    tmp->dir = tmp->r = 0;
    tmp->buf[0] = task;
  }
  ++q->count;
  fio_unlock(&q->lock);
  return 0;
no_mem:
  fio_unlock(&q->lock);
  return -1;
}

/** Pops a task from the queue (FIFO). Returns a NULL task on error. */
SFUNC fio_queue_task_s fio_queue_pop(fio_queue_s *q) {
  fio_queue_task_s t = {.fn = NULL};
  fio___task_ring_s *to_free = NULL;
  if (!q->count)
    return t;
  fio_lock(&q->lock);
  if (!q->count)
    goto finish;
  if (!(t = fio___task_ring_pop(q->r)).fn) {
    to_free = q->r;
    q->r = to_free->next;
    to_free->next = NULL;
    t = fio___task_ring_pop(q->r);
  }
  if (t.fn && !(--q->count) && q->r != &q->mem) {
    if (to_free && to_free != &q->mem) { // edge case? never happens?
      FIO_MEM_FREE_(to_free, sizeof(*to_free));
    }
    to_free = q->r;
    q->r = q->w = &q->mem;
    q->mem.w = q->mem.r = q->mem.dir = 0;
  }
finish:
  fio_unlock(&q->lock);
  if (to_free && to_free != &q->mem) {
    FIO_MEM_FREE_(to_free, sizeof(*to_free));
  }
  return t;
}

/** Performs a task from the queue. Returns -1 on error (queue empty). */
SFUNC int fio_queue_perform(fio_queue_s *q) {
  fio_queue_task_s t = fio_queue_pop(q);
  if (t.fn) {
    t.fn(t.udata1, t.udata2);
    return 0;
  }
  return -1;
}

/** Performs all tasks in the queue. */
SFUNC void fio_queue_perform_all(fio_queue_s *q) {
  fio_queue_task_s t;
  while ((t = fio_queue_pop(q)).fn)
    t.fn(t.udata1, t.udata2);
}

/* *****************************************************************************
Timer Queue Implementation
***************************************************************************** */

FIO_IFUNC void fio___timer_insert(fio___timer_event_s **pos,
                                  fio___timer_event_s *e) {
  while (*pos && e->due >= (*pos)->due)
    pos = &((*pos)->next);
  e->next = *pos;
  *pos = e;
}

FIO_IFUNC fio___timer_event_s *fio___timer_pop(fio___timer_event_s **pos,
                                               uint64_t due) {
  if (!*pos || (*pos)->due > due)
    return NULL;
  fio___timer_event_s *t = *pos;
  *pos = t->next;
  return t;
}

FIO_IFUNC fio___timer_event_s *
fio___timer_event_new(fio_timer_schedule_args_s args) {
  fio___timer_event_s *t = NULL;
  t = (fio___timer_event_s *)FIO_MEM_CALLOC_(sizeof(*t), 1);
  if (!t)
    goto init_error;
  if (!args.repetitions)
    args.repetitions = 1;
  *t = (fio___timer_event_s){
      .fn = args.fn,
      .udata1 = args.udata1,
      .udata2 = args.udata2,
      .on_finish = args.on_finish,
      .due = args.start_at + args.every,
      .every = args.every,
      .repetitions = args.repetitions,
  };
  return t;
init_error:
  if (args.on_finish)
    args.on_finish(args.udata1, args.udata2);
  return NULL;
}

FIO_IFUNC void fio___timer_event_free(fio_timer_queue_s *tq,
                                      fio___timer_event_s *t) {
  if (tq && (t->repetitions < 0 || fio_atomic_sub_fetch(&t->repetitions, 1))) {
    fio_lock(&tq->lock);
    fio___timer_insert(&tq->next, t);
    fio_unlock(&tq->lock);
    return;
  }
  if (t->on_finish)
    t->on_finish(t->udata1, t->udata2);
  FIO_MEM_FREE_(t, sizeof(*t));
}

SFUNC void fio___timer_perform(void *timer_, void *t_) {
  fio_timer_queue_s *tq = (fio_timer_queue_s *)timer_;
  fio___timer_event_s *t = (fio___timer_event_s *)t_;
  if (t->fn(t->udata1, t->udata2))
    tq = NULL;
  fio___timer_event_free(tq, t);
}

/** Pushes due events from the timer queue to an event queue. */
SFUNC size_t fio_timer_push2queue(fio_queue_s *queue,
                                  fio_timer_queue_s *timer,
                                  uint64_t start_at) {
  size_t r = 0;
  if (!start_at)
    start_at = fio_time_milli();
  if (fio_trylock(&timer->lock))
    return 0;
  fio___timer_event_s *t;
  while ((t = fio___timer_pop(&timer->next, start_at))) {
    fio_queue_push(
        queue, .fn = fio___timer_perform, .udata1 = timer, .udata2 = t);
    ++r;
  }
  fio_unlock(&timer->lock);
  return r;
}

void fio_timer_schedule___(void); /* sublimetext marker */
/** Adds a time-bound event to the timer queue. */
SFUNC void fio_timer_schedule FIO_NOOP(fio_timer_queue_s *timer,
                                       fio_timer_schedule_args_s args) {
  fio___timer_event_s *t = NULL;
  if (!timer || !args.fn || !args.every)
    goto no_timer_queue;
  if (!args.start_at)
    args.start_at = fio_time_milli();
  t = fio___timer_event_new(args);
  if (!t)
    return;
  fio_lock(&timer->lock);
  fio___timer_insert(&timer->next, t);
  fio_unlock(&timer->lock);
  return;
no_timer_queue:
  if (args.on_finish)
    args.on_finish(args.udata1, args.udata2);
  FIO_LOG_ERROR("fio_timer_schedule called with illegal arguments.");
}

/**
 * Clears any waiting timer bound tasks.
 *
 * NOTE:
 *
 * The timer queue must NEVER be freed when there's a chance that timer tasks
 * are waiting to be performed in a `fio_queue_s`.
 *
 * This is due to the fact that the tasks may try to reschedule themselves (if
 * they repeat).
 */
SFUNC void fio_timer_clear(fio_timer_queue_s *tq) {
  fio___timer_event_s *next;
  fio_lock(&tq->lock);
  next = tq->next;
  tq->next = NULL;
  fio_unlock(&tq->lock);
  while (next) {
    fio___timer_event_s *tmp = next;
    next = next->next;
    fio___timer_event_free(NULL, tmp);
  }
}

/* *****************************************************************************
Queue - test
***************************************************************************** */
#ifdef FIO_TEST_CSTL

#ifndef FIO___QUEUE_TEST_PRINT
#define FIO___QUEUE_TEST_PRINT 0
#endif

#include "pthread.h"

#define FIO___QUEUE_TOTAL_COUNT (512 * 1024)

typedef struct {
  fio_queue_s *q;
  uintptr_t count;
} fio___queue_test_s;

FIO_SFUNC void fio___queue_test_sample_task(void *i_count, void *unused2) {
  (void)(unused2);
  fio_atomic_add((uintptr_t *)i_count, 1);
}

FIO_SFUNC void fio___queue_test_sched_sample_task(void *t_, void *i_count) {
  fio___queue_test_s *t = (fio___queue_test_s *)t_;
  for (size_t i = 0; i < t->count; i++) {
    fio_queue_push(t->q, .fn = fio___queue_test_sample_task, .udata1 = i_count);
  }
}

FIO_SFUNC int fio___queue_test_timer_task(void *i_count, void *unused2) {
  fio_atomic_add((uintptr_t *)i_count, 1);
  return (unused2 ? -1 : 0);
}

FIO_SFUNC void FIO_NAME_TEST(stl, queue)(void) {
  fprintf(stderr, "* Testing facil.io task scheduling (fio_queue)\n");
  fio_queue_s *q = fio_queue_new();

  fprintf(stderr, "\t- size of queue object (fio_queue_s): %zu\n", sizeof(*q));
  fprintf(stderr,
          "\t- size of queue ring buffer (per allocation): %zu\n",
          sizeof(q->mem));
  fprintf(stderr,
          "\t- event slots per queue allocation: %zu\n",
          (size_t)FIO_QUEUE_TASKS_PER_ALLOC);

  const size_t max_threads = 12; // assumption / pure conjuncture...
  uintptr_t i_count;
  clock_t start, end;
  i_count = 0;
  start = clock();
  for (size_t i = 0; i < FIO___QUEUE_TOTAL_COUNT; i++) {
    fio___queue_test_sample_task(&i_count, NULL);
  }
  end = clock();
  if (FIO___QUEUE_TEST_PRINT) {
    fprintf(
        stderr,
        "\t- Queueless (direct call) counter: %lu cycles with i_count = %lu\n",
        (unsigned long)(end - start),
        (unsigned long)i_count);
  }
  size_t i_count_should_be = i_count;
  i_count = 0;
  start = clock();
  for (size_t i = 0; i < FIO___QUEUE_TOTAL_COUNT; i++) {
    fio_queue_push(
        q, .fn = fio___queue_test_sample_task, .udata1 = (void *)&i_count);
    fio_queue_perform(q);
  }
  end = clock();
  if (FIO___QUEUE_TEST_PRINT) {
    fprintf(stderr,
            "\t- single task counter: %lu cycles with i_count = %lu\n",
            (unsigned long)(end - start),
            (unsigned long)i_count);
  }
  FIO_ASSERT(i_count == i_count_should_be, "ERROR: queue count invalid\n");

  if (FIO___QUEUE_TEST_PRINT) {
    fprintf(stderr, "\n");
  }

  for (size_t i = 1; i < 32 && FIO___QUEUE_TOTAL_COUNT >> i; ++i) {
    i_count = 0;
    fio___queue_test_s info = {
        .q = q, .count = (uintptr_t)(FIO___QUEUE_TOTAL_COUNT >> i)};
    const size_t tasks = 1 << i;
    start = clock();
    for (size_t j = 0; j < tasks; ++j) {
      fio_queue_push(
          q, fio___queue_test_sched_sample_task, (void *)&info, &i_count);
    }
    FIO_ASSERT(fio_queue_count(q), "tasks not counted?!");
    {
      const size_t t_count = (i % max_threads) + 1;
      union {
        void *(*t)(void *);
        void (*act)(fio_queue_s *);
      } thread_tasks;
      thread_tasks.act = fio_queue_perform_all;
      pthread_t *threads =
          (pthread_t *)FIO_MEM_CALLOC(sizeof(*threads), t_count);
      for (size_t j = 0; j < t_count; ++j) {
        if (pthread_create(threads + j, NULL, thread_tasks.t, q)) {
          abort();
        }
      }
      for (size_t j = 0; j < t_count; ++j) {
        pthread_join(threads[j], NULL);
      }
      FIO_MEM_FREE(threads, sizeof(*threads) * t_count);
    }

    end = clock();
    if (FIO___QUEUE_TEST_PRINT) {
      fprintf(stderr,
              "- queue performed using %zu threads, %zu scheduling loops (%zu "
              "each):\n"
              "    %lu cycles with i_count = %lu\n",
              ((i % max_threads) + 1),
              tasks,
              info.count,
              (unsigned long)(end - start),
              (unsigned long)i_count);
    } else {
      fprintf(stderr, ".");
    }
    FIO_ASSERT(i_count == i_count_should_be, "ERROR: queue count invalid\n");
  }
  if (!(FIO___QUEUE_TEST_PRINT))
    fprintf(stderr, "\n");
  FIO_ASSERT(q->w == &q->mem,
             "queue library didn't release dynamic queue (should be static)");
  fio_queue_free(q);
  {
    fprintf(stderr, "* testing urgent insertion\n");
    fio_queue_s q2 = FIO_QUEUE_INIT(q2);
    for (size_t i = 0; i < (FIO_QUEUE_TASKS_PER_ALLOC * 3); ++i) {
      FIO_ASSERT(!fio_queue_push_urgent(&q2,
                                        .fn = (void (*)(void *, void *))(i + 1),
                                        .udata1 = (void *)(i + 1)),
                 "fio_queue_push_urgent failed");
    }
    FIO_ASSERT(q2.r->next && q2.r->next->next && !q2.r->next->next->next,
               "should have filled only three task blocks");
    for (size_t i = 0; i < (FIO_QUEUE_TASKS_PER_ALLOC * 3); ++i) {
      fio_queue_task_s t = fio_queue_pop(&q2);
      FIO_ASSERT(
          t.fn && (size_t)t.udata1 == (FIO_QUEUE_TASKS_PER_ALLOC * 3) - i,
          "fio_queue_push_urgent pop ordering error [%zu] %zu != %zu (%p)",
          i,
          (size_t)t.udata1,
          (FIO_QUEUE_TASKS_PER_ALLOC * 3) - i,
          (void *)(uintptr_t)t.fn);
    }
    FIO_ASSERT(fio_queue_pop(&q2).fn == NULL,
               "pop overflow after urgent tasks");
    fio_queue_destroy(&q2);
  }
  {
    fprintf(stderr,
            "* Testing facil.io timer scheduling (fio_timer_queue_s)\n");
    fprintf(stderr, "  Note: Errors SHOULD print out to the log.\n");
    fio_queue_s q2 = FIO_QUEUE_INIT(q2);
    uintptr_t tester = 0;
    fio_timer_queue_s tq = FIO_TIMER_QUEUE_INIT;

    /* test failuers */
    fio_timer_schedule(&tq,
                       .udata1 = &tester,
                       .on_finish = fio___queue_test_sample_task,
                       .every = 100,
                       .repetitions = -1);
    FIO_ASSERT(tester == 1,
               "fio_timer_schedule should have called `on_finish`");
    tester = 0;
    fio_timer_schedule(NULL,
                       .fn = fio___queue_test_timer_task,
                       .udata1 = &tester,
                       .on_finish = fio___queue_test_sample_task,
                       .every = 100,
                       .repetitions = -1);
    FIO_ASSERT(tester == 1,
               "fio_timer_schedule should have called `on_finish`");
    tester = 0;
    fio_timer_schedule(&tq,
                       .fn = fio___queue_test_timer_task,
                       .udata1 = &tester,
                       .on_finish = fio___queue_test_sample_task,
                       .every = 0,
                       .repetitions = -1);
    FIO_ASSERT(tester == 1,
               "fio_timer_schedule should have called `on_finish`");

    /* test endless task */
    tester = 0;
    fio_timer_schedule(&tq,
                       .fn = fio___queue_test_timer_task,
                       .udata1 = &tester,
                       .on_finish = fio___queue_test_sample_task,
                       .every = 1,
                       .repetitions = -1,
                       .start_at = fio_time_milli() - 10);
    FIO_ASSERT(tester == 0,
               "fio_timer_schedule should have scheduled the task.");
    for (size_t i = 0; i < 10; ++i) {
      fio_timer_push2queue(&q2, &tq, fio_time_milli());
      FIO_ASSERT(fio_queue_count(&q2) == 1, "task should have been scheduled");
      fio_queue_perform(&q2);
      FIO_ASSERT(!fio_queue_count(&q2), "queue should be empty");
      FIO_ASSERT(tester == i + 1,
                 "task should have been performed (%zu).",
                 (size_t)tester);
    }
    tester = 0;
    fio_timer_clear(&tq);
    FIO_ASSERT(tester == 1, "fio_timer_clear should have called `on_finish`");

    /* test single-use task */
    tester = 0;
    uint64_t milli_now = fio_time_milli();
    fio_timer_schedule(&tq,
                       .fn = fio___queue_test_timer_task,
                       .udata1 = &tester,
                       .on_finish = fio___queue_test_sample_task,
                       .every = 100,
                       .repetitions = 1,
                       .start_at = milli_now - 10);
    FIO_ASSERT(tester == 0,
               "fio_timer_schedule should have scheduled the task.");
    fio_timer_schedule(&tq,
                       .fn = fio___queue_test_timer_task,
                       .udata1 = &tester,
                       .on_finish = fio___queue_test_sample_task,
                       .every = 1,
                       // .repetitions = 1, // auto-value is 1
                       .start_at = milli_now - 10);
    FIO_ASSERT(tester == 0,
               "fio_timer_schedule should have scheduled the task.");
    FIO_ASSERT(fio_timer_next_at(&tq) == milli_now - 9,
               "fio_timer_next_at value error.");
    fio_timer_push2queue(&q2, &tq, milli_now);
    FIO_ASSERT(fio_queue_count(&q2) == 1, "task should have been scheduled");
    FIO_ASSERT(fio_timer_next_at(&tq) == milli_now + 90,
               "fio_timer_next_at value error for unscheduled task.");
    fio_queue_perform(&q2);
    FIO_ASSERT(!fio_queue_count(&q2), "queue should be empty");
    FIO_ASSERT(tester == 2,
               "task should have been performed and on_finish called (%zu).",
               (size_t)tester);
    fio_timer_clear(&tq);
    FIO_ASSERT(
        tester == 3,
        "fio_timer_clear should have called on_finish of future task (%zu).",
        (size_t)tester);
    FIO_ASSERT(!tq.next, "timer queue should be empty.");
    fio_queue_destroy(&q2);
  }
  fprintf(stderr, "* passed.\n");
}
#endif /* FIO_TEST_CSTL */

/* *****************************************************************************
Queue/Timer Cleanup
***************************************************************************** */
#endif /* FIO_EXTERN_COMPLETE */
#undef FIO_QUEUE
#endif /* FIO_QUEUE */
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#include "003 atomics.h"            /* Development inclusion - ignore line */
#include "004 bitwise.h"            /* Development inclusion - ignore line */
#include "005 riskyhash.h"          /* Development inclusion - ignore line */
#include "006 atol.h"               /* Development inclusion - ignore line */
#include "202 hashmap.h"            /* Development inclusion - ignore line */
#define FIO_CLI                     /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************










                  CLI helpers - command line interface parsing









***************************************************************************** */
#if defined(FIO_CLI) && !defined(H___FIO_CLI_H)
#define H___FIO_CLI_H 1

/* *****************************************************************************
Internal Macro Implementation
***************************************************************************** */

/** Used internally. */
#define FIO_CLI_STRING__TYPE_I 0x1
#define FIO_CLI_BOOL__TYPE_I 0x2
#define FIO_CLI_INT__TYPE_I 0x3
#define FIO_CLI_PRINT__TYPE_I 0x4
#define FIO_CLI_PRINT_LINE__TYPE_I 0x5
#define FIO_CLI_PRINT_HEADER__TYPE_I 0x6

/** Indicates the CLI argument should be a String (default). */
#define FIO_CLI_STRING(line) (line), ((char *)FIO_CLI_STRING__TYPE_I)
/** Indicates the CLI argument is a Boolean value. */
#define FIO_CLI_BOOL(line) (line), ((char *)FIO_CLI_BOOL__TYPE_I)
/** Indicates the CLI argument should be an Integer (numerical). */
#define FIO_CLI_INT(line) (line), ((char *)FIO_CLI_INT__TYPE_I)
/** Indicates the CLI string should be printed as is with proper offset. */
#define FIO_CLI_PRINT(line) (line), ((char *)FIO_CLI_PRINT__TYPE_I)
/** Indicates the CLI string should be printed as is with no offset. */
#define FIO_CLI_PRINT_LINE(line) (line), ((char *)FIO_CLI_PRINT_LINE__TYPE_I)
/** Indicates the CLI string should be printed as a header. */
#define FIO_CLI_PRINT_HEADER(line)                                             \
  (line), ((char *)FIO_CLI_PRINT_HEADER__TYPE_I)

/* *****************************************************************************
CLI API
***************************************************************************** */

/**
 * This function parses the Command Line Interface (CLI), creating a temporary
 * "dictionary" that allows easy access to the CLI using their names or aliases.
 *
 * Command line arguments may be typed. If an optional type requirement is
 * provided and the provided arument fails to match the required type, execution
 * will end and an error message will be printed along with a short "help".
 *
 * The function / macro accepts the following arguments:
 * - `argc`: command line argument count.
 * - `argv`: command line argument list (array).
 * - `unnamed_min`: the required minimum of un-named arguments.
 * - `unnamed_max`: the maximum limit of un-named arguments.
 * - `description`: a C string containing the program's description.
 * - named arguments list: a list of C strings describing named arguments.
 *
 * The following optional type requirements are:
 *
 * * FIO_CLI_STRING(desc_line)       - (default) string argument.
 * * FIO_CLI_BOOL(desc_line)         - boolean argument (no value).
 * * FIO_CLI_INT(desc_line)          - integer argument.
 * * FIO_CLI_PRINT_HEADER(desc_line) - extra header for output.
 * * FIO_CLI_PRINT(desc_line)        - extra information for output.
 *
 * Argument names MUST start with the '-' character. The first word starting
 * without the '-' character will begin the description for the CLI argument.
 *
 * The arguments "-?", "-h", "-help" and "--help" are automatically handled
 * unless overridden.
 *
 * Un-named arguments shouldn't be listed in the named arguments list.
 *
 * Example use:
 *
 *    fio_cli_start(argc, argv, 0, 0, "The NAME example accepts the following:",
 *                  FIO_CLI_PRINT_HREADER("Concurrency:"),
 *                  FIO_CLI_INT("-t -thread number of threads to run."),
 *                  FIO_CLI_INT("-w -workers number of workers to run."),
 *                  FIO_CLI_PRINT_HREADER("Address Binding:"),
 *                  "-b, -address the address to bind to.",
 *                  FIO_CLI_INT("-p,-port the port to bind to."),
 *                  FIO_CLI_PRINT("\t\tset port to zero (0) for Unix s."),
 *                  FIO_CLI_PRINT_HREADER("Logging:"),
 *                  FIO_CLI_BOOL("-v -log enable logging."));
 *
 *
 * This would allow access to the named arguments:
 *
 *      fio_cli_get("-b") == fio_cli_get("-address");
 *
 *
 * Once all the data was accessed, free the parsed data dictionary using:
 *
 *      fio_cli_end();
 *
 * It should be noted, arguments will be recognized in a number of forms, i.e.:
 *
 *      app -t=1 -p3000 -a localhost
 *
 * This function is NOT thread safe.
 */
#define fio_cli_start(argc, argv, unnamed_min, unnamed_max, description, ...)  \
  fio_cli_start((argc),                                                        \
                (argv),                                                        \
                (unnamed_min),                                                 \
                (unnamed_max),                                                 \
                (description),                                                 \
                (char const *[]){__VA_ARGS__, (char const *)NULL})
/**
 * Never use the function directly, always use the MACRO, because the macro
 * attaches a NULL marker at the end of the `names` argument collection.
 */
SFUNC void fio_cli_start FIO_NOOP(int argc,
                                  char const *argv[],
                                  int unnamed_min,
                                  int unnamed_max,
                                  char const *description,
                                  char const **names);
/**
 * Clears the memory used by the CLI dictionary, removing all parsed data.
 *
 * This function is NOT thread safe.
 */
SFUNC void fio_cli_end(void);

/** Returns the argument's value as a NUL terminated C String. */
SFUNC char const *fio_cli_get(char const *name);

/** Returns the argument's value as an integer. */
SFUNC int fio_cli_get_i(char const *name);

/** This MACRO returns the argument's value as a boolean. */
#define fio_cli_get_bool(name) (fio_cli_get((name)) != NULL)

/** Returns the number of unnamed argument. */
SFUNC unsigned int fio_cli_unnamed_count(void);

/** Returns the unnamed argument using a 0 based `index`. */
SFUNC char const *fio_cli_unnamed(unsigned int index);

/**
 * Sets the argument's value as a NUL terminated C String (no copy!).
 *
 * CAREFUL: This does not automatically detect aliases or type violations! it
 * will only effect the specific name given, even if invalid. i.e.:
 *
 *     fio_cli_start(argc, argv,
 *                  "this is example accepts the following options:",
 *                  "-p -port the port to bind to", FIO_CLI_INT;
 *
 *     fio_cli_set("-p", "hello"); // fio_cli_get("-p") != fio_cli_get("-port");
 *
 * Note: this does NOT copy the C strings to memory. Memory should be kept alive
 *       until `fio_cli_end` is called.
 *
 * This function is NOT thread safe.
 */
SFUNC void fio_cli_set(char const *name, char const *value);

/**
 * This MACRO is the same as:
 *
 *     if(!fio_cli_get(name)) {
 *       fio_cli_set(name, value)
 *     }
 *
 * See fio_cli_set for notes and restrictions.
 */
#define fio_cli_set_default(name, value)                                       \
  if (!fio_cli_get((name)))                                                    \
    fio_cli_set(name, value);

/* *****************************************************************************
CLI Implementation
***************************************************************************** */
#ifdef FIO_EXTERN_COMPLETE

/* *****************************************************************************
CLI Data Stores
***************************************************************************** */

typedef struct {
  const char *buf;
  size_t len;
} fio___cli_cstr_s;

#define FIO_RISKY_HASH
#define FIO_MAP_TYPE const char *
#define FIO_MAP_KEY fio___cli_cstr_s
#define FIO_MAP_KEY_CMP(o1, o2)                                                \
  (o1.len == o2.len &&                                                         \
   (!o1.len || o1.buf == o2.buf ||                                             \
    (o1.buf && o2.buf && !memcmp(o1.buf, o2.buf, o1.len))))
#define FIO_MAP_NAME fio___cli_hash
#ifndef FIO_STL_KEEP__
#define FIO_STL_KEEP__ 1
#endif
#include __FILE__
#if FIO_STL_KEEP__ == 1
#undef FIO_STL_KEEP__
#endif

static fio___cli_hash_s fio___cli_aliases = FIO_MAP_INIT;
static fio___cli_hash_s fio___cli_values = FIO_MAP_INIT;
static size_t fio___cli_unnamed_count = 0;

typedef struct {
  int unnamed_min;
  int unnamed_max;
  int pos;
  int unnamed_count;
  int argc;
  char const **argv;
  char const *description;
  char const **names;
} fio_cli_parser_data_s;

#define FIO_CLI_HASH_VAL(s)                                                    \
  fio_risky_hash((s).buf, (s).len, (uint64_t)fio_cli_start)

/* *****************************************************************************
Default parameter storage
***************************************************************************** */

static fio___cli_cstr_s fio___cli_default_values;

/** extracts the "default" marker from a string's line */
FIO_SFUNC fio___cli_cstr_s fio___cli_map_line2default(char const *line) {
  fio___cli_cstr_s n = {.buf = line};
  /* skip aliases */
  while (n.buf[n.len] == '-') {
    while (n.buf[n.len] && n.buf[n.len] != ' ' && n.buf[n.len] != ',')
      ++n.len;
    while (n.buf[n.len] && (n.buf[n.len] == ' ' || n.buf[n.len] == ',')) {
      ++n.len;
    }
    n.buf += n.len;
    n.len = 0;
  }
  /* a default is maked with (value) or ("value"), both escapable with '\\' */
  if (n.buf[0] != '(')
    goto no_default;
  ++n.buf;
  if (n.buf[0] == '"') {
    ++n.buf;
    /* seek default value end with `")` */
    while (n.buf[n.len] && !(n.buf[n.len] == '"' && n.buf[n.len + 1] == ')'))
      ++n.len;
    if ((n.buf[n.len] != '"' || n.buf[n.len + 1] != ')'))
      goto no_default;
  } else {
    /* seek default value end with `)` */
    while (n.buf[n.len] && n.buf[n.len] != ')')
      ++n.len;
    if (n.buf[n.len] != ')')
      goto no_default;
  }

  return n;
no_default:
  n.buf = NULL;
  n.len = 0;
  return n;
}

FIO_IFUNC fio___cli_cstr_s fio___cli_map_store_default(fio___cli_cstr_s d) {
  fio___cli_cstr_s val = {.buf = NULL, .len = 0};
  if (!d.len || !d.buf)
    return val;
  {
    void *tmp = FIO_MEM_REALLOC((void *)fio___cli_default_values.buf,
                                fio___cli_default_values.len,
                                fio___cli_default_values.len + d.len + 1,
                                fio___cli_default_values.len);
    if (!tmp)
      return val;
    fio___cli_default_values.buf = (const char *)tmp;
  }
  val.buf = fio___cli_default_values.buf + fio___cli_default_values.len;
  val.len = d.len;
  fio___cli_default_values.len += d.len + 1;

  ((char *)val.buf)[val.len] = 0;
  memcpy((char *)val.buf, d.buf, val.len);
  FIO_LOG_DEBUG("CLI stored a string: %s", val.buf);
  return val;
}

/* *****************************************************************************
CLI Parsing
***************************************************************************** */

FIO_SFUNC void fio___cli_map_line2alias(char const *line) {
  fio___cli_cstr_s n = {.buf = line};
  /* if a line contains a default value, store that value with the aliases. */
  fio___cli_cstr_s def =
      fio___cli_map_store_default(fio___cli_map_line2default(line));
  while (n.buf[0] == '-') {
    while (n.buf[n.len] && n.buf[n.len] != ' ' && n.buf[n.len] != ',') {
      ++n.len;
    }
    const char *old = NULL;
    fio___cli_hash_set(
        &fio___cli_aliases, FIO_CLI_HASH_VAL(n), n, (char const *)line, &old);
    if (def.buf) {
      fio___cli_hash_set(
          &fio___cli_values, FIO_CLI_HASH_VAL(n), n, def.buf, NULL);
    }
#ifdef FIO_LOG_ERROR
    if (old) {
      FIO_LOG_ERROR("CLI argument name conflict detected\n"
                    "         The following two directives conflict:\n"
                    "\t%s\n\t%s\n",
                    old,
                    line);
    }
#endif
    while (n.buf[n.len] && (n.buf[n.len] == ' ' || n.buf[n.len] == ',')) {
      ++n.len;
    }
    n.buf += n.len;
    n.len = 0;
  }
}

FIO_SFUNC char const *fio___cli_get_line_type(fio_cli_parser_data_s *parser,
                                              const char *line) {
  if (!line) {
    return NULL;
  }
  char const **pos = parser->names;
  while (*pos) {
    switch ((intptr_t)*pos) {
    case FIO_CLI_STRING__TYPE_I:       /* fallthrough */
    case FIO_CLI_BOOL__TYPE_I:         /* fallthrough */
    case FIO_CLI_INT__TYPE_I:          /* fallthrough */
    case FIO_CLI_PRINT__TYPE_I:        /* fallthrough */
    case FIO_CLI_PRINT_LINE__TYPE_I:   /* fallthrough */
    case FIO_CLI_PRINT_HEADER__TYPE_I: /* fallthrough */
      ++pos;
      continue;
    }
    if (line == *pos) {
      goto found;
    }
    ++pos;
  }
  return NULL;
found:
  switch ((size_t)pos[1]) {
  case FIO_CLI_STRING__TYPE_I:       /* fallthrough */
  case FIO_CLI_BOOL__TYPE_I:         /* fallthrough */
  case FIO_CLI_INT__TYPE_I:          /* fallthrough */
  case FIO_CLI_PRINT__TYPE_I:        /* fallthrough */
  case FIO_CLI_PRINT_LINE__TYPE_I:   /* fallthrough */
  case FIO_CLI_PRINT_HEADER__TYPE_I: /* fallthrough */
    return pos[1];
  }
  return NULL;
}

FIO_SFUNC void fio___cli_print_line(char const *desc, char const *name) {
  char buf[1024];
  size_t pos = 0;
  while (name[0] == '.' || name[0] == '/')
    ++name;
  while (*desc) {
    if (desc[0] == 'N' && desc[1] == 'A' && desc[2] == 'M' && desc[3] == 'E') {
      buf[pos++] = 0;
      desc += 4;
      fprintf(stderr, "%s%s", buf, name);
      pos = 0;
    } else {
      buf[pos++] = *desc;
      ++desc;
      if (pos >= 980) {
        buf[pos++] = 0;
        fwrite(buf, pos, sizeof(*buf), stderr);
        pos = 0;
      }
    }
  }
  if (pos)
    fwrite(buf, pos, sizeof(*buf), stderr);
}

FIO_SFUNC void fio___cli_set_arg(fio___cli_cstr_s arg,
                                 char const *value,
                                 char const *line,
                                 fio_cli_parser_data_s *parser) {
  char const *type = NULL;
  /* handle unnamed argument */
  if (!line || !arg.len) {
    if (!value) {
      goto print_help;
    }
    if (!strcmp(value, "-?") || !strcasecmp(value, "-h") ||
        !strcasecmp(value, "-help") || !strcasecmp(value, "--help")) {
      goto print_help;
    }
    fio___cli_cstr_s n = {.len = (size_t)++parser->unnamed_count};
    fio___cli_hash_set(&fio___cli_values, n.len, n, value, NULL);
    if (parser->unnamed_max >= 0 &&
        parser->unnamed_count > parser->unnamed_max) {
      arg.len = 0;
      goto error;
    }
    FIO_LOG_DEBUG2("(CLI) set an unnamed argument: %s", value);
    FIO_ASSERT_DEBUG(fio___cli_hash_get(&fio___cli_values, n.len, n) == value,
                     "(CLI) set argument failed!");
    return;
  }

  /* validate data types */
  type = fio___cli_get_line_type(parser, line);
  switch ((size_t)type) {
  case FIO_CLI_BOOL__TYPE_I:
    if (value && value != parser->argv[parser->pos + 1]) {
      goto error;
    }
    value = "1";
    break;
  case FIO_CLI_INT__TYPE_I:
    if (value) {
      char const *tmp = value;
      fio_atol((char **)&tmp);
      if (*tmp) {
        goto error;
      }
    }
    /* fallthrough */
  case FIO_CLI_STRING__TYPE_I:
    if (!value)
      goto error;
    if (!value[0])
      goto finish;
    break;
  }

  /* add values using all aliases possible */
  {
    fio___cli_cstr_s n = {.buf = line};
    while (n.buf[0] == '-') {
      while (n.buf[n.len] && n.buf[n.len] != ' ' && n.buf[n.len] != ',') {
        ++n.len;
      }
      fio___cli_hash_set(
          &fio___cli_values, FIO_CLI_HASH_VAL(n), n, value, NULL);
      FIO_LOG_DEBUG2("(CLI) set argument %.*s = %s", (int)n.len, n.buf, value);
      FIO_ASSERT_DEBUG(fio___cli_hash_get(
                           &fio___cli_values, FIO_CLI_HASH_VAL(n), n) == value,
                       "(CLI) set argument failed!");
      while (n.buf[n.len] && (n.buf[n.len] == ' ' || n.buf[n.len] == ',')) {
        ++n.len;
      }
      n.buf += n.len;
      n.len = 0;
    }
  }

finish:

  /* handle additional argv progress (if value is on separate argv) */
  if (value && parser->pos < parser->argc &&
      value == parser->argv[parser->pos + 1])
    ++parser->pos;
  return;

error: /* handle errors*/
  FIO_LOG_DEBUG2("(CLI) error detected, printing help and exiting.");
  fprintf(stderr,
          "\n\r\x1B[31mError:\x1B[0m invalid argument %.*s %s %s\n\n",
          (int)arg.len,
          arg.buf,
          arg.len ? "with value" : "",
          value ? (value[0] ? value : "(empty)") : "(null)");
print_help:
  if (parser->description) {
    fprintf(stderr, "\n");
    fio___cli_print_line(parser->description, parser->argv[0]);
    fprintf(stderr, "\n");
  } else {
    const char *name_tmp = parser->argv[0];
    while (name_tmp[0] == '.' || name_tmp[0] == '/')
      ++name_tmp;
    fprintf(stderr,
            "\nAvailable command-line options for \x1B[1m%s\x1B[0m:\n",
            name_tmp);
  }
  /* print out each line's arguments */
  char const **pos = parser->names;
  while (*pos) {
    switch ((intptr_t)*pos) {
    case FIO_CLI_STRING__TYPE_I:     /* fallthrough */
    case FIO_CLI_BOOL__TYPE_I:       /* fallthrough */
    case FIO_CLI_INT__TYPE_I:        /* fallthrough */
    case FIO_CLI_PRINT__TYPE_I:      /* fallthrough */
    case FIO_CLI_PRINT_LINE__TYPE_I: /* fallthrough */
    case FIO_CLI_PRINT_HEADER__TYPE_I:
      ++pos;
      continue;
    }
    type = (char *)FIO_CLI_STRING__TYPE_I;
    switch ((intptr_t)pos[1]) {
    case FIO_CLI_PRINT__TYPE_I:
      fprintf(stderr, "          \t");
      fio___cli_print_line(pos[0], parser->argv[0]);
      fprintf(stderr, "\n");
      pos += 2;
      continue;
    case FIO_CLI_PRINT_LINE__TYPE_I:
      fio___cli_print_line(pos[0], parser->argv[0]);
      fprintf(stderr, "\n");
      pos += 2;
      continue;
    case FIO_CLI_PRINT_HEADER__TYPE_I:
      fprintf(stderr, "\n\x1B[4m");
      fio___cli_print_line(pos[0], parser->argv[0]);
      fprintf(stderr, "\x1B[0m\n");
      pos += 2;
      continue;

    case FIO_CLI_STRING__TYPE_I: /* fallthrough */
    case FIO_CLI_BOOL__TYPE_I:   /* fallthrough */
    case FIO_CLI_INT__TYPE_I:    /* fallthrough */
      type = pos[1];
    }
    /* print line @ pos, starting with main argument name */
    int alias_count = 0;
    int first_len = 0;
    size_t tmp = 0;
    char const *const p = *pos;
    fio___cli_cstr_s def = fio___cli_map_line2default(p);
    while (p[tmp] == '-') {
      while (p[tmp] && p[tmp] != ' ' && p[tmp] != ',') {
        if (!alias_count)
          ++first_len;
        ++tmp;
      }
      ++alias_count;
      while (p[tmp] && (p[tmp] == ' ' || p[tmp] == ',')) {
        ++tmp;
      }
    }
    if (def.len) {
      tmp = (size_t)((def.buf + def.len + 1) - p);
      tmp += (p[tmp] == ')'); /* in case of `")` */
      while (p[tmp] && (p[tmp] == ' ' || p[tmp] == ',')) {
        ++tmp;
      }
    }
    switch ((size_t)type) {
    case FIO_CLI_STRING__TYPE_I:
      fprintf(stderr,
              " \x1B[1m%-10.*s\x1B[0m\x1B[2m\t\"\" \x1B[0m%s\n",
              first_len,
              p,
              p + tmp);
      break;
    case FIO_CLI_BOOL__TYPE_I:
      fprintf(stderr, " \x1B[1m%-10.*s\x1B[0m\t   %s\n", first_len, p, p + tmp);
      break;
    case FIO_CLI_INT__TYPE_I:
      fprintf(stderr,
              " \x1B[1m%-10.*s\x1B[0m\x1B[2m\t## \x1B[0m%s\n",
              first_len,
              p,
              p + tmp);
      break;
    }
    /* print aliase information */
    tmp = first_len;
    while (p[tmp] && (p[tmp] == ' ' || p[tmp] == ',')) {
      ++tmp;
    }
    while (p[tmp] == '-') {
      const size_t start = tmp;
      while (p[tmp] && p[tmp] != ' ' && p[tmp] != ',') {
        ++tmp;
      }
      int padding = first_len - (tmp - start);
      if (padding < 0)
        padding = 0;
      switch ((size_t)type) {
      case FIO_CLI_STRING__TYPE_I:
        fprintf(stderr,
                " \x1B[1m%-10.*s\x1B[0m\x1B[2m\t\"\" \x1B[0m%.*s\x1B[2msame as "
                "%.*s\x1B[0m\n",
                (int)(tmp - start),
                p + start,
                padding,
                "",
                first_len,
                p);
        break;
      case FIO_CLI_BOOL__TYPE_I:
        fprintf(stderr,
                " \x1B[1m%-10.*s\x1B[0m\t   %.*s\x1B[2msame as %.*s\x1B[0m\n",
                (int)(tmp - start),
                p + start,
                padding,
                "",
                first_len,
                p);
        break;
      case FIO_CLI_INT__TYPE_I:
        fprintf(stderr,
                " \x1B[1m%-10.*s\x1B[0m\x1B[2m\t## \x1B[0m%.*s\x1B[2msame as "
                "%.*s\x1B[0m\n",
                (int)(tmp - start),
                p + start,
                padding,
                "",
                first_len,
                p);
        break;
      }
    }
    /* print default information */
    if (def.len)
      fprintf(stderr,
              "           \t\x1B[2mdefault value: %.*s\x1B[0m\n",
              (int)def.len,
              def.buf);
    ++pos;
  }
  fprintf(stderr,
          "\nUse any of the following input formats:\n"
          "\t-arg <value>\t-arg=<value>\t-arg<value>\n"
          "\n"
          "Use \x1B[1m-h\x1B[0m , \x1B[1m-help\x1B[0m or "
          "\x1B[1m-?\x1B[0m "
          "to get this information again.\n"
          "\n");
  fio_cli_end();
  exit(0);
}

/* *****************************************************************************
CLI Initialization
***************************************************************************** */

void fio_cli_start___(void); /* sublime text marker */
SFUNC void fio_cli_start FIO_NOOP(int argc,
                                  char const *argv[],
                                  int unnamed_min,
                                  int unnamed_max,
                                  char const *description,
                                  char const **names) {
  if (unnamed_max >= 0 && unnamed_max < unnamed_min)
    unnamed_max = unnamed_min;
  fio_cli_parser_data_s parser = {
      .unnamed_min = unnamed_min,
      .unnamed_max = unnamed_max,
      .pos = 0,
      .argc = argc,
      .argv = argv,
      .description = description,
      .names = names,
  };

  if (fio___cli_hash_count(&fio___cli_values)) {
    fio_cli_end();
  }

  /* prepare aliases hash map */

  char const **line = names;
  while (*line) {
    switch ((intptr_t)*line) {
    case FIO_CLI_STRING__TYPE_I:       /* fallthrough */
    case FIO_CLI_BOOL__TYPE_I:         /* fallthrough */
    case FIO_CLI_INT__TYPE_I:          /* fallthrough */
    case FIO_CLI_PRINT__TYPE_I:        /* fallthrough */
    case FIO_CLI_PRINT_LINE__TYPE_I:   /* fallthrough */
    case FIO_CLI_PRINT_HEADER__TYPE_I: /* fallthrough */
      ++line;
      continue;
    }
    if (line[1] != (char *)FIO_CLI_PRINT__TYPE_I &&
        line[1] != (char *)FIO_CLI_PRINT_LINE__TYPE_I &&
        line[1] != (char *)FIO_CLI_PRINT_HEADER__TYPE_I)
      fio___cli_map_line2alias(*line);
    ++line;
  }

  /* parse existing arguments */

  while ((++parser.pos) < argc) {
    char const *value = NULL;
    fio___cli_cstr_s n = {.buf = argv[parser.pos],
                          .len = strlen(argv[parser.pos])};
    if (parser.pos + 1 < argc) {
      value = argv[parser.pos + 1];
    }
    const char *l = NULL;
    while (n.len && !(l = fio___cli_hash_get(
                          &fio___cli_aliases, FIO_CLI_HASH_VAL(n), n))) {
      --n.len;
      value = n.buf + n.len;
    }
    if (n.len && value && value[0] == '=') {
      ++value;
    }
    // fprintf(stderr, "Setting %.*s to %s\n", (int)n.len, n.buf, value);
    fio___cli_set_arg(n, value, l, &parser);
  }

  /* Cleanup and save state for API */
  fio___cli_hash_destroy(&fio___cli_aliases);
  fio___cli_unnamed_count = parser.unnamed_count;
  /* test for required unnamed arguments */
  if (parser.unnamed_count < parser.unnamed_min)
    fio___cli_set_arg((fio___cli_cstr_s){.len = 0}, NULL, NULL, &parser);
}

/* *****************************************************************************
CLI Destruction
***************************************************************************** */

SFUNC void __attribute__((destructor)) fio_cli_end(void) {
  fio___cli_hash_destroy(&fio___cli_values);
  fio___cli_hash_destroy(&fio___cli_aliases);
  fio___cli_unnamed_count = 0;
  if (fio___cli_default_values.buf) {
    FIO_MEM_FREE((void *)fio___cli_default_values.buf,
                 fio___cli_default_values.len);
    fio___cli_default_values.buf = NULL;
    fio___cli_default_values.len = 0;
  }
}
/* *****************************************************************************
CLI Data Access API
***************************************************************************** */

/** Returns the argument's value as a NUL terminated C String. */
SFUNC char const *fio_cli_get(char const *name) {
  fio___cli_cstr_s n = {.buf = name, .len = strlen(name)};
  if (!fio___cli_hash_count(&fio___cli_values)) {
    return NULL;
  }
  char const *val =
      fio___cli_hash_get(&fio___cli_values, FIO_CLI_HASH_VAL(n), n);
  return val;
}

/** Returns the argument's value as an integer. */
SFUNC int fio_cli_get_i(char const *name) {
  char const *val = fio_cli_get(name);
  return fio_atol((char **)&val);
}

/** Returns the number of unrecognized argument. */
SFUNC unsigned int fio_cli_unnamed_count(void) {
  return (unsigned int)fio___cli_unnamed_count;
}

/** Returns the unrecognized argument using a 0 based `index`. */
SFUNC char const *fio_cli_unnamed(unsigned int index) {
  if (!fio___cli_hash_count(&fio___cli_values) || !fio___cli_unnamed_count) {
    return NULL;
  }
  fio___cli_cstr_s n = {.buf = NULL, .len = index + 1};
  return fio___cli_hash_get(&fio___cli_values, index + 1, n);
}

/**
 * Sets the argument's value as a NUL terminated C String (no copy!).
 *
 * Note: this does NOT copy the C strings to memory. Memory should be kept
 * alive until `fio_cli_end` is called.
 */
SFUNC void fio_cli_set(char const *name, char const *value) {
  fio___cli_cstr_s n = (fio___cli_cstr_s){.buf = name, .len = strlen(name)};
  fio___cli_hash_set(&fio___cli_values, FIO_CLI_HASH_VAL(n), n, value, NULL);
}

/* *****************************************************************************
CLI - test
***************************************************************************** */
#ifdef FIO_TEST_CSTL
FIO_SFUNC void FIO_NAME_TEST(stl, cli)(void) {
  const char *argv[] = {
      "appname",
      "-i11",
      "-i2=2",
      "-i3",
      "3",
      "-t",
      "-s",
      "test",
      "unnamed",
  };
  const int argc = sizeof(argv) / sizeof(argv[0]);
  fprintf(stderr, "* Testing CLI helpers.\n");
  { /* avoid macro for C++ */
    const char *arguments[] = {
        FIO_CLI_INT("-integer1 -i1 first integer"),
        FIO_CLI_INT("-integer2 -i2 second integer"),
        FIO_CLI_INT("-integer3 -i3 third integer"),
        FIO_CLI_INT("-integer4 -i4 (4) fourth integer"),
        FIO_CLI_INT("-integer5 -i5 (\"5\") fifth integer"),
        FIO_CLI_BOOL("-boolean -t boolean"),
        FIO_CLI_BOOL("-boolean_false -f boolean"),
        FIO_CLI_STRING("-str -s a string"),
        FIO_CLI_PRINT_HEADER("Printing stuff"),
        FIO_CLI_PRINT_LINE("does nothing, but shouldn't crash either"),
        FIO_CLI_PRINT("does nothing, but shouldn't crash either"),
        NULL,
    };
    fio_cli_start FIO_NOOP(argc, argv, 0, -1, NULL, arguments);
  }
  FIO_ASSERT(fio_cli_get_i("-i2") == 2, "CLI second integer error.");
  FIO_ASSERT(fio_cli_get_i("-i3") == 3, "CLI third integer error.");
  FIO_ASSERT(fio_cli_get_i("-i4") == 4,
             "CLI fourth integer error (%s).",
             fio_cli_get("-i4"));
  FIO_ASSERT(fio_cli_get_i("-i5") == 5,
             "CLI fifth integer error (%s).",
             fio_cli_get("-i5"));
  FIO_ASSERT(fio_cli_get_i("-i1") == 1, "CLI first integer error.");
  FIO_ASSERT(fio_cli_get_i("-i2") == fio_cli_get_i("-integer2"),
             "CLI second integer error.");
  FIO_ASSERT(fio_cli_get_i("-i3") == fio_cli_get_i("-integer3"),
             "CLI third integer error.");
  FIO_ASSERT(fio_cli_get_i("-i1") == fio_cli_get_i("-integer1"),
             "CLI first integer error.");
  FIO_ASSERT(fio_cli_get_i("-t") == 1, "CLI boolean true error.");
  FIO_ASSERT(fio_cli_get_i("-f") == 0, "CLI boolean false error.");
  FIO_ASSERT(!strcmp(fio_cli_get("-s"), "test"), "CLI string error.");
  FIO_ASSERT(fio_cli_unnamed_count() == 1, "CLI unnamed count error.");
  FIO_ASSERT(!strcmp(fio_cli_unnamed(0), "unnamed"), "CLI unnamed error.");
  fio_cli_set("-manual", "okay");
  FIO_ASSERT(!strcmp(fio_cli_get("-manual"), "okay"), "CLI set/get error.");
  fio_cli_end();
  FIO_ASSERT(fio_cli_get_i("-i1") == 0, "CLI cleanup error.");
}
#endif /* FIO_TEST_CSTL */

/* *****************************************************************************
CLI - cleanup
***************************************************************************** */
#endif /* FIO_EXTERN_COMPLETE*/
#endif /* FIO_CLI */
#undef FIO_CLI
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************










                        Basic Socket Helpers / IO Polling








Example:
********************************************************************************

#define FIO_SOCK
#define FIO_CLI
#define FIO_LOG
#include "fio-stl.h" // __FILE__

typedef struct {
  int fd;
  unsigned char is_client;
} state_s;

static void on_data_server(int fd, size_t index, void *udata) {
  (void)udata; // unused for server
  (void)index; // we don't use the array index in this example
  char buf[65536];
  memcpy(buf, "echo: ", 6);
  ssize_t len = 0;
  struct sockaddr_storage peer;
  socklen_t peer_addrlen = sizeof(peer);
  len = recvfrom(fd, buf + 6, (65536 - 7), 0, (struct sockaddr *)&peer,
                 &peer_addrlen);
  if (len <= 0)
    return;
  buf[len + 6] = 0;
  fprintf(stderr, "Recieved: %s", buf + 6);
  // sends all data in UDP, with TCP sending may be partial
  len =
      sendto(fd, buf, len + 6, 0, (const struct sockaddr *)&peer, peer_addrlen);
  if (len < 0)
    perror("error");
}

static void on_data_client(int fd, size_t index, void *udata) {
  state_s *state = (state_s *)udata;
  fprintf(stderr, "on_data_client %zu\n", index);
  if (!index) // stdio is index 0 in the fd list
    goto is_stdin;
  char buf[65536];
  ssize_t len = 0;
  struct sockaddr_storage peer;
  socklen_t peer_addrlen = sizeof(peer);
  len = recvfrom(fd, buf, 65535, 0, (struct sockaddr *)&peer, &peer_addrlen);
  if (len <= 0)
    return;
  buf[len] = 0;
  fprintf(stderr, "%s", buf);
  return;
is_stdin:
  len = read(fd, buf, 65535);
  if (len <= 0)
    return;
  buf[len] = 0;
  // sends all data in UDP, with TCP sending may be partial
  len = send(state->fd, buf, len, 0);
  fprintf(stderr, "Sent: %zd bytes\n", len);
  if (len < 0)
    perror("error");
  return;
  (void)udata;
}

int main(int argc, char const *argv[]) {
  // Using CLI to set address, port and client/server mode.
  fio_cli_start(
      argc, argv, 0, 0, "UDP echo server / client example.",
      FIO_CLI_PRINT_HEADER("Address Binding"),
      FIO_CLI_STRING("-address -b address to listen / connect to."),
      FIO_CLI_INT("-port -p port to listen / connect to. Defaults to 3030."),
      FIO_CLI_PRINT_HEADER("Operation Mode"),
      FIO_CLI_BOOL("-client -c Client mode."),
      FIO_CLI_BOOL("-verbose -v verbose mode (debug messages on)."));

  if (fio_cli_get_bool("-v"))
    FIO_LOG_LEVEL = FIO_LOG_LEVEL_DEBUG;
  fio_cli_set_default("-p", "3030");

  // Using FIO_SOCK functions for setting up UDP server / client
  state_s state = {.is_client = fio_cli_get_bool("-c")};
  state.fd = fio_sock_open(
      fio_cli_get("-b"), fio_cli_get("-p"),
      FIO_SOCK_UDP | FIO_SOCK_NONBLOCK |
          (fio_cli_get_bool("-c") ? FIO_SOCK_CLIENT : FIO_SOCK_SERVER));

  if (state.fd == -1) {
    FIO_LOG_FATAL("Couldn't open socket!");
    exit(1);
  }
  FIO_LOG_DEBUG("UDP socket open on fd %d", state.fd);

  if (state.is_client) {
    int i =
        send(state.fd, "Client hello... further data will be sent using REPL\n",
             53, 0);
    fprintf(stderr, "Sent: %d bytes\n", i);
    if (i < 0)
      perror("error");
    while (fio_sock_poll(.on_data = on_data_client, .udata = (void *)&state,
                         .timeout = 1000,
                         .fds = FIO_SOCK_POLL_LIST(
                             FIO_SOCK_POLL_R(fileno(stdin)),
                             FIO_SOCK_POLL_R(state.fd))) >= 0)
      ;
  } else {
    while (fio_sock_poll(.on_data = on_data_server, .udata = (void *)&state,
                         .timeout = 1000,
                         .fds = FIO_SOCK_POLL_LIST(
                             FIO_SOCK_POLL_R(state.fd))) >= 0)
      ;
  }
  // we should cleanup, though we'll exit with Ctrl+C, so it's won't matter.
  fio_cli_end();
  close(state.fd);
  return 0;
  (void)argv;
}


***************************************************************************** */
#if defined(FIO_SOCK) && FIO_HAVE_UNIX_TOOLS && !defined(FIO_SOCK_POLL_LIST)
#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

/* *****************************************************************************
IO Poll - API
***************************************************************************** */
#define FIO_SOCK_POLL_RW(fd_)                                                  \
  (struct pollfd) { .fd = fd_, .events = (POLLIN | POLLOUT) }
#define FIO_SOCK_POLL_R(fd_)                                                   \
  (struct pollfd) { .fd = fd_, .events = POLLIN }
#define FIO_SOCK_POLL_W(fd_)                                                   \
  (struct pollfd) { .fd = fd_, .events = POLLOUT }
#define FIO_SOCK_POLL_LIST(...)                                                \
  (struct pollfd[]) {                                                          \
    __VA_ARGS__, (struct pollfd) { .fd = -1 }                                  \
  }

typedef struct {
  /** Called after polling but before any events are processed. */
  void (*before_events)(void *udata);
  /** Called when the fd can be written too (available outgoing buffer). */
  void (*on_ready)(int fd, size_t index, void *udata);
  /** Called when data iis available to be read from the fd. */
  void (*on_data)(int fd, size_t index, void *udata);
  /** Called on error or when the fd was closed. */
  void (*on_error)(int fd, size_t index, void *udata);
  /** Called after polling and after all events are processed. */
  void (*after_events)(void *udata);
  /** An opaque user data pointer. */
  void *udata;
  /** A pointer to the fd pollin array. */
  struct pollfd *fds;
  /**
   * the number of fds to listen to.
   *
   * If zero, and `fds` is set, it will be auto-calculated trying to find the
   * first array member where `events == 0`. Make sure to supply this end
   * marker, of the buffer may overrun!
   */
  uint32_t count;
  /** timeout for the polling system call. */
  int timeout;
} fio_sock_poll_args;

/**
 * The `fio_sock_poll` function uses the `poll` system call to poll a simple IO
 * list.
 *
 * The list must end with a `struct pollfd` with it's `events` set to zero. No
 * other member of the list should have their `events` data set to zero.
 *
 * It is recommended to use the `FIO_SOCK_POLL_LIST(...)` and
 * `FIO_SOCK_POLL_[RW](fd)` macros. i.e.:
 *
 *     int count = fio_sock_poll(.on_ready = on_ready,
 *                         .on_data = on_data,
 *                         .fds = FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_RW(io_fd)));
 *
 * NOTE: The `poll` system call should perform reasonably well for light loads
 * (short lists). However, for complex IO needs or heavier loads, use the
 * system's native IO API, such as kqueue or epoll.
 */
FIO_IFUNC int fio_sock_poll(fio_sock_poll_args args);
#define fio_sock_poll(...) fio_sock_poll((fio_sock_poll_args){__VA_ARGS__})

typedef enum {
  FIO_SOCK_SERVER = 0,
  FIO_SOCK_CLIENT = 1,
  FIO_SOCK_NONBLOCK = 2,
  FIO_SOCK_TCP = 4,
  FIO_SOCK_UDP = 8,
  FIO_SOCK_UNIX = 16,
} fio_sock_open_flags_e;

/** A helper macro that waits on a single IO with no callbacks (0 = no event) */
#define FIO_SOCK_WAIT_RW(fd, timeout_)                                         \
  fio_sock_poll(.timeout = (timeout_),                                         \
                .count = 1,                                                    \
                FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_RW((fd))))

/** A helper macro that waits on a single IO with no callbacks (0 = no event) */
#define FIO_SOCK_WAIT_R(fd, timeout_)                                          \
  fio_sock_poll(.timeout = (timeout_),                                         \
                .count = 1,                                                    \
                FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_R((fd))))

/** A helper macro that waits on a single IO with no callbacks (0 = no event) */
#define FIO_SOCK_WAIT_W(fd, timeout_)                                          \
  fio_sock_poll(.timeout = (timeout_),                                         \
                .count = 1,                                                    \
                FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_W((fd))))

/**
 * Creates a new socket according to the provided flags.
 *
 * The `port` string will be ignored when `FIO_SOCK_UNIX` is set.
 */
FIO_IFUNC int fio_sock_open(const char *restrict address,
                            const char *restrict port,
                            uint16_t flags);

/**
 * Attempts to resolve an address to a valid IP6 / IP4 address pointer.
 *
 * The `sock_type` element should be a socket type, such as `SOCK_DGRAM` (UDP)
 * or `SOCK_STREAM` (TCP/IP).
 *
 * The address should be freed using `fio_sock_address_free`.
 */
FIO_IFUNC struct addrinfo *fio_sock_address_new(const char *restrict address,
                                                const char *restrict port,
                                                int sock_type);

/** Frees the pointer returned by `fio_sock_address_new`. */
FIO_IFUNC void fio_sock_address_free(struct addrinfo *a);

/** Creates a new network socket and binds it to a local address. */
SFUNC int fio_sock_open_local(struct addrinfo *addr, int nonblock);

/** Creates a new network socket and connects it to a remote address. */
SFUNC int fio_sock_open_remote(struct addrinfo *addr, int nonblock);

/** Creates a new Unix socket and binds it to a local address. */
SFUNC int fio_sock_open_unix(const char *address, int is_client, int nonblock);

/** Sets a file descriptor / socket to non blocking state. */
SFUNC int fio_sock_set_non_block(int fd);

/* *****************************************************************************
IO Poll - Implementation (always static / inlined)
***************************************************************************** */

FIO_SFUNC void fio___sock_poll_mock_ev(int fd, size_t index, void *udata) {
  (void)fd;
  (void)index;
  (void)udata;
}

int fio_sock_poll____(void); /* sublime text marker */
FIO_IFUNC int fio_sock_poll FIO_NOOP(fio_sock_poll_args args) {
  size_t event_count = 0;
  size_t limit = 0;
  if (!args.fds)
    goto empty_list;
  if (!args.count)
    while (args.fds[args.count].events)
      ++args.count;
  if (!args.count)
    goto empty_list;

  /* move if statement out of loop using a move callback */
  if (!args.on_ready)
    args.on_ready = fio___sock_poll_mock_ev;
  if (!args.on_data)
    args.on_data = fio___sock_poll_mock_ev;
  if (!args.on_error)
    args.on_error = fio___sock_poll_mock_ev;

  event_count = poll(args.fds, args.count, args.timeout);
  if (args.before_events)
    args.before_events(args.udata);
  if (event_count <= 0)
    goto finish;
  for (size_t i = 0; i < args.count && limit < event_count; ++i) {
    if (!args.fds[i].revents)
      continue;
    ++limit;
    if ((args.fds[i].revents & POLLOUT))
      args.on_ready(args.fds[i].fd, i, args.udata);
    if ((args.fds[i].revents & POLLIN))
      args.on_data(args.fds[i].fd, i, args.udata);
    if ((args.fds[i].revents & (POLLERR | POLLNVAL)))
      args.on_error(args.fds[i].fd, i, args.udata); /* TODO: POLLHUP ? */
  }
finish:
  if (args.after_events)
    args.after_events(args.udata);
  return event_count;
empty_list:
  if (args.timeout)
    FIO_THREAD_WAIT(args.timeout);
  if (args.before_events)
    args.before_events(args.udata);
  if (args.after_events)
    args.after_events(args.udata);
  return 0;
}

/**
 * Creates a new socket according to the provided flags.
 *
 * The `port` string will be ignored when `FIO_SOCK_UNIX` is set.
 */
FIO_IFUNC int fio_sock_open(const char *restrict address,
                            const char *restrict port,
                            uint16_t flags) {
  struct addrinfo *addr = NULL;
  int fd;
  switch ((flags & ((uint16_t)FIO_SOCK_TCP | (uint16_t)FIO_SOCK_UDP |
                    (uint16_t)FIO_SOCK_UNIX))) {
  case FIO_SOCK_UDP:
    addr = fio_sock_address_new(address, port, SOCK_DGRAM);
    if (!addr) {
      FIO_LOG_ERROR("(fio_sock_open) address error: %s", strerror(errno));
      return -1;
    }
    if ((flags & FIO_SOCK_CLIENT)) {
      fd = fio_sock_open_remote(addr, (flags & FIO_SOCK_NONBLOCK));
    } else {
      fd = fio_sock_open_local(addr, (flags & FIO_SOCK_NONBLOCK));
    }
    fio_sock_address_free(addr);
    return fd;
  case FIO_SOCK_TCP:
    addr = fio_sock_address_new(address, port, SOCK_STREAM);
    if (!addr) {
      FIO_LOG_ERROR("(fio_sock_open) address error: %s", strerror(errno));
      return -1;
    }
    if ((flags & FIO_SOCK_CLIENT)) {
      fd = fio_sock_open_remote(addr, (flags & FIO_SOCK_NONBLOCK));
    } else {
      fd = fio_sock_open_local(addr, (flags & FIO_SOCK_NONBLOCK));
      if (fd != -1 && listen(fd, SOMAXCONN) == -1) {
        FIO_LOG_ERROR("(fio_sock_open) failed on call to listen: %s",
                      strerror(errno));
        close(fd);
        fd = -1;
      }
    }
    fio_sock_address_free(addr);
    return fd;
  case FIO_SOCK_UNIX:
    return fio_sock_open_unix(
        address, (flags & FIO_SOCK_CLIENT), (flags & FIO_SOCK_NONBLOCK));
  }
  FIO_LOG_ERROR("(fio_sock_open) the FIO_SOCK_TCP, FIO_SOCK_UDP, and "
                "FIO_SOCK_UNIX flags are exclisive");
  return -1;
}

FIO_IFUNC struct addrinfo *
fio_sock_address_new(const char *restrict address,
                     const char *restrict port,
                     int sock_type /*i.e., SOCK_DGRAM */) {
  struct addrinfo addr_hints = (struct addrinfo){0}, *a;
  int e;
  addr_hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
  addr_hints.ai_socktype = sock_type;
  addr_hints.ai_flags = AI_PASSIVE; // use my IP

  if ((e = getaddrinfo(address, (port ? port : "0"), &addr_hints, &a)) != 0) {
    FIO_LOG_ERROR("(fio_sock_address_new) error: %s", gai_strerror(e));
    return NULL;
  }
  return a;
}

FIO_IFUNC void fio_sock_address_free(struct addrinfo *a) { freeaddrinfo(a); }

/* *****************************************************************************
FIO_SOCK - Implementation
***************************************************************************** */
#if defined(FIO_EXTERN_COMPLETE)

/** Sets a file descriptor / socket to non blocking state. */
SFUNC int fio_sock_set_non_block(int fd) {
/* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
  /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
  int flags;
  if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
    flags = 0;
#ifdef O_CLOEXEC
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK | O_CLOEXEC);
#else
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#endif
#elif defined(FIONBIO)
  /* Otherwise, use the old way of doing it */
  static int flags = 1;
  return ioctl(fd, FIONBIO, &flags);
#else
#error No functions / argumnet macros for non-blocking sockets.
#endif
}

/** Creates a new network socket and binds it to a local address. */
SFUNC int fio_sock_open_local(struct addrinfo *addr, int nonblock) {
  int fd = -1;
  for (struct addrinfo *p = addr; p != NULL; p = p->ai_next) {
    if ((fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      FIO_LOG_DEBUG("socket creation error %s", strerror(errno));
      continue;
    }
    {
      // avoid the "address taken"
      int optval = 1;
      setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    }
    if (bind(fd, p->ai_addr, p->ai_addrlen) == -1) {
      close(fd);
      fd = -1;
      continue;
    }
    if (nonblock && fio_sock_set_non_block(fd) == -1) {
      close(fd);
      fd = -1;
      continue;
    }
    break;
  }
  if (fd == -1) {
    FIO_LOG_DEBUG("socket binding/creation error %s", strerror(errno));
  }
  return fd;
}

/** Creates a new network socket and connects it to a remote address. */
SFUNC int fio_sock_open_remote(struct addrinfo *addr, int nonblock) {
  int fd = -1;
  for (struct addrinfo *p = addr; p != NULL; p = p->ai_next) {
    if ((fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      FIO_LOG_DEBUG("socket creation error %s", strerror(errno));
      continue;
    }

    if (nonblock && fio_sock_set_non_block(fd) == -1) {
      close(fd);
      fd = -1;
      continue;
    }
    if (connect(fd, p->ai_addr, p->ai_addrlen) == -1 && errno != EINPROGRESS) {
      close(fd);
      fd = -1;
      continue;
    }
    break;
  }
  if (fd == -1) {
    FIO_LOG_DEBUG("socket connection/creation error %s", strerror(errno));
  }
  return fd;
}

/** Creates a new Unix socket and binds it to a local address. */
SFUNC int fio_sock_open_unix(const char *address, int is_client, int nonblock) {
  /* Unix socket */
  struct sockaddr_un addr = {0};
  size_t addr_len = strlen(address);
  if (addr_len >= sizeof(addr.sun_path)) {
    FIO_LOG_ERROR(
        "(fio_sock_open_unix) address too long (%zu bytes > %zu bytes).",
        addr_len,
        sizeof(addr.sun_path) - 1);
    errno = ENAMETOOLONG;
    return -1;
  }
  addr.sun_family = AF_UNIX;
  memcpy(addr.sun_path, address, addr_len + 1); /* copy the NUL byte. */
#if defined(__APPLE__)
  addr.sun_len = addr_len;
#endif
  // get the file descriptor
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd == -1) {
    FIO_LOG_DEBUG("couldn't open unix socket (client? == %d) %s",
                  is_client,
                  strerror(errno));
    return -1;
  }
  /* chmod for foreign connections */
  fchmod(fd, S_IRWXO | S_IRWXG | S_IRWXU);
  if (nonblock && fio_sock_set_non_block(fd) == -1) {
    FIO_LOG_DEBUG("couldn't set socket to nonblocking mode");
    close(fd);
    return -1;
  }
  if (is_client) {
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1 &&
        errno != EINPROGRESS) {
      FIO_LOG_DEBUG("couldn't connect unix client: %s", strerror(errno));
      close(fd);
      return -1;
    }
  } else {
    unlink(addr.sun_path);
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
      FIO_LOG_DEBUG("couldn't bind unix socket to %s", address);
      // umask(old_umask);
      close(fd);
      return -1;
    }
    // umask(old_umask);
    if (listen(fd, SOMAXCONN) < 0) {
      FIO_LOG_DEBUG("couldn't start listening to unix socket at %s", address);
      close(fd);
      return -1;
    }
  }
  return fd;
}

#endif
/* *****************************************************************************
Socket helper testing
***************************************************************************** */
FIO_SFUNC void fio___sock_test_before_events(void *udata) {
  *(size_t *)udata = 0;
}
FIO_SFUNC void fio___sock_test_on_event(int fd, size_t index, void *udata) {
  *(size_t *)udata += 1;
  if (errno) {
    FIO_LOG_WARNING("(possibly expected) %s", strerror(errno));
    errno = 0;
  }
  (void)fd;
  (void)index;
}
FIO_SFUNC void fio___sock_test_after_events(void *udata) {
  if (*(size_t *)udata)
    *(size_t *)udata += 1;
}

FIO_SFUNC void FIO_NAME_TEST(stl, sock)(void) {
  fprintf(stderr,
          "* Testing socket helpers (FIO_SOCK) - partial tests only!\n");
#ifdef __cplusplus
  FIO_LOG_WARNING("fio_sock_poll test only runs in C - the FIO_SOCK_POLL_LIST "
                  "macro doesn't work in C++ and writing the test without it "
                  "is a headache.");
#else
  struct {
    const char *address;
    const char *port;
    const char *msg;
    uint16_t flag;
  } server_tests[] = {
      {"127.0.0.1", "9437", "TCP", FIO_SOCK_TCP},
#ifdef P_tmpdir
      {P_tmpdir "/tmp_unix_testing_socket_facil_io.sock",
       NULL,
       "Unix",
       FIO_SOCK_UNIX},
#else
      {"./tmp_unix_testing_socket_facil_io.sock", NULL, "Unix", FIO_SOCK_UNIX},
#endif
      /* accept doesn't work with UDP, not like this... UDP test is seperate */
      // {"127.0.0.1", "9437", "UDP", FIO_SOCK_UDP},
      {.address = NULL},
  };
  for (size_t i = 0; server_tests[i].address; ++i) {
    size_t flag = (size_t)-1;
    errno = 0;
    fprintf(stderr, "* Testing %s socket API\n", server_tests[i].msg);
    int srv = fio_sock_open(server_tests[i].address,
                            server_tests[i].port,
                            server_tests[i].flag | FIO_SOCK_SERVER);
    FIO_ASSERT(srv != -1, "server socket failed to open: %s", strerror(errno));
    flag = (size_t)-1;
    fio_sock_poll(.before_events = fio___sock_test_before_events,
                  .on_ready = NULL,
                  .on_data = NULL,
                  .on_error = fio___sock_test_on_event,
                  .after_events = fio___sock_test_after_events,
                  .udata = &flag);
    FIO_ASSERT(!flag, "before_events not called for missing list! (%zu)", flag);
    flag = (size_t)-1;
    fio_sock_poll(.before_events = fio___sock_test_before_events,
                  .on_ready = NULL,
                  .on_data = NULL,
                  .on_error = fio___sock_test_on_event,
                  .after_events = fio___sock_test_after_events,
                  .udata = &flag,
                  .fds = FIO_SOCK_POLL_LIST({.fd = -1}));
    FIO_ASSERT(!flag, "before_events not called for empty list! (%zu)", flag);
    flag = (size_t)-1;
    fio_sock_poll(.before_events = fio___sock_test_before_events,
                  .on_ready = NULL,
                  .on_data = NULL,
                  .on_error = fio___sock_test_on_event,
                  .after_events = fio___sock_test_after_events,
                  .udata = &flag,
                  .fds = FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_RW(srv)));
    FIO_ASSERT(!flag, "No event should have occured here! (%zu)", flag);
    flag = (size_t)-1;
    fio_sock_poll(.before_events = fio___sock_test_before_events,
                  .on_ready = NULL,
                  .on_data = fio___sock_test_on_event,
                  .on_error = NULL,
                  .after_events = fio___sock_test_after_events,
                  .udata = &flag,
                  .fds = FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_RW(srv)));
    FIO_ASSERT(!flag, "No event should have occured here! (%zu)", flag);
    flag = (size_t)-1;
    fio_sock_poll(.before_events = fio___sock_test_before_events,
                  .on_ready = fio___sock_test_on_event,
                  .on_data = NULL,
                  .on_error = NULL,
                  .after_events = fio___sock_test_after_events,
                  .udata = &flag,
                  .fds = FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_RW(srv)));
    FIO_ASSERT(!flag, "No event should have occured here! (%zu)", flag);

    int cl = fio_sock_open(server_tests[i].address,
                           server_tests[i].port,
                           server_tests[i].flag | FIO_SOCK_CLIENT);
    FIO_ASSERT(cl != -1, "client socket failed to open");
    fio_sock_poll(.before_events = fio___sock_test_before_events,
                  .on_ready = NULL,
                  .on_data = NULL,
                  .on_error = fio___sock_test_on_event,
                  .after_events = fio___sock_test_after_events,
                  .udata = &flag,
                  .fds = FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_RW(cl)));
    FIO_ASSERT(!flag, "No event should have occured here! (%zu)", flag);
    fio_sock_poll(.before_events = fio___sock_test_before_events,
                  .on_ready = NULL,
                  .on_data = fio___sock_test_on_event,
                  .on_error = NULL,
                  .after_events = fio___sock_test_after_events,
                  .udata = &flag,
                  .fds = FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_RW(cl)));
    FIO_ASSERT(!flag, "No event should have occured here! (%zu)", flag);
    // // is it possible to write to a still-connecting socket?
    // fio_sock_poll(.before_events = fio___sock_test_before_events,
    //               .after_events = fio___sock_test_after_events,
    //               .on_ready = fio___sock_test_on_event, .on_data = NULL,
    //               .on_error = NULL, .udata = &flag,
    //               .fds = FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_RW(cl)));
    // FIO_ASSERT(!flag, "No event should have occured here! (%zu)", flag);
    FIO_LOG_INFO("error may print when polling server for `write`.");
    fio_sock_poll(.before_events = fio___sock_test_before_events,
                  .on_ready = NULL,
                  .on_data = fio___sock_test_on_event,
                  .on_error = NULL,
                  .after_events = fio___sock_test_after_events,
                  .udata = &flag,
                  .timeout = 100,
                  .fds = FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_RW(srv)));
    FIO_ASSERT(flag == 2, "Event should have occured here! (%zu)", flag);
    FIO_LOG_INFO("error may have been emitted.");

    int accepted = accept(srv, NULL, NULL);
    FIO_ASSERT(accepted != -1, "client socket failed to open");
    fio_sock_poll(.before_events = fio___sock_test_before_events,
                  .on_ready = fio___sock_test_on_event,
                  .on_data = NULL,
                  .on_error = NULL,
                  .after_events = fio___sock_test_after_events,
                  .udata = &flag,
                  .timeout = 100,
                  .fds = FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_RW(cl)));
    FIO_ASSERT(flag, "Event should have occured here! (%zu)", flag);
    fio_sock_poll(.before_events = fio___sock_test_before_events,
                  .on_ready = fio___sock_test_on_event,
                  .on_data = NULL,
                  .on_error = NULL,
                  .after_events = fio___sock_test_after_events,
                  .udata = &flag,
                  .timeout = 100,
                  .fds = FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_RW(accepted)));
    FIO_ASSERT(flag, "Event should have occured here! (%zu)", flag);
    fio_sock_poll(.before_events = fio___sock_test_before_events,
                  .on_ready = NULL,
                  .on_data = fio___sock_test_on_event,
                  .on_error = NULL,
                  .after_events = fio___sock_test_after_events,
                  .udata = &flag,
                  .fds = FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_RW(cl)));
    FIO_ASSERT(!flag, "No event should have occured here! (%zu)", flag);

    if (write(accepted, "hello", 5) > 0) {
      // wait for read
      fio_sock_poll(.before_events = fio___sock_test_before_events,
                    .on_ready = NULL,
                    .on_data = fio___sock_test_on_event,
                    .on_error = NULL,
                    .after_events = fio___sock_test_after_events,
                    .udata = &flag,
                    .timeout = 100,
                    .fds = FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_R(cl)));
      // test read/write
      fio_sock_poll(.before_events = fio___sock_test_before_events,
                    .on_ready = fio___sock_test_on_event,
                    .on_data = fio___sock_test_on_event,
                    .on_error = NULL,
                    .after_events = fio___sock_test_after_events,
                    .udata = &flag,
                    .timeout = 100,
                    .fds = FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_RW(cl)));
      {
        char buf[64];
        errno = 0;
        FIO_ASSERT(read(cl, buf, 64) > 0,
                   "Read should have read some data...\n\t"
                   "error: %s",
                   strerror(errno));
      }
      FIO_ASSERT(flag == 3, "Event should have occured here! (%zu)", flag);
    } else
      FIO_ASSERT(0, "write failed! error: %s", strerror(errno));
    close(accepted);
    close(cl);
    close(srv);
    fio_sock_poll(.before_events = fio___sock_test_before_events,
                  .on_ready = NULL,
                  .on_data = NULL,
                  .on_error = fio___sock_test_on_event,
                  .after_events = fio___sock_test_after_events,
                  .udata = &flag,
                  .fds = FIO_SOCK_POLL_LIST(FIO_SOCK_POLL_RW(cl)));
    FIO_ASSERT(flag, "Event should have occured here! (%zu)", flag);
    if (FIO_SOCK_UNIX == server_tests[i].flag)
      unlink(server_tests[i].address);
  }
  {
    /* UDP semi test */
    fprintf(stderr, "* Testing UDP socket (abbreviated test)\n");
    int srv = fio_sock_open(NULL, "9437", FIO_SOCK_UDP | FIO_SOCK_SERVER);
    int n = 0; /* try for 32Mb */
    socklen_t sn = sizeof(n);
    if (-1 != getsockopt(srv, SOL_SOCKET, SO_RCVBUF, &n, &sn) &&
        sizeof(n) == sn)
      fprintf(stderr, "\t- UDP default receive buffer is %d bytes\n", n);
    n = 32 * 1024 * 1024; /* try for 32Mb */
    sn = sizeof(n);
    while (setsockopt(srv, SOL_SOCKET, SO_RCVBUF, &n, sn) == -1) {
      /* failed - repeat attempt at 0.5Mb interval */
      if (n >= (1024 * 1024)) // OS may have returned max value
        n -= 512 * 1024;
      else
        break;
    }
    if (-1 != getsockopt(srv, SOL_SOCKET, SO_RCVBUF, &n, &sn) &&
        sizeof(n) == sn)
      fprintf(stderr, "\t- UDP receive buffer could be set to %d bytes\n", n);
    FIO_ASSERT(
        srv != -1, "Couldn't open UDP server socket: %s", strerror(errno));
    int cl = fio_sock_open(NULL, "9437", FIO_SOCK_UDP | FIO_SOCK_CLIENT);
    FIO_ASSERT(
        cl != -1, "Couldn't open UDP client socket: %s", strerror(errno));
    FIO_ASSERT(send(cl, "hello", 5, 0) != -1,
               "couldn't send datagram from client");
    char buf[64];
    FIO_ASSERT(recvfrom(srv, buf, 64, 0, NULL, NULL) != -1,
               "couldn't read datagram");
    FIO_ASSERT(!memcmp(buf, "hello", 5), "transmission error");
    close(srv);
    close(cl);
  }
#endif /* __cplusplus */
}
/* *****************************************************************************
FIO_SOCK - cleanup
***************************************************************************** */
#undef FIO_SOCK
#endif
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************










                            Linked Lists (embeded)








Example:

```c
// initial `include` defines the `FIO_LIST_NODE` macro and type
#include "fio-stl.h"
// list element
typedef struct {
  long l;
  FIO_LIST_NODE node;
  int i;
  double d;
} my_list_s;
// create linked list helper functions
#define FIO_LIST_NAME my_list
#include "fio-stl.h"

void example(void) {
  FIO_LIST_HEAD list = FIO_LIST_INIT(list);
  for (int i = 0; i < 10; ++i) {
    my_list_s *n = malloc(sizeof(*n));
    n->i = i;
    my_list_push(&list, n);
  }
  int i = 0;
  while (my_list_any(&list)) {
    my_list_s *n = my_list_shift(&list);
    if (i != n->i) {
      fprintf(stderr, "list error - value mismatch\n"), exit(-1);
    }
    free(n);
    ++i;
  }
  if (i != 10) {
    fprintf(stderr, "list error - count error\n"), exit(-1);
  }
}
```

***************************************************************************** */

/* *****************************************************************************
Linked Lists (embeded) - Type
***************************************************************************** */

#if defined(FIO_LIST_NAME)

#ifndef FIO_LIST_TYPE
/** Name of the list type and function prefix, defaults to FIO_LIST_NAME_s */
#define FIO_LIST_TYPE FIO_NAME(FIO_LIST_NAME, s)
#endif

#ifndef FIO_LIST_NODE_NAME
/** List types must contain at least one node element, defaults to `node`. */
#define FIO_LIST_NODE_NAME node
#endif

#ifdef FIO_PTR_TAG_TYPE
#define FIO_LIST_TYPE_PTR FIO_PTR_TAG_TYPE
#else
#define FIO_LIST_TYPE_PTR FIO_LIST_TYPE *
#endif

/* *****************************************************************************
Linked Lists (embeded) - API
***************************************************************************** */

/** Initialize FIO_LIST_HEAD objects - already defined. */
/* FIO_LIST_INIT(obj) */

/** Returns a non-zero value if there are any linked nodes in the list. */
IFUNC int FIO_NAME(FIO_LIST_NAME, any)(const FIO_LIST_HEAD *head);

/** Returns a non-zero value if the list is empty. */
IFUNC int FIO_NAME_BL(FIO_LIST_NAME, empty)(const FIO_LIST_HEAD *head);

/** Removes a node from the list, Returns NULL if node isn't linked. */
IFUNC FIO_LIST_TYPE_PTR FIO_NAME(FIO_LIST_NAME, remove)(FIO_LIST_TYPE_PTR node);

/** Pushes an existing node to the end of the list. Returns node. */
IFUNC FIO_LIST_TYPE_PTR FIO_NAME(FIO_LIST_NAME,
                                 push)(FIO_LIST_HEAD *restrict head,
                                       FIO_LIST_TYPE_PTR restrict node);

/** Pops a node from the end of the list. Returns NULL if list is empty. */
IFUNC FIO_LIST_TYPE_PTR FIO_NAME(FIO_LIST_NAME, pop)(FIO_LIST_HEAD *head);

/** Adds an existing node to the beginning of the list. Returns node. */
IFUNC FIO_LIST_TYPE_PTR FIO_NAME(FIO_LIST_NAME,
                                 unshift)(FIO_LIST_HEAD *restrict head,
                                          FIO_LIST_TYPE_PTR restrict node);

/** Removed a node from the start of the list. Returns NULL if list is empty. */
IFUNC FIO_LIST_TYPE_PTR FIO_NAME(FIO_LIST_NAME, shift)(FIO_LIST_HEAD *head);

/** Returns a pointer to a list's element, from a pointer to a node. */
IFUNC FIO_LIST_TYPE_PTR FIO_NAME(FIO_LIST_NAME, root)(FIO_LIST_HEAD *ptr);

/* *****************************************************************************
Linked Lists (embeded) - Implementation
***************************************************************************** */
#ifdef FIO_EXTERN_COMPLETE

/** Returns a non-zero value if there are any linked nodes in the list. */
IFUNC int FIO_NAME(FIO_LIST_NAME, any)(const FIO_LIST_HEAD *head) {
  FIO_PTR_TAG_VALID_OR_RETURN(head, 0);
  head = (FIO_LIST_HEAD *)(FIO_PTR_UNTAG(head));
  return head->next != head;
}

/** Returns a non-zero value if the list is empty. */
IFUNC int FIO_NAME_BL(FIO_LIST_NAME, empty)(const FIO_LIST_HEAD *head) {
  FIO_PTR_TAG_VALID_OR_RETURN(head, 0);
  head = (FIO_LIST_HEAD *)(FIO_PTR_UNTAG(head));
  return head->next == head;
}

/** Removes a node from the list, always returning the node. */
IFUNC FIO_LIST_TYPE_PTR FIO_NAME(FIO_LIST_NAME,
                                 remove)(FIO_LIST_TYPE_PTR node_) {
  FIO_PTR_TAG_VALID_OR_RETURN(node_, (FIO_LIST_TYPE_PTR)0);
  FIO_LIST_TYPE *node = (FIO_LIST_TYPE *)(FIO_PTR_UNTAG(node_));
  if (node->FIO_LIST_NODE_NAME.next == &node->FIO_LIST_NODE_NAME)
    return NULL;
  node->FIO_LIST_NODE_NAME.prev->next = node->FIO_LIST_NODE_NAME.next;
  node->FIO_LIST_NODE_NAME.next->prev = node->FIO_LIST_NODE_NAME.prev;
  node->FIO_LIST_NODE_NAME.next = node->FIO_LIST_NODE_NAME.prev =
      &node->FIO_LIST_NODE_NAME;
  return node_;
}

/** Pushes an existing node to the end of the list. Returns node or NULL. */
IFUNC FIO_LIST_TYPE_PTR FIO_NAME(FIO_LIST_NAME,
                                 push)(FIO_LIST_HEAD *restrict head,
                                       FIO_LIST_TYPE_PTR restrict node_) {
  FIO_PTR_TAG_VALID_OR_RETURN(head, (FIO_LIST_TYPE_PTR)NULL);
  FIO_PTR_TAG_VALID_OR_RETURN(node_, (FIO_LIST_TYPE_PTR)NULL);
  head = (FIO_LIST_HEAD *)(FIO_PTR_UNTAG(head));
  FIO_LIST_TYPE *restrict node = (FIO_LIST_TYPE *)(FIO_PTR_UNTAG(node_));
  node->FIO_LIST_NODE_NAME.prev = head->prev;
  node->FIO_LIST_NODE_NAME.next = head;
  head->prev->next = &node->FIO_LIST_NODE_NAME;
  head->prev = &node->FIO_LIST_NODE_NAME;
  return node_;
}

/** Pops a node from the end of the list. Returns NULL if list is empty. */
IFUNC FIO_LIST_TYPE_PTR FIO_NAME(FIO_LIST_NAME, pop)(FIO_LIST_HEAD *head) {
  FIO_PTR_TAG_VALID_OR_RETURN(head, (FIO_LIST_TYPE_PTR)NULL);
  head = (FIO_LIST_HEAD *)(FIO_PTR_UNTAG(head));
  return FIO_NAME(FIO_LIST_NAME, remove)(
      FIO_PTR_FROM_FIELD(FIO_LIST_TYPE, FIO_LIST_NODE_NAME, head->prev));
}

/** Adds an existing node to the beginning of the list. Returns node or NULL. */
IFUNC FIO_LIST_TYPE_PTR FIO_NAME(FIO_LIST_NAME,
                                 unshift)(FIO_LIST_HEAD *restrict head,
                                          FIO_LIST_TYPE_PTR restrict node) {
  FIO_PTR_TAG_VALID_OR_RETURN(head, (FIO_LIST_TYPE_PTR)NULL);
  FIO_PTR_TAG_VALID_OR_RETURN(node, (FIO_LIST_TYPE_PTR)NULL);
  head = (FIO_LIST_HEAD *)(FIO_PTR_UNTAG(head));
  return FIO_NAME(FIO_LIST_NAME, push)(head->next, node);
}

/** Removed a node from the start of the list. Returns NULL if list is empty. */
IFUNC FIO_LIST_TYPE_PTR FIO_NAME(FIO_LIST_NAME, shift)(FIO_LIST_HEAD *head) {
  FIO_PTR_TAG_VALID_OR_RETURN(head, (FIO_LIST_TYPE_PTR)NULL);
  head = (FIO_LIST_HEAD *)(FIO_PTR_UNTAG(head));
  return FIO_NAME(FIO_LIST_NAME, remove)(
      FIO_PTR_FROM_FIELD(FIO_LIST_TYPE, FIO_LIST_NODE_NAME, head->next));
}

/** Removed a node from the start of the list. Returns NULL if list is empty. */
IFUNC FIO_LIST_TYPE_PTR FIO_NAME(FIO_LIST_NAME, root)(FIO_LIST_HEAD *ptr) {
  FIO_PTR_TAG_VALID_OR_RETURN(ptr, (FIO_LIST_TYPE_PTR)NULL);
  ptr = (FIO_LIST_HEAD *)(FIO_PTR_UNTAG(ptr));
  return FIO_PTR_FROM_FIELD(FIO_LIST_TYPE, FIO_LIST_NODE_NAME, ptr);
}

/* *****************************************************************************
Linked Lists (embeded) - cleanup
***************************************************************************** */

#endif /* FIO_EXTERN_COMPLETE */
#undef FIO_LIST_NAME
#undef FIO_LIST_TYPE
#undef FIO_LIST_NODE_NAME
#undef FIO_LIST_TYPE_PTR
#endif
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************










                            Dynamic Arrays








Example:

```c
typedef struct {
  int i;
  float f;
} foo_s;

#define FIO_ARRAY_NAME ary
#define FIO_ARRAY_TYPE foo_s
#define FIO_ARRAY_TYPE_CMP(a,b) (a.i == b.i && a.f == b.f)
#include "fio_cstl.h"

void example(void) {
  ary_s a = FIO_ARRAY_INIT;
  foo_s *p = ary_push(&a, (foo_s){.i = 42});
  FIO_ARRAY_EACH(&a, pos) { // pos will be a pointer to the element
    fprintf(stderr, "* [%zu]: %p : %d\n", (size_t)(pos - ary2ptr(&a)), pos->i);
  }
  ary_destroy(&a);
}
```

***************************************************************************** */

#ifdef FIO_ARRAY_NAME

#ifndef FIO_ARRAY_TYPE
/** The type for array elements (an array of FIO_ARRAY_TYPE) */
#define FIO_ARRAY_TYPE void *
/** An invalid value for that type (if any). */
#define FIO_ARRAY_TYPE_INVALID NULL
#define FIO_ARRAY_TYPE_INVALID_SIMPLE 1
#else
#ifndef FIO_ARRAY_TYPE_INVALID
/** An invalid value for that type (if any). */
#define FIO_ARRAY_TYPE_INVALID ((FIO_ARRAY_TYPE){0})
/* internal flag - do not set */
#define FIO_ARRAY_TYPE_INVALID_SIMPLE 1
#endif
#endif

#ifndef FIO_ARRAY_TYPE_COPY
/** Handles a copy operation for an array's element. */
#define FIO_ARRAY_TYPE_COPY(dest, src) (dest) = (src)
/* internal flag - do not set */
#define FIO_ARRAY_TYPE_COPY_SIMPLE 1
#endif

#ifndef FIO_ARRAY_TYPE_DESTROY
/** Handles a destroy / free operation for an array's element. */
#define FIO_ARRAY_TYPE_DESTROY(obj)
/* internal flag - do not set */
#define FIO_ARRAY_TYPE_DESTROY_SIMPLE 1
#endif

#ifndef FIO_ARRAY_TYPE_CMP
/** Handles a comparison operation for an array's element. */
#define FIO_ARRAY_TYPE_CMP(a, b) (a) == (b)
/* internal flag - do not set */
#define FIO_ARRAY_TYPE_CMP_SIMPLE 1
#endif

#ifndef FIO_ARRAY_TYPE_CONCAT_COPY
#define FIO_ARRAY_TYPE_CONCAT_COPY FIO_ARRAY_TYPE_COPY
#define FIO_ARRAY_TYPE_CONCAT_SIMPLE FIO_ARRAY_TYPE_COPY_SIMPLE
#endif
/**
 * The FIO_ARRAY_DESTROY_AFTER_COPY macro should be set if
 * FIO_ARRAY_TYPE_DESTROY should be called after FIO_ARRAY_TYPE_COPY when an
 * object is removed from the array after being copied to an external container
 * (an `old` pointer)
 */
#ifndef FIO_ARRAY_DESTROY_AFTER_COPY
#if !FIO_ARRAY_TYPE_DESTROY_SIMPLE && !FIO_ARRAY_TYPE_COPY_SIMPLE
#define FIO_ARRAY_DESTROY_AFTER_COPY 1
#else
#define FIO_ARRAY_DESTROY_AFTER_COPY 0
#endif
#endif

/* Extra empty slots when allocating memory. */
#ifndef FIO_ARRAY_PADDING
#define FIO_ARRAY_PADDING 4
#endif

/* Sets memory growth to exponentially increase. Consumes more memory. */
#ifndef FIO_ARRAY_EXPONENTIAL
#define FIO_ARRAY_EXPONENTIAL 0
#endif

#undef FIO_ARRAY_SIZE2WORDS
#define FIO_ARRAY_SIZE2WORDS(size)                                             \
  ((sizeof(FIO_ARRAY_TYPE) & 1)                                                \
       ? (((size) & (~15)) + 16)                                               \
       : (sizeof(FIO_ARRAY_TYPE) & 2)                                          \
             ? (((size) & (~7)) + 8)                                           \
             : (sizeof(FIO_ARRAY_TYPE) & 4)                                    \
                   ? (((size) & (~3)) + 4)                                     \
                   : (sizeof(FIO_ARRAY_TYPE) & 8) ? (((size) & (~1)) + 2)      \
                                                  : (size))

/* *****************************************************************************
Dynamic Arrays - type
***************************************************************************** */

typedef struct {
  FIO_ARRAY_TYPE *ary;
  uint32_t capa;
  uint32_t start;
  uint32_t end;
} FIO_NAME(FIO_ARRAY_NAME, s);

#ifdef FIO_PTR_TAG_TYPE
#define FIO_ARRAY_PTR FIO_PTR_TAG_TYPE
#else
#define FIO_ARRAY_PTR FIO_NAME(FIO_ARRAY_NAME, s) *
#endif

/* *****************************************************************************
Dynamic Arrays - API
***************************************************************************** */

#ifndef FIO_ARRAY_INIT
/* Initialization macro. */
#define FIO_ARRAY_INIT                                                         \
  { 0 }
#endif

#ifndef FIO_REF_CONSTRUCTOR_ONLY

/* Allocates a new array object on the heap and initializes it's memory. */
IFUNC FIO_ARRAY_PTR FIO_NAME(FIO_ARRAY_NAME, new)(void);

/* Frees an array's internal data AND it's container! */
IFUNC void FIO_NAME(FIO_ARRAY_NAME, free)(FIO_ARRAY_PTR ary);

#endif /* FIO_REF_CONSTRUCTOR_ONLY */

/* Destroys any objects stored in the array and frees the internal state. */
IFUNC void FIO_NAME(FIO_ARRAY_NAME, destroy)(FIO_ARRAY_PTR ary);

/** Returns the number of elements in the Array. */
IFUNC uint32_t FIO_NAME(FIO_ARRAY_NAME, count)(FIO_ARRAY_PTR ary);

/** Returns the current, temporary, array capacity (it's dynamic). */
IFUNC uint32_t FIO_NAME(FIO_ARRAY_NAME, capa)(FIO_ARRAY_PTR ary);

/**
 * Reserves a minimal capacity for the array.
 *
 * If `capa` is negative, new memory will be allocated at the beginning of the
 * array rather then it's end.
 *
 * Returns the array's new capacity.
 *
 * Note: the reserved capacity includes existing data. If the requested reserved
 * capacity is equal (or less) then the existing capacity, nothing will be done.
 */
IFUNC uint32_t FIO_NAME(FIO_ARRAY_NAME, reserve)(FIO_ARRAY_PTR ary,
                                                 int32_t capa);

/**
 * Adds all the items in the `src` Array to the end of the `dest` Array.
 *
 * The `src` Array remain untouched.
 *
 * Always returns the destination array (`dest`).
 */
SFUNC FIO_ARRAY_PTR FIO_NAME(FIO_ARRAY_NAME, concat)(FIO_ARRAY_PTR dest,
                                                     FIO_ARRAY_PTR src);

/**
 * Sets `index` to the value in `data`.
 *
 * If `index` is negative, it will be counted from the end of the Array (-1 ==
 * last element).
 *
 * If `old` isn't NULL, the existing data will be copied to the location pointed
 * to by `old` before the copy in the Array is destroyed.
 *
 * Returns a pointer to the new object, or NULL on error.
 */
IFUNC FIO_ARRAY_TYPE *FIO_NAME(FIO_ARRAY_NAME, set)(FIO_ARRAY_PTR ary,
                                                    int32_t index,
                                                    FIO_ARRAY_TYPE data,
                                                    FIO_ARRAY_TYPE *old);

/**
 * Returns the value located at `index` (no copying is performed).
 *
 * If `index` is negative, it will be counted from the end of the Array (-1 ==
 * last element).
 */
IFUNC FIO_ARRAY_TYPE FIO_NAME(FIO_ARRAY_NAME, get)(FIO_ARRAY_PTR ary,
                                                   int32_t index);

/**
 * Returns the index of the object or -1 if the object wasn't found.
 *
 * If `start_at` is negative (i.e., -1), than seeking will be performed in
 * reverse, where -1 == last index (-2 == second to last, etc').
 */
IFUNC int32_t FIO_NAME(FIO_ARRAY_NAME, find)(FIO_ARRAY_PTR ary,
                                             FIO_ARRAY_TYPE data,
                                             int32_t start_at);

/**
 * Removes an object from the array, MOVING all the other objects to prevent
 * "holes" in the data.
 *
 * If `old` is set, the data is copied to the location pointed to by `old`
 * before the data in the array is destroyed.
 *
 * Returns 0 on success and -1 on error.
 *
 * This action is O(n) where n in the length of the array.
 * It could get expensive.
 */
IFUNC int FIO_NAME(FIO_ARRAY_NAME, remove)(FIO_ARRAY_PTR ary,
                                           int32_t index,
                                           FIO_ARRAY_TYPE *old);

/**
 * Removes all occurrences of an object from the array (if any), MOVING all the
 * existing objects to prevent "holes" in the data.
 *
 * Returns the number of items removed.
 *
 * This action is O(n) where n in the length of the array.
 * It could get expensive.
 */
IFUNC uint32_t FIO_NAME(FIO_ARRAY_NAME, remove2)(FIO_ARRAY_PTR ary,
                                                 FIO_ARRAY_TYPE data);

/** Attempts to lower the array's memory consumption. */
IFUNC void FIO_NAME(FIO_ARRAY_NAME, compact)(FIO_ARRAY_PTR ary);

/**
 * Returns a pointer to the C array containing the objects.
 */
IFUNC FIO_ARRAY_TYPE *FIO_NAME2(FIO_ARRAY_NAME, ptr)(FIO_ARRAY_PTR ary);

/**
 * Pushes an object to the end of the Array. Returns a pointer to the new object
 * or NULL on error.
 */
IFUNC FIO_ARRAY_TYPE *FIO_NAME(FIO_ARRAY_NAME, push)(FIO_ARRAY_PTR ary,
                                                     FIO_ARRAY_TYPE data);

/**
 * Removes an object from the end of the Array.
 *
 * If `old` is set, the data is copied to the location pointed to by `old`
 * before the data in the array is destroyed.
 *
 * Returns -1 on error (Array is empty) and 0 on success.
 */
IFUNC int FIO_NAME(FIO_ARRAY_NAME, pop)(FIO_ARRAY_PTR ary, FIO_ARRAY_TYPE *old);

/**
 * Unshifts an object to the beginning of the Array. Returns a pointer to the
 * new object or NULL on error.
 *
 * This could be expensive, causing `memmove`.
 */
IFUNC FIO_ARRAY_TYPE *FIO_NAME(FIO_ARRAY_NAME, unshift)(FIO_ARRAY_PTR ary,
                                                        FIO_ARRAY_TYPE data);

/**
 * Removes an object from the beginning of the Array.
 *
 * If `old` is set, the data is copied to the location pointed to by `old`
 * before the data in the array is destroyed.
 *
 * Returns -1 on error (Array is empty) and 0 on success.
 */
IFUNC int FIO_NAME(FIO_ARRAY_NAME, shift)(FIO_ARRAY_PTR ary,
                                          FIO_ARRAY_TYPE *old);

/**
 * Iteration using a callback for each entry in the array.
 *
 * The callback task function must accept an the entry data as well as an opaque
 * user pointer.
 *
 * If the callback returns -1, the loop is broken. Any other value is ignored.
 *
 * Returns the relative "stop" position, i.e., the number of items processed +
 * the starting point.
 */
IFUNC uint32_t FIO_NAME(FIO_ARRAY_NAME,
                        each)(FIO_ARRAY_PTR ary,
                              int32_t start_at,
                              int (*task)(FIO_ARRAY_TYPE obj, void *arg),
                              void *arg);

#ifndef FIO_ARRAY_EACH
/**
 * Iterates through the list using a `for` loop.
 *
 * Access the object with the pointer `pos`. The `pos` variable can be named
 * however you please.
 *
 * Avoid editing the array during a FOR loop, although I hope it's possible, I
 * wouldn't count on it.
 *
 * **Note**: doesn't support automatic pointer tagging / untagging.
 */
#define FIO_ARRAY_EACH(array, pos)                                             \
  if ((array)->ary)                                                            \
    for (__typeof__((array)->ary) start__tmp__ = (array)->ary,                 \
                                  pos = ((array)->ary + (array)->start);       \
         pos < (array)->ary + (array)->end;                                    \
         (pos = (array)->ary + (pos - start__tmp__) + 1),                      \
                                  (start__tmp__ = (array)->ary))
#endif

#ifdef FIO_EXTERN_COMPLETE

/* *****************************************************************************
Dynamic Arrays - internal helpers
***************************************************************************** */

#define FIO_ARRAY_POS2ABS(ary, pos)                                            \
  (pos > 0 ? (ary->start + pos) : (ary->end - pos))

#define FIO_ARRAY_AB_CT(cond, a, b) ((b) ^ ((0 - ((cond)&1)) & ((a) ^ (b))))

/* *****************************************************************************
Dynamic Arrays - implementation
***************************************************************************** */

#ifndef FIO_REF_CONSTRUCTOR_ONLY
/* Allocates a new array object on the heap and initializes it's memory. */
IFUNC FIO_ARRAY_PTR FIO_NAME(FIO_ARRAY_NAME, new)(void) {
  FIO_NAME(FIO_ARRAY_NAME, s) *a =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)FIO_MEM_CALLOC_(sizeof(*a), 1);
  return (FIO_ARRAY_PTR)FIO_PTR_TAG(a);
}

/* Frees an array's internal data AND it's container! */
IFUNC void FIO_NAME(FIO_ARRAY_NAME, free)(FIO_ARRAY_PTR ary_) {
  FIO_PTR_TAG_VALID_OR_RETURN_VOID(ary_);
  FIO_NAME(FIO_ARRAY_NAME, s) *ary =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
  FIO_NAME(FIO_ARRAY_NAME, destroy)(ary_);
  FIO_MEM_FREE_(ary, sizeof(*ary));
}
#endif /* FIO_REF_CONSTRUCTOR_ONLY */

/* Destroys any objects stored in the array and frees the internal state. */
IFUNC void FIO_NAME(FIO_ARRAY_NAME, destroy)(FIO_ARRAY_PTR ary_) {
  FIO_PTR_TAG_VALID_OR_RETURN_VOID(ary_);
  FIO_NAME(FIO_ARRAY_NAME, s) *ary =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
  /* move array data to a temp var., protecting against recursive nesting */
  FIO_NAME(FIO_ARRAY_NAME, s) tmp = *ary;
  *ary = (FIO_NAME(FIO_ARRAY_NAME, s))FIO_ARRAY_INIT;
#if !FIO_ARRAY_TYPE_DESTROY_SIMPLE
  for (size_t i = tmp.start; i < tmp.end; ++i) {
    FIO_ARRAY_TYPE_DESTROY(tmp.ary[i]);
  }
#endif
  FIO_MEM_FREE_(tmp.ary, tmp.capa * sizeof(*tmp.ary));
}

/** Returns the number of elements in the Array. */
IFUNC uint32_t FIO_NAME(FIO_ARRAY_NAME, count)(FIO_ARRAY_PTR ary_) {
  FIO_PTR_TAG_VALID_OR_RETURN(ary_, 0);
  FIO_NAME(FIO_ARRAY_NAME, s) *ary =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
  return (ary->end - ary->start);
}

/** Returns the current, temporary, array capacity (it's dynamic). */
IFUNC uint32_t FIO_NAME(FIO_ARRAY_NAME, capa)(FIO_ARRAY_PTR ary_) {
  FIO_PTR_TAG_VALID_OR_RETURN(ary_, 0);
  FIO_NAME(FIO_ARRAY_NAME, s) *ary =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
  return ary->capa;
}
/** Reserves a minimal capacity for the array. */
IFUNC uint32_t FIO_NAME(FIO_ARRAY_NAME, reserve)(FIO_ARRAY_PTR ary_,
                                                 int32_t capa) {
  FIO_PTR_TAG_VALID_OR_RETURN(ary_, 0);
  FIO_NAME(FIO_ARRAY_NAME, s) *ary =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
  if (!ary)
    return 0;
  const uint32_t s = ary->start;
  const uint32_t e = ary->end;
  if (capa > 0) {
    if (ary->capa >= (uint32_t)capa)
      return ary->capa;
    FIO_NAME(FIO_ARRAY_NAME, set)(ary_, capa - 1, FIO_ARRAY_TYPE_INVALID, NULL);
    ary->end = ary->start + (e - s);
  } else {
    if (ary->capa >= (uint32_t)(0 - capa))
      return ary->capa;
    FIO_NAME(FIO_ARRAY_NAME, set)(ary_, capa, FIO_ARRAY_TYPE_INVALID, NULL);
    ary->start = ary->end - (e - s);
  }
  return ary->capa;
}

/**
 * Adds all the items in the `src` Array to the end of the `dest` Array.
 *
 * The `src` Array remain untouched.
 *
 * Returns `dest` on success or NULL on error (i.e., no memory).
 */
SFUNC FIO_ARRAY_PTR FIO_NAME(FIO_ARRAY_NAME, concat)(FIO_ARRAY_PTR dest_,
                                                     FIO_ARRAY_PTR src_) {
  FIO_PTR_TAG_VALID_OR_RETURN(dest_, (FIO_ARRAY_PTR)NULL);
  FIO_PTR_TAG_VALID_OR_RETURN(src_, (FIO_ARRAY_PTR)NULL);
  FIO_NAME(FIO_ARRAY_NAME, s) *dest =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(dest_));
  FIO_NAME(FIO_ARRAY_NAME, s) *src =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(src_));
  if (!dest || !src || !src->end || src->end - src->start == 0)
    return dest_;
  /* avoid '-' in (dest->capa < dest->end + src->end - src->start) */
  if (dest->capa + src->start < src->end + dest->end) {
    /* insufficiant memory, (re)allocate */
    uint32_t new_capa = dest->end + (src->end - src->start);
    FIO_ARRAY_TYPE *tmp =
        (FIO_ARRAY_TYPE *)FIO_MEM_REALLOC_(dest->ary,
                                           dest->capa * sizeof(*tmp),
                                           new_capa * sizeof(*tmp),
                                           dest->end * sizeof(*tmp));
    if (!tmp)
      return (FIO_ARRAY_PTR)(NULL);
    dest->ary = tmp;
    dest->capa = new_capa;
  }
  /* copy data */
#if FIO_ARRAY_TYPE_COPY_SIMPLE && FIO_ARRAY_TYPE_CONCAT_SIMPLE
  memcpy(dest->ary + dest->end, src->ary + src->start, src->end - src->start);
#else
  for (size_t i = 0; i + src->start < src->end; ++i) {
    FIO_ARRAY_TYPE_CONCAT_COPY((dest->ary + dest->end + i)[0],
                               (src->ary + i + src->start)[0]);
  }
#endif
  /* update dest */
  dest->end += src->end - src->start;
  return dest_;
}

/**
 * Sets `index` to the value in `data`.
 *
 * If `index` is negative, it will be counted from the end of the Array (-1 ==
 * last element).
 *
 * If `old` isn't NULL, the existing data will be copied to the location pointed
 * to by `old` before the copy in the Array is destroyed.
 *
 * Returns a pointer to the new object, or NULL on error.
 */
IFUNC FIO_ARRAY_TYPE *FIO_NAME(FIO_ARRAY_NAME, set)(FIO_ARRAY_PTR ary_,
                                                    int32_t index,
                                                    FIO_ARRAY_TYPE data,
                                                    FIO_ARRAY_TYPE *old) {
#if FIO_ARRAY_EXPONENTIAL
#define FIO_ARRAY_ADD2CAPA ary->capa + FIO_ARRAY_PADDING
#else
#define FIO_ARRAY_ADD2CAPA FIO_ARRAY_PADDING
#endif

  FIO_PTR_TAG_VALID_OR_GOTO(ary_, invalid_ptr);
  {
    FIO_NAME(FIO_ARRAY_NAME, s) *ary =
        (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
    uint8_t pre_existing = 1;
    if (index >= 0) {
      /* zero based (look forward) */
      index = index + ary->start;
      if ((uint32_t)index >= ary->capa) {
        /* we need more memory */
        uint32_t new_capa =
            FIO_ARRAY_SIZE2WORDS(((uint32_t)index + FIO_ARRAY_ADD2CAPA));
        FIO_ARRAY_TYPE *tmp =
            (FIO_ARRAY_TYPE *)FIO_MEM_REALLOC_(ary->ary,
                                               ary->capa * sizeof(*tmp),
                                               new_capa * sizeof(*tmp),
                                               ary->end * sizeof(*tmp));
        if (!tmp)
          return NULL;
        ary->ary = tmp;
        ary->capa = new_capa;
      }
      ary->ary[ary->end++] = FIO_ARRAY_TYPE_INVALID;
      if ((uint32_t)index >= ary->end) {
        /* we to initialize memory between ary->end and index + ary->start */
        pre_existing = 0;
#if FIO_ARRAY_TYPE_INVALID_SIMPLE
        memset(ary->ary + ary->end, 0, (index - ary->end) * sizeof(*ary->ary));
#else
        for (size_t i = ary->end; i <= (size_t)index; ++i) {
          FIO_ARRAY_TYPE_COPY(ary->ary[i], FIO_ARRAY_TYPE_INVALID);
        }
#endif
        ary->end = index + 1;
      }
    } else {
      /* -1 based (look backwards) */
      index += ary->end;
      if (index < 0) {
        /* we need more memory at the HEAD (requires copying) */
        const uint32_t new_capa = FIO_ARRAY_SIZE2WORDS(
            ((uint32_t)ary->capa + FIO_ARRAY_ADD2CAPA + ((uint32_t)0 - index)));
        const uint32_t valid_data = ary->end - ary->start;
        index -= ary->end; /* return to previous state */
        FIO_ARRAY_TYPE *tmp =
            (FIO_ARRAY_TYPE *)FIO_MEM_CALLOC_(new_capa, sizeof(*tmp));
        if (!tmp)
          return NULL;
        if (valid_data)
          memcpy(tmp + new_capa - valid_data,
                 ary->ary + ary->start,
                 valid_data * sizeof(*tmp));
        FIO_MEM_FREE_(ary->ary, sizeof(*ary->ary) * ary->capa);
        ary->end = ary->capa = new_capa;
        index += new_capa;
        ary->ary = tmp;
#if FIO_ARRAY_TYPE_INVALID_SIMPLE
        ary->start = index;
#else
        ary->start = new_capa - valid_data;
#endif
      }
      if ((uint32_t)index < ary->start) {
        /* initialize memory between `index` and `ary->start-1` */
        pre_existing = 0;
#if FIO_ARRAY_TYPE_INVALID_SIMPLE
        memset(ary->ary + index, 0, (ary->start - index) * sizeof(*ary->ary));
        ary->start = index;
#else
        while ((uint32_t)index < ary->start) {
          --ary->start;
          ary->ary[ary->start] = FIO_ARRAY_TYPE_INVALID;
          // FIO_ARRAY_TYPE_COPY(ary->ary[ary->start], FIO_ARRAY_TYPE_INVALID);
        }
#endif
      }
    }
    /* copy / clear object */
    if (old) {
      FIO_ARRAY_TYPE_COPY((*old), ary->ary[index]);
#if FIO_ARRAY_DESTROY_AFTER_COPY
      if (pre_existing) {
        FIO_ARRAY_TYPE_DESTROY(ary->ary[index]);
      }
#endif
    } else if (pre_existing) {
      FIO_ARRAY_TYPE_DESTROY(ary->ary[index]);
    }

    ary->ary[index] = FIO_ARRAY_TYPE_INVALID;
    FIO_ARRAY_TYPE_COPY(ary->ary[index], data);
    return ary->ary + index;
  }

invalid_ptr:
  FIO_ARRAY_TYPE_DESTROY(data);
  return NULL;
}
#undef FIO_ARRAY_ADD2CAPA
/**
 * Returns the value located at `index` (no copying is performed).
 *
 * If `index` is negative, it will be counted from the end of the Array (-1 ==
 * last element).
 */
IFUNC FIO_ARRAY_TYPE FIO_NAME(FIO_ARRAY_NAME, get)(FIO_ARRAY_PTR ary_,
                                                   int32_t index) {
  FIO_PTR_TAG_VALID_OR_RETURN(ary_, FIO_ARRAY_TYPE_INVALID);
  FIO_NAME(FIO_ARRAY_NAME, s) *ary =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
  index += FIO_ARRAY_AB_CT(index >= 0, ary->start, ary->end);
  if (index < 0 || (uint32_t)index >= ary->end)
    return FIO_ARRAY_TYPE_INVALID;
  return ary->ary[index];
}

/**
 * Returns the index of the object or -1 if the object wasn't found.
 *
 * If `start_at` is negative (i.e., -1), than seeking will be performed in
 * reverse, where -1 == last index (-2 == second to last, etc').
 */
IFUNC int32_t FIO_NAME(FIO_ARRAY_NAME, find)(FIO_ARRAY_PTR ary_,
                                             FIO_ARRAY_TYPE data,
                                             int32_t start_at) {
  FIO_PTR_TAG_VALID_OR_RETURN(ary_, -1);
  FIO_NAME(FIO_ARRAY_NAME, s) *ary =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
  if (start_at >= 0) {
    /* seek forwards */
    while ((uint32_t)start_at < ary->end) {
      if (FIO_ARRAY_TYPE_CMP(ary->ary[start_at], data))
        return start_at;
      ++start_at;
    }
  } else {
    /* seek backwards */
    start_at = start_at + ary->end;
    if (start_at >= (int32_t)ary->end)
      start_at = ary->end - 1;
    while (start_at > (int32_t)ary->start) {
      if (FIO_ARRAY_TYPE_CMP(ary->ary[start_at], data))
        return start_at;
      --start_at;
    }
  }
  return -1;
}

/**
 * Removes an object from the array, MOVING all the other objects to prevent
 * "holes" in the data.
 *
 * If `old` is set, the data is copied to the location pointed to by `old`
 * before the data in the array is destroyed.
 *
 * Returns 0 on success and -1 on error.
 */
IFUNC int FIO_NAME(FIO_ARRAY_NAME, remove)(FIO_ARRAY_PTR ary_,
                                           int32_t index,
                                           FIO_ARRAY_TYPE *old) {
  FIO_PTR_TAG_VALID_OR_RETURN(ary_, -1);
  FIO_NAME(FIO_ARRAY_NAME, s) *ary =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
  index += FIO_ARRAY_AB_CT(index >= 0, ary->start, ary->end);
  if (!ary || (uint32_t)index >= ary->end || index < (int32_t)ary->start) {
    FIO_ARRAY_TYPE_COPY(*old, FIO_ARRAY_TYPE_INVALID);
    return -1;
  }
  if (old) {
    FIO_ARRAY_TYPE_COPY(*old, ary->ary[index]);
#if FIO_ARRAY_DESTROY_AFTER_COPY
    FIO_ARRAY_TYPE_DESTROY(ary->ary[index]);
#endif
  } else {
    FIO_ARRAY_TYPE_DESTROY(ary->ary[index]);
  }
  if ((uint32_t)index == ary->start) {
    /* unshift */
    ++ary->start;
  } else {
    /* pop? */
    --ary->end;
    if (ary->end != (uint32_t)index) {
      memmove(ary->ary + index,
              ary->ary + index + 1,
              (ary->end - index) * sizeof(*old));
    }
  }
  return 0;
}

/**
 * Removes all occurrences of an object from the array (if any), MOVING all the
 * existing objects to prevent "holes" in the data.
 *
 * Returns the number of items removed.
 */
IFUNC uint32_t FIO_NAME(FIO_ARRAY_NAME, remove2)(FIO_ARRAY_PTR ary_,
                                                 FIO_ARRAY_TYPE data) {
  FIO_PTR_TAG_VALID_OR_RETURN(ary_, 0);
  FIO_NAME(FIO_ARRAY_NAME, s) *ary =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
  size_t c = 0;
  size_t i = ary->start;
  while (i < ary->end) {
    if (!(FIO_ARRAY_TYPE_CMP(ary->ary[i + c], data))) {
      ary->ary[i] = ary->ary[i + c];
      ++i;
      continue;
    }
    FIO_ARRAY_TYPE_DESTROY(ary->ary[i + c]);
    --ary->end;
    ++c;
  }
  return c;
}

/** Attempts to lower the array's memory consumption. */
IFUNC void FIO_NAME(FIO_ARRAY_NAME, compact)(FIO_ARRAY_PTR ary_) {
  FIO_PTR_TAG_VALID_OR_RETURN_VOID(ary_);
  FIO_NAME(FIO_ARRAY_NAME, s) *ary =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
  FIO_ARRAY_TYPE *tmp = NULL;
  if (!(ary->end - ary->start))
    goto finish;
  tmp = (FIO_ARRAY_TYPE *)FIO_MEM_CALLOC((ary->end - ary->start), sizeof(*tmp));
  if (!tmp)
    return;
  memcpy(
      tmp, ary->ary + ary->start, (ary->end - ary->start) * sizeof(*ary->ary));
finish:
  if (ary->ary) {
    FIO_MEM_FREE_(ary->ary, ary->capa * sizeof(*ary->ary));
  }
  *ary = (FIO_NAME(FIO_ARRAY_NAME, s)){
      .ary = tmp,
      .capa = (ary->end - ary->start),
      .start = 0,
      .end = (ary->end - ary->start),
  };
}

/**
 * Returns a pointer to the C array containing the objects.
 */
IFUNC FIO_ARRAY_TYPE *FIO_NAME2(FIO_ARRAY_NAME, ptr)(FIO_ARRAY_PTR ary_) {
  FIO_PTR_TAG_VALID_OR_RETURN(ary_, (FIO_ARRAY_TYPE *)NULL);
  FIO_NAME(FIO_ARRAY_NAME, s) *ary =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
  return ary->ary + ary->start;
}

/**
 * Pushes an object to the end of the Array. Returns -1 on error.
 */
IFUNC FIO_ARRAY_TYPE *FIO_NAME(FIO_ARRAY_NAME, push)(FIO_ARRAY_PTR ary_,
                                                     FIO_ARRAY_TYPE data) {
  FIO_PTR_TAG_VALID_OR_GOTO(ary_, invalid_ptr);
  {
    FIO_NAME(FIO_ARRAY_NAME, s) *ary =
        (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
    FIO_ARRAY_TYPE *pos;
    if (ary->end >= ary->capa)
      goto needs_memory;
    pos = ary->ary + ary->end;
    *pos = FIO_ARRAY_TYPE_INVALID;
    ++ary->end;
    FIO_ARRAY_TYPE_COPY(*pos, data);
    return pos;
  needs_memory:
    return FIO_NAME(FIO_ARRAY_NAME, set)(ary_, ary->end, data, NULL);
  }
invalid_ptr:
  FIO_ARRAY_TYPE_DESTROY(data);
  return NULL;
}

/**
 * Removes an object from the end of the Array.
 *
 * If `old` is set, the data is copied to the location pointed to by `old`
 * before the data in the array is destroyed.
 *
 * Returns -1 on error (Array is empty) and 0 on success.
 */
IFUNC int FIO_NAME(FIO_ARRAY_NAME, pop)(FIO_ARRAY_PTR ary_,
                                        FIO_ARRAY_TYPE *old) {
  FIO_PTR_TAG_VALID_OR_RETURN(ary_, -1);
  FIO_NAME(FIO_ARRAY_NAME, s) *ary =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
  if (!ary || ary->start == ary->end) {
    FIO_ARRAY_TYPE_COPY(*old, FIO_ARRAY_TYPE_INVALID);
    return -1;
  }
  --ary->end;
  if (old) {
    FIO_ARRAY_TYPE_COPY(*old, ary->ary[ary->end]);
#if FIO_ARRAY_DESTROY_AFTER_COPY
    FIO_ARRAY_TYPE_DESTROY(ary->ary[ary->end]);
#endif
  } else {
    FIO_ARRAY_TYPE_DESTROY(ary->ary[ary->end]);
  }
  return 0;
}

/**
 * Unshifts an object to the beginning of the Array. Returns -1 on error.
 *
 * This could be expensive, causing `memmove`.
 */
IFUNC FIO_ARRAY_TYPE *FIO_NAME(FIO_ARRAY_NAME, unshift)(FIO_ARRAY_PTR ary_,
                                                        FIO_ARRAY_TYPE data) {
  FIO_PTR_TAG_VALID_OR_GOTO(ary_, invalid_ptr);

  {
    FIO_NAME(FIO_ARRAY_NAME, s) *ary =
        (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
    if (ary->start) {
      --ary->start;
      FIO_ARRAY_TYPE *pos = ary->ary + ary->start;
      *pos = FIO_ARRAY_TYPE_INVALID;
      FIO_ARRAY_TYPE_COPY(*pos, data);
      return pos;
    }
    return FIO_NAME(FIO_ARRAY_NAME, set)(ary_, -1 - ary->end, data, NULL);
  }
invalid_ptr:
  FIO_ARRAY_TYPE_DESTROY(data);
  return NULL;
}

/**
 * Removes an object from the beginning of the Array.
 *
 * If `old` is set, the data is copied to the location pointed to by `old`
 * before the data in the array is destroyed.
 *
 * Returns -1 on error (Array is empty) and 0 on success.
 */
IFUNC int FIO_NAME(FIO_ARRAY_NAME, shift)(FIO_ARRAY_PTR ary_,
                                          FIO_ARRAY_TYPE *old) {
  FIO_PTR_TAG_VALID_OR_RETURN(ary_, -1);
  FIO_NAME(FIO_ARRAY_NAME, s) *ary =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
  if (!ary || ary->start == ary->end) {
    FIO_ARRAY_TYPE_COPY(*old, FIO_ARRAY_TYPE_INVALID);
    return -1;
  }
  if (old) {
    FIO_ARRAY_TYPE_COPY(*old, ary->ary[ary->start]);
#if FIO_ARRAY_DESTROY_AFTER_COPY
    FIO_ARRAY_TYPE_DESTROY(ary->ary[ary->start]);
#endif
  } else {
    FIO_ARRAY_TYPE_DESTROY(ary->ary[ary->start]);
  }
  ++ary->start;
  return 0;
}

/**
 * Iteration using a callback for each entry in the array.
 *
 * The callback task function must accept an the entry data as well as an opaque
 * user pointer.
 *
 * If the callback returns -1, the loop is broken. Any other value is ignored.
 *
 * Returns the relative "stop" position, i.e., the number of items processed +
 * the starting point.
 */
IFUNC uint32_t FIO_NAME(FIO_ARRAY_NAME,
                        each)(FIO_ARRAY_PTR ary_,
                              int32_t start_at,
                              int (*task)(FIO_ARRAY_TYPE obj, void *arg),
                              void *arg) {
  FIO_PTR_TAG_VALID_OR_RETURN(ary_, 0);
  FIO_NAME(FIO_ARRAY_NAME, s) *ary =
      (FIO_NAME(FIO_ARRAY_NAME, s) *)(FIO_PTR_UNTAG(ary_));
  if (!ary || !task)
    return start_at;
  if (start_at < 0)
    start_at += ary->end - ary->start;
  if (start_at < 0)
    start_at = 0;
  for (size_t i = ary->start + start_at; i < ary->end; ++i) {
    if (task(ary->ary[i], arg) == -1) {
      return (uint32_t)((i + 1) - ary->start);
    }
  }
  return ary->end - ary->start;
}

/* *****************************************************************************
Dynamic Arrays - cleanup
***************************************************************************** */
#endif /* FIO_EXTERN_COMPLETE */

#undef FIO_ARRAY_NAME
#undef FIO_ARRAY_TYPE
#undef FIO_ARRAY_TYPE_INVALID
#undef FIO_ARRAY_TYPE_INVALID_SIMPLE
#undef FIO_ARRAY_TYPE_COPY
#undef FIO_ARRAY_TYPE_COPY_SIMPLE
#undef FIO_ARRAY_TYPE_DESTROY
#undef FIO_ARRAY_TYPE_DESTROY_SIMPLE
#undef FIO_ARRAY_DESTROY_AFTER_COPY
#undef FIO_ARRAY_TYPE_CMP
#undef FIO_ARRAY_TYPE_CMP_SIMPLE
#undef FIO_ARRAY_TYPE_CONCAT_SIMPLE
#undef FIO_ARRAY_TYPE_CONCAT_COPY
#undef FIO_ARRAY_PADDING
#undef FIO_ARRAY_SIZE2WORDS
#undef FIO_ARRAY_POS2ABS
#undef FIO_ARRAY_AB_CT
#undef FIO_ARRAY_PTR
#undef FIO_ARRAY_EXPONENTIAL
#endif /* FIO_ARRAY_NAME */
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************









                            Hash Maps / Sets




Hash Maps inherently cause memory cache misses, making them slow when they grow
beyond the CPU cache size.

This Map implementation attempts to minimize some of the re-hashing costs
associated with CPU cache misses by keeping the data and the hash values in an
array.


Example - string based map which automatically copies and frees string data:

```c
#define FIO_RISKY_HASH 1 // for hash value computation
#define FIO_ATOL 1       // for string <=> number conversion
#define FIO_MALLOC 1     // using the custom memory allocator
#include "fio-stl.h"

#define FIO_MAP_NAME mstr

#define FIO_MAP_TYPE char *
#define FIO_MAP_TYPE_DESTROY(s) fio_free(s)
#define FIO_MAP_TYPE_COPY(dest, src)                                           \
  do {                                                                         \
    size_t l = sizeof(char) * (strlen(src) + 1);                               \
    dest = fio_malloc(l);                                                      \
    memcpy(dest, src, l);                                                      \
  } while (0)

#define FIO_MAP_KEY char *
#define FIO_MAP_KEY_CMP(a, b) (!strcmp((a), (b)))
#define FIO_MAP_KEY_DESTROY(s) fio_free(s)
#define FIO_MAP_KEY_COPY(dest, src)                                            \
  do {                                                                         \
    size_t l = sizeof(char) * (strlen(src) + 1);                               \
    dest = fio_malloc(l);                                                      \
    memcpy(dest, src, l);                                                      \
  } while (0)

#include "fio-stl.h"

void main(void) {
  mstr_s map = FIO_MAP_INIT;
  for (size_t i = 0; i < 16; ++i) {
    /. create and insert keys
    char key_buf[48];
    char val_buf[48];
    size_t key_len = fio_ltoa(key_buf, i, 2);
    key_buf[key_len] = 0;
    val_buf[fio_ltoa(val_buf, i, 16)] = 0;
    mstr_set(&map, fio_risky_hash(key_buf, key_len, 0), key_buf, val_buf,
                NULL);
  }
  fprintf(stderr, "Mapping binary representation strings to hex:\n");
  FIO_MAP_EACH2(mstr, &map, pos) {
    // print keys in insertion order
    fprintf(stderr, "%s => %s\n", pos->obj.key, pos->obj.value);
  }
  for (size_t i = 15; i < 16; --i) {
    // search keys out of order
    char key_buf[48];
    size_t key_len = fio_ltoa(key_buf, i, 2);
    key_buf[key_len] = 0;
    char *val = mstr_get(&map, fio_risky_hash(key_buf, key_len, 0), key_buf);
    fprintf(stderr, "found %s => %s\n", key_buf, val);
  }
  mstr_destroy(&map); // will automatically free strings
}
```

***************************************************************************** */
#ifdef FIO_MAP_NAME

/* *****************************************************************************
Hash Map / Set - type and hash macros
***************************************************************************** */

#ifndef FIO_MAP_TYPE
/** The type for the elements in the map */
#define FIO_MAP_TYPE void *
/** An invalid value for that type (if any). */
#define FIO_MAP_TYPE_INVALID NULL
#else
#ifndef FIO_MAP_TYPE_INVALID
/** An invalid value for that type (if any). */
#define FIO_MAP_TYPE_INVALID ((FIO_MAP_TYPE){0})
#endif /* FIO_MAP_TYPE_INVALID */
#endif /* FIO_MAP_TYPE */

#ifndef FIO_MAP_TYPE_COPY
/** Handles a copy operation for an value. */
#define FIO_MAP_TYPE_COPY(dest, src) (dest) = (src)
/* internal flag - do not set */
#define FIO_MAP_TYPE_COPY_SIMPLE 1
#endif

#ifndef FIO_MAP_TYPE_DESTROY
/** Handles a destroy / free operation for a map's value. */
#define FIO_MAP_TYPE_DESTROY(obj)
/* internal flag - do not set */
#define FIO_MAP_TYPE_DESTROY_SIMPLE 1
#endif

#ifndef FIO_MAP_TYPE_DISCARD
/** Handles discarded value data (i.e., insert without overwrite). */
#define FIO_MAP_TYPE_DISCARD(obj)
#endif

#ifndef FIO_MAP_TYPE_CMP
/** Handles a comparison operation for a map's value. */
#define FIO_MAP_TYPE_CMP(a, b) 1
#endif

/**
 * The FIO_MAP_DESTROY_AFTER_COPY macro should be set if FIO_MAP_TYPE_DESTROY
 * should be called after FIO_MAP_TYPE_COPY when an object is removed from the
 * array after being copied to an external container (an `old` pointer)
 */
#ifndef FIO_MAP_DESTROY_AFTER_COPY
#if !FIO_MAP_TYPE_DESTROY_SIMPLE && !FIO_MAP_TYPE_COPY_SIMPLE
#define FIO_MAP_DESTROY_AFTER_COPY 1
#else
#define FIO_MAP_DESTROY_AFTER_COPY 0
#endif
#endif /* FIO_MAP_DESTROY_AFTER_COPY */

/** The maximum number of elements allowed before removing old data (FIFO) */
#ifndef FIO_MAP_MAX_ELEMENTS
#define FIO_MAP_MAX_ELEMENTS 0
#endif

/* The maximum number of bins to rotate when (partial/full) collisions occure */
#ifndef FIO_MAP_MAX_SEEK /* LIMITED to 255 */
#define FIO_MAP_MAX_SEEK (96U)
#endif

/* The maximum number of full hash collisions that can be consumed */
#ifndef FIO_MAP_MAX_FULL_COLLISIONS /* LIMITED to 255 */
#define FIO_MAP_MAX_FULL_COLLISIONS (22U)
#endif

/* Prime numbers are better */
#ifndef FIO_MAP_CUCKOO_STEPS
#define FIO_MAP_CUCKOO_STEPS (0x43F82D0B) /* should be a high prime */
#endif

/* Hash to Array optimization limit in log2. MUST be less then 8. */
#ifndef FIO_MAP_SEEK_AS_ARRAY_LOG_LIMIT
#define FIO_MAP_SEEK_AS_ARRAY_LOG_LIMIT 3
#endif

/**
 * Normally, FIO_MAP uses 32bit internal indexing and types.
 *
 * This limits the map to approximately 2 billion items (2,147,483,648).
 * Depending on possible 32 bit hash collisions, more items may be inserted.
 *
 * If FIO_MAP_BIG is be defined, 64 bit addressing is used, increasing the
 * maximum number of items to... hmm... a lot (1 << 63).
 */
#ifdef FIO_MAP_BIG
#define FIO_MAP_SIZE_TYPE uint64_t
#define FIO_MAP_INDEX_USED_BIT ((uint64_t)1 << 63)
#else
#define FIO_MAP_SIZE_TYPE uint32_t
#define FIO_MAP_INDEX_USED_BIT ((uint32_t)1 << 31)
#endif /* FIO_MAP_BIG */

/* the last (-1) index is always reserved, it will make "holes" */
#define FIO_MAP_INDEX_INVALID ((FIO_MAP_SIZE_TYPE)-1)

/* all bytes == 0 means the index was never used */
#define FIO_MAP_INDEX_UNUSED ((FIO_MAP_SIZE_TYPE)0)

#define FIO_MAP_INDEX_CALC(index, hash, index_mask)                            \
  (((hash) & (~(index_mask))) | ((index) & (index_mask)) |                     \
   FIO_MAP_INDEX_USED_BIT)

#ifndef FIO_MAP_HASH
/** The type for map hash value (an X bit integer) */
#define FIO_MAP_HASH uint64_t
#endif

/** An invalid hash value (all bits are zero). */
#define FIO_MAP_HASH_INVALID ((FIO_MAP_HASH)0)

/** tests if the hash value is valid (not reserved). */
#define FIO_MAP_HASH_IS_INVALID(h) ((h) == FIO_MAP_HASH_INVALID)

/** the value to be used when the hash is a reserved value. */
#define FIO_MAP_HASH_FIXED ((FIO_MAP_HASH)-1LL)

/** the value to be used when the hash is a reserved value. */
#define FIO_MAP_HASH_FIX(h)                                                    \
  (FIO_MAP_HASH_IS_INVALID(h) ? FIO_MAP_HASH_FIXED : (h))

/* *****************************************************************************
Map - Hash Map - a Hash Map is basically a couplet Set
***************************************************************************** */
/* Defining a key makes a Hash Map instead of a Set */
#ifdef FIO_MAP_KEY

#ifndef FIO_MAP_KEY_INVALID
/** An invalid value for the hash map key type (if any). */
#define FIO_MAP_KEY_INVALID ((FIO_MAP_KEY){0})
#endif

#ifndef FIO_MAP_KEY_COPY
/** Handles a copy operation for a hash maps key. */
#define FIO_MAP_KEY_COPY(dest, src) (dest) = (src)
#endif

#ifndef FIO_MAP_KEY_DESTROY
/** Handles a destroy / free operation for a hash maps key. */
#define FIO_MAP_KEY_DESTROY(obj)
/* internal flag - do not set */
#define FIO_MAP_KEY_DESTROY_SIMPLE 1
#endif

#ifndef FIO_MAP_KEY_DISCARD
/** Handles discarded element data (i.e., when overwriting only the value). */
#define FIO_MAP_KEY_DISCARD(obj)
#endif

#ifndef FIO_MAP_KEY_CMP
/** Handles a comparison operation for a hash maps key. */
#define FIO_MAP_KEY_CMP(a, b) 1
#endif

typedef struct {
  FIO_MAP_KEY key;
  FIO_MAP_TYPE value;
} FIO_NAME(FIO_MAP_NAME, couplet_s);

FIO_IFUNC void
FIO_NAME(FIO_MAP_NAME, _couplet_copy)(FIO_NAME(FIO_MAP_NAME, couplet_s) * dest,
                                      FIO_NAME(FIO_MAP_NAME, couplet_s) * src) {
  FIO_MAP_KEY_COPY((dest->key), (src->key));
  FIO_MAP_TYPE_COPY((dest->value), (src->value));
}

FIO_IFUNC void FIO_NAME(FIO_MAP_NAME,
                        _couplet_destroy)(FIO_NAME(FIO_MAP_NAME, couplet_s) *
                                          c) {
  FIO_MAP_KEY_DESTROY(c->key);
  FIO_MAP_TYPE_DESTROY(c->value);
  (void)c; /* in case where macros do nothing */
}

/** FIO_MAP_OBJ is either a couplet (for hash maps) or the objet (for sets) */
#define FIO_MAP_OBJ FIO_NAME(FIO_MAP_NAME, couplet_s)

/** FIO_MAP_OBJ_KEY is FIO_MAP_KEY for hash maps or FIO_MAP_TYPE for sets */
#define FIO_MAP_OBJ_KEY FIO_MAP_KEY

#define FIO_MAP_OBJ_INVALID                                                    \
  ((FIO_NAME(FIO_MAP_NAME, couplet_s)){.key = FIO_MAP_KEY_INVALID,             \
                                       .value = FIO_MAP_TYPE_INVALID})

#define FIO_MAP_OBJ_COPY(dest, src)                                            \
  FIO_NAME(FIO_MAP_NAME, _couplet_copy)(&(dest), &(src))

#define FIO_MAP_OBJ_DESTROY(obj)                                               \
  FIO_NAME(FIO_MAP_NAME, _couplet_destroy)(&(obj))

#define FIO_MAP_OBJ_CMP(a, b) FIO_MAP_KEY_CMP((a).key, (b).key)
#define FIO_MAP_OBJ_KEY_CMP(a, key_) FIO_MAP_KEY_CMP((a).key, (key_))
#define FIO_MAP_OBJ2KEY(o) (o).key
#define FIO_MAP_OBJ2TYPE(o) (o).value

#define FIO_MAP_OBJ_DISCARD(o)                                                 \
  do {                                                                         \
    FIO_MAP_TYPE_DISCARD(((o).value));                                         \
    FIO_MAP_KEY_DISCARD(((o).key));                                            \
  } while (0);

#if FIO_MAP_DESTROY_AFTER_COPY
#define FIO_MAP_OBJ_DESTROY_AFTER FIO_MAP_OBJ_DESTROY
#else
#define FIO_MAP_OBJ_DESTROY_AFTER(obj) FIO_MAP_KEY_DESTROY((obj).key);
#endif /* FIO_MAP_DESTROY_AFTER_COPY */

/* *****************************************************************************
Map - Set
***************************************************************************** */
#else /* FIO_MAP_KEY */
/** FIO_MAP_OBJ is either a couplet (for hash maps) or the objet (for sets) */
#define FIO_MAP_OBJ FIO_MAP_TYPE
/** FIO_MAP_OBJ_KEY is FIO_MAP_KEY for hash maps or FIO_MAP_TYPE for sets */
#define FIO_MAP_OBJ_KEY FIO_MAP_TYPE
#define FIO_MAP_OBJ_INVALID FIO_MAP_TYPE_INVALID
#define FIO_MAP_OBJ_COPY FIO_MAP_TYPE_COPY
#define FIO_MAP_OBJ_DESTROY FIO_MAP_TYPE_DESTROY
#define FIO_MAP_OBJ_CMP FIO_MAP_TYPE_CMP
#define FIO_MAP_OBJ_KEY_CMP FIO_MAP_TYPE_CMP
#define FIO_MAP_OBJ2KEY(o) (o)
#define FIO_MAP_OBJ2TYPE(o) (o)
#define FIO_MAP_OBJ_DISCARD FIO_MAP_TYPE_DISCARD
#define FIO_MAP_KEY_DISCARD(_ignore)
#if FIO_MAP_DESTROY_AFTER_COPY
#define FIO_MAP_OBJ_DESTROY_AFTER FIO_MAP_TYPE_DESTROY
#else
#define FIO_MAP_OBJ_DESTROY_AFTER(obj)
#endif /* FIO_MAP_DESTROY_AFTER_COPY */

#endif /* FIO_MAP_KEY */

/* *****************************************************************************
Hash Map / Set - types
***************************************************************************** */

typedef struct {
  FIO_MAP_HASH hash;
  FIO_MAP_OBJ obj;
} FIO_NAME(FIO_MAP_NAME, each_s);

typedef struct {
  FIO_NAME(FIO_MAP_NAME, each_s) * map;
  FIO_MAP_SIZE_TYPE count;
  FIO_MAP_SIZE_TYPE w; /* writing position */
  uint8_t bits;
  uint8_t under_attack;
} FIO_NAME(FIO_MAP_NAME, s);

#define FIO_MAP_S FIO_NAME(FIO_MAP_NAME, s)

#ifdef FIO_PTR_TAG_TYPE
#define FIO_MAP_PTR FIO_PTR_TAG_TYPE
#else
#define FIO_MAP_PTR FIO_NAME(FIO_MAP_NAME, s) *
#endif

/* *****************************************************************************
Hash Map / Set - API (initialization)
***************************************************************************** */

#ifndef FIO_MAP_INIT
/* Initialization macro. */
#define FIO_MAP_INIT                                                           \
  { 0 }
#endif

#ifndef FIO_REF_CONSTRUCTOR_ONLY
/**
 * Allocates a new map on the heap.
 */
FIO_IFUNC FIO_MAP_PTR FIO_NAME(FIO_MAP_NAME, new)(void);

/**
 * Frees a map that was allocated on the heap.
 */
FIO_IFUNC void FIO_NAME(FIO_MAP_NAME, free)(FIO_MAP_PTR m);

#endif /* FIO_REF_CONSTRUCTOR_ONLY */

/**
 * Empties the Map, keeping the current resources and memory for future use.
 */
FIO_IFUNC void FIO_NAME(FIO_MAP_NAME, clear)(FIO_MAP_PTR m);

/**
 * Destroys the map's internal data and re-initializes it.
 */
FIO_IFUNC void FIO_NAME(FIO_MAP_NAME, destroy)(FIO_MAP_PTR m);

/* *****************************************************************************
Hash Map / Set - API (set/get)
***************************************************************************** */

/** Returns the object in the hash map (if any) or FIO_MAP_TYPE_INVALID. */
FIO_IFUNC FIO_MAP_TYPE FIO_NAME(FIO_MAP_NAME, get)(FIO_MAP_PTR m,
                                                   FIO_MAP_HASH hash,
                                                   FIO_MAP_OBJ_KEY key);

/** Returns a pointer to the object in the hash map (if any) or NULL. */
FIO_IFUNC FIO_MAP_TYPE *FIO_NAME(FIO_MAP_NAME, get_ptr)(FIO_MAP_PTR m,
                                                        FIO_MAP_HASH hash,
                                                        FIO_MAP_OBJ_KEY key);
/**
 * Inserts an object to the hash map, returning the new object.
 *
 * If `old` is given, existing data will be copied to that location.
 */
FIO_IFUNC FIO_MAP_TYPE FIO_NAME(FIO_MAP_NAME, set)(FIO_MAP_PTR m,
                                                   FIO_MAP_HASH hash,
#ifdef FIO_MAP_KEY
                                                   FIO_MAP_OBJ_KEY key,
#endif /* FIO_MAP_KEY */
                                                   FIO_MAP_TYPE obj,
                                                   FIO_MAP_TYPE *old);

/**
 * Removes an object from the hash map.
 *
 * If `old` is given, existing data will be copied to that location.
 *
 * Returns 0 on success or -1 if the object couldn't be found.
 */
SFUNC int FIO_NAME(FIO_MAP_NAME, remove)(FIO_MAP_PTR m,
                                         FIO_MAP_HASH hash,
                                         FIO_MAP_OBJ_KEY key,
                                         FIO_MAP_TYPE *old);

/**
 * Inserts an object to the hash map, returning the existing or new object.
 */
FIO_IFUNC FIO_MAP_TYPE FIO_NAME(FIO_MAP_NAME,
                                set_if_missing)(FIO_MAP_PTR m,
                                                FIO_MAP_HASH hash,
#ifdef FIO_MAP_KEY
                                                FIO_MAP_OBJ_KEY key,
#endif /* FIO_MAP_KEY */
                                                FIO_MAP_TYPE obj);

/* *****************************************************************************
Hash Map / Set - API (misc)
***************************************************************************** */

/** Returns the number of objects in the map. */
FIO_IFUNC FIO_MAP_SIZE_TYPE FIO_NAME(FIO_MAP_NAME, count)(FIO_MAP_PTR m);

/** Returns the current map's theoretical capacity. */
FIO_IFUNC FIO_MAP_SIZE_TYPE FIO_NAME(FIO_MAP_NAME, capa)(FIO_MAP_PTR m);

/** Reserves a minimal capacity for the hash map, might reserve more. */
FIO_IFUNC FIO_MAP_SIZE_TYPE FIO_NAME(FIO_MAP_NAME,
                                     reserve)(FIO_MAP_PTR m,
                                              FIO_MAP_SIZE_TYPE capa);

/** Rehashes the Hash Map / Set. Usually this is performed automatically. */
FIO_IFUNC int FIO_NAME(FIO_MAP_NAME, rehash)(FIO_MAP_PTR m);

/** Attempts to lower the map's memory consumption. */
FIO_IFUNC int FIO_NAME(FIO_MAP_NAME, compact)(FIO_MAP_PTR m);

/* *****************************************************************************
Hash Map / Set - API (iterration)
***************************************************************************** */

/**
 * Returns a pointer to the (next) object's information in the map.
 *
 * To access the object information, use:
 *
 *    MAP_each_s * pos = MAP_each_next(map, NULL);
 *
 * - `i->hash` to access the hash value.
 *
 * - `i->obj` to access the object's data.
 *
 *    For Hash Maps, use `i->obj.key` and `i->obj.value`.
 *
 * Returns the first object if `pos == NULL` and there are objects in the map.
 *
 * Returns the next object if `pos` is valid.
 *
 * Returns NULL if `pos` was the last object or no object exist.
 *
 */
FIO_IFUNC FIO_NAME(FIO_MAP_NAME, each_s) *
    FIO_NAME(FIO_MAP_NAME, each_next)(FIO_MAP_PTR m,
                                      FIO_NAME(FIO_MAP_NAME, each_s) * pos);

#ifndef FIO_MAP_EACH
/**
 * A macro for a `for` loop that iterates over all the Map's objects (in
 * order).
 *
 * Use this macro for small Hash Maps / Sets.
 *
 * - `map_p` is a pointer to the Hash Map / Set variable.
 *
 * - `pos` is a temporary variable name to be created for iteration. This
 *    variable may SHADOW external variables, be aware.
 *
 * To access the object information, use:
 *
 * - `pos->hash` to access the hash value.
 *
 * - `pos->obj` to access the object's data.
 *
 *    For Hash Maps, use `pos->obj.key` and `pos->obj.value`.
 *
 *
 * Each loop **SHOULD** test for a valid object using (unlike FIO_MAP_EACH2):
 *
 *      if (!pos->hash) continue;
 *
 */
#define FIO_MAP_EACH(map_p, pos)                                               \
  for (__typeof__((map_p)->map) pos = (map_p)->map,                            \
                                end__ = (map_p)->map + (map_p)->w;             \
       pos < end__;                                                            \
       ++pos)
#endif

#ifndef FIO_MAP_EACH2
/**
 * A macro for a `for` loop that iterates over all the Map's objects (in
 * order).
 *
 * Use this macro for small Hash Maps / Sets.
 *
 * - `map_type` is the Map's type name/function prefix, same as FIO_MAP_NAME.
 *
 * - `map_p` is a pointer to the Hash Map / Set variable.
 *
 * - `pos` is a temporary variable name to be created for iteration. This
 *    variable may SHADOW external variables, be aware.
 *
 * To access the object information, use:
 *
 * - `pos->hash` to access the hash value.
 *
 * - `pos->obj` to access the object's data.
 *
 *    For Hash Maps, use `pos->obj.key` and `pos->obj.value`.
 */
#define FIO_MAP_EACH2(map_type, map_p, pos)                                    \
  for (FIO_NAME(map_type, each_s) *pos =                                       \
           FIO_NAME(map_type, each_next)(map_p, NULL);                         \
       pos;                                                                    \
       pos = FIO_NAME(map_type, each_next)(map_p, pos))
#endif

/**
 * Iteration using a callback for each element in the map.
 *
 * The callback task function must accept an element variable as well as an
 * opaque user pointer.
 *
 * If the callback returns -1, the loop is broken. Any other value is ignored.
 *
 * Returns the relative "stop" position, i.e., the number of items processed +
 * the starting point.
 */
SFUNC FIO_MAP_SIZE_TYPE FIO_NAME(FIO_MAP_NAME,
                                 each)(FIO_MAP_PTR m,
                                       ssize_t start_at,
                                       int (*task)(FIO_MAP_TYPE obj, void *arg),
                                       void *arg);

#ifdef FIO_MAP_KEY
/**
 * Returns the current `key` within an `each` task.
 *
 * Only available within an `each` loop.
 *
 * For sets, returns the hash value, for hash maps, returns the key value.
 */
SFUNC FIO_MAP_KEY FIO_NAME(FIO_MAP_NAME, each_get_key)(void);
#else
/**
 * Returns the current `key` within an `each` task.
 *
 * Only available within an `each` loop.
 *
 * For sets, returns the hash value, for hash maps, returns the key value.
 */
SFUNC FIO_MAP_HASH FIO_NAME(FIO_MAP_NAME, each_get_key)(void);
#endif

/* *****************************************************************************





Hash Map / Set - Implementation - INLINE





***************************************************************************** */

/* *****************************************************************************
Hash Map / Set - Internal API (Helpers)
***************************************************************************** */

/* INTERNAL helper: computes a map's capacity according to the bits. */
#define FIO_MAP_CAPA(bits) (((FIO_MAP_SIZE_TYPE)1 << bits) - 1)

/* INTERNAL returns the required memory size in bytes. */
FIO_IFUNC size_t FIO_NAME(FIO_MAP_NAME, __byte_size)(uint8_t bits);

/* INTERNAL reduces the hash's bit-width . */
FIO_IFUNC FIO_MAP_SIZE_TYPE FIO_NAME(FIO_MAP_NAME,
                                     __hash2index)(FIO_MAP_HASH hash,
                                                   uint8_t bits);

/* INTERNAL helper: destroys each object in the map. */
FIO_IFUNC void FIO_NAME(FIO_MAP_NAME, __destroy_each_entry)(FIO_MAP_S *m);

typedef struct {
  FIO_MAP_SIZE_TYPE i;
  FIO_MAP_SIZE_TYPE imap;
} FIO_NAME(FIO_MAP_NAME, __pos_s);

/** INTERNAL: returns position information (potential / existing). */
SFUNC FIO_NAME(FIO_MAP_NAME, __pos_s)
    FIO_NAME(FIO_MAP_NAME, __get_pos)(FIO_MAP_S *m,
                                      FIO_MAP_HASH fixed_hash,
                                      FIO_MAP_SIZE_TYPE index_hash,
                                      FIO_MAP_OBJ_KEY key);

/** INTERNAL: sets an object in the map. */
SFUNC FIO_MAP_TYPE *FIO_NAME(FIO_MAP_NAME, __set)(FIO_MAP_S *m,
                                                  FIO_MAP_HASH hash,
#ifdef FIO_MAP_KEY
                                                  FIO_MAP_OBJ_KEY key,
#endif /* FIO_MAP_KEY */
                                                  FIO_MAP_TYPE obj,
                                                  FIO_MAP_TYPE *old,
                                                  uint8_t overwrite);

/** INTERNAL: rehashes a hash map where all the map's bytes are set to zero. */
SFUNC int FIO_NAME(FIO_MAP_NAME, __rehash_router)(FIO_MAP_S *m);

/** INTERNAL: reserves (at least) the requested capacity of objects. */
FIO_MAP_SIZE_TYPE FIO_NAME(FIO_MAP_NAME, __reserve)(FIO_MAP_S *m,
                                                    FIO_MAP_SIZE_TYPE capa);

/** INTERNAL: reallocates the map's memory (without rehashing). */
SFUNC int FIO_NAME(FIO_MAP_NAME, __map_realloc)(FIO_NAME(FIO_MAP_NAME, s) * m,
                                                uint8_t bits);

/** Internal: returns the untagged pointer or NULL. */
FIO_IFUNC const FIO_NAME(FIO_MAP_NAME, s) *
    FIO_NAME(FIO_MAP_NAME, __untag)(FIO_MAP_PTR m_);

/** Internal: returns the internal index map. */
FIO_IFUNC FIO_MAP_SIZE_TYPE *FIO_NAME(FIO_MAP_NAME, __imap)(FIO_MAP_S *m);

/* *****************************************************************************
Hash Map / Set - Internal API (Helpers) - INLINE
***************************************************************************** */

/* INTERNAL returns the required memory size in bytes. */
FIO_IFUNC size_t FIO_NAME(FIO_MAP_NAME, __byte_size)(uint8_t bits) {
  size_t r = 1;
  r <<= bits;
  return ((r * sizeof(FIO_MAP_SIZE_TYPE)) +
          ((r - 1) * sizeof(FIO_NAME(FIO_MAP_NAME, each_s))));
}

/* INTERNAL reduces the hash's bit-width . */
FIO_IFUNC FIO_MAP_SIZE_TYPE FIO_NAME(FIO_MAP_NAME,
                                     __hash2index)(FIO_MAP_HASH hash,
                                                   uint8_t bits) {
  return (((hash) ^ (((hash)*FIO_MAP_CUCKOO_STEPS) >> (63 & (bits)))) |
          FIO_MAP_INDEX_USED_BIT);
}

FIO_IFUNC void FIO_NAME(FIO_MAP_NAME, __destroy_each_entry)(FIO_MAP_S *m) {
#if !FIO_MAP_TYPE_DESTROY_SIMPLE || !FIO_MAP_KEY_DESTROY_SIMPLE
  if (m->w == m->count) {
    for (FIO_NAME(FIO_MAP_NAME, each_s) *pos = m->map, *end_ = m->map + m->w;
         pos < end_;
         ++pos) {
      FIO_MAP_OBJ_DESTROY(pos->obj);
    }
  } else {
    for (FIO_NAME(FIO_MAP_NAME, each_s) *pos = m->map, *end_ = m->map + m->w;
         pos < end_;
         ++pos) {
      if (!pos->hash)
        continue;
      FIO_MAP_OBJ_DESTROY(pos->obj);
    }
  }
#endif
  (void)m; /* possible NOOP */
}

/** Internal: returns the untagged pointer or NULL. */
FIO_IFUNC const FIO_NAME(FIO_MAP_NAME, s) *
    FIO_NAME(FIO_MAP_NAME, __untag)(FIO_MAP_PTR m_) {
  return (FIO_MAP_S *)FIO_PTR_UNTAG(m_);
}

/** Internal: returns the internal index map. */
FIO_IFUNC FIO_MAP_SIZE_TYPE *FIO_NAME(FIO_MAP_NAME, __imap)(FIO_MAP_S *m) {
  FIO_MAP_SIZE_TYPE *r;
  const FIO_MAP_SIZE_TYPE capa = FIO_MAP_CAPA(m->bits);
  r = (FIO_MAP_SIZE_TYPE *)(m->map + capa);
  return r;
}

/* *****************************************************************************
Hash Map / Set - API (initialization inlined)
***************************************************************************** */

#ifndef FIO_REF_CONSTRUCTOR_ONLY
/**
 * Allocates a new map on the heap.
 */
FIO_IFUNC FIO_MAP_PTR FIO_NAME(FIO_MAP_NAME, new)(void) {
  FIO_MAP_PTR r;
  FIO_MAP_S *const m = (FIO_MAP_S *)FIO_MEM_CALLOC_(sizeof(*m), 1);
  // no need to initialize the map object, since all bytes are zero.
  r = (FIO_MAP_PTR)FIO_PTR_TAG(m);
  return r;
}

/**
 * Frees a map that was allocated on the heap.
 */
FIO_IFUNC void FIO_NAME(FIO_MAP_NAME, free)(FIO_MAP_PTR m_) {
  FIO_MAP_S *const m = (FIO_MAP_S *)FIO_PTR_UNTAG(m_);
  if (!m || !m_)
    return;
  FIO_NAME(FIO_MAP_NAME, destroy)(m);
  FIO_MEM_FREE_(m, sizeof(*m));
}

#endif /* FIO_REF_CONSTRUCTOR_ONLY */

/**
 * Empties the Map, keeping the current resources and memory for future use.
 */
FIO_IFUNC void FIO_NAME(FIO_MAP_NAME, clear)(FIO_MAP_PTR m_) {
  FIO_MAP_S *const m = (FIO_MAP_S *)FIO_PTR_UNTAG(m_);
  if (!m || !m_)
    return;
  if (m->map) {
    FIO_NAME(FIO_MAP_NAME, __destroy_each_entry)(m);
    memset(m->map, 0, FIO_NAME(FIO_MAP_NAME, __byte_size)(m->bits));
  }
  *m = (FIO_MAP_S){.map = m->map};
}

/**
 * Destroys the map's internal data and re-initializes it.
 */
FIO_IFUNC void FIO_NAME(FIO_MAP_NAME, destroy)(FIO_MAP_PTR m_) {
  FIO_MAP_S *const m = (FIO_MAP_S *)FIO_PTR_UNTAG(m_);
  if (!m || !m_)
    return;
  if (m->map) {
    FIO_NAME(FIO_MAP_NAME, __destroy_each_entry)(m);
    FIO_MEM_FREE_(m->map, FIO_NAME(FIO_MAP_NAME, __byte_size)(m->bits));
  }
  *m = (FIO_MAP_S){0};
}

/* *****************************************************************************
Hash Map / Set - API (set/get) inlined
***************************************************************************** */

/** Returns the object in the hash map (if any) or FIO_MAP_TYPE_INVALID. */
FIO_IFUNC FIO_MAP_TYPE FIO_NAME(FIO_MAP_NAME, get)(FIO_MAP_PTR m_,
                                                   FIO_MAP_HASH hash,
                                                   FIO_MAP_OBJ_KEY key) {
  FIO_MAP_S *const m = (FIO_MAP_S *)FIO_PTR_UNTAG(m_);
  if (!m || !m_)
    return FIO_MAP_TYPE_INVALID;
  hash = FIO_MAP_HASH_FIX(hash);
  const FIO_MAP_SIZE_TYPE ihash =
      FIO_NAME(FIO_MAP_NAME, __hash2index)(hash, m->bits);
  FIO_NAME(FIO_MAP_NAME, __pos_s)
  i = FIO_NAME(FIO_MAP_NAME, __get_pos)(m, hash, ihash, key);
  if (i.i != FIO_MAP_INDEX_INVALID)
    return FIO_MAP_OBJ2TYPE(m->map[i.i].obj);
  return FIO_MAP_TYPE_INVALID;
}

/** Returns a pointer to the object in the hash map (if any) or NULL. */
FIO_IFUNC FIO_MAP_TYPE *FIO_NAME(FIO_MAP_NAME, get_ptr)(FIO_MAP_PTR m_,
                                                        FIO_MAP_HASH hash,
                                                        FIO_MAP_OBJ_KEY key) {
  FIO_MAP_S *const m = (FIO_MAP_S *)FIO_PTR_UNTAG(m_);
  if (!m || !m_)
    return NULL;
  hash = FIO_MAP_HASH_FIX(hash);
  const FIO_MAP_SIZE_TYPE ihash =
      FIO_NAME(FIO_MAP_NAME, __hash2index)(hash, m->bits);
  FIO_NAME(FIO_MAP_NAME, __pos_s)
  i = FIO_NAME(FIO_MAP_NAME, __get_pos)(m, hash, ihash, key);
  if (i.i != FIO_MAP_INDEX_INVALID)
    return &FIO_MAP_OBJ2TYPE(m->map[i.i].obj);
  return NULL;
}

/**
 * Inserts an object to the hash map, returning the new object.
 *
 * If `old` is given, existing data will be copied to that location.
 */
FIO_IFUNC FIO_MAP_TYPE FIO_NAME(FIO_MAP_NAME, set)(FIO_MAP_PTR m_,
                                                   FIO_MAP_HASH hash,
#ifdef FIO_MAP_KEY
                                                   FIO_MAP_OBJ_KEY key,
#endif /* FIO_MAP_KEY */
                                                   FIO_MAP_TYPE obj,
                                                   FIO_MAP_TYPE *old) {
  FIO_MAP_TYPE *p;
  FIO_MAP_S *const m = (FIO_MAP_S *)FIO_PTR_UNTAG(m_);
  if (!m || !m_)
    return FIO_MAP_TYPE_INVALID;
  p = FIO_NAME(FIO_MAP_NAME, __set)(m,
                                    hash,
#ifdef FIO_MAP_KEY
                                    key,
#endif /* FIO_MAP_KEY */
                                    obj,
                                    old,
                                    1);

  if (p)
    return *p;
  return FIO_MAP_TYPE_INVALID;
}

/**
 * Inserts an object to the hash map, returning the existing or new object.
 */
FIO_IFUNC FIO_MAP_TYPE FIO_NAME(FIO_MAP_NAME,
                                set_if_missing)(FIO_MAP_PTR m_,
                                                FIO_MAP_HASH hash,
#ifdef FIO_MAP_KEY
                                                FIO_MAP_OBJ_KEY key,
#endif /* FIO_MAP_KEY */
                                                FIO_MAP_TYPE obj) {
  FIO_MAP_TYPE *p;
  FIO_MAP_S *const m = (FIO_MAP_S *)FIO_PTR_UNTAG(m_);
  if (!m || !m_)
    return FIO_MAP_TYPE_INVALID;

  p = FIO_NAME(FIO_MAP_NAME, __set)(m,
                                    hash,
#ifdef FIO_MAP_KEY
                                    key,
#endif /* FIO_MAP_KEY */
                                    obj,
                                    NULL,
                                    0);
  if (p)
    return *p;
  return FIO_MAP_TYPE_INVALID;
}

/* *****************************************************************************
Hash Map / Set - API (inlined)
***************************************************************************** */

/** Returns the number of objects in the map. */
FIO_IFUNC FIO_MAP_SIZE_TYPE FIO_NAME(FIO_MAP_NAME, count)(FIO_MAP_PTR m_) {
  FIO_MAP_S *const m = (FIO_MAP_S *)FIO_PTR_UNTAG(m_);
  if (!m || !m_)
    return 0;
  return m->count;
}

/** Returns the current map's theoretical capacity. */
FIO_IFUNC FIO_MAP_SIZE_TYPE FIO_NAME(FIO_MAP_NAME, capa)(FIO_MAP_PTR m_) {
  FIO_MAP_S *const m = (FIO_MAP_S *)FIO_PTR_UNTAG(m_);
  if (!m || !m->map || !m->bits)
    return 0;
  return FIO_MAP_CAPA(m->bits);
}

/** Reserves a minimal capacity for the hash map. */
FIO_IFUNC FIO_MAP_SIZE_TYPE FIO_NAME(FIO_MAP_NAME,
                                     reserve)(FIO_MAP_PTR m_,
                                              FIO_MAP_SIZE_TYPE capa) {
  FIO_MAP_S *const m = (FIO_MAP_S *)FIO_PTR_UNTAG(m_);
  if (!m || !m_)
    return 0;
  uint8_t bits = 2;
  if (capa == (FIO_MAP_SIZE_TYPE)-1)
    return FIO_MAP_INDEX_INVALID;
  while (((size_t)1 << bits) <= capa)
    ++bits;
  if (FIO_NAME(FIO_MAP_NAME, __map_realloc)(m, bits) ||
      FIO_NAME(FIO_MAP_NAME, __rehash_router)(m))
    return FIO_MAP_INDEX_INVALID;
  return FIO_MAP_CAPA(m->bits);
}

/** Rehashes the Hash Map / Set. Usually this is performed automatically. */
FIO_IFUNC int FIO_NAME(FIO_MAP_NAME, rehash)(FIO_MAP_PTR m_) {
  FIO_MAP_S *const m = (FIO_MAP_S *)FIO_PTR_UNTAG(m_);
  if (!m || !m_ || !m->map || !m->bits)
    return 0;
  return FIO_NAME(FIO_MAP_NAME, __rehash_router)(m);
}

/** Attempts to lower the map's memory consumption. */
FIO_IFUNC int FIO_NAME(FIO_MAP_NAME, compact)(FIO_MAP_PTR m_) {
  int r = 0;
  FIO_MAP_S *const m = (FIO_MAP_S *)FIO_PTR_UNTAG(m_);
  if (!m || !m->map || !m->bits)
    return r;
  uint8_t bits = 1;
  while ((FIO_MAP_SIZE_TYPE)1 << bits <= m->count)
    ++bits;
  r = FIO_NAME(FIO_MAP_NAME, __map_realloc)(m, bits);
  r |= FIO_NAME(FIO_MAP_NAME, __rehash_router)(m);
  return r;
}

/** Returns a pointer to the (next) object's information in the map. */
FIO_IFUNC FIO_NAME(FIO_MAP_NAME, each_s) *
    FIO_NAME(FIO_MAP_NAME, each_next)(FIO_MAP_PTR m_,
                                      FIO_NAME(FIO_MAP_NAME, each_s) * pos) {
  FIO_MAP_S *const m = (FIO_MAP_S *)FIO_PTR_UNTAG(m_);
  if (!m || !m_)
    return NULL;
  if (!pos)
    pos = m->map - 1;
  for (;;) {
    ++pos;
    if (pos >= m->map + m->w)
      return NULL;
    if (pos->hash)
      return pos;
  }
}

/* *****************************************************************************



Hash Map / Set - Implementation



***************************************************************************** */
#ifdef FIO_EXTERN_COMPLETE

/* *****************************************************************************
Hash Map / Set - Internal API (Helpers) - Internal Memory Management
***************************************************************************** */

/** Internal: removes "holes" from the internal array. */
FIO_IFUNC void FIO_NAME(FIO_MAP_NAME, __compact_forced)(FIO_MAP_S *m,
                                                        uint8_t zero) {
  FIO_MAP_SIZE_TYPE r = 0, w = 0;
  /* consume continuous data */
  while (r < m->w && m->map[r].hash)
    ++r;
  /* fill holes in array */
  for (w = (r++); r < m->w; ++r) {
    if (!m->map[r].hash)
      continue;
    if (r + m->count == m->w + w && r + 4 < m->w) {
      /* filled last hole (optimized?) */
      memmove(m->map + w, m->map + r, sizeof(*m->map) * (m->w - r));
      w += (m->w - r);
      break;
    }
    m->map[w++] = m->map[r];
  }
  m->w = w;
  FIO_ASSERT_DEBUG(m->w == m->count, "implementation error?");

  if (zero) {
    FIO_ASSERT_DEBUG(FIO_NAME(FIO_MAP_NAME, __byte_size)(m->bits) >
                         (sizeof(*m->map) * m->w),
                     "always true");
    memset(m->map + w,
           0,
           FIO_NAME(FIO_MAP_NAME, __byte_size)(m->bits) -
               (sizeof(*m->map) * m->w));
  }
}

/** Internal: reallocates the map's memory, zeroing out as needed. */
SFUNC int FIO_NAME(FIO_MAP_NAME, __map_realloc)(FIO_NAME(FIO_MAP_NAME, s) * m,
                                                uint8_t bits) {
  if (bits >= (sizeof(FIO_MAP_SIZE_TYPE) << 3))
    return -1;
  if (bits < 2)
    bits = 2;
  const size_t old_capa = FIO_MAP_CAPA(m->bits);
  const size_t new_capa = FIO_MAP_CAPA(bits);
  if (new_capa != old_capa) {
    if (new_capa < m->count) {
      /* not enough room for existing items - recall function with valid value
       */
      while (((size_t)1 << bits) <= m->count)
        ++bits;
      return FIO_NAME(FIO_MAP_NAME, __map_realloc)(m, bits);
    }
    if (new_capa <= m->w) {
      /* we need to compact the array (remove holes) before reallocating */
      FIO_NAME(FIO_MAP_NAME, __compact_forced)(m, 0);
    }
    FIO_NAME(FIO_MAP_NAME, each_s) *tmp =
        (FIO_NAME(FIO_MAP_NAME, each_s) *)FIO_MEM_REALLOC_(
            m->map,
            FIO_NAME(FIO_MAP_NAME, __byte_size)(m->bits),
            FIO_NAME(FIO_MAP_NAME, __byte_size)(bits),
            m->w * sizeof(*m->map));
    if (!tmp)
      return -1;
    m->map = tmp;
    m->bits = bits;
  }
  FIO_MAP_SIZE_TYPE *imap = FIO_NAME(FIO_MAP_NAME, __imap)(m);
  if (new_capa > old_capa) {
    if (!FIO_MEM_INTERNAL_MALLOC_) {
      /* realloc might return recycled (junk filled) memory */
      memset(imap, 0, ((size_t)1 << m->bits) * sizeof(FIO_MAP_SIZE_TYPE));
    } else if ((sizeof(*m->map) & 15) && ((uintptr_t)imap & 15)) {
      /* fio_realloc2 might copy junk data in the last 15 bytes (big word) */
      memset(imap, 0, ((uintptr_t)imap & 15));
    }
  } else {
    /* when shrinking (or staying), we might have junk data by design... */
    memset(imap, 0, ((size_t)1 << m->bits) * sizeof(FIO_MAP_SIZE_TYPE));
  }
  return 0;
}

/** Internal: frees the map's memory. */
FIO_IFUNC void FIO_NAME(FIO_MAP_NAME,
                        __map_free_map)(FIO_NAME(FIO_MAP_NAME, each_s) * map,
                                        uint8_t bits) {
  const size_t old_size = (1 << bits) - 1;
  FIO_MEM_FREE_(map,
                (old_size * (sizeof(*map)) +
                 (((size_t)old_size + 1) * sizeof(FIO_MAP_SIZE_TYPE))));
  (void)old_size; /* if unused */
}

/* *****************************************************************************
Hash Map / Set - Internal API (Helpers) - Map Positioning
***************************************************************************** */

/** INTERNAL: returns position information (potential / existing). */
SFUNC FIO_NAME(FIO_MAP_NAME, __pos_s)
    FIO_NAME(FIO_MAP_NAME, __get_pos)(FIO_MAP_S *m,
                                      FIO_MAP_HASH hash,
                                      FIO_MAP_SIZE_TYPE ihash,
                                      FIO_MAP_OBJ_KEY key) {
  const size_t imask = ((FIO_MAP_SIZE_TYPE)1 << m->bits) - 1;
  const size_t test_mask = ~imask;
  FIO_MAP_SIZE_TYPE *imap = FIO_NAME(FIO_MAP_NAME, __imap)(m);
  FIO_NAME(FIO_MAP_NAME, __pos_s)
  r = {.i = FIO_MAP_INDEX_INVALID, .imap = FIO_MAP_INDEX_INVALID};
  size_t i = ihash;
  if (!m->map)
    return r;
  ihash &= test_mask;
  uint8_t full_collisions = 0;
  unsigned int attempts = (imask < FIO_MAP_MAX_SEEK) ? imask : FIO_MAP_MAX_SEEK;
  if (FIO_MAP_SEEK_AS_ARRAY_LOG_LIMIT >= m->bits)
    goto seek_as_array;
  do {
    i &= imask;
    if (!imap[i]) {
      /* unused empty slot */
      if (r.imap == FIO_MAP_INDEX_INVALID)
        r.imap = i; /* update returned index only if no previous index exits */
      return r;
    }
    if (imap[i] == FIO_MAP_INDEX_INVALID) {
      /* known hole, could be filled by `__set` */
      if (r.imap == FIO_MAP_INDEX_INVALID)
        r.imap = i; /* update returned index only if no previous index exits */
    } else if ((imap[i] & test_mask) == ihash &&
               m->map[(imap[i] & imask)].hash == hash) {
      /* potential hit */
      if (m->under_attack ||
          FIO_MAP_OBJ_KEY_CMP(m->map[(imap[i] & imask)].obj, key)) {
        /* object found */
        r = (FIO_NAME(FIO_MAP_NAME, __pos_s)){
            .i = (FIO_MAP_SIZE_TYPE)(imap[i] & imask),
            .imap = (FIO_MAP_SIZE_TYPE)i,
        };
        return r;
      }
      if (++full_collisions >= FIO_MAP_MAX_FULL_COLLISIONS) {
        m->under_attack = 1;
        FIO_LOG_SECURITY("(core type) Map under attack?"
                         " (multiple full collisions)");
      }
    }
    i += FIO_MAP_CUCKOO_STEPS;
  } while (attempts--);
  return r;

seek_as_array:
  for (i = 0; i < m->w; ++i) {
    if (m->map[i].hash == hash &&
        FIO_MAP_OBJ_KEY_CMP(m->map[(i & imask)].obj, key)) {
      r = (FIO_NAME(FIO_MAP_NAME, __pos_s)){
          .i = (FIO_MAP_SIZE_TYPE)i,
          .imap = (FIO_MAP_SIZE_TYPE)i,
      };
      return r;
    }
  }
  if (m->w <= FIO_MAP_CAPA(m->bits))
    r.imap = m->w;
  return r;

  (void)key; /* just in case it's always true */
}

/* *****************************************************************************
Hash Map / Set - Internal API (Helpers) - Rehashing
***************************************************************************** */

/** Internal: rehashes the map. */
FIO_IFUNC int FIO_NAME(FIO_MAP_NAME, __rehash_no_holes)(FIO_MAP_S *m) {
  size_t pos = 0;
  FIO_MAP_SIZE_TYPE *imap = FIO_NAME(FIO_MAP_NAME, __imap)(m);
  FIO_NAME(FIO_MAP_NAME, each_s) *map = m->map;
  while (pos < m->w) {
    const FIO_MAP_SIZE_TYPE ihash =
        FIO_NAME(FIO_MAP_NAME, __hash2index)(map[pos].hash, m->bits);
    FIO_NAME(FIO_MAP_NAME, __pos_s)
    i = FIO_NAME(FIO_MAP_NAME, __get_pos)(
        m, map[pos].hash, ihash, FIO_MAP_OBJ2KEY(map[pos].obj));
    if (i.imap == FIO_MAP_INDEX_INVALID) {
      pos = 0;
      if (FIO_NAME(FIO_MAP_NAME, __map_realloc)(m, m->bits + 1))
        return -1;
      map = m->map;
      imap = FIO_NAME(FIO_MAP_NAME, __imap)(m);
      continue;
    }
    imap[i.imap] = (pos) | (ihash & (~(((FIO_MAP_SIZE_TYPE)1 << m->bits) - 1)));
    ++pos;
  }
  return 0;
}
FIO_IFUNC int FIO_NAME(FIO_MAP_NAME, __rehash_has_holes)(FIO_MAP_S *m) {
  FIO_NAME(FIO_MAP_NAME, __compact_forced)(m, 1);
  return FIO_NAME(FIO_MAP_NAME, __rehash_no_holes)(m);
  (void)m;
  return 0;
}

SFUNC int FIO_NAME(FIO_MAP_NAME, __rehash_router)(FIO_MAP_S *m) {
  if (m->count == m->w)
    return FIO_NAME(FIO_MAP_NAME, __rehash_no_holes)(m);
  return FIO_NAME(FIO_MAP_NAME, __rehash_has_holes)(m);
}

/* *****************************************************************************
Hash Map / Set - Internal API (Helpers) - Object Insertion / Removal
***************************************************************************** */

/** INTERNAL: sets an object in the map. */
SFUNC FIO_MAP_TYPE *FIO_NAME(FIO_MAP_NAME, __set)(FIO_MAP_S *m,
                                                  FIO_MAP_HASH hash,
#ifdef FIO_MAP_KEY
                                                  FIO_MAP_KEY key,
#endif /* FIO_MAP_KEY */
                                                  FIO_MAP_TYPE obj,
                                                  FIO_MAP_TYPE *old,
                                                  uint8_t overwrite) {

  hash = FIO_MAP_HASH_FIX(hash); // isn't called by caller...
  const FIO_MAP_SIZE_TYPE ihash =
      FIO_NAME(FIO_MAP_NAME, __hash2index)(hash, m->bits);
  FIO_NAME(FIO_MAP_NAME, __pos_s)
  i = FIO_NAME(FIO_MAP_NAME, __get_pos)(m,
                                        hash,
                                        ihash,
#ifdef FIO_MAP_KEY
                                        key
#else
                                        obj
#endif /* FIO_MAP_KEY */
  );
  if (i.i == FIO_MAP_INDEX_INVALID) {
    /* add new object */

    /* grow memory or clean existing memory */
    const FIO_MAP_SIZE_TYPE capa = FIO_MAP_CAPA(m->bits);
    if (m->w >= capa || i.imap == FIO_MAP_INDEX_INVALID) {
      if (m->w > m->count)
        FIO_NAME(FIO_MAP_NAME, __compact_forced)(m, 1);
      else if (FIO_NAME(FIO_MAP_NAME, __map_realloc)(m, m->bits + 1))
        goto error;
      i.imap = FIO_MAP_INDEX_INVALID;
    }

#if FIO_MAP_MAX_ELEMENTS
    /* are we using more elements then we should? */
    if (m->count >= FIO_MAP_MAX_ELEMENTS) {
      FIO_MAP_SIZE_TYPE pos = 0;
      while (!m->map[pos].hash)
        ++pos;
      FIO_ASSERT_DEBUG(pos < m->w, "always true.");
      if (i.imap != FIO_MAP_INDEX_INVALID) {
        const FIO_MAP_SIZE_TYPE tmp_ihash =
            FIO_NAME(FIO_MAP_NAME, __hash2index)(m->map[pos].hash, m->bits);
        FIO_NAME(FIO_MAP_NAME, __pos_s)
        i_tmp = FIO_NAME(FIO_MAP_NAME, __get_pos)(
            m, m->map[pos].hash, tmp_ihash, FIO_MAP_OBJ2KEY(m->map[pos].obj));
        FIO_ASSERT_DEBUG(i_tmp.i != FIO_MAP_INDEX_INVALID &&
                             i_tmp.imap != FIO_MAP_INDEX_INVALID,
                         "always true.");
        FIO_NAME(FIO_MAP_NAME, __imap)(m)[i_tmp.imap] = FIO_MAP_INDEX_INVALID;
      }
      FIO_MAP_OBJ_DESTROY(m->map[pos].obj);
      m->map[pos].obj = FIO_MAP_OBJ_INVALID;
      m->map[pos].hash = 0;
      --m->count;
    }
#endif

    /* copy information */
#ifdef FIO_MAP_KEY
    FIO_MAP_TYPE_COPY((m->map[m->w].obj.value), obj);
    FIO_MAP_KEY_COPY((m->map[m->w].obj.key), key);
#else
    FIO_MAP_TYPE_COPY((m->map[m->w].obj), obj);
#endif
    m->map[m->w].hash = hash;
    i.i = m->w;
    ++m->w;
    ++m->count;

    if (i.imap == FIO_MAP_INDEX_INVALID) {
      FIO_NAME(FIO_MAP_NAME, __rehash_router)(m);
    } else {
      FIO_MAP_SIZE_TYPE *imap = FIO_NAME(FIO_MAP_NAME, __imap)(m);
      imap[i.imap] =
          (m->w - 1) | (ihash & (~(((FIO_MAP_SIZE_TYPE)1 << m->bits) - 1)));
    }
    if (old)
      *old = FIO_MAP_TYPE_INVALID;
    return &FIO_MAP_OBJ2TYPE(m->map[i.i].obj);
  }
  /* existing. overwrite? */
  if (overwrite) {
#ifdef FIO_MAP_KEY
    if (old) {
      FIO_MAP_TYPE_COPY((*old), (m->map[i.i].obj.value));
#if FIO_MAP_DESTROY_AFTER_COPY
      FIO_MAP_TYPE_DESTROY(m->map[i.i].obj.value);
#endif
    } else {
      FIO_MAP_TYPE_DESTROY(m->map[i.i].obj.value);
    }
    FIO_MAP_TYPE_COPY((m->map[i.i].obj.value), obj);
    FIO_MAP_KEY_DISCARD(key);
#else  /* !FIO_MAP_KEY */
    if (old) {
      FIO_MAP_TYPE_COPY((*old), (m->map[i.i].obj));
      FIO_MAP_OBJ_DESTROY_AFTER((m->map[i.i].obj));
    } else {
      FIO_MAP_OBJ_DESTROY((m->map[i.i].obj));
    }
    FIO_MAP_OBJ_COPY((m->map[i.i].obj), obj);
#endif /* FIO_MAP_KEY */
  } else {
    FIO_MAP_KEY_DISCARD(key);
    FIO_MAP_TYPE_DISCARD(obj);
  }
  return &FIO_MAP_OBJ2TYPE(m->map[i.i].obj);

error:

  FIO_MAP_KEY_DISCARD(key);
  FIO_MAP_TYPE_DISCARD(obj);
  FIO_NAME(FIO_MAP_NAME, __map_realloc)(m, m->bits);
  FIO_NAME(FIO_MAP_NAME, __rehash_router)(m);
  return NULL;
}

/**
 * Removes an object from the hash map.
 */
SFUNC int FIO_NAME(FIO_MAP_NAME, remove)(FIO_MAP_PTR m_,
                                         FIO_MAP_HASH hash,
                                         FIO_MAP_OBJ_KEY key,
                                         FIO_MAP_TYPE *old) {
  FIO_MAP_S *const m = (FIO_MAP_S *)FIO_PTR_UNTAG(m_);
  if (!m || !m_ || !m->count)
    goto not_found;
  {
    hash = FIO_MAP_HASH_FIX(hash);
    const FIO_MAP_SIZE_TYPE ihash =
        FIO_NAME(FIO_MAP_NAME, __hash2index)(hash, m->bits);
    FIO_NAME(FIO_MAP_NAME, __pos_s)
    i = FIO_NAME(FIO_MAP_NAME, __get_pos)(m, hash, ihash, key);
    if (i.i == FIO_MAP_INDEX_INVALID)
      goto not_found;
    if (old) {
      FIO_MAP_TYPE_COPY((*old), FIO_MAP_OBJ2TYPE(m->map[i.i].obj));
      FIO_MAP_OBJ_DESTROY_AFTER((m->map[i.i].obj));
    } else {
      FIO_MAP_OBJ_DESTROY(m->map[i.i].obj);
    }
    m->map[i.i].obj = FIO_MAP_OBJ_INVALID;
    m->map[i.i].hash = 0;
    if (i.imap != FIO_MAP_INDEX_INVALID) {
      FIO_NAME(FIO_MAP_NAME, __imap)(m)[i.imap] = FIO_MAP_INDEX_INVALID;
    }
    --m->count;
    while (m->w && !m->map[m->w - 1].hash)
      --m->w;
  }
  return 0;
not_found:
  if (old)
    *old = FIO_MAP_TYPE_INVALID;
  return -1;
}

/* *****************************************************************************

Hash Map / Set - Iteration

***************************************************************************** */

FIO_SFUNC __thread FIO_MAP_SIZE_TYPE FIO_NAME(FIO_MAP_NAME, __each_pos) = 0;
FIO_SFUNC __thread FIO_NAME(FIO_MAP_NAME, s) *
    FIO_NAME(FIO_MAP_NAME, __each_map) = NULL;

/**
 * Iteration using a callback for each element in the map.
 *
 * The callback task function must accept an element variable as well as an
 * opaque user pointer.
 *
 * If the callback returns -1, the loop is broken. Any other value is ignored.
 *
 * Returns the relative "stop" position, i.e., the number of items processed +
 * the starting point.
 */
IFUNC FIO_MAP_SIZE_TYPE FIO_NAME(FIO_MAP_NAME,
                                 each)(FIO_MAP_PTR m_,
                                       ssize_t start_at,
                                       int (*task)(FIO_MAP_TYPE obj, void *arg),
                                       void *arg) {
  FIO_NAME(FIO_MAP_NAME, s) *const m =
      (FIO_NAME(FIO_MAP_NAME, s) *)FIO_PTR_UNTAG(m_);
  if (!m || !m_ || !m->map) {
    return 0;
  }

  FIO_NAME(FIO_MAP_NAME, s) *old_map = FIO_NAME(FIO_MAP_NAME, __each_map);
  if (start_at < 0) {
    start_at = m->count + start_at;
    if (start_at < 0)
      start_at = 0;
  }
  if ((FIO_MAP_SIZE_TYPE)start_at >= m->count)
    return m->count;
  FIO_MAP_SIZE_TYPE old_pos = FIO_NAME(FIO_MAP_NAME, __each_pos);
  FIO_MAP_SIZE_TYPE count = 0;
  FIO_NAME(FIO_MAP_NAME, __each_pos) = 0;
  FIO_NAME(FIO_MAP_NAME, __each_map) = m;
  count = start_at;
  if (m->w == m->count) {
    FIO_NAME(FIO_MAP_NAME, __each_pos) = start_at;
  } else {
    while (start_at) {
      FIO_MAP_SIZE_TYPE tmp = FIO_NAME(FIO_MAP_NAME, __each_pos);
      ++FIO_NAME(FIO_MAP_NAME, __each_pos);
      if (FIO_MAP_HASH_IS_INVALID(m->map[tmp].hash)) {
        continue;
      }
      --start_at;
    }
  }
  while (count < m->count && (++count) &&
         task(FIO_MAP_OBJ2TYPE(m->map[FIO_NAME(FIO_MAP_NAME, __each_pos)].obj),
              arg) != -1)
    ++FIO_NAME(FIO_MAP_NAME, __each_pos);
  FIO_NAME(FIO_MAP_NAME, __each_pos) = old_pos;
  FIO_NAME(FIO_MAP_NAME, __each_map) = old_map;
  return count;
}

#ifdef FIO_MAP_KEY
/**
 * Returns the current `key` within an `each` task.
 *
 * Only available within an `each` loop.
 *
 * For sets, returns the hash value, for hash maps, returns the key value.
 */
SFUNC FIO_MAP_KEY FIO_NAME(FIO_MAP_NAME, each_get_key)(void) {
  if (!FIO_NAME(FIO_MAP_NAME, __each_map) ||
      !FIO_NAME(FIO_MAP_NAME, __each_map)->map)
    return FIO_MAP_KEY_INVALID;
  return FIO_NAME(FIO_MAP_NAME, __each_map)
      ->map[FIO_NAME(FIO_MAP_NAME, __each_pos)]
      .obj.key;
}
#else
/**
 * Returns the current `key` within an `each` task.
 *
 * Only available within an `each` loop.
 *
 * For sets, returns the hash value, for hash maps, returns the key value.
 */
SFUNC FIO_MAP_HASH FIO_NAME(FIO_MAP_NAME, each_get_key)(void) {
  if (!FIO_NAME(FIO_MAP_NAME, __each_map) ||
      !FIO_NAME(FIO_MAP_NAME, __each_map)->map)
    return FIO_MAP_HASH_INVALID;
  return FIO_NAME(FIO_MAP_NAME, __each_map)
      ->map[FIO_NAME(FIO_MAP_NAME, __each_pos)]
      .hash;
}
#endif

/* *****************************************************************************
Hash Map / Set - cleanup
***************************************************************************** */
#endif /* FIO_EXTERN_COMPLETE */

#undef FIO_MAP_NAME
#undef FIO_MAP_BIG
#undef FIO_MAP_CAPA
#undef FIO_MAP_CUCKOO_STEPS
#undef FIO_MAP_DESTROY_AFTER_COPY

#undef FIO_MAP_HASH
#undef FIO_MAP_HASH_FIX
#undef FIO_MAP_HASH_FIXED
#undef FIO_MAP_HASH_INVALID
#undef FIO_MAP_HASH_IS_INVALID

#undef FIO_MAP_INDEX_CALC
#undef FIO_MAP_INDEX_INVALID
#undef FIO_MAP_INDEX_UNUSED
#undef FIO_MAP_INDEX_USED_BIT

#undef FIO_MAP_KEY
#undef FIO_MAP_KEY_CMP
#undef FIO_MAP_KEY_COPY
#undef FIO_MAP_KEY_DESTROY
#undef FIO_MAP_KEY_DESTROY_SIMPLE
#undef FIO_MAP_KEY_DISCARD
#undef FIO_MAP_KEY_INVALID

#undef FIO_MAP_MAX_ELEMENTS
#undef FIO_MAP_MAX_FULL_COLLISIONS
#undef FIO_MAP_MAX_SEEK

#undef FIO_MAP_OBJ
#undef FIO_MAP_OBJ2KEY
#undef FIO_MAP_OBJ2TYPE
#undef FIO_MAP_OBJ_CMP
#undef FIO_MAP_OBJ_COPY
#undef FIO_MAP_OBJ_DESTROY
#undef FIO_MAP_OBJ_DESTROY_AFTER
#undef FIO_MAP_OBJ_DISCARD
#undef FIO_MAP_OBJ_INVALID
#undef FIO_MAP_OBJ_KEY
#undef FIO_MAP_OBJ_KEY_CMP
#undef FIO_MAP_PTR
#undef FIO_MAP_S
#undef FIO_MAP_SEEK_AS_ARRAY_LOG_LIMIT
#undef FIO_MAP_SIZE_TYPE
#undef FIO_MAP_TYPE
#undef FIO_MAP_TYPE_CMP
#undef FIO_MAP_TYPE_COPY
#undef FIO_MAP_TYPE_COPY_SIMPLE
#undef FIO_MAP_TYPE_DESTROY
#undef FIO_MAP_TYPE_DESTROY_SIMPLE
#undef FIO_MAP_TYPE_DISCARD
#undef FIO_MAP_TYPE_INVALID

#endif /* FIO_MAP_NAME */
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************










                        Dynamic Strings (binary safe)










***************************************************************************** */
#ifdef FIO_STR_SMALL
#ifndef FIO_STR_NAME
#define FIO_STR_NAME FIO_STR_SMALL
#endif
#ifndef FIO_STR_OPTIMIZE4IMMUTABILITY
#define FIO_STR_OPTIMIZE4IMMUTABILITY 1
#endif
#endif /* FIO_STR_SMALL */

#if defined(FIO_STR_NAME)

#ifndef FIO_STR_OPTIMIZE_EMBEDDED
/**
 * For each unit (0 by default), adds `sizeof(char *)` bytes to the type size,
 * increasing the amount of strings that could be embedded within the type
 * without memory allocation.
 *
 * For example, when using a referrence counter wrapper on a 64bit system, it
 * would make sense to set this value to 1 - allowing the type size to fully
 * utilize a 16 byte memory allocation alignment.
 */
#define FIO_STR_OPTIMIZE_EMBEDDED 0
#endif

#ifndef FIO_STR_OPTIMIZE4IMMUTABILITY
/**
 * Optimizes the struct to minimal size that can store the string length and a
 * pointer.
 *
 * By avoiding extra (mutable related) data, such as the allocated memory's
 * capacity, strings require less memory. However, this does introduce a
 * performance penalty when editing the string data.
 */
#define FIO_STR_OPTIMIZE4IMMUTABILITY 0
#endif

#if FIO_STR_OPTIMIZE4IMMUTABILITY
/* enforce limit after which FIO_STR_OPTIMIZE4IMMUTABILITY makes no sense */
#if FIO_STR_OPTIMIZE_EMBEDDED > 1
#undef FIO_STR_OPTIMIZE_EMBEDDED
#define FIO_STR_OPTIMIZE_EMBEDDED 1
#endif
#else
/* enforce limit due to 6 bit embedded string length limit */
#if FIO_STR_OPTIMIZE_EMBEDDED > 4
#undef FIO_STR_OPTIMIZE_EMBEDDED
#define FIO_STR_OPTIMIZE_EMBEDDED 4
#endif
#endif /* FIO_STR_OPTIMIZE4IMMUTABILITY*/

/* *****************************************************************************
String API - Initialization and Destruction
***************************************************************************** */

/**
 * The `fio_str_s` type should be considered opaque.
 *
 * The type's attributes should be accessed ONLY through the accessor
 * functions: `fio_str2cstr`, `fio_str_len`, `fio_str2ptr`, `fio_str_capa`,
 * etc'.
 *
 * Note: when the `small` flag is present, the structure is ignored and used
 * as raw memory for a small String (no additional allocation). This changes
 * the String's behavior drastically and requires that the accessor functions
 * be used.
 */
typedef struct {
  uint8_t special; /* Flags and small string data */
  uint8_t reserved[(sizeof(void *) * (1 + FIO_STR_OPTIMIZE_EMBEDDED)) -
                   (sizeof(uint8_t))]; /* padding length */
#if !FIO_STR_OPTIMIZE4IMMUTABILITY
  size_t capa; /* known capacity for longer Strings */
  size_t len;  /* String length for longer Strings */
#endif         /* FIO_STR_OPTIMIZE4IMMUTABILITY */
  char *buf;   /* pointer for longer Strings */
} FIO_NAME(FIO_STR_NAME, s);

#ifdef FIO_PTR_TAG_TYPE
#define FIO_STR_PTR FIO_PTR_TAG_TYPE
#else
#define FIO_STR_PTR FIO_NAME(FIO_STR_NAME, s) *
#endif

#ifndef FIO_STR_INIT
/**
 * This value should be used for initialization. For example:
 *
 *      // on the stack
 *      fio_str_s str = FIO_STR_INIT;
 *
 *      // or on the heap
 *      fio_str_s *str = malloc(sizeof(*str));
 *      *str = FIO_STR_INIT;
 *
 * Remember to cleanup:
 *
 *      // on the stack
 *      fio_str_destroy(&str);
 *
 *      // or on the heap
 *      fio_str_free(str);
 *      free(str);
 */
#define FIO_STR_INIT                                                           \
  { .special = 0 }

/**
 * This macro allows the container to be initialized with existing data, as long
 * as it's memory was allocated with the same allocator (`malloc` /
 * `fio_malloc`).
 *
 * The `capacity` value should exclude the NUL character (if exists).
 *
 * NOTE: This macro isn't valid for FIO_STR_SMALL (or strings with the
 * FIO_STR_OPTIMIZE4IMMUTABILITY optimization)
 */
#define FIO_STR_INIT_EXISTING(buffer, length, capacity)                        \
  { .capa = (capacity), .len = (length), .buf = (buffer) }

/**
 * This macro allows the container to be initialized with existing static data,
 * that shouldn't be freed.
 *
 * NOTE: This macro isn't valid for FIO_STR_SMALL (or strings with the
 * FIO_STR_OPTIMIZE4IMMUTABILITY optimization)
 */
#define FIO_STR_INIT_STATIC(buffer)                                            \
  { .special = 4, .len = strlen((buffer)), .buf = (char *)(buffer) }

/**
 * This macro allows the container to be initialized with existing static data,
 * that shouldn't be freed.
 *
 * NOTE: This macro isn't valid for FIO_STR_SMALL (or strings with the
 * FIO_STR_OPTIMIZE4IMMUTABILITY optimization)
 */
#define FIO_STR_INIT_STATIC2(buffer, length)                                   \
  { .special = 4, .len = (length), .buf = (char *)(buffer) }

#endif /* FIO_STR_INIT */

#ifndef FIO_REF_CONSTRUCTOR_ONLY

/** Allocates a new String object on the heap. */
FIO_IFUNC FIO_STR_PTR FIO_NAME(FIO_STR_NAME, new)(void);

/**
 * Destroys the string and frees the container (if allocated with `new`).
 */
FIO_IFUNC void FIO_NAME(FIO_STR_NAME, free)(FIO_STR_PTR s);

#endif /* FIO_REF_CONSTRUCTOR_ONLY */

/**
 * Initializes the container with the provided static / constant string.
 *
 * The string will be copied to the container **only** if it will fit in the
 * container itself. Otherwise, the supplied pointer will be used as is and it
 * should remain valid until the string is destroyed.
 *
 * The final string can be safely be destroyed (using the `destroy` function).
 */
FIO_IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, init_const)(FIO_STR_PTR s,
                                                            const char *str,
                                                            size_t len);

/**
 * Initializes the container with a copy of the provided dynamic string.
 *
 * The string is always copied and the final string must be destroyed (using the
 * `destroy` function).
 */
FIO_IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, init_copy)(FIO_STR_PTR s,
                                                           const char *str,
                                                           size_t len);

/**
 * Initializes the container with a copy of an existing String object.
 *
 * The string is always copied and the final string must be destroyed (using the
 * `destroy` function).
 */
FIO_IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, init_copy2)(FIO_STR_PTR dest,
                                                            FIO_STR_PTR src);

/**
 * Frees the String's resources and reinitializes the container.
 *
 * Note: if the container isn't allocated on the stack, it should be freed
 * separately using the appropriate `free` function.
 */
FIO_IFUNC void FIO_NAME(FIO_STR_NAME, destroy)(FIO_STR_PTR s);

/**
 * Returns a C string with the existing data, re-initializing the String.
 *
 * Note: the String data is removed from the container, but the container
 * isn't freed.
 *
 * Returns NULL if there's no String data.
 *
 * NOTE: Returned string is ALWAYS dynamically allocated. Remember to free.
 */
FIO_IFUNC char *FIO_NAME(FIO_STR_NAME, detach)(FIO_STR_PTR s);

/* *****************************************************************************
String API - String state (data pointers, length, capacity, etc')
***************************************************************************** */

/** Returns the String's complete state (capacity, length and pointer).  */
FIO_IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, info)(const FIO_STR_PTR s);

/** Returns a pointer (`char *`) to the String's content. */
FIO_IFUNC char *FIO_NAME2(FIO_STR_NAME, ptr)(FIO_STR_PTR s);

/** Returns the String's length in bytes. */
FIO_IFUNC size_t FIO_NAME(FIO_STR_NAME, len)(FIO_STR_PTR s);

/** Returns the String's existing capacity (total used & available memory). */
FIO_IFUNC size_t FIO_NAME(FIO_STR_NAME, capa)(FIO_STR_PTR s);

/**
 * Prevents further manipulations to the String's content.
 */
FIO_IFUNC void FIO_NAME(FIO_STR_NAME, freeze)(FIO_STR_PTR s);

/**
 * Returns true if the string is frozen.
 */
FIO_IFUNC uint8_t FIO_NAME_BL(FIO_STR_NAME, frozen)(FIO_STR_PTR s);

/** Returns 1 if memory was allocated and (the String must be destroyed). */
FIO_IFUNC int FIO_NAME_BL(FIO_STR_NAME, allocated)(const FIO_STR_PTR s);

/**
 * Binary comparison returns `1` if both strings are equal and `0` if not.
 */
FIO_IFUNC int FIO_NAME_BL(FIO_STR_NAME, eq)(const FIO_STR_PTR str1,
                                            const FIO_STR_PTR str2);

/**
 * Returns the string's Risky Hash value.
 *
 * Note: Hash algorithm might change without notice.
 */
FIO_IFUNC uint64_t FIO_NAME(FIO_STR_NAME, hash)(const FIO_STR_PTR s,
                                                uint64_t seed);

/* *****************************************************************************
String API - Memory management
***************************************************************************** */

/**
 * Sets the new String size without reallocating any memory (limited by
 * existing capacity).
 *
 * Returns the updated state of the String.
 *
 * Note: When shrinking, any existing data beyond the new size may be
 * corrupted.
 */
FIO_IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, resize)(FIO_STR_PTR s,
                                                        size_t size);

/**
 * Performs a best attempt at minimizing memory consumption.
 *
 * Actual effects depend on the underlying memory allocator and it's
 * implementation. Not all allocators will free any memory.
 */
FIO_IFUNC void FIO_NAME(FIO_STR_NAME, compact)(FIO_STR_PTR s);

#if !FIO_STR_OPTIMIZE4IMMUTABILITY
/**
 * Reserves (at least) `amount` of bytes for the string's data.
 *
 * The reserved count includes used data. If `amount` is less than the current
 * string length, the string will be truncated(!).
 *
 * May corrupt the string length information (if string is assumed to be
 * immutable), make sure to call `resize` with the updated information once the
 * editing is done.
 *
 * Returns the updated state of the String.
 */
SFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, reserve)(FIO_STR_PTR s,
                                                     size_t amount);
#define FIO_STR_RESERVE_NAME reserve
#else
/** INTERNAL - DO NOT USE! */
SFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, __reserve)(FIO_STR_PTR s,
                                                       size_t amount);
#define FIO_STR_RESERVE_NAME __reserve
#endif
/* *****************************************************************************
String API - UTF-8 State
***************************************************************************** */

/** Returns 1 if the String is UTF-8 valid and 0 if not. */
SFUNC size_t FIO_NAME(FIO_STR_NAME, utf8_valid)(FIO_STR_PTR s);

/** Returns the String's length in UTF-8 characters. */
SFUNC size_t FIO_NAME(FIO_STR_NAME, utf8_len)(FIO_STR_PTR s);

/**
 * Takes a UTF-8 character selection information (UTF-8 position and length)
 * and updates the same variables so they reference the raw byte slice
 * information.
 *
 * If the String isn't UTF-8 valid up to the requested selection, than `pos`
 * will be updated to `-1` otherwise values are always positive.
 *
 * The returned `len` value may be shorter than the original if there wasn't
 * enough data left to accommodate the requested length. When a `len` value of
 * `0` is returned, this means that `pos` marks the end of the String.
 *
 * Returns -1 on error and 0 on success.
 */
SFUNC int FIO_NAME(FIO_STR_NAME,
                   utf8_select)(FIO_STR_PTR s, intptr_t *pos, size_t *len);

/* *****************************************************************************
String API - Content Manipulation and Review
***************************************************************************** */

/**
 * Writes data at the end of the String.
 */
FIO_IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, write)(FIO_STR_PTR s,
                                                       const void *src,
                                                       size_t src_len);

/**
 * Writes a number at the end of the String using normal base 10 notation.
 */
IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, write_i)(FIO_STR_PTR s,
                                                     int64_t num);

/**
 * Writes a number at the end of the String using Hex (base 16) notation.
 */
IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, write_hex)(FIO_STR_PTR s,
                                                       int64_t num);
/**
 * Appends the `src` String to the end of the `dest` String.
 *
 * If `dest` is empty, the resulting Strings will be equal.
 */
IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, concat)(FIO_STR_PTR dest,
                                                    FIO_STR_PTR const src);

/** Alias for fio_str_concat */
FIO_IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, join)(FIO_STR_PTR dest,
                                                      FIO_STR_PTR const src) {
  return FIO_NAME(FIO_STR_NAME, concat)(dest, src);
}

/**
 * Replaces the data in the String - replacing `old_len` bytes starting at
 * `start_pos`, with the data at `src` (`src_len` bytes long).
 *
 * Negative `start_pos` values are calculated backwards, `-1` == end of
 * String.
 *
 * When `old_len` is zero, the function will insert the data at `start_pos`.
 *
 * If `src_len == 0` than `src` will be ignored and the data marked for
 * replacement will be erased.
 */
SFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, replace)(FIO_STR_PTR s,
                                                     intptr_t start_pos,
                                                     size_t old_len,
                                                     const void *src,
                                                     size_t src_len);

/**
 * Writes to the String using a vprintf like interface.
 *
 * Data is written to the end of the String.
 */
SFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, vprintf)(FIO_STR_PTR s,
                                                     const char *format,
                                                     va_list argv);

/**
 * Writes to the String using a printf like interface.
 *
 * Data is written to the end of the String.
 */
SFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME,
                              printf)(FIO_STR_PTR s, const char *format, ...);

#if FIO_HAVE_UNIX_TOOLS
/**
 * Reads data from a file descriptor `fd` at offset `start_at` and pastes it's
 * contents (or a slice of it) at the end of the String. If `limit == 0`, than
 * the data will be read until EOF.
 *
 * The file should be a regular file or the operation might fail (can't be used
 * for sockets).
 *
 * The file descriptor will remain open and should be closed manually.
 *
 * Currently implemented only on POSIX systems.
 */
SFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, readfd)(FIO_STR_PTR s,
                                                    int fd,
                                                    intptr_t start_at,
                                                    intptr_t limit);
/**
 * Opens the file `filename` and pastes it's contents (or a slice ot it) at
 * the end of the String. If `limit == 0`, than the data will be read until
 * EOF.
 *
 * If the file can't be located, opened or read, or if `start_at` is beyond
 * the EOF position, NULL is returned in the state's `data` field.
 *
 * Currently implemented only on POSIX systems.
 */
SFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, readfile)(FIO_STR_PTR s,
                                                      const char *filename,
                                                      intptr_t start_at,
                                                      intptr_t limit);
#endif

/* *****************************************************************************
String API - C / JSON escaping
***************************************************************************** */

/**
 * Writes data at the end of the String, escaping the data using JSON semantics.
 *
 * The JSON semantic are common to many programming languages, promising a UTF-8
 * String while making it easy to read and copy the string during debugging.
 */
IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, write_escape)(FIO_STR_PTR s,
                                                          const void *data,
                                                          size_t data_len);

/**
 * Writes an escaped data into the string after unescaping the data.
 */
IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, write_unescape)(FIO_STR_PTR s,
                                                            const void *escaped,
                                                            size_t len);

/* *****************************************************************************
String API - Base64 support
***************************************************************************** */

/**
 * Writes data at the end of the String, encoding the data as Base64 encoded
 * data.
 */
IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME,
                              write_base64enc)(FIO_STR_PTR s,
                                               const void *data,
                                               size_t data_len,
                                               uint8_t url_encoded);

/**
 * Writes decoded base64 data to the end of the String.
 */
IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME,
                              write_base64dec)(FIO_STR_PTR s,
                                               const void *encoded,
                                               size_t encoded_len);

/* *****************************************************************************
String API - Testing
***************************************************************************** */
#ifdef FIO_STR_WRITE_TEST_FUNC
/**
 * Tests the fio_str functionality.
 */
SFUNC void FIO_NAME(FIO_STR_NAME, __dynamic_test)(void);
#endif
/* *****************************************************************************


                             String Implementation

                           IMPLEMENTATION - INLINED


***************************************************************************** */

/* *****************************************************************************
String Macro Helpers
***************************************************************************** */

#define FIO_STR_IS_SMALL(s) (((s)->special & 1) || !(s)->buf)
#define FIO_STR_SMALL_LEN(s) ((size_t)((s)->special >> 2))
#define FIO_STR_SMALL_LEN_SET(s, l)                                            \
  ((s)->special = (((s)->special & 2) | ((uint8_t)(l) << 2) | 1))
#define FIO_STR_SMALL_CAPA(s) (sizeof(*(s)) - 2)
#define FIO_STR_SMALL_DATA(s) ((char *)((s)->reserved))

#define FIO_STR_BIG_DATA(s) ((s)->buf)
#define FIO_STR_BIG_IS_DYNAMIC(s) (!((s)->special & 4))
#define FIO_STR_BIG_SET_STATIC(s) ((s)->special |= 4)
#define FIO_STR_BIG_FREE_BUF(s) (FIO_MEM_FREE_((s)->buf, FIO_STR_BIG_CAPA((s))))

#define FIO_STR_IS_FROZEN(s) ((s)->special & 2)
#define FIO_STR_FREEZE_(s) ((s)->special |= 2)
#define FIO_STR_THAW_(s) ((s)->special ^= (uint8_t)2)

#if FIO_STR_OPTIMIZE4IMMUTABILITY
#define FIO_STR_BIG_LEN(s)                                                     \
  ((sizeof(void *) == 4)                                                       \
       ? (((size_t)(s)->reserved[0]) | ((size_t)(s)->reserved[1] << 8) |       \
          ((size_t)(s)->reserved[2] << 16))                                    \
       : (((size_t)(s)->reserved[0]) | ((size_t)(s)->reserved[1] << 8) |       \
          ((size_t)(s)->reserved[2] << 16) |                                   \
          ((size_t)(s)->reserved[3] << 24) |                                   \
          ((size_t)(s)->reserved[4] << 32) |                                   \
          ((size_t)(s)->reserved[5] << 40) |                                   \
          ((size_t)(s)->reserved[6] << 48)))
#define FIO_STR_BIG_LEN_SET(s, l)                                              \
  do {                                                                         \
    if (sizeof(void *) == 4) {                                                 \
      if (!((l) & ((~(size_t)0) << 24))) {                                     \
        (s)->reserved[0] = (l)&0xFF;                                           \
        (s)->reserved[1] = ((size_t)(l) >> 8) & 0xFF;                          \
        (s)->reserved[2] = ((size_t)(l) >> 16) & 0xFF;                         \
      } else {                                                                 \
        FIO_LOG_ERROR("facil.io small string length error - too long");        \
        (s)->reserved[0] = 0xFF;                                               \
        (s)->reserved[1] = 0xFF;                                               \
        (s)->reserved[2] = 0xFF;                                               \
      }                                                                        \
    } else {                                                                   \
      if (!((l) & ((~(size_t)0) << 56))) {                                     \
        (s)->reserved[0] = (l)&0xff;                                           \
        (s)->reserved[1] = ((size_t)(l) >> 8) & 0xFF;                          \
        (s)->reserved[2] = ((size_t)(l) >> 16) & 0xFF;                         \
        (s)->reserved[3] = ((size_t)(l) >> 24) & 0xFF;                         \
        (s)->reserved[4] = ((size_t)(l) >> 32) & 0xFF;                         \
        (s)->reserved[5] = ((size_t)(l) >> 40) & 0xFF;                         \
        (s)->reserved[6] = ((size_t)(l) >> 48) & 0xFF;                         \
      } else {                                                                 \
        FIO_LOG_ERROR("facil.io small string length error - too long");        \
        (s)->reserved[0] = 0xFF;                                               \
        (s)->reserved[1] = 0xFF;                                               \
        (s)->reserved[2] = 0xFF;                                               \
        (s)->reserved[3] = 0xFF;                                               \
        (s)->reserved[4] = 0xFF;                                               \
        (s)->reserved[5] = 0xFF;                                               \
        (s)->reserved[6] = 0xFF;                                               \
      }                                                                        \
    }                                                                          \
  } while (0)
#define FIO_STR_BIG_CAPA(s) FIO_STR_CAPA2WORDS(FIO_STR_BIG_LEN((s)))
#define FIO_STR_BIG_CAPA_SET(s, capa)
#else
#define FIO_STR_BIG_LEN(s) ((s)->len)
#define FIO_STR_BIG_LEN_SET(s, l) ((s)->len = (l))
#define FIO_STR_BIG_CAPA(s) ((s)->capa)
#define FIO_STR_BIG_CAPA_SET(s, capa) (FIO_STR_BIG_CAPA(s) = (capa))
#endif

/**
 * Rounds up allocated capacity to the closest 2 words byte boundary (leaving 1
 * byte space for the NUL byte).
 *
 * This shouldn't effect actual allocation size and should only minimize the
 * effects of the memory allocator's alignment rounding scheme.
 *
 * To clarify:
 *
 * Memory allocators are required to allocate memory on the minimal alignment
 * required by the largest type (`long double`), which usually results in memory
 * allocations using this alignment as a minimal spacing.
 *
 * For example, on 64 bit architectures, it's likely that `malloc(18)` will
 * allocate the same amount of memory as `malloc(32)` due to alignment concerns.
 *
 * In fact, with some allocators (i.e., jemalloc), spacing increases for larger
 * allocations - meaning the allocator will round up to more than 16 bytes, as
 * noted here: http://jemalloc.net/jemalloc.3.html#size_classes
 *
 * Note that this increased spacing, doesn't occure with facil.io's allocator,
 * since it uses 16 byte alignment right up until allocations are routed
 * directly to `mmap` (due to their size, usually over 12KB).
 */
#define FIO_STR_CAPA2WORDS(num)                                                \
  ((size_t)((size_t)(num) | (sizeof(long double) - 1)))

/* *****************************************************************************
String Constructors (inline)
***************************************************************************** */
#ifndef FIO_REF_CONSTRUCTOR_ONLY

/** Allocates a new String object on the heap. */
FIO_IFUNC FIO_STR_PTR FIO_NAME(FIO_STR_NAME, new)(void) {
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_MEM_CALLOC_(sizeof(*s), 1);
  return (FIO_STR_PTR)FIO_PTR_TAG(s);
}

/** Destroys the string and frees the container (if allocated with `new`). */
FIO_IFUNC void FIO_NAME(FIO_STR_NAME, free)(FIO_STR_PTR s_) {
  FIO_NAME(FIO_STR_NAME, destroy)(s_);
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (s) {
    FIO_MEM_FREE_(s, sizeof(*s));
  }
}

#endif /* FIO_REF_CONSTRUCTOR_ONLY */

/**
 * Frees the String's resources and reinitializes the container.
 *
 * Note: if the container isn't allocated on the stack, it should be freed
 * separately using the appropriate `free` function.
 */
FIO_IFUNC void FIO_NAME(FIO_STR_NAME, destroy)(FIO_STR_PTR s_) {
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s || !s_)
    return;
  if (!FIO_STR_IS_SMALL(s) && FIO_STR_BIG_IS_DYNAMIC(s)) {
    FIO_STR_BIG_FREE_BUF(s);
  }
  *s = (FIO_NAME(FIO_STR_NAME, s))FIO_STR_INIT;
}

/**
 * Returns a C string with the existing data, re-initializing the String.
 *
 * Note: the String data is removed from the container, but the container
 * isn't freed.
 *
 * Returns NULL if there's no String data.
 */
FIO_IFUNC char *FIO_NAME(FIO_STR_NAME, detach)(FIO_STR_PTR s_) {
  char *data = NULL;
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s || !s_) {
    return data;
  }
  if (FIO_STR_IS_SMALL(s)) {
    if (FIO_STR_SMALL_LEN(s)) { /* keep these ifs apart */
      data = (char *)FIO_MEM_CALLOC_(sizeof(*data), (FIO_STR_SMALL_LEN(s) + 1));
      if (data)
        memcpy(data, FIO_STR_SMALL_DATA(s), (FIO_STR_SMALL_LEN(s) + 1));
    }
  } else {
    if (FIO_STR_BIG_IS_DYNAMIC(s)) {
      data = FIO_STR_BIG_DATA(s);
    } else if (FIO_STR_BIG_LEN(s)) {
      data = (char *)FIO_MEM_CALLOC_(sizeof(*data), (FIO_STR_BIG_LEN(s) + 1));
      if (data)
        memcpy(data, FIO_STR_BIG_DATA(s), FIO_STR_BIG_LEN(s) + 1);
    }
  }
  *s = (FIO_NAME(FIO_STR_NAME, s)){0};
  return data;
}

/**
 * Performs a best attempt at minimizing memory consumption.
 *
 * Actual effects depend on the underlying memory allocator and it's
 * implementation. Not all allocators will free any memory.
 */
FIO_IFUNC void FIO_NAME(FIO_STR_NAME, compact)(FIO_STR_PTR s_) {
#if FIO_STR_OPTIMIZE4IMMUTABILITY
  (void)s_;
#else
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s_ || !s || FIO_STR_IS_SMALL(s) || FIO_STR_BIG_IS_DYNAMIC(s) ||
      FIO_STR_CAPA2WORDS(FIO_NAME(FIO_STR_NAME, len)(s_)) >=
          FIO_NAME(FIO_STR_NAME, capa)(s_))
    return;
  FIO_NAME(FIO_STR_NAME, s) tmp = FIO_STR_INIT;
  fio_str_info_s i = FIO_NAME(FIO_STR_NAME, info)(s_);
  FIO_NAME(FIO_STR_NAME, init_copy)
  ((FIO_STR_PTR)FIO_PTR_TAG(&tmp), i.buf, i.len);
  FIO_NAME(FIO_STR_NAME, destroy)(s_);
  *s = tmp;
#endif
}

/* *****************************************************************************
String Initialization (inline)
***************************************************************************** */

/**
 * Initializes the container with the provided static / constant string.
 *
 * The string will be copied to the container **only** if it will fit in the
 * container itself. Otherwise, the supplied pointer will be used as is and it
 * should remain valid until the string is destroyed.
 *
 * The final string can be safely be destroyed (using the `destroy` function).
 */
FIO_IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, init_const)(FIO_STR_PTR s_,
                                                            const char *str,
                                                            size_t len) {
  fio_str_info_s i = {0};
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s || !s_) {
    FIO_LOG_ERROR("Attempted to initialize a NULL String.");
    return i;
  }
  *s = (FIO_NAME(FIO_STR_NAME, s)){0};
  if (len < FIO_STR_SMALL_CAPA(s)) {
    FIO_STR_SMALL_LEN_SET(s, len);
    if (len && str)
      memcpy(FIO_STR_SMALL_DATA(s), str, len);
    FIO_STR_SMALL_DATA(s)[len] = 0;
    i = (fio_str_info_s){.buf = FIO_STR_SMALL_DATA(s),
                         .len = len,
                         .capa = FIO_STR_SMALL_CAPA(s)};
    return i;
  }
  FIO_STR_BIG_DATA(s) = (char *)str;
  FIO_STR_BIG_LEN_SET(s, len);
  FIO_STR_BIG_SET_STATIC(s);
  i = (fio_str_info_s){.buf = FIO_STR_BIG_DATA(s), .len = len, .capa = 0};
  return i;
}

/**
 * Initializes the container with the provided dynamic string.
 *
 * The string is always copied and the final string must be destroyed (using the
 * `destroy` function).
 */
FIO_IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, init_copy)(FIO_STR_PTR s_,
                                                           const char *str,
                                                           size_t len) {
  fio_str_info_s i = {0};
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s || !s_) {
    FIO_LOG_ERROR("Attempted to initialize a NULL String.");
    return i;
  }
  *s = (FIO_NAME(FIO_STR_NAME, s)){0};
  if (len < FIO_STR_SMALL_CAPA(s)) {
    FIO_STR_SMALL_LEN_SET(s, len);
    if (len && str)
      memcpy(FIO_STR_SMALL_DATA(s), str, len);
    FIO_STR_SMALL_DATA(s)[len] = 0;
    i = (fio_str_info_s){.buf = FIO_STR_SMALL_DATA(s),
                         .len = len,
                         .capa = FIO_STR_SMALL_CAPA(s)};
    return i;
  }

  {
    char *buf = (char *)FIO_MEM_CALLOC_(FIO_STR_CAPA2WORDS(len) + 1, 1);
    if (!buf)
      return i;
    i = (fio_str_info_s){
        .buf = buf, .len = len, .capa = FIO_STR_CAPA2WORDS(len)};
  }
  FIO_STR_BIG_CAPA_SET(s, i.capa);
  FIO_STR_BIG_DATA(s) = i.buf;
  FIO_STR_BIG_LEN_SET(s, len);
  if (str)
    memcpy(FIO_STR_BIG_DATA(s), str, len);
  return i;
}

/**
 * Initializes the container with a copy of an existing String object.
 *
 * The string is always copied and the final string must be destroyed (using the
 * `destroy` function).
 */
FIO_IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, init_copy2)(FIO_STR_PTR dest,
                                                            FIO_STR_PTR src) {
  fio_str_info_s i = FIO_NAME(FIO_STR_NAME, info)(src);
  i = FIO_NAME(FIO_STR_NAME, init_copy)(dest, i.buf, i.len);
  return i;
}

/* *****************************************************************************
String Information (inline)
***************************************************************************** */

/** Returns the String's complete state (capacity, length and pointer).  */
FIO_IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, info)(const FIO_STR_PTR s_) {
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s || !s_)
    return (fio_str_info_s){0};
  if (FIO_STR_IS_SMALL(s))
    return (fio_str_info_s){
        .buf = FIO_STR_SMALL_DATA(s),
        .len = FIO_STR_SMALL_LEN(s),
        .capa = (FIO_STR_IS_FROZEN(s) ? 0 : FIO_STR_SMALL_CAPA(s)),
    };

  return (fio_str_info_s){
      .buf = FIO_STR_BIG_DATA(s),
      .len = FIO_STR_BIG_LEN(s),
      .capa = (FIO_STR_IS_FROZEN(s) ? 0 : FIO_STR_BIG_CAPA(s)),
  };
}

/** Returns a pointer (`char *`) to the String's content. */
FIO_IFUNC char *FIO_NAME2(FIO_STR_NAME, ptr)(FIO_STR_PTR s_) {
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s || !s_)
    return NULL;
  return FIO_STR_IS_SMALL(s) ? FIO_STR_SMALL_DATA(s) : FIO_STR_BIG_DATA(s);
}

/** Returns the String's length in bytes. */
FIO_IFUNC size_t FIO_NAME(FIO_STR_NAME, len)(FIO_STR_PTR s_) {
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s || !s_)
    return 0;
  return FIO_STR_IS_SMALL(s) ? FIO_STR_SMALL_LEN(s) : FIO_STR_BIG_LEN(s);
}

/** Returns the String's existing capacity (total used & available memory). */
FIO_IFUNC size_t FIO_NAME(FIO_STR_NAME, capa)(FIO_STR_PTR s_) {
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s || !s_)
    return 0;
  if (FIO_STR_IS_SMALL(s))
    return FIO_STR_SMALL_CAPA(s);
  if (FIO_STR_BIG_IS_DYNAMIC(s))
    return FIO_STR_BIG_CAPA(s);
  return 0;
}

/**
 * Sets the new String size without reallocating any memory (limited by
 * existing capacity).
 */
FIO_IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, resize)(FIO_STR_PTR s_,
                                                        size_t size) {
  fio_str_info_s i = {.capa = FIO_NAME(FIO_STR_NAME, capa)(s_)};
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s_ || !s || FIO_STR_IS_FROZEN(s)) {
    i = FIO_NAME(FIO_STR_NAME, info)(s_);
    return i;
  }
  /* resize may be used to reserve memory in advance while setting size  */
  if (i.capa < size) {
    i = FIO_NAME(FIO_STR_NAME, FIO_STR_RESERVE_NAME)(s_, size);
  }

  if (FIO_STR_IS_SMALL(s)) {
    FIO_STR_SMALL_DATA(s)[size] = 0;
    FIO_STR_SMALL_LEN_SET(s, size);
    i = (fio_str_info_s){
        .buf = FIO_STR_SMALL_DATA(s),
        .len = size,
        .capa = FIO_STR_SMALL_CAPA(s),
    };
  } else {
    FIO_STR_BIG_DATA(s)[size] = 0;
    FIO_STR_BIG_LEN_SET(s, size);
    i = (fio_str_info_s){
        .buf = FIO_STR_BIG_DATA(s),
        .len = size,
        .capa = i.capa,
    };
  }
  return i;
}

/**
 * Prevents further manipulations to the String's content.
 */
FIO_IFUNC void FIO_NAME(FIO_STR_NAME, freeze)(FIO_STR_PTR s_) {
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s || !s_)
    return;
  FIO_STR_FREEZE_(s);
}

/**
 * Returns true if the string is frozen.
 */
FIO_IFUNC uint8_t FIO_NAME_BL(FIO_STR_NAME, frozen)(FIO_STR_PTR s_) {
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s || !s_)
    return 1;
  return FIO_STR_IS_FROZEN(s);
}

/** Returns 1 if memory was allocated and (the String must be destroyed). */
FIO_IFUNC int FIO_NAME_BL(FIO_STR_NAME, allocated)(const FIO_STR_PTR s_) {
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  return (s_ && s && !FIO_STR_IS_SMALL(s) && FIO_STR_BIG_IS_DYNAMIC(s));
}

/**
 * Binary comparison returns `1` if both strings are equal and `0` if not.
 */
FIO_IFUNC int FIO_NAME_BL(FIO_STR_NAME, eq)(const FIO_STR_PTR str1_,
                                            const FIO_STR_PTR str2_) {
  if (str1_ == str2_)
    return 1;
  FIO_NAME(FIO_STR_NAME, s) *const str1 =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(str1_);
  FIO_NAME(FIO_STR_NAME, s) *const str2 =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(str2_);
  if (str1 == str2)
    return 1;
  if (!str1 || !str2)
    return 0;
  fio_str_info_s s1 = FIO_NAME(FIO_STR_NAME, info)(str1_);
  fio_str_info_s s2 = FIO_NAME(FIO_STR_NAME, info)(str2_);
  return FIO_STR_INFO_IS_EQ(s1, s2);
}

/**
 * Returns the string's Risky Hash value.
 *
 * Note: Hash algorithm might change without notice.
 */
FIO_IFUNC uint64_t FIO_NAME(FIO_STR_NAME, hash)(const FIO_STR_PTR s_,
                                                uint64_t seed) {
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s || !s_)
    return fio_risky_hash(NULL, 0, seed);
  if (FIO_STR_IS_SMALL(s))
    return fio_risky_hash(
        (void *)FIO_STR_SMALL_DATA(s), FIO_STR_SMALL_LEN(s), seed);
  return fio_risky_hash((void *)FIO_STR_BIG_DATA(s), FIO_STR_BIG_LEN(s), seed);
}

/* *****************************************************************************
String API - Content Manipulation and Review (inline)
***************************************************************************** */

/**
 * Writes data at the end of the String (similar to `fio_str_insert` with the
 * argument `pos == -1`).
 */
FIO_IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, write)(FIO_STR_PTR s_,
                                                       const void *src,
                                                       size_t src_len) {
  FIO_NAME(FIO_STR_NAME, s) *s = (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s_ || !s || !src_len || FIO_STR_IS_FROZEN(s))
    return FIO_NAME(FIO_STR_NAME, info)(s_);
  size_t const org_len = FIO_NAME(FIO_STR_NAME, len)(s_);
  fio_str_info_s state = FIO_NAME(FIO_STR_NAME, resize)(s_, src_len + org_len);
  if (src)
    memcpy(state.buf + org_len, src, src_len);
  return state;
}

/* *****************************************************************************


                             String Implementation

                               IMPLEMENTATION


***************************************************************************** */
#ifdef FIO_EXTERN_COMPLETE

/* *****************************************************************************
String Implementation - Memory management
***************************************************************************** */

/**
 * Reserves at least `amount` of bytes for the string's data (reserved count
 * includes used data).
 *
 * Returns the current state of the String.
 */
SFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME,
                              FIO_STR_RESERVE_NAME)(FIO_STR_PTR s_,
                                                    size_t amount) {
  fio_str_info_s i = {0};
  FIO_NAME(FIO_STR_NAME, s) *const s =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s_ || !s || FIO_STR_IS_FROZEN(s)) {
    i = FIO_NAME(FIO_STR_NAME, info)(s_);
    return i;
  }
  /* result is an embedded string */
  if (amount <= FIO_STR_SMALL_CAPA(s)) {
    if (!FIO_STR_IS_SMALL(s)) {
      /* shrink from allocated(?) string */
      FIO_NAME(FIO_STR_NAME, s) tmp = FIO_STR_INIT;
      FIO_NAME(FIO_STR_NAME, init_copy)
      ((FIO_STR_PTR)FIO_PTR_TAG(&tmp),
       FIO_STR_BIG_DATA(s),
       ((FIO_STR_SMALL_CAPA(s) > FIO_STR_BIG_LEN(s)) ? FIO_STR_BIG_LEN(s)
                                                     : FIO_STR_SMALL_CAPA(s)));
      if (FIO_STR_BIG_IS_DYNAMIC(s))
        FIO_STR_BIG_FREE_BUF(s);
      *s = tmp;
    }
    i = (fio_str_info_s){
        .buf = FIO_STR_SMALL_DATA(s),
        .len = FIO_STR_SMALL_LEN(s),
        .capa = FIO_STR_SMALL_CAPA(s),
    };
    return i;
  }
  /* round up to allocation boundary */
  amount = FIO_STR_CAPA2WORDS(amount);
  if (FIO_STR_IS_SMALL(s)) {
    /* from small to big */
    FIO_NAME(FIO_STR_NAME, s) tmp = FIO_STR_INIT;
    FIO_NAME(FIO_STR_NAME, init_copy)
    ((FIO_STR_PTR)FIO_PTR_TAG(&tmp), NULL, amount);
    memcpy(
        FIO_STR_BIG_DATA(&tmp), FIO_STR_SMALL_DATA(s), FIO_STR_SMALL_CAPA(s));
    FIO_STR_BIG_LEN_SET(&tmp, FIO_STR_SMALL_LEN(s));
    *s = tmp;
    i = (fio_str_info_s){
        .buf = FIO_STR_BIG_DATA(s),
        .len = FIO_STR_BIG_LEN(s),
        .capa = amount,
    };
    return i;
  } else if (FIO_STR_BIG_IS_DYNAMIC(s) && FIO_STR_BIG_CAPA(s) == amount) {
    i = (fio_str_info_s){
        .buf = FIO_STR_BIG_DATA(s),
        .len = FIO_STR_BIG_LEN(s),
        .capa = amount,
    };
  } else {
    /* from big to big - grow / shrink */
    size_t data_len = FIO_STR_BIG_LEN(s);
    const size_t __attribute__((unused)) old_capa = FIO_STR_BIG_CAPA(s);
    if (data_len > amount) {
      /* truncate */
      data_len = amount;
      FIO_STR_BIG_LEN_SET(s, data_len);
    }
    char *tmp = NULL;
    if (FIO_STR_BIG_IS_DYNAMIC(s)) {
      tmp = (char *)FIO_MEM_REALLOC_(
          FIO_STR_BIG_DATA(s), old_capa, amount + 1, data_len);
      (void)old_capa; /* might not be used by macro */
    } else {
      tmp = (char *)FIO_MEM_CALLOC_(amount + 1, sizeof(char));
      if (tmp) {
        s->special = 0;
        if (data_len)
          memcpy(tmp, FIO_STR_BIG_DATA(s), data_len);
      }
    }
    if (tmp) {
      tmp[data_len] = 0;
      FIO_STR_BIG_DATA(s) = tmp;
      FIO_STR_BIG_CAPA_SET(s, amount);
    } else {
      amount = FIO_STR_BIG_CAPA(s);
    }
    i = (fio_str_info_s){
        .buf = FIO_STR_BIG_DATA(s),
        .len = data_len,
        .capa = amount,
    };
  }
  return i;
}

/* *****************************************************************************
String Implementation - UTF-8 State
***************************************************************************** */

#ifndef FIO_STR_UTF8_CODE_POINT
/**
 * Maps the first 5 bits in a byte (0b11111xxx) to a UTF-8 codepoint length.
 *
 * Codepoint length 0 == error.
 *
 * The first valid length can be any value between 1 to 4.
 *
 * A continuation byte (second, third or forth) valid length marked as 5.
 *
 * To map was populated using the following Ruby script:
 *
 *      map = []; 32.times { map << 0 }; (0..0b1111).each {|i| map[i] = 1} ;
 *      (0b10000..0b10111).each {|i| map[i] = 5} ;
 *      (0b11000..0b11011).each {|i| map[i] = 2} ;
 *      (0b11100..0b11101).each {|i| map[i] = 3} ;
 *      map[0b11110] = 4; map;
 */
static __attribute__((unused))
uint8_t fio__str_utf8_map[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               5, 5, 5, 5, 5, 5, 5, 5, 2, 2, 2, 2, 3, 3, 4, 0};

/**
 * Advances the `ptr` by one utf-8 character, placing the value of the UTF-8
 * character into the i32 variable (which must be a signed integer with 32bits
 * or more). On error, `i32` will be equal to `-1` and `ptr` will not step
 * forwards.
 *
 * The `end` value is only used for overflow protection.
 */
#define FIO_STR_UTF8_CODE_POINT(ptr, end, i32)                                 \
  do {                                                                         \
    switch (fio__str_utf8_map[((uint8_t *)(ptr))[0] >> 3]) {                   \
    case 1:                                                                    \
      (i32) = ((uint8_t *)(ptr))[0];                                           \
      ++(ptr);                                                                 \
      break;                                                                   \
    case 2:                                                                    \
      if (((ptr) + 2 > (end)) ||                                               \
          fio__str_utf8_map[((uint8_t *)(ptr))[1] >> 3] != 5) {                \
        (i32) = -1;                                                            \
        break;                                                                 \
      }                                                                        \
      (i32) =                                                                  \
          ((((uint8_t *)(ptr))[0] & 31) << 6) | (((uint8_t *)(ptr))[1] & 63);  \
      (ptr) += 2;                                                              \
      break;                                                                   \
    case 3:                                                                    \
      if (((ptr) + 3 > (end)) ||                                               \
          fio__str_utf8_map[((uint8_t *)(ptr))[1] >> 3] != 5 ||                \
          fio__str_utf8_map[((uint8_t *)(ptr))[2] >> 3] != 5) {                \
        (i32) = -1;                                                            \
        break;                                                                 \
      }                                                                        \
      (i32) = ((((uint8_t *)(ptr))[0] & 15) << 12) |                           \
              ((((uint8_t *)(ptr))[1] & 63) << 6) |                            \
              (((uint8_t *)(ptr))[2] & 63);                                    \
      (ptr) += 3;                                                              \
      break;                                                                   \
    case 4:                                                                    \
      if (((ptr) + 4 > (end)) ||                                               \
          fio__str_utf8_map[((uint8_t *)(ptr))[1] >> 3] != 5 ||                \
          fio__str_utf8_map[((uint8_t *)(ptr))[2] >> 3] != 5 ||                \
          fio__str_utf8_map[((uint8_t *)(ptr))[3] >> 3] != 5) {                \
        (i32) = -1;                                                            \
        break;                                                                 \
      }                                                                        \
      (i32) = ((((uint8_t *)(ptr))[0] & 7) << 18) |                            \
              ((((uint8_t *)(ptr))[1] & 63) << 12) |                           \
              ((((uint8_t *)(ptr))[2] & 63) << 6) |                            \
              (((uint8_t *)(ptr))[3] & 63);                                    \
      (ptr) += 4;                                                              \
      break;                                                                   \
    default:                                                                   \
      (i32) = -1;                                                              \
      break;                                                                   \
    }                                                                          \
  } while (0);
#endif

/** Returns 1 if the String is UTF-8 valid and 0 if not. */
SFUNC size_t FIO_NAME(FIO_STR_NAME, utf8_valid)(FIO_STR_PTR s_) {
  FIO_NAME(FIO_STR_NAME, s) *s = (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s || !s_)
    return 0;
  fio_str_info_s state = FIO_NAME(FIO_STR_NAME, info)(s_);
  if (!state.len)
    return 1;
  char *const end = state.buf + state.len;
  int32_t c = 0;
  do {
    FIO_STR_UTF8_CODE_POINT(state.buf, end, c);
  } while (c > 0 && state.buf < end);
  return state.buf == end && c >= 0;
}

/** Returns the String's length in UTF-8 characters. */
SFUNC size_t FIO_NAME(FIO_STR_NAME, utf8_len)(FIO_STR_PTR s_) {
  fio_str_info_s state = FIO_NAME(FIO_STR_NAME, info)(s_);
  if (!state.len)
    return 0;
  char *end = state.buf + state.len;
  size_t utf8len = 0;
  int32_t c = 0;
  do {
    ++utf8len;
    FIO_STR_UTF8_CODE_POINT(state.buf, end, c);
  } while (c > 0 && state.buf < end);
  if (state.buf != end || c == -1) {
    /* invalid */
    return 0;
  }
  return utf8len;
}

/**
 * Takes a UTF-8 character selection information (UTF-8 position and length)
 * and updates the same variables so they reference the raw byte slice
 * information.
 *
 * If the String isn't UTF-8 valid up to the requested selection, than `pos`
 * will be updated to `-1` otherwise values are always positive.
 *
 * The returned `len` value may be shorter than the original if there wasn't
 * enough data left to accommodate the requested length. When a `len` value of
 * `0` is returned, this means that `pos` marks the end of the String.
 *
 * Returns -1 on error and 0 on success.
 */
SFUNC int FIO_NAME(FIO_STR_NAME,
                   utf8_select)(FIO_STR_PTR s_, intptr_t *pos, size_t *len) {
  fio_str_info_s state = FIO_NAME(FIO_STR_NAME, info)(s_);
  int32_t c = 0;
  char *p = state.buf;
  char *const end = state.buf + state.len;
  size_t start;

  if (!state.buf)
    goto error;
  if (!state.len || *pos == -1)
    goto at_end;

  if (*pos) {
    if ((*pos) > 0) {
      start = *pos;
      while (start && p < end && c >= 0) {
        FIO_STR_UTF8_CODE_POINT(p, end, c);
        --start;
      }
      if (c == -1)
        goto error;
      if (start || p >= end)
        goto at_end;
      *pos = p - state.buf;
    } else {
      /* walk backwards */
      p = state.buf + state.len - 1;
      c = 0;
      ++*pos;
      do {
        switch (fio__str_utf8_map[((uint8_t *)p)[0] >> 3]) {
        case 5:
          ++c;
          break;
        case 4:
          if (c != 3)
            goto error;
          c = 0;
          ++(*pos);
          break;
        case 3:
          if (c != 2)
            goto error;
          c = 0;
          ++(*pos);
          break;
        case 2:
          if (c != 1)
            goto error;
          c = 0;
          ++(*pos);
          break;
        case 1:
          if (c)
            goto error;
          ++(*pos);
          break;
        default:
          goto error;
        }
        --p;
      } while (p > state.buf && *pos);
      if (c)
        goto error;
      ++p; /* There's always an extra back-step */
      *pos = (p - state.buf);
    }
  }

  /* find end */
  start = *len;
  while (start && p < end && c >= 0) {
    FIO_STR_UTF8_CODE_POINT(p, end, c);
    --start;
  }
  if (c == -1 || p > end)
    goto error;
  *len = p - (state.buf + (*pos));
  return 0;

at_end:
  *pos = state.len;
  *len = 0;
  return 0;
error:
  *pos = -1;
  *len = 0;
  return -1;
}

/* *****************************************************************************
String Implementation - Content Manipulation and Review
***************************************************************************** */

/**
 * Writes a number at the end of the String using normal base 10 notation.
 */
IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, write_i)(FIO_STR_PTR s_,
                                                     int64_t num) {
  /* because fio_ltoa uses an internal buffer, we "save" a `memcpy` loop and
   * minimize memory allocations by re-implementing the same logic in a
   * dedicated fasion. */
  FIO_NAME(FIO_STR_NAME, s) *s = (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  if (!s || FIO_STR_IS_FROZEN(s))
    return FIO_NAME(FIO_STR_NAME, info)(s_);
  fio_str_info_s i;
  if (!num)
    goto write_zero;
  {
    char buf[22];
    uint64_t l = 0;
    uint8_t neg = 0;
    int64_t t = num / 10;
    if (num < 0) {
      num = 0 - num; /* might fail due to overflow, but fixed with tail (t) */
      t = (int64_t)0 - t;
      neg = 1;
    }
    while (num) {
      buf[l++] = '0' + (num - (t * 10));
      num = t;
      t = num / 10;
    }
    if (neg) {
      buf[l++] = '-';
    }
    i = FIO_NAME(FIO_STR_NAME, resize)(s_, FIO_NAME(FIO_STR_NAME, len)(s_) + l);

    while (l) {
      --l;
      i.buf[i.len - (l + 1)] = buf[l];
    }
  }
  return i;
write_zero:
  i = FIO_NAME(FIO_STR_NAME, resize)(s_, FIO_NAME(FIO_STR_NAME, len)(s_) + 1);
  i.buf[i.len - 1] = '0';
  return i;
}

/**
 * Writes a number at the end of the String using Hex (base 16) notation.
 */
IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, write_hex)(FIO_STR_PTR s,
                                                       int64_t num) {
  /* using base 16 with fio_ltoa, buffer might minimize memory allocation. */
  char buf[(sizeof(int64_t) * 2) + 8];
  size_t written = fio_ltoa(buf, num, 16);
  return FIO_NAME(FIO_STR_NAME, write)(s, buf, written);
}

/**
 * Appends the `src` String to the end of the `dest` String.
 *
 * If `dest` is empty, the resulting Strings will be equal.
 */
IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, concat)(FIO_STR_PTR dest_,
                                                    FIO_STR_PTR const src_) {
  FIO_NAME(FIO_STR_NAME, s) *dest =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(dest_);
  FIO_NAME(FIO_STR_NAME, s) *src =
      (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(src_);
  if (!dest || !src || !dest_ || !src_ || FIO_STR_IS_FROZEN(dest)) {
    return FIO_NAME(FIO_STR_NAME, info)(dest_);
  }
  fio_str_info_s src_state = FIO_NAME(FIO_STR_NAME, info)(src_);
  if (!src_state.len)
    return FIO_NAME(FIO_STR_NAME, info)(dest_);
  const size_t old_len = FIO_NAME(FIO_STR_NAME, len)(dest_);
  fio_str_info_s state =
      FIO_NAME(FIO_STR_NAME, resize)(dest_, src_state.len + old_len);
  memcpy(state.buf + old_len, src_state.buf, src_state.len);
  return state;
}

/**
 * Replaces the data in the String - replacing `old_len` bytes starting at
 * `start_pos`, with the data at `src` (`src_len` bytes long).
 *
 * Negative `start_pos` values are calculated backwards, `-1` == end of
 * String.
 *
 * When `old_len` is zero, the function will insert the data at `start_pos`.
 *
 * If `src_len == 0` than `src` will be ignored and the data marked for
 * replacement will be erased.
 */
SFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, replace)(FIO_STR_PTR s_,
                                                     intptr_t start_pos,
                                                     size_t old_len,
                                                     const void *src,
                                                     size_t src_len) {
  FIO_NAME(FIO_STR_NAME, s) *s = (FIO_NAME(FIO_STR_NAME, s) *)FIO_PTR_UNTAG(s_);
  fio_str_info_s state = FIO_NAME(FIO_STR_NAME, info)(s_);
  if (!s_ || !s || FIO_STR_IS_FROZEN(s) || (!old_len && !src_len) ||
      (!src && src_len))
    return state;

  if (start_pos < 0) {
    /* backwards position indexing */
    start_pos += FIO_NAME(FIO_STR_NAME, len)(s_) + 1;
    if (start_pos < 0)
      start_pos = 0;
  }

  if (start_pos + old_len >= state.len) {
    /* old_len overflows the end of the String */
    if (FIO_STR_IS_SMALL(s)) {
      FIO_STR_SMALL_LEN_SET(s, start_pos);
    } else {
      FIO_STR_BIG_LEN_SET(s, start_pos);
    }
    return FIO_NAME(FIO_STR_NAME, write)(s_, src, src_len);
  }

  /* data replacement is now always in the middle (or start) of the String */
  const size_t new_size = state.len + (src_len - old_len);

  if (old_len != src_len) {
    /* there's an offset requiring an adjustment */
    if (old_len < src_len) {
      /* make room for new data */
      const size_t offset = src_len - old_len;
      state = FIO_NAME(FIO_STR_NAME, resize)(s_, state.len + offset);
    }
    memmove(state.buf + start_pos + src_len,
            state.buf + start_pos + old_len,
            (state.len - start_pos) - old_len);
  }
  if (src_len) {
    memcpy(state.buf + start_pos, src, src_len);
  }

  return FIO_NAME(FIO_STR_NAME, resize)(s_, new_size);
}

/**
 * Writes to the String using a vprintf like interface.
 *
 * Data is written to the end of the String.
 */
SFUNC fio_str_info_s __attribute__((format(printf, 2, 0)))
FIO_NAME(FIO_STR_NAME,
         vprintf)(FIO_STR_PTR s_, const char *format, va_list argv) {
  va_list argv_cpy;
  va_copy(argv_cpy, argv);
  int len = vsnprintf(NULL, 0, format, argv_cpy);
  va_end(argv_cpy);
  if (len <= 0)
    return FIO_NAME(FIO_STR_NAME, info)(s_);
  fio_str_info_s state =
      FIO_NAME(FIO_STR_NAME, resize)(s_, len + FIO_NAME(FIO_STR_NAME, len)(s_));
  if (state.capa >= (size_t)len)
    vsnprintf(state.buf + (state.len - len), len + 1, format, argv);
  return state;
}

/**
 * Writes to the String using a printf like interface.
 *
 * Data is written to the end of the String.
 */
SFUNC fio_str_info_s __attribute__((format(printf, 2, 3)))
FIO_NAME(FIO_STR_NAME, printf)(FIO_STR_PTR s_, const char *format, ...) {
  va_list argv;
  va_start(argv, format);
  fio_str_info_s state = FIO_NAME(FIO_STR_NAME, vprintf)(s_, format, argv);
  va_end(argv);
  return state;
}

/* *****************************************************************************
String API - C / JSON escaping
***************************************************************************** */

/* constant time (non-branching) if statement used in a loop as a helper */
#define FIO_STR_WRITE_ESCAPED_CT_OR(cond, a, b)                                \
  ((b) ^                                                                       \
   ((0 - ((((cond) | (0 - (cond))) >> ((sizeof((cond)) << 3) - 1)) & 1)) &     \
    ((a) ^ (b))))

/**
 * Writes data at the end of the String, escaping the data using JSON semantics.
 *
 * The JSON semantic are common to many programming languages, promising a UTF-8
 * String while making it easy to read and copy the string during debugging.
 */
IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, write_escape)(FIO_STR_PTR s,
                                                          const void *src_,
                                                          size_t len) {
  // clang-format off
  const char escape_hex_chars[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  // clang-format on
  const uint8_t *src = (const uint8_t *)src_;
  size_t extra_len = 0;
  size_t at = 0;
  uint8_t set_at = 1;

  /* collect escaping requiremnents */
  for (size_t i = 0; i < len; ++i) {
    /* skip valid ascii */
    if ((src[i] > 34 && src[i] < 127 && src[i] != '\\') || src[i] == '!' ||
        src[i] == ' ')
      continue;
    /* skip valid UTF-8 */
    switch (fio__str_utf8_map[src[i] >> 3]) {
    case 4:
      if (fio__str_utf8_map[src[i + 3] >> 3] != 5) {
        break; /* from switch */
      }
    /* fallthrough */
    case 3:
      if (fio__str_utf8_map[src[i + 2] >> 3] != 5) {
        break; /* from switch */
      }
    /* fallthrough */
    case 2:
      if (fio__str_utf8_map[src[i + 1] >> 3] != 5) {
        break; /* from switch */
      }
      i += fio__str_utf8_map[src[i] >> 3] - 1;
      continue;
    }
    /* store first instance of character that needs escaping */
    at = FIO_STR_WRITE_ESCAPED_CT_OR(set_at, i, at);
    set_at = 0;

    /* count extra bytes */
    switch (src[i]) {
    case '\b': /* fallthrough */
    case '\f': /* fallthrough */
    case '\n': /* fallthrough */
    case '\r': /* fallthrough */
    case '\t': /* fallthrough */
    case '"':  /* fallthrough */
    case '\\': /* fallthrough */
    case '/':  /* fallthrough */
      ++extra_len;
      break;
    default:
      /* escaping all control charactes and non-UTF-8 characters */
      extra_len += 5;
    }
  }
  /* reserve space and copy any valid "head" */
  fio_str_info_s dest;
  {
    const size_t org_len = FIO_NAME(FIO_STR_NAME, len)(s);
#if !FIO_STR_OPTIMIZE4IMMUTABILITY
    /* often, after `write_escape` come quotes */
    FIO_NAME(FIO_STR_NAME, reserve)(s, org_len + extra_len + len + 4);
#endif
    dest = FIO_NAME(FIO_STR_NAME, resize)(s, org_len + extra_len + len);
    dest.len = org_len;
  }
  dest.buf += dest.len;
  /* is escaping required? - simple memcpy if we don't need to escape */
  if (set_at) {
    memcpy(dest.buf, src, len);
    dest.buf -= dest.len;
    dest.len += len;
    return dest;
  }
  /* simple memcpy until first char that needs escaping */
  if (at >= 8) {
    memcpy(dest.buf, src, at);
  } else {
    at = 0;
  }
  /* start escaping */
  for (size_t i = at; i < len; ++i) {
    /* skip valid ascii */
    if ((src[i] > 34 && src[i] < 127 && src[i] != '\\') || src[i] == '!' ||
        src[i] == ' ') {
      dest.buf[at++] = src[i];
      continue;
    }
    /* skip valid UTF-8 */
    switch (fio__str_utf8_map[src[i] >> 3]) {
    case 4:
      if (fio__str_utf8_map[src[i + 3] >> 3] != 5) {
        break; /* from switch */
      }
    /* fallthrough */
    case 3:
      if (fio__str_utf8_map[src[i + 2] >> 3] != 5) {
        break; /* from switch */
      }
    /* fallthrough */
    case 2:
      if (fio__str_utf8_map[src[i + 1] >> 3] != 5) {
        break; /* from switch */
      }
      switch (fio__str_utf8_map[src[i] >> 3]) {
      case 4:
        dest.buf[at++] = src[i++]; /* fallthrough */
      case 3:
        dest.buf[at++] = src[i++]; /* fallthrough */
      case 2:
        dest.buf[at++] = src[i++];
        dest.buf[at++] = src[i];
      }
      continue;
    }

    /* write escape sequence */
    dest.buf[at++] = '\\';
    switch (src[i]) {
    case '\b':
      dest.buf[at++] = 'b';
      break;
    case '\f':
      dest.buf[at++] = 'f';
      break;
    case '\n':
      dest.buf[at++] = 'n';
      break;
    case '\r':
      dest.buf[at++] = 'r';
      break;
    case '\t':
      dest.buf[at++] = 't';
      break;
    case '"':
      dest.buf[at++] = '"';
      break;
    case '\\':
      dest.buf[at++] = '\\';
      break;
    case '/':
      dest.buf[at++] = '/';
      break;
    default:
      /* escaping all control charactes and non-UTF-8 characters */
      if (src[i] < 127) {
        dest.buf[at++] = 'u';
        dest.buf[at++] = '0';
        dest.buf[at++] = '0';
        dest.buf[at++] = escape_hex_chars[src[i] >> 4];
        dest.buf[at++] = escape_hex_chars[src[i] & 15];
      } else {
        /* non UTF-8 data... encode as...? */
        dest.buf[at++] = 'x';
        dest.buf[at++] = escape_hex_chars[src[i] >> 4];
        dest.buf[at++] = escape_hex_chars[src[i] & 15];
      }
    }
  }
  return FIO_NAME(FIO_STR_NAME, resize)(s, dest.len + at);
}

/**
 * Writes an escaped data into the string after unescaping the data.
 */
IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, write_unescape)(FIO_STR_PTR s,
                                                            const void *src_,
                                                            size_t len) {
  const uint8_t is_hex[] = {
      0,  0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0,  0,  0,
      0,  0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0,  0,  0,
      0,  0,  0,  0, 0, 0,  0,  0,  1,  2,  3,  4, 5, 6, 7, 8, 9, 10, 0,  0,
      0,  0,  0,  0, 0, 11, 12, 13, 14, 15, 16, 0, 0, 0, 0, 0, 0, 0,  0,  0,
      0,  0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 11, 12, 13,
      14, 15, 16, 0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0,  0,  0,
      0,  0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0,  0,  0,
      0,  0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0,  0,  0,
      0,  0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0,  0,  0,
      0,  0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0,  0,  0,
      0,  0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0,  0,  0,
      0,  0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0,  0,  0,
      0,  0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0};

  fio_str_info_s dest;
  {
    const size_t org_len = FIO_NAME(FIO_STR_NAME, len)(s);
    dest = FIO_NAME(FIO_STR_NAME, resize)(s, org_len + len);
    dest.len = org_len;
  }
  size_t at = 0;
  const uint8_t *src = (const uint8_t *)src_;
  const uint8_t *end = src + len;
  dest.buf += dest.len;
  while (src < end) {
#if 1 /* A/B performance at a later stage */
    if (*src != '\\') {
      const uint8_t *escape_pos = (const uint8_t *)memchr(src, '\\', end - src);
      if (!escape_pos)
        escape_pos = end;
      const size_t valid_len = escape_pos - src;
      if (valid_len) {
        memmove(dest.buf + at, src, valid_len);
        at += valid_len;
        src = escape_pos;
      }
    }
#else
#if __x86_64__ || __aarch64__
    /* levarege unaligned memory access to test and copy 8 bytes at a time */
    while (src + 8 <= end) {
      const uint64_t wanted1 = 0x0101010101010101ULL * '\\';
      const uint64_t eq1 =
          ~((*((uint64_t *)src)) ^ wanted1); /* 0 == eq. inverted, all bits 1 */
      const uint64_t t0 = (eq1 & 0x7f7f7f7f7f7f7f7fllu) + 0x0101010101010101llu;
      const uint64_t t1 = (eq1 & 0x8080808080808080llu);
      if ((t0 & t1)) {
        break; /* from 8 byte seeking algorithm */
      }
      *(uint64_t *)(dest.buf + at) = *(uint64_t *)src;
      src += 8;
      at += 8;
    }
#endif
    while (src < end && *src != '\\') {
      dest.buf[at++] = *(src++);
    }
#endif
    if (end - src == 1) {
      dest.buf[at++] = *(src++);
    }
    if (src >= end)
      break;
    /* escaped data - src[0] == '\\' */
    ++src;
    switch (src[0]) {
    case 'b':
      dest.buf[at++] = '\b';
      ++src;
      break; /* from switch */
    case 'f':
      dest.buf[at++] = '\f';
      ++src;
      break; /* from switch */
    case 'n':
      dest.buf[at++] = '\n';
      ++src;
      break; /* from switch */
    case 'r':
      dest.buf[at++] = '\r';
      ++src;
      break; /* from switch */
    case 't':
      dest.buf[at++] = '\t';
      ++src;
      break; /* from switch */
    case 'u': {
      /* test UTF-8 notation */
      if (is_hex[src[1]] && is_hex[src[2]] && is_hex[src[3]] &&
          is_hex[src[4]]) {
        uint32_t u =
            ((((is_hex[src[1]] - 1) << 4) | (is_hex[src[2]] - 1)) << 8) |
            (((is_hex[src[3]] - 1) << 4) | (is_hex[src[4]] - 1));
        if ((((is_hex[src[1]] - 1) << 4) | (is_hex[src[2]] - 1)) == 0xD8U &&
            src[5] == '\\' && src[6] == 'u' && is_hex[src[7]] &&
            is_hex[src[8]] && is_hex[src[9]] && is_hex[src[10]]) {
          /* surrogate-pair */
          u = (u & 0x03FF) << 10;
          u |= ((((((is_hex[src[7]] - 1) << 4) | (is_hex[src[8]] - 1)) << 8) |
                 (((is_hex[src[9]] - 1) << 4) | (is_hex[src[10]] - 1))) &
                0x03FF);
          u += 0x10000;
          src += 6;
        }
        if (u <= 127) {
          dest.buf[at++] = u;
        } else if (u <= 2047) {
          dest.buf[at++] = 192 | (u >> 6);
          dest.buf[at++] = 128 | (u & 63);
        } else if (u <= 65535) {
          dest.buf[at++] = 224 | (u >> 12);
          dest.buf[at++] = 128 | ((u >> 6) & 63);
          dest.buf[at++] = 128 | (u & 63);
        } else {
          dest.buf[at++] = 240 | ((u >> 18) & 7);
          dest.buf[at++] = 128 | ((u >> 12) & 63);
          dest.buf[at++] = 128 | ((u >> 6) & 63);
          dest.buf[at++] = 128 | (u & 63);
        }
        src += 5;
        break; /* from switch */
      } else
        goto invalid_escape;
    }
    case 'x': { /* test for hex notation */
      if (is_hex[src[1]] && is_hex[src[2]]) {
        dest.buf[at++] = ((is_hex[src[1]] - 1) << 4) | (is_hex[src[2]] - 1);
        src += 3;
        break; /* from switch */
      } else
        goto invalid_escape;
    }
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7': { /* test for octal notation */
      if (src[0] >= '0' && src[0] <= '7' && src[1] >= '0' && src[1] <= '7') {
        dest.buf[at++] = ((src[0] - '0') << 3) | (src[1] - '0');
        src += 2;
        break; /* from switch */
      } else
        goto invalid_escape;
    }
    case '"':
    case '\\':
    case '/':
    /* fallthrough */
    default:
    invalid_escape:
      dest.buf[at++] = *(src++);
    }
  }
  return FIO_NAME(FIO_STR_NAME, resize)(s, dest.len + at);
}

#undef FIO_STR_WRITE_ESCAPED_CT_OR

/* *****************************************************************************
String - Base64 support
***************************************************************************** */

/**
 * Writes data at the end of the String, encoding the data as Base64 encoded
 * data.
 */
IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME,
                              write_base64enc)(FIO_STR_PTR s_,
                                               const void *data,
                                               size_t len,
                                               uint8_t url_encoded) {
  if (!s_ || !FIO_PTR_UNTAG(s_) || !len ||
      FIO_NAME_BL(FIO_STR_NAME, frozen)(s_))
    return FIO_NAME(FIO_STR_NAME, info)(s_);

  /* the base64 encoding array */
  const char *encoding;
  if (url_encoded == 0) {
    encoding =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
  } else {
    encoding =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";
  }

  /* base64 length and padding information */
  int groups = len / 3;
  const int mod = len - (groups * 3);
  const int target_size = (groups + (mod != 0)) * 4;
  const uint32_t org_len = FIO_NAME(FIO_STR_NAME, len)(s_);
  fio_str_info_s i = FIO_NAME(FIO_STR_NAME, resize)(s_, org_len + target_size);
  char *writer = i.buf + org_len;
  const unsigned char *reader = (const unsigned char *)data;

  /* write encoded data */
  while (groups) {
    --groups;
    const unsigned char tmp1 = *(reader++);
    const unsigned char tmp2 = *(reader++);
    const unsigned char tmp3 = *(reader++);
    *(writer++) = encoding[(tmp1 >> 2) & 63];
    *(writer++) = encoding[(((tmp1 & 3) << 4) | ((tmp2 >> 4) & 15))];
    *(writer++) = encoding[((tmp2 & 15) << 2) | ((tmp3 >> 6) & 3)];
    *(writer++) = encoding[tmp3 & 63];
  }

  /* write padding / ending */
  switch (mod) {
  case 2: {
    const unsigned char tmp1 = *(reader++);
    const unsigned char tmp2 = *(reader++);
    *(writer++) = encoding[(tmp1 >> 2) & 63];
    *(writer++) = encoding[((tmp1 & 3) << 4) | ((tmp2 >> 4) & 15)];
    *(writer++) = encoding[((tmp2 & 15) << 2)];
    *(writer++) = '=';
  } break;
  case 1: {
    const unsigned char tmp1 = *(reader++);
    *(writer++) = encoding[(tmp1 >> 2) & 63];
    *(writer++) = encoding[(tmp1 & 3) << 4];
    *(writer++) = '=';
    *(writer++) = '=';
  } break;
  }
  return i;
}

/**
 * Writes decoded base64 data to the end of the String.
 */
IFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME,
                              write_base64dec)(FIO_STR_PTR s_,
                                               const void *encoded_,
                                               size_t len) {
  /*
  Base64 decoding array. Generation script (Ruby):

a = []; a[255] = 0
s = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=".bytes;
s.length.times {|i| a[s[i]] = (i << 1) | 1 };
s = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+,".bytes;
s.length.times {|i| a[s[i]] = (i << 1) | 1 };
s = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_".bytes;
s.length.times {|i| a[s[i]] = (i << 1) | 1 }; a.map!{ |i| i.to_i }; a

  */
  const uint8_t base64_decodes[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   125, 127,
      125, 0,   127, 105, 107, 109, 111, 113, 115, 117, 119, 121, 123, 0,   0,
      0,   129, 0,   0,   0,   1,   3,   5,   7,   9,   11,  13,  15,  17,  19,
      21,  23,  25,  27,  29,  31,  33,  35,  37,  39,  41,  43,  45,  47,  49,
      51,  0,   0,   0,   0,   127, 0,   53,  55,  57,  59,  61,  63,  65,  67,
      69,  71,  73,  75,  77,  79,  81,  83,  85,  87,  89,  91,  93,  95,  97,
      99,  101, 103, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0};
#define FIO_BASE64_BITVAL(x) ((base64_decodes[(x)] >> 1) & 63)

  if (!s_ || !FIO_PTR_UNTAG(s_) || !len || !encoded_ ||
      FIO_NAME_BL(FIO_STR_NAME, frozen)(s_))
    return FIO_NAME(FIO_STR_NAME, info)(s_);

  const uint8_t *encoded = (const uint8_t *)encoded_;

  /* skip unknown data at end */
  while (len && !base64_decodes[encoded[len - 1]]) {
    len--;
  }

  /* reserve memory space */
  const uint32_t org_len = FIO_NAME(FIO_STR_NAME, len)(s_);
  fio_str_info_s i =
      FIO_NAME(FIO_STR_NAME, resize)(s_, org_len + ((len >> 2) * 3) + 3);
  i.buf += org_len;

  /* decoded and count actual length */
  int32_t written = 0;
  uint8_t tmp1, tmp2, tmp3, tmp4;
  while (len >= 4) {
    if (isspace((*encoded))) {
      while (len && isspace((*encoded))) {
        len--;
        encoded++;
      }
      continue;
    }
    tmp1 = *(encoded++);
    tmp2 = *(encoded++);
    tmp3 = *(encoded++);
    tmp4 = *(encoded++);
    if (!base64_decodes[tmp1] || !base64_decodes[tmp2] ||
        !base64_decodes[tmp3] || !base64_decodes[tmp4]) {
      return (fio_str_info_s){.buf = NULL};
    }
    *(i.buf++) =
        (FIO_BASE64_BITVAL(tmp1) << 2) | (FIO_BASE64_BITVAL(tmp2) >> 4);
    *(i.buf++) =
        (FIO_BASE64_BITVAL(tmp2) << 4) | (FIO_BASE64_BITVAL(tmp3) >> 2);
    *(i.buf++) = (FIO_BASE64_BITVAL(tmp3) << 6) | (FIO_BASE64_BITVAL(tmp4));
    /* make sure we don't loop forever */
    len -= 4;
    /* count written bytes */
    written += 3;
  }
  /* skip white spaces */
  while (len && isspace((*encoded))) {
    len--;
    encoded++;
  }
  /* decode "tail" - if any (mis-encoded, shouldn't happen) */
  tmp1 = 0;
  tmp2 = 0;
  tmp3 = 0;
  tmp4 = 0;
  switch (len) {
  case 1:
    tmp1 = *(encoded++);
    if (!base64_decodes[tmp1]) {
      return (fio_str_info_s){.buf = NULL};
    }
    *(i.buf++) = FIO_BASE64_BITVAL(tmp1);
    written += 1;
    break;
  case 2:
    tmp1 = *(encoded++);
    tmp2 = *(encoded++);
    if (!base64_decodes[tmp1] || !base64_decodes[tmp2]) {
      return (fio_str_info_s){.buf = NULL};
    }
    *(i.buf++) =
        (FIO_BASE64_BITVAL(tmp1) << 2) | (FIO_BASE64_BITVAL(tmp2) >> 6);
    *(i.buf++) = (FIO_BASE64_BITVAL(tmp2) << 4);
    written += 2;
    break;
  case 3:
    tmp1 = *(encoded++);
    tmp2 = *(encoded++);
    tmp3 = *(encoded++);
    if (!base64_decodes[tmp1] || !base64_decodes[tmp2] ||
        !base64_decodes[tmp3]) {
      return (fio_str_info_s){.buf = NULL};
    }
    *(i.buf++) =
        (FIO_BASE64_BITVAL(tmp1) << 2) | (FIO_BASE64_BITVAL(tmp2) >> 6);
    *(i.buf++) =
        (FIO_BASE64_BITVAL(tmp2) << 4) | (FIO_BASE64_BITVAL(tmp3) >> 2);
    *(i.buf++) = FIO_BASE64_BITVAL(tmp3) << 6;
    written += 3;
    break;
  }
#undef FIO_BASE64_BITVAL

  if (encoded[-1] == '=') {
    i.buf--;
    written--;
    if (encoded[-2] == '=') {
      i.buf--;
      written--;
    }
    if (written < 0)
      written = 0;
  }

  // if (FIO_STR_IS_SMALL(s_))
  //   FIO_STR_SMALL_LEN_SET(s_, (org_len + written));
  // else
  //   FIO_STR_BIG_LEN_SET(s_, (org_len + written));

  return FIO_NAME(FIO_STR_NAME, resize)(s_, org_len + written);
}

/* *****************************************************************************
String - read file
***************************************************************************** */

#if FIO_HAVE_UNIX_TOOLS
#ifndef H___FIO_UNIX_TOOLS4STR_INCLUDED_H
#define H___FIO_UNIX_TOOLS4STR_INCLUDED_H
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif /* H___FIO_UNIX_TOOLS4STR_INCLUDED_H */

/**
 * Reads data from a file descriptor `fd` at offset `start_at` and pastes it's
 * contents (or a slice of it) at the end of the String. If `limit == 0`, than
 * the data will be read until EOF.
 *
 * The file should be a regular file or the operation might fail (can't be used
 * for sockets).
 *
 * The file descriptor will remain open and should be closed manually.
 *
 * Currently implemented only on POSIX systems.
 */
SFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, readfd)(FIO_STR_PTR s_,
                                                    int fd,
                                                    intptr_t start_at,
                                                    intptr_t limit) {
  struct stat f_data;
  fio_str_info_s state = {.buf = NULL};
  if (fd == -1 || fstat(fd, &f_data) == -1) {
    return state;
  }

  if (f_data.st_size <= 0 || start_at >= f_data.st_size) {
    state = FIO_NAME(FIO_STR_NAME, info)(s_);
    return state;
  }

  if (limit <= 0 || f_data.st_size < (limit + start_at)) {
    limit = f_data.st_size - start_at;
  }

  const size_t org_len = FIO_NAME(FIO_STR_NAME, len)(s_);
  size_t write_pos = org_len;
  state = FIO_NAME(FIO_STR_NAME, resize)(s_, org_len + limit);
  if (state.capa < (org_len + limit) || !state.buf) {
    return state;
  }

  while (limit) {
    /* copy up to 128Mb at a time... why? because pread might fail */
    const size_t to_read =
        (limit & (((size_t)1 << 27) - 1)) | ((!!(limit >> 27)) << 27);
    if (pread(fd, state.buf + write_pos, to_read, start_at) !=
        (ssize_t)to_read) {
      goto error;
    }
    limit -= to_read;
    write_pos += to_read;
    start_at += to_read;
  }
  return state;

error:
  FIO_NAME(FIO_STR_NAME, resize)(s_, org_len);
  state.buf = NULL;
  state.len = state.capa = 0;
  return state;
}

/**
 * Opens the file `filename` and pastes it's contents (or a slice ot it) at
 * the end of the String. If `limit == 0`, than the data will be read until
 * EOF.
 *
 * If the file can't be located, opened or read, or if `start_at` is beyond
 * the EOF position, NULL is returned in the state's `data` field.
 *
 * Currently implemented only on POSIX systems.
 */
SFUNC fio_str_info_s FIO_NAME(FIO_STR_NAME, readfile)(FIO_STR_PTR s_,
                                                      const char *filename,
                                                      intptr_t start_at,
                                                      intptr_t limit) {
  fio_str_info_s state = {.buf = NULL};
  /* POSIX implementations. */
  if (filename == NULL || !s_)
    return state;
  int file = -1;
  char *path = NULL;
  size_t path_len = 0;

  if (filename[0] == '~' && (filename[1] == '/' || filename[1] == '\\')) {
    char *home = getenv("HOME");
    if (home) {
      size_t filename_len = strlen(filename);
      size_t home_len = strlen(home);
      if ((home_len + filename_len) >= (1 << 16)) {
        /* too long */
        return state;
      }
      if (home[home_len - 1] == '/' || home[home_len - 1] == '\\')
        --home_len;
      path_len = home_len + filename_len - 1;
      path = (char *)FIO_MEM_CALLOC_(sizeof(*path), path_len + 1);
      if (!path)
        return state;
      memcpy(path, home, home_len);
      memcpy(path + home_len, filename + 1, filename_len);
      path[path_len] = 0;
      filename = path;
    }
  }

  file = open(filename, O_RDONLY);
  if (-1 == file) {
    goto finish;
  }
  state = FIO_NAME(FIO_STR_NAME, readfd)(s_, file, start_at, limit);
  close(file);

finish:
  if (path) {
    FIO_MEM_FREE_(path, path_len + 1);
  }
  return state;
}

#endif /* FIO_HAVE_UNIX_TOOLS */

/* *****************************************************************************


                                    String Test


***************************************************************************** */
#ifdef FIO_STR_WRITE_TEST_FUNC

/**
 * Tests the fio_str functionality.
 */
SFUNC void FIO_NAME(FIO_STR_NAME, __dynamic_test)(void) {
  FIO_NAME(FIO_STR_NAME, s) str = {0}; /* test zeroed out memory */
#define FIO__STR_SMALL_CAPA FIO_STR_SMALL_CAPA(&str)
  fprintf(
      stderr,
      "* Testing core string features for " FIO_MACRO2STR(FIO_STR_NAME) ".\n");
  fprintf(stderr,
          "* String container size (without wrapper): %zu\n",
          sizeof(FIO_NAME(FIO_STR_NAME, s)));
  fprintf(stderr,
          "* Self-contained capacity (FIO_STR_SMALL_CAPA): %zu\n",
          FIO__STR_SMALL_CAPA);
  FIO_ASSERT(!FIO_NAME_BL(FIO_STR_NAME, frozen)(&str), "new string is frozen");
  FIO_ASSERT(FIO_NAME(FIO_STR_NAME, capa)(&str) == FIO__STR_SMALL_CAPA,
             "small string capacity returned %zu",
             FIO_NAME(FIO_STR_NAME, capa)(&str));
  FIO_ASSERT(FIO_NAME(FIO_STR_NAME, len)(&str) == 0,
             "small string length reporting error!");
  FIO_ASSERT(
      FIO_NAME2(FIO_STR_NAME, ptr)(&str) == ((char *)(&str) + 1),
      "small string pointer reporting error (%zd offset)!",
      (ssize_t)(((char *)(&str) + 1) - FIO_NAME2(FIO_STR_NAME, ptr)(&str)));
  FIO_NAME(FIO_STR_NAME, write)(&str, "World", 4);
  FIO_ASSERT(FIO_STR_IS_SMALL(&str),
             "small string writing error - not small on small write!");
  FIO_ASSERT(FIO_NAME(FIO_STR_NAME, capa)(&str) == FIO__STR_SMALL_CAPA,
             "Small string capacity reporting error after write!");
  FIO_ASSERT(FIO_NAME(FIO_STR_NAME, len)(&str) == 4,
             "small string length reporting error after write!");
  FIO_ASSERT(FIO_NAME2(FIO_STR_NAME, ptr)(&str) == (char *)&str + 1,
             "small string pointer reporting error after write!");
  FIO_ASSERT(strlen(FIO_NAME2(FIO_STR_NAME, ptr)(&str)) == 4,
             "small string NUL missing after write (%zu)!",
             strlen(FIO_NAME2(FIO_STR_NAME, ptr)(&str)));
  FIO_ASSERT(!strcmp(FIO_NAME2(FIO_STR_NAME, ptr)(&str), "Worl"),
             "small string write error (%s)!",
             FIO_NAME2(FIO_STR_NAME, ptr)(&str));
  FIO_ASSERT(FIO_NAME2(FIO_STR_NAME, ptr)(&str) ==
                 FIO_NAME(FIO_STR_NAME, info)(&str).buf,
             "small string `data` != `info.buf` (%p != %p)",
             (void *)FIO_NAME2(FIO_STR_NAME, ptr)(&str),
             (void *)FIO_NAME(FIO_STR_NAME, info)(&str).buf);

  FIO_NAME(FIO_STR_NAME, FIO_STR_RESERVE_NAME)
  (&str, sizeof(FIO_NAME(FIO_STR_NAME, s)));
  FIO_ASSERT(!FIO_STR_IS_SMALL(&str),
             "Long String reporting as small after capacity update!");
  FIO_ASSERT(FIO_NAME(FIO_STR_NAME, capa)(&str) >=
                 sizeof(FIO_NAME(FIO_STR_NAME, s)) - 1,
             "Long String capacity update error (%zu != %zu)!",
             FIO_NAME(FIO_STR_NAME, capa)(&str),
             sizeof(FIO_NAME(FIO_STR_NAME, s)));
  FIO_ASSERT(FIO_NAME2(FIO_STR_NAME, ptr)(&str) ==
                 FIO_NAME(FIO_STR_NAME, info)(&str).buf,
             "Long String `ptr` !>= "
             "`cstr(s).buf` (%p != %p)",
             (void *)FIO_NAME2(FIO_STR_NAME, ptr)(&str),
             (void *)FIO_NAME(FIO_STR_NAME, info)(&str).buf);

#if FIO_STR_OPTIMIZE4IMMUTABILITY
  /* immutable string length is updated after `reserve` to reflect new capa */
  FIO_NAME(FIO_STR_NAME, resize)(&str, 4);
#endif
  FIO_ASSERT(
      FIO_NAME(FIO_STR_NAME, len)(&str) == 4,
      "Long String length changed during conversion from small string (%zu)!",
      FIO_NAME(FIO_STR_NAME, len)(&str));
  FIO_ASSERT(FIO_NAME2(FIO_STR_NAME, ptr)(&str) == str.buf,
             "Long String pointer reporting error after capacity update!");
  FIO_ASSERT(strlen(FIO_NAME2(FIO_STR_NAME, ptr)(&str)) == 4,
             "Long String NUL missing after capacity update (%zu)!",
             strlen(FIO_NAME2(FIO_STR_NAME, ptr)(&str)));
  FIO_ASSERT(!strcmp(FIO_NAME2(FIO_STR_NAME, ptr)(&str), "Worl"),
             "Long String value changed after capacity update (%s)!",
             FIO_NAME2(FIO_STR_NAME, ptr)(&str));

  FIO_NAME(FIO_STR_NAME, write)(&str, "d!", 2);
  FIO_ASSERT(!strcmp(FIO_NAME2(FIO_STR_NAME, ptr)(&str), "World!"),
             "Long String `write` error (%s)!",
             FIO_NAME2(FIO_STR_NAME, ptr)(&str));

  FIO_NAME(FIO_STR_NAME, replace)(&str, 0, 0, "Hello ", 6);
  FIO_ASSERT(!strcmp(FIO_NAME2(FIO_STR_NAME, ptr)(&str), "Hello World!"),
             "Long String `insert` error (%s)!",
             FIO_NAME2(FIO_STR_NAME, ptr)(&str));

  FIO_NAME(FIO_STR_NAME, resize)(&str, 6);
  FIO_ASSERT(!strcmp(FIO_NAME2(FIO_STR_NAME, ptr)(&str), "Hello "),
             "Long String `resize` clipping error (%s)!",
             FIO_NAME2(FIO_STR_NAME, ptr)(&str));

  FIO_NAME(FIO_STR_NAME, replace)(&str, 6, 0, "My World!", 9);
  FIO_ASSERT(!strcmp(FIO_NAME2(FIO_STR_NAME, ptr)(&str), "Hello My World!"),
             "Long String `replace` error when testing overflow (%s)!",
             FIO_NAME2(FIO_STR_NAME, ptr)(&str));

  FIO_NAME(FIO_STR_NAME, FIO_STR_RESERVE_NAME)
  (&str, FIO_NAME(FIO_STR_NAME, len)(&str)); /* may truncate */

  FIO_NAME(FIO_STR_NAME, replace)(&str, -10, 2, "Big", 3);
  FIO_ASSERT(!strcmp(FIO_NAME2(FIO_STR_NAME, ptr)(&str), "Hello Big World!"),
             "Long String `replace` error when testing splicing (%s)!",
             FIO_NAME2(FIO_STR_NAME, ptr)(&str));

  FIO_ASSERT(FIO_NAME(FIO_STR_NAME, capa)(&str) ==
                     FIO_STR_CAPA2WORDS(strlen("Hello Big World!")) ||
                 !FIO_NAME_BL(FIO_STR_NAME, allocated)(&str),
             "Long String `replace` capacity update error "
             "(%zu >=? %zu)!",
             FIO_NAME(FIO_STR_NAME, capa)(&str),
             FIO_STR_CAPA2WORDS(strlen("Hello Big World!")));

  if (FIO_NAME(FIO_STR_NAME, len)(&str) < (sizeof(str) - 2)) {
    FIO_NAME(FIO_STR_NAME, compact)(&str);
    FIO_ASSERT(FIO_STR_IS_SMALL(&str),
               "Compacting didn't change String to small!");
    FIO_ASSERT(FIO_NAME(FIO_STR_NAME, len)(&str) == strlen("Hello Big World!"),
               "Compacting altered String length! (%zu != %zu)!",
               FIO_NAME(FIO_STR_NAME, len)(&str),
               strlen("Hello Big World!"));
    FIO_ASSERT(!strcmp(FIO_NAME2(FIO_STR_NAME, ptr)(&str), "Hello Big World!"),
               "Compact data error (%s)!",
               FIO_NAME2(FIO_STR_NAME, ptr)(&str));
    FIO_ASSERT(FIO_NAME(FIO_STR_NAME, capa)(&str) == sizeof(str) - 2,
               "Compacted String capacity reporting error!");
  } else {
    fprintf(stderr, "* skipped `compact` test!\n");
  }

  {
    FIO_NAME(FIO_STR_NAME, freeze)(&str);
    FIO_ASSERT(FIO_NAME_BL(FIO_STR_NAME, frozen)(&str),
               "Frozen String not flagged as frozen.");
    fio_str_info_s old_state = FIO_NAME(FIO_STR_NAME, info)(&str);
    FIO_NAME(FIO_STR_NAME, write)(&str, "more data to be written here", 28);
    FIO_NAME(FIO_STR_NAME, replace)
    (&str, 2, 1, "more data to be written here", 28);
    fio_str_info_s new_state = FIO_NAME(FIO_STR_NAME, info)(&str);
    FIO_ASSERT(old_state.len == new_state.len, "Frozen String length changed!");
    FIO_ASSERT(old_state.buf == new_state.buf,
               "Frozen String pointer changed!");
    FIO_ASSERT(
        old_state.capa == new_state.capa,
        "Frozen String capacity changed (allowed, but shouldn't happen)!");
    FIO_STR_THAW_(&str);
  }
  FIO_NAME(FIO_STR_NAME, printf)(&str, " %u", 42);
  FIO_ASSERT(!strcmp(FIO_NAME2(FIO_STR_NAME, ptr)(&str), "Hello Big World! 42"),
             "`printf` data error (%s)!",
             FIO_NAME2(FIO_STR_NAME, ptr)(&str));

  {
    FIO_NAME(FIO_STR_NAME, s) str2 = FIO_STR_INIT;
    FIO_NAME(FIO_STR_NAME, concat)(&str2, &str);
    FIO_ASSERT(FIO_NAME_BL(FIO_STR_NAME, eq)(&str, &str2),
               "`concat` error, strings not equal (%s != %s)!",
               FIO_NAME2(FIO_STR_NAME, ptr)(&str),
               FIO_NAME2(FIO_STR_NAME, ptr)(&str2));
    FIO_NAME(FIO_STR_NAME, write)(&str2, ":extra data", 11);
    FIO_ASSERT(!FIO_NAME_BL(FIO_STR_NAME, eq)(&str, &str2),
               "`write` error after copy, strings equal "
               "((%zu)%s == (%zu)%s)!",
               FIO_NAME(FIO_STR_NAME, len)(&str),
               FIO_NAME2(FIO_STR_NAME, ptr)(&str),
               FIO_NAME(FIO_STR_NAME, len)(&str2),
               FIO_NAME2(FIO_STR_NAME, ptr)(&str2));

    FIO_NAME(FIO_STR_NAME, destroy)(&str2);
  }

  FIO_NAME(FIO_STR_NAME, destroy)(&str);

  FIO_NAME(FIO_STR_NAME, write_i)(&str, -42);
  FIO_ASSERT(FIO_NAME(FIO_STR_NAME, len)(&str) == 3 &&
                 !memcmp("-42", FIO_NAME2(FIO_STR_NAME, ptr)(&str), 3),
             "write_i output error ((%zu) %s != -42)",
             FIO_NAME(FIO_STR_NAME, len)(&str),
             FIO_NAME2(FIO_STR_NAME, ptr)(&str));
  FIO_NAME(FIO_STR_NAME, destroy)(&str);

  {
    fprintf(stderr, "* Testing string `readfile`.\n");
    FIO_NAME(FIO_STR_NAME, s) *s = FIO_NAME(FIO_STR_NAME, new)();
    FIO_ASSERT(
        FIO_PTR_UNTAG(s), "error, string not allocated (%p)!", (void *)s);
    fio_str_info_s state = FIO_NAME(FIO_STR_NAME, readfile)(s, __FILE__, 0, 0);

    FIO_ASSERT(state.len && state.buf,
               "error, no data was read for file %s!",
               __FILE__);

    FIO_ASSERT(!memcmp(state.buf,
                       "/* "
                       "******************************************************"
                       "***********************",
                       80),
               "content error, header mismatch!\n %s",
               state.buf);
    fprintf(stderr, "* Testing UTF-8 validation and length.\n");
    FIO_ASSERT(FIO_NAME(FIO_STR_NAME, utf8_valid)(s),
               "`utf8_valid` error, code in this file "
               "should be valid!");
    FIO_ASSERT(FIO_NAME(FIO_STR_NAME, utf8_len)(s) &&
                   (FIO_NAME(FIO_STR_NAME, utf8_len)(s) <=
                    FIO_NAME(FIO_STR_NAME, len)(s)) &&
                   (FIO_NAME(FIO_STR_NAME, utf8_len)(s) >=
                    (FIO_NAME(FIO_STR_NAME, len)(s)) >> 1),
               "`utf8_len` error, invalid value (%zu / %zu!",
               FIO_NAME(FIO_STR_NAME, utf8_len)(s),
               FIO_NAME(FIO_STR_NAME, len)(s));

    if (1) {
      /* String content == whole file (this file) */
      intptr_t pos = -11;
      size_t len = 20;
      fprintf(stderr, "* Testing UTF-8 positioning.\n");

      FIO_ASSERT(FIO_NAME(FIO_STR_NAME, utf8_select)(s, &pos, &len) == 0,
                 "`select` returned error for negative "
                 "pos! (%zd, %zu)",
                 (ssize_t)pos,
                 len);
      FIO_ASSERT(pos ==
                     (intptr_t)state.len - 10, /* no UTF-8 bytes in this file */
                 "`utf8_select` error, negative position "
                 "invalid! (%zd)",
                 (ssize_t)pos);
      FIO_ASSERT(len == 10,
                 "`utf8_select` error, trancated length "
                 "invalid! (%zd)",
                 (ssize_t)len);
      pos = 10;
      len = 20;
      FIO_ASSERT(FIO_NAME(FIO_STR_NAME, utf8_select)(s, &pos, &len) == 0,
                 "`utf8_select` returned error! (%zd, %zu)",
                 (ssize_t)pos,
                 len);
      FIO_ASSERT(pos == 10,
                 "`utf8_select` error, position invalid! (%zd)",
                 (ssize_t)pos);
      FIO_ASSERT(len == 20,
                 "`utf8_select` error, length invalid! (%zd)",
                 (ssize_t)len);
    }
    FIO_NAME(FIO_STR_NAME, free)(s);
  }
  FIO_NAME(FIO_STR_NAME, destroy)(&str);
  if (1) {
    /* Testing UTF-8 */
    const char *utf8_sample = /* three hearts, small-big-small*/
        "\xf0\x9f\x92\x95\xe2\x9d\xa4\xef\xb8\x8f\xf0\x9f\x92\x95";
    FIO_NAME(FIO_STR_NAME, write)(&str, utf8_sample, strlen(utf8_sample));
    intptr_t pos = -2;
    size_t len = 2;
    FIO_ASSERT(FIO_NAME(FIO_STR_NAME, utf8_select)(&str, &pos, &len) == 0,
               "`utf8_select` returned error for negative pos on "
               "UTF-8 data! (%zd, %zu)",
               (ssize_t)pos,
               len);
    FIO_ASSERT(pos == (intptr_t)FIO_NAME(FIO_STR_NAME, len)(&str) -
                          4, /* 4 byte emoji */
               "`utf8_select` error, negative position invalid on "
               "UTF-8 data! (%zd)",
               (ssize_t)pos);
    FIO_ASSERT(len == 4, /* last utf-8 char is 4 byte long */
               "`utf8_select` error, trancated length invalid on "
               "UTF-8 data! (%zd)",
               (ssize_t)len);
    pos = 1;
    len = 20;
    FIO_ASSERT(FIO_NAME(FIO_STR_NAME, utf8_select)(&str, &pos, &len) == 0,
               "`utf8_select` returned error on UTF-8 data! "
               "(%zd, %zu)",
               (ssize_t)pos,
               len);
    FIO_ASSERT(pos == 4,
               "`utf8_select` error, position invalid on "
               "UTF-8 data! (%zd)",
               (ssize_t)pos);
    FIO_ASSERT(len == 10,
               "`utf8_select` error, length invalid on "
               "UTF-8 data! (%zd)",
               (ssize_t)len);
    pos = 1;
    len = 3;
    FIO_ASSERT(FIO_NAME(FIO_STR_NAME, utf8_select)(&str, &pos, &len) == 0,
               "`utf8_select` returned error on UTF-8 data "
               "(2)! (%zd, %zu)",
               (ssize_t)pos,
               len);
    FIO_ASSERT(len ==
                   10, /* 3 UTF-8 chars: 4 byte + 4 byte + 2 byte codes == 10 */
               "`utf8_select` error, length invalid on UTF-8 data! "
               "(%zd)",
               (ssize_t)len);
  }
  FIO_NAME(FIO_STR_NAME, destroy)(&str);
  if (1) {
    /* Testing Static initialization and writing */
#if FIO_STR_OPTIMIZE4IMMUTABILITY
    FIO_NAME(FIO_STR_NAME, init_const)(&str, "Welcome", 7);
#else
    str = (FIO_NAME(FIO_STR_NAME, s))FIO_STR_INIT_STATIC("Welcome");
#endif
    FIO_ASSERT(FIO_NAME(FIO_STR_NAME, capa)(&str) == 0 ||
                   FIO_STR_IS_SMALL(&str),
               "Static string capacity non-zero.");
    FIO_ASSERT(FIO_NAME(FIO_STR_NAME, len)(&str) > 0,
               "Static string length should be automatically calculated.");
    FIO_ASSERT(!FIO_NAME_BL(FIO_STR_NAME, allocated)(&str),
               "Static strings shouldn't be dynamic.");
    FIO_NAME(FIO_STR_NAME, destroy)(&str);

#if FIO_STR_OPTIMIZE4IMMUTABILITY
    FIO_NAME(FIO_STR_NAME, init_const)
    (&str,
     "Welcome to a very long static string that should not fit within a "
     "containing struct... hopefuly",
     95);
#else
    str = (FIO_NAME(FIO_STR_NAME, s))FIO_STR_INIT_STATIC(
        "Welcome to a very long static string that should not fit within a "
        "containing struct... hopefuly");
#endif
    FIO_ASSERT(FIO_NAME(FIO_STR_NAME, capa)(&str) == 0 ||
                   FIO_STR_IS_SMALL(&str),
               "Static string capacity non-zero.");
    FIO_ASSERT(FIO_NAME(FIO_STR_NAME, len)(&str) > 0,
               "Static string length should be automatically calculated.");
    FIO_ASSERT(!FIO_NAME_BL(FIO_STR_NAME, allocated)(&str),
               "Static strings shouldn't be dynamic.");
    FIO_NAME(FIO_STR_NAME, destroy)(&str);

#if FIO_STR_OPTIMIZE4IMMUTABILITY
    FIO_NAME(FIO_STR_NAME, init_const)(&str, "Welcome", 7);
#else
    str = (FIO_NAME(FIO_STR_NAME, s))FIO_STR_INIT_STATIC("Welcome");
#endif
    fio_str_info_s state = FIO_NAME(FIO_STR_NAME, write)(&str, " Home", 5);
    FIO_ASSERT(state.capa > 0, "Static string not converted to non-static.");
    FIO_ASSERT(FIO_NAME_BL(FIO_STR_NAME, allocated)(&str) ||
                   FIO_STR_IS_SMALL(&str),
               "String should be dynamic after `write`.");

    char *cstr = FIO_NAME(FIO_STR_NAME, detach)(&str);
    FIO_ASSERT(cstr, "`detach` returned NULL");
    FIO_ASSERT(
        !memcmp(cstr, "Welcome Home\0", 13), "`detach` string error: %s", cstr);
    FIO_MEM_FREE(cstr, state.capa);
    FIO_ASSERT(FIO_NAME(FIO_STR_NAME, len)(&str) == 0,
               "`detach` data wasn't cleared.");
    FIO_NAME(FIO_STR_NAME, destroy)
    (&str); /* does nothing, but what the heck... */
  }
  {
    fprintf(stderr, "* Testing Base64 encoding / decoding.\n");
    FIO_NAME(FIO_STR_NAME, destroy)(&str); /* does nothing, but why not... */

    FIO_NAME(FIO_STR_NAME, s) b64message = FIO_STR_INIT;
    fio_str_info_s b64i = FIO_NAME(FIO_STR_NAME, write)(
        &b64message, "Hello World, this is the voice of peace:)", 41);
    for (int i = 0; i < 256; ++i) {
      uint8_t c = i;
      b64i = FIO_NAME(FIO_STR_NAME, write)(&b64message, &c, 1);
      FIO_ASSERT(FIO_NAME(FIO_STR_NAME, len)(&b64message) == (size_t)(42 + i),
                 "Base64 message length error (%zu != %zu)",
                 FIO_NAME(FIO_STR_NAME, len)(&b64message),
                 (size_t)(42 + i));
      FIO_ASSERT(FIO_NAME2(FIO_STR_NAME, ptr)(&b64message)[41 + i] == (char)c,
                 "Base64 message data error");
    }
    fio_str_info_s encoded =
        FIO_NAME(FIO_STR_NAME, write_base64enc)(&str, b64i.buf, b64i.len, 1);
    /* prevent encoded data from being deallocated during unencoding */
    encoded = FIO_NAME(FIO_STR_NAME, FIO_STR_RESERVE_NAME)(
        &str, encoded.len + ((encoded.len >> 2) * 3) + 8);
    fio_str_info_s decoded;
    {
      FIO_NAME(FIO_STR_NAME, s) tmps;
      FIO_NAME(FIO_STR_NAME, init_copy2)(&tmps, &str);
      decoded = FIO_NAME(FIO_STR_NAME,
                         write_base64dec)(&str,
                                          FIO_NAME2(FIO_STR_NAME, ptr)(&tmps),
                                          FIO_NAME(FIO_STR_NAME, len)(&tmps));
      FIO_NAME(FIO_STR_NAME, destroy)(&tmps);
      encoded.buf = decoded.buf;
    }
    FIO_ASSERT(encoded.len, "Base64 encoding failed");
    FIO_ASSERT(
        decoded.len > encoded.len, "Base64 decoding failed:\n%s", encoded.buf);
    FIO_ASSERT(b64i.len == decoded.len - encoded.len,
               "Base 64 roundtrip length error, %zu != %zu (%zu - %zu):\n %s",
               b64i.len,
               decoded.len - encoded.len,
               decoded.len,
               encoded.len,
               decoded.buf);

    FIO_ASSERT(!memcmp(b64i.buf, decoded.buf + encoded.len, b64i.len),
               "Base 64 roundtrip failed:\n %s",
               decoded.buf);
    FIO_NAME(FIO_STR_NAME, destroy)(&b64message);
    FIO_NAME(FIO_STR_NAME, destroy)(&str);
  }
  {
    fprintf(stderr, "* Testing JSON style character escaping / unescaping.\n");
    FIO_NAME(FIO_STR_NAME, s) unescaped = FIO_STR_INIT;
    fio_str_info_s ue;
    const char *utf8_sample = /* three hearts, small-big-small*/
        "\xf0\x9f\x92\x95\xe2\x9d\xa4\xef\xb8\x8f\xf0\x9f\x92\x95";
    FIO_NAME(FIO_STR_NAME, write)(&unescaped, utf8_sample, strlen(utf8_sample));
    for (int i = 0; i < 256; ++i) {
      uint8_t c = i;
      ue = FIO_NAME(FIO_STR_NAME, write)(&unescaped, &c, 1);
    }
    fio_str_info_s encoded =
        FIO_NAME(FIO_STR_NAME, write_escape)(&str, ue.buf, ue.len);
    // fprintf(stderr, "* %s\n", encoded.buf);
    fio_str_info_s decoded;
    {
      FIO_NAME(FIO_STR_NAME, s) tmps;
      FIO_NAME(FIO_STR_NAME, init_copy2)(&tmps, &str);
      decoded = FIO_NAME(FIO_STR_NAME,
                         write_unescape)(&str,
                                         FIO_NAME2(FIO_STR_NAME, ptr)(&tmps),
                                         FIO_NAME(FIO_STR_NAME, len)(&tmps));
      FIO_NAME(FIO_STR_NAME, destroy)(&tmps);
      encoded.buf = decoded.buf;
    }
    FIO_ASSERT(!memcmp(encoded.buf, utf8_sample, strlen(utf8_sample)),
               "valid UTF-8 data shouldn't be escaped:\n%.*s\n%s",
               (int)encoded.len,
               encoded.buf,
               decoded.buf);
    FIO_ASSERT(encoded.len, "JSON encoding failed");
    FIO_ASSERT(
        decoded.len > encoded.len, "JSON decoding failed:\n%s", encoded.buf);
    FIO_ASSERT(ue.len == decoded.len - encoded.len,
               "JSON roundtrip length error, %zu != %zu (%zu - %zu):\n %s",
               ue.len,
               decoded.len - encoded.len,
               decoded.len,
               encoded.len,
               decoded.buf);

    FIO_ASSERT(!memcmp(ue.buf, decoded.buf + encoded.len, ue.len),
               "JSON roundtrip failed:\n %s",
               decoded.buf);
    FIO_NAME(FIO_STR_NAME, destroy)(&unescaped);
    FIO_NAME(FIO_STR_NAME, destroy)(&str);
  }
}
#undef FIO__STR_SMALL_CAPA
#undef FIO_STR_WRITE_TEST_FUNC
#endif /* FIO_STR_WRITE_TEST_FUNC */

/* *****************************************************************************
String Cleanup
***************************************************************************** */
#endif /* FIO_EXTERN_COMPLETE */

#undef FIO_STR_SMALL
#undef FIO_STR_SMALL_CAPA
#undef FIO_STR_SMALL_DATA
#undef FIO_STR_SMALL_LEN
#undef FIO_STR_SMALL_LEN_SET

#undef FIO_STR_BIG_CAPA
#undef FIO_STR_BIG_CAPA_SET
#undef FIO_STR_BIG_DATA
#undef FIO_STR_BIG_FREE_BUF
#undef FIO_STR_BIG_IS_DYNAMIC
#undef FIO_STR_BIG_LEN
#undef FIO_STR_BIG_LEN_SET
#undef FIO_STR_BIG_SET_STATIC

#undef FIO_STR_CAPA2WORDS
#undef FIO_STR_FREEZE_

#undef FIO_STR_IS_FROZEN
#undef FIO_STR_IS_SMALL
#undef FIO_STR_NAME

#undef FIO_STR_OPTIMIZE4IMMUTABILITY
#undef FIO_STR_OPTIMIZE_EMBEDDED
#undef FIO_STR_PTR
#undef FIO_STR_THAW_
#undef FIO_STR_WRITE_ESCAPED_CT_OR
#undef FIO_STR_RESERVE_NAME

#endif /* FIO_STR_NAME */
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************










                      Reference Counting / Wrapper
                   (must be placed after all type macros)









***************************************************************************** */

#ifdef FIO_REF_NAME

#ifndef fio_atomic_add
#error FIO_REF_NAME requires enabling the FIO_ATOMIC extension.
#endif

#ifndef FIO_REF_TYPE
#define FIO_REF_TYPE FIO_NAME(FIO_REF_NAME, s)
#endif

#ifndef FIO_REF_INIT
#define FIO_REF_INIT(obj)
#endif

#ifndef FIO_REF_DESTROY
#define FIO_REF_DESTROY(obj)
#endif

#ifndef FIO_REF_METADATA_INIT
#define FIO_REF_METADATA_INIT(meta)
#endif

#ifndef FIO_REF_METADATA_DESTROY
#define FIO_REF_METADATA_DESTROY(meta)
#endif

/**
 * FIO_REF_CONSTRUCTOR_ONLY allows the reference counter constructor (TYPE_new)
 * to be the only constructor function.
 *
 * When set, the reference counting functions will use `X_new` and `X_free`.
 * Otherwise (assuming `X_new` and `X_free` are already defined), the reference
 * counter will define `X_new2` and `X_free2` instead.
 */
#ifdef FIO_REF_CONSTRUCTOR_ONLY
#define FIO_REF_CONSTRUCTOR new
#define FIO_REF_DESTRUCTOR free
#else
#define FIO_REF_CONSTRUCTOR new2
#define FIO_REF_DESTRUCTOR free2
#endif

typedef struct {
  volatile uint32_t ref;
#ifdef FIO_REF_METADATA
  FIO_REF_METADATA metadata;
#endif
  FIO_REF_TYPE wrapped;
} FIO_NAME(FIO_REF_NAME, _wrapper_s);

#ifdef FIO_PTR_TAG_TYPE
#define FIO_REF_TYPE_PTR FIO_PTR_TAG_TYPE
#else
#define FIO_REF_TYPE_PTR FIO_REF_TYPE *
#endif

/* *****************************************************************************
Reference Counter (Wrapper) API
***************************************************************************** */

/** Allocates a reference counted object. */
IFUNC FIO_REF_TYPE_PTR FIO_NAME(FIO_REF_NAME, FIO_REF_CONSTRUCTOR)(void);

/** Increases the reference count. */
IFUNC FIO_REF_TYPE_PTR FIO_NAME(FIO_REF_NAME, up_ref)(FIO_REF_TYPE_PTR wrapped);

/**
 * Frees a reference counted object (or decreases the reference count).
 *
 * Returns 1 if the object was actually freed, returns 0 otherwise.
 */
IFUNC int FIO_NAME(FIO_REF_NAME, FIO_REF_DESTRUCTOR)(FIO_REF_TYPE_PTR wrapped);

#ifdef FIO_REF_METADATA
/** Returns a pointer to the object's metadata, if defined. */
IFUNC FIO_REF_METADATA *FIO_NAME(FIO_REF_NAME,
                                 metadata)(FIO_REF_TYPE_PTR wrapped);
#endif

/* *****************************************************************************
Reference Counter (Wrapper) Implementation
***************************************************************************** */
#ifdef FIO_EXTERN_COMPLETE

/** Allocates a reference counted object. */
IFUNC FIO_REF_TYPE_PTR FIO_NAME(FIO_REF_NAME, FIO_REF_CONSTRUCTOR)(void) {
  FIO_NAME(FIO_REF_NAME, _wrapper_s) *o =
      (FIO_NAME(FIO_REF_NAME, _wrapper_s) *)FIO_MEM_CALLOC_(sizeof(*o), 1);
  if (!o)
    return (FIO_REF_TYPE_PTR)(FIO_PTR_TAG((FIO_REF_TYPE *)o));
  o->ref = 1;
  FIO_REF_METADATA_INIT((o->metadata));
  FIO_REF_INIT(o->wrapped);
  FIO_REF_TYPE *ret = &o->wrapped;
  return (FIO_REF_TYPE_PTR)(FIO_PTR_TAG(ret));
}

/** Increases the reference count. */
IFUNC FIO_REF_TYPE_PTR FIO_NAME(FIO_REF_NAME,
                                up_ref)(FIO_REF_TYPE_PTR wrapped_) {
  FIO_REF_TYPE *wrapped = (FIO_REF_TYPE *)(FIO_PTR_UNTAG(wrapped_));
  FIO_NAME(FIO_REF_NAME, _wrapper_s) *o =
      FIO_PTR_FROM_FIELD(FIO_NAME(FIO_REF_NAME, _wrapper_s), wrapped, wrapped);
  fio_atomic_add(&o->ref, 1);
  return wrapped_;
}

/** Frees a reference counted object (or decreases the reference count). */
IFUNC int FIO_NAME(FIO_REF_NAME,
                   FIO_REF_DESTRUCTOR)(FIO_REF_TYPE_PTR wrapped_) {
  FIO_REF_TYPE *wrapped = (FIO_REF_TYPE *)(FIO_PTR_UNTAG(wrapped_));
  if (!wrapped)
    return -1;
  FIO_NAME(FIO_REF_NAME, _wrapper_s) *o =
      FIO_PTR_FROM_FIELD(FIO_NAME(FIO_REF_NAME, _wrapper_s), wrapped, wrapped);
  if (!o)
    return -1;
  if (fio_atomic_sub_fetch(&o->ref, 1))
    return 0;
  FIO_REF_DESTROY(o->wrapped);
  FIO_REF_METADATA_DESTROY((o->metadata));
  FIO_MEM_FREE_(o, sizeof(*o));
  return 1;
}

#ifdef FIO_REF_METADATA
/** Returns a pointer to the object's metadata, if defined. */
IFUNC FIO_REF_METADATA *FIO_NAME(FIO_REF_NAME,
                                 metadata)(FIO_REF_TYPE_PTR wrapped_) {
  FIO_REF_TYPE *wrapped = (FIO_REF_TYPE *)(FIO_PTR_UNTAG(wrapped_));
  FIO_NAME(FIO_REF_NAME, _wrapper_s) *o =
      FIO_PTR_FROM_FIELD(FIO_NAME(FIO_REF_NAME, _wrapper_s), wrapped, wrapped);
  return &o->metadata;
}
#endif

/* *****************************************************************************
Reference Counter (Wrapper) Cleanup
***************************************************************************** */

#endif /* FIO_EXTERN_COMPLETE */
#undef FIO_REF_NAME
#undef FIO_REF_TYPE
#undef FIO_REF_INIT
#undef FIO_REF_DESTROY
#undef FIO_REF_METADATA
#undef FIO_REF_METADATA_INIT
#undef FIO_REF_METADATA_DESTROY
#undef FIO_REF_TYPE_PTR
#undef FIO_REF_CONSTRUCTOR_ONLY
#undef FIO_REF_CONSTRUCTOR
#undef FIO_REF_DESTRUCTOR
#endif
/* *****************************************************************************










                            Common Cleanup










***************************************************************************** */

/* *****************************************************************************
Common cleanup
***************************************************************************** */
#ifndef FIO_STL_KEEP__

#undef FIO_EXTERN
#undef SFUNC
#undef IFUNC
#undef SFUNC_
#undef IFUNC_
#undef FIO_PTR_TAG
#undef FIO_PTR_UNTAG
#undef FIO_PTR_TAG_TYPE
#undef FIO_PTR_TAG_VALIDATE
#undef FIO_PTR_TAG_VALID_OR_RETURN
#undef FIO_PTR_TAG_VALID_OR_RETURN_VOID
#undef FIO_PTR_TAG_VALID_OR_GOTO
#undef FIO_MEM_CALLOC_
#undef FIO_MEM_REALLOC_
#undef FIO_MEM_FREE_
#undef FIO_MEM_INTERNAL_MALLOC_
#undef FIO_MALLOC_TMP_USE_SYSTEM

/* undefine FIO_EXTERN_COMPLETE only if it was defined locally */
#if FIO_EXTERN_COMPLETE == 2
#undef FIO_EXTERN_COMPLETE
#endif

#else

#undef SFUNC
#undef IFUNC
#define SFUNC SFUNC_
#define IFUNC IFUNC_

#endif /* !FIO_STL_KEEP__ */
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#include "003 atomics.h"            /* Development inclusion - ignore line */
#include "004 bitwise.h"            /* Development inclusion - ignore line */
#include "005 riskyhash.h"          /* Development inclusion - ignore line */
#include "006 atol.h"               /* Development inclusion - ignore line */
#include "051 json.h"               /* Development inclusion - ignore line */
#include "201 array.h"              /* Development inclusion - ignore line */
#include "202 hashmap.h"            /* Development inclusion - ignore line */
#include "203 string.h"             /* Development inclusion - ignore line */
#include "299 reference counter.h"  /* Development inclusion - ignore line */
#include "700 cleanup.h"            /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************














                          FIOBJ - soft (dynamic) types









FIOBJ - dynamic types

These are dynamic types that use pointer tagging for fast type identification.

Pointer tagging on 64 bit systems allows for 3 bits at the lower bits. On most
32 bit systems this is also true due to allocator alignment. When in doubt, use
the provided custom allocator.

To keep the 64bit memory address alignment on 32bit systems, a 32bit metadata
integer is added when a virtual function table is missing. This doesn't effect
memory consumption on 64 bit systems and uses 4 bytes on 32 bit systems.

Note: this code is placed at the end of the STL file, since it leverages most of
the SLT features and could be affected by their inclusion.
***************************************************************************** */
#if defined(FIO_FIOBJ) && !defined(H___FIOBJ___H)
#define H___FIOBJ___H

/* *****************************************************************************
FIOBJ compilation settings (type names and JSON nesting limits).

Type Naming Macros for FIOBJ types. By default, results in:
- fiobj_true()
- fiobj_false()
- fiobj_null()
- fiobj_num_new() ... (etc')
- fiobj_float_new() ... (etc')
- fiobj_str_new() ... (etc')
- fiobj_array_new() ... (etc')
- fiobj_hash_new() ... (etc')
***************************************************************************** */

#define FIOBJ___NAME_TRUE true
#define FIOBJ___NAME_FALSE false
#define FIOBJ___NAME_NULL null
#define FIOBJ___NAME_NUMBER num
#define FIOBJ___NAME_FLOAT float
#define FIOBJ___NAME_STRING str
#define FIOBJ___NAME_ARRAY array
#define FIOBJ___NAME_HASH hash

#ifndef FIOBJ_MAX_NESTING
/**
 * Sets the limit on nesting level transversal by recursive functions.
 *
 * This effects JSON output / input and the `fiobj_each2` function since they
 * are recursive.
 *
 * HOWEVER: this value will NOT effect the recursive `fiobj_free` which could
 * (potentially) expload the stack if given melformed input such as cyclic data
 * structures.
 *
 * Values should be less than 32K.
 */
#define FIOBJ_MAX_NESTING 512
#endif

/* make sure roundtrips work */
#ifndef JSON_MAX_DEPTH
#define JSON_MAX_DEPTH FIOBJ_MAX_NESTING
#endif
/* *****************************************************************************
General Requirements / Macros
***************************************************************************** */

#define FIO_ATOL 1
#define FIO_ATOMIC 1
#include __FILE__

#ifdef FIOBJ_EXTERN
#define FIOBJ_FUNC
#define FIOBJ_IFUNC
#define FIOBJ_EXTERN_OBJ extern
#define FIOBJ_EXTERN_OBJ_IMP __attribute__((weak))

#else /* FIO_EXTERN */
#define FIOBJ_FUNC static __attribute__((unused))
#define FIOBJ_IFUNC static inline __attribute__((unused))
#define FIOBJ_EXTERN_OBJ static __attribute__((unused))
#define FIOBJ_EXTERN_OBJ_IMP static __attribute__((unused))
#ifndef FIOBJ_EXTERN_COMPLETE /* force implementation, emitting static data */
#define FIOBJ_EXTERN_COMPLETE 2
#endif /* FIOBJ_EXTERN_COMPLETE */

#endif /* FIO_EXTERN */

#ifdef FIO_LOG_PRINT__
#define FIOBJ_LOG_PRINT__(...) FIO_LOG_PRINT__(__VA_ARGS__)
#else
#define FIOBJ_LOG_PRINT__(...)
#endif

#ifdef __cplusplus /* C++ doesn't allow declarations for static variables */
#undef FIOBJ_EXTERN_OBJ
#undef FIOBJ_EXTERN_OBJ_IMP
#define FIOBJ_EXTERN_OBJ extern "C"
#define FIOBJ_EXTERN_OBJ_IMP extern "C" __attribute__((weak))
#endif

/* *****************************************************************************
Debugging / Leak Detection
***************************************************************************** */
#if (TEST || DEBUG) && !defined(FIOBJ_MARK_MEMORY)
#define FIOBJ_MARK_MEMORY 1
#endif

#if FIOBJ_MARK_MEMORY
size_t __attribute__((weak)) FIOBJ_MARK_MEMORY_ALLOC_COUNTER;
size_t __attribute__((weak)) FIOBJ_MARK_MEMORY_FREE_COUNTER;
#define FIOBJ_MARK_MEMORY_ALLOC()                                              \
  fio_atomic_add(&FIOBJ_MARK_MEMORY_ALLOC_COUNTER, 1)
#define FIOBJ_MARK_MEMORY_FREE()                                               \
  fio_atomic_add(&FIOBJ_MARK_MEMORY_FREE_COUNTER, 1)
#define FIOBJ_MARK_MEMORY_PRINT()                                              \
  FIOBJ_LOG_PRINT__(                                                           \
      ((FIOBJ_MARK_MEMORY_ALLOC_COUNTER == FIOBJ_MARK_MEMORY_FREE_COUNTER)     \
           ? 4 /* FIO_LOG_LEVEL_INFO */                                        \
           : 3 /* FIO_LOG_LEVEL_WARNING */),                                   \
      ((FIOBJ_MARK_MEMORY_ALLOC_COUNTER == FIOBJ_MARK_MEMORY_FREE_COUNTER)     \
           ? "INFO: total FIOBJ allocations: %zu (%zu/%zu)"                    \
           : "WARNING: LEAKED! FIOBJ allocations: %zu (%zu/%zu)"),             \
      FIOBJ_MARK_MEMORY_ALLOC_COUNTER - FIOBJ_MARK_MEMORY_FREE_COUNTER,        \
      FIOBJ_MARK_MEMORY_FREE_COUNTER,                                          \
      FIOBJ_MARK_MEMORY_ALLOC_COUNTER)
#define FIOBJ_MARK_MEMORY_ENABLED 1

#else

#define FIOBJ_MARK_MEMORY_ALLOC_COUNTER 0 /* when testing unmarked FIOBJ */
#define FIOBJ_MARK_MEMORY_FREE_COUNTER 0  /* when testing unmarked FIOBJ */
#define FIOBJ_MARK_MEMORY_ALLOC()
#define FIOBJ_MARK_MEMORY_FREE()
#define FIOBJ_MARK_MEMORY_PRINT()
#define FIOBJ_MARK_MEMORY_ENABLED 0
#endif

/* *****************************************************************************
The FIOBJ Type
***************************************************************************** */

/** Use the FIOBJ type for dynamic types. */
typedef struct FIOBJ_s {
  struct FIOBJ_s *compiler_validation_type;
} * FIOBJ;

/** FIOBJ type enum for common / primitive types. */
typedef enum {
  FIOBJ_T_NUMBER = 0x01, /* 0b001 3 bits taken for small numbers */
  FIOBJ_T_PRIMITIVE = 2, /* 0b010 a lonely second bit signifies a primitive */
  FIOBJ_T_STRING = 3,    /* 0b011 */
  FIOBJ_T_ARRAY = 4,     /* 0b100 */
  FIOBJ_T_HASH = 5,      /* 0b101 */
  FIOBJ_T_FLOAT = 6,     /* 0b110 */
  FIOBJ_T_OTHER = 7,     /* 0b111 dynamic type - test content */
} fiobj_class_en;

#define FIOBJ_T_NULL 2   /* 0b010 a lonely second bit signifies a primitive */
#define FIOBJ_T_TRUE 18  /* 0b010 010 - primitive value */
#define FIOBJ_T_FALSE 34 /* 0b100 010 - primitive value */

/** Use the macros to avoid future API changes. */
#define FIOBJ_TYPE(o) fiobj_type(o)
/** Use the macros to avoid future API changes. */
#define FIOBJ_TYPE_IS(o, type) (fiobj_type(o) == type)
/** Identifies an invalid type identifier (returned from FIOBJ_TYPE(o) */
#define FIOBJ_T_INVALID 0
/** Identifies an invalid object */
#define FIOBJ_INVALID 0
/** Tests if the object is (probably) a valid FIOBJ */
#define FIOBJ_IS_INVALID(o) (((uintptr_t)(o)&7UL) == 0)
#define FIOBJ_TYPE_CLASS(o) ((fiobj_class_en)(((uintptr_t)o) & 7UL))
#define FIOBJ_PTR_UNTAG(o) ((uintptr_t)o & (~7ULL))
/** Returns an objects type. This isn't limited to known types. */
FIO_IFUNC size_t fiobj_type(FIOBJ o);

/* *****************************************************************************
FIOBJ Memory Management
***************************************************************************** */

/** Increases an object's reference count (or copies) and returns it. */
FIO_IFUNC FIOBJ fiobj_dup(FIOBJ o);

/** Decreases an object's reference count or frees it. */
FIO_IFUNC void fiobj_free(FIOBJ o);

/* *****************************************************************************
FIOBJ Data / Info
***************************************************************************** */

/** Compares two objects. */
FIO_IFUNC unsigned char FIO_NAME_BL(fiobj, eq)(FIOBJ a, FIOBJ b);

/** Returns a temporary String representation for any FIOBJ object. */
FIO_IFUNC fio_str_info_s FIO_NAME2(fiobj, cstr)(FIOBJ o);

/** Returns an integer representation for any FIOBJ object. */
FIO_IFUNC intptr_t FIO_NAME2(fiobj, i)(FIOBJ o);

/** Returns a float (double) representation for any FIOBJ object. */
FIO_IFUNC double FIO_NAME2(fiobj, f)(FIOBJ o);

/* *****************************************************************************
FIOBJ Containers (iteration)
***************************************************************************** */

/**
 * Performs a task for each element held by the FIOBJ object.
 *
 * If `task` returns -1, the `each` loop will break (stop).
 *
 * Returns the "stop" position - the number of elements processed + `start_at`.
 */
FIO_SFUNC uint32_t fiobj_each1(FIOBJ o,
                               int32_t start_at,
                               int (*task)(FIOBJ child, void *arg),
                               void *arg);

/**
 * Performs a task for the object itself and each element held by the FIOBJ
 * object or any of it's elements (a deep task).
 *
 * The order of performance is by order of appearance, as if all nesting levels
 * were flattened.
 *
 * If `task` returns -1, the `each` loop will break (stop).
 *
 * Returns the number of elements processed.
 */
FIOBJ_FUNC uint32_t fiobj_each2(FIOBJ o,
                                int (*task)(FIOBJ child, void *arg),
                                void *arg);

/* *****************************************************************************
FIOBJ Primitives (NULL, True, False)
***************************************************************************** */

/** Returns the `true` primitive. */
FIO_IFUNC FIOBJ FIO_NAME(fiobj, FIOBJ___NAME_TRUE)(void) {
  return (FIOBJ)(FIOBJ_T_TRUE);
}

/** Returns the `false` primitive. */
FIO_IFUNC FIOBJ FIO_NAME(fiobj, FIOBJ___NAME_FALSE)(void) {
  return (FIOBJ)(FIOBJ_T_FALSE);
}

/** Returns the `nil` / `null` primitive. */
FIO_IFUNC FIOBJ FIO_NAME(fiobj, FIOBJ___NAME_NULL)(void) {
  return (FIOBJ)(FIOBJ_T_NULL);
}

/* *****************************************************************************
FIOBJ Type - Extendability (FIOBJ_T_OTHER)
***************************************************************************** */

/** FIOBJ types can be extended using virtual function tables. */
typedef struct {
  /**
   * MUST return a unique number to identify object type.
   *
   * Numbers (type IDs) under 100 are reserved. Numbers under 40 are illegal.
   */
  size_t type_id;
  /** Test for equality between two objects with the same `type_id` */
  unsigned char (*is_eq)(FIOBJ restrict a, FIOBJ restrict b);
  /** Converts an object to a String */
  fio_str_info_s (*to_s)(FIOBJ o);
  /** Converts an object to an integer */
  intptr_t (*to_i)(FIOBJ o);
  /** Converts an object to a double */
  double (*to_f)(FIOBJ o);
  /** Returns the number of exposed elements held by the object, if any. */
  uint32_t (*count)(FIOBJ o);
  /** Iterates the exposed elements held by the object. See `fiobj_each1`. */
  uint32_t (*each1)(FIOBJ o,
                    int32_t start_at,
                    int (*task)(FIOBJ child, void *arg),
                    void *arg);
  /**
   * Decreases the reference count and/or frees the object, calling `free2` for
   * any nested objects.
   *
   * Returns 0 if the object is still alive or 1 if the object was freed. The
   * return value is currently ignored, but this might change in the future.
   */
  int (*free2)(FIOBJ o);
} FIOBJ_class_vtable_s;

FIOBJ_EXTERN_OBJ const FIOBJ_class_vtable_s FIOBJ___OBJECT_CLASS_VTBL;

#define FIO_REF_CONSTRUCTOR_ONLY 1
#define FIO_REF_NAME fiobj_object
#define FIO_REF_TYPE void *
#define FIO_REF_METADATA const FIOBJ_class_vtable_s *
#define FIO_REF_METADATA_INIT(m)                                               \
  do {                                                                         \
    m = &FIOBJ___OBJECT_CLASS_VTBL;                                            \
    FIOBJ_MARK_MEMORY_ALLOC();                                                 \
  } while (0)
#define FIO_REF_METADATA_DESTROY(m)                                            \
  do {                                                                         \
    FIOBJ_MARK_MEMORY_FREE();                                                  \
  } while (0)
#define FIO_PTR_TAG(p) ((uintptr_t)p | FIOBJ_T_OTHER)
#define FIO_PTR_UNTAG(p) FIOBJ_PTR_UNTAG(p)
#define FIO_PTR_TAG_TYPE FIOBJ
#include __FILE__

/* *****************************************************************************
FIOBJ Integers
***************************************************************************** */

/** Creates a new Number object. */
FIO_IFUNC FIOBJ FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), new)(intptr_t i);

/** Reads the number from a FIOBJ Number. */
FIO_IFUNC intptr_t FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), i)(FIOBJ i);

/** Reads the number from a FIOBJ Number, fitting it in a double. */
FIO_IFUNC double FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), f)(FIOBJ i);

/** Returns a String representation of the number (in base 10). */
FIOBJ_FUNC fio_str_info_s FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER),
                                    cstr)(FIOBJ i);

/** Frees a FIOBJ number. */
FIO_IFUNC void FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), free)(FIOBJ i);

FIOBJ_EXTERN_OBJ const FIOBJ_class_vtable_s FIOBJ___NUMBER_CLASS_VTBL;

/* *****************************************************************************
FIOBJ Floats
***************************************************************************** */

/** Creates a new Float (double) object. */
FIO_IFUNC FIOBJ FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), new)(double i);

/** Reads the number from a FIOBJ Float rounding it to an integer. */
FIO_IFUNC intptr_t FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), i)(FIOBJ i);

/** Reads the value from a FIOBJ Float, as a double. */
FIO_IFUNC double FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), f)(FIOBJ i);

/** Returns a String representation of the float. */
FIOBJ_FUNC fio_str_info_s FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT),
                                    cstr)(FIOBJ i);

/** Frees a FIOBJ Float. */
FIO_IFUNC void FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), free)(FIOBJ i);

FIOBJ_EXTERN_OBJ const FIOBJ_class_vtable_s FIOBJ___FLOAT_CLASS_VTBL;

/* *****************************************************************************
FIOBJ Strings
***************************************************************************** */

#define FIO_STR_NAME FIO_NAME(fiobj, FIOBJ___NAME_STRING)
#define FIO_STR_OPTIMIZE_EMBEDDED 1
#define FIO_REF_NAME FIO_NAME(fiobj, FIOBJ___NAME_STRING)
#define FIO_REF_CONSTRUCTOR_ONLY 1
#define FIO_REF_DESTROY(s)                                                     \
  do {                                                                         \
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), destroy)((FIOBJ)&s);        \
    FIOBJ_MARK_MEMORY_FREE();                                                  \
  } while (0)
#define FIO_REF_INIT(s_)                                                       \
  do {                                                                         \
    s_ = (FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), s))FIO_STR_INIT;      \
    FIOBJ_MARK_MEMORY_ALLOC();                                                 \
  } while (0)
#define FIO_REF_METADATA uint32_t /* for 32bit system padding */
#define FIO_PTR_TAG(p) ((uintptr_t)p | FIOBJ_T_STRING)
#define FIO_PTR_UNTAG(p) FIOBJ_PTR_UNTAG(p)
#define FIO_PTR_TAG_TYPE FIOBJ
#include __FILE__

/* Creates a new FIOBJ string object, copying the data to the new string. */
FIO_IFUNC FIOBJ FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING),
                         new_cstr)(const char *ptr, size_t len) {
  FIOBJ s = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), new)();
  FIO_ASSERT_ALLOC(FIOBJ_PTR_UNTAG(s));
  FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)(s, ptr, len);
  return s;
}

/* Creates a new FIOBJ string object with (at least) the requested capacity. */
FIO_IFUNC FIOBJ FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING),
                         new_buf)(size_t capa) {
  FIOBJ s = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), new)();
  FIO_ASSERT_ALLOC(FIOBJ_PTR_UNTAG(s));
  FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), reserve)(s, capa);
  return s;
}

/* Creates a new FIOBJ string object, copying the origin (`fiobj2cstr`). */
FIO_IFUNC FIOBJ FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING),
                         new_copy)(FIOBJ original) {
  FIOBJ s = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), new)();
  FIO_ASSERT_ALLOC(FIOBJ_PTR_UNTAG(s));
  fio_str_info_s i = FIO_NAME2(fiobj, cstr)(original);
  FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)(s, i.buf, i.len);
  return s;
}

/** Returns information about the string. Same as fiobj_str_info(). */
FIO_IFUNC fio_str_info_s FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING),
                                   cstr)(FIOBJ s) {
  return FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), info)(s);
}

/**
 * Creates a temporary FIOBJ String object on the stack.
 *
 * String data might be allocated dynamically.
 */
#define FIOBJ_STR_TEMP_VAR(str_name)                                           \
  struct {                                                                     \
    uint64_t i1;                                                               \
    uint64_t i2;                                                               \
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), s) s;                       \
  } FIO_NAME(str_name, __auto_mem_tmp) = {                                     \
      0x7f7f7f7f7f7f7f7fULL, 0x7f7f7f7f7f7f7f7fULL, FIO_STR_INIT};             \
  FIOBJ str_name =                                                             \
      (FIOBJ)(((uintptr_t) & (FIO_NAME(str_name, __auto_mem_tmp).s)) |         \
              FIOBJ_T_STRING);

/**
 * Creates a temporary FIOBJ String object on the stack, initialized with a
 * static string.
 *
 * String data might be allocated dynamically.
 */
#define FIOBJ_STR_TEMP_VAR_STATIC(str_name, buf_, len_)                        \
  struct {                                                                     \
    uint64_t i1;                                                               \
    uint64_t i2;                                                               \
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), s) s;                       \
  } FIO_NAME(str_name,                                                         \
             __auto_mem_tmp) = {0x7f7f7f7f7f7f7f7fULL,                         \
                                0x7f7f7f7f7f7f7f7fULL,                         \
                                FIO_STR_INIT_STATIC2((buf_), (len_))};         \
  FIOBJ str_name =                                                             \
      (FIOBJ)(((uintptr_t) & (FIO_NAME(str_name, __auto_mem_tmp).s)) |         \
              FIOBJ_T_STRING);

/**
 * Creates a temporary FIOBJ String object on the stack, initialized with a
 * static string.
 *
 * String data might be allocated dynamically.
 */
#define FIOBJ_STR_TEMP_VAR_EXISTING(str_name, buf_, len_, capa_)               \
  struct {                                                                     \
    uint64_t i1;                                                               \
    uint64_t i2;                                                               \
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), s) s;                       \
  } FIO_NAME(str_name, __auto_mem_tmp) = {                                     \
      0x7f7f7f7f7f7f7f7fULL,                                                   \
      0x7f7f7f7f7f7f7f7fULL,                                                   \
      FIO_STR_INIT_EXISTING((buf_), (len_), (capa_))};                         \
  FIOBJ str_name =                                                             \
      (FIOBJ)(((uintptr_t) & (FIO_NAME(str_name, __auto_mem_tmp).s)) |         \
              FIOBJ_T_STRING);

/** Resets a temporary FIOBJ String, freeing and any resources allocated. */
#define FIOBJ_STR_TEMP_DESTROY(str_name)                                       \
  FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), destroy)(str_name);

/* *****************************************************************************
FIOBJ Arrays
***************************************************************************** */

#define FIO_ARRAY_NAME FIO_NAME(fiobj, FIOBJ___NAME_ARRAY)
#define FIO_REF_NAME FIO_NAME(fiobj, FIOBJ___NAME_ARRAY)
#define FIO_REF_CONSTRUCTOR_ONLY 1
#define FIO_REF_DESTROY(a)                                                     \
  do {                                                                         \
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), destroy)((FIOBJ)&a);         \
    FIOBJ_MARK_MEMORY_FREE();                                                  \
  } while (0)
#define FIO_REF_INIT(a)                                                        \
  do {                                                                         \
    a = (FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), s))FIO_ARRAY_INIT;      \
    FIOBJ_MARK_MEMORY_ALLOC();                                                 \
  } while (0)
#define FIO_REF_METADATA uint32_t /* for 32bit system padding */
#define FIO_ARRAY_TYPE FIOBJ
#define FIO_ARRAY_TYPE_CMP(a, b) FIO_NAME_BL(fiobj, eq)((a), (b))
#define FIO_ARRAY_TYPE_DESTROY(o) fiobj_free(o)
#define FIO_ARRAY_TYPE_CONCAT_COPY(dest, obj)                                  \
  do {                                                                         \
    dest = fiobj_dup(obj);                                                     \
  } while (0)
#define FIO_PTR_TAG(p) ((uintptr_t)p | FIOBJ_T_ARRAY)
#define FIO_PTR_UNTAG(p) FIOBJ_PTR_UNTAG(p)
#define FIO_PTR_TAG_TYPE FIOBJ
#include __FILE__

/* *****************************************************************************
FIOBJ Hash Maps
***************************************************************************** */

#define FIO_MAP_NAME FIO_NAME(fiobj, FIOBJ___NAME_HASH)
#define FIO_REF_NAME FIO_NAME(fiobj, FIOBJ___NAME_HASH)
#define FIO_REF_CONSTRUCTOR_ONLY 1
#define FIO_REF_DESTROY(a)                                                     \
  do {                                                                         \
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), destroy)((FIOBJ)&a);          \
    FIOBJ_MARK_MEMORY_FREE();                                                  \
  } while (0)
#define FIO_REF_INIT(a)                                                        \
  do {                                                                         \
    a = (FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), s))FIO_MAP_INIT;         \
    FIOBJ_MARK_MEMORY_ALLOC();                                                 \
  } while (0)
#define FIO_REF_METADATA uint32_t /* for 32bit system padding */
#define FIO_MAP_TYPE FIOBJ
#define FIO_MAP_TYPE_DESTROY(o) fiobj_free(o)
#define FIO_MAP_KEY FIOBJ
#define FIO_MAP_KEY_CMP(a, b) FIO_NAME_BL(fiobj, eq)((a), (b))
#define FIO_MAP_KEY_COPY(dest, o) (dest = fiobj_dup(o))
#define FIO_MAP_KEY_DESTROY(o) fiobj_free(o)
#define FIO_PTR_TAG(p) ((uintptr_t)p | FIOBJ_T_HASH)
#define FIO_PTR_UNTAG(p) FIOBJ_PTR_UNTAG(p)
#define FIO_PTR_TAG_TYPE FIOBJ
#include __FILE__

/** Calculates an object's hash value for a specific hash map object. */
FIO_IFUNC uint64_t FIO_NAME2(fiobj, hash)(FIOBJ target_hash, FIOBJ object_key);

/** Inserts a value to a hash map, with a default hash value calculation. */
FIO_IFUNC FIOBJ FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                         set2)(FIOBJ hash, FIOBJ key, FIOBJ value);

/** Finds a value in a hash map, with a default hash value calculation. */
FIO_IFUNC FIOBJ FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), get2)(FIOBJ hash,
                                                                   FIOBJ key);

/** Removes a value from a hash map, with a default hash value calculation. */
FIO_IFUNC int FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                       remove2)(FIOBJ hash, FIOBJ key, FIOBJ *old);

/**
 * Sets a String value in a hash map, allocating the String and automatically
 * calculating the hash value.
 */
FIO_IFUNC
FIOBJ FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
               set3)(FIOBJ hash, const char *key, size_t len, FIOBJ value);

/**
 * Finds a String value in a hash map, using a temporary String and
 * automatically calculating the hash value.
 */
FIO_IFUNC FIOBJ FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                         get3)(FIOBJ hash, const char *buf, size_t len);

/**
 * Removes a String value in a hash map, using a temporary String and
 * automatically calculating the hash value.
 */
FIO_IFUNC int
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
             remove3)(FIOBJ hash, const char *buf, size_t len, FIOBJ *old);

/* *****************************************************************************
FIOBJ JSON support
***************************************************************************** */

/**
 * Returns a JSON valid FIOBJ String, representing the object.
 *
 * If `dest` is an existing String, the formatted JSON data will be appended to
 * the existing string.
 */
FIO_IFUNC FIOBJ FIO_NAME2(fiobj, json)(FIOBJ dest, FIOBJ o, uint8_t beautify);

/**
 * Updates a Hash using JSON data.
 *
 * Parsing errors and non-dictionary object JSON data are silently ignored,
 * attempting to update the Hash as much as possible before any errors
 * encountered.
 *
 * Conflicting Hash data is overwritten (preferring the new over the old).
 *
 * Returns the number of bytes consumed. On Error, 0 is returned and no data is
 * consumed.
 */
FIOBJ_FUNC size_t FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                           update_json)(FIOBJ hash, fio_str_info_s str);

/** Helper function, calls `fiobj_hash_update_json` with string information */
FIO_IFUNC size_t FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                          update_json2)(FIOBJ hash, char *ptr, size_t len);

/**
 * Parses a C string for JSON data. If `consumed` is not NULL, the `size_t`
 * variable will contain the number of bytes consumed before the parser stopped
 * (due to either error or end of a valid JSON data segment).
 *
 * Returns a FIOBJ object matching the JSON valid C string `str`.
 *
 * If the parsing failed (no complete valid JSON data) `FIOBJ_INVALID` is
 * returned.
 */
FIOBJ_FUNC FIOBJ fiobj_json_parse(fio_str_info_s str, size_t *consumed);

/** Helper macro, calls `fiobj_json_parse` with string information */
#define fiobj_json_parse2(data_, len_, consumed)                               \
  fiobj_json_parse((fio_str_info_s){.buf = data_, .len = len_}, consumed)

/* *****************************************************************************







FIOBJ - Implementation - Inline / Macro like fucntions







***************************************************************************** */

/* *****************************************************************************
The FIOBJ Type
***************************************************************************** */

/** Returns an objects type. This isn't limited to known types. */
FIO_IFUNC size_t fiobj_type(FIOBJ o) {
  switch (FIOBJ_TYPE_CLASS(o)) {
  case FIOBJ_T_PRIMITIVE:
    switch ((uintptr_t)(o)) {
    case FIOBJ_T_NULL:
      return FIOBJ_T_NULL;
    case FIOBJ_T_TRUE:
      return FIOBJ_T_TRUE;
    case FIOBJ_T_FALSE:
      return FIOBJ_T_FALSE;
    };
    return FIOBJ_T_INVALID;
  case FIOBJ_T_NUMBER:
    return FIOBJ_T_NUMBER;
  case FIOBJ_T_FLOAT:
    return FIOBJ_T_FLOAT;
  case FIOBJ_T_STRING:
    return FIOBJ_T_STRING;
  case FIOBJ_T_ARRAY:
    return FIOBJ_T_ARRAY;
  case FIOBJ_T_HASH:
    return FIOBJ_T_HASH;
  case FIOBJ_T_OTHER:
    return (*fiobj_object_metadata(o))->type_id;
  }
  if (!o)
    return FIOBJ_T_NULL;
  return FIOBJ_T_INVALID;
}

/* *****************************************************************************
FIOBJ Memory Management
***************************************************************************** */

/** Increases an object's reference count (or copies) and returns it. */
FIO_IFUNC FIOBJ fiobj_dup(FIOBJ o) {
  switch (FIOBJ_TYPE_CLASS(o)) {
  case FIOBJ_T_PRIMITIVE: /* fallthrough */
  case FIOBJ_T_NUMBER:    /* fallthrough */
  case FIOBJ_T_FLOAT:     /* fallthrough */
    return o;
  case FIOBJ_T_STRING: /* fallthrough */
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), up_ref)(o);
    break;
  case FIOBJ_T_ARRAY: /* fallthrough */
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), up_ref)(o);
    break;
  case FIOBJ_T_HASH: /* fallthrough */
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), up_ref)(o);
    break;
  case FIOBJ_T_OTHER: /* fallthrough */
    fiobj_object_up_ref(o);
  }
  return o;
}

/** Decreases an object's reference count or frees it. */
FIO_IFUNC void fiobj_free(FIOBJ o) {
  switch (FIOBJ_TYPE_CLASS(o)) {
  case FIOBJ_T_PRIMITIVE: /* fallthrough */
  case FIOBJ_T_NUMBER:    /* fallthrough */
  case FIOBJ_T_FLOAT:
    return;
  case FIOBJ_T_STRING:
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), free)(o);
    return;
  case FIOBJ_T_ARRAY:
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), free)(o);
    return;
  case FIOBJ_T_HASH:
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), free)(o);
    return;
  case FIOBJ_T_OTHER:
    (*fiobj_object_metadata(o))->free2(o);
    return;
  }
}

/* *****************************************************************************
FIOBJ Data / Info
***************************************************************************** */

/** Internal: compares two nestable objects. */
FIOBJ_FUNC unsigned char fiobj___test_eq_nested(FIOBJ restrict a,
                                                FIOBJ restrict b);

/** Compares two objects. */
FIO_IFUNC unsigned char FIO_NAME_BL(fiobj, eq)(FIOBJ a, FIOBJ b) {
  if (a == b)
    return 1;
  if (FIOBJ_TYPE_CLASS(a) != FIOBJ_TYPE_CLASS(b))
    return 0;
  switch (FIOBJ_TYPE_CLASS(a)) {
  case FIOBJ_T_PRIMITIVE:
  case FIOBJ_T_NUMBER: /* fallthrough */
  case FIOBJ_T_FLOAT:  /* fallthrough */
    return a == b;
  case FIOBJ_T_STRING:
    return FIO_NAME_BL(FIO_NAME(fiobj, FIOBJ___NAME_STRING), eq)(a, b);
  case FIOBJ_T_ARRAY:
    return fiobj___test_eq_nested(a, b);
  case FIOBJ_T_HASH:
    return fiobj___test_eq_nested(a, b);
  case FIOBJ_T_OTHER:
    if ((*fiobj_object_metadata(a))->count(a) ||
        (*fiobj_object_metadata(b))->count(b)) {
      if ((*fiobj_object_metadata(a))->count(a) !=
          (*fiobj_object_metadata(b))->count(b))
        return 0;
      return fiobj___test_eq_nested(a, b);
    }
    return (*fiobj_object_metadata(a))->type_id ==
               (*fiobj_object_metadata(b))->type_id &&
           (*fiobj_object_metadata(a))->is_eq(a, b);
  }
  return 0;
}

#define FIOBJ2CSTR_BUFFER_LIMIT 4096
__thread char __attribute__((weak))
fiobj___2cstr___buffer__perthread[FIOBJ2CSTR_BUFFER_LIMIT];

/** Returns a temporary String representation for any FIOBJ object. */
FIO_IFUNC fio_str_info_s FIO_NAME2(fiobj, cstr)(FIOBJ o) {
  switch (FIOBJ_TYPE_CLASS(o)) {
  case FIOBJ_T_PRIMITIVE:
    switch ((uintptr_t)(o)) {
    case FIOBJ_T_NULL:
      return (fio_str_info_s){.buf = (char *)"null", .len = 4};
    case FIOBJ_T_TRUE:
      return (fio_str_info_s){.buf = (char *)"true", .len = 4};
    case FIOBJ_T_FALSE:
      return (fio_str_info_s){.buf = (char *)"false", .len = 5};
    };
    return (fio_str_info_s){.buf = (char *)""};
  case FIOBJ_T_NUMBER:
    return FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), cstr)(o);
  case FIOBJ_T_FLOAT:
    return FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), cstr)(o);
  case FIOBJ_T_STRING:
    return FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), info)(o);
  case FIOBJ_T_ARRAY: /* fallthrough */
  case FIOBJ_T_HASH: {
    FIOBJ j = FIO_NAME2(fiobj, json)(FIOBJ_INVALID, o, 0);
    if (!j || FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(j) >=
                  FIOBJ2CSTR_BUFFER_LIMIT) {
      fiobj_free(j);
      return (fio_str_info_s){.buf = (FIOBJ_TYPE_CLASS(o) == FIOBJ_T_ARRAY
                                          ? (char *)"[...]"
                                          : (char *)"{...}"),
                              .len = 5};
    }
    fio_str_info_s i = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), info)(j);
    memcpy(fiobj___2cstr___buffer__perthread, i.buf, i.len + 1);
    fiobj_free(j);
    i.buf = fiobj___2cstr___buffer__perthread;
    return i;
  }
  case FIOBJ_T_OTHER:
    return (*fiobj_object_metadata(o))->to_s(o);
  }
  if (!o)
    return (fio_str_info_s){.buf = (char *)"null", .len = 4};
  return (fio_str_info_s){.buf = (char *)""};
}

/** Returns an integer representation for any FIOBJ object. */
FIO_IFUNC intptr_t FIO_NAME2(fiobj, i)(FIOBJ o) {
  fio_str_info_s tmp;
  switch (FIOBJ_TYPE_CLASS(o)) {
  case FIOBJ_T_PRIMITIVE:
    switch ((uintptr_t)(o)) {
    case FIOBJ_T_NULL:
      return 0;
    case FIOBJ_T_TRUE:
      return 1;
    case FIOBJ_T_FALSE:
      return 0;
    };
    return -1;
  case FIOBJ_T_NUMBER:
    return FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), i)(o);
  case FIOBJ_T_FLOAT:
    return FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), i)(o);
  case FIOBJ_T_STRING:
    tmp = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), info)(o);
    if (!tmp.len)
      return 0;
    return fio_atol(&tmp.buf);
  case FIOBJ_T_ARRAY:
    return FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), count)(o);
  case FIOBJ_T_HASH:
    return FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), count)(o);
  case FIOBJ_T_OTHER:
    return (*fiobj_object_metadata(o))->to_i(o);
  }
  if (!o)
    return 0;
  return -1;
}

/** Returns a float (double) representation for any FIOBJ object. */
FIO_IFUNC double FIO_NAME2(fiobj, f)(FIOBJ o) {
  fio_str_info_s tmp;
  switch (FIOBJ_TYPE_CLASS(o)) {
  case FIOBJ_T_PRIMITIVE:
    switch ((uintptr_t)(o)) {
    case FIOBJ_T_FALSE: /* fallthrough */
    case FIOBJ_T_NULL:
      return 0.0;
    case FIOBJ_T_TRUE:
      return 1.0;
    };
    return -1.0;
  case FIOBJ_T_NUMBER:
    return FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), f)(o);
  case FIOBJ_T_FLOAT:
    return FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), f)(o);
  case FIOBJ_T_STRING:
    tmp = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), info)(o);
    if (!tmp.len)
      return 0;
    return (double)fio_atof(&tmp.buf);
  case FIOBJ_T_ARRAY:
    return (double)FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), count)(o);
  case FIOBJ_T_HASH:
    return (double)FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), count)(o);
  case FIOBJ_T_OTHER:
    return (*fiobj_object_metadata(o))->to_f(o);
  }
  if (!o)
    return 0.0;
  return -1.0;
}

/* *****************************************************************************
FIOBJ Integers
***************************************************************************** */

#define FIO_REF_NAME fiobj___bignum
#define FIO_REF_TYPE intptr_t
#define FIO_REF_METADATA const FIOBJ_class_vtable_s *
#define FIO_REF_METADATA_INIT(m)                                               \
  do {                                                                         \
    m = &FIOBJ___NUMBER_CLASS_VTBL;                                            \
    FIOBJ_MARK_MEMORY_ALLOC();                                                 \
  } while (0)
#define FIO_REF_METADATA_DESTROY(m)                                            \
  do {                                                                         \
    FIOBJ_MARK_MEMORY_FREE();                                                  \
  } while (0)
#define FIO_PTR_TAG(p) ((uintptr_t)p | FIOBJ_T_OTHER)
#define FIO_PTR_UNTAG(p) FIOBJ_PTR_UNTAG(p)
#define FIO_PTR_TAG_TYPE FIOBJ
#include __FILE__

#define FIO_NUMBER_ENCODE(i) (((uintptr_t)(i) << 3) | FIOBJ_T_NUMBER)
#define FIO_NUMBER_REVESE(i)                                                   \
  ((intptr_t)(((uintptr_t)(i) >> 3) |                                          \
              ((((uintptr_t)(i) >> ((sizeof(uintptr_t) * 8) - 1)) *            \
                ((uintptr_t)3 << ((sizeof(uintptr_t) * 8) - 3))))))

/** Creates a new Number object. */
FIO_IFUNC FIOBJ FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER),
                         new)(intptr_t i) {
  FIOBJ o = (FIOBJ)FIO_NUMBER_ENCODE(i);
  if (FIO_NUMBER_REVESE(o) == i)
    return o;
  o = fiobj___bignum_new2();
  FIO_PTR_MATH_RMASK(intptr_t, o, 3)[0] = i;
  return o;
}

/** Reads the number from a FIOBJ number. */
FIO_IFUNC intptr_t FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), i)(FIOBJ i) {
  if (FIOBJ_TYPE_CLASS(i) == FIOBJ_T_NUMBER)
    return FIO_NUMBER_REVESE(i);
  return FIO_PTR_MATH_RMASK(intptr_t, i, 3)[0];
}

/** Reads the number from a FIOBJ number, fitting it in a double. */
FIO_IFUNC double FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), f)(FIOBJ i) {
  return (double)FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), i)(i);
}

/** Frees a FIOBJ number. */
FIO_IFUNC void FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), free)(FIOBJ i) {
  if (FIOBJ_TYPE_CLASS(i) == FIOBJ_T_NUMBER)
    return;
  fiobj___bignum_free2(i);
  return;
}
#undef FIO_NUMBER_ENCODE
#undef FIO_NUMBER_REVESE

/* *****************************************************************************
FIOBJ Floats
***************************************************************************** */

#define FIO_REF_NAME fiobj___bigfloat
#define FIO_REF_TYPE double
#define FIO_REF_METADATA const FIOBJ_class_vtable_s *
#define FIO_REF_METADATA_INIT(m)                                               \
  do {                                                                         \
    m = &FIOBJ___FLOAT_CLASS_VTBL;                                             \
    FIOBJ_MARK_MEMORY_ALLOC();                                                 \
  } while (0)
#define FIO_REF_METADATA_DESTROY(m)                                            \
  do {                                                                         \
    FIOBJ_MARK_MEMORY_FREE();                                                  \
  } while (0)
#define FIO_PTR_TAG(p) ((uintptr_t)p | FIOBJ_T_OTHER)
#define FIO_PTR_UNTAG(p) FIOBJ_PTR_UNTAG(p)
#define FIO_PTR_TAG_TYPE FIOBJ
#include __FILE__

/** Creates a new Float object. */
FIO_IFUNC FIOBJ FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), new)(double i) {
  FIOBJ ui;
  if (sizeof(double) <= sizeof(FIOBJ)) {
    union {
      double d;
      uintptr_t i;
    } punned;
    punned.i = 0; /* dead code, but leave it, just in case */
    punned.d = i;
    if ((punned.i & 7) == 0) {
      return (FIOBJ)(punned.i | FIOBJ_T_FLOAT);
    }
  }
  ui = fiobj___bigfloat_new2();
  FIO_PTR_MATH_RMASK(double, ui, 3)[0] = i;
  return ui;
}

/** Reads the integer part from a FIOBJ Float. */
FIO_IFUNC intptr_t FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), i)(FIOBJ i) {
  return (intptr_t)floor(FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), f)(i));
}

/** Reads the number from a FIOBJ number, fitting it in a double. */
FIO_IFUNC double FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), f)(FIOBJ i) {
  if (sizeof(double) <= sizeof(FIOBJ) && FIOBJ_TYPE_CLASS(i) == FIOBJ_T_FLOAT) {
    union {
      double d;
      uintptr_t i;
    } punned;
    punned.d = 0; /* dead code, but leave it, just in case */
    punned.i = (uintptr_t)i;
    punned.i = ((uintptr_t)i & (~(uintptr_t)7ULL));
    return punned.d;
  }
  return FIO_PTR_MATH_RMASK(double, i, 3)[0];
}

/** Frees a FIOBJ number. */
FIO_IFUNC void FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), free)(FIOBJ i) {
  if (FIOBJ_TYPE_CLASS(i) == FIOBJ_T_FLOAT)
    return;
  fiobj___bigfloat_free2(i);
  return;
}

/* *****************************************************************************
FIOBJ Basic Iteration
***************************************************************************** */

/**
 * Performs a task for each element held by the FIOBJ object.
 *
 * If `task` returns -1, the `each` loop will break (stop).
 *
 * Returns the "stop" position - the number of elements processed + `start_at`.
 */
FIO_SFUNC uint32_t fiobj_each1(FIOBJ o,
                               int32_t start_at,
                               int (*task)(FIOBJ child, void *arg),
                               void *arg) {
  switch (FIOBJ_TYPE_CLASS(o)) {
  case FIOBJ_T_PRIMITIVE: /* fallthrough */
  case FIOBJ_T_NUMBER:    /* fallthrough */
  case FIOBJ_T_STRING:    /* fallthrough */
  case FIOBJ_T_FLOAT:
    return 0;
  case FIOBJ_T_ARRAY:
    return FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY),
                    each)(o, start_at, task, arg);
  case FIOBJ_T_HASH:
    return FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                    each)(o, start_at, task, arg);
  case FIOBJ_T_OTHER:
    return (*fiobj_object_metadata(o))->each1(o, start_at, task, arg);
  }
  return 0;
}

/* *****************************************************************************
FIOBJ Hash Maps
***************************************************************************** */

/** Calculates an object's hash value for a specific hash map object. */
FIO_IFUNC uint64_t FIO_NAME2(fiobj, hash)(FIOBJ target_hash, FIOBJ o) {
  switch (FIOBJ_TYPE_CLASS(o)) {
  case FIOBJ_T_PRIMITIVE:
    return fio_risky_hash(&o, sizeof(o), (uint64_t)target_hash + (uintptr_t)o);
  case FIOBJ_T_NUMBER: {
    uintptr_t tmp = FIO_NAME2(fiobj, i)(o);
    return fio_risky_hash(&tmp, sizeof(tmp), (uint64_t)target_hash);
  }
  case FIOBJ_T_FLOAT: {
    double tmp = FIO_NAME2(fiobj, f)(o);
    return fio_risky_hash(&tmp, sizeof(tmp), (uint64_t)target_hash);
  }
  case FIOBJ_T_STRING: /* fallthrough */
    return FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING),
                    hash)(o, (uint64_t)target_hash);
  case FIOBJ_T_ARRAY: {
    uint64_t h = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), count)(o);
    size_t c = 0;
    h += fio_risky_hash(&h, sizeof(h), (uint64_t)target_hash + FIOBJ_T_ARRAY);
    FIO_ARRAY_EACH(((FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY),
                              s) *)((uintptr_t)o & (~(uintptr_t)7))),
                   pos) {
      h += FIO_NAME2(fiobj, hash)(target_hash + FIOBJ_T_ARRAY + (c++), *pos);
    }
    return h;
  }
  case FIOBJ_T_HASH: {
    uint64_t h = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), count)(o);
    size_t c = 0;
    h += fio_risky_hash(&h, sizeof(h), (uint64_t)target_hash + FIOBJ_T_HASH);
    FIO_MAP_EACH2(FIO_NAME(fiobj, FIOBJ___NAME_HASH), o, pos) {
      h += FIO_NAME2(fiobj, hash)(target_hash + FIOBJ_T_HASH + (c++),
                                  pos->obj.key);
      h += FIO_NAME2(fiobj, hash)(target_hash + FIOBJ_T_HASH + (c++),
                                  pos->obj.value);
    }
    return h;
  }
  case FIOBJ_T_OTHER: {
    /* TODO: can we avoid "stringifying" the object? */
    fio_str_info_s tmp = (*fiobj_object_metadata(o))->to_s(o);
    return fio_risky_hash(tmp.buf, tmp.len, (uint64_t)target_hash);
  }
  }
  return 0;
}

/** Inserts a value to a hash map, with a default hash value calculation. */
FIO_IFUNC FIOBJ FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                         set2)(FIOBJ hash, FIOBJ key, FIOBJ value) {
  return FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), set)(
      hash, FIO_NAME2(fiobj, hash)(hash, key), key, value, NULL);
}

/** Finds a value in a hash map, automatically calculating the hash value. */
FIO_IFUNC FIOBJ FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), get2)(FIOBJ hash,
                                                                   FIOBJ key) {
  if (FIOBJ_TYPE_CLASS(hash) != FIOBJ_T_HASH)
    return FIOBJ_INVALID;
  return FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                  get)(hash, FIO_NAME2(fiobj, hash)(hash, key), key);
}

/** Removes a value from a hash map, with a default hash value calculation. */
FIO_IFUNC int FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                       remove2)(FIOBJ hash, FIOBJ key, FIOBJ *old) {
  return FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                  remove)(hash, FIO_NAME2(fiobj, hash)(hash, key), key, old);
}

/**
 * Sets a String value in a hash map, allocating the String and automatically
 * calculating the hash value.
 */
FIO_IFUNC FIOBJ FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                         set3)(FIOBJ hash,
                               const char *key,
                               size_t len,
                               FIOBJ value) {
  FIOBJ tmp = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), new)();
  FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)(tmp, (char *)key, len);
  FIOBJ v = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), set)(
      hash, fio_risky_hash(key, len, (uint64_t)hash), tmp, value, NULL);
  fiobj_free(tmp);
  return v;
}

/**
 * Finds a String value in a hash map, using a temporary String and
 * automatically calculating the hash value.
 */
FIO_IFUNC FIOBJ FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                         get3)(FIOBJ hash, const char *buf, size_t len) {
  if (FIOBJ_TYPE_CLASS(hash) != FIOBJ_T_HASH)
    return FIOBJ_INVALID;
  FIOBJ_STR_TEMP_VAR_STATIC(tmp, buf, len);
  FIOBJ v = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                     get)(hash, fio_risky_hash(buf, len, (uint64_t)hash), tmp);
  return v;
}

/**
 * Removes a String value in a hash map, using a temporary String and
 * automatically calculating the hash value.
 */
FIO_IFUNC int
FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
         remove3)(FIOBJ hash, const char *buf, size_t len, FIOBJ *old) {
  FIOBJ_STR_TEMP_VAR_STATIC(tmp, buf, len);
  int r = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), remove)(
      hash, fio_risky_hash(buf, len, (uint64_t)hash), tmp, old);
  FIOBJ_STR_TEMP_DESTROY(tmp);
  return r;
}

/* *****************************************************************************
FIOBJ JSON support (inline functions)
***************************************************************************** */

typedef struct {
  FIOBJ json;
  size_t level;
  uint8_t beautify;
} fiobj___json_format_internal__s;

/* internal helper funnction for recursive JSON formatting. */
FIOBJ_FUNC void
fiobj___json_format_internal__(fiobj___json_format_internal__s *, FIOBJ);

/** Helper function, calls `fiobj_hash_update_json` with string information */
FIO_IFUNC size_t FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                          update_json2)(FIOBJ hash, char *ptr, size_t len) {
  return FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                  update_json)(hash, (fio_str_info_s){.buf = ptr, .len = len});
}

/**
 * Returns a JSON valid FIOBJ String, representing the object.
 *
 * If `dest` is an existing String, the formatted JSON data will be appended to
 * the existing string.
 */
FIO_IFUNC FIOBJ FIO_NAME2(fiobj, json)(FIOBJ dest, FIOBJ o, uint8_t beautify) {
  fiobj___json_format_internal__s args =
      (fiobj___json_format_internal__s){.json = dest, .beautify = beautify};
  if (FIOBJ_TYPE_CLASS(dest) != FIOBJ_T_STRING)
    args.json = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), new)();
  fiobj___json_format_internal__(&args, o);
  return args.json;
}

/* *****************************************************************************
FIOBJ - Implementation
***************************************************************************** */
#ifdef FIOBJ_EXTERN_COMPLETE

/* *****************************************************************************
FIOBJ Basic Object vtable
***************************************************************************** */

FIOBJ_EXTERN_OBJ_IMP const FIOBJ_class_vtable_s FIOBJ___OBJECT_CLASS_VTBL = {
    .type_id = 99, /* type IDs below 100 are reserved. */
};

/* *****************************************************************************
FIOBJ Complex Iteration
***************************************************************************** */
typedef struct {
  FIOBJ obj;
  size_t pos;
} fiobj____stack_element_s;

#define FIO_ARRAY_NAME fiobj____active_stack
#define FIO_ARRAY_TYPE fiobj____stack_element_s
#define FIO_ARRAY_COPY(dest, src)                                              \
  do {                                                                         \
    (dest).obj = fiobj_dup((src).obj);                                         \
    (dest).pos = (src).pos;                                                    \
  } while (0)
#define FIO_ARRAY_TYPE_CMP(a, b) (a).obj == (b).obj
#define FIO_ARRAY_DESTROY(o) fiobj_free(o)
#include __FILE__
#define FIO_ARRAY_TYPE_CMP(a, b) (a).obj == (b).obj
#define FIO_ARRAY_NAME fiobj____stack
#define FIO_ARRAY_TYPE fiobj____stack_element_s
#include __FILE__

typedef struct {
  int (*task)(FIOBJ, void *);
  void *arg;
  FIOBJ next;
  size_t count;
  fiobj____stack_s stack;
  uint32_t end;
  uint8_t stop;
} fiobj_____each2_data_s;

FIO_SFUNC uint32_t fiobj____each2_element_count(FIOBJ o) {
  switch (FIOBJ_TYPE_CLASS(o)) {
  case FIOBJ_T_PRIMITIVE: /* fallthrough */
  case FIOBJ_T_NUMBER:    /* fallthrough */
  case FIOBJ_T_STRING:    /* fallthrough */
  case FIOBJ_T_FLOAT:
    return 0;
  case FIOBJ_T_ARRAY:
    return FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), count)(o);
  case FIOBJ_T_HASH:
    return FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), count)(o);
  case FIOBJ_T_OTHER: /* fallthrough */
    return (*fiobj_object_metadata(o))->count(o);
  }
  return 0;
}
FIO_SFUNC int fiobj____each2_wrapper_task(FIOBJ child, void *arg) {
  fiobj_____each2_data_s *d = (fiobj_____each2_data_s *)arg;
  d->stop = (d->task(child, d->arg) == -1);
  ++d->count;
  if (d->stop)
    return -1;
  uint32_t c = fiobj____each2_element_count(child);
  if (c) {
    d->next = child;
    d->end = c;
    return -1;
  }
  return 0;
}

/**
 * Performs a task for the object itself and each element held by the FIOBJ
 * object or any of it's elements (a deep task).
 *
 * The order of performance is by order of appearance, as if all nesting levels
 * were flattened.
 *
 * If `task` returns -1, the `each` loop will break (stop).
 *
 * Returns the number of elements processed.
 */
FIOBJ_FUNC uint32_t fiobj_each2(FIOBJ o,
                                int (*task)(FIOBJ child, void *arg),
                                void *arg) {
  /* TODO - move to recursion with nesting limiter? */
  fiobj_____each2_data_s d = {
      .task = task,
      .arg = arg,
      .next = FIOBJ_INVALID,
      .stack = FIO_ARRAY_INIT,
  };
  fiobj____stack_element_s i = {.obj = o, .pos = 0};
  uint32_t end = fiobj____each2_element_count(o);
  fiobj____each2_wrapper_task(i.obj, &d);
  while (!d.stop && i.obj && i.pos < end) {
    i.pos = fiobj_each1(i.obj, i.pos, fiobj____each2_wrapper_task, &d);
    if (d.next != FIOBJ_INVALID) {
      if (fiobj____stack_count(&d.stack) + 1 > FIOBJ_MAX_NESTING) {
        FIO_LOG_ERROR("FIOBJ nesting level too deep (%u)."
                      "`fiobj_each2` stopping loop early.",
                      (unsigned int)fiobj____stack_count(&d.stack));
        d.stop = 1;
        continue;
      }
      fiobj____stack_push(&d.stack, i);
      i.pos = 0;
      i.obj = d.next;
      d.next = FIOBJ_INVALID;
      end = d.end;
    } else {
      /* re-collect end position to acommodate for changes */
      end = fiobj____each2_element_count(i.obj);
    }
    while (i.pos >= end && fiobj____stack_count(&d.stack)) {
      fiobj____stack_pop(&d.stack, &i);
      end = fiobj____each2_element_count(i.obj);
    }
  };
  fiobj____stack_destroy(&d.stack);
  return d.count;
}

/* *****************************************************************************
FIOBJ Hash / Array / Other (enumerable) Equality test.
***************************************************************************** */

FIO_SFUNC __thread size_t fiobj___test_eq_nested_level = 0;
/** Internal: compares two nestable objects. */
FIOBJ_FUNC unsigned char fiobj___test_eq_nested(FIOBJ restrict a,
                                                FIOBJ restrict b) {
  if (a == b)
    return 1;
  if (FIOBJ_TYPE_CLASS(a) != FIOBJ_TYPE_CLASS(b))
    return 0;
  if (fiobj____each2_element_count(a) != fiobj____each2_element_count(b))
    return 0;
  if (!fiobj____each2_element_count(a))
    return 1;
  if (fiobj___test_eq_nested_level >= FIOBJ_MAX_NESTING)
    return 0;
  ++fiobj___test_eq_nested_level;

  switch (FIOBJ_TYPE_CLASS(a)) {
  case FIOBJ_T_PRIMITIVE:
  case FIOBJ_T_NUMBER: /* fallthrough */
  case FIOBJ_T_FLOAT:  /* fallthrough */
  case FIOBJ_T_STRING: /* fallthrough */
    /* should never happen... this function is for enumerable objects */
    return a == b;
  case FIOBJ_T_ARRAY:
    /* test each array member with matching index */
    {
      const size_t count = fiobj____each2_element_count(a);
      for (size_t i = 0; i < count; ++i) {
        if (!FIO_NAME_BL(fiobj, eq)(
                FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), get)(a, i),
                FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), get)(b, i)))
          goto unequal;
      }
    }
    goto equal;
  case FIOBJ_T_HASH:
    FIO_MAP_EACH2(FIO_NAME(fiobj, FIOBJ___NAME_HASH), a, pos) {
      FIOBJ val = fiobj_hash_get2(b, pos->obj.key);
      if (!FIO_NAME_BL(fiobj, eq)(val, pos->obj.value))
        goto equal;
    }
    goto equal;
  case FIOBJ_T_OTHER:
    return (*fiobj_object_metadata(a))->is_eq(a, b);
  }
equal:
  --fiobj___test_eq_nested_level;
  return 1;
unequal:
  --fiobj___test_eq_nested_level;
  return 0;
}

/* *****************************************************************************
FIOBJ general helpers
***************************************************************************** */
FIO_SFUNC __thread char fiobj___tmp_buffer[256];

FIO_SFUNC uint32_t fiobj___count_noop(FIOBJ o) {
  return 0;
  (void)o;
}

/* *****************************************************************************
FIOBJ Integers (bigger numbers)
***************************************************************************** */

FIOBJ_FUNC unsigned char FIO_NAME_BL(fiobj___num, eq)(FIOBJ restrict a,
                                                      FIOBJ restrict b) {
  return FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), i)(a) ==
         FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), i)(b);
}

FIOBJ_FUNC fio_str_info_s FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER),
                                    cstr)(FIOBJ i) {
  size_t len = fio_ltoa(fiobj___tmp_buffer,
                        FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), i)(i),
                        10);
  fiobj___tmp_buffer[len] = 0;
  return (fio_str_info_s){.buf = fiobj___tmp_buffer, .len = len};
}

FIOBJ_EXTERN_OBJ_IMP const FIOBJ_class_vtable_s FIOBJ___NUMBER_CLASS_VTBL = {
    /**
     * MUST return a unique number to identify object type.
     *
     * Numbers (IDs) under 100 are reserved.
     */
    .type_id = FIOBJ_T_NUMBER,
    /** Test for equality between two objects with the same `type_id` */
    .is_eq = FIO_NAME_BL(fiobj___num, eq),
    /** Converts an object to a String */
    .to_s = FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), cstr),
    /** Converts and object to an integer */
    .to_i = FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), i),
    /** Converts and object to a float */
    .to_f = FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), f),
    /** Returns the number of exposed elements held by the object, if any. */
    .count = fiobj___count_noop,
    /** Iterates the exposed elements held by the object. See `fiobj_each1`. */
    .each1 = NULL,
    /** Deallocates the element (but NOT any of it's exposed elements). */
    .free2 = fiobj___bignum_free2,
};

/* *****************************************************************************
FIOBJ Floats (bigger / smaller doubles)
***************************************************************************** */

FIOBJ_FUNC unsigned char FIO_NAME_BL(fiobj___float, eq)(FIOBJ restrict a,
                                                        FIOBJ restrict b) {
  return FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), i)(a) ==
         FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), i)(b);
}

FIOBJ_FUNC fio_str_info_s FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT),
                                    cstr)(FIOBJ i) {
  size_t len = fio_ftoa(fiobj___tmp_buffer,
                        FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), f)(i),
                        10);
  fiobj___tmp_buffer[len] = 0;
  return (fio_str_info_s){.buf = fiobj___tmp_buffer, .len = len};
}

FIOBJ_EXTERN_OBJ_IMP const FIOBJ_class_vtable_s FIOBJ___FLOAT_CLASS_VTBL = {
    /**
     * MUST return a unique number to identify object type.
     *
     * Numbers (IDs) under 100 are reserved.
     */
    .type_id = FIOBJ_T_FLOAT,
    /** Test for equality between two objects with the same `type_id` */
    .is_eq = FIO_NAME_BL(fiobj___float, eq),
    /** Converts an object to a String */
    .to_s = FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), cstr),
    /** Converts and object to an integer */
    .to_i = FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), i),
    /** Converts and object to a float */
    .to_f = FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), f),
    /** Returns the number of exposed elements held by the object, if any. */
    .count = fiobj___count_noop,
    /** Iterates the exposed elements held by the object. See `fiobj_each1`. */
    .each1 = NULL,
    /** Deallocates the element (but NOT any of it's exposed elements). */
    .free2 = fiobj___bigfloat_free2,
};

/* *****************************************************************************
FIOBJ JSON support - output
***************************************************************************** */

FIO_IFUNC void fiobj___json_format_internal_beauty_pad(FIOBJ json,
                                                       size_t level) {
  size_t pos = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(json);
  fio_str_info_s tmp = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING),
                                resize)(json, (level << 1) + pos + 2);
  tmp.buf[pos++] = '\r';
  tmp.buf[pos++] = '\n';
  for (size_t i = 0; i < level; ++i) {
    tmp.buf[pos++] = ' ';
    tmp.buf[pos++] = ' ';
  }
}

FIOBJ_FUNC void
fiobj___json_format_internal__(fiobj___json_format_internal__s *args, FIOBJ o) {
  switch (FIOBJ_TYPE(o)) {
  case FIOBJ_T_TRUE:   /* fallthrough */
  case FIOBJ_T_FALSE:  /* fallthrough */
  case FIOBJ_T_NULL:   /* fallthrough */
  case FIOBJ_T_NUMBER: /* fallthrough */
  case FIOBJ_T_FLOAT:  /* fallthrough */
  {
    fio_str_info_s info = FIO_NAME2(fiobj, cstr)(o);
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)
    (args->json, info.buf, info.len);
    return;
  }
  case FIOBJ_T_STRING: /* fallthrough */
  default: {
    fio_str_info_s info = FIO_NAME2(fiobj, cstr)(o);
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)(args->json, "\"", 1);
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write_escape)
    (args->json, info.buf, info.len);
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)(args->json, "\"", 1);
    return;
  }
  case FIOBJ_T_ARRAY:
    if (args->level == FIOBJ_MAX_NESTING) {
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)
      (args->json, "[ ]", 3);
      return;
    }
    {
      ++args->level;
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)(args->json, "[", 1);
      const uint32_t len =
          FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), count)(o);
      for (size_t i = 0; i < len; ++i) {
        if (args->beautify) {
          fiobj___json_format_internal_beauty_pad(args->json, args->level);
        }
        FIOBJ child = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), get)(o, i);
        fiobj___json_format_internal__(args, child);
        if (i + 1 < len)
          FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)
        (args->json, ",", 1);
      }
      --args->level;
      if (args->beautify) {
        fiobj___json_format_internal_beauty_pad(args->json, args->level);
      }
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)(args->json, "]", 1);
    }
    return;
  case FIOBJ_T_HASH:
    if (args->level == FIOBJ_MAX_NESTING) {
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)
      (args->json, "{ }", 3);
      return;
    }
    {
      ++args->level;
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)(args->json, "{", 1);
      size_t i = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), count)(o);
      if (i) {
        FIO_MAP_EACH2(FIO_NAME(fiobj, FIOBJ___NAME_HASH), o, couplet) {
          if (args->beautify) {
            fiobj___json_format_internal_beauty_pad(args->json, args->level);
          }
          fio_str_info_s info = FIO_NAME2(fiobj, cstr)(couplet->obj.key);
          FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)
          (args->json, "\"", 1);
          FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write_escape)
          (args->json, info.buf, info.len);
          FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)
          (args->json, "\":", 2);
          fiobj___json_format_internal__(args, couplet->obj.value);
          if (--i)
            FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)
          (args->json, ",", 1);
        }
      }
      --args->level;
      if (args->beautify) {
        fiobj___json_format_internal_beauty_pad(args->json, args->level);
      }
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)(args->json, "}", 1);
    }
    return;
  }
}

/* *****************************************************************************
FIOBJ JSON parsing
***************************************************************************** */

#define FIO_JSON
#include __FILE__

/* FIOBJ JSON parser */
typedef struct {
  fio_json_parser_s p;
  FIOBJ key;
  FIOBJ top;
  FIOBJ target;
  FIOBJ stack[JSON_MAX_DEPTH + 1];
  uint8_t so; /* stack offset */
} fiobj_json_parser_s;

static inline void fiobj_json_add2parser(fiobj_json_parser_s *p, FIOBJ o) {
  if (p->top) {
    if (FIOBJ_TYPE_CLASS(p->top) == FIOBJ_T_HASH) {
      if (p->key) {
        FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), set2)(p->top, p->key, o);
        fiobj_free(p->key);
        p->key = FIOBJ_INVALID;
      } else {
        p->key = o;
      }
    } else {
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)(p->top, o);
    }
  } else {
    p->top = o;
  }
}

/** a NULL object was detected */
static inline void fio_json_on_null(fio_json_parser_s *p) {
  fiobj_json_add2parser((fiobj_json_parser_s *)p,
                        FIO_NAME(fiobj, FIOBJ___NAME_NULL)());
}
/** a TRUE object was detected */
static inline void fio_json_on_true(fio_json_parser_s *p) {
  fiobj_json_add2parser((fiobj_json_parser_s *)p,
                        FIO_NAME(fiobj, FIOBJ___NAME_TRUE)());
}
/** a FALSE object was detected */
static inline void fio_json_on_false(fio_json_parser_s *p) {
  fiobj_json_add2parser((fiobj_json_parser_s *)p,
                        FIO_NAME(fiobj, FIOBJ___NAME_FALSE)());
}
/** a Numberl was detected (long long). */
static inline void fio_json_on_number(fio_json_parser_s *p, long long i) {
  fiobj_json_add2parser((fiobj_json_parser_s *)p,
                        FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), new)(i));
}
/** a Float was detected (double). */
static inline void fio_json_on_float(fio_json_parser_s *p, double f) {
  fiobj_json_add2parser((fiobj_json_parser_s *)p,
                        FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), new)(f));
}
/** a String was detected (int / float). update `pos` to point at ending */
static inline void
fio_json_on_string(fio_json_parser_s *p, const void *start, size_t len) {
  FIOBJ str = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), new)();
  FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write_unescape)
  (str, start, len);
  fiobj_json_add2parser((fiobj_json_parser_s *)p, str);
}
/** a dictionary object was detected */
static inline int fio_json_on_start_object(fio_json_parser_s *p) {
  fiobj_json_parser_s *pr = (fiobj_json_parser_s *)p;
  if (pr->target) {
    /* push NULL, don't free the objects */
    pr->stack[pr->so++] = FIOBJ_INVALID;
    pr->top = pr->target;
    pr->target = FIOBJ_INVALID;
  } else {
    FIOBJ hash = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), new)();
    fiobj_json_add2parser(pr, hash);
    pr->stack[pr->so++] = pr->top;
    pr->top = hash;
  }
  return 0;
}
/** a dictionary object closure detected */
static inline void fio_json_on_end_object(fio_json_parser_s *p) {
  fiobj_json_parser_s *pr = (fiobj_json_parser_s *)p;
  if (pr->key) {
    FIO_LOG_WARNING("(JSON parsing) malformed JSON, "
                    "ignoring dangling Hash key.");
    fiobj_free(pr->key);
    pr->key = FIOBJ_INVALID;
  }
  pr->top = FIOBJ_INVALID;
  if (pr->so)
    pr->top = pr->stack[--pr->so];
}
/** an array object was detected */
static int fio_json_on_start_array(fio_json_parser_s *p) {
  fiobj_json_parser_s *pr = (fiobj_json_parser_s *)p;
  if (pr->target)
    return -1;
  FIOBJ ary = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), new)();
  fiobj_json_add2parser(pr, ary);
  pr->stack[pr->so++] = pr->top;
  pr->top = ary;
  return 0;
}
/** an array closure was detected */
static inline void fio_json_on_end_array(fio_json_parser_s *p) {
  fiobj_json_parser_s *pr = (fiobj_json_parser_s *)p;
  pr->top = FIOBJ_INVALID;
  if (pr->so)
    pr->top = pr->stack[--pr->so];
}
/** the JSON parsing is complete */
static void fio_json_on_json(fio_json_parser_s *p) {
  (void)p; /* nothing special... right? */
}
/** the JSON parsing is complete */
static inline void fio_json_on_error(fio_json_parser_s *p) {
  fiobj_json_parser_s *pr = (fiobj_json_parser_s *)p;
  fiobj_free(pr->stack[0]);
  fiobj_free(pr->key);
  *pr = (fiobj_json_parser_s){.top = FIOBJ_INVALID};
  FIO_LOG_DEBUG("JSON on_error callback called.");
}

/**
 * Updates a Hash using JSON data.
 *
 * Parsing errors and non-dictionary object JSON data are silently ignored,
 * attempting to update the Hash as much as possible before any errors
 * encountered.
 *
 * Conflicting Hash data is overwritten (preferring the new over the old).
 *
 * Returns the number of bytes consumed. On Error, 0 is returned and no data is
 * consumed.
 */
FIOBJ_FUNC size_t FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH),
                           update_json)(FIOBJ hash, fio_str_info_s str) {
  if (hash == FIOBJ_INVALID)
    return 0;
  fiobj_json_parser_s p = {.top = FIOBJ_INVALID, .target = hash};
  size_t consumed = fio_json_parse(&p.p, str.buf, str.len);
  fiobj_free(p.key);
  if (p.top != hash)
    fiobj_free(p.top);
  return consumed;
}

/** Returns a JSON valid FIOBJ String, representing the object. */
FIOBJ_FUNC FIOBJ fiobj_json_parse(fio_str_info_s str, size_t *consumed_p) {
  fiobj_json_parser_s p = {.top = FIOBJ_INVALID};
  register const size_t consumed = fio_json_parse(&p.p, str.buf, str.len);
  if (consumed_p) {
    *consumed_p = consumed;
  }
  if (!consumed || p.p.depth) {
    if (p.top) {
      FIO_LOG_DEBUG("WARNING - JSON failed secondary validation, no on_error");
    }
#if DEBUG
    FIOBJ s = FIO_NAME2(fiobj, json)(FIOBJ_INVALID, p.top, 0);
    FIO_LOG_DEBUG("JSON data being deleted:\n%s",
                  FIO_NAME2(fiobj, cstr)(s).buf);
    fiobj_free(s);
#endif
    fiobj_free(p.stack[0]);
    p.top = FIOBJ_INVALID;
  }
  fiobj_free(p.key);
  return p.top;
}

/* *****************************************************************************
FIOBJ and JSON testing
***************************************************************************** */
#ifdef FIO_TEST_CSTL
FIO_SFUNC int FIO_NAME_TEST(stl, fiobj_task)(FIOBJ o, void *e_) {
  static size_t index = 0;
  int *expect = (int *)e_;
  if (expect[index] == -1) {
    FIO_ASSERT(FIOBJ_TYPE(o) == FIOBJ_T_ARRAY,
               "each2 ordering issue [%zu] (array).",
               index);
  } else {
    FIO_ASSERT(FIO_NAME2(fiobj, i)(o) == expect[index],
               "each2 ordering issue [%zu] (number) %ld != %d",
               index,
               FIO_NAME2(fiobj, i)(o),
               expect[index]);
  }
  ++index;
  return 0;
}

FIO_SFUNC void FIO_NAME_TEST(stl, fiobj)(void) {
  FIOBJ o = FIOBJ_INVALID;
  if (!FIOBJ_MARK_MEMORY_ENABLED) {
    FIO_LOG_WARNING("FIOBJ defined without allocation counter. "
                    "Tests might not be complete.");
  }
  /* primitives - (in)sanity */
  {
    fprintf(stderr, "* Testing FIOBJ primitives.\n");
    FIO_ASSERT(FIOBJ_TYPE(o) == FIOBJ_T_NULL,
               "invalid FIOBJ type should be FIOBJ_T_NULL.");
    FIO_ASSERT(!FIO_NAME_BL(fiobj, eq)(o, FIO_NAME(fiobj, FIOBJ___NAME_NULL)()),
               "invalid FIOBJ is NOT a fiobj_null().");
    FIO_ASSERT(!FIO_NAME_BL(fiobj, eq)(FIO_NAME(fiobj, FIOBJ___NAME_TRUE)(),
                                       FIO_NAME(fiobj, FIOBJ___NAME_NULL)()),
               "fiobj_true() is NOT fiobj_null().");
    FIO_ASSERT(!FIO_NAME_BL(fiobj, eq)(FIO_NAME(fiobj, FIOBJ___NAME_FALSE)(),
                                       FIO_NAME(fiobj, FIOBJ___NAME_NULL)()),
               "fiobj_false() is NOT fiobj_null().");
    FIO_ASSERT(!FIO_NAME_BL(fiobj, eq)(FIO_NAME(fiobj, FIOBJ___NAME_FALSE)(),
                                       FIO_NAME(fiobj, FIOBJ___NAME_TRUE)()),
               "fiobj_false() is NOT fiobj_true().");
    FIO_ASSERT(FIOBJ_TYPE(FIO_NAME(fiobj, FIOBJ___NAME_NULL)()) == FIOBJ_T_NULL,
               "fiobj_null() type should be FIOBJ_T_NULL.");
    FIO_ASSERT(FIOBJ_TYPE(FIO_NAME(fiobj, FIOBJ___NAME_TRUE)()) == FIOBJ_T_TRUE,
               "fiobj_true() type should be FIOBJ_T_TRUE.");
    FIO_ASSERT(FIOBJ_TYPE(FIO_NAME(fiobj, FIOBJ___NAME_FALSE)()) ==
                   FIOBJ_T_FALSE,
               "fiobj_false() type should be FIOBJ_T_FALSE.");
    FIO_ASSERT(FIO_NAME_BL(fiobj, eq)(FIO_NAME(fiobj, FIOBJ___NAME_NULL)(),
                                      FIO_NAME(fiobj, FIOBJ___NAME_NULL)()),
               "fiobj_null() should be equal to self.");
    FIO_ASSERT(FIO_NAME_BL(fiobj, eq)(FIO_NAME(fiobj, FIOBJ___NAME_TRUE)(),
                                      FIO_NAME(fiobj, FIOBJ___NAME_TRUE)()),
               "fiobj_true() should be equal to self.");
    FIO_ASSERT(FIO_NAME_BL(fiobj, eq)(FIO_NAME(fiobj, FIOBJ___NAME_FALSE)(),
                                      FIO_NAME(fiobj, FIOBJ___NAME_FALSE)()),
               "fiobj_false() should be equal to self.");
  }
  {
    fprintf(stderr, "* Testing FIOBJ integers.\n");
    uint8_t allocation_flags = 0;
    for (uint8_t bit = 0; bit < (sizeof(intptr_t) * 8); ++bit) {
      uintptr_t i = (uintptr_t)1 << bit;
      o = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), new)((intptr_t)i);
      FIO_ASSERT(FIO_NAME2(fiobj, i)(o) == (intptr_t)i,
                 "Number not reversible at bit %d (%zd != %zd)!",
                 (int)bit,
                 (ssize_t)FIO_NAME2(fiobj, i)(o),
                 (ssize_t)i);
      allocation_flags |= (FIOBJ_TYPE_CLASS(o) == FIOBJ_T_NUMBER) ? 1 : 2;
      fiobj_free(o);
    }
    FIO_ASSERT(allocation_flags == 3,
               "no bits are allocated / no allocations optimized away (%d)",
               (int)allocation_flags);
  }
  {
    fprintf(stderr, "* Testing FIOBJ floats.\n");
    uint8_t allocation_flags = 0;
    for (uint8_t bit = 0; bit < (sizeof(double) * 8); ++bit) {
      union {
        double d;
        uint64_t i;
      } punned;
      punned.i = (uint64_t)1 << bit;
      o = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), new)(punned.d);
      FIO_ASSERT(FIO_NAME2(fiobj, f)(o) == punned.d,
                 "Float not reversible at bit %d (%lf != %lf)!",
                 (int)bit,
                 FIO_NAME2(fiobj, f)(o),
                 punned.d);
      allocation_flags |= (FIOBJ_TYPE_CLASS(o) == FIOBJ_T_FLOAT) ? 1 : 2;
      fiobj_free(o);
    }
    FIO_ASSERT(allocation_flags == 3,
               "no bits are allocated / no allocations optimized away (%d)",
               (int)allocation_flags);
  }
  {
    fprintf(stderr, "* Testing FIOBJ each2.\n");
    FIOBJ a = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), new)();
    o = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), new)();
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)(o, a);
    for (int i = 1; i < 10; ++i) // 1, 2, 3 ... 10
    {
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)
      (a, FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), new)(i));
      if (i % 3 == 0) {
        a = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), new)();
        FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)(o, a);
      }
    }
    int expectation[] = {
        -1 /* array */, -1, 1, 2, 3, -1, 4, 5, 6, -1, 7, 8, 9, -1};
    size_t c =
        fiobj_each2(o, FIO_NAME_TEST(stl, fiobj_task), (void *)expectation);
    FIO_ASSERT(c == FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), count)(o) +
                        9 + 1,
               "each2 repetition count error");
    fiobj_free(o);
  }
  {
    fprintf(stderr, "* Testing FIOBJ JSON handling.\n");
    char json[] =
        "                    "
        "\n# comment 1"
        "\n// comment 2"
        "\n/* comment 3 */"
        "{\"true\":true,\"false\":false,\"null\":null,\"array\":[1,2,3,4.2,"
        "\"five\"],"
        "\"string\":\"hello\\tjson\\bworld!\\r\\n\",\"hash\":{\"true\":true,"
        "\"false\":false},\"array2\":[1,2,3,4.2,\"five\",{\"hash\":true}]}";
    o = fiobj_json_parse2(json, strlen(json), NULL);
    FIO_ASSERT(o, "JSON parsing failed - no data returned.");
    FIOBJ j = FIO_NAME2(fiobj, json)(FIOBJ_INVALID, o, 0);
#if DEBUG
    fprintf(stderr, "JSON: %s\n", FIO_NAME2(fiobj, cstr)(j).buf);
#endif
    FIO_ASSERT(FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(j) ==
                   strlen(json + 61),
               "JSON roundtrip failed (length error).");
    FIO_ASSERT(!memcmp(json + 61,
                       FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING), ptr)(j),
                       strlen(json + 61)),
               "JSON roundtrip failed (data error).");
    fiobj_free(o);
    fiobj_free(j);
    o = FIOBJ_INVALID;
  }
  {
    fprintf(stderr, "* Testing FIOBJ array equality test (fiobj_is_eq).\n");
    FIOBJ a1 = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), new)();
    FIOBJ a2 = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), new)();
    FIOBJ n1 = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), new)();
    FIOBJ n2 = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), new)();
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)(a1, fiobj_null());
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)(a2, fiobj_null());
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)(n1, fiobj_true());
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)(n2, fiobj_true());
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)(a1, n1);
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)(a2, n2);
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)
    (a1, FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), new_cstr)("test", 4));
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)
    (a2, FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), new_cstr)("test", 4));
    FIO_ASSERT(FIO_NAME_BL(fiobj, eq)(a1, a2), "equal arrays aren't equal?");
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)(n1, fiobj_null());
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)(n2, fiobj_false());
    FIO_ASSERT(!FIO_NAME_BL(fiobj, eq)(a1, a2), "unequal arrays are equal?");
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), remove)(n1, -1, NULL);
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), remove)(n2, -1, NULL);
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), remove)(a1, 0, NULL);
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), remove)(a2, -1, NULL);
    FIO_ASSERT(!FIO_NAME_BL(fiobj, eq)(a1, a2), "unequal arrays are equal?");
    fiobj_free(a1);
    fiobj_free(a2);
  }
  {
    fprintf(stderr, "* Testing FIOBJ array ownership.\n");
    FIOBJ a = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), new)();
    for (int i = 1; i <= TEST_REPEAT; ++i) {
      FIOBJ tmp = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING),
                           new_cstr)("number: ", 8);
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write_i)(tmp, i);
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)(a, tmp);
    }
    FIOBJ shifted = FIOBJ_INVALID;
    FIOBJ popped = FIOBJ_INVALID;
    FIOBJ removed = FIOBJ_INVALID;
    FIOBJ set = FIOBJ_INVALID;
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), shift)(a, &shifted);
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), pop)(a, &popped);
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), set)
    (a, 1, FIO_NAME(fiobj, FIOBJ___NAME_TRUE)(), &set);
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), remove)(a, 2, &removed);
    fiobj_free(a);
    if (1) {
      FIO_ASSERT(
          FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(popped) ==
                  strlen("number: " FIO_MACRO2STR(TEST_REPEAT)) &&
              !memcmp(
                  "number: " FIO_MACRO2STR(TEST_REPEAT),
                  FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING), ptr)(popped),
                  FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(popped)),
          "Object popped from Array lost it's value %s",
          FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING), ptr)(popped));
      FIO_ASSERT(FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(shifted) ==
                         9 &&
                     !memcmp("number: 1",
                             FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING),
                                       ptr)(shifted),
                             9),
                 "Object shifted from Array lost it's value %s",
                 FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING), ptr)(shifted));
      FIO_ASSERT(
          FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(set) == 9 &&
              !memcmp("number: 3",
                      FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING), ptr)(set),
                      9),
          "Object retrieved from Array using fiobj_array_set() lost it's "
          "value %s",
          FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING), ptr)(set));
      FIO_ASSERT(
          FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(removed) == 9 &&
              !memcmp(
                  "number: 4",
                  FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING), ptr)(removed),
                  9),
          "Object retrieved from Array using fiobj_array_set() lost it's "
          "value %s",
          FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING), ptr)(removed));
    }
    fiobj_free(shifted);
    fiobj_free(popped);
    fiobj_free(set);
    fiobj_free(removed);
  }
  {
    fprintf(stderr, "* Testing FIOBJ hash ownership after concat.\n");
    FIOBJ a1, a2;
    a1 = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), new)();
    a2 = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), new)();
    for (int i = 0; i < TEST_REPEAT; ++i) {
      FIOBJ str = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), new)();
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write_i)(str, i);
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)(a1, str);
    }
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), concat)(a2, a1);
    fiobj_free(a1);
    for (int i = 0; i < TEST_REPEAT; ++i) {
      FIOBJ_STR_TEMP_VAR(tmp);
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write_i)(tmp, i);
      FIO_ASSERT(FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(FIO_NAME(
                     FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), get)(a2, i)) ==
                     FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(tmp),
                 "string length zeroed out - string freed?");
      FIO_ASSERT(
          !memcmp(FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING), ptr)(tmp),
                  FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING), ptr)(FIO_NAME(
                      FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), get)(a2, i)),
                  FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(tmp)),
          "string data error - string freed?");
      FIOBJ_STR_TEMP_DESTROY(tmp);
    }
    fiobj_free(a2);
  }
  {
    fprintf(stderr, "* Testing FIOBJ hash ownership.\n");
    o = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), new)();
    for (int i = 1; i <= TEST_REPEAT; ++i) {
      FIOBJ tmp = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING),
                           new_cstr)("number: ", 8);
      FIOBJ k = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), new)(i);
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write_i)(tmp, i);
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), set2)(o, k, tmp);
      fiobj_free(k);
    }

    FIOBJ set = FIOBJ_INVALID;
    FIOBJ removed = FIOBJ_INVALID;
    FIOBJ k = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), new)(1);
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), remove2)(o, k, &removed);
    fiobj_free(k);
    k = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), new)(2);
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), set)
    (o, fiobj2hash(o, k), k, FIO_NAME(fiobj, FIOBJ___NAME_TRUE)(), &set);
    fiobj_free(k);
    FIO_ASSERT(set, "fiobj_hash_set2 didn't copy information to old pointer?");
    FIO_ASSERT(removed,
               "fiobj_hash_remove2 didn't copy information to old pointer?");
    // fiobj_hash_set(o, uintptr_t hash, FIOBJ key, FIOBJ value, FIOBJ *old)
    FIO_ASSERT(
        FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(removed) ==
                strlen("number: 1") &&
            !memcmp(
                "number: 1",
                FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING), ptr)(removed),
                FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(removed)),
        "Object removed from Hash lost it's value %s",
        FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING), ptr)(removed));
    FIO_ASSERT(
        FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(set) ==
                strlen("number: 2") &&
            !memcmp("number: 2",
                    FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING), ptr)(set),
                    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(set)),
        "Object removed from Hash lost it's value %s",
        FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING), ptr)(set));

    fiobj_free(removed);
    fiobj_free(set);
    fiobj_free(o);
  }

#if FIOBJ_MARK_MEMORY
  {
    fprintf(stderr, "* Testing FIOBJ for memory leaks.\n");
    FIOBJ a = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), new)();
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), reserve)(a, 64);
    for (uint8_t bit = 0; bit < (sizeof(intptr_t) * 8); ++bit) {
      uintptr_t i = (uintptr_t)1 << bit;
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)
      (a, FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_NUMBER), new)((intptr_t)i));
    }
    FIOBJ h = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), new)();
    FIOBJ key = FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), new)();
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)(key, "array", 5);
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), set2)(h, key, a);
    FIO_ASSERT(FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_HASH), get2)(h, key) == a,
               "FIOBJ Hash retrival failed");
    FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)(a, key);
    if (0) {
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)
      (a, FIO_NAME(fiobj, FIOBJ___NAME_NULL)());
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)
      (a, FIO_NAME(fiobj, FIOBJ___NAME_TRUE)());
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)
      (a, FIO_NAME(fiobj, FIOBJ___NAME_FALSE)());
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_ARRAY), push)
      (a, FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_FLOAT), new)(0.42));

      FIOBJ json = FIO_NAME2(fiobj, json)(FIOBJ_INVALID, h, 0);
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write)(json, "\n", 1);
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), reserve)
      (json,
       FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(json)
           << 1); /* prevent memory realloc */
      FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), write_escape)
      (json,
       FIO_NAME2(FIO_NAME(fiobj, FIOBJ___NAME_STRING), ptr)(json),
       FIO_NAME(FIO_NAME(fiobj, FIOBJ___NAME_STRING), len)(json) - 1);
      fprintf(stderr, "%s\n", FIO_NAME2(fiobj, cstr)(json).buf);
      fiobj_free(json);
    }
    fiobj_free(h);

    FIO_ASSERT(FIOBJ_MARK_MEMORY_ALLOC_COUNTER ==
                   FIOBJ_MARK_MEMORY_FREE_COUNTER,
               "FIOBJ leak detected (freed %zu/%zu)",
               FIOBJ_MARK_MEMORY_FREE_COUNTER,
               FIOBJ_MARK_MEMORY_ALLOC_COUNTER);
  }
#endif
  fprintf(stderr, "* Passed.\n");
}
#endif /* FIO_TEST_CSTL */
/* *****************************************************************************
FIOBJ cleanup
***************************************************************************** */

#endif /* FIOBJ_EXTERN_COMPLETE */
#undef FIOBJ_FUNC
#undef FIOBJ_IFUNC
#undef FIOBJ_EXTERN
#undef FIOBJ_EXTERN_COMPLETE
#undef FIOBJ_EXTERN_OBJ
#undef FIOBJ_EXTERN_OBJ_IMP
#endif /* FIO_FIOBJ */
#undef FIO_FIOBJ
/* *****************************************************************************
Copyright: Boaz Segev, 2019-2020
License: ISC / MIT (choose your license)

Feel free to copy, use and enjoy according to the license provided.
***************************************************************************** */
#ifndef H___FIO_CSTL_INCLUDE_ONCE_H /* Development inclusion - ignore line */
#include "000 header.h"             /* Development inclusion - ignore line */
#endif                              /* Development inclusion - ignore line */
/* *****************************************************************************














                                Testing














***************************************************************************** */

#if !defined(FIO_FIO_TEST_CSTL_ONLY_ONCE) && (defined(FIO_TEST_CSTL))
#define FIO_FIO_TEST_CSTL_ONLY_ONCE 1

#ifdef FIO_EXTERN_TEST
void fio_test_dynamic_types(void);
#else
FIO_SFUNC void fio_test_dynamic_types(void);
#endif
#if !defined(FIO_EXTERN_TEST) || defined(FIO_EXTERN_COMPLETE)

/* Common testing values / Macros */
#define TEST_FUNC static __attribute__((unused))
#define TEST_REPEAT 4096

/* Make sure logging and FIOBJ memory marking are set. */
#if !defined(FIO_LOG) || defined(FIO_LOG2STDERR2)
#define FIO_LOG
#endif
#ifndef FIOBJ_MARK_MEMORY
#define FIOBJ_MARK_MEMORY 1
#endif
#ifndef FIO_FIOBJ
#define FIO_FIOBJ
#endif

/* Add non-type options to minimize `#include` instructions */
#define FIO_ATOL
#define FIO_BITWISE
#define FIO_BITMAP
#define FIO_RAND
#define FIO_ATOMIC
#define FIO_RISKY_HASH
#include __FILE__

TEST_FUNC uintptr_t fio___dynamic_types_test_tag(uintptr_t i) { return i | 1; }
TEST_FUNC uintptr_t fio___dynamic_types_test_untag(uintptr_t i) {
  return i & (~((uintptr_t)1UL));
}

/* *****************************************************************************
URL parsing - Test
***************************************************************************** */

#define FIO_URL
#include __FILE__

/* *****************************************************************************
Linked List - Test
***************************************************************************** */

typedef struct {
  int data;
  FIO_LIST_NODE node;
} ls____test_s;

#define FIO_LIST_NAME ls____test
#define FIO_PTR_TAG(p) fio___dynamic_types_test_tag(((uintptr_t)p))
#define FIO_PTR_UNTAG(p) fio___dynamic_types_test_untag(((uintptr_t)p))

#include __FILE__

TEST_FUNC void fio___dynamic_types_test___linked_list_test(void) {
  fprintf(stderr, "* Testing linked lists.\n");
  FIO_LIST_HEAD ls = FIO_LIST_INIT(ls);
  for (int i = 0; i < TEST_REPEAT; ++i) {
    ls____test_s *node =
        ls____test_push(&ls, (ls____test_s *)FIO_MEM_CALLOC(sizeof(*node), 1));
    node->data = i;
  }
  int tester = 0;
  FIO_LIST_EACH(ls____test_s, node, &ls, pos) {
    FIO_ASSERT(pos->data == tester++,
               "Linked list ordering error for push or each");
    FIO_ASSERT(ls____test_root(&pos->node) == pos,
               "Linked List root offset error");
  }
  FIO_ASSERT(tester == TEST_REPEAT,
             "linked list EACH didn't loop through all the list");
  while (ls____test_any(&ls)) {
    ls____test_s *node = ls____test_pop(&ls);
    node = (ls____test_s *)fio___dynamic_types_test_untag((uintptr_t)(node));
    FIO_ASSERT(node, "Linked list pop or any failed");
    FIO_ASSERT(node->data == --tester, "Linked list ordering error for pop");
    FIO_MEM_FREE(node, sizeof(*node));
  }
  tester = TEST_REPEAT;
  for (int i = 0; i < TEST_REPEAT; ++i) {
    ls____test_s *node = ls____test_unshift(
        &ls, (ls____test_s *)FIO_MEM_CALLOC(sizeof(*node), 1));
    node->data = i;
  }
  FIO_LIST_EACH(ls____test_s, node, &ls, pos) {
    FIO_ASSERT(pos->data == --tester,
               "Linked list ordering error for unshift or each");
  }
  FIO_ASSERT(tester == 0,
             "linked list EACH didn't loop through all the list after unshift");
  tester = TEST_REPEAT;
  while (ls____test_any(&ls)) {
    ls____test_s *node = ls____test_shift(&ls);
    node = (ls____test_s *)fio___dynamic_types_test_untag((uintptr_t)(node));
    FIO_ASSERT(node, "Linked list pop or any failed");
    FIO_ASSERT(node->data == --tester, "Linked list ordering error for shift");
    FIO_MEM_FREE(node, sizeof(*node));
  }
  FIO_ASSERT(FIO_NAME_BL(ls____test, empty)(&ls),
             "Linked list empty should have been true");
  for (int i = 0; i < TEST_REPEAT; ++i) {
    ls____test_s *node =
        ls____test_push(&ls, (ls____test_s *)FIO_MEM_CALLOC(sizeof(*node), 1));
    node->data = i;
  }
  FIO_LIST_EACH(ls____test_s, node, &ls, pos) {
    ls____test_remove(pos);
    pos = (ls____test_s *)fio___dynamic_types_test_untag((uintptr_t)(pos));
    FIO_MEM_FREE(pos, sizeof(*pos));
  }
  FIO_ASSERT(FIO_NAME_BL(ls____test, empty)(&ls),
             "Linked list empty should have been true");
}

/* *****************************************************************************
Dynamic Array - Test
***************************************************************************** */

static int ary____test_was_destroyed = 0;
#define FIO_ARRAY_NAME ary____test
#define FIO_ARRAY_TYPE int
#define FIO_REF_NAME ary____test
#define FIO_REF_INIT(obj) obj = (ary____test_s)FIO_ARRAY_INIT
#define FIO_REF_DESTROY(obj)                                                   \
  do {                                                                         \
    ary____test_destroy(&obj);                                                 \
    ary____test_was_destroyed = 1;                                             \
  } while (0)
#define FIO_ATOMIC
#define FIO_PTR_TAG(p) fio___dynamic_types_test_tag(((uintptr_t)p))
#define FIO_PTR_UNTAG(p) fio___dynamic_types_test_untag(((uintptr_t)p))
#include __FILE__

#define FIO_ARRAY_NAME ary2____test
#define FIO_ARRAY_TYPE uint8_t
#define FIO_ARRAY_TYPE_INVALID 0xFF
#define FIO_ARRAY_TYPE_COPY(dest, src) (dest) = (src)
#define FIO_ARRAY_TYPE_DESTROY(obj) (obj = FIO_ARRAY_TYPE_INVALID)
#define FIO_ARRAY_TYPE_CMP(a, b) (a) == (b)
#define FIO_PTR_TAG(p) fio___dynamic_types_test_tag(((uintptr_t)p))
#define FIO_PTR_UNTAG(p) fio___dynamic_types_test_untag(((uintptr_t)p))
#include __FILE__

static int fio_____dynamic_test_array_task(int o, void *c_) {
  ((size_t *)(c_))[0] += o;
  if (((size_t *)(c_))[0] >= 256)
    return -1;
  return 0;
}

TEST_FUNC void fio___dynamic_types_test___array_test(void) {
  int tmp = 0;
  ary____test_s a = FIO_ARRAY_INIT;
  fprintf(stderr, "* Testing dynamic arrays.\n");

  fprintf(stderr, "* Testing on stack, push/pop.\n");
  /* test stack allocated array (initialization) */
  FIO_ASSERT(ary____test_capa(&a) == 0,
             "Freshly initialized array should have zero capacity");
  FIO_ASSERT(ary____test_count(&a) == 0,
             "Freshly initialized array should have zero elements");
  memset(&a, 1, sizeof(a));
  a = (ary____test_s)FIO_ARRAY_INIT;
  FIO_ASSERT(ary____test_capa(&a) == 0,
             "Reinitialized array should have zero capacity");
  FIO_ASSERT(ary____test_count(&a) == 0,
             "Reinitialized array should have zero elements");
  ary____test_push(&a, 1);
  ary____test_push(&a, 2);
  /* test get/set array functions */
  FIO_ASSERT(ary____test_get(&a, 1) == 2,
             "`get` by index failed to return correct element.");
  FIO_ASSERT(ary____test_get(&a, -1) == 2,
             "last element `get` failed to return correct element.");
  FIO_ASSERT(ary____test_get(&a, 0) == 1,
             "`get` by index 0 failed to return correct element.");
  FIO_ASSERT(ary____test_get(&a, -2) == 1,
             "last element `get(-2)` failed to return correct element.");
  ary____test_pop(&a, &tmp);
  FIO_ASSERT(tmp == 2, "pop failed to set correct element.");
  ary____test_pop(&a, &tmp);
  /* array is now empty */
  ary____test_push(&a, 1);
  ary____test_push(&a, 2);
  ary____test_push(&a, 3);
  ary____test_set(&a, 99, 1, NULL);
  FIO_ASSERT(ary____test_count(&a) == 100,
             "set with 100 elements should force create elements.");
  FIO_ASSERT(ary____test_get(&a, 0) == 1,
             "Intialized element should be kept (index 0)");
  FIO_ASSERT(ary____test_get(&a, 1) == 2,
             "Intialized element should be kept (index 1)");
  FIO_ASSERT(ary____test_get(&a, 2) == 3,
             "Intialized element should be kept (index 2)");
  for (int i = 3; i < 99; ++i) {
    FIO_ASSERT(ary____test_get(&a, i) == 0, "Unintialized element should be 0");
  }
  ary____test_remove2(&a, 0);
  FIO_ASSERT(ary____test_count(&a) == 4,
             "remove2 should have removed all zero elements.");
  FIO_ASSERT(ary____test_get(&a, 0) == 1,
             "remove2 should have compacted the array (index 0)");
  FIO_ASSERT(ary____test_get(&a, 1) == 2,
             "remove2 should have compacted the array (index 1)");
  FIO_ASSERT(ary____test_get(&a, 2) == 3,
             "remove2 should have compacted the array (index 2)");
  FIO_ASSERT(ary____test_get(&a, 3) == 1,
             "remove2 should have compacted the array (index 4)");
  tmp = 9;
  ary____test_remove(&a, 0, &tmp);
  FIO_ASSERT(tmp == 1, "remove should have copied the value to the pointer.");
  FIO_ASSERT(ary____test_count(&a) == 3,
             "remove should have removed an element.");
  FIO_ASSERT(ary____test_get(&a, 0) == 2,
             "remove should have compacted the array.");
  /* test stack allocated array (destroy) */
  ary____test_destroy(&a);
  FIO_ASSERT(ary____test_capa(&a) == 0,
             "Destroyed array should have zero capacity");
  FIO_ASSERT(ary____test_count(&a) == 0,
             "Destroyed array should have zero elements");
  FIO_ASSERT(a.ary == NULL, "Destroyed array shouldn't have memory allocated");
  ary____test_push(&a, 1);
  ary____test_push(&a, 2);
  ary____test_push(&a, 3);
  ary____test_reserve(&a, 100);
  FIO_ASSERT(ary____test_count(&a) == 3,
             "reserve shouldn't effect itme count.");
  FIO_ASSERT(ary____test_capa(&a) >= 100, "reserve should reserve.");
  FIO_ASSERT(ary____test_get(&a, 0) == 1,
             "Element should be kept after reserve (index 0)");
  FIO_ASSERT(ary____test_get(&a, 1) == 2,
             "Element should be kept after reserve (index 1)");
  FIO_ASSERT(ary____test_get(&a, 2) == 3,
             "Element should be kept after reserve (index 2)");
  ary____test_compact(&a);
  FIO_ASSERT(ary____test_capa(&a) == 3, "reserve shouldn't effect itme count.");
  ary____test_destroy(&a);

  /* Round 2 - heap, shift/unshift, negative ary_set index */

  fprintf(stderr, "* Testing on heap, shift/unshift.\n");
  /* test heap allocated array (initialization) */
  ary____test_s *pa = ary____test_new();
  FIO_ASSERT(ary____test_capa(pa) == 0,
             "Freshly initialized array should have zero capacity");
  FIO_ASSERT(ary____test_count(pa) == 0,
             "Freshly initialized array should have zero elements");
  ary____test_unshift(pa, 2);
  ary____test_unshift(pa, 1);
  /* test get/set/shift/unshift array functions */
  FIO_ASSERT(ary____test_get(pa, 1) == 2,
             "`get` by index failed to return correct element.");
  FIO_ASSERT(ary____test_get(pa, -1) == 2,
             "last element `get` failed to return correct element.");
  FIO_ASSERT(ary____test_get(pa, 0) == 1,
             "`get` by index 0 failed to return correct element.");
  FIO_ASSERT(ary____test_get(pa, -2) == 1,
             "last element `get(-2)` failed to return correct element.");
  ary____test_shift(pa, &tmp);
  FIO_ASSERT(tmp == 1, "shift failed to set correct element.");
  ary____test_shift(pa, &tmp);
  FIO_ASSERT(tmp == 2, "shift failed to set correct element.");
  /* array now empty */
  ary____test_unshift(pa, 1);
  ary____test_unshift(pa, 2);
  ary____test_unshift(pa, 3);
  ary____test_set(pa, -100, 1, NULL);
  FIO_ASSERT(ary____test_count(pa) == 100,
             "set with 100 elements should force create elements.");
  // FIO_ARRAY_EACH(pa, pos) {
  //   fprintf(stderr, "[%zu]  %d\n", (size_t)(pos -
  //   FIO_NAME2(ary____test,ptr)(pa)), *pos);
  // }
  FIO_ASSERT(ary____test_get(pa, 99) == 1,
             "Intialized element should be kept (index 99)");
  FIO_ASSERT(ary____test_get(pa, 98) == 2,
             "Intialized element should be kept (index 98)");
  FIO_ASSERT(ary____test_get(pa, 97) == 3,
             "Intialized element should be kept (index 97)");
  for (int i = 1; i < 97; ++i) {
    FIO_ASSERT(ary____test_get(pa, i) == 0, "Unintialized element should be 0");
  }
  ary____test_remove2(pa, 0);
  FIO_ASSERT(ary____test_count(pa) == 4,
             "remove2 should have removed all zero elements.");
  FIO_ASSERT(ary____test_get(pa, 0) == 1, "remove2 should have kept index 0");
  FIO_ASSERT(ary____test_get(pa, 1) == 3, "remove2 should have kept index 1");
  FIO_ASSERT(ary____test_get(pa, 2) == 2, "remove2 should have kept index 2");
  FIO_ASSERT(ary____test_get(pa, 3) == 1, "remove2 should have kept index 3");
  tmp = 9;
  ary____test_remove(pa, 0, &tmp);
  FIO_ASSERT(tmp == 1, "remove should have copied the value to the pointer.");
  FIO_ASSERT(ary____test_count(pa) == 3,
             "remove should have removed an element.");
  FIO_ASSERT(ary____test_get(pa, 0) == 3,
             "remove should have compacted the array.");
  /* test heap allocated array (destroy) */
  ary____test_destroy(pa);
  FIO_ASSERT(ary____test_capa(pa) == 0,
             "Destroyed array should have zero capacity");
  FIO_ASSERT(ary____test_count(pa) == 0,
             "Destroyed array should have zero elements");
  FIO_ASSERT(FIO_NAME2(ary____test, ptr)(pa) == NULL,
             "Destroyed array shouldn't have memory allocated");
  ary____test_unshift(pa, 1);
  ary____test_unshift(pa, 2);
  ary____test_unshift(pa, 3);
  ary____test_reserve(pa, -100);
  FIO_ASSERT(ary____test_count(pa) == 3,
             "reserve shouldn't change item count.");
  FIO_ASSERT(ary____test_capa(pa) >= 100, "reserve should reserve.");
  FIO_ASSERT(ary____test_get(pa, 0) == 3, "reserve should have kept index 0");
  FIO_ASSERT(ary____test_get(pa, 1) == 2, "reserve should have kept index 1");
  FIO_ASSERT(ary____test_get(pa, 2) == 1, "reserve should have kept index 2");
  ary____test_destroy(pa);
  ary____test_free(pa);

  fprintf(stderr, "* Testing non-zero value for uninitialized elements.\n");
  ary2____test_s a2 = FIO_ARRAY_INIT;
  ary2____test_set(&a2, 99, 1, NULL);
  FIO_ARRAY_EACH(&a2, pos) {
    FIO_ASSERT(
        (*pos == 0xFF || (pos - FIO_NAME2(ary2____test, ptr)(&a2)) == 99),
        "uninitialized elements should be initialized as "
        "FIO_ARRAY_TYPE_INVALID");
  }
  ary2____test_set(&a2, -200, 1, NULL);
  FIO_ASSERT(ary2____test_count(&a2) == 200, "array should have 100 items.");
  FIO_ARRAY_EACH(&a2, pos) {
    FIO_ASSERT((*pos == 0xFF ||
                (pos - FIO_NAME2(ary2____test, ptr)(&a2)) == 0 ||
                (pos - FIO_NAME2(ary2____test, ptr)(&a2)) == 199),
               "uninitialized elements should be initialized as "
               "FIO_ARRAY_TYPE_INVALID (index %zd)",
               (pos - FIO_NAME2(ary2____test, ptr)(&a2)));
  }
  ary2____test_destroy(&a2);

  /* Round 3 - heap, with reference counting */
  fprintf(stderr, "* Testing reference counting.\n");
  /* test heap allocated array (initialization) */
  pa = ary____test_new2();
  ary____test_up_ref(pa);
  ary____test_unshift(pa, 2);
  ary____test_unshift(pa, 1);
  ary____test_free2(pa);
  FIO_ASSERT(!ary____test_was_destroyed,
             "reference counted array destroyed too early.");
  FIO_ASSERT(ary____test_get(pa, 1) == 2,
             "`get` by index failed to return correct element.");
  FIO_ASSERT(ary____test_get(pa, -1) == 2,
             "last element `get` failed to return correct element.");
  FIO_ASSERT(ary____test_get(pa, 0) == 1,
             "`get` by index 0 failed to return correct element.");
  FIO_ASSERT(ary____test_get(pa, -2) == 1,
             "last element `get(-2)` failed to return correct element.");
  ary____test_free2(pa);
  FIO_ASSERT(ary____test_was_destroyed,
             "reference counted array not destroyed.");

  fprintf(stderr, "* Testing dynamic arrays helpers.\n");
  for (size_t i = 0; i < TEST_REPEAT; ++i) {
    ary____test_push(&a, i);
  }
  FIO_ASSERT(ary____test_count(&a) == TEST_REPEAT, "push object count error");
  {
    size_t c = 0;
    size_t i = ary____test_each(&a, 3, fio_____dynamic_test_array_task, &c);
    FIO_ASSERT(i < 64, "too many objects counted in each loop.");
    FIO_ASSERT(c >= 256 && c < 512, "each loop too long.");
  }
  for (size_t i = 0; i < TEST_REPEAT; ++i) {
    FIO_ASSERT((size_t)ary____test_get(&a, i) == i,
               "push order / insert issue");
  }
  ary____test_destroy(&a);
  for (size_t i = 0; i < TEST_REPEAT; ++i) {
    ary____test_unshift(&a, i);
  }
  FIO_ASSERT(ary____test_count(&a) == TEST_REPEAT,
             "unshift object count error");
  for (size_t i = 0; i < TEST_REPEAT; ++i) {
    int old = 0;
    ary____test_pop(&a, &old);
    FIO_ASSERT((size_t)old == i, "shift order / insert issue");
  }
  ary____test_destroy(&a);
}

/* *****************************************************************************
Hash Map / Set - test
***************************************************************************** */

/* a simple set of numbers */
#define FIO_MAP_NAME set_____test
#define FIO_MAP_TYPE size_t
#define FIO_MAP_TYPE_CMP(a, b) ((a) == (b))
#define FIO_PTR_TAG(p) fio___dynamic_types_test_tag(((uintptr_t)p))
#define FIO_PTR_UNTAG(p) fio___dynamic_types_test_untag(((uintptr_t)p))
#include __FILE__

/* a simple set of numbers */
#define FIO_MAP_NAME set2_____test
#define FIO_MAP_TYPE size_t
#define FIO_MAP_TYPE_CMP(a, b) 1
#define FIO_PTR_TAG(p) fio___dynamic_types_test_tag(((uintptr_t)p))
#define FIO_PTR_UNTAG(p) fio___dynamic_types_test_untag(((uintptr_t)p))
#include __FILE__

TEST_FUNC size_t map_____test_key_copy_counter = 0;
TEST_FUNC void map_____test_key_copy(char **dest, char *src) {
  *dest = (char *)FIO_MEM_CALLOC(strlen(src) + 1, sizeof(*dest));
  FIO_ASSERT(*dest, "not memory to allocate key in map_test")
  strcpy(*dest, src);
  ++map_____test_key_copy_counter;
}
TEST_FUNC void map_____test_key_destroy(char **dest) {
  FIO_MEM_FREE(*dest, strlen(*dest) + 1);
  *dest = NULL;
  --map_____test_key_copy_counter;
}

/* keys are strings, values are numbers */
#define FIO_MAP_KEY char *
#define FIO_MAP_KEY_CMP(a, b) (strcmp((a), (b)) == 0)
#define FIO_MAP_KEY_COPY(a, b) map_____test_key_copy(&(a), (b))
#define FIO_MAP_KEY_DESTROY(a) map_____test_key_destroy(&(a))
#define FIO_MAP_TYPE size_t
#define FIO_MAP_NAME map_____test
#include __FILE__

#define HASHOFi(i) i /* fio_risky_hash(&(i), sizeof((i)), 0) */
#define HASHOFs(s) fio_risky_hash(s, strlen((s)), 0)

TEST_FUNC int set_____test_each_task(size_t o, void *a_) {
  uintptr_t *i_p = (uintptr_t *)a_;
  FIO_ASSERT(o == ++(*i_p), "set_each started at a bad offset!");
  FIO_ASSERT(HASHOFi((o - 1)) == set_____test_each_get_key(),
             "set_each key error!");
  return 0;
}

TEST_FUNC void fio___dynamic_types_test___map_test(void) {
  {
    set_____test_s m = FIO_MAP_INIT;
    fprintf(stderr, "* Testing dynamic hash / set maps.\n");

    fprintf(stderr, "* Testing set (hash map where value == key).\n");
    FIO_ASSERT(set_____test_count(&m) == 0,
               "freshly initialized map should have no objects");
    FIO_ASSERT(set_____test_capa(&m) == 0,
               "freshly initialized map should have no capacity");
    FIO_ASSERT(set_____test_reserve(&m, (TEST_REPEAT >> 1)) >=
                   (TEST_REPEAT >> 1),
               "reserve should increase capacity.");
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      set_____test_set_if_missing(&m, HASHOFi(i), i + 1);
    }
    {
      uintptr_t pos_test = (TEST_REPEAT >> 1);
      size_t count =
          set_____test_each(&m, pos_test, set_____test_each_task, &pos_test);
      FIO_ASSERT(count == set_____test_count(&m),
                 "set_each tast returned the wrong counter.");
      FIO_ASSERT(count == pos_test, "set_each position testing error");
    }

    FIO_ASSERT(set_____test_count(&m) == TEST_REPEAT,
               "After inserting %zu items to set, got %zu items",
               (size_t)TEST_REPEAT,
               (size_t)set_____test_count(&m));
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      FIO_ASSERT(set_____test_get(&m, HASHOFi(i), i + 1) == i + 1,
                 "item retrival error in set.");
    }
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      FIO_ASSERT(set_____test_get(&m, HASHOFi(i), i + 2) == 0,
                 "item retrival error in set - object comparisson error?");
    }

    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      set_____test_set_if_missing(&m, HASHOFi(i), i + 1);
    }
    {
      size_t i = 0;
      FIO_MAP_EACH2(set_____test, &m, pos) {
        FIO_ASSERT(pos->obj == pos->hash + 1 || !(~pos->hash),
                   "FIO_MAP_EACH loop out of order?")
        ++i;
      }
      FIO_ASSERT(i == set_____test_count(&m), "FIO_MAP_EACH loop incomplete?")
    }
    FIO_ASSERT(set_____test_count(&m) == TEST_REPEAT,
               "Inserting existing object should keep existing object.");
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      FIO_ASSERT(set_____test_get(&m, HASHOFi(i), i + 1) == i + 1,
                 "item retrival error in set - insert failed to update?");
      FIO_ASSERT(set_____test_get_ptr(&m, HASHOFi(i), i + 1) &&
                     set_____test_get_ptr(&m, HASHOFi(i), i + 1)[0] == i + 1,
                 "pointer retrival error in set.");
    }

    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      size_t old = 5;
      set_____test_set(&m, HASHOFi(i), i + 2, &old);
      FIO_ASSERT(old == 0,
                 "old pointer not initialized with old (or missing) data");
    }

    FIO_ASSERT(set_____test_count(&m) == (TEST_REPEAT * 2),
               "full hash collision shoudn't break map until attack limit.");
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      FIO_ASSERT(set_____test_get(&m, HASHOFi(i), i + 2) == i + 2,
                 "item retrival error in set - overwrite failed to update?");
    }
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      FIO_ASSERT(set_____test_get(&m, HASHOFi(i), i + 1) == i + 1,
                 "item retrival error in set - collision resolution error?");
    }

    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      size_t old = 5;
      set_____test_remove(&m, HASHOFi(i), i + 1, &old);
      FIO_ASSERT(old == i + 1,
                 "removed item not initialized with old (or missing) data");
    }
    FIO_ASSERT(set_____test_count(&m) == TEST_REPEAT,
               "removal should update object count.");
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      FIO_ASSERT(set_____test_get(&m, HASHOFi(i), i + 1) == 0,
                 "removed items should be unavailable");
    }
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      FIO_ASSERT(set_____test_get(&m, HASHOFi(i), i + 2) == i + 2,
                 "previous items should be accessible after removal");
    }
    set_____test_destroy(&m);
  }
  {
    set2_____test_s m = FIO_MAP_INIT;
    fprintf(stderr, "* Testing set map without value comparison.\n");
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      set2_____test_set_if_missing(&m, HASHOFi(i), i + 1);
    }

    FIO_ASSERT(set2_____test_count(&m) == TEST_REPEAT,
               "After inserting %zu items to set, got %zu items",
               (size_t)TEST_REPEAT,
               (size_t)set2_____test_count(&m));
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      FIO_ASSERT(set2_____test_get(&m, HASHOFi(i), 0) == i + 1,
                 "item retrival error in set.");
    }

    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      set2_____test_set_if_missing(&m, HASHOFi(i), i + 2);
    }
    FIO_ASSERT(set2_____test_count(&m) == TEST_REPEAT,
               "Inserting existing object should keep existing object.");
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      FIO_ASSERT(set2_____test_get(&m, HASHOFi(i), 0) == i + 1,
                 "item retrival error in set - insert failed to update?");
    }

    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      size_t old = 5;
      set2_____test_set(&m, HASHOFi(i), i + 2, &old);
      FIO_ASSERT(old == i + 1,
                 "old pointer not initialized with old (or missing) data");
    }

    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      FIO_ASSERT(set2_____test_get(&m, HASHOFi(i), 0) == i + 2,
                 "item retrival error in set - overwrite failed to update?");
    }
    {
      /* test partial removal */
      for (size_t i = 1; i < TEST_REPEAT; i += 2) {
        size_t old = 5;
        set2_____test_remove(&m, HASHOFi(i), 0, &old);
        FIO_ASSERT(old == i + 2,
                   "removed item not initialized with old (or missing) data "
                   "(%zu != %zu)",
                   old,
                   i + 2);
      }
      for (size_t i = 1; i < TEST_REPEAT; i += 2) {
        FIO_ASSERT(set2_____test_get(&m, HASHOFi(i), 0) == 0,
                   "previous items should NOT be accessible after removal");
        set2_____test_set_if_missing(&m, HASHOFi(i), i + 2);
      }
    }
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      size_t old = 5;
      set2_____test_remove(&m, HASHOFi(i), 0, &old);
      FIO_ASSERT(old == i + 2,
                 "removed item not initialized with old (or missing) data "
                 "(%zu != %zu)",
                 old,
                 i + 2);
    }
    FIO_ASSERT(set2_____test_count(&m) == 0,
               "removal should update object count.");
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      FIO_ASSERT(set2_____test_get(&m, HASHOFi(i), 0) == 0,
                 "previous items should NOT be accessible after removal");
    }
    set2_____test_destroy(&m);
  }

  {
    map_____test_s *m = map_____test_new();
    fprintf(stderr, "* Testing hash map.\n");
    FIO_ASSERT(map_____test_count(m) == 0,
               "freshly initialized map should have no objects");
    FIO_ASSERT(map_____test_capa(m) == 0,
               "freshly initialized map should have no capacity");
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      char buffer[64];
      int l = snprintf(buffer, 63, "%zu", i);
      buffer[l] = 0;
      map_____test_set(m, HASHOFs(buffer), buffer, i + 1, NULL);
    }
    FIO_ASSERT(map_____test_key_copy_counter == TEST_REPEAT,
               "key copying error - was the key copied?");
    FIO_ASSERT(map_____test_count(m) == TEST_REPEAT,
               "After inserting %zu items to map, got %zu items",
               (size_t)TEST_REPEAT,
               (size_t)map_____test_count(m));
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      char buffer[64];
      int l = snprintf(buffer + 1, 61, "%zu", i);
      buffer[l + 1] = 0;
      FIO_ASSERT(map_____test_get(m, HASHOFs(buffer + 1), buffer + 1) == i + 1,
                 "item retrival error in map.");
      FIO_ASSERT(map_____test_get_ptr(m, HASHOFs(buffer + 1), buffer + 1) &&
                     map_____test_get_ptr(
                         m, HASHOFs(buffer + 1), buffer + 1)[0] == i + 1,
                 "pointer retrival error in map.");
    }
    map_____test_free(m);
    FIO_ASSERT(map_____test_key_copy_counter == 0,
               "key destruction error - was the key freed?");
  }
  {
    set_____test_s s = FIO_MAP_INIT;
    map_____test_s m = FIO_MAP_INIT;
    fprintf(stderr, "* Testing attack resistance (SHOULD print warnings).\n");
    for (size_t i = 0; i < TEST_REPEAT; ++i) {
      char buf[64];
      fio_ltoa(buf, i, 16);
      set_____test_set(&s, 1, i + 1, NULL);
      map_____test_set(&m, 1, buf, i + 1, NULL);
    }
    FIO_ASSERT(set_____test_count(&s) != TEST_REPEAT,
               "full collision protection failed (set)?");
    FIO_ASSERT(map_____test_count(&m) != TEST_REPEAT,
               "full collision protection failed (map)?");
    FIO_ASSERT(set_____test_count(&s) != 1,
               "full collision test failed to push elements (set)?");
    FIO_ASSERT(map_____test_count(&m) != 1,
               "full collision test failed to push elements (map)?");
    set_____test_destroy(&s);
    map_____test_destroy(&m);
  }
}

#undef HASHOFi
#undef HASHOFs

/* *****************************************************************************
Dynamic Strings - test
***************************************************************************** */

#define FIO_STR_NAME fio_big_str
#define FIO_STR_WRITE_TEST_FUNC
#include __FILE__

#define FIO_STR_SMALL fio_small_str
#define FIO_STR_WRITE_TEST_FUNC
#include __FILE__

/**
 * Tests the fio_str functionality.
 */
TEST_FUNC void fio___dynamic_types_test___str(void) {
  fio_big_str___dynamic_test();
  fio_small_str___dynamic_test();
}

/* *****************************************************************************
Time - test
***************************************************************************** */
#define FIO_TIME
#include __FILE__
/* *****************************************************************************
Queue - test
***************************************************************************** */
#define FIO_QUEUE
#include __FILE__
/* *****************************************************************************
CLI - test
***************************************************************************** */
#define FIO_CLI
#include __FILE__
/* *****************************************************************************
Memory Allocation - test
***************************************************************************** */
#define FIO_MALLOC
#include __FILE__
/* *****************************************************************************
Socket helper testing
***************************************************************************** */
#define FIO_SOCK
#include __FILE__

/* *****************************************************************************
FIOBJ and JSON testing
***************************************************************************** */
#define FIO_FIOBJ
#include __FILE__

/* *****************************************************************************
Environment printout
***************************************************************************** */

#define FIO_PRINT_SIZE_OF(T) fprintf(stderr, "\t" #T "\t%zu Bytes\n", sizeof(T))

TEST_FUNC void FIO_NAME_TEST(stl, type_sizes)(void) {
  switch (sizeof(void *)) {
  case 2:
    fprintf(stderr, "* 16bit words size (unexpected, unknown effects).\n");
    break;
  case 4:
    fprintf(stderr, "* 32bit words size (some features might be slower).\n");
    break;
  case 8:
    fprintf(stderr, "* 64bit words size okay.\n");
    break;
  case 16:
    fprintf(stderr, "* 128bit words size... wow!\n");
    break;
  default:
    fprintf(stderr, "* Unknown words size %zubit!\n", sizeof(void *) << 3);
    break;
  }
  fprintf(stderr, "* Using the following type sizes:\n");
  FIO_PRINT_SIZE_OF(char);
  FIO_PRINT_SIZE_OF(short);
  FIO_PRINT_SIZE_OF(int);
  FIO_PRINT_SIZE_OF(float);
  FIO_PRINT_SIZE_OF(long);
  FIO_PRINT_SIZE_OF(double);
  FIO_PRINT_SIZE_OF(size_t);
  FIO_PRINT_SIZE_OF(void *);
}
#undef FIO_PRINT_SIZE_OF

/* *****************************************************************************
Locking - Speed Test
***************************************************************************** */
#define FIO___LOCK2_TEST_TASK (1LU << 25)
#define FIO___LOCK2_TEST_THREADS 32U
#define FIO___LOCK2_TEST_REPEAT 1

#ifndef H___FIO_LOCK2___H
#include <pthread.h>
#endif

FIO_SFUNC void fio___lock_speedtest_task_inner(void *s) {
  size_t *r = (size_t *)s;
  static size_t i;
  for (i = 0; i < FIO___LOCK2_TEST_TASK; ++i) {
    __asm__ volatile("" ::: "memory"); /* clobber CPU registers */
    ++r[0];
  }
}

static void *fio___lock_mytask_lock(void *s) {
  static fio_lock_i lock = FIO_LOCK_INIT;
  fio_lock(&lock);
  if (s)
    fio___lock_speedtest_task_inner(s);
  fio_unlock(&lock);
  return NULL;
}

#ifdef H___FIO_LOCK2___H
static void *fio___lock_mytask_lock2(void *s) {
  static fio_lock2_s lock = {FIO_LOCK_INIT};
  fio_lock2(&lock, 1);
  if (s)
    fio___lock_speedtest_task_inner(s);
  fio_unlock2(&lock, 1);
  return NULL;
}
#endif

static void *fio___lock_mytask_mutex(void *s) {
  static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_lock(&mutex);
  if (s)
    fio___lock_speedtest_task_inner(s);
  pthread_mutex_unlock(&mutex);
  return NULL;
}

FIO_SFUNC void FIO_NAME_TEST(stl, lock_speed)(void) {
  uint64_t start, end;
  pthread_t threads[FIO___LOCK2_TEST_THREADS];

  struct {
    size_t type_size;
    const char *type_name;
    const char *name;
    void *(*task)(void *);
  } test_funcs[] = {
      {
          .type_size = sizeof(fio_lock_i),
          .type_name = "fio_lock_i",
          .name = "fio_lock      (spinlock)",
          .task = fio___lock_mytask_lock,
      },
#ifdef H___FIO_LOCK2___H
      {
          .type_size = sizeof(fio_lock2_s),
          .type_name = "fio_lock2_s",
          .name = "fio_lock2 (pause/resume)",
          .task = fio___lock_mytask_lock2,
      },
#endif
      {
          .type_size = sizeof(pthread_mutex_t),
          .type_name = "pthread_mutex_t",
          .name = "pthreads (pthread_mutex)",
          .task = fio___lock_mytask_mutex,
      },
      {
          .name = NULL,
          .task = NULL,
      },
  };
  fprintf(stderr, "* Speed testing The following types:\n");
  for (size_t fn = 0; test_funcs[fn].name; ++fn) {
    fprintf(stderr,
            "\t%s\t(%zu bytes)\n",
            test_funcs[fn].type_name,
            test_funcs[fn].type_size);
  }
#ifndef H___FIO_LOCK2___H
  FIO_LOG_WARNING("Won't test `fio_lock2` functions (needs `FIO_LOCK2`).");
#endif

  start = fio_time_micro();
  for (size_t i = 0; i < FIO___LOCK2_TEST_TASK; ++i) {
    __asm__ volatile("" ::: "memory"); /* clobber CPU registers */
  }
  end = fio_time_micro();
  fprintf(stderr,
          "\n* Speed testing locking schemes - no contention, short work (%zu "
          "mms):\n"
          "\t\t(%zu itterations)\n",
          (size_t)(end - start),
          (size_t)FIO___LOCK2_TEST_TASK);

  for (int test_repeat = 0; test_repeat < FIO___LOCK2_TEST_REPEAT;
       ++test_repeat) {
    if (FIO___LOCK2_TEST_REPEAT > 1)
      fprintf(
          stderr, "%s (%d)\n", (test_repeat ? "Round" : "Warmup"), test_repeat);
    for (size_t fn = 0; test_funcs[fn].name; ++fn) {
      test_funcs[fn].task(NULL); /* warmup */
      start = fio_time_micro();
      for (size_t i = 0; i < FIO___LOCK2_TEST_TASK; ++i) {
        __asm__ volatile("" ::: "memory"); /* clobber CPU registers */
        test_funcs[fn].task(NULL);
      }
      end = fio_time_micro();
      fprintf(stderr,
              "\t%s: %zu mms\n",
              test_funcs[fn].name,
              (size_t)(end - start));
    }
  }

  fprintf(stderr,
          "\n* Speed testing locking schemes - no contention, long work ");
  start = fio_time_micro();
  for (size_t i = 0; i < FIO___LOCK2_TEST_THREADS; ++i) {
    size_t result = 0;
    __asm__ volatile("" ::: "memory"); /* clobber CPU registers */
    fio___lock_speedtest_task_inner(&result);
  }
  end = fio_time_micro();
  fprintf(stderr, " %zu mms\n", (size_t)(end - start));
  clock_t long_work = end - start;
  fprintf(stderr, "(%zu mms):\n", long_work);
  for (int test_repeat = 0; test_repeat < FIO___LOCK2_TEST_REPEAT;
       ++test_repeat) {
    if (FIO___LOCK2_TEST_REPEAT > 1)
      fprintf(
          stderr, "%s (%d)\n", (test_repeat ? "Round" : "Warmup"), test_repeat);
    for (size_t fn = 0; test_funcs[fn].name; ++fn) {
      size_t result = 0;
      test_funcs[fn].task((void *)&result); /* warmup */
      result = 0;
      start = fio_time_micro();
      for (size_t i = 0; i < FIO___LOCK2_TEST_THREADS; ++i) {
        __asm__ volatile("" ::: "memory"); /* clobber CPU registers */
        test_funcs[fn].task(&result);
      }
      end = fio_time_micro();
      fprintf(stderr,
              "\t%s: %zu mms (%zu mms)\n",
              test_funcs[fn].name,
              (size_t)(end - start),
              (size_t)(end - (start + long_work)));
      FIO_ASSERT(result == (FIO___LOCK2_TEST_TASK * FIO___LOCK2_TEST_THREADS),
                 "%s final result error.",
                 test_funcs[fn].name);
    }
  }

  fprintf(stderr,
          "\n* Speed testing locking schemes - %zu threads, long work (%zu "
          "mms):\n",
          (size_t)FIO___LOCK2_TEST_THREADS,
          long_work);
  for (int test_repeat = 0; test_repeat < FIO___LOCK2_TEST_REPEAT;
       ++test_repeat) {
    if (FIO___LOCK2_TEST_REPEAT > 1)
      fprintf(
          stderr, "%s (%d)\n", (test_repeat ? "Round" : "Warmup"), test_repeat);
    for (size_t fn = 0; test_funcs[fn].name; ++fn) {
      size_t result = 0;
      test_funcs[fn].task((void *)&result); /* warmup */
      result = 0;
      start = fio_time_micro();
      for (size_t i = 0; i < FIO___LOCK2_TEST_THREADS; ++i) {
        pthread_create(threads + i, NULL, test_funcs[fn].task, &result);
      }
      for (size_t i = 0; i < FIO___LOCK2_TEST_THREADS; ++i) {
        pthread_join(threads[i], NULL);
      }
      end = fio_time_micro();
      fprintf(stderr,
              "\t%s: %zu mms (%zu mms)\n",
              test_funcs[fn].name,
              (size_t)(end - start),
              (size_t)(end - (start + long_work)));
      FIO_ASSERT(result == (FIO___LOCK2_TEST_TASK * FIO___LOCK2_TEST_THREADS),
                 "%s final result error.",
                 test_funcs[fn].name);
    }
  }
}

/* *****************************************************************************
Testing functiun
***************************************************************************** */

TEST_FUNC void fio____test_dynamic_types__stack_poisoner(void) {
  const size_t len = 1UL << 16;
  uint8_t buf[1UL << 16];
  __asm__ __volatile__("" ::: "memory");
  memset(buf, (int)(~0U), len);
  __asm__ __volatile__("" ::: "memory");
  fio_trylock(buf);
}

TEST_FUNC void fio_test_dynamic_types(void) {
  char *filename = (char *)FIO__FILE__;
  while (filename[0] == '.' && filename[1] == '/')
    filename += 2;
  fio____test_dynamic_types__stack_poisoner();
  fprintf(stderr, "===============\n");
  fprintf(stderr, "Testing Dynamic Types (%s)\n", filename);
  fprintf(
      stderr,
      "facil.io core: version \x1B[1m" FIO_VERSION_STRING "\x1B[0m\n"
      "The facil.io library was originally coded by \x1B[1mBoaz Segev\x1B[0m.\n"
      "Please give credit where credit is due.\n"
      "\x1B[1mYour support is only fair\x1B[0m - give value for value.\n"
      "(code contributions / donations)\n\n");
  fprintf(stderr, "===============\n");
  FIO_LOG_DEBUG("example FIO_LOG_DEBUG message.");
  FIO_LOG_DEBUG2("example FIO_LOG_DEBUG2 message.");
  FIO_LOG_INFO("example FIO_LOG_INFO message.");
  FIO_LOG_WARNING("example FIO_LOG_WARNING message.");
  FIO_LOG_SECURITY("example FIO_LOG_SECURITY message.");
  FIO_LOG_ERROR("example FIO_LOG_ERROR message.");
  FIO_LOG_FATAL("example FIO_LOG_FATAL message.");
  fprintf(stderr, "===============\n");
  FIO_NAME_TEST(stl, type_sizes)();
  fprintf(stderr, "===============\n");
  FIO_NAME_TEST(stl, random)();
  fprintf(stderr, "===============\n");
  FIO_NAME_TEST(stl, atomics)();
  fprintf(stderr, "===============\n");
  FIO_NAME_TEST(stl, bitwise)();
  fprintf(stderr, "===============\n");
  FIO_NAME_TEST(stl, atol)();
  fprintf(stderr, "===============\n");
  FIO_NAME_TEST(stl, url)();
  fprintf(stderr, "===============\n");
  fio___dynamic_types_test___linked_list_test();
  fprintf(stderr, "===============\n");
  fio___dynamic_types_test___array_test();
  fprintf(stderr, "===============\n");
  fio___dynamic_types_test___map_test();
  fprintf(stderr, "===============\n");
  fio___dynamic_types_test___str();
  fprintf(stderr, "===============\n");
  FIO_NAME_TEST(stl, time)();
  fprintf(stderr, "===============\n");
  FIO_NAME_TEST(stl, queue)();
  fprintf(stderr, "===============\n");
  FIO_NAME_TEST(stl, cli)();
  fprintf(stderr, "===============\n");
  FIO_NAME_TEST(stl, mem)();
  fprintf(stderr, "===============\n");
  FIO_NAME_TEST(stl, sock)();
  fprintf(stderr, "===============\n");
  FIO_NAME_TEST(stl, fiobj)();
  fprintf(stderr, "===============\n");
  FIO_NAME_TEST(stl, risky)();
  fprintf(stderr, "===============\n");
  FIO_NAME_TEST(stl, lock_speed)();
  fprintf(stderr, "===============\n");
  {
    char timebuf[64];
    fio_time2rfc7231(timebuf, fio_time_real().tv_sec);
    fprintf(stderr,
            "On %s\n"
            "Testing \x1B[1mPASSED\x1B[0m "
            "for facil.io core version: "
            "\x1B[1m" FIO_VERSION_STRING "\x1B[0m"
            "\n",
            timebuf);
  }
  fprintf(stderr,
          "\nThe facil.io library was originally coded by \x1B[1mBoaz "
          "Segev\x1B[0m.\n"
          "Please give credit where credit is due.\n"
          "\x1B[1mYour support is only fair\x1B[0m - give value for value.\n"
          "(code contributions / donations)\n\n");
#if !defined(FIO_NO_COOKIE)
  fio___();
#endif
}

/* *****************************************************************************
Testing cleanup
***************************************************************************** */

#undef FIO_TEST_CSTL
#undef TEST_REPEAT
#undef TEST_FUNC
#undef FIO_ASSERT

#endif /* FIO_EXTERN_COMPLETE */
#endif
/* *****************************************************************************




















***************************************************************************** */

/* *****************************************************************************
C++ extern end
***************************************************************************** */
/* support C++ */
#ifdef __cplusplus
}
#endif
