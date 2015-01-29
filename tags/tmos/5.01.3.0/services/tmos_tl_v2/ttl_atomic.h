/*
 * ttl_atomic.h
 *
 *  Created on: Mar 26, 2013
 *      Author: miro
 */

#ifndef TTL_ATOMIC_H_
#define TTL_ATOMIC_H_

#include <ttl_config.h>
#include <tmos_atomic.h>

namespace ttl
{

/*******************************************************************************
 * Atomic Traits
 ******************************************************************************/
template<typename T>
  struct atomic_traits
  {
    typedef typename T::value        			value;
    typedef volatile typename  T::pointer     	pointer;
  };

/// Partial specialization for atomic types.
template<>
  struct atomic_traits<char>
  {
    typedef char   				value;
    typedef volatile char*     	pointer;
  };

template<>
  struct atomic_traits<unsigned char>
  {
    typedef char   		value;
    typedef char*     	pointer;
  };

/*******************************************************************************
 * atomic add
 ******************************************************************************/
template<typename T>
static inline T atomic_fetch_add( volatile T* obj, T arg)
{
	T res;

	do{
		res = atomic_fetch( obj );
	} while (atomic_store(obj, res+arg));
	return res;
}

template<typename T>
static inline T atomic_fetch_sub(volatile T* obj, T arg)
{
	T res;

	do{
		res = atomic_fetch(obj);
	} while (atomic_store(obj, res-arg));
	return res;
}

template<typename T>
static inline T atomic_add_fetch( volatile T* obj, T arg)
{
	T res;

	do{
		res = atomic_fetch( obj );
		res += arg;
	} while (atomic_store(obj, res));
	return res;
}

template<typename T>
static inline T atomic_sub_fetch( volatile T* obj, T arg)
{
	T res;

	do{
		res = atomic_fetch( obj );
		res -= arg;
	} while (atomic_store(obj, res));
	return res;
}

template<typename T>
static inline T atomic_and_fetch(volatile T* obj, T arg)
{
	T res;

	do{
		res = atomic_fetch(obj);
		res &= arg;
	} while (atomic_store(obj, res));
	return res;
}

template<typename T>
static inline T atomic_or_fetch(volatile T* obj, T arg)
{
	T res;

	do{
		res = atomic_fetch(obj);
		res |= arg;
	} while (atomic_store(obj, res));
	return res;
}

template<typename T>
static inline T atomic_xor_fetch(volatile T* obj, T arg)
{
	T res;

	do{
		res = atomic_fetch(obj);
		res ^= arg;
	} while (atomic_store(obj, res));
	return res;
}


/*******************************************************************************
 * atomic
 ******************************************************************************/
template<typename T>
  struct atomic
  {
  private:
    volatile T _M_i;

  public:
  	//------------------------ construct & destruct --------------------------//
    atomic() = default;
    ~atomic() = default;
    atomic(const atomic&) = delete;
    constexpr atomic(T __i) : _M_i(__i) { }

  	//-------------------------- operators -----------------------------------//
    atomic& operator=(const atomic&) = delete;
    atomic& operator=(const atomic&) volatile = delete;

	operator T() const			{	return load();}

	T operator=(T __i)	{ store(__i);	return __i;	}
	T operator++(int)	{	return atomic_fetch_add(&_M_i, static_cast<T>(1));}
	T operator--(int)	{	return atomic_fetch_sub(&_M_i, static_cast<T>(1));}
	T operator++()		{	return atomic_add_fetch(&_M_i, static_cast<T>(1));}
	T operator--()		{	return atomic_sub_fetch(&_M_i, static_cast<T>(1));}
	T operator+=(T __i)	{	return atomic_add_fetch(&_M_i, __i);}
	T operator-=(T __i)	{	return atomic_sub_fetch(&_M_i, __i);}
	T operator&=(T __i)	{	return atomic_and_fetch(&_M_i, __i);}
	T operator|=(T __i)	{	return atomic_or_fetch(&_M_i, __i);}
	T operator^=(T __i)	{	return atomic_xor_fetch(&_M_i, __i);}



  	//------------------------ member functions ------------------------------//
    void store(T value)
    {
    	do{
        	atomic_fetch(&_M_i);
    	} while (atomic_store(&_M_i, value));
    }

    T load() const {return _M_i;}

    T exchange(T __i);

    bool compare_exchange(T&, T);

  };


/// Partial specialization for pointer types.
template<typename T>
struct atomic<T*>
{
private:
	typedef T* pointer;

	pointer _M_p;

public:
	atomic():_M_p(nullptr) {};
	~atomic() = default;
	atomic(const atomic &) = delete;
	atomic& operator=(const atomic&) = delete;
	atomic& operator=(const atomic&) volatile = delete;

	// Requires pointer convertible to _M_p.
	constexpr atomic(pointer __p): _M_p (__p)
	{}

	operator pointer() const	{	return load();}

	pointer operator=(pointer __p)
	{
		store(__p);
		return __p;
	}

	pointer operator++(int)	{	return fetch_add(1);}
	pointer operator--(int)	{	return fetch_sub(1);}
	pointer operator++()	{	return fetch_add(1) + 1;}
	pointer operator--()	{	return fetch_sub(1) -1;}
	pointer	operator+=(ptrdiff_t __d) {	return fetch_add(__d) + __d;}
	pointer	operator-=(ptrdiff_t __d) {	return fetch_sub(__d) - __d;}


	void store(pointer __p)
	{
		do{
			atomic_fetch(&_M_p);
		} while (atomic_store(&_M_p, __p));
	}

	pointer	load() const
	{
		return _M_p;
	}

	pointer	exchange(pointer __p)
	{
		return __sync_lock_test_and_set(&_M_p, __p);
	}

	pointer fetch_add(ptrdiff_t __d)
	{	return __sync_fetch_and_add(&_M_p, __d);}


	pointer fetch_sub(ptrdiff_t __d)
	{	return __sync_fetch_and_sub(&_M_p, __d);}

};

typedef atomic<char>			atomic_char;
typedef atomic<unsigned char>	atomic_uchar;
typedef atomic<short>			atomic_short;
typedef atomic<unsigned short>	atomic_ushort;
typedef atomic<int>				atomic_int;
typedef atomic<unsigned int>	atomic_uint;

} // namespace ttl



#endif /* TTL_ATOMIC_H_ */
