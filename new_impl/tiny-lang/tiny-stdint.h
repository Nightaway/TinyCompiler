#ifndef __TINY_STDINT_H__
#define __TINY_STDINT_H__

#include <stddef.h>
#include <stdio.h>

#if defined(_WIN32) && !defined(__MINGW32__)


typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;  // NOLINT
typedef unsigned short uint16_t;  // NOLINT
typedef int int32_t;
typedef unsigned int uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
// intptr_t and friends are defined in crtdefs.h through stdio.h.

#else


#endif


#endif