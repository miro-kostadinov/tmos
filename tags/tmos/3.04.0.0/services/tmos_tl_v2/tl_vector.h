/*
 * tl_vector.h
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

#ifndef TL_VECTOR_H_
#define TL_VECTOR_H_

#include <tmos++.h>
#include <tl_type_traits.h>
#include <tl_allocator.h>
#include <tl_uninitialized.h>
//#include <tl_initializer_list.h>
#include <initializer_list>

void* operator new(size_t size, void* obj);

namespace tmos
{

template<typename _Tp, typename _Alloc>
struct _Vector_base
{
	typedef typename _Alloc::template rebind<_Tp>::other _Tp_alloc_type;

	struct _Vector_impl: public _Tp_alloc_type
	{
		typename _Tp_alloc_type::pointer _M_start;
		typename _Tp_alloc_type::pointer _M_finish;
		typename _Tp_alloc_type::pointer _M_end_of_storage;

		_Vector_impl() :
				_Tp_alloc_type(), _M_start(0), _M_finish(0), _M_end_of_storage(
						0)
		{
		}

		_Vector_impl(_Tp_alloc_type const& __a) :
				_Tp_alloc_type(__a), _M_start(0), _M_finish(0), _M_end_of_storage(
						0)
		{
		}
	};

public:
	typedef _Alloc allocator_type;

	_Tp_alloc_type&
	_M_get_Tp_allocator()
	{
		return *static_cast<_Tp_alloc_type*>(&this->_M_impl);
	}

	const _Tp_alloc_type&
	_M_get_Tp_allocator() const
	{
		return *static_cast<const _Tp_alloc_type*>(&this->_M_impl);
	}

	allocator_type get_allocator() const
	{
		return allocator_type(_M_get_Tp_allocator());
	}

	_Vector_base() :
			_M_impl()
	{
	}

	_Vector_base(const allocator_type& __a) :
			_M_impl(__a)
	{
	}

	_Vector_base(size_t __n) :
			_M_impl()
	{
		this->_M_impl._M_start = this->_M_allocate(__n);
		this->_M_impl._M_finish = this->_M_impl._M_start;
		this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
	}

	_Vector_base(size_t __n, const allocator_type& __a) :
			_M_impl(__a)
	{
		this->_M_impl._M_start = this->_M_allocate(__n);
		this->_M_impl._M_finish = this->_M_impl._M_start;
		this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
	}

	~_Vector_base()
	{
		_M_deallocate(this->_M_impl._M_start,
				this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	}

public:
	_Vector_impl _M_impl;

	typename _Tp_alloc_type::pointer _M_allocate(size_t __n)
	{
		return __n != 0 ? _M_impl.allocate(__n) : 0;
	}

	void _M_deallocate(typename _Tp_alloc_type::pointer __p, size_t __n)
	{
		if (__p)
			_M_impl.deallocate(__p, __n);
	}
};



/**
 *  @brief A standard container which offers fixed time access to
 *  individual elements in any order.
 *
 *  @ingroup sequences
 *
 *  Meets the requirements of a <a href="tables.html#65">container</a>, a
 *  <a href="tables.html#66">reversible container</a>, and a
 *  <a href="tables.html#67">sequence</a>, including the
 *  <a href="tables.html#68">optional sequence requirements</a> with the
 *  %exception of @c push_front and @c pop_front.
 *
 *  In some terminology a %vector can be described as a dynamic
 *  C-style array, it offers fast and efficient access to individual
 *  elements in any order and saves the user from worrying about
 *  memory and size allocation.  Subscripting ( @c [] ) access is
 *  also provided as with C-style arrays.
*/
template<typename _Tp, typename _Alloc = tmos::allocator<_Tp> >
class vector: protected _Vector_base<_Tp, _Alloc>
{
	// Concept requirements.
	typedef typename _Alloc::value_type 	_Alloc_value_type;
	typedef _Vector_base<_Tp, _Alloc> 		_Base;
	typedef typename _Base::_Tp_alloc_type 	_Tp_alloc_type;

public:
	typedef _Tp value_type;
	typedef typename _Tp_alloc_type::pointer pointer;
	typedef typename _Tp_alloc_type::const_pointer const_pointer;
	typedef typename _Tp_alloc_type::reference reference;
	typedef typename _Tp_alloc_type::const_reference const_reference;
	typedef __tmos_cxx:: __normal_iterator <pointer, vector> iterator;
	typedef __tmos_cxx::__normal_iterator<const_pointer, vector>
	const_iterator;
	typedef tmos::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef tmos::reverse_iterator<iterator> reverse_iterator;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef _Alloc allocator_type;

protected:
	using _Base::_M_allocate;
	using _Base::_M_deallocate;
	using _Base::_M_impl;
	using _Base::_M_get_Tp_allocator;

public:
	// [23.2.4.1] construct/copy/destroy
	// (assign() and get_allocator() are also listed in this section)
	/**
	 *  @brief  Default constructor creates no elements.
	 */
	vector()
	: _Base()
	{}

	/**
	 *  @brief  Creates a %vector with no elements.
	 *  @param  a  An allocator object.
	 */
	explicit
	vector(const allocator_type& __a)
	: _Base(__a)
	{}

	/**
	 *  @brief  Creates a %vector with copies of an exemplar element.
	 *  @param  n  The number of elements to initially create.
	 *  @param  value  An element to copy.
	 *  @param  a  An allocator.
	 *
	 *  This constructor fills the %vector with @a n copies of @a value.
	 */
	explicit
	vector(size_type __n, const value_type& __value = value_type(),
			const allocator_type& __a = allocator_type())
	: _Base(__n, __a)
	{
		_M_fill_initialize(__n, __value);
	}

	/**
	 *  @brief  %Vector copy constructor.
	 *  @param  x  A %vector of identical element and allocator types.
	 *
	 *  The newly-created %vector uses a copy of the allocation
	 *  object used by @a x.  All the elements of @a x are copied,
	 *  but any extra memory in
	 *  @a x (for fast expansion) will not be copied.
	 */
	vector(const vector& __x)
	: _Base(__x.size(), __x._M_get_Tp_allocator())
	{	this->_M_impl._M_finish =
		tmos::__uninitialized_copy_a(__x.begin(), __x.end(),
				this->_M_impl._M_start,
				_M_get_Tp_allocator());
	}

    /**
     *  @brief  Builds a %vector from an initializer list.
     *  @param  l  An initializer_list.
     *  @param  a  An allocator.
     *
     *  Create a %vector consisting of copies of the elements in the
     *  initializer_list @a l.
     *
     *  This will call the element type's copy constructor N times
     *  (where N is @a l.size()) and do no memory reallocation.
     */
	vector(std::initializer_list<value_type> __l,
			const allocator_type& __a = allocator_type())
	: _Base(__a)
	{
		_M_range_initialize(__l.begin(), __l.end(),
				random_access_iterator_tag());
	}

	/**
	 *  @brief  Builds a %vector from a range.
	 *  @param  first  An input iterator.
	 *  @param  last  An input iterator.
	 *  @param  a  An allocator.
	 *
	 *  Create a %vector consisting of copies of the elements from
	 *  [first,last).
	 *
	 *  If the iterators are forward, bidirectional, or
	 *  random-access, then this will call the elements' copy
	 *  constructor N times (where N is distance(first,last)) and do
	 *  no memory reallocation.  But if only input iterators are
	 *  used, then this will do at most 2N calls to the copy
	 *  constructor, and logN memory reallocations.
	 */
	template<typename _InputIterator>
	vector(_InputIterator __first, _InputIterator __last,
			const allocator_type& __a = allocator_type())
	: _Base(__a)
	{
		// Check whether it's an integral type.  If so, it's not an iterator.
		typedef typename tmos::__is_integer<_InputIterator>::__type _Integral;
		_M_initialize_dispatch(__first, __last, _Integral());
	}

	/**
	 *  The dtor only erases the elements, and note that if the
	 *  elements themselves are pointers, the pointed-to memory is
	 *  not touched in any way.  Managing the pointer is the user's
	 *  responsibility.
	 */
	~vector()
	{	tmos::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
				_M_get_Tp_allocator());}

	/**
	 *  @brief  %Vector assignment operator.
	 *  @param  x  A %vector of identical element and allocator types.
	 *
	 *  All the elements of @a x are copied, but any extra memory in
	 *  @a x (for fast expansion) will not be copied.  Unlike the
	 *  copy constructor, the allocator object is not copied.
	 */
	vector&
	operator=(const vector& __x);

	/**
	 *  @brief  %Vector move assignment operator.
	 *  @param  x  A %vector of identical element and allocator types.
	 *
	 *  The contents of @a x are moved into this %vector (without copying).
	 *  @a x is a valid, but unspecified %vector.
	 */
	vector&
	operator=(vector&& __x)
	{
		// NB: DR 1204.
		// NB: DR 675.
		this->clear();
		this->swap(__x);
		return *this;
	}

    /**
     *  @brief  %Vector list assignment operator.
     *  @param  l  An initializer_list.
     *
     *  This function fills a %vector with copies of the elements in the
     *  initializer list @a l.
     *
     *  Note that the assignment completely changes the %vector and
     *  that the resulting %vector's size is the same as the number
     *  of elements assigned.  Old data may be lost.
     */
	vector&
	operator=(std::initializer_list<value_type> __l)
	{
		this->assign(__l.begin(), __l.end());
		return *this;
	}


	/**
	 *  @brief  Assigns a given value to a %vector.
	 *  @param  n  Number of elements to be assigned.
	 *  @param  val  Value to be assigned.
	 *
	 *  This function fills a %vector with @a n copies of the given
	 *  value.  Note that the assignment completely changes the
	 *  %vector and that the resulting %vector's size is the same as
	 *  the number of elements assigned.  Old data may be lost.
	 */
	void
	assign(size_type __n, const value_type& __val)
	{	_M_fill_assign(__n, __val);}

	/**
	 *  @brief  Assigns a range to a %vector.
	 *  @param  first  An input iterator.
	 *  @param  last   An input iterator.
	 *
	 *  This function fills a %vector with copies of the elements in the
	 *  range [first,last).
	 *
	 *  Note that the assignment completely changes the %vector and
	 *  that the resulting %vector's size is the same as the number
	 *  of elements assigned.  Old data may be lost.
	 */
	template<typename _InputIterator>
	void
	assign(_InputIterator __first, _InputIterator __last)
	{
		// Check whether it's an integral type.  If so, it's not an iterator.
		typedef typename tmos::__is_integer<_InputIterator>::__type _Integral;
		_M_assign_dispatch(__first, __last, _Integral());
	}

	/// Get a copy of the memory allocation object.
	using _Base::get_allocator;

	// iterators
	/**
	 *  Returns a read/write iterator that points to the first
	 *  element in the %vector.  Iteration is done in ordinary
	 *  element order.
	 */
	iterator
	begin()
	{	return iterator(this->_M_impl._M_start);}

	/**
	 *  Returns a read-only (constant) iterator that points to the
	 *  first element in the %vector.  Iteration is done in ordinary
	 *  element order.
	 */
	const_iterator
	begin() const
	{	return const_iterator(this->_M_impl._M_start);}

	/**
	 *  Returns a read/write iterator that points one past the last
	 *  element in the %vector.  Iteration is done in ordinary
	 *  element order.
	 */
	iterator
	end()
	{	return iterator(this->_M_impl._M_finish);}

	/**
	 *  Returns a read-only (constant) iterator that points one past
	 *  the last element in the %vector.  Iteration is done in
	 *  ordinary element order.
	 */
	const_iterator
	end() const
	{	return const_iterator(this->_M_impl._M_finish);}

	/**
	 *  Returns a read/write reverse iterator that points to the
	 *  last element in the %vector.  Iteration is done in reverse
	 *  element order.
	 */
	reverse_iterator
	rbegin()
	{	return reverse_iterator(end());}

	/**
	 *  Returns a read-only (constant) reverse iterator that points
	 *  to the last element in the %vector.  Iteration is done in
	 *  reverse element order.
	 */
	const_reverse_iterator
	rbegin() const
	{	return const_reverse_iterator(end());}

	/**
	 *  Returns a read/write reverse iterator that points to one
	 *  before the first element in the %vector.  Iteration is done
	 *  in reverse element order.
	 */
	reverse_iterator
	rend()
	{	return reverse_iterator(begin());}

	/**
	 *  Returns a read-only (constant) reverse iterator that points
	 *  to one before the first element in the %vector.  Iteration
	 *  is done in reverse element order.
	 */
	const_reverse_iterator
	rend() const
	{	return const_reverse_iterator(begin());}

	/**
	 *  Returns a read-only (constant) iterator that points to the
	 *  first element in the %vector.  Iteration is done in ordinary
	 *  element order.
	 */
	const_iterator
	cbegin() const
	{	return const_iterator(this->_M_impl._M_start);}

	/**
	 *  Returns a read-only (constant) iterator that points one past
	 *  the last element in the %vector.  Iteration is done in
	 *  ordinary element order.
	 */
	const_iterator
	cend() const
	{	return const_iterator(this->_M_impl._M_finish);}

	/**
	 *  Returns a read-only (constant) reverse iterator that points
	 *  to the last element in the %vector.  Iteration is done in
	 *  reverse element order.
	 */
	const_reverse_iterator
	crbegin() const
	{	return const_reverse_iterator(end());}

	/**
	 *  Returns a read-only (constant) reverse iterator that points
	 *  to one before the first element in the %vector.  Iteration
	 *  is done in reverse element order.
	 */
	const_reverse_iterator
	crend() const
	{	return const_reverse_iterator(begin());}

	// [23.2.4.2] capacity
	/**  Returns the number of elements in the %vector.  */
	size_type
	size() const
	{	return size_type(this->_M_impl._M_finish - this->_M_impl._M_start);}

	/**  Returns the size() of the largest possible %vector.  */
	size_type
	max_size() const
	{	return _M_get_Tp_allocator().max_size();}

	/**
	 *  @brief  Resizes the %vector to the specified number of elements.
	 *  @param  new_size  Number of elements the %vector should contain.
	 *
	 *  This function will %resize the %vector to the specified
	 *  number of elements.  If the number is smaller than the
	 *  %vector's current size the %vector is truncated, otherwise
	 *  default constructed elements are appended.
	 */
	void
	resize(size_type __new_size)
	{
		if (__new_size > size())
		_M_default_append(__new_size - size());
		else if (__new_size < size())
		_M_erase_at_end(this->_M_impl._M_start + __new_size);
	}

	/**
	 *  @brief  Resizes the %vector to the specified number of elements.
	 *  @param  new_size  Number of elements the %vector should contain.
	 *  @param  x  Data with which new elements should be populated.
	 *
	 *  This function will %resize the %vector to the specified
	 *  number of elements.  If the number is smaller than the
	 *  %vector's current size the %vector is truncated, otherwise
	 *  the %vector is extended and new elements are populated with
	 *  given data.
	 */
	void
	resize(size_type __new_size, const value_type& __x)
	{
		if (__new_size > size())
		insert(end(), __new_size - size(), __x);
		else if (__new_size < size())
		_M_erase_at_end(this->_M_impl._M_start + __new_size);
	}

	/**
	 *  Returns the total number of elements that the %vector can
	 *  hold before needing to allocate more memory.
	 */
	size_type
	capacity() const
	{	return size_type(this->_M_impl._M_end_of_storage
				- this->_M_impl._M_start);}

	/**
	 *  Returns true if the %vector is empty.  (Thus begin() would
	 *  equal end().)
	 */
	bool
	empty() const
	{	return begin() == end();}

	/**
	 *  @brief  Attempt to preallocate enough memory for specified number of
	 *          elements.
	 *  @param  n  Number of elements required.
	 *  @throw  tmos::length_error  If @a n exceeds @c max_size().
	 *
	 *  This function attempts to reserve enough memory for the
	 *  %vector to hold the specified number of elements.  If the
	 *  number requested is more than max_size(), length_error is
	 *  thrown.
	 *
	 *  The advantage of this function is that if optimal code is a
	 *  necessity and the user can determine the number of elements
	 *  that will be required, the user can reserve the memory in
	 *  %advance, and thus prevent a possible reallocation of memory
	 *  and copying of %vector data.
	 */
	void
	reserve(size_type __n);

	// element access
	/**
	 *  @brief  Subscript access to the data contained in the %vector.
	 *  @param n The index of the element for which data should be
	 *  accessed.
	 *  @return  Read/write reference to data.
	 *
	 *  This operator allows for easy, array-style, data access.
	 *  Note that data access with this operator is unchecked and
	 *  out_of_range lookups are not defined. (For checked lookups
	 *  see at().)
	 */
	reference
	operator[](size_type __n)
	{	return *(this->_M_impl._M_start + __n);}

	/**
	 *  @brief  Subscript access to the data contained in the %vector.
	 *  @param n The index of the element for which data should be
	 *  accessed.
	 *  @return  Read-only (constant) reference to data.
	 *
	 *  This operator allows for easy, array-style, data access.
	 *  Note that data access with this operator is unchecked and
	 *  out_of_range lookups are not defined. (For checked lookups
	 *  see at().)
	 */
	const_reference
	operator[](size_type __n) const
	{	return *(this->_M_impl._M_start + __n);}

