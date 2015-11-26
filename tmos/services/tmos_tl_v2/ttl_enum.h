/*
 * ttl_enum.h based on Better Enums, see comments bellow
 *
 *  Created on: Nov 26, 2015
 *      Author: miro
 */

#ifndef SERVICES_TMOS_TL_V2_TTL_ENUM_H_
#define SERVICES_TMOS_TL_V2_TTL_ENUM_H_



// This file is part of Better Enums, released under the BSD 2-clause license.
// See doc/LICENSE for details, or visit http://github.com/aantron/better-enums.


#include <ttl_config.h>
#include <ttl_iterator.h>
#include <ttl_algo.h>






// Higher-order preprocessor macros.

#ifdef BETTER_ENUMS_MACRO_FILE
#   include BETTER_ENUMS_MACRO_FILE
#else

#define BETTER_ENUMS__PP_MAP(macro, data, ...) \
    BETTER_ENUMS__ID( \
        BETTER_ENUMS__APPLY( \
            BETTER_ENUMS__PP_MAP_VAR_COUNT, \
            BETTER_ENUMS__PP_COUNT(__VA_ARGS__)) \
        (macro, data, __VA_ARGS__))

#define BETTER_ENUMS__PP_MAP_VAR_COUNT(count) BETTER_ENUMS__M ## count

#define BETTER_ENUMS__APPLY(macro, ...) BETTER_ENUMS__ID(macro(__VA_ARGS__))

#define BETTER_ENUMS__ID(x) x

#define BETTER_ENUMS__M1(m, d, x) m(d,0,x)
#define BETTER_ENUMS__M2(m,d,x,...) m(d,1,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M1(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M3(m,d,x,...) m(d,2,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M2(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M4(m,d,x,...) m(d,3,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M3(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M5(m,d,x,...) m(d,4,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M4(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M6(m,d,x,...) m(d,5,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M5(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M7(m,d,x,...) m(d,6,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M6(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M8(m,d,x,...) m(d,7,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M7(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M9(m,d,x,...) m(d,8,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M8(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M10(m,d,x,...) m(d,9,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M9(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M11(m,d,x,...) m(d,10,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M10(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M12(m,d,x,...) m(d,11,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M11(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M13(m,d,x,...) m(d,12,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M12(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M14(m,d,x,...) m(d,13,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M13(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M15(m,d,x,...) m(d,14,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M14(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M16(m,d,x,...) m(d,15,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M15(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M17(m,d,x,...) m(d,16,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M16(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M18(m,d,x,...) m(d,17,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M17(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M19(m,d,x,...) m(d,18,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M18(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M20(m,d,x,...) m(d,19,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M19(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M21(m,d,x,...) m(d,20,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M20(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M22(m,d,x,...) m(d,21,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M21(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M23(m,d,x,...) m(d,22,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M22(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M24(m,d,x,...) m(d,23,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M23(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M25(m,d,x,...) m(d,24,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M24(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M26(m,d,x,...) m(d,25,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M25(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M27(m,d,x,...) m(d,26,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M26(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M28(m,d,x,...) m(d,27,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M27(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M29(m,d,x,...) m(d,28,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M28(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M30(m,d,x,...) m(d,29,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M29(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M31(m,d,x,...) m(d,30,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M30(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M32(m,d,x,...) m(d,31,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M31(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M33(m,d,x,...) m(d,32,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M32(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M34(m,d,x,...) m(d,33,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M33(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M35(m,d,x,...) m(d,34,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M34(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M36(m,d,x,...) m(d,35,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M35(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M37(m,d,x,...) m(d,36,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M36(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M38(m,d,x,...) m(d,37,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M37(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M39(m,d,x,...) m(d,38,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M38(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M40(m,d,x,...) m(d,39,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M39(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M41(m,d,x,...) m(d,40,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M40(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M42(m,d,x,...) m(d,41,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M41(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M43(m,d,x,...) m(d,42,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M42(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M44(m,d,x,...) m(d,43,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M43(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M45(m,d,x,...) m(d,44,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M44(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M46(m,d,x,...) m(d,45,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M45(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M47(m,d,x,...) m(d,46,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M46(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M48(m,d,x,...) m(d,47,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M47(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M49(m,d,x,...) m(d,48,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M48(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M50(m,d,x,...) m(d,49,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M49(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M51(m,d,x,...) m(d,50,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M50(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M52(m,d,x,...) m(d,51,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M51(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M53(m,d,x,...) m(d,52,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M52(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M54(m,d,x,...) m(d,53,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M53(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M55(m,d,x,...) m(d,54,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M54(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M56(m,d,x,...) m(d,55,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M55(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M57(m,d,x,...) m(d,56,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M56(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M58(m,d,x,...) m(d,57,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M57(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M59(m,d,x,...) m(d,58,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M58(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M60(m,d,x,...) m(d,59,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M59(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M61(m,d,x,...) m(d,60,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M60(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M62(m,d,x,...) m(d,61,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M61(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M63(m,d,x,...) m(d,62,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M62(m,d,__VA_ARGS__))
#define BETTER_ENUMS__M64(m,d,x,...) m(d,63,x) \
    BETTER_ENUMS__ID(BETTER_ENUMS__M63(m,d,__VA_ARGS__))

