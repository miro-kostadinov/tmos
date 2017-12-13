/*
 * tmos_stdlib.h
 *
 *  Created on: 10.11.2010
 *      Author: Miroslav Kostadinov
 */

#ifndef TMOS_STDLIB_H_
#define TMOS_STDLIB_H_

#include <trace.h>

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
inline static void* usr_realloc(void* volatile ptr, unsigned int volatile size);

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
int atoi(const char *ptr );
#if USE_TMOS_LLONG
long long atoll(const char *ptr);
#endif

/** Convert hes string to integer
 *
 * @param ptr
 * @return
 */
unsigned int ahextoi(const char *ptr );

/**
 * @brief String list
 *	The string list is a list of non-empty strings terminated by empty string.
 *
 *	Examples:
 *	 -	0, 0  - empty list ("")
 *	 -	'h', 'e', 'l', 'l', 'o', 0, 0 - list with one element ("hello", "")
 */
typedef const char* STR_LIST;

/**
 * Macro to create STR_LIST item
 *
 * Examples:
 * @code
 * 	STR_LIST my_list = SZ(first) SZ(second);
 *  //This is equivalent to:
 * const char * my_list = "first\0second\0";
 * @endcode
 */
#define SZ(item) # item "\0"

/**
 * Search a string with list of options to match
 * @param str string to scan
 * @param sl list of possible matches
 * @param dwRead matched symbols (position in the string)
 * @return index of the matched item or 0
 */
unsigned int find_in_list(const char* str, STR_LIST sl, unsigned int* dwRead);

/**
 * Return the position of the list item at specific position
 * @param sl string list
 * @param pos position
 * @return
 */
const char* szlist_at(STR_LIST sl, unsigned int pos);

unsigned int seconds_since(unsigned int time);
unsigned int ms_since(unsigned int time);

#ifdef __cplusplus
constexpr void* operator new(size_t size, void* obj)
{
    return (obj);
}
}
#endif

#endif /* TMOS_STDLIB_H_ */
