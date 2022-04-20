#include <limits.h>

#ifndef BTC_TYPES

#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef __int64  int64;
typedef unsigned __int64  uint64;
#else
typedef long long  int64;
typedef unsigned long long  uint64;
#endif

/*
//stdint.h
typedef signed char		int8_t;
typedef unsigned char		uint8_t;
typedef short			int16_t;
typedef unsigned short		uint16_t;
typedef int			int32_t;
typedef unsigned		uint32_t;
typedef long long		int64_t;
typedef unsigned long long	uint64_t;
*/

#define BTC_TYPES
#endif