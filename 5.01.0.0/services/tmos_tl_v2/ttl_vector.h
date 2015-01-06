/*
 * ttl_vector.h
 *
 *  Created on: Dec 4, 2014
 *      Author: miro
 */

#ifndef TTL_VECTOR_H_
#define TTL_VECTOR_H_

#include <ttl_config.h>
#include <ttl_iterator.h>
#include <ttl_algo.h>


namespace ttl
{
/*******************************************************************************
 * vector
 ******************************************************************************/
template<typename T>
struct vector
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

  	pointer			p_data;
  	size_type		m_capacity;
  	size_type		m_size;
public:
	//------------------------ construct & destruct --------------------------//
    vector(): p_data(nullptr), m_capacity(0), m_size(0) {};
    ~vector()
    {
    	if(p_data)
    	{
            destroy (begin(), end());
            delete (char*)p_data;
    	}
    }


	//----------------------------- iterators --------------------------------//
    inline iterator		begin()			{ return iterator (p_data);		}
    inline iterator		end()			{ return iterator (p_data + m_size);}
    inline reverse_iterator	rbegin()	{ return reverse_iterator (end());	}
    inline reverse_iterator	rend()		{ return reverse_iterator (begin());}

	//-------------------------- const iterators -----------------------------//
    inline const_iterator	begin () const	{ return const_iterator (p_data);	}
    inline const_iterator	end () const	{ return const_iterator (p_data + m_size);	}
    inline const_iterator	cbegin () const	{ return begin(); }
    inline const_iterator	cend () const	{ return end(); }
    inline const_reverse_iterator	rbegin() const	{ return const_reverse_iterator (end());	}
    inline const_reverse_iterator	rend() const	{ return const_reverse_iterator (begin());	}
    inline const_reverse_iterator	crbegin() const	{ return rbegin(); }
    inline const_reverse_iterator	crend() const	{ return rend(); }

	//----------------------------- capacity ---------------------------------//
    inline size_t		reserve (size_type n);
    inline size_t		resize (size_type n);
    inline size_type 	capacity () const	{ return m_capacity;}
    inline size_type	size () const		{ return m_size;	}
    inline size_type	max_size () const	{ return RAM_SIZE / sizeof(T);	}
    inline bool			empty () const		{ return !m_size;	}

	//-------------------------- element access ------------------------------//
    inline iterator			data ()			{ return p_data; }
    inline const_iterator	data () const	{ return p_data; }
    inline const_iterator	cdata () const	{ return p_data; }
    inline reference		at (size_type i)		{ ASSERT (i < size()); return begin()[i]; }
    inline const_reference	at (size_type i) const		{ ASSERT (i < size()); return begin()[i]; }
    inline reference		operator[] (size_type i)	{ return at (i); }
    inline const_reference	operator[] (size_type i) const	{ return at (i); }
    inline reference		front ()		{ return at(0); }
    inline const_reference	front () const	{ return at(0); }
    inline reference		back ()			{ ASSERT (!empty()); return end()[-1]; }
    inline const_reference	back () const	{ ASSERT (!empty()); return end()[-1]; }

	//----------------------------- modifiers --------------------------------//
    inline void			assign (const_iterator i1, const_iterator i2);
    inline void			assign (size_type n, const T& v);
    inline iterator		push_back (const T& v = T());
    inline void			pop_back();
    inline void			clear ()	{ destroy (begin(), end()); m_size=0; }
    inline void			swap (vector& v);
    inline iterator		insert (const_iterator ip, const T& v);
    inline iterator		insert (const_iterator ip, size_type n, const T& v);
    inline iterator		insert (const_iterator ip, const_iterator i1, const_iterator i2);
    inline iterator		erase (const_iterator ep, size_type n = 1);
    inline iterator		erase (const_iterator ep1, const_iterator ep2);

	//----------------------------- observers --------------------------------//

	//----------------------------- operators --------------------------------//
    inline const vector&	operator= (const vector& v);



private:
    inline iterator		insert_hole(const_iterator ip, size_type n);
};


/// Copies contents of v.
template <typename T>
inline const vector<T>& vector<T>::operator= (const vector<T>& v)
{
    assign (v.begin(), v.end());
    return (*this);
}

/// Copies the range [i1, i2)
template <typename T>
inline void vector<T>::assign(const_iterator i1, const_iterator i2)
{
    resize (distance (i1, i2));
    ::ttl::copy (i1, i2, begin());
}

