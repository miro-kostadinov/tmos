/*
 * ttl_memory.h
 *
 *  Created on: Dec 4, 2014
 *      Author: miro
 */

#ifndef TTL_MEMORY_H_
#define TTL_MEMORY_H_

#include <ttl_config.h>
#include <ttl_type_traits.h>

namespace ttl
{

/*******************************************************************************
 * 								 destroy
 ******************************************************************************/

/// Calls the destructor of p without calling delete.
template <typename T>
inline void destroy (T* p)
{
    p->~T();
}

/// Calls the destructor on elements in range [first, last) without calling delete.
template <typename ForwardIterator>
inline void destroy (ForwardIterator first, ForwardIterator last)
{
    typedef typename iterator_traits<ForwardIterator>::value_type value_type;
	if(!ttl::has_trivial_destructor<value_type>::value)
		while(first < last)
			destroy (&*first++);
}

/*******************************************************************************
 * 								 construct
 ******************************************************************************/

/// Calls the placement new on  p.
template <typename T>
inline void construct (T* p, const T& value)
{
    new (p) T (value);
}

/// Calls the placement new on p.
template <typename T>
inline void construct (T* p)
{
    new (p) T;
}

/// Calls the placement new on p.
template <typename ForwardIterator>
inline void construct (ForwardIterator first, ForwardIterator last)
{
    typedef typename iterator_traits<ForwardIterator>::value_type value_type;
    if (ttl::is_integral<value_type>::value)
    	memset ( first, 0, max(distance(first,last),0)*sizeof(value_type));
    else
		while(first < last)
    		construct (&*first++);
}

/*******************************************************************************
 * 								 uninitialized_copy
 ******************************************************************************/
/// Copies [first, last) into result by calling copy constructors in result.
template <typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy (InputIterator first, InputIterator last, ForwardIterator result)
{
    for (; first < last; ++result, ++first)
	construct (&*result, *first);
    return (result);
}

/// Copies [first, first + n) into result by calling copy constructors in result.
template <typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy_n (InputIterator first, size_t n, ForwardIterator result)
{
    for (++n; --n; ++result, ++first)
	construct (&*result, *first);
    return (result);
}



} // namespace ttl


#endif /* TTL_MEMORY_H_ */
