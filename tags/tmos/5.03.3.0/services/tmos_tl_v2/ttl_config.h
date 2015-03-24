/*
 * ttl_config.h
 *
 *  Created on: Mar 21, 2013
 *      Author: miro
 */

#ifndef TTL_CONFIG_H_
#define TTL_CONFIG_H_

#include <tmos.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 TMOS Template Library Configuration
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

extern "C" void __ttl_trow(unsigned int what, const char* file, unsigned int line, const char* func);

#define EXCEPT_OUT_OF_RANGE	 	0x83
#define EXCEPT_RANGE_CONSTRUCT 	0x84
#define EXCEPT_OUT_OF_MEMORY 	RES_OUT_OF_MEMORY

#ifndef TTL_THROW
#define TTL_THROW(what) __ttl_trow(what, __FILE__, __LINE__, __FUNCTION__)
#endif

# define __try      if (true)
# define __catch(X) if (false)
# define __throw_exception_again

/* Define to the value of the EOF integer constant. */
#define _TTLCXX_STDIO_EOF -1

namespace ttl
{

typedef int 			ptrdiff_t;
typedef unsigned int 	size_t;

template<typename T>
constexpr bool is_ram_ptr(T* p)
{
	return (reinterpret_cast<int>(p)>>24)==0x20;
}

}

#endif /* TTL_CONFIG_H_ */