#define BETTER_ENUMS__PP_COUNT_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10,   \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, \
    _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, \
    _56, _57, _58, _59, _60, _61, _62, _63, _64, count, ...) count

#define BETTER_ENUMS__PP_COUNT(...) \
    BETTER_ENUMS__ID(BETTER_ENUMS__PP_COUNT_IMPL(__VA_ARGS__, 64, 63, 62, 61,  \
        60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43,\
        42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25,\
        24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6,\
        5, 4, 3, 2, 1))

#define BETTER_ENUMS__ITERATE(X, f, l) X(f, l, 0) X(f, l, 1) X(f, l, 2)        \
    X(f, l, 3) X(f, l, 4) X(f, l, 5) X(f, l, 6) X(f, l, 7) X(f, l, 8)          \
    X(f, l, 9) X(f, l, 10) X(f, l, 11) X(f, l, 12) X(f, l, 13) X(f, l, 14)     \
    X(f, l, 15) X(f, l, 16) X(f, l, 17) X(f, l, 18) X(f, l, 19) X(f, l, 20)    \
    X(f, l, 21) X(f, l, 22) X(f, l, 23)

#endif // #ifdef BETTER_ENUMS_MACRO_FILE else case



namespace better_enums {


// Optional type.

template <typename T>
constexpr inline T _default()
{
    return (typename T::_enumerated)0;
}

template <>
constexpr inline const char* _default<const char*>()
{
    return nullptr;
}

template <>
constexpr inline ttl::size_t _default<ttl::size_t>()
{
    return 0;
}

template <typename T>
struct optional
{
	constexpr optional() :    _valid(false), _value(_default<T>()) { }

	constexpr optional(T v) : _valid(true), _value(v) { }

	constexpr const T& operator *() const { return _value; }
	constexpr const T* operator ->() const { return &_value; }

	constexpr operator bool() const { return _valid; }

	constexpr const T& value() const { return _value; }

  private:
    bool    _valid;
    T       _value;
};

template <typename CastTo, typename Element>
constexpr static optional<CastTo>
_map_index(const Element *array, optional<ttl::size_t> index)
{
    return index ? (CastTo)array[*index] : optional<CastTo>();
}



template <typename T>
constexpr static T* _or_null(optional<T*> maybe)
{
    return maybe ? *maybe : nullptr;
}



// Functional sequencing. This is essentially a comma operator wrapped in a
// constexpr function. g++ 4.7 doesn't "accept" integral constants in the second
// position for the comma operator, and emits an external symbol, which then
// causes a linking error.

template <typename T, typename U>
constexpr U
continue_with(T ignored, U value) { return value; }



// Values array declaration helper.

template <typename EnumType>
struct _eat_assign {
    explicit constexpr _eat_assign(EnumType value) : _value(value)
        { }

    template <typename Any>
    constexpr const _eat_assign&
    operator =(Any dummy) const { return *this; }

    constexpr operator EnumType () const { return _value; }

  private:
    EnumType    _value;
};



// Iterables.

template <typename Element>
struct _Iterable {
    typedef const Element*  iterator;

