/*
 * ttl_iterator.h
 *
 *  Created on: Mar 21, 2013
 *      Author: miro
 */

#ifndef TTL_ITERATOR_H_
#define TTL_ITERATOR_H_

#include <ttl_config.h>


namespace ttl
{

/*******************************************************************************
 * iterator tags
 ******************************************************************************/
///  Marking input iterators.
struct input_iterator_tag { };

///  Marking output iterators.
struct output_iterator_tag { };

/// Forward iterators support a superset of input iterator operations.
struct forward_iterator_tag : public input_iterator_tag { };

/// Bidirectional iterators support a superset of forward iterator
/// operations.
struct bidirectional_iterator_tag : public forward_iterator_tag { };

/// Random-access iterators support a superset of bidirectional
/// iterator operations.
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

/*******************************************************************************
 * iterator
 ******************************************************************************/
template<typename _Category, typename _Tp, typename _Distance = ptrdiff_t,
         typename _Pointer = _Tp*, typename _Reference = _Tp&>
  struct iterator
  {
    typedef _Category  iterator_category;
    typedef _Tp        value_type;
    typedef _Distance  difference_type;
    typedef _Pointer   pointer;
    typedef _Reference reference;
  };

/*******************************************************************************
 * Iterator Traits
 ******************************************************************************/
template<typename _Iterator>
  struct iterator_traits
  {
    typedef typename _Iterator::iterator_category iterator_category;
    typedef typename _Iterator::value_type        value_type;
    typedef typename _Iterator::difference_type   difference_type;
    typedef typename _Iterator::pointer           pointer;
    typedef typename _Iterator::reference         reference;
  };

/// Partial specialization for pointer types.
template<typename _Tp>
  struct iterator_traits<_Tp*>
  {
    typedef random_access_iterator_tag 	iterator_category;
    typedef _Tp                         value_type;
    typedef ptrdiff_t                   difference_type;
    typedef _Tp*                        pointer;
    typedef _Tp&                        reference;
  };

/// Partial specialization for const pointer types.
template<typename _Tp>
  struct iterator_traits<const _Tp*>
  {
    typedef random_access_iterator_tag 	iterator_category;
    typedef _Tp                         value_type;
    typedef ptrdiff_t                   difference_type;
    typedef const _Tp*                  pointer;
    typedef const _Tp&                  reference;
  };

/**
 *  This function is not a part of the C++ standard but is syntactic
 *  sugar for internal library use only.
*/
template<typename _Iter>
inline typename iterator_traits<_Iter>::iterator_category
	__iterator_category(const _Iter&)
{
	return typename iterator_traits<_Iter>::iterator_category();
}

// If _Iterator has a base returns it otherwise _Iterator is returned
// untouched
template<typename _Iterator, bool _HasBase>
struct _Iter_base
{
	typedef _Iterator iterator_type;
	static iterator_type _S_base(_Iterator __it)
	{
		return __it;
	}
};

template<typename _Iterator>
struct _Iter_base<_Iterator, true>
{
	typedef typename _Iterator::iterator_type iterator_type;
	static iterator_type _S_base(_Iterator __it)
	{
		return __it.base();
	}
};

/*******************************************************************************
 * Reverse iterator
 ******************************************************************************/
template<typename _Iterator>
class reverse_iterator: public iterator<
		typename iterator_traits<_Iterator>::iterator_category,
		typename iterator_traits<_Iterator>::value_type,
		typename iterator_traits<_Iterator>::difference_type,
		typename iterator_traits<_Iterator>::pointer,
		typename iterator_traits<_Iterator>::reference>
{
protected:
	_Iterator current;

	typedef iterator_traits<_Iterator> __traits_type;

public:
	typedef _Iterator iterator_type;
	typedef typename __traits_type::difference_type difference_type;
	typedef typename __traits_type::pointer pointer;
	typedef typename __traits_type::reference reference;

	//------------------------ construct & destruct --------------------------//
	/// default constructor
	reverse_iterator() :
			current()
	{
	}

	explicit reverse_iterator(iterator_type __x) :
			current(__x)
	{
	}

	///  copy constructor
	reverse_iterator(const reverse_iterator& __x) :
			current(__x.current)
	{
	}

	iterator_type base() const
	{
		return current;
	}

	reference operator*() const
	{
		_Iterator __tmp = current;
		return *--__tmp;
	}

	pointer operator->() const
	{
		return &(operator*());
	}

	reverse_iterator& operator++()
	{
		--current;
		return *this;
	}

	reverse_iterator operator++(int)
	{
		reverse_iterator __tmp = *this;
		--current;
		return __tmp;
	}

	reverse_iterator& operator--()
	{
		++current;
		return *this;
	}

	reverse_iterator operator--(int)
	{
		reverse_iterator __tmp = *this;
		++current;
		return __tmp;
	}

	reverse_iterator operator+(difference_type __n) const
	{
		return reverse_iterator(current - __n);
	}

	reverse_iterator& operator+=(difference_type __n)
	{
		current -= __n;
		return *this;
	}

	reverse_iterator operator-(difference_type __n) const
	{
		return reverse_iterator(current + __n);
	}

	reverse_iterator& operator-=(difference_type __n)
	{
		current += __n;
		return *this;
	}

	reference operator[](difference_type __n) const
	{
		return *(*this + __n);
	}
};

/*******************************************************************************
 * iterator distance
 ******************************************************************************/

template<typename _InputIterator>
inline typename iterator_traits<_InputIterator>::difference_type
__distance(	_InputIterator __first, _InputIterator __last, input_iterator_tag)
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
__distance(	_RandomAccessIterator __first, _RandomAccessIterator __last,
		random_access_iterator_tag)
{
	return __last - __first;
}

template<typename _InputIterator>
inline typename iterator_traits<_InputIterator>::difference_type
distance( _InputIterator __first, _InputIterator __last)
{
	return __distance(__first, __last, __iterator_category(__first));
}

/*******************************************************************************
 * iterator advance
 ******************************************************************************/
template<typename _InputIterator, typename _Distance>
inline void __advance(_InputIterator& __i, _Distance __n, input_iterator_tag)
{
	while (__n--)
		++__i;
}

template<typename _BidirectionalIterator, typename _Distance>
inline void __advance(_BidirectionalIterator& __i, _Distance __n, bidirectional_iterator_tag)
{
	if (__n > 0)
		while (__n--)
			++__i;
	else
		while (__n++)
			--__i;
}

template<typename _RandomAccessIterator, typename _Distance>
inline void __advance(_RandomAccessIterator& __i, _Distance __n, random_access_iterator_tag)
{
     __i += __n;
}

template<typename _InputIterator, typename _Distance>
inline void advance(_InputIterator& __i, _Distance __n)
{
	typename iterator_traits<_InputIterator>::difference_type __d = __n;

	__advance(__i, __d, __iterator_category(__i));
}

/*******************************************************************************
 * iterator next
 ******************************************************************************/

template<typename _ForwardIterator>
inline _ForwardIterator next(_ForwardIterator __x,
		typename iterator_traits<_ForwardIterator>::difference_type __n = 1)
{
	ttl::advance(__x, __n);
	return __x;
}

/*******************************************************************************
 * iterator prev
 ******************************************************************************/
template<typename _BidirectionalIterator>
inline _BidirectionalIterator prev(_BidirectionalIterator __x,
		typename iterator_traits<_BidirectionalIterator>::difference_type __n =	1)
{
	ttl::advance(__x, -__n);
	return __x;
}

} // namespace ttl



#endif /* TTL_ITERATOR_H_ */
