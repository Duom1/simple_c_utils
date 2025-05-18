#ifndef PORTABLE_TYPES_H
#define PORTABLE_TYPES_H

#include <limits.h>

/* Define fixed-width integer types */

/* 8-bit types */
#if UCHAR_MAX == 0xFF
typedef unsigned char u8;
typedef signed char i8;
#else
#error "Cannot determine 8-bit types"
#endif

/* 16-bit types */
#if USHRT_MAX == 0xFFFF
typedef unsigned short u16;
typedef signed short i16;
#elif UINT_MAX == 0xFFFF
typedef unsigned int u16;
typedef signed int i16;
#else
#error "Cannot determine 16-bit types"
#endif

/* 32-bit types */
#if UINT_MAX == 0xFFFFFFFF
typedef unsigned int u32;
typedef signed int i32;
#elif ULONG_MAX == 0xFFFFFFFF
typedef unsigned long u32;
typedef signed long i32;
#else
#error "Cannot determine 32-bit types"
#endif

/* 64-bit types (Toggle 64-bit Support) */
#ifdef ALLOW_64
#ifdef USE_LONG_LONG
#if defined(ULLONG_MAX) && ULLONG_MAX == 0xFFFFFFFFFFFFFFFF
typedef unsigned long long u64;
typedef signed long long i64;
#else
#error "Cannot determine 64-bit types with long long"
#endif
#else
#if ULONG_MAX == 0xFFFFFFFFFFFFFFFF
typedef unsigned long u64;
typedef signed long i64;
#else
#error "Cannot determine 64-bit types without long long"
#endif
#endif
#endif

#endif /* PORTABLE_TYPES_H */