    constexpr iterator begin() const { return iterator(_array); }
    constexpr iterator end() const
        { return iterator(_array + _size); }
    constexpr ttl::size_t size() const { return _size; }
    constexpr const Element& operator [](ttl::size_t index) const
        { return _array[index]; }

    constexpr _Iterable(const Element *array, ttl::size_t s) :
        _array(array), _size(s) { }

  private:
    const Element * const   _array;
    const ttl::size_t       _size;
};



// String routines.

constexpr static const char       *_name_enders = "= \t\n";

constexpr inline bool _ends_name(char c, ttl::size_t index = 0)
{
    return
        c == _name_enders[index] ? true  :
        _name_enders[index] == '\0' ? false :
        _ends_name(c, index + 1);
}

constexpr inline bool _has_initializer(const char *s,
                                                     ttl::size_t index = 0)
{
    return
        s[index] == '\0' ? false :
        s[index] == '=' ? true :
        _has_initializer(s, index + 1);
}

constexpr inline ttl::size_t
_constant_length(const char *s, ttl::size_t index = 0)
{
    return _ends_name(s[index]) ? index : _constant_length(s, index + 1);
}

constexpr inline char
_select(const char *from, ttl::size_t from_length, ttl::size_t index)
{
    return index >= from_length ? '\0' : from[index];
}

constexpr inline char _to_lower_ascii(char c)
{
    return c >= 0x41 && c <= 0x5A ? (char)(c + 0x20) : c;
}

constexpr inline bool _names_match(const char *stringizedName,
                                                 const char *referenceName,
                                                 ttl::size_t index = 0)
{
    return
        _ends_name(stringizedName[index]) ? referenceName[index] == '\0' :
        referenceName[index] == '\0' ? false :
        stringizedName[index] != referenceName[index] ? false :
        _names_match(stringizedName, referenceName, index + 1);
}

constexpr inline bool
_names_match_nocase(const char *stringizedName, const char *referenceName,
                    ttl::size_t index = 0)
{
    return
        _ends_name(stringizedName[index]) ? referenceName[index] == '\0' :
        referenceName[index] == '\0' ? false :
        _to_lower_ascii(stringizedName[index]) !=
            _to_lower_ascii(referenceName[index]) ? false :
        _names_match_nocase(stringizedName, referenceName, index + 1);
}

inline void _trim_names(const char * const *raw_names,
                        const char **trimmed_names,
                        char *storage, ttl::size_t count)
{
    ttl::size_t     offset = 0;

    for (ttl::size_t index = 0; index < count; ++index)
    {
        trimmed_names[index] = storage + offset;

        ttl::size_t trimmed_length = ::strcspn(raw_names[index], _name_enders);
        storage[offset + trimmed_length] = '\0';

        ttl::size_t raw_length = ::strlen(raw_names[index]);
        offset += raw_length + 1;
    }
}



// Eager initialization.
template <typename Enum>
struct _initialize_at_program_start {
    _initialize_at_program_start() { Enum::initialize(); }
};

} // namespace better_enums



// Array generation macros.

#define BETTER_ENUMS__EAT_ASSIGN_SINGLE(EnumType, index, expression)           \
    ((::better_enums::_eat_assign<EnumType>)EnumType::expression),

#define BETTER_ENUMS__EAT_ASSIGN(EnumType, ...)                                \
    BETTER_ENUMS__ID(                                                          \
        BETTER_ENUMS__PP_MAP(                                                  \
            BETTER_ENUMS__EAT_ASSIGN_SINGLE, EnumType, __VA_ARGS__))




#define BETTER_ENUMS__SELECT_SINGLE_CHARACTER(from, from_length, index)        \
    ::better_enums::_select(from, from_length, index),

#define BETTER_ENUMS__SELECT_CHARACTERS(from, from_length)                     \
    BETTER_ENUMS__ITERATE(                                                     \
        BETTER_ENUMS__SELECT_SINGLE_CHARACTER, from, from_length)