protected:
	/// Safety check used only from at().
	void
	_M_range_check(size_type __n) const
	{
		if (__n >= this->size())
		__throw_out_of_range(__N("vector::_M_range_check"));
	}

public:
	/**
	 *  @brief  Provides access to the data contained in the %vector.
	 *  @param n The index of the element for which data should be
	 *  accessed.
	 *  @return  Read/write reference to data.
	 *  @throw  tmos::out_of_range  If @a n is an invalid index.
	 *
	 *  This function provides for safer data access.  The parameter
	 *  is first checked that it is in the range of the vector.  The
	 *  function throws out_of_range if the check fails.
	 */
	reference
	at(size_type __n)
	{
		_M_range_check(__n);
		return (*this)[__n];
	}

	/**
	 *  @brief  Provides access to the data contained in the %vector.
	 *  @param n The index of the element for which data should be
	 *  accessed.
	 *  @return  Read-only (constant) reference to data.
	 *  @throw  tmos::out_of_range  If @a n is an invalid index.
	 *
	 *  This function provides for safer data access.  The parameter
	 *  is first checked that it is in the range of the vector.  The
	 *  function throws out_of_range if the check fails.
	 */
	const_reference
	at(size_type __n) const
	{
		_M_range_check(__n);
		return (*this)[__n];
	}

	/**
	 *  Returns a read/write reference to the data at the first
	 *  element of the %vector.
	 */
	reference
	front()
	{	return *begin();}

	/**
	 *  Returns a read-only (constant) reference to the data at the first
	 *  element of the %vector.
	 */
	const_reference
	front() const
	{	return *begin();}

	/**
	 *  Returns a read/write reference to the data at the last
	 *  element of the %vector.
	 */
	reference
	back()
	{	return *(end() - 1);}

	/**
	 *  Returns a read-only (constant) reference to the data at the
	 *  last element of the %vector.
	 */
	const_reference
	back() const
	{	return *(end() - 1);}

	// _GLIBCXX_RESOLVE_LIB_DEFECTS
	// DR 464. Suggestion for new member functions in standard containers.
	// data access
	/**
	 *   Returns a pointer such that [data(), data() + size()) is a valid
	 *   range.  For a non-empty %vector, data() == &front().
	 */
	_Tp* data()
	{
		return tmos::__addressof(front());
	}

	const _Tp*	data() const
	{
		return tmos::__addressof(front());
	}

	// [23.2.4.3] modifiers
	/**
	 *  @brief  Add data to the end of the %vector.
	 *  @param  x  Data to be added.
	 *
	 *  This is a typical stack operation.  The function creates an
	 *  element at the end of the %vector and assigns the given data
	 *  to it.  Due to the nature of a %vector this operation can be
	 *  done in constant time if the %vector has preallocated space
	 *  available.
	 */
	void
	push_back(const value_type& __x)
	{
		if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
		{
			this->_M_impl.construct(this->_M_impl._M_finish, __x);
			++this->_M_impl._M_finish;
		}
		else
		_M_insert_aux(end(), __x);
	}

//	void
//	push_back(value_type&& __x)
//	{	emplace_back(tmos::move(__x));}

	template<typename... _Args>
	void
	emplace_back(_Args&&... __args);

	/**
	 *  @brief  Removes last element.
	 *
	 *  This is a typical stack operation. It shrinks the %vector by one.
	 *
	 *  Note that no data is returned, and if the last element's
	 *  data is needed, it should be retrieved before pop_back() is
	 *  called.
	 */
	void
	pop_back()
	{
		--this->_M_impl._M_finish;
		this->_M_impl.destroy(this->_M_impl._M_finish);
	}

	/**
	 *  @brief  Inserts an object in %vector before specified iterator.
	 *  @param  position  An iterator into the %vector.
	 *  @param  args  Arguments.
	 *  @return  An iterator that points to the inserted data.
	 *
	 *  This function will insert an object of type T constructed
	 *  with T(tmos::forward<Args>(args)...) before the specified location.
	 *  Note that this kind of operation could be expensive for a %vector
	 *  and if it is frequently used the user should consider using
	 *  tmos::list.
	 */
	template<typename... _Args>
	iterator
	emplace(iterator __position, _Args&&... __args);

	/**
	 *  @brief  Inserts given value into %vector before specified iterator.
	 *  @param  position  An iterator into the %vector.
	 *  @param  x  Data to be inserted.
	 *  @return  An iterator that points to the inserted data.
	 *
	 *  This function will insert a copy of the given value before
	 *  the specified location.  Note that this kind of operation
	 *  could be expensive for a %vector and if it is frequently
	 *  used the user should consider using tmos::list.
	 */
	iterator
	insert(iterator __position, const value_type& __x);

