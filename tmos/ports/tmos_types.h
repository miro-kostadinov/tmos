/**************************************************************************//**
 * @ingroup ports
 * @{
 * @file     tmos_types.h
 * @brief    TMOS basic types
 * @version  V3.00
 * @date     20. November 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/
#ifndef TMOS_TYPES_H_
#define TMOS_TYPES_H_

#ifndef __ASSEMBLY__

#ifdef __cplusplus
static inline int IS_NOT_NULL(const void* x)		//!< Suppress GCC 7.X warnings
{
	asm volatile (
			""
			: "+r" (x)
			:
			:  );
	return (int)x;
}
#endif

#define __no_init		__attribute__ ((section (".ramnoinitsec")))

#ifdef __cplusplus
#	define EXTERN_C		extern "C"
namespace tmos
{
typedef unsigned int size_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;
union ureg_t
{
	uint8_t   as_u8;
	uint16_t  as_u16;
	uint32_t  as_u32;
};

typedef unsigned int RES_CODE;
#ifndef min
#define min(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#endif
#ifndef max
#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#endif
#ifndef abs
#define abs(a) (((a)<0)?(-(a)):(a))
#endif
#ifndef NULL
#define NULL 0
#endif
}
using namespace tmos;
#else
#	define EXTERN_C		extern

typedef unsigned int size_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned char uint8_t;
typedef char int8_t;
union ureg_t
{
	uint8_t   as_u8;
	uint16_t  as_u16;
	uint32_t  as_u32;
};

typedef unsigned int RES_CODE;
#ifndef min
#define min(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#endif
#ifndef max
#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#endif
#ifndef NULL
#define NULL ((void *)0)
#endif

#endif

#define WEAK 			__attribute__ ((weak))
#define WEAK_C 			EXTERN_C WEAK


/**
 * IO definitions
 *
 * define access restrictions to peripheral registers
 */

#define __I     volatile const	/*!< defines 'read only' permissions      */
#define __O     volatile        /*!< defines 'write only' permissions     */
#define __IO	volatile        /*!< defines 'read / write' permissions   */

#	define CAST(type, value) ((type *) (value))
#	define REG_ACCESS(type, address) (*(type*)(address)) /**< C code: Register value */
#else
#	define CAST(type, value) (value)
#	define REG_ACCESS(type, address) (address) /**< Assembly code: Register address */
#endif  /* __ASSEMBLY__ */

#endif /* TMOS_TYPES_H_ */
/** @} ingroup ports */
