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

#define __no_init		__attribute__ ((section (".ramnoinitsec")))

#ifdef __cplusplus
#	define EXTERN_C		extern "C"
#else
#	define EXTERN_C		extern
#endif

#define WEAK 			__attribute__ ((weak))
#define WEAK_C 			EXTERN_C WEAK

typedef unsigned int size_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned int RES_CODE;

#define NULL 0

#define min(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })

/**
 * IO definitions
 *
 * define access restrictions to peripheral registers
 */

#define __I     volatile const	/*!< defines 'read only' permissions      */
#define __O     volatile        /*!< defines 'write only' permissions     */
#define __IO	volatile        /*!< defines 'read / write' permissions   */

#endif /* TMOS_TYPES_H_ */
/** @} ingroup ports */