//	/**
//	 *  @brief  Inserts given rvalue into %vector before specified iterator.
//	 *  @param  position  An iterator into the %vector.
//	 *  @param  x  Data to be inserted.
//	 *  @return  An iterator that points to the inserted data.
//	 *
//	 *  This function will insert a copy of the given rvalue before
//	 *  the specified location.  Note that this kind of operation
//	 *  could be expensive for a %vector and if it is frequently
//	 *  used the user should consider using tmos::list.
//	 */
//	iterator
//	insert(iterator __position, value_type&& __x)
//	{	return emplace(__position, tmos::move(__x));}


	/**
	 *  @brief  Inserts a number of copies of given data into the %vector.
	 *  @param  position  An iterator into the %vector.
	 *  @param  n  Number of elements to be inserted.
	 *  @param  x  Data to be inserted.
	 *
	 *  This function will insert a specified number of copies of
	 *  the given data before the location specified by @a position.
	 *
	 *  Note that this kind of operation could be expensive for a
	 *  %vector and if it is frequently used the user should
	 *  consider using tmos::list.
	 */
	void
	insert(iterator __position, size_type __n, const value_type& __x)
	{	_M_fill_insert(__position, __n, __x);}

	/**
	 *  @brief  Inserts a range into the %vector.
	 *  @param  position  An iterator into the %vector.
	 *  @param  first  An input iterator.
	 *  @param  last   An input iterator.
	 *
	 *  This function will insert copies of the data in the range
	 *  [first,last) into the %vector before the location specified
	 *  by @a pos.
	 *
	 *  Note that this kind of operation could be expensive for a
	 *  %vector and if it is frequently used the user should
	 *  consider using tmos::list.
	 */
	template<typename _InputIterator>
	void
	insert(iterator __position, _InputIterator __first,
			_InputIterator __last)
	{
		// Check whether it's an integral type.  If so, it's not an iterator.
		typedef typename tmos::__is_integer<_InputIterator>::__type _Integral;
		_M_insert_dispatch(__position, __first, __last, _Integral());
	}

	/**
	 *  @brief  Remove element at given position.
	 *  @param  position  Iterator pointing to element to be erased.
	 *  @return  An iterator pointing to the next element (or end()).
	 *
	 *  This function will erase the element at the given position and thus
	 *  shorten the %vector by one.
	 *
	 *  Note This operation could be expensive and if it is
	 *  frequently used the user should consider using tmos::list.
	 *  The user is also cautioned that this function only erases
	 *  the element, and that if the element is itself a pointer,
	 *  the pointed-to memory is not touched in any way.  Managing
	 *  the pointer is the user's responsibility.
	 */
	iterator
	erase(iterator __position);

	/**
	 *  @brief  Remove a range of elements.
	 *  @param  first  Iterator pointing to the first element to be erased.
	 *  @param  last  Iterator pointing to one past the last element to be
	 *                erased.
	 *  @return  An iterator pointing to the element pointed to by @a last
	 *           prior to erasing (or end()).
	 *
	 *  This function will erase the elements in the range [first,last) and
	 *  shorten the %vector accordingly.
	 *
	 *  Note This operation could be expensive and if it is
	 *  frequently used the user should consider using tmos::list.
	 *  The user is also cautioned that this function only erases
	 *  the elements, and that if the elements themselves are
	 *  pointers, the pointed-to memory is not touched in any way.
	 *  Managing the pointer is the user's responsibility.
	 */
	iterator
	erase(iterator __first, iterator __last);

	/**
	 *  @brief  Swaps data with another %vector.
	 *  @param  x  A %vector of the same element and allocator types.
	 *
	 *  This exchanges the elements between two vectors in constant time.
	 *  (Three pointers, so it should be quite fast.)
	 *  Note that the global tmos::swap() function is specialized such that
	 *  tmos::swap(v1,v2) will feed to this function.
	 */
	void
	swap(vector& __x)
	{
		tmos::swap(this->_M_impl._M_start, __x._M_impl._M_start);
		tmos::swap(this->_M_impl._M_finish, __x._M_impl._M_finish);
		tmos::swap(this->_M_impl._M_end_of_storage,
				__x._M_impl._M_end_of_storage);

		// _GLIBCXX_RESOLVE_LIB_DEFECTS
		// 431. Swapping containers with unequal allocators.
		tmos::__alloc_swap<_Tp_alloc_type>::_S_do_it(_M_get_Tp_allocator(),
				__x._M_get_Tp_allocator());
	}

	/**
	 *  Erases all the elements.  Note that this function only erases the
	 *  elements, and that if the elements themselves are pointers, the
	 *  pointed-to memory is not touched in any way.  Managing the pointer is
	 *  the user's responsibility.
	 */
	void
	clear()
	{	_M_erase_at_end(this->_M_impl._M_start);}

