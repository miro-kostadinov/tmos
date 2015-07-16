/*
 * ttl_pair.h
 *
 *  Created on: Dec 15, 2014
 *      Author: miro
 */

#ifndef TTL_PAIR_H_
#define TTL_PAIR_H_

#include <ttl_config.h>
#include <ttl_iterator.h>
#include <ttl_algo.h>


namespace ttl
{
/*******************************************************************************
 * pair
 ******************************************************************************/
template <typename T1, typename T2>
class pair
{
public:
    typedef T1  	first_type;
    typedef T2		second_type;

    first_type		first;
    second_type		second;
    //------------------------ construct & destruct --------------------------//

    /// Default constructor.
    inline constexpr	pair (): first (T1()), second (T2()) {};
    /// Initializes members with a, and b.
    inline		pair (const T1& a, const T2& b)	: first (a), second (b) {};

    template <typename T3, typename T4>
    inline		pair (const pair<T3,T4>& p2)	: first (p2.first), second (p2.second) {};

	//----------------------------- operators --------------------------------//
    inline pair&	operator= (const pair& p2)	{ first = p2.first; second = p2.second; return (*this); };
    template <typename T3, typename T4>
    inline pair&	operator= (const pair<T3,T4>& p2) { first = p2.first; second = p2.second; return (*this); };
    inline bool		operator== (const pair& v)const	{ return (first == v.first && second == v.second); };
    inline bool		operator< (const pair& v) const	{ return (first < v.first || (first == v.first && second < v.second)); };

    inline void		swap (pair& v)	{ ::ttl::swap(first,v.first); ::ttl::(second,v.second); };
};


/// Returns a pair object with (a,b)
template <typename T1, typename T2>
inline pair<T1,T2> make_pair (const T1& a, const T2& b)
    { return (pair<T1,T2> (a, b)); }



} // namespace ttl


#endif /* TTL_PAIR_H_ */
