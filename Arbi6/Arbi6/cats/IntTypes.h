#ifndef __NATIVE_INT_TYPES_H
#define __NATIVE_INT_TYPES_H

#if (defined(__GNUC__)  || defined(__GCCXML__) || defined(__SNC__))
#include <stdint.h>
typedef uint8_t		uint8;
typedef uint16_t	uint16;
typedef uint32_t	uint32;
typedef int8_t		int8;
typedef int16_t		int16;
typedef int32_t		int32;
typedef int64_t		int64;
typedef uint64_t	uint64;
typedef unsigned int uint;
typedef float		float32;
typedef double		float64;

#  ifndef _atoi64
#  define _atoi64		atoll
#  endif
#endif

#if !defined(_STDINT_H) && !defined(_SN_STDINT_H) && !defined(_SYS_STDINT_H_) && !defined(_STDINT) && !defined(_MACHTYPES_H_) && !defined(_STDINT_H_)
	typedef unsigned char       uint8;
	typedef unsigned short      uint16;
	typedef unsigned __int32    uint32;
	typedef signed char         int8;
	typedef signed short        int16;
	typedef __int32				int32;
	
	#if defined(_MSC_VER) && _MSC_VER < 1300
		typedef unsigned __int64    uint64;
		typedef signed __int64   	int64;
	#else
		typedef unsigned long long int    uint64;
		typedef signed long long   	int64;
	#endif

	typedef unsigned int uint;
	typedef float		float32;
	typedef double		float64;
#endif


#endif