protected:
	/**
	 *  Memory expansion handler.  Uses the member allocation function to
	 *  obtain @a n bytes of memory, and then copies [first,last) into it.
	 */
	template<typename _ForwardIterator>
	pointer
	_M_allocate_and_copy(size_type __n,
			_ForwardIterator __first, _ForwardIterator __last)
	{
		pointer __result = this->_M_allocate(__n);
		__try
		{
			tmos::__uninitialized_copy_a(__first, __last, __result,
					_M_get_Tp_allocator());
			return __result;
		}
		__catch(...)
		{
			_M_deallocate(__result, __n);
			__throw_exception_again;
		}
	}

	// Internal constructor functions follow.

	// Called by the range constructor to implement [23.1.1]/9

	// _GLIBCXX_RESOLVE_LIB_DEFECTS
	// 438. Ambiguity in the "do the right thing" clause
	template<typename _Integer>
	void
	_M_initialize_dispatch(_Integer __n, _Integer __value, __true_type)
	{
		this->_M_impl._M_start = _M_allocate(static_cast<size_type>(__n));
		this->_M_impl._M_end_of_storage =
		this->_M_impl._M_start + static_cast<size_type>(__n);
		_M_fill_initialize(static_cast<size_type>(__n), __value);
	}

	// Called by the range constructor to implement [23.1.1]/9
	template<typename _InputIterator>
	void
	_M_initialize_dispatch(_InputIterator __first, _InputIterator __last,
			__false_type)
	{
		typedef typename tmos::iterator_traits<_InputIterator>::
		iterator_category _IterCategory;
		_M_range_initialize(__first, __last, _IterCategory());
	}

	// Called by the second initialize_dispatch above
	template<typename _InputIterator>
	void
	_M_range_initialize(_InputIterator __first,
			_InputIterator __last, tmos::input_iterator_tag)
	{
		for (; __first != __last; ++__first)
		push_back(*__first);
	}

	// Called by the second initialize_dispatch above
	template<typename _ForwardIterator>
	void
	_M_range_initialize(_ForwardIterator __first,
			_ForwardIterator __last, tmos::forward_iterator_tag)
	{
		const size_type __n = tmos::distance(__first, __last);
		this->_M_impl._M_start = this->_M_allocate(__n);
		this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
		this->_M_impl._M_finish =
		tmos::__uninitialized_copy_a(__first, __last,
				this->_M_impl._M_start,
				_M_get_Tp_allocator());
	}

	// Called by the first initialize_dispatch above and by the
	// vector(n,value,a) constructor.
	void
	_M_fill_initialize(size_type __n, const value_type& __value)
	{
		tmos::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value,
				_M_get_Tp_allocator());
		this->_M_impl._M_finish = this->_M_impl._M_end_of_storage;
	}

	// Internal assign functions follow.  The *_aux functions do the actual
	// assignment work for the range versions.

	// Called by the range assign to implement [23.1.1]/9

	// _GLIBCXX_RESOLVE_LIB_DEFECTS
	// 438. Ambiguity in the "do the right thing" clause
	template<typename _Integer>
	void
	_M_assign_dispatch(_Integer __n, _Integer __val, __true_type)
	{	_M_fill_assign(__n, __val);}

	// Called by the range assign to implement [23.1.1]/9
	template<typename _InputIterator>
	void
	_M_assign_dispatch(_InputIterator __first, _InputIterator __last,
			__false_type)
	{
		typedef typename tmos::iterator_traits<_InputIterator>::
		iterator_category _IterCategory;
		_M_assign_aux(__first, __last, _IterCategory());
	}

	// Called by the second assign_dispatch above
	template<typename _InputIterator>
	void
	_M_assign_aux(_InputIterator __first, _InputIterator __last,
			tmos::input_iterator_tag);

	// Called by the second assign_dispatch above
	template<typename _ForwardIterator>
	void
	_M_assign_aux(_ForwardIterator __first, _ForwardIterator __last,
			tmos::forward_iterator_tag);

	// Called by assign(n,t), and the range assign when it turns out
	// to be the same thing.
	void
	_M_fill_assign(size_type __n, const value_type& __val);

	// Internal insert functions follow.

	// Called by the range insert to implement [23.1.1]/9

	// _GLIBCXX_RESOLVE_LIB_DEFECTS
	// 438. Ambiguity in the "do the right thing" clause
	template<typename _Integer>
	void
	_M_insert_dispatch(iterator __pos, _Integer __n, _Integer __val,
			__true_type)
	{	_M_fill_insert(__pos, __n, __val);}

	// Called by the range insert to implement [23.1.1]/9
	template<typename _InputIterator>
	void
	_M_insert_dispatch(iterator __pos, _InputIterator __first,
			_InputIterator __last, __false_type)
	{
		typedef typename tmos::iterator_traits<_InputIterator>::
		iterator_category _IterCategory;
		_M_range_insert(__pos, __first, __last, _IterCategory());
	}

	// Called by the second insert_dispatch above
	template<typename _InputIterator>
	void
	_M_range_insert(iterator __pos, _InputIterator __first,
			_InputIterator __last, tmos::input_iterator_tag);

	// Called by the second insert_dispatch above
	template<typename _ForwardIterator>
	void
	_M_range_insert(iterator __pos, _ForwardIterator __first,
			_ForwardIterator __last, tmos::forward_iterator_tag);

	// Called by insert(p,n,x), and the range insert when it turns out to be
	// the same thing.
	void
	_M_fill_insert(iterator __pos, size_type __n, const value_type& __x);

	// Called by resize(n).
	void
	_M_default_append(size_type __n);

	// Called by insert(p,x)
	template<typename... _Args>
	void
	_M_insert_aux(iterator __position, _Args&&... __args);

	// Called by the latter.
	size_type
	_M_check_len(size_type __n, const char* __s) const
	{
		if (max_size() - size() < __n)
		__throw_length_error(__N(__s));

		const size_type __len = size() + tmos::max(size(), __n);
		return (__len < size() || __len > max_size()) ? max_size() : __len;
	}

	// Internal erase functions follow.

	// Called by erase(q1,q2), clear(), resize(), _M_fill_assign,
	// _M_assign_aux.
	void
	_M_erase_at_end(pointer __pos)
	{
		tmos::_Destroy(__pos, this->_M_impl._M_finish, _M_get_Tp_allocator());
		this->_M_impl._M_finish = __pos;
	}
};

/**
 *  @brief  Vector equality comparison.
 *  @param  x  A %vector.
 *  @param  y  A %vector of the same type as @a x.
 *  @return  True iff the size and elements of the vectors are equal.
 *
 *  This is an equivalence relation.  It is linear in the size of the
 *  vectors.  Vectors are considered equivalent if their sizes are equal,
 *  and if corresponding elements compare equal.
 */
template<typename _Tp, typename _Alloc>
inline bool operator==(const vector<_Tp, _Alloc>& __x,
		const vector<_Tp, _Alloc>& __y)
{
	return (__x.size() == __y.size()
			&& tmos::equal(__x.begin(), __x.end(), __y.begin()));
}

/**
 *  @brief  Vector ordering relation.
 *  @param  x  A %vector.
 *  @param  y  A %vector of the same type as @a x.
 *  @return  True iff @a x is lexicographically less than @a y.
 *
 *  This is a total ordering relation.  It is linear in the size of the
 *  vectors.  The elements must be comparable with @c <.
 *
 *  See tmos::lexicographical_compare() for how the determination is made.
 */
template<typename _Tp, typename _Alloc>
inline bool operator<(const vector<_Tp, _Alloc>& __x,
		const vector<_Tp, _Alloc>& __y)
{
	return tmos::lexicographical_compare(__x.begin(), __x.end(), __y.begin(),
			__y.end());
}

/// Based on operator==
template<typename _Tp, typename _Alloc>
inline bool operator!=(const vector<_Tp, _Alloc>& __x,
		const vector<_Tp, _Alloc>& __y)
{
	return !(__x == __y);
}

/// Based on operator<
template<typename _Tp, typename _Alloc>
inline bool operator>(const vector<_Tp, _Alloc>& __x,
		const vector<_Tp, _Alloc>& __y)
{
	return __y < __x;
}

/// Based on operator<
template<typename _Tp, typename _Alloc>
inline bool operator<=(const vector<_Tp, _Alloc>& __x,
		const vector<_Tp, _Alloc>& __y)
{
	return !(__y < __x);
}

/// Based on operator<
template<typename _Tp, typename _Alloc>
inline bool operator>=(const vector<_Tp, _Alloc>& __x,
		const vector<_Tp, _Alloc>& __y)
{
	return !(__x < __y);
}

/// See tmos::vector::swap().
template<typename _Tp, typename _Alloc>
inline void swap(vector<_Tp, _Alloc>& __x, vector<_Tp, _Alloc>& __y)
{
	__x.swap(__y);
}

} // namespace tmos

