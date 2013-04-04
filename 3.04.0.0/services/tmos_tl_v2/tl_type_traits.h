/*
 * tl_type_traits.h
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

#ifndef TL_TYPE_TRAITS_H_
#define TL_TYPE_TRAITS_H_

#include <tmos++.h>

namespace __tmos_cxx
{

// Define a nested type if some predicate holds.
template<bool, typename>
  struct __enable_if
  { };

template<typename _Tp>
  struct __enable_if<true, _Tp>
  { typedef _Tp __type; };


// Conditional expression for types. If true, first, if false, second.
template<bool _Cond, typename _Iftrue, typename _Iffalse>
  struct __conditional_type
  { typedef _Iftrue __type; };

template<typename _Iftrue, typename _Iffalse>
  struct __conditional_type<false, _Iftrue, _Iffalse>
  { typedef _Iffalse __type; };


// Given an integral builtin type, return the corresponding unsigned type.
template<typename _Tp>
  struct __add_unsigned
  {
  private:
    typedef __enable_if<tmos::__is_integer<_Tp>::__value, _Tp> __if_type;

  public:
    typedef typename __if_type::__type __type;
  };

template<>
  struct __add_unsigned<char>
  { typedef unsigned char __type; };

template<>
  struct __add_unsigned<signed char>
  { typedef unsigned char __type; };

template<>
  struct __add_unsigned<short>
  { typedef unsigned short __type; };

template<>
  struct __add_unsigned<int>
  { typedef unsigned int __type; };

template<>
  struct __add_unsigned<long>
  { typedef unsigned long __type; };

template<>
  struct __add_unsigned<long long>
  { typedef unsigned long long __type; };

// Declare but don't define.
template<>
  struct __add_unsigned<bool>;

template<>
  struct __add_unsigned<wchar_t>;


// Given an integral builtin type, return the corresponding signed type.
template<typename _Tp>
  struct __remove_unsigned
  {
  private:
    typedef __enable_if<tmos::__is_integer<_Tp>::__value, _Tp> __if_type;

  public:
    typedef typename __if_type::__type __type;
  };

template<>
  struct __remove_unsigned<char>
  { typedef signed char __type; };

template<>
  struct __remove_unsigned<unsigned char>
  { typedef signed char __type; };

template<>
  struct __remove_unsigned<unsigned short>
  { typedef short __type; };

template<>
  struct __remove_unsigned<unsigned int>
  { typedef int __type; };

template<>
  struct __remove_unsigned<unsigned long>
  { typedef long __type; };

template<>
  struct __remove_unsigned<unsigned long long>
  { typedef long long __type; };

// Declare but don't define.
template<>
  struct __remove_unsigned<bool>;

template<>
  struct __remove_unsigned<wchar_t>;


// For use in string and vstring.
template<typename _Type>
  inline bool
  __is_null_pointer(_Type* __ptr)
  { return __ptr == 0; }

template<typename _Type>
  inline bool
  __is_null_pointer(_Type)
  { return false; }


// For complex and cmath
template<typename _Tp, bool = tmos::__is_integer<_Tp>::__value>
  struct __promote
  { typedef double __type; };

// No nested __type member for non-integer non-floating point types,
// allows this type to be used for SFINAE to constrain overloads in
// <cmath> and <complex> to only the intended types.
template<typename _Tp>
  struct __promote<_Tp, false>
  { };

template<>
  struct __promote<long double>
  { typedef long double __type; };

template<>
  struct __promote<double>
  { typedef double __type; };

template<>
  struct __promote<float>
  { typedef float __type; };

template<typename _Tp, typename _Up,
         typename _Tp2 = typename __promote<_Tp>::__type,
         typename _Up2 = typename __promote<_Up>::__type>
  struct __promote_2
  {
    typedef __typeof__(_Tp2() + _Up2()) __type;
  };

template<typename _Tp, typename _Up, typename _Vp,
         typename _Tp2 = typename __promote<_Tp>::__type,
         typename _Up2 = typename __promote<_Up>::__type,
         typename _Vp2 = typename __promote<_Vp>::__type>
  struct __promote_3
  {
    typedef __typeof__(_Tp2() + _Up2() + _Vp2()) __type;
  };

template<typename _Tp, typename _Up, typename _Vp, typename _Wp,
         typename _Tp2 = typename __promote<_Tp>::__type,
         typename _Up2 = typename __promote<_Up>::__type,
         typename _Vp2 = typename __promote<_Vp>::__type,
         typename _Wp2 = typename __promote<_Wp>::__type>
  struct __promote_4
  {
    typedef __typeof__(_Tp2() + _Up2() + _Vp2() + _Wp2()) __type;
  };

  // ---------------- numeric traits
#define __glibcxx_signed(_Tp) ((_Tp)(-1) < 0)
#define __glibcxx_digits(_Tp) \
  (sizeof(_Tp) * __CHAR_BIT__ - __glibcxx_signed(_Tp))

#define __glibcxx_min(_Tp) \
  (__glibcxx_signed(_Tp) ? (_Tp)1 << __glibcxx_digits(_Tp) : (_Tp)0)

#define __glibcxx_max(_Tp) \
  (__glibcxx_signed(_Tp) ? \
   (((((_Tp)1 << (__glibcxx_digits(_Tp) - 1)) - 1) << 1) + 1) : ~(_Tp)0)

  template<typename _Value>
    struct __numeric_traits_integer
    {
      // Only integers for initialization of member constant.
      static const _Value __min = __glibcxx_min(_Value);
      static const _Value __max = __glibcxx_max(_Value);

      // NB: these two also available in tmos::numeric_limits as compile
      // time constants, but <limits> is big and we avoid including it.
      static const bool __is_signed = __glibcxx_signed(_Value);
      static const int __digits = __glibcxx_digits(_Value);
    };

  template<typename _Value>
    const _Value __numeric_traits_integer<_Value>::__min;

  template<typename _Value>
    const _Value __numeric_traits_integer<_Value>::__max;

  template<typename _Value>
    const bool __numeric_traits_integer<_Value>::__is_signed;

  template<typename _Value>
    const int __numeric_traits_integer<_Value>::__digits;

#undef __glibcxx_signed
#undef __glibcxx_digits
#undef __glibcxx_min
#undef __glibcxx_max

#define __glibcxx_floating(_Tp, _Fval, _Dval, _LDval) \
  (tmos::__are_same<_Tp, float>::__value ? _Fval \
   : tmos::__are_same<_Tp, double>::__value ? _Dval : _LDval)

#define __glibcxx_max_digits10(_Tp) \
  (2 + __glibcxx_floating(_Tp, __FLT_MANT_DIG__, __DBL_MANT_DIG__, \
			  __LDBL_MANT_DIG__) * 643L / 2136)

#define __glibcxx_digits10(_Tp) \
  __glibcxx_floating(_Tp, __FLT_DIG__, __DBL_DIG__, __LDBL_DIG__)

#define __glibcxx_max_exponent10(_Tp) \
  __glibcxx_floating(_Tp, __FLT_MAX_10_EXP__, __DBL_MAX_10_EXP__, \
		     __LDBL_MAX_10_EXP__)

  template<typename _Value>
    struct __numeric_traits_floating
    {
      // Only floating point types. See N1822.
      static const int __max_digits10 = __glibcxx_max_digits10(_Value);

      // See above comment...
      static const bool __is_signed = true;
      static const int __digits10 = __glibcxx_digits10(_Value);
      static const int __max_exponent10 = __glibcxx_max_exponent10(_Value);
    };

  template<typename _Value>
    const int __numeric_traits_floating<_Value>::__max_digits10;

  template<typename _Value>
    const bool __numeric_traits_floating<_Value>::__is_signed;

  template<typename _Value>
    const int __numeric_traits_floating<_Value>::__digits10;

  template<typename _Value>
    const int __numeric_traits_floating<_Value>::__max_exponent10;

  template<typename _Value>
    struct __numeric_traits
    : public __conditional_type<tmos::__is_integer<_Value>::__value,
				__numeric_traits_integer<_Value>,
				__numeric_traits_floating<_Value> >::__type
    { };






} // namespace tmos


#endif /* TL_TYPE_TRAITS_H_ */
