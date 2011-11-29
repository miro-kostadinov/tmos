/*
 * tmos_stdlib.h
 *
 *  Created on: 10.11.2010
 *      Author: Miroslav Kostadinov
 */

#ifndef TMOS_STDLIB_H_
#define TMOS_STDLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 Dynamic memory
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#define offsetof(type, member)  __builtin_offsetof (type, member)


void  svc_pool_init(void* start, void* end);
void* svc_pool_malloc(unsigned int size, void* pool);
void  svc_pool_free(void* ptr, void* pool);

void* svc_malloc(unsigned int size);
void  svc_free(void* ptr);
void* svc_realloc(void* ptr, unsigned int size);
void* tsk_malloc_clear(unsigned int size);

#if TRACE_MEMORY_LEVEL >= TRACE_DEFAULT_LEVEL
	void* tsk_malloc(unsigned int size);
	void  tsk_free(void* ptr);
	void* tsk_realloc(void* ptr, unsigned int size);
#else
#	define tsk_malloc(size) 		usr_malloc(size)
#	define tsk_free(ptr) 			usr_free(ptr)
#	define tsk_realloc(ptr, size) 	usr_realloc(ptr, size)
#endif

#define dyn_sizeof(ptr)	((((unsigned short*)ptr)[-2])*4 -4)

/** Convert string to integer
 *
 * Parses the C string str interpreting its content as an integral number, which
 * is returned as an int value.
 * @param ptr
 * @return
 */
unsigned int atoi(const char *ptr );

/** Convert hes string to integer
 *
 * @param ptr
 * @return
 */
unsigned int ahextoi(const char *ptr );

#ifdef __cplusplus
}
#endif

#endif /* TMOS_STDLIB_H_ */