namespace tmos
{
template<typename _Tp, typename _Alloc>
void vector<_Tp, _Alloc>::reserve(size_type __n)
{
	if (__n > this->max_size())
		__throw_length_error(__N("vector::reserve"));
	if (this->capacity() < __n)
	{
		const size_type __old_size = size();
		pointer __tmp = _M_allocate_and_copy(__n,
				_GLIBCXX_MAKE_MOVE_ITERATOR(this->_M_impl._M_start),
				_GLIBCXX_MAKE_MOVE_ITERATOR(this->_M_impl._M_finish));
		tmos::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
				_M_get_Tp_allocator());
		_M_deallocate(this->_M_impl._M_start,
				this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
		this->_M_impl._M_start = __tmp;
		this->_M_impl._M_finish = __tmp + __old_size;
		this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
	}
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
template<typename _Tp, typename _Alloc>
  template<typename... _Args>
    void
    vector<_Tp, _Alloc>::
    emplace_back(_Args&&... __args)
    {
	if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
	  {
	    this->_M_impl.construct(this->_M_impl._M_finish,
				    tmos::forward<_Args>(__args)...);
	    ++this->_M_impl._M_finish;
	  }
	else
	  _M_insert_aux(end(), tmos::forward<_Args>(__args)...);
    }
#endif

template<typename _Tp, typename _Alloc>
typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::insert(
		iterator __position, const value_type& __x)
{
	const size_type __n = __position - begin();
	if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage
			&& __position == end())
	{
		this->_M_impl.construct(this->_M_impl._M_finish, __x);
		++this->_M_impl._M_finish;
	}
	else
	{
#ifdef __GXX_EXPERIMENTAL_CXX0X__
		if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
		{
			_Tp __x_copy = __x;
			_M_insert_aux(__position, tmos::move(__x_copy));
		}
		else
#endif
		_M_insert_aux(__position, __x);
	}
	return iterator(this->_M_impl._M_start + __n);
}

template<typename _Tp, typename _Alloc>
typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::erase(
		iterator __position)
{
	if (__position + 1 != end())
		_GLIBCXX_MOVE3(__position + 1, end(), __position);
	--this->_M_impl._M_finish;
	this->_M_impl.destroy(this->_M_impl._M_finish);
	return __position;
}

template<typename _Tp, typename _Alloc>
typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::erase(
		iterator __first, iterator __last)
{
	if (__first != __last)
	{
		if (__last != end())
			_GLIBCXX_MOVE3(__last, end(), __first);
		_M_erase_at_end(__first.base() + (end() - __last));
	}
	return __first;
}

template<typename _Tp, typename _Alloc>
vector<_Tp, _Alloc>&
vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
	if (&__x != this)
	{
		const size_type __xlen = __x.size();
		if (__xlen > capacity())
		{
			pointer __tmp = _M_allocate_and_copy(__xlen, __x.begin(),
					__x.end());
			tmos::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
					_M_get_Tp_allocator());
			_M_deallocate(this->_M_impl._M_start,
					this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
			this->_M_impl._M_start = __tmp;
			this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __xlen;
		}
		else if (size() >= __xlen)
		{
			tmos::_Destroy(tmos::copy(__x.begin(), __x.end(), begin()), end(),
					_M_get_Tp_allocator());
		}
		else
		{
			tmos::copy(__x._M_impl._M_start, __x._M_impl._M_start + size(),
					this->_M_impl._M_start);
			tmos::__uninitialized_copy_a(__x._M_impl._M_start + size(),
					__x._M_impl._M_finish, this->_M_impl._M_finish,
					_M_get_Tp_allocator());
		}
		this->_M_impl._M_finish = this->_M_impl._M_start + __xlen;
	}
	return *this;
}

template<typename _Tp, typename _Alloc>
void vector<_Tp, _Alloc>::_M_fill_assign(size_t __n, const value_type& __val)
{
	if (__n > capacity())
	{
		vector __tmp(__n, __val, _M_get_Tp_allocator());
		__tmp.swap(*this);
	}
	else if (__n > size())
	{
		tmos::fill(begin(), end(), __val);
		tmos::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - size(),
				__val, _M_get_Tp_allocator());
		this->_M_impl._M_finish += __n - size();
	}
	else
		_M_erase_at_end(tmos::fill_n(this->_M_impl._M_start, __n, __val));
}

template<typename _Tp, typename _Alloc>
template<typename _InputIterator>
void vector<_Tp, _Alloc>::_M_assign_aux(_InputIterator __first,
		_InputIterator __last, tmos::input_iterator_tag)
{
	pointer __cur(this->_M_impl._M_start);
	for (; __first != __last && __cur != this->_M_impl._M_finish;
			++__cur, ++__first)
		*__cur = *__first;
	if (__first == __last)
		_M_erase_at_end(__cur);
	else
		insert(end(), __first, __last);
}

template<typename _Tp, typename _Alloc>
template<typename _ForwardIterator>
void vector<_Tp, _Alloc>::_M_assign_aux(_ForwardIterator __first,
		_ForwardIterator __last, tmos::forward_iterator_tag)
{
	const size_type __len = tmos::distance(__first, __last);

	if (__len > capacity())
	{
		pointer __tmp(_M_allocate_and_copy(__len, __first, __last));
		tmos::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
				_M_get_Tp_allocator());
		_M_deallocate(this->_M_impl._M_start,
				this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
		this->_M_impl._M_start = __tmp;
		this->_M_impl._M_finish = this->_M_impl._M_start + __len;
		this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
	}
	else if (size() >= __len)
		_M_erase_at_end(tmos::copy(__first, __last, this->_M_impl._M_start));
	else
	{
		_ForwardIterator __mid = __first;
		tmos::advance(__mid, size());
		tmos::copy(__first, __mid, this->_M_impl._M_start);
		this->_M_impl._M_finish = tmos::__uninitialized_copy_a(__mid, __last,
				this->_M_impl._M_finish, _M_get_Tp_allocator());
	}
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
template<typename _Tp, typename _Alloc>
template<typename... _Args>
typename vector<_Tp, _Alloc>::iterator
vector<_Tp, _Alloc>::
emplace(iterator __position, _Args&&... __args)
{
	const size_type __n = __position - begin();
	if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage
			&& __position == end())
	{
		this->_M_impl.construct(this->_M_impl._M_finish,
				tmos::forward<_Args>(__args)...);
		++this->_M_impl._M_finish;
	}
	else
	_M_insert_aux(__position, tmos::forward<_Args>(__args)...);
	return iterator(this->_M_impl._M_start + __n);
}

