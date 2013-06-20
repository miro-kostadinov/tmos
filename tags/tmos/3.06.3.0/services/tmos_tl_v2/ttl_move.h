/*
 * ttl_move.h
 *
 *  Created on: Mar 28, 2013
 *      Author: miro
 */

#ifndef TTL_MOVE_H_
#define TTL_MOVE_H_

#include <ttl_config.h>
#include <ttl_type_traits.h>

namespace ttl
{

/*******************************************************************************
 * 								addressof
 * Obtains the actual address of the object or function arg, even in presence of
 * overloaded operator&
 ******************************************************************************/
template<typename _Tp>
inline _Tp* addressof(_Tp& __r) noexcept
{
	return reinterpret_cast<_Tp*>
		(&const_cast<char&>(reinterpret_cast<const volatile char&>(__r)));
}


/*******************************************************************************
 * 								forward
 * Forwards the argument to another function exactly as it was passed to the
 * calling function.
 ******************************************************************************/

///   Forward an lvalue.
template<typename _Tp>
constexpr _Tp&& forward(typename ttl::remove_reference<_Tp>::type& __t) noexcept
{
	return static_cast<_Tp&&>(__t);
}

///  Forward an rvalue.
template<typename _Tp>
constexpr _Tp&& forward(typename ttl::remove_reference<_Tp>::type&& __t) noexcept
{
	static_assert(!ttl::is_lvalue_reference<_Tp>::value, "template argument"
			" substituting _Tp is an lvalue reference type");
	return static_cast<_Tp&&>(__t);
}
/*******************************************************************************
 * 								move
 * obtains an rvalue reference to its argument.
 ******************************************************************************/
template<typename _Tp>
constexpr typename ttl::remove_reference<_Tp>::type&& move(_Tp&& __t) noexcept
{
	return static_cast<typename ttl::remove_reference<_Tp>::type&&>(__t);
}


/*******************************************************************************
 * 					Swaps two values.
 ******************************************************************************/
template<typename _Tp>
inline void swap(_Tp& a, _Tp& b)
{
    _Tp tmp = ttl::move(a);
	a = ttl::move(b);
	b = ttl::move(tmp);
}

/// Swap the contents of two arrays.
template<typename _Tp, size_t _Nm>
inline void swap(_Tp (&a)[_Nm], _Tp (&b)[_Nm])
{
	for (size_t n = 0; n < _Nm; ++n)
		swap(a[n], b[n]);
}



} // namespace ttl


#endif /* TTL_MOVE_H_ */
