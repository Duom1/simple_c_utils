#ifndef DU_PORTABLE_TYPES_H
#define DU_PORTABLE_TYPES_H

#include <limits.h>

/* Define fixed-width integer types */

/* 8-bit types */
#if UCHAR_MAX == 0xFF
typedef unsigned char du_u8;
typedef signed char du_i8;
#else
#error "Cannot determine 8-bit types"
#endif

/* 16-bit types */
#if USHRT_MAX == 0xFFFF
typedef unsigned short du_u16;
typedef signed short du_i16;
#elif UINT_MAX == 0xFFFF
typedef unsigned int du_u16;
typedef signed int du_i16;
#else
#error "Cannot determine 16-bit types"
#endif

/* 32-bit types */
#if UINT_MAX == 0xFFFFFFFF
typedef unsigned int du_u32;
typedef signed int du_i32;
#elif ULONG_MAX == 0xFFFFFFFF
typedef unsigned long du_u32;
typedef signed long du_i32;
#else
#error "Cannot determine 32-bit types"
#endif

/* 64-bit types (Toggle 64-bit Support) */
#ifdef ALLOW_64
#ifdef USE_LONG_LONG
#if defined(ULLONG_MAX) && ULLONG_MAX == 0xFFFFFFFFFFFFFFFF
typedef unsigned long long du_u64;
typedef signed long long du_i64;
#else
#error "Cannot determine 64-bit types with long long"
#endif
#else
#if ULONG_MAX == 0xFFFFFFFFFFFFFFFF
typedef unsigned long du_u64;
typedef signed long du_i64;
#else
#error "Cannot determine 64-bit types without long long"
#endif
#endif
#endif

#endif /* DU_PORTABLE_TYPES_H */
