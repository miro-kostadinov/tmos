/*
 * at91_types.h
 *
 *  Created on: 23.11.2010
 *      Author: Miroslav Kostadinov
 */

#ifndef AT91_TYPES_H_
#define AT91_TYPES_H_

#ifndef __ASSEMBLY__

#	ifndef __cplusplus
		typedef volatile const uint32_t RoReg; /**< Read only 32-bit register (volatile const unsigned int) */
#	else
		typedef volatile       uint32_t RoReg; /**< Read only 32-bit register (volatile const unsigned int) */
#	endif

	typedef volatile       uint32_t WoReg; /**< Write only 32-bit register (volatile unsigned int) */
	typedef volatile       uint32_t RwReg; /**< Read-Write 32-bit register (volatile unsigned int) */
#	define CAST(type, value) ((type *) value)
#	define REG_ACCESS(type, address) (*(type*)address) /**< C code: Register value */

#else

#	define CAST(type, value) (value)
#	define REG_ACCESS(type, address) (address) /**< Assembly code: Register address */

#endif

#endif /* AT91_TYPES_H_ */