/// Copies n elements with value v.
template <typename T>
inline void vector<T>::assign(size_type n, const T& v)
{
    resize (n);
    ::ttl::fill_n (begin(), n, v);
}

/// Allocates space for at least \p n elements.
template <typename T>
inline size_t vector<T>::reserve(size_type n)
{
    if (n > m_capacity)
    {
    	pointer new_data = (pointer) ::operator new (n * sizeof(T));
    	if(new_data)
    	{
    		if(p_data)
    		{
        		if(m_size)
        			memcpy(new_data, p_data, m_size * sizeof(T));
    			free(p_data);
    		}
    		p_data = new_data;
    	    m_capacity = n;
    	}
    }
    return m_capacity;
}

/// Resizes the vector to contain n elements.
template <typename T>
inline size_t vector<T>::resize(size_type n)
{
    if (m_size != n)
    {
        if (m_size < n)
        {
            if (m_capacity < n)
            	n = reserve (n);

            if(n)
            	construct(end(), end() + n - m_size);
        } else
        {
            destroy(begin()+n, end());
        }
        m_size = n;
    }
    return m_size;
}

/// Inserts value v at the end of the vector.
template <typename T>
inline typename vector<T>::iterator vector<T>::push_back(const T& v)
{
	iterator d= end();
	size_type n = m_size+1;

	if(reserve(n)>=n)
	{
		d = end();
	    construct(d, v);
		m_size = n;
	}
    return (d);
}

/// Removes value at the end of the vector.
template <typename T>
inline void vector<T>::pop_back()
{
	if(m_size)
	{
		m_size--;
		destroy (end());
	}
}

/// Exchanges the content by the content of v
template <typename T>
inline void vector<T>::swap(vector& v)
{
	::ttl::swap(p_data, v.p_data);
	::ttl::swap(m_capacity, v.m_capacity);
	::ttl::swap(m_size, v.m_size);
}

/// Inserts n uninitialized elements at  ip.
template <typename T>
inline typename vector<T>::iterator vector<T>::insert_hole(const_iterator ip, size_type n)
{
	iterator d= NULL;

	if(ip >= p_data)
	{
		size_type pos;

		pos = ip - p_data;
		if(pos <= m_size)
		{
			size_type nsz = m_size+n;

			if(reserve(nsz) >= nsz)
			{
				if(pos < m_size)
				{
					::memmove(ip+n, ip, (m_size-pos)*sizeof(T));
				}
				m_size = nsz;
				d = p_data +n;
			}
		}
	}
    return (d);
}


/// Inserts value v at offset ip.
template <typename T>
inline typename vector<T>::iterator vector<T>::insert(const_iterator ip, const T& v)
{
    iterator d = insert_hole(ip, 1);
	if(d)
	    construct(d, v);
    return (d);
}

/// Inserts n elements with value v at offsets ip.
template <typename T>
inline typename vector<T>::iterator vector<T>::insert(const_iterator ip, size_type n, const T& v)
{
    iterator d = insert_hole (ip, n);
    if(d)
    {
    	while(n--)
    	    construct(d+n, v);
    }
    return (d);
}

/// Inserts range [i1, i2] at offset ip.
template <typename T>
inline typename vector<T>::iterator vector<T>::insert (const_iterator ip, const_iterator i1, const_iterator i2)
{
	iterator d = NULL;
	size_type n;

	if(i1 <= i2)
	{
		n = i2 - i1;
		d = insert_hole(ip, n);
		if(d)
		    uninitialized_copy (i1, i2, d);
	}
    return (d);
}

/// Removes count elements at offset ep.
template <typename T>
inline typename vector<T>::iterator vector<T>::erase (const_iterator ep, size_type n)
{
    iterator d = const_cast<iterator>(ep);
	size_type nsz;
    if( n && d && d < end())
    {
    	nsz = end() - d;
    	if(n > nsz)
    		n = nsz;
    	if(n)
    	{
            destroy (d, d+n);
            m_size -= n;
            nsz -= n;
            if(nsz)
            	::memmove(d, d+n, nsz*sizeof(T));
    	}
    }
    return (d);
}

/// Removes elements from ep1 toep2.
template <typename T>
inline typename vector<T>::iterator vector<T>::erase (const_iterator ep1, const_iterator ep2)
{
    return (erase (ep1, ep2-ep1));
}


} // namespace ttl




#endif /* TTL_VECTOR_H_ */
