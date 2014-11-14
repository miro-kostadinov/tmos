/*
 * ttl_algo.h
 *
 *  Created on: Apr 10, 2013
 *      Author: miro
 *
 *  This file is modified version of STL::stl_algo.h
 */
// Algorithm implementation -*- C++ -*-

// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009,
// 2010, 2011
// Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
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

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

#ifndef TTL_ALGO_H_
#define TTL_ALGO_H_

#include <ttl_config.h>
#include <ttl_iterator.h>

namespace ttl
{
/// This is an overload used by find() for the Input Iterator case.
template<typename _InputIterator, typename _Tp>
  inline _InputIterator
  __find(_InputIterator __first, _InputIterator __last,
	   const _Tp& __val, input_iterator_tag)
  {
    while (__first != __last && !(*__first == __val))
	++__first;
    return __first;
  }

/// This is an overload used by find() for the RAI case.
template<typename _RandomAccessIterator, typename _Tp>
_RandomAccessIterator __find(_RandomAccessIterator __first,
		_RandomAccessIterator __last, const _Tp& __val,
		random_access_iterator_tag)
{
	typename iterator_traits<_RandomAccessIterator>::difference_type __trip_count =
			(__last - __first) >> 2;

	for (; __trip_count > 0; --__trip_count)
	{
		if (*__first == __val)
			return __first;
		++__first;

		if (*__first == __val)
			return __first;
		++__first;

		if (*__first == __val)
			return __first;
		++__first;

		if (*__first == __val)
			return __first;
		++__first;
	}

	switch (__last - __first)
	{
	case 3:
		if (*__first == __val)
			return __first;
		++__first;
	case 2:
		if (*__first == __val)
			return __first;
		++__first;
	case 1:
		if (*__first == __val)
			return __first;
		++__first;
	case 0:
	default:
		return __last;
	}
}



/**
 *  @brief Find the first occurrence of a value in a sequence.
 *  @ingroup non_mutating_algorithms
 *  @param  first  An input iterator.
 *  @param  last   An input iterator.
 *  @param  val    The value to find.
 *  @return   The first iterator @c i in the range @p [first,last)
 *  such that @c *i == @p val, or @p last if no such iterator exists.
*/
template<typename _InputIterator, typename _Tp>
inline _InputIterator find(_InputIterator __first, _InputIterator __last,
		const _Tp& __val)
{
	return ttl::__find(__first, __last, __val, ttl::__iterator_category(__first));
}

/*******************************************************************************
 * 									fill_n
 *
 * Assigns value to the first n elements of the sequence pointed by first.
 * The return value is first + n.
 ******************************************************************************/
template <typename OutputIterator, typename T>
inline OutputIterator fill_n (OutputIterator first, size_t n, const T& value)
{
	for (; n; --n, ++first)
		*first = value;
	return (first);
}

/*******************************************************************************
 * 								 swap
 ******************************************************************************/
template <typename Iterator>
inline void iter_swap (Iterator a, Iterator b)
{
    swap(*a, *b);
}

/*******************************************************************************
 * 								 swap_ranges
 * Swaps corresponding elements of [first, last) and [result,)
 ******************************************************************************/
template <typename ForwardIterator1, typename ForwardIterator2>
inline ForwardIterator2 swap_ranges (ForwardIterator1 first, ForwardIterator2 last, ForwardIterator2 result)
{
	for (; first != last; ++first, ++result)
		iter_swap(first, result);
	return (result);
}

} // namespace ttl


#endif /* TTL_ALGO_H_ */
