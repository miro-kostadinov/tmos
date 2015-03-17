/*
 * tl_initializer_list.h
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

#ifndef TL_INITIALIZER_LIST_H_
#define TL_INITIALIZER_LIST_H_

#include <tmos++.h>

namespace tmos
{
/// initializer_list
template<class _E>
class initializer_list
{
public:
	typedef _E value_type;
	typedef const _E& reference;
	typedef const _E& const_reference;
	typedef size_t size_type;
	typedef const _E* iterator;
	typedef const _E* const_iterator;

private:
	iterator _M_array;
	size_type _M_len;

	// The compiler can call a private constructor.
	constexpr initializer_list(const_iterator __a, size_type __l) :
			_M_array(__a), _M_len(__l)
	{
	}

public:
	constexpr initializer_list() :
			_M_array(0), _M_len(0)
	{
	}

	// Number of elements.
	constexpr size_type size()
	{	return _M_len;}

	// First element.
	constexpr const_iterator begin()
	{	return _M_array;}

	// One past the last element.
	constexpr const_iterator end()
	{	return begin() + size();}
};

/**
 *  @brief  Return an iterator pointing to the first element of
 *          the initilizer_list.
 *  @param  il  Initializer list.
 */
template<class _Tp>
constexpr const _Tp*
begin(initializer_list<_Tp> __ils)
{	return __ils.begin();}

/**
 *  @brief  Return an iterator pointing to one past the last element
 *          of the initilizer_list.
 *  @param  il  Initializer list.
 */
template<class _Tp>
constexpr const _Tp*
end(initializer_list<_Tp> __ils)
{	return __ils.end();}

} // namespace tmos

#endif /* TL_INITIALIZER_LIST_H_ */