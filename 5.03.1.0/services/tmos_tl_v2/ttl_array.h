/*
 * ttl_array.h
 *
 *  Created on: Mar 21, 2013
 *      Author: miro
 */

#ifndef TTL_ARRAY_H_
#define TTL_ARRAY_H_

#include <ttl_config.h>
#include <ttl_iterator.h>
#include <ttl_algo.h>


namespace ttl
{
/*******************************************************************************
 * array
 ******************************************************************************/
template<typename T, ttl::size_t _Nm>
struct array
{
  	typedef T 										value_type;
  	typedef T* 										pointer;
  	typedef const T* 								const_pointer;
  	typedef value_type& 							reference;
  	typedef const value_type& 						const_reference;
  	typedef pointer		 							iterator;
  	typedef const_pointer	 						const_iterator;
  	typedef ttl::size_t 							size_type;
  	typedef ttl::ptrdiff_t 							difference_type;
  	typedef ttl::reverse_iterator<iterator> 		reverse_iterator;
  	typedef ttl::reverse_iterator<const_iterator> 	const_reverse_iterator;

	static value_type dummy_item;

	// Support for zero-sized arrays mandatory.
	value_type _m_data[_Nm ? _Nm : 1];

	//------------------------ construct & destruct --------------------------//

	//----------------------------- iterators --------------------------------//
	iterator 		 begin()	{ return iterator(_m_data);	}
	iterator		 end()		{ return iterator(&_m_data[_Nm]); }
	reverse_iterator rbegin()	{ return reverse_iterator(end()); }
	reverse_iterator rend()		{ return reverse_iterator(begin());	}


	//-------------------------- const iterators -----------------------------//
	const_iterator begin() const 	{ return const_iterator(_m_data); }
	const_iterator end() const		{ return const_iterator(_m_data); }
	const_iterator cbegin() const	{ return const_iterator(_m_data); }
	const_iterator cend() const		{ return const_iterator(_m_data); }
	const_reverse_iterator rbegin()  const { return const_reverse_iterator(end()); }
	const_reverse_iterator rend() 	 const { return const_reverse_iterator(begin()); }
	const_reverse_iterator crbegin() const { return const_reverse_iterator(end());	}
	const_reverse_iterator crend() 	 const { return const_reverse_iterator(begin()); }

	//----------------------------- capacity ---------------------------------//
	constexpr size_type size() 		const { return _Nm; }
	constexpr size_type max_size()	const { return _Nm; }
	constexpr bool 		empty() 	const { return _Nm == 0; }

	//-------------------------- element access ------------------------------//
	reference 		front()			{ return at(0); }
	const_reference front() const	{ return at(0); }
	reference 		back()			{ return at(_Nm -1); }
	const_reference back() 	const	{ return at(_Nm -1); }
	reference operator[](size_type n)
	{
		return at(n);
	}

	const_reference operator[](size_type n) const
	{
		return at(n);
	}

	reference at(size_type __n)
	{
		if (__n >= _Nm)
		{
			TTL_THROW(EXCEPT_OUT_OF_RANGE);
			return dummy_item;
		}
		return _m_data[__n];
	}

	const_reference at(size_type __n) const
	{
		if (__n >= _Nm)
		{
			TTL_THROW(EXCEPT_OUT_OF_RANGE);
			return dummy_item;
		}
		return _m_data[__n];
	}

	//----------------------------- modifiers --------------------------------//
	void fill(const value_type& __u)
	{
		ttl::fill_n(begin(), size(), __u);
	}

	void swap(array& __other)
	{
		ttl::swap_ranges(begin(), end(), __other.begin());
	}

	//----------------------------- observers --------------------------------//
	T* data()
	{
		return _m_data;
	}

	const T* data() const
	{
		return _m_data;
	}
};

template<typename T, ttl::size_t _Nm>
T array<T, _Nm> ::dummy_item;

} // namespace ttl


#endif /* TTL_ARRAY_H_ */
