/*
 * tl_construct.h
 *
 *  Created on: Mar 19, 2013
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

#ifndef TL_CONSTRUCT_H_
#define TL_CONSTRUCT_H_

#include <tmos++.h>

namespace tmos
{
/**
 * Constructs an object in existing memory by invoking an allocated
 * object's constructor with an initializer.
 */
template<typename _T1, typename _T2>
  inline void
  _Construct(_T1* __p, const _T2& __value)
  {
    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 402. wrong new expression in [some_]allocator::construct
    ::new(static_cast<void*>(__p)) _T1(__value);
  }

/**
 * Destroy the object pointed to by a pointer type.
 */
template<typename _Tp>
  inline void
  _Destroy(_Tp* __pointer)
  { __pointer->~_Tp(); }

template<bool>
  struct _Destroy_aux
  {
    template<typename _ForwardIterator>
      static void
      __destroy(_ForwardIterator __first, _ForwardIterator __last)
	{
	  for (; __first != __last; ++__first)
	    tmos::_Destroy(tmos::__addressof(*__first));
	}
  };

template<>
  struct _Destroy_aux<true>
  {
    template<typename _ForwardIterator>
      static void
      __destroy(_ForwardIterator, _ForwardIterator) { }
  };

/**
 * Destroy a range of objects.  If the value_type of the object has
 * a trivial destructor, the compiler should optimize all of this
 * away, otherwise the objects' destructors must be invoked.
 */
template<typename _ForwardIterator>
  inline void
  _Destroy(_ForwardIterator __first, _ForwardIterator __last)
  {
    typedef typename iterator_traits<_ForwardIterator>::value_type
                     _Value_type;
    tmos::_Destroy_aux<__has_trivial_destructor(_Value_type)>::
	__destroy(__first, __last);
  }

/**
 * Destroy a range of objects using the supplied allocator.  For
 * nondefault allocators we do not optimize away invocation of
 * destroy() even if _Tp has a trivial destructor.
 */

template <typename _Tp> class allocator;

template<typename _ForwardIterator, typename _Allocator>
  void
  _Destroy(_ForwardIterator __first, _ForwardIterator __last,
	     _Allocator& __alloc)
  {
    for (; __first != __last; ++__first)
	__alloc.destroy(tmos::__addressof(*__first));
  }

template<typename _ForwardIterator, typename _Tp>
  inline void
  _Destroy(_ForwardIterator __first, _ForwardIterator __last,
	     allocator<_Tp>&)
  {
    _Destroy(__first, __last);
  }

} // namespace tmos

#endif /* TL_CONSTRUCT_H_ */
