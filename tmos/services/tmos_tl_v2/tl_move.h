/*
 * tl_move.h
 *
 *  Created on: Mar 18, 2013
 *      Author: miro
 */

// This file is derived from the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

#ifndef TL_MOVE_H_
#define TL_MOVE_H_

#include <tmos++.h>

#define _GLIBCXX_MOVE(__val) tmos::move(__val)
#define _GLIBCXX_FORWARD(_Tp, __val) tmos::forward<_Tp>(__val)


namespace tmos
{
// Used, in C++03 mode too, by allocators, etc.
template<typename _Tp>
inline _Tp* __addressof(_Tp& __r)
{
	return reinterpret_cast<_Tp*>
//	(&const_cast<char&>(reinterpret_cast<const volatile char&>(__r)));
	(&(__r));
}

/// forward (as per N3143)
template<typename _Tp>
inline _Tp&&
forward(typename tmos::remove_reference<_Tp>::type& __t)
{	return static_cast<_Tp&&>(__t);}

template<typename _Tp>
inline _Tp&&
forward(typename tmos::remove_reference<_Tp>::type&& __t)
{
	static_assert(!tmos::is_lvalue_reference<_Tp>::value, "template argument"
			" substituting _Tp is an lvalue reference type");
	return static_cast<_Tp&&>(__t);
}

/**
 *  @brief Move a value.
 *  @ingroup mutating_algorithms
 *  @param  __t  A thing of arbitrary type.
 *  @return Same, moved.
 */
template<typename _Tp>
inline typename tmos::remove_reference<_Tp>::type&&
move(_Tp&& __t)
{	return static_cast<typename tmos::remove_reference<_Tp>::type&&>(__t);}

/// declval, from type_traits.

/**
 *  @brief Returns the actual address of the object or function
 *         referenced by r, even in the presence of an overloaded
 *         operator&.
 *  @param  __r  Reference to an object or function.
 *  @return   The actual address.
*/
template<typename _Tp>
  inline _Tp*
  addressof(_Tp& __r)
  { return tmos::__addressof(__r); }
/**
 *  @brief Swaps two values.
 *  @ingroup mutating_algorithms
 *  @param  __a  A thing of arbitrary type.
 *  @param  __b  Another thing of arbitrary type.
 *  @return   Nothing.
*/
template<typename _Tp>
  inline void swap(_Tp& __a, _Tp& __b)
  {
    _Tp __tmp = _GLIBCXX_MOVE(__a);
    __a = _GLIBCXX_MOVE(__b);
    __b = _GLIBCXX_MOVE(__tmp);
  }

// _GLIBCXX_RESOLVE_LIB_DEFECTS
// DR 809. tmos::swap should be overloaded for array types.
template<typename _Tp, size_t _Nm>
  inline void swap(_Tp (&__a)[_Nm], _Tp (&__b)[_Nm])
  {
    for (size_t __n = 0; __n < _Nm; ++__n)
    	swap(__a[__n], __b[__n]);
  }



} // namespace tmos


#endif /* TL_MOVE_H_ */