template<typename _Tp, typename _Alloc>
template<typename... _Args>
void
vector<_Tp, _Alloc>::
_M_insert_aux(iterator __position, _Args&&... __args)
#else
template<typename _Tp, typename _Alloc>
void vector<_Tp, _Alloc>::_M_insert_aux(iterator __position, const _Tp& __x)
#endif
{
	if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
	{
		this->_M_impl.construct(this->_M_impl._M_finish,
				_GLIBCXX_MOVE(*(this->_M_impl._M_finish - 1)));
		++this->_M_impl._M_finish;
#ifndef __GXX_EXPERIMENTAL_CXX0X__
		_Tp __x_copy = __x;
#endif
		_GLIBCXX_MOVE_BACKWARD3(__position.base(), this->_M_impl._M_finish - 2,
				this->_M_impl._M_finish - 1);
#ifndef __GXX_EXPERIMENTAL_CXX0X__
		*__position = __x_copy;
#else
		*__position = _Tp(tmos::forward<_Args>(__args)...);
#endif
	}
	else
	{
		const size_type __len = _M_check_len(size_type(1),
				"vector::_M_insert_aux");
		const size_type __elems_before = __position - begin();
		pointer __new_start(this->_M_allocate(__len));
		pointer __new_finish(__new_start);
		__try
		{
			// The order of the three operations is dictated by the C++0x
			// case, where the moves could alter a new element belonging
			// to the existing vector.  This is an issue only for callers
			// taking the element by const lvalue ref (see 23.1/13).
			this->_M_impl.construct(__new_start + __elems_before,
#ifdef __GXX_EXPERIMENTAL_CXX0X__
					tmos::forward<_Args>(__args)...);
#else
					__x);
#endif
			__new_finish = 0;

			__new_finish = tmos::__uninitialized_move_a(this->_M_impl._M_start,
					__position.base(), __new_start, _M_get_Tp_allocator());
			++__new_finish;

			__new_finish = tmos::__uninitialized_move_a(__position.base(),
					this->_M_impl._M_finish, __new_finish,
					_M_get_Tp_allocator());
		}
		__catch(...)
		{
			if (!__new_finish)
				this->_M_impl.destroy(__new_start + __elems_before);
			else
				tmos::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
			_M_deallocate(__new_start, __len);
			__throw_exception_again;
		}
		tmos::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
				_M_get_Tp_allocator());
		_M_deallocate(this->_M_impl._M_start,
				this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
		this->_M_impl._M_start = __new_start;
		this->_M_impl._M_finish = __new_finish;
		this->_M_impl._M_end_of_storage = __new_start + __len;
	}
}

template<typename _Tp, typename _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n,
		const value_type& __x)
{
	if (__n != 0)
	{
		if (size_type(this->_M_impl._M_end_of_storage - this->_M_impl._M_finish)
				>= __n)
		{
			value_type __x_copy = __x;
			const size_type __elems_after = end() - __position;
			pointer __old_finish(this->_M_impl._M_finish);
			if (__elems_after > __n)
			{
				tmos::__uninitialized_move_a(this->_M_impl._M_finish - __n,
						this->_M_impl._M_finish, this->_M_impl._M_finish,
						_M_get_Tp_allocator());
				this->_M_impl._M_finish += __n;
				_GLIBCXX_MOVE_BACKWARD3(__position.base(), __old_finish - __n,
						__old_finish);
				tmos::fill(__position.base(), __position.base() + __n, __x_copy);
			}
			else
			{
				tmos::__uninitialized_fill_n_a(this->_M_impl._M_finish,
						__n - __elems_after, __x_copy, _M_get_Tp_allocator());
				this->_M_impl._M_finish += __n - __elems_after;
				tmos::__uninitialized_move_a(__position.base(), __old_finish,
						this->_M_impl._M_finish, _M_get_Tp_allocator());
				this->_M_impl._M_finish += __elems_after;
				tmos::fill(__position.base(), __old_finish, __x_copy);
			}
		}
		else
		{
			const size_type __len = _M_check_len(__n, "vector::_M_fill_insert");
			const size_type __elems_before = __position - begin();
			pointer __new_start(this->_M_allocate(__len));
			pointer __new_finish(__new_start);
			__try
			{
				// See _M_insert_aux above.
				tmos::__uninitialized_fill_n_a(__new_start + __elems_before, __n,
						__x, _M_get_Tp_allocator());
				__new_finish = 0;

				__new_finish = tmos::__uninitialized_move_a(
						this->_M_impl._M_start, __position.base(), __new_start,
						_M_get_Tp_allocator());
				__new_finish += __n;

				__new_finish = tmos::__uninitialized_move_a(__position.base(),
						this->_M_impl._M_finish, __new_finish,
						_M_get_Tp_allocator());
			}
			__catch(...)
			{
				if (!__new_finish)
					tmos::_Destroy(__new_start + __elems_before,
							__new_start + __elems_before + __n,
							_M_get_Tp_allocator());
				else
					tmos::_Destroy(__new_start, __new_finish,
							_M_get_Tp_allocator());
				_M_deallocate(__new_start, __len);
				__throw_exception_again;
			}
			tmos::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
					_M_get_Tp_allocator());
			_M_deallocate(this->_M_impl._M_start,
					this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
			this->_M_impl._M_start = __new_start;
			this->_M_impl._M_finish = __new_finish;
			this->_M_impl._M_end_of_storage = __new_start + __len;
		}
	}
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
template<typename _Tp, typename _Alloc>
void
vector<_Tp, _Alloc>::
_M_default_append(size_type __n)
{
	if (__n != 0)
	{
		if (size_type(this->_M_impl._M_end_of_storage
						- this->_M_impl._M_finish) >= __n)
		{
			tmos::__uninitialized_default_n_a(this->_M_impl._M_finish,
					__n, _M_get_Tp_allocator());
			this->_M_impl._M_finish += __n;
		}
		else
		{
			const size_type __len =
			_M_check_len(__n, "vector::_M_default_append");
			const size_type __old_size = this->size();
			pointer __new_start(this->_M_allocate(__len));
			pointer __new_finish(__new_start);
			__try
			{
				__new_finish =
				tmos::__uninitialized_move_a(this->_M_impl._M_start,
						this->_M_impl._M_finish,
						__new_start,
						_M_get_Tp_allocator());
				tmos::__uninitialized_default_n_a(__new_finish, __n,
						_M_get_Tp_allocator());
				__new_finish += __n;
			}
			__catch(...)
			{
				tmos::_Destroy(__new_start, __new_finish,
						_M_get_Tp_allocator());
				_M_deallocate(__new_start, __len);
				__throw_exception_again;
			}
			tmos::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
					_M_get_Tp_allocator());
			_M_deallocate(this->_M_impl._M_start,
					this->_M_impl._M_end_of_storage
					- this->_M_impl._M_start);
			this->_M_impl._M_start = __new_start;
			this->_M_impl._M_finish = __new_finish;
			this->_M_impl._M_end_of_storage = __new_start + __len;
		}
	}
}
#endif

template<typename _Tp, typename _Alloc>
template<typename _InputIterator>
void vector<_Tp, _Alloc>::_M_range_insert(iterator __pos,
		_InputIterator __first, _InputIterator __last, tmos::input_iterator_tag)
{
	for (; __first != __last; ++__first)
	{
		__pos = insert(__pos, *__first);
		++__pos;
	}
}

