/*
 * ttl_pvector.h
 *
 *  Created on: Dec 16, 2014
 *      Author: miro
 */

#ifndef TTL_PVECTOR_H_
#define TTL_PVECTOR_H_


#include <ttl_config.h>
#include <ttl_iterator.h>
#include <ttl_algo.h>
#include <ttl_pair.h>
#include <ttl_vector.h>




namespace ttl
{
/*******************************************************************************
 * pair vector
 ******************************************************************************/
template <typename K, typename V>
struct pair_vector : public vector<pair<K,V> >
{
    typedef K										key_type;
    typedef V										data_type;
    typedef const K&								const_key_ref;
    typedef const V&								const_data_ref;
    typedef const pair_vector<K,V>&					rcself_t;
    typedef vector<pair<K,V> >						base_class;
    typedef typename base_class::value_type			value_type;
    typedef typename base_class::size_type			size_type;
    typedef typename base_class::pointer			pointer;
    typedef typename base_class::const_pointer		const_pointer;
    typedef typename base_class::reference			reference;
    typedef typename base_class::const_reference	const_reference;
    typedef typename base_class::const_iterator		const_iterator;
    typedef typename base_class::iterator			iterator;
    typedef typename base_class::reverse_iterator	reverse_iterator;
    typedef typename base_class::const_reverse_iterator	const_reverse_iterator;

	//------------------------ construct & destruct --------------------------//
    inline				pair_vector ()				: base_class() {};
    explicit inline		pair_vector(size_type n)	: base_class(n) {};

	//----------------------------- iterators --------------------------------//
    inline iterator		begin (void)		{ return (base_class::begin()); }
    inline iterator		end (void)			{ return (base_class::end()); }

	//-------------------------- const iterators -----------------------------//
    inline const_iterator	begin (void) const		{ return (base_class::begin()); }
    inline const_iterator	end (void) const		{ return (base_class::end()); }

    //----------------------------- capacity ---------------------------------//
    inline size_type		size () const		{ return (base_class::size()); }
    inline size_type		count (const_key_ref k, const_iterator i1=begin(), const_iterator i2=end()) const;

    //-------------------------- element access ------------------------------//
    inline const_data_ref	at (const_key_ref k) const	{ ASSERT (find(k) != end()); return (find(k)->second); }
    inline data_type&		at (const_key_ref k)		{ ASSERT (find(k) != end()); return (find(k)->second); }
    inline const_data_ref	operator[] (const_key_ref i) const	{ return (at(i)); }
    data_type&			operator[] (const_key_ref i);

    //----------------------------- modifiers --------------------------------//
    inline void			assign (const_iterator i1, const_iterator i2)	{ base_class::assign(i1, i2); }
    inline iterator		push_back (const_reference v)	{ return base_class::push_back(v); }
    inline void			clear (void)		{ base_class::clear(); }
    inline void			swap (pair_vector<K,V>& v)		{ base_class::swap (v); }
    iterator			insert (const_reference v);
    inline iterator		insert (const_iterator ip, const_reference v) {return base_class::insert(ip, v);};
    inline iterator		insert (const_iterator ip, const_iterator i1, const_iterator i2) {return base_class::insert(ip, i1, i2);};
    inline iterator		erase (iterator ep)		{ return (base_class::erase (ep)); }
    inline iterator		erase (iterator ep1, iterator ep2) { return (base_class::erase (ep1, ep2)); }

    //----------------------------- observers --------------------------------//

	//----------------------------- operators --------------------------------//
    inline rcself_t		operator= (rcself_t v)		{ base_class::operator= (v); return (*this); }


    inline const_iterator	find(const_key_ref k, const_iterator i1=begin(), const_iterator i2=end()) const;
    inline iterator			find(const_key_ref k, iterator i1=begin(), iterator i2=end()) ;
    inline const_iterator	find_data (const_data_ref v, const_iterator i1=begin(), const_iterator i2=end()) const;
    inline iterator			find_data (const_data_ref v, iterator i1=begin(), iterator i2=end());


};

template <typename K, typename V>
inline typename pair_vector<K,V>::size_type	pair_vector<K,V>::count(
		const_key_ref k, const_iterator i1, const_iterator i2) const
{
	size_type n=0;
    for(; i1 != i2; ++i1)
    {
    	if(k == i1->first)
    		n++;
    }
    return (n);
}

template <typename K, typename V>
typename pair_vector<K,V>::iterator	pair_vector<K,V>::insert (const_reference v)
{
	iterator d;
	d = find(v.first, begin(), end());
	if(d == end())
		d = push_back(v);
	else
		d->second = v.second;
	return (d);
}


/// Returns the pair<K,V> where K = k.
template <typename K, typename V>
inline typename pair_vector<K,V>::const_iterator pair_vector<K,V>::find(
		const_key_ref k, const_iterator i1, const_iterator i2) const
{
    for(; i1 != i2; ++i1)
    {
    	if(k == i1->first)
    		break;
    }
    return (i1);
}

/// Returns the pair<K,V> where K = k.
template <typename K, typename V>
inline typename pair_vector<K,V>::iterator pair_vector<K,V>::find(
		const_key_ref k, iterator i1, iterator i2)
{
    for(; i1 != i2; ++i1)
    {
    	if(k == i1->first)
    		break;
    }
    return (i1);
}

template <typename K, typename V>
inline typename pair_vector<K,V>::const_iterator pair_vector<K,V>::find_data(
		const_data_ref v, const_iterator i1, const_iterator i2) const
{
    for(; i1 != i2; ++i1)
    {
    	if(v == i1->second)
    		break;
    }
    return (i1);
}

template <typename K, typename V>
inline typename pair_vector<K,V>::iterator pair_vector<K,V>::find_data(
		const_data_ref v, iterator i1, iterator i2)
{
    for(; i1 != i2; ++i1)
    {
    	if(v == i1->second)
    		break;
    }
    return (i1);
}


} // namespace ttl

#endif /* TTL_PVECTOR_H_ */
