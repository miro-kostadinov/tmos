/*
 * tl_array.h
 *
 *  Created on: Mar 15, 2013
 *      Author: miro
 */

#ifndef TL_ARRAY_H_
#define TL_ARRAY_H_

#include <tmos++.h>
#include <tl_iterator.h>
#include <tl_algorithm.h>
#include <tl_move.h>

namespace tmos
{

  template<typename _Tp, tmos::size_t _Nm>
  struct array
  {
  	typedef _Tp 									value_type;
  	typedef _Tp* 									pointer;
  	typedef const _Tp* 								const_pointer;
  	typedef value_type& 							reference;
  	typedef const value_type& 						const_reference;
  	typedef value_type* 							iterator;
  	typedef const value_type* 						const_iterator;
  	typedef tmos::size_t 							size_type;
  	typedef tmos::ptrdiff_t 						difference_type;
  	typedef tmos::reverse_iterator<iterator> 		reverse_iterator;
  	typedef tmos::reverse_iterator<const_iterator> 	const_reverse_iterator;

  	static value_type dummy_item;

  	// Support for zero-sized arrays mandatory.
  	value_type _M_instance[_Nm ? _Nm : 1];

  	// No explicit construct/copy/destroy for aggregate type.

  	// DR 776.
  	void fill(const value_type& __u)
  	{
  		tmos::fill_n(begin(), size(), __u);
  	}

  	void swap(array& __other)
  	{
  		tmos::swap_ranges(begin(), end(), __other.begin());
  	}

  	// Iterators.
  	iterator begin()
  	{
  		return iterator(tmos::__addressof(_M_instance[0]));
  	}

  	const_iterator begin() const
  	{
  		return const_iterator(tmos::__addressof(_M_instance[0]));
  	}

  	iterator end()
  	{
  		return iterator(tmos::__addressof(_M_instance[_Nm]));
  	}

  	const_iterator end() const
  	{
  		return const_iterator(tmos::__addressof(_M_instance[_Nm]));
  	}

  	reverse_iterator rbegin()
  	{
  		return reverse_iterator(end());
  	}

  	const_reverse_iterator rbegin() const
  	{
  		return const_reverse_iterator(end());
  	}

  	reverse_iterator rend()
  	{
  		return reverse_iterator(begin());
  	}

  	const_reverse_iterator rend() const
  	{
  		return const_reverse_iterator(begin());
  	}

  	const_iterator cbegin() const
  	{
  		return const_iterator(tmos::__addressof(_M_instance[0]));
  	}

  	const_iterator cend() const
  	{
  		return const_iterator(tmos::__addressof(_M_instance[_Nm]));
  	}

  	const_reverse_iterator crbegin() const
  	{
  		return const_reverse_iterator(end());
  	}

  	const_reverse_iterator crend() const
  	{
  		return const_reverse_iterator(begin());
  	}

  	// Capacity.
  	constexpr size_type size() const
  	{	return _Nm;}

  	constexpr size_type max_size() const
  	{	return _Nm;}

  	constexpr bool empty() const
  	{
  		return size() == 0;
  	}

  	// Element access.
  	reference operator[](size_type __n)
  	{
  		return _M_instance[__n];
  	}

  	const_reference operator[](size_type __n) const
  	{
  		return _M_instance[__n];
  	}

  	reference at(size_type __n)
  	{
  		if (__n >= _Nm)
  			return dummy_item;
  		return _M_instance[__n];
  	}

  	const_reference at(size_type __n) const
  	{
  		if (__n >= _Nm)
  			return dummy_item;
  		return _M_instance[__n];
  	}

  	reference front()
  	{
  		return *begin();
  	}

  	const_reference front() const
  	{
  		return *begin();
  	}

  	reference back()
  	{
  		return _Nm ? *(end() - 1) : *end();
  	}

  	const_reference back() const
  	{
  		return _Nm ? *(end() - 1) : *end();
  	}

  	_Tp* data()
  	{
  		return tmos::__addressof(_M_instance[0]);
  	}

  	const _Tp* data() const
  	{
  		return tmos::__addressof(_M_instance[0]);
  	}
};

// Array comparisons.
template<typename _Tp, tmos::size_t _Nm>
inline bool operator==(const array<_Tp, _Nm>& __one,
		const array<_Tp, _Nm>& __two)
{
	return tmos::equal(__one.begin(), __one.end(), __two.begin());
}

template<typename _Tp, tmos::size_t _Nm>
inline bool operator!=(const array<_Tp, _Nm>& __one,
		const array<_Tp, _Nm>& __two)
{
	return !(__one == __two);
}

template<typename _Tp, tmos::size_t _Nm>
inline bool operator<(const array<_Tp, _Nm>& __a, const array<_Tp, _Nm>& __b)
{
	return tmos::lexicographical_compare(__a.begin(), __a.end(), __b.begin(),
			__b.end());
}

template<typename _Tp, tmos::size_t _Nm>
inline bool operator>(const array<_Tp, _Nm>& __one,
		const array<_Tp, _Nm>& __two)
{
	return __two < __one;
}

template<typename _Tp, tmos::size_t _Nm>
inline bool operator<=(const array<_Tp, _Nm>& __one,
		const array<_Tp, _Nm>& __two)
{
	return !(__one > __two);
}

template<typename _Tp, tmos::size_t _Nm>
inline bool operator>=(const array<_Tp, _Nm>& __one,
		const array<_Tp, _Nm>& __two)
{
	return !(__one < __two);
}

// Specialized algorithms [6.2.2.2].
template<typename _Tp, tmos::size_t _Nm>
inline void swap(array<_Tp, _Nm>& __one, array<_Tp, _Nm>& __two)
{
	__one.swap(__two);
}

// Tuple interface to class template array [6.2.2.5].

/// tuple_size
template<typename _Tp>
class tuple_size;

/// tuple_element
template<tmos::size_t _Int, typename _Tp>
class tuple_element;

template<typename _Tp, tmos::size_t _Nm>
struct tuple_size<array<_Tp, _Nm> >
{
	static const tmos::size_t value = _Nm;
};

template<typename _Tp, tmos::size_t _Nm>
const tmos::size_t tuple_size<array<_Tp, _Nm> >::value;

template<tmos::size_t _Int, typename _Tp, tmos::size_t _Nm>
struct tuple_element<_Int, array<_Tp, _Nm> >
{
	typedef _Tp type;
};

template<tmos::size_t _Int, typename _Tp, tmos::size_t _Nm>
inline _Tp&
get(array<_Tp, _Nm>& __arr)
{
	return __arr[_Int];
}

template<tmos::size_t _Int, typename _Tp, tmos::size_t _Nm>
inline const _Tp&
get(const array<_Tp, _Nm>& __arr)
{
	return __arr[_Int];
}




} // namespace tmos

#endif /* TL_ARRAY_H_ */