#define BETTER_ENUMS__TRIM_SINGLE_STRING(ignored, index, expression)           \
constexpr ttl::size_t   _length_ ## index =                                    \
    ::better_enums::_constant_length(#expression);                             \
constexpr const char    _trimmed_ ## index [] =                                \
    { BETTER_ENUMS__SELECT_CHARACTERS(#expression, _length_ ## index) };       \
constexpr const char    *_final_ ## index =                                    \
    ::better_enums::_has_initializer(#expression) ?                            \
        _trimmed_ ## index : #expression;

#define BETTER_ENUMS__TRIM_STRINGS(...)                                        \
    BETTER_ENUMS__ID(                                                          \
        BETTER_ENUMS__PP_MAP(                                                  \
            BETTER_ENUMS__TRIM_SINGLE_STRING, ignored, __VA_ARGS__))



#define BETTER_ENUMS__REFER_TO_SINGLE_STRING(ignored, index, expression)       \
    _final_ ## index,

#define BETTER_ENUMS__REFER_TO_STRINGS(...)                                    \
    BETTER_ENUMS__ID(                                                          \
        BETTER_ENUMS__PP_MAP(                                                  \
            BETTER_ENUMS__REFER_TO_SINGLE_STRING, ignored, __VA_ARGS__))



#define BETTER_ENUMS__STRINGIZE_SINGLE(ignored, index, expression)  #expression,

#define BETTER_ENUMS__STRINGIZE(...)                                           \
    BETTER_ENUMS__ID(                                                          \
        BETTER_ENUMS__PP_MAP(                                                  \
            BETTER_ENUMS__STRINGIZE_SINGLE, ignored, __VA_ARGS__))

#define BETTER_ENUMS__RESERVE_STORAGE_SINGLE(ignored, index, expression)       \
    #expression ","

#define BETTER_ENUMS__RESERVE_STORAGE(...)                                     \
    BETTER_ENUMS__ID(                                                          \
        BETTER_ENUMS__PP_MAP(                                                  \
            BETTER_ENUMS__RESERVE_STORAGE_SINGLE, ignored, __VA_ARGS__))



// The enums proper.

#define BETTER_ENUMS__NS(EnumType)  better_enums::_data_ ## EnumType


#define BETTER_ENUMS__TYPE(SetUnderlyingType, SwitchType, GenerateSwitchType,  \
                           GenerateStrings, ToStringConstexpr,                 \
                           DeclareInitialize, DefineInitialize, CallInitialize,\
                           Enum, Underlying, ...)                              \
                                                                               \
namespace better_enums {                                                       \
namespace _data_ ## Enum {                                                     \
                                                                               \
BETTER_ENUMS__ID(GenerateSwitchType(Underlying, __VA_ARGS__))                  \
                                                                               \
}                                                                              \
}                                                                              \
                                                                               \
class Enum {                                                                   \
  private:                                                                     \
    typedef ::better_enums::optional<Enum>                  _optional;         \
    typedef ::better_enums::optional<ttl::size_t>           _optional_index;   \
                                                                               \
  public:                                                                      \
    typedef Underlying                                      _integral;         \
                                                                               \
    enum _enumerated SetUnderlyingType(Underlying) { __VA_ARGS__ };            \
                                                                               \
				constexpr Enum(_enumerated value) : _value(value) { }        \
                                                                               \
		constexpr operator SwitchType(Enum)() const                  \
    {                                                                          \
        return SwitchType(Enum)(_value);                                       \
    }                                                                          \
                                                                               \
				constexpr _integral _to_integral() const;                    \
	constexpr static Enum                                        \
    _from_integral_unchecked(_integral value);                                 \
    constexpr static _optional                                   \
    _from_integral_nothrow(_integral value);                                   \
                                                                               \
    ToStringConstexpr const char* _to_string() const;                          \
	constexpr static _optional                                   \
    _from_string_nothrow(const char *name);                                    \
                                                                               \
	constexpr static _optional                                   \
    _from_string_nocase_nothrow(const char *name);                             \
                                                                               \
				constexpr static bool _is_valid(_integral value);            \
				constexpr static bool _is_valid(const char *name);           \
				constexpr static bool _is_valid_nocase(const char *name);    \
                                                                               \
    typedef ::better_enums::_Iterable<Enum>             _value_iterable;       \
    typedef ::better_enums::_Iterable<const char*>      _name_iterable;        \
                                                                               \
    typedef _value_iterable::iterator                   _value_iterator;       \
    typedef _name_iterable::iterator                    _name_iterator;        \
                                                                               \
				constexpr static const ttl::size_t _size_constant =          \
        BETTER_ENUMS__ID(BETTER_ENUMS__PP_COUNT(__VA_ARGS__));                 \
        constexpr static ttl::size_t _size()                         \
        { return _size_constant; }                                             \
                                                                               \
			constexpr static const char* _name();                        \
			constexpr static _value_iterable _values();                  \
    ToStringConstexpr static _name_iterable _names();                          \
                                                                               \
    _integral      _value;                                                     \
                                                                               \
                                                                               \
  private:                                                                     \
  	  Enum() { }															\
    explicit constexpr Enum(const _integral &value) :            \
        _value(value) { }                                                      \
                                                                               \
    DeclareInitialize                                                          \
                                                                               \
				constexpr static _optional_index                             \
    _from_value_loop(_integral value, ttl::size_t index = 0);                  \
    constexpr static _optional_index                             \
    _from_string_loop(const char *name, ttl::size_t index = 0);                \
    constexpr static _optional_index                             \
    _from_string_nocase_loop(const char *name, ttl::size_t index = 0);         \
                                                                               \
    friend struct ::better_enums::_initialize_at_program_start<Enum>;          \
};                                                                             \
                                                                               \
namespace better_enums {                                                       \
namespace _data_ ## Enum {                                                     \
                                                                               \
static ::better_enums::_initialize_at_program_start<Enum>                      \
                                                _force_initialization;         \
                                                                               \
enum _PutNamesInThisScopeAlso { __VA_ARGS__ };                                 \
                                                                               \
	constexpr const Enum      _value_array[] =                       \
    { BETTER_ENUMS__ID(BETTER_ENUMS__EAT_ASSIGN(Enum, __VA_ARGS__)) };         \
                                                                               \
BETTER_ENUMS__ID(GenerateStrings(Enum, __VA_ARGS__))                           \
                                                                               \
}                                                                              \
}                                                                              \
                                                                               \
			constexpr inline const Enum                                      \
operator +(Enum::_enumerated enumerated)                                       \
{                                                                              \
    return (Enum)enumerated;                                                   \
}                                                                              \
                                                                               \
		constexpr inline Enum::_optional_index                           \
Enum::_from_value_loop(Enum::_integral value, ttl::size_t index)               \
{                                                                              \
    return                                                                     \
        index == _size() ?                                                     \
            _optional_index() :                                                \
            BETTER_ENUMS__NS(Enum)::_value_array[index]._value == value ?      \
                _optional_index(index) :                                       \
                _from_value_loop(value, index + 1);                            \
}                                                                              \
                                                                               \
		constexpr inline Enum::_optional_index                           \
Enum::_from_string_loop(const char *name, ttl::size_t index)                   \
{                                                                              \
    return                                                                     \
        index == _size() ? _optional_index() :                                 \
        ::better_enums::_names_match(                                          \
            BETTER_ENUMS__NS(Enum)::_raw_names()[index], name) ?               \
            _optional_index(index) :                                           \
            _from_string_loop(name, index + 1);                                \
}                                                                              \
                                                                               \
			constexpr inline Enum::_optional_index                           \
Enum::_from_string_nocase_loop(const char *name, ttl::size_t index)            \
{                                                                              \
    return                                                                     \
        index == _size() ? _optional_index() :                                 \
            ::better_enums::_names_match_nocase(                               \
                BETTER_ENUMS__NS(Enum)::_raw_names()[index], name) ?           \
                    _optional_index(index) :                                   \
                    _from_string_nocase_loop(name, index + 1);                 \
}                                                                              \
                                                                               \
			constexpr inline Enum::_integral Enum::_to_integral() const      \
{                                                                              \
    return _integral(_value);                                                  \
}                                                                              \
                                                                               \
			constexpr inline Enum                                            \
Enum::_from_integral_unchecked(_integral value)                                \
{                                                                              \
    return (_enumerated)value;                                                 \
}                                                                              \
                                                                               \
			constexpr inline Enum::_optional                                 \
Enum::_from_integral_nothrow(_integral value)                                  \
{                                                                              \
    return                                                                     \
        ::better_enums::_map_index<Enum>(BETTER_ENUMS__NS(Enum)::_value_array, \
                                         _from_value_loop(value));             \
}                                                                              \
                                                                               \
                                                                               \
ToStringConstexpr inline const char* Enum::_to_string() const                  \
{                                                                              \
    return                                                                     \
        ::better_enums::_or_null(                                              \
            ::better_enums::_map_index<const char*>(                           \
                BETTER_ENUMS__NS(Enum)::_name_array(),                         \
                _from_value_loop(CallInitialize(_value))));                    \
}                                                                              \
                                                                               \
			constexpr inline Enum::_optional                                 \
Enum::_from_string_nothrow(const char *name)                                   \
{                                                                              \
    return                                                                     \
        ::better_enums::_map_index<Enum>(                                      \
            BETTER_ENUMS__NS(Enum)::_value_array, _from_string_loop(name));    \
}                                                                              \
                                                                               \
                                                                               \
			constexpr inline Enum::_optional                                 \
Enum::_from_string_nocase_nothrow(const char *name)                            \
{                                                                              \
    return                                                                     \
        ::better_enums::_map_index<Enum>(BETTER_ENUMS__NS(Enum)::_value_array, \
                                         _from_string_nocase_loop(name));      \
}                                                                              \
                                                                               \
                                                                               \
			constexpr inline bool Enum::_is_valid(_integral value)           \
{                                                                              \
    return _from_value_loop(value);                                            \
}                                                                              \
                                                                               \
			constexpr inline bool Enum::_is_valid(const char *name)          \
{                                                                              \
    return _from_string_loop(name);                                            \
}                                                                              \
                                                                               \
			constexpr inline bool Enum::_is_valid_nocase(const char *name)   \
{                                                                              \
    return _from_string_nocase_loop(name);                                     \
}                                                                              \
                                                                               \
			constexpr inline const char* Enum::_name()                       \
{                                                                              \
    return #Enum;                                                              \
}                                                                              \
                                                                               \
			constexpr inline Enum::_value_iterable Enum::_values()           \
{                                                                              \
    return _value_iterable(BETTER_ENUMS__NS(Enum)::_value_array, _size());     \
}                                                                              \
                                                                               \
ToStringConstexpr inline Enum::_name_iterable Enum::_names()                   \
{                                                                              \
    return                                                                     \
        _name_iterable(BETTER_ENUMS__NS(Enum)::_name_array(),                  \
                       CallInitialize(_size()));                               \
}                                                                              \
                                                                               \
DefineInitialize(Enum)                                                         \
                                                                               \
			constexpr inline bool operator ==(const Enum &a, const Enum &b)  \
    { return a._to_integral() == b._to_integral(); }                           \
	constexpr inline bool operator !=(const Enum &a, const Enum &b)  \
    { return a._to_integral() != b._to_integral(); }                           \
	constexpr inline bool operator <(const Enum &a, const Enum &b)   \
    { return a._to_integral() < b._to_integral(); }                            \
	constexpr inline bool operator <=(const Enum &a, const Enum &b)  \
    { return a._to_integral() <= b._to_integral(); }                           \
	constexpr inline bool operator >(const Enum &a, const Enum &b)   \
    { return a._to_integral() > b._to_integral(); }                            \
	constexpr inline bool operator >=(const Enum &a, const Enum &b)  \
    { return a._to_integral() >= b._to_integral(); }



// Enum feature options.

// C++98, C++11
#define BETTER_ENUMS__CXX98_UNDERLYING_TYPE(Underlying)

// C++11
#define BETTER_ENUMS__CXX11_UNDERLYING_TYPE(Underlying)                        \
    : Underlying

// C++98, C++11
#define BETTER_ENUMS__REGULAR_ENUM_SWITCH_TYPE(Type)                           \
    _enumerated

// C++11
#define BETTER_ENUMS__ENUM_CLASS_SWITCH_TYPE(Type)                             \
    BETTER_ENUMS__NS(Type)::_EnumClassForSwitchStatements

// C++98, C++11
#define BETTER_ENUMS__REGULAR_ENUM_SWITCH_TYPE_GENERATE(Underlying, ...)

// C++11
#define BETTER_ENUMS__ENUM_CLASS_SWITCH_TYPE_GENERATE(Underlying, ...)         \
    enum class _EnumClassForSwitchStatements : Underlying { __VA_ARGS__ };

// C++98
#define BETTER_ENUMS__CXX98_TRIM_STRINGS_ARRAYS(Enum, ...)                     \
    inline const char** _raw_names()                                           \
    {                                                                          \
        static const char   *value[] =                                         \
            { BETTER_ENUMS__ID(BETTER_ENUMS__STRINGIZE(__VA_ARGS__)) };        \
        return value;                                                          \
    }                                                                          \
                                                                               \
    inline char* _name_storage()                                               \
    {                                                                          \
        static char         storage[] =                                        \
            BETTER_ENUMS__ID(BETTER_ENUMS__RESERVE_STORAGE(__VA_ARGS__));      \
        return storage;                                                        \
    }                                                                          \
                                                                               \
    inline const char** _name_array()                                          \
    {                                                                          \
        static const char   *value[Enum::_size_constant];                      \
        return value;                                                          \
    }                                                                          \
                                                                               \
    inline bool& _initialized()                                                \
    {                                                                          \
        static bool         value = false;                                     \
        return value;                                                          \
    }

// C++11 fast version
#define BETTER_ENUMS__CXX11_PARTIAL_CONSTEXPR_TRIM_STRINGS_ARRAYS(Enum, ...)   \
    constexpr const char    *_the_raw_names[] =                                \
        { BETTER_ENUMS__ID(BETTER_ENUMS__STRINGIZE(__VA_ARGS__)) };            \
                                                                               \
    constexpr const char * const * _raw_names()                                \
    {                                                                          \
        return _the_raw_names;                                                 \
    }                                                                          \
                                                                               \
    inline char* _name_storage()                                               \
    {                                                                          \
        static char         storage[] =                                        \
            BETTER_ENUMS__ID(BETTER_ENUMS__RESERVE_STORAGE(__VA_ARGS__));      \
        return storage;                                                        \
    }                                                                          \
                                                                               \
    inline const char** _name_array()                                          \
    {                                                                          \
        static const char   *value[Enum::_size_constant];                      \
        return value;                                                          \
    }                                                                          \
                                                                               \
    inline bool& _initialized()                                                \
    {                                                                          \
        static bool         value = false;                                     \
        return value;                                                          \
    }

// C++11 slow all-constexpr version
#define BETTER_ENUMS__CXX11_FULL_CONSTEXPR_TRIM_STRINGS_ARRAYS(Enum, ...)      \
    BETTER_ENUMS__ID(BETTER_ENUMS__TRIM_STRINGS(__VA_ARGS__))                  \
                                                                               \
    constexpr const char * const    _the_name_array[] =                        \
        { BETTER_ENUMS__ID(BETTER_ENUMS__REFER_TO_STRINGS(__VA_ARGS__)) };     \
                                                                               \
    constexpr const char * const * _name_array()                               \
    {                                                                          \
        return _the_name_array;                                                \
    }                                                                          \
                                                                               \
    constexpr const char * const * _raw_names()                                \
    {                                                                          \
        return _the_name_array;                                                \
    }



// C++98, C++11 fast version
#define BETTER_ENUMS__DO_DECLARE_INITIALIZE                                    \
    static int initialize();

// C++11 slow all-constexpr version
#define BETTER_ENUMS__DECLARE_EMPTY_INITIALIZE                                 \
    static int initialize() { return 0; }

// C++98, C++11 fast version
#define BETTER_ENUMS__DO_DEFINE_INITIALIZE(Enum)                               \
    inline int Enum::initialize()                                              \
    {                                                                          \
        if (BETTER_ENUMS__NS(Enum)::_initialized())                            \
            return 0;                                                          \
                                                                               \
        ::better_enums::_trim_names(BETTER_ENUMS__NS(Enum)::_raw_names(),      \
                                    BETTER_ENUMS__NS(Enum)::_name_array(),     \
                                    BETTER_ENUMS__NS(Enum)::_name_storage(),   \
                                    _size());                                  \
                                                                               \
        BETTER_ENUMS__NS(Enum)::_initialized() = true;                         \
                                                                               \
        return 0;                                                              \
    }

// C++11 slow all-constexpr version
#define BETTER_ENUMS__DO_NOT_DEFINE_INITIALIZE(Enum)

// C++98, C++11 fast version
#define BETTER_ENUMS__DO_CALL_INITIALIZE(value)                                \
    ::better_enums::continue_with(initialize(), value)

// C++11 slow all-constexpr version
#define BETTER_ENUMS__DO_NOT_CALL_INITIALIZE(value)                            \
    value



// User feature selection.

#ifdef BETTER_ENUMS_STRICT_CONVERSION
#   define BETTER_ENUMS__DEFAULT_SWITCH_TYPE                                   \
        BETTER_ENUMS__ENUM_CLASS_SWITCH_TYPE
#   define BETTER_ENUMS__DEFAULT_SWITCH_TYPE_GENERATE                          \
        BETTER_ENUMS__ENUM_CLASS_SWITCH_TYPE_GENERATE
#else
#   define BETTER_ENUMS__DEFAULT_SWITCH_TYPE                                   \
        BETTER_ENUMS__REGULAR_ENUM_SWITCH_TYPE
#   define BETTER_ENUMS__DEFAULT_SWITCH_TYPE_GENERATE                          \
        BETTER_ENUMS__REGULAR_ENUM_SWITCH_TYPE_GENERATE
#endif



#   define BETTER_ENUMS__DEFAULT_TRIM_STRINGS_ARRAYS                           \
        BETTER_ENUMS__CXX11_FULL_CONSTEXPR_TRIM_STRINGS_ARRAYS
#   define BETTER_ENUMS__DEFAULT_DECLARE_INITIALIZE                            \
        BETTER_ENUMS__DECLARE_EMPTY_INITIALIZE
#   define BETTER_ENUMS__DEFAULT_DEFINE_INITIALIZE                             \
        BETTER_ENUMS__DO_NOT_DEFINE_INITIALIZE
#   define BETTER_ENUMS__DEFAULT_CALL_INITIALIZE                               \
        BETTER_ENUMS__DO_NOT_CALL_INITIALIZE



// Top-level macros.

#define BETTER_ENUM(Enum, Underlying, ...)                                     \
    BETTER_ENUMS__ID(BETTER_ENUMS__TYPE(                                       \
        BETTER_ENUMS__CXX11_UNDERLYING_TYPE,                                   \
        BETTER_ENUMS__DEFAULT_SWITCH_TYPE,                                     \
        BETTER_ENUMS__DEFAULT_SWITCH_TYPE_GENERATE,                            \
        BETTER_ENUMS__DEFAULT_TRIM_STRINGS_ARRAYS,                             \
		constexpr,                               \
        BETTER_ENUMS__DEFAULT_DECLARE_INITIALIZE,                              \
        BETTER_ENUMS__DEFAULT_DEFINE_INITIALIZE,                               \
        BETTER_ENUMS__DEFAULT_CALL_INITIALIZE,                                 \
        Enum, Underlying, __VA_ARGS__))

#define SLOW_ENUM(Enum, Underlying, ...)                                       \
    BETTER_ENUMS__ID(BETTER_ENUMS__TYPE(                                       \
        BETTER_ENUMS__CXX11_UNDERLYING_TYPE,                                   \
        BETTER_ENUMS__DEFAULT_SWITCH_TYPE,                                     \
        BETTER_ENUMS__DEFAULT_SWITCH_TYPE_GENERATE,                            \
        BETTER_ENUMS__CXX11_FULL_CONSTEXPR_TRIM_STRINGS_ARRAYS,                \
		constexpr,                             \
        BETTER_ENUMS__DECLARE_EMPTY_INITIALIZE,                                \
        BETTER_ENUMS__DO_NOT_DEFINE_INITIALIZE,                                \
        BETTER_ENUMS__DO_NOT_CALL_INITIALIZE,                                  \
        Enum, Underlying, __VA_ARGS__))






#endif /* SERVICES_TMOS_TL_V2_TTL_ENUM_H_ */
