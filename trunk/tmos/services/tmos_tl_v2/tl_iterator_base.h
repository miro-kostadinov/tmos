/*
 * tl_iterator_base.h
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

#ifndef TL_ITERATOR_BASE_H_
#define TL_ITERATOR_BASE_H_

#include <tmos++.h>

namespace tmos
{

template<typename _InputIterator>
  inline typename iterator_traits<_InputIterator>::difference_type
  __distance(_InputIterator __first, _InputIterator __last,
             input_iterator_tag)
  {
    typename iterator_traits<_InputIterator>::difference_type __n = 0;
    while (__first != __last)
	{
	  ++__first;
	  ++__n;
	}
    return __n;
  }

template<typename _RandomAccessIterator>
  inline typename iterator_traits<_RandomAccessIterator>::difference_type
  __distance(_RandomAccessIterator __first, _RandomAccessIterator __last,
             random_access_iterator_tag)
  {
    return __last - __first;
  }

/**
 *  @brief A generalization of pointer arithmetic.
 *  @param  first  An input iterator.
 *  @param  last  An input iterator.
 *  @return  The distance between them.
 *
 *  Returns @c n such that first + n == last.  This requires that @p last
 *  must be reachable from @p first.  Note that @c n may be negative.
 *
 *  For random access iterators, this uses their @c + and @c - operations
 *  and are constant time.  For other %iterator classes they are linear time.
*/
template<typename _InputIterator>
  inline typename iterator_traits<_InputIterator>::difference_type
  distance(_InputIterator __first, _InputIterator __last)
  {
    // concept requirements -- taken care of in __distance
    return tmos::__distance(__first, __last,
			     tmos::__iterator_category(__first));
  }

template<typename _InputIterator, typename _Distance>
  inline void
  __advance(_InputIterator& __i, _Distance __n, input_iterator_tag)
  {
    while (__n--)
	++__i;
  }

template<typename _BidirectionalIterator, typename _Distance>
  inline void
  __advance(_BidirectionalIterator& __i, _Distance __n,
	      bidirectional_iterator_tag)
  {
    if (__n > 0)
      while (__n--)
	  ++__i;
    else
      while (__n++)
	  --__i;
  }

template<typename _RandomAccessIterator, typename _Distance>
  inline void
  __advance(_RandomAccessIterator& __i, _Distance __n,
            random_access_iterator_tag)
  {
    __i += __n;
  }

/**
 *  @brief A generalization of pointer arithmetic.
 *  @param  i  An input iterator.
 *  @param  n  The @a delta by which to change @p i.
 *  @return  Nothing.
 *
 *  This increments @p i by @p n.  For bidirectional and random access
 *  iterators, @p n may be negative, in which case @p i is decremented.
 *
 *  For random access iterators, this uses their @c + and @c - operations
 *  and are constant time.  For other %iterator classes they are linear time.
*/
template<typename _InputIterator, typename _Distance>
  inline void
  advance(_InputIterator& __i, _Distance __n)
  {
    // concept requirements -- taken care of in __advance
    typename iterator_traits<_InputIterator>::difference_type __d = __n;
    tmos::__advance(__i, __d, tmos::__iterator_category(__i));
  }


} // namespace tmos


#endif /* TL_ITERATOR_BASE_H_ */
