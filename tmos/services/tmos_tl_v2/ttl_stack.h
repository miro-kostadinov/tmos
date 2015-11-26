/*
 * ttl_stack.h
 *
 *  Created on: Nov 17, 2015
 *      Author: miro
 */

#ifndef SERVICES_TMOS_TL_V2_TTL_STACK_H_
#define SERVICES_TMOS_TL_V2_TTL_STACK_H_

#include <ttl_config.h>
#include <ttl_vector.h>


namespace ttl
{
/*******************************************************************************
 * stack
 ******************************************************************************/
template <typename T>
struct stack
{
    typedef T										value_type;
    typedef T*										pointer;
  	typedef const T* 								const_pointer;
  	typedef value_type& 							reference;
  	typedef const value_type& 						const_reference;
  	typedef ttl::size_t 							size_type;
  	typedef ttl::ptrdiff_t 							difference_type;

private:
    vector<T> m_storage;

public:
	//------------------------ construct & destruct --------------------------//
    inline				stack (void)			: m_storage () { }
    explicit inline		stack (const vector<T>& s)	: m_storage (s) { }
    explicit inline		stack (const stack& s)		: m_storage (s.m_storage) { }

	//----------------------------- capacity ---------------------------------//
    inline bool			empty (void) const		{ return (m_storage.empty()); }
    inline size_type	size (void) const		{ return (m_storage.size()); }

	//-------------------------- element access ------------------------------//
    inline reference		top (void)			{ return (m_storage.back()); }
    inline const_reference	top (void) const	{ return (m_storage.back()); }

	//----------------------------- modifiers --------------------------------//
    inline void			push (const value_type& v)	{ m_storage.push_back (v); }
    inline void			pop (void)					{ m_storage.pop_back(); }

	//----------------------------- operators --------------------------------//
    inline bool			operator== (const stack& s) const	{ return (m_storage == s.m_storage); }
    inline bool			operator< (const stack& s) const	{ return (m_storage.size() < s.m_storage.size()); }
};

} // namespace ttl



#endif /* SERVICES_TMOS_TL_V2_TTL_STACK_H_ */
