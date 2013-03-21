/*
 * tl_allocator.h
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

#ifndef TL_ALLOCATOR_H_
#define TL_ALLOCATOR_H_

#include <tmos++.h>

namespace __tmos_cxx
{
using tmos::size_t;
using tmos::ptrdiff_t;

/**
 *  @brief  An allocator that uses global new, as per [20.4].
 *  @ingroup allocators
 *
 *  This is precisely the allocator defined in the C++ Standard.
 *    - all allocation calls operator new
 *    - all deallocation calls operator delete
 */
template<typename _Tp>
  class new_allocator
  {
  public:
    typedef size_t     size_type;
    typedef ptrdiff_t  difference_type;
    typedef _Tp*       pointer;
    typedef const _Tp* const_pointer;
    typedef _Tp&       reference;
    typedef const _Tp& const_reference;
    typedef _Tp        value_type;

    template<typename _Tp1>
      struct rebind
      { typedef new_allocator<_Tp1> other; };

    new_allocator() throw() { }

    new_allocator(const new_allocator&) throw() { }

    template<typename _Tp1>
      new_allocator(const new_allocator<_Tp1>&) throw() { }

    ~new_allocator() throw() { }

    pointer
    address(reference __x) const { return tmos::__addressof(__x); }

    const_pointer
    address(const_reference __x) const { return tmos::__addressof(__x); }

    // NB: __n is permitted to be 0.  The C++ standard says nothing
    // about what the return value is when __n == 0.
    pointer
    allocate(size_type __n, const void* = 0)
    {
	if (__n > this->max_size())
	  __throw_bad_alloc();

	return static_cast<_Tp*>(::operator new(__n * sizeof(_Tp)));
    }

    // __p is not permitted to be a null pointer.
    void
    deallocate(pointer __p, size_type)
    { ::operator delete(__p); }

    size_type
    max_size() const throw()
    { return size_t(-1) / sizeof(_Tp); }

    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 402. wrong new expression in [some_] allocator::construct
    void
    construct(pointer __p, const _Tp& __val)
    { ::new((void *)__p) _Tp(__val); }

    void
    destroy(pointer __p) { __p->~_Tp(); }
  };

template<typename _Tp>
  inline bool
  operator==(const new_allocator<_Tp>&, const new_allocator<_Tp>&)
  { return true; }

template<typename _Tp>
  inline bool
  operator!=(const new_allocator<_Tp>&, const new_allocator<_Tp>&)
  { return false; }

} // namespace __tmos_cxx

namespace tmos
{

/**
 * @defgroup allocators Allocators
 * @ingroup memory
 *
 * Classes encapsulating memory operations.
 */

template<typename _Tp>
class allocator;

/// allocator<void> specialization.
template<>
class allocator<void>
{
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef void* pointer;
	typedef const void* const_pointer;
	typedef void value_type;

	template<typename _Tp1>
	struct rebind
	{
		typedef allocator<_Tp1> other;
	};
};

/**
 * @brief  The @a standard allocator, as per [20.4].
 * @ingroup allocators
 *
 *  Further details:
 *  http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt04ch11.html
 */
template<typename _Tp>
class allocator: public __tmos_cxx::new_allocator<_Tp>
{
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef _Tp* pointer;
	typedef const _Tp* const_pointer;
	typedef _Tp& reference;
	typedef const _Tp& const_reference;
	typedef _Tp value_type;

	template<typename _Tp1>
	struct rebind
	{
		typedef allocator<_Tp1> other;
	};

	allocator() throw ()
	{
	}

	allocator(const allocator& __a) throw () :
		__tmos_cxx::new_allocator<_Tp>(__a)
	{
	}

	template<typename _Tp1>
	allocator(const allocator<_Tp1>&) throw ()
	{
	}

	~allocator() throw ()
	{
	}

	// Inherit everything else.
};

template<typename _T1, typename _T2>
inline bool operator==(const allocator<_T1>&, const allocator<_T2>&)
{
	return true;
}

template<typename _Tp>
inline bool operator==(const allocator<_Tp>&, const allocator<_Tp>&)
{
	return true;
}

template<typename _T1, typename _T2>
inline bool operator!=(const allocator<_T1>&, const allocator<_T2>&)
{
	return false;
}

template<typename _Tp>
inline bool operator!=(const allocator<_Tp>&, const allocator<_Tp>&)
{
	return false;
}

// Inhibit implicit instantiations for required instantiations,
// which are defined via explicit instantiations elsewhere.
#if _GLIBCXX_EXTERN_TEMPLATE
extern template class allocator<char>;
extern template class allocator<wchar_t>;
#endif


// To implement Option 3 of DR 431.
template<typename _Alloc, bool = __is_empty(_Alloc)>
struct __alloc_swap
{
	static void _S_do_it(_Alloc&, _Alloc&)
	{
	}
};

template<typename _Alloc>
struct __alloc_swap<_Alloc, false>
{
	static void _S_do_it(_Alloc& __one, _Alloc& __two)
	{
		// Precondition: swappable allocators.
		if (__one != __two)
			swap(__one, __two);
	}
};

// Optimize for stateless allocators.
template<typename _Alloc, bool = __is_empty(_Alloc)>
struct __alloc_neq
{
	static bool _S_do_it(const _Alloc&, const _Alloc&)
	{
		return false;
	}
};

template<typename _Alloc>
struct __alloc_neq<_Alloc, false>
{
	static bool _S_do_it(const _Alloc& __one, const _Alloc& __two)
	{
		return __one != __two;
	}
};


} // namespace tmos

#endif /* TL_ALLOCATOR_H_ */
