/*
 * tl_cpp_type_traits.h
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

#ifndef TL_CPP_TYPE_TRAITS_H_
#define TL_CPP_TYPE_TRAITS_H_


#include <tmos++.h>

namespace tmos
{

struct __true_type { };
struct __false_type { };

template<bool>
  struct __truth_type
  { typedef __false_type __type; };

template<>
  struct __truth_type<true>
  { typedef __true_type __type; };

// N.B. The conversions to bool are needed due to the issue
// explained in c++/19404.
template<class _Sp, class _Tp>
  struct __traitor
  {
    enum { __value = bool(_Sp::__value) || bool(_Tp::__value) };
    typedef typename __truth_type<__value>::__type __type;
  };

// Compare for equality of types.
template<typename, typename>
  struct __are_same
  {
    enum { __value = 0 };
    typedef __false_type __type;
  };

template<typename _Tp>
  struct __are_same<_Tp, _Tp>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

// Holds if the template-argument is a void type.
template<typename _Tp>
  struct __is_void
  {
    enum { __value = 0 };
    typedef __false_type __type;
  };

template<>
  struct __is_void<void>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

//
// Integer types
//
template<typename _Tp>
  struct __is_integer
  {
    enum { __value = 0 };
    typedef __false_type __type;
  };

// Thirteen specializations (yes there are eleven standard integer
// types; <em>long long</em> and <em>unsigned long long</em> are
// supported as extensions)
template<>
  struct __is_integer<bool>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

template<>
  struct __is_integer<char>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

template<>
  struct __is_integer<signed char>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

template<>
  struct __is_integer<unsigned char>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

template<>
  struct __is_integer<short>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

template<>
  struct __is_integer<unsigned short>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

template<>
  struct __is_integer<int>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

template<>
  struct __is_integer<unsigned int>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

template<>
  struct __is_integer<long>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

template<>
  struct __is_integer<unsigned long>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

template<>
  struct __is_integer<long long>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

template<>
  struct __is_integer<unsigned long long>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

//
// Floating point types
//
template<typename _Tp>
  struct __is_floating
  {
    enum { __value = 0 };
    typedef __false_type __type;
  };

// three specializations (float, double and 'long double')
template<>
  struct __is_floating<float>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

template<>
  struct __is_floating<double>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

template<>
  struct __is_floating<long double>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

//
// Pointer types
//
template<typename _Tp>
  struct __is_pointer
  {
    enum { __value = 0 };
    typedef __false_type __type;
  };

template<typename _Tp>
  struct __is_pointer<_Tp*>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

//
// Normal iterator type
//
template<typename _Tp>
  struct __is_normal_iterator
  {
    enum { __value = 0 };
    typedef __false_type __type;
  };

//template<typename _Iterator, typename _Container>
//  struct __is_normal_iterator< __gnu_cxx::__normal_iterator<_Iterator,
//							      _Container> >
//  {
//    enum { __value = 1 };
//    typedef __true_type __type;
//  };

//
// An arithmetic type is an integer type or a floating point type
//
template<typename _Tp>
  struct __is_arithmetic
  : public __traitor<__is_integer<_Tp>, __is_floating<_Tp> >
  { };

//
// A fundamental type is `void' or and arithmetic type
//
template<typename _Tp>
  struct __is_fundamental
  : public __traitor<__is_void<_Tp>, __is_arithmetic<_Tp> >
  { };

//
// A scalar type is an arithmetic type or a pointer type
//
template<typename _Tp>
  struct __is_scalar
  : public __traitor<__is_arithmetic<_Tp>, __is_pointer<_Tp> >
  { };

//
// For use in std::copy and std::find overloads for streambuf iterators.
//
template<typename _Tp>
  struct __is_char
  {
    enum { __value = 0 };
    typedef __false_type __type;
  };

template<>
  struct __is_char<char>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

#ifdef TMOS_USE_WCHAR_T
template<>
  struct __is_char<wchar_t>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };
#endif

template<typename _Tp>
  struct __is_byte
  {
    enum { __value = 0 };
    typedef __false_type __type;
  };

template<>
  struct __is_byte<char>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

template<>
  struct __is_byte<signed char>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

template<>
  struct __is_byte<unsigned char>
  {
    enum { __value = 1 };
    typedef __true_type __type;
  };

//
// Move iterator type
//
template<typename _Tp>
  struct __is_move_iterator
  {
    enum { __value = 0 };
    typedef __false_type __type;
  };


} // namespace tmos

#endif /* TL_CPP_TYPE_TRAITS_H_ */