template<typename _Tp, typename _Alloc>
template<typename _ForwardIterator>
void vector<_Tp, _Alloc>::_M_range_insert(iterator __position,
		_ForwardIterator __first, _ForwardIterator __last,
		tmos::forward_iterator_tag)
{
	if (__first != __last)
	{
		const size_type __n = tmos::distance(__first, __last);
		if (size_type(this->_M_impl._M_end_of_storage - this->_M_impl._M_finish)
				>= __n)
		{
			const size_type __elems_after = end() - __position;
			pointer __old_finish(this->_M_impl._M_finish);
			if (__elems_after > __n)
			{
				tmos::__uninitialized_move_a(this->_M_impl._M_finish - __n,
						this->_M_impl._M_finish, this->_M_impl._M_finish,
						_M_get_Tp_allocator());
				this->_M_impl._M_finish += __n;
				_GLIBCXX_MOVE_BACKWARD3(__position.base(), __old_finish - __n,
						__old_finish);
				tmos::copy(__first, __last, __position);
			}
			else
			{
				_ForwardIterator __mid = __first;
				tmos::advance(__mid, __elems_after);
				tmos::__uninitialized_copy_a(__mid, __last,
						this->_M_impl._M_finish, _M_get_Tp_allocator());
				this->_M_impl._M_finish += __n - __elems_after;
				tmos::__uninitialized_move_a(__position.base(), __old_finish,
						this->_M_impl._M_finish, _M_get_Tp_allocator());
				this->_M_impl._M_finish += __elems_after;
				tmos::copy(__first, __mid, __position);
			}
		}
		else
		{
			const size_type __len = _M_check_len(__n,
					"vector::_M_range_insert");
			pointer __new_start(this->_M_allocate(__len));
			pointer __new_finish(__new_start);
			__try
			{
				__new_finish = tmos::__uninitialized_move_a(
						this->_M_impl._M_start, __position.base(), __new_start,
						_M_get_Tp_allocator());
				__new_finish = tmos::__uninitialized_copy_a(__first, __last,
						__new_finish, _M_get_Tp_allocator());
				__new_finish = tmos::__uninitialized_move_a(__position.base(),
						this->_M_impl._M_finish, __new_finish,
						_M_get_Tp_allocator());
			}
			__catch(...)
			{
				tmos::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
				_M_deallocate(__new_start, __len);
				__throw_exception_again;
			}
			tmos::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
					_M_get_Tp_allocator());
			_M_deallocate(this->_M_impl._M_start,
					this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
			this->_M_impl._M_start = __new_start;
			this->_M_impl._M_finish = __new_finish;
			this->_M_impl._M_end_of_storage = __new_start + __len;
		}
	}
}


// vector<bool>

template<typename _Alloc>
void vector<bool, _Alloc>::reserve(size_type __n)
{
	if (__n > this->max_size())
		__throw_length_error(__N("vector::reserve"));
	if (this->capacity() < __n)
	{
		_Bit_type* __q = this->_M_allocate(__n);
		this->_M_impl._M_finish = _M_copy_aligned(begin(), end(),
				iterator(__q, 0));
		this->_M_deallocate();
		this->_M_impl._M_start = iterator(__q, 0);
		this->_M_impl._M_end_of_storage = (__q
				+ (__n + int(_S_word_bit) - 1) / int(_S_word_bit));
	}
}

template<typename _Alloc>
void vector<bool, _Alloc>::_M_fill_insert(iterator __position, size_type __n,
		bool __x)
{
	if (__n == 0)
		return;
	if (capacity() - size() >= __n)
	{
		tmos::copy_backward(__position, end(),
				this->_M_impl._M_finish + difference_type(__n));
		tmos::fill(__position, __position + difference_type(__n), __x);
		this->_M_impl._M_finish += difference_type(__n);
	}
	else
	{
		const size_type __len = _M_check_len(__n,
				"vector<bool>::_M_fill_insert");
		_Bit_type * __q = this->_M_allocate(__len);
		iterator __i = _M_copy_aligned(begin(), __position, iterator(__q, 0));
		tmos::fill(__i, __i + difference_type(__n), __x);
		this->_M_impl._M_finish = tmos::copy(__position, end(),
				__i + difference_type(__n));
		this->_M_deallocate();
		this->_M_impl._M_end_of_storage = (__q
				+ ((__len + int(_S_word_bit) - 1) / int(_S_word_bit)));
		this->_M_impl._M_start = iterator(__q, 0);
	}
}

template<typename _Alloc>
template<typename _ForwardIterator>
void vector<bool, _Alloc>::_M_insert_range(iterator __position,
		_ForwardIterator __first, _ForwardIterator __last,
		tmos::forward_iterator_tag)
{
	if (__first != __last)
	{
		size_type __n = tmos::distance(__first, __last);
		if (capacity() - size() >= __n)
		{
			tmos::copy_backward(__position, end(),
					this->_M_impl._M_finish + difference_type(__n));
			tmos::copy(__first, __last, __position);
			this->_M_impl._M_finish += difference_type(__n);
		}
		else
		{
			const size_type __len = _M_check_len(__n,
					"vector<bool>::_M_insert_range");
			_Bit_type * __q = this->_M_allocate(__len);
			iterator __i = _M_copy_aligned(begin(), __position,
					iterator(__q, 0));
			__i = tmos::copy(__first, __last, __i);
			this->_M_impl._M_finish = tmos::copy(__position, end(), __i);
			this->_M_deallocate();
			this->_M_impl._M_end_of_storage = (__q
					+ ((__len + int(_S_word_bit) - 1) / int(_S_word_bit)));
			this->_M_impl._M_start = iterator(__q, 0);
		}
	}
}

template<typename _Alloc>
void vector<bool, _Alloc>::_M_insert_aux(iterator __position, bool __x)
{
	if (this->_M_impl._M_finish._M_p != this->_M_impl._M_end_of_storage)
	{
		tmos::copy_backward(__position, this->_M_impl._M_finish,
				this->_M_impl._M_finish + 1);
		*__position = __x;
		++this->_M_impl._M_finish;
	}
	else
	{
		const size_type __len = _M_check_len(size_type(1),
				"vector<bool>::_M_insert_aux");
		_Bit_type * __q = this->_M_allocate(__len);
		iterator __i = _M_copy_aligned(begin(), __position, iterator(__q, 0));
		*__i++ = __x;
		this->_M_impl._M_finish = tmos::copy(__position, end(), __i);
		this->_M_deallocate();
		this->_M_impl._M_end_of_storage = (__q
				+ ((__len + int(_S_word_bit) - 1) / int(_S_word_bit)));
		this->_M_impl._M_start = iterator(__q, 0);
	}
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
template<typename _Alloc>
  size_t
  hash<_GLIBCXX_STD_C::vector<bool, _Alloc>>::
  operator()(const _GLIBCXX_STD_C::vector<bool, _Alloc>& __b) const
  {
    size_t __hash = 0;
    using _GLIBCXX_STD_C::_S_word_bit;
    using _GLIBCXX_STD_C::_Bit_type;

    const size_t __words = __b.size() / _S_word_bit;
    if (__words)
	{
	  const size_t __clength = __words * sizeof(_Bit_type);
	  __hash = tmos::_Hash_impl::hash(__b._M_impl._M_start._M_p, __clength);
	}

    const size_t __extrabits = __b.size() % _S_word_bit;
    if (__extrabits)
	{
	  _Bit_type __hiword = *__b._M_impl._M_finish._M_p;
	  __hiword &= ~((~static_cast<_Bit_type>(0)) << __extrabits);

	  const size_t __clength
	    = (__extrabits + __CHAR_BIT__ - 1) / __CHAR_BIT__;
	  if (__words)
	    __hash = tmos::_Hash_impl::hash(&__hiword, __clength, __hash);
	  else
	    __hash = tmos::_Hash_impl::hash(&__hiword, __clength);
	}

    return __hash;
  }
#endif // __GXX_EXPERIMENTAL_CXX0X__
} // namespace std



#endif /* TL_VECTOR_H_ */
