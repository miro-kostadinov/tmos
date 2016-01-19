/*
 * ttl_string.h
 *
 *  Created on: Apr 3, 2013
 *      Author: miro
 */

#ifndef TTL_STRING_H_
#define TTL_STRING_H_

#include <tmos.h>
#include <tmos_string.h>
#include <tmos_stdio.h>
#include <tmos_stdlib.h>
#include <stdarg.h>

#include <ttl_config.h>
#include <ttl_atomic.h>
#include <ttl_iterator.h>
#include <ttl_cpp_type_traits.h>
#include <ttl_initializer_list.h>
#include <ttl_move.h>

namespace ttl
{
/*******************************************************************************
 * string and memory headers
 ******************************************************************************/
/**
 *  In the TMOS memory pool each memory allocation has hidden header.
 *  We can use it when we need the allocation size.
 */
struct alloc_header
{
	unsigned short alloc_size;		//!< memory chunk size
	unsigned short alloc_prev;		//!< reserved, never use this !!
};

/**
 * The string data starts with reference counter and element count
 */
struct data_header
{
	atomic_short m_refcount;			//!< reference counter
	unsigned short m_length;			//!< number of elements
};

struct string_header : public alloc_header, public data_header
{
};

/*******************************************************************************
 * basic_string
 ******************************************************************************/
template <typename CharT>
class basic_string
{
public:
  	//----------------------------- types  -----------------------------------//
  	typedef CharT									value_type;
  	typedef value_type* 							pointer;
  	typedef const value_type*						const_pointer;
  	typedef value_type& 							reference;
  	typedef const value_type& 						const_reference;
  	typedef pointer		 							iterator;
  	typedef const_pointer	 						const_iterator;
  	typedef ttl::size_t 							size_type;
  	typedef ttl::ptrdiff_t 							difference_type;
  	typedef ttl::reverse_iterator<iterator> 		reverse_iterator;
  	typedef ttl::reverse_iterator<const_iterator> 	const_reverse_iterator;

    static const size_type	npos = static_cast<size_type>(-1);
	static CharT dummy_char; 		//!< dummy char returned as reference for out of bounds

	/// allocate storage for n chars + 1 terminator space, and set length
public:
	static pointer m_allocate(size_t n, size_t len=0)
	{
		void* p;

		if(n++)
		{
			p = ::operator new(sizeof(data_header) + n * sizeof(value_type) );
			if(!p)
				TTL_THROW(EXCEPT_OUT_OF_MEMORY);
			static_cast<data_header*>(p)->m_refcount =1;
			static_cast<data_header*>(p)->m_length = len;
			p = static_cast<data_header*>(p) + 1;
			static_cast<pointer>(p)[len] =0;
		} else
		{
			p = nullptr;
		}
		return static_cast<pointer>(p);
	}

	void m_set_size(size_type n)
	{
		m_data[n] = 0;
		string_head()->m_length = n;
	}


protected:
  	pointer m_data;

	/// return pointer to the full header (alloc_size, refs, size)
  	static string_header* string_head(pointer p)
  	{
  		return reinterpret_cast<string_header*>(reinterpret_cast<void*>(p)) - 1;
  	}

  	string_header* string_head()
  	{
  		return reinterpret_cast<string_header*>(reinterpret_cast<void*>(m_data)) - 1;
  	}

  	const string_header* string_head() const
  	{
  		return reinterpret_cast<const string_header*>(reinterpret_cast<void*>(m_data)) - 1;
  	}

  	/// return pointer for memory free() and realloc()
  	static data_header* mem_head(pointer p)
  	{
  		return reinterpret_cast<data_header*>(reinterpret_cast<void*>(p)) - 1;
  	}


  	/// capacity of memory allocated string
	static size_type m_capacity(const_pointer p)
	{
		size_type n=0;
		const string_header* head;

		if(p)
		{
			head = reinterpret_cast<const string_header*>(
							reinterpret_cast<const void*>(p)) - 1;
			n = ((head->alloc_size-2)*4)/sizeof(CharT);
			if(n)
				n--;	//terminating 0
		}

		return n;
	}

//	size_type m_add_size(size_type n)
//	{
//		size_type old_size;
//		string_header* head;
//
//		head = string_head();
//		old_size = head->m_length;
//		n= old_size+n;
//		head->m_length = n;
//		m_data[n] = 0;
//		return old_size;
//	}

	void m_memcpy(const_pointer s, size_type n, size_type pos)
	{
		pointer p = m_data + pos;
		while(n--)
			p[n] = s[n];
	}

	void m_memmove(const_pointer s, size_type n, size_type pos)
	{
		pointer p = m_data + pos;
		if(p >= s)
		{
			//move to right
			while(n--)
				p[n] = s[n];

		} else
		{
			while(n--)
			{
				p[0] = s[0];
				++p;
				++s;
			}
		}
	}

	void m_memset(value_type c, size_type n, size_type pos)
	{
		pointer p = m_data + pos;
		while(n--)
			p[n] = c;
	}

	void m_append(const CharT*s, size_type n)
	{
		m_make_editable_copy(n);
		if(m_data)
		{
			pointer p = m_data + string_head()->m_length;

			string_head()->m_length += n;
			p[n] = 0;
			while(n--)
				p[n] = s[n];
		}
	}

	void m_append(size_type sz, const CharT*s, size_type n)
	{
		if(m_reserve(sz, sz+n))
		{
			m_set_size(sz+n);
			pointer p = m_data + sz;

			while(n--)
				p[n] = s[n];
		}
	}

	static const_pointer m_find(const_pointer s, size_type n, reference __a)
	{
		for (size_type i = 0; i < n; ++i)
			if (s[i] == __a)
				return s + i;
		return 0;
	}

    static int m_compare(const_pointer s1, const_pointer s2, size_type n)
    {
    	int res = 0;
		for (size_type i = 0; i < n; ++i)
		{
			res = s1[i] - s2[i];
			if(res)
				break;
		}
		return res;
    }


    size_type m_limit(size_type pos, size_type n) const
    {
    	size_type sz = size();
    	if(pos > sz)
    	{
			TTL_THROW(EXCEPT_OUT_OF_RANGE);
    		return 0;
    	}
    	sz -= pos;
		return (n < sz) ? n : sz;
    }

protected:
  	void m_grab()
  	{
  		if (is_ram_ptr(m_data))
  			string_head()->m_refcount++;
  	}

	void m_dispose()
	{

  		if (is_ram_ptr(m_data))
  		{
  			if( --string_head()->m_refcount <= 0)
  			{
  				::operator delete(mem_head(m_data));
  			}
  		}

	}
	size_type m_reserve(size_type old_len, size_type res)
  	{
  		pointer new_data;

  		if(is_ram_ptr(m_data))
  		{
  			string_header* head = string_head();

			if ( head->m_refcount != 1
					|| res >= ((head->alloc_size - 2) * 4) / sizeof(CharT))
  			{
  	  			new_data = m_allocate(res, old_len);
  	  			if(new_data)
  	  				memcpy(new_data, m_data, old_len*sizeof(CharT));
  	  			else
  	  				res =0;
  			    m_dispose();
  			    m_data = new_data;
  			}
  		} else
  		{
			new_data = m_allocate(res, old_len);
			if (new_data)
				memcpy(new_data, m_data, old_len * sizeof(CharT));
			else
				res = 0;
			m_data = new_data;
  		}
  		return res;
  	}

  	void m_make_editable_copy(size_type res=0, bool forse_realloc=false)
  	{
  		pointer new_data;

  		if(is_ram_ptr(m_data))
  		{
  			string_header* head = string_head();
  			res += head->m_length;

			if (forse_realloc || head->m_refcount != 1
					|| res > ((head->alloc_size - 2) * 4) / sizeof(CharT))
  			{
  	  			new_data = m_allocate(res, head->m_length);
  	  			if(new_data)
  	  				memcpy(new_data, m_data, head->m_length*sizeof(CharT));
  			    m_dispose();
  			    m_data = new_data;
  			}
  		} else
  		{
  			size_type old_len = strlen(m_data);

  			res += old_len;
			new_data = m_allocate(res, old_len);
			if(new_data)
				memcpy(new_data, m_data, old_len*sizeof(CharT));
			m_data = new_data;
  		}

  	}

	template<class _InIterator>
	static CharT* m_construct_aux(_InIterator __beg, _InIterator __end, __false_type)
	{
		typedef typename iterator_traits<_InIterator>::iterator_category _Tag;
		return m_construct(__beg, __end, _Tag());
	}

	template<class _Integer>
	static CharT* m_construct_aux(_Integer __beg, _Integer __end, __true_type)
	{
		return m_construct(static_cast<size_type>(__beg), __end);
	}


	template<class _InIterator>
	static CharT* m_construct(_InIterator __beg, _InIterator __end)
	{
		typedef typename ttl::__is_integer<_InIterator>::__type _Integral;
		return m_construct_aux(__beg, __end, _Integral());
	}

	template<class _InIterator>
	static CharT* m_construct(_InIterator __beg, _InIterator __end, input_iterator_tag)
	{
		pointer p = nullptr;

		if (__beg != __end )
		{
			size_type len = 0;

			p = m_allocate(4);
			if(p == nullptr)
				return p;

			while (__beg != __end)
			{
				if(len == m_capacity(p))
				{
					p = static_cast<pointer>(tsk_realloc(mem_head(p),
							sizeof(data_header) + len * sizeof(CharT) + 8));
					if(p == nullptr)
						return p;
				}
    			p[len++] = *__beg;
    			++__beg;
			}
			string_head(p)->m_length = len;
		}
    	return p;
	}

	// For forward_iterators up to random_access_iterators, used for
	// string::iterator, CharT*, etc.
	template<class _FwdIterator>
	static CharT* m_construct(_FwdIterator __beg, _FwdIterator __end, forward_iterator_tag)
	{
		pointer p = nullptr;

		if (__beg != __end )
		{
			if (__beg == 0)
				TTL_THROW(EXCEPT_RANGE_CONSTRUCT);
			else
			{
				size_type n;

				n = static_cast<size_type>(ttl::distance(__beg,	__end));
		    	p = m_allocate(n, n);
		    	if(p)
		    	{
		    		while(n--)
		    		{
		    			p[n] = *__beg;
		    			++__beg;
		    		}
		    	}
			}
		}
    	return p;
	}

    static CharT* m_construct(size_type n, CharT c)
    {
    	pointer p = m_allocate(n, n);

    	if(p)
    		while(n--)
    			p[n] = c;

    	return p;
    }

	template<class _Integer>
	basic_string& m_replace_dispatch(size_type pos1, size_type n1, _Integer n2,
			_Integer val, __true_type)
	{	return m_replace_aux(pos1, n1, n2, val);}

	template<class _InputIterator>
	basic_string& m_replace_dispatch(size_type pos1, size_type n1,
			_InputIterator k1, _InputIterator k2, __false_type)
	{
		const basic_string s(k1, k2);
		return replace(pos1, n1, s);
	}

    basic_string& m_replace_aux(size_type pos1, size_type n1, size_type n2, CharT c);

public:
  	//------------------------ construct & destruct --------------------------//

  	/// default constructor
  	basic_string() : m_data(nullptr) { }

  	/// copy constructor
  	basic_string(const basic_string& str);

  	/// from c-string constructor
    basic_string(const CharT* s);

    /// substring constructor
    basic_string(const basic_string& str, size_t pos, size_t len = npos);

    /// from buffer
    basic_string(const char* s, size_t n);

    /// fill constructor
    basic_string(size_type n, CharT c);

    /// range constructor
    template <class InputIterator>
      basic_string(InputIterator first, InputIterator last);

    /// initializer list
    basic_string(std::initializer_list<CharT> il);

    ///  move constructor
    basic_string(basic_string&& str) noexcept : m_data(str.m_data)
    {
    	str.m_data = nullptr;
    }

    /// destructor
    ~basic_string() { m_dispose(); }

  	//----------------------------- operator = -------------------------------//

    /// copy operator
    basic_string& operator= (const basic_string& str)
    { return this->assign(str); }

    /// = c-string
    basic_string& operator= (const CharT* s)
    { return this->assign(s); }

    /// = character
    basic_string& operator= (CharT c)
    {
		this->assign(1, c);
		return *this;
    }

    /// = initializer list
    basic_string& operator= (std::initializer_list<CharT> il)
    {
		this->assign(il.begin(), il.size());
		return *this;
    }

    /// move operator
    basic_string& operator= (basic_string&& str) noexcept
    {
		// NB: DR 1204.
		this->swap(str);
		return *this;
    }

  	//----------------------------- iterators --------------------------------//

    iterator begin() noexcept
    {
    	m_make_editable_copy();
		return iterator(m_data);
    }

    const_iterator begin() const noexcept
    { return const_iterator(m_data); }

    iterator end() noexcept
    {
    	m_make_editable_copy();
		return iterator(m_data + this->size());
    }

    const_iterator end() const noexcept
    { return const_iterator(m_data + this->size()); }

    reverse_iterator rbegin() noexcept
    { return reverse_iterator(this->end()); }

    const_reverse_iterator rbegin() const noexcept
    { return const_reverse_iterator(this->end()); }

    reverse_iterator rend() noexcept
    { return reverse_iterator(this->begin()); }

    const_reverse_iterator rend() const noexcept
    { return const_reverse_iterator(this->begin()); }

    const_iterator cbegin() const noexcept
    { return const_iterator(m_data); }

    const_iterator cend() const noexcept
    { return const_iterator(m_data + this->size()); }

    const_reverse_iterator crbegin() const noexcept
    { return const_reverse_iterator(this->end()); }

    const_reverse_iterator crend() const noexcept
    { return const_reverse_iterator(this->begin()); }

  	//----------------------------- capacity ---------------------------------//
    size_type size() const noexcept
    { return  is_ram_ptr(m_data)? string_head()->m_length: ::strlen(m_data); }

    size_type length() const noexcept
    { return size(); }

    size_type max_size() const noexcept
    { return 0x8000; }

    void resize(size_type n, CharT c);

    void resize(size_type n)
    { this->resize(n, CharT()); }

    void shrink_to_fit()
    {
		if (capacity() > size())
		{
			__try
			{
				m_make_editable_copy(0, true); //force re-alloc
			}
			__catch(...)
			{}
		}
    }

    size_type capacity() const noexcept
    { return is_ram_ptr(m_data)? m_capacity(m_data): 0; }

    size_type reserve(size_type res = 0);

    void clear() noexcept
    {
    	if(is_ram_ptr(m_data))
    	{
    		if(string_head()->m_refcount == 1)
    		{
    			m_set_size(0);
    			return;
    		}
        	m_dispose();
    	}
    	m_data=nullptr;
    }

    bool empty() const noexcept
    { return m_data? !m_data[0]: 1; }

  	//-------------------------- element access ------------------------------//
    reference operator[](size_type pos)				    { return at(pos); }

    const_reference operator[] (size_type pos) const    { return at(pos); }

    CharT&  at(size_type n);

    const CharT& at(size_type n) const;

    reference front()								    { return at(0); }

    const_reference front() const					    { return at(0); }

    reference  back()							   	    { return at(size()-1);}

    const_reference back() const					    { return at(size() - 1); }

  	//----------------------------- modifiers --------------------------------//
    basic_string& operator+=(const basic_string& str)
    { return this->append(str); }

    basic_string& operator+=(const CharT* s)
    { return this->append(s); }

    basic_string& operator+=(CharT c)
    {
    	this->push_back(c);
    	return *this;
    }

    basic_string& operator+=(std::initializer_list<CharT> il)
    { return this->append(il.begin(), il.size()); }

    /// Appends string str
    basic_string& append(const basic_string& str);

    /// Appends a substring [pos, pos+n) of str
    basic_string& append(const basic_string& str, size_type pos, size_type n);

    /// Appends the first count characters of character string pointed to by s
    basic_string& append(const CharT* s, size_type n);

    /// Appends the null-terminated character string pointed to by s.
    basic_string& append(const CharT* s)
    { return this->append(s, strlen(s));  }

    /// Appends count copies of character c
    basic_string& append(size_type n, CharT c);

    /// Appends characters in the initializer list ilist.
    basic_string& append(std::initializer_list<CharT> il)
    { return this->append(il.begin(), il.size()); }

    /// Appends characters in the range [first, last)
    template<class _InputIterator>
      basic_string& append(_InputIterator k1, _InputIterator k2)
    {
		typedef typename ttl::__is_integer<_InputIterator>::__type _Integral;
		return m_replace_dispatch(size(), 0, k1, k2, _Integral());
    }

    void push_back(CharT c)
    {
		size_type sz = size();

		if(m_reserve(sz, sz+1))
		{
			m_data[sz++] = c;
			m_set_size(sz);
		}
    }

    basic_string& assign(const basic_string& str)
    {
    	if(this != &str)
    	{
        	m_dispose();
        	m_data = str.m_data;
        	m_grab();
    	}
    	return *this;
    }

    basic_string& assign(basic_string&& str)
    {
		this->swap(str);
		return *this;
    }

    basic_string& assign(const basic_string& str, size_type pos, size_type n)
    { return this->assign(str.m_data + pos, str.m_limit(pos, n)); }

    basic_string& assign(const CharT* s, size_type n)
    {
		return assign(basic_string(s, n));
    }

    basic_string& assign(const CharT* s)
    { return this->assign(s, strlen(s)); }

    basic_string& assign(size_type n, CharT c)
    { return m_replace_aux(size_type(0), this->size(), n, c); }

    template<class _InputIterator>
      basic_string&   assign(_InputIterator __first, _InputIterator __last)
    { return this->replace(m_data, m_data + size(), __first, __last); }

    basic_string&  assign(std::initializer_list<CharT> il)
    { return this->assign(il.begin(), il.size()); }

    void insert(iterator p, size_type n, CharT c)
    { this->replace(p, p, n, c);  }

    template<class _InputIterator>
      void insert(iterator p, _InputIterator k1, _InputIterator k2)
    { this->replace(p, p, k1, k2); }

    void insert(iterator __p, std::initializer_list<CharT> il)
    {
    	this->insert(__p - m_data, il.begin(), il.size());
    }

    basic_string& insert(size_type pos1, const basic_string& str)
    { return this->insert(pos1, str, size_type(0), str.size()); }

    basic_string& insert(size_type pos1, const basic_string& str,
	     size_type pos2, size_type n)
    { return this->insert(pos1, str.m_data + pos2, str.m_limit(pos2, n)); }

    basic_string& insert(size_type pos, const CharT* s, size_type n);

    basic_string& insert(size_type pos, const CharT* s)
    { return this->insert(pos, s, strlen(s)); }

    basic_string& insert(size_type pos, size_type n, CharT c)
    { return m_replace_aux(pos, size_type(0), n, c); }

    iterator insert(iterator p, CharT c)
    {
		const size_type pos = p - m_data;
		m_replace_aux(pos, size_type(0), size_type(1), c);
		return iterator(m_data + pos);
    }

    basic_string& erase(size_type pos = 0, size_type n = npos);

    iterator erase(iterator p)
    { return erase(p-m_data, 1);   }

    iterator erase(iterator i1, iterator i2)
    { return erase(i1 - m_data, i2 - i1);   }


    basic_string& replace(size_type pos, size_type n, const basic_string& str)
    { return this->replace(pos, n, str.m_data, str.size()); }

    basic_string& replace(size_type pos1, size_type n1, const basic_string& str,
	      size_type pos2, size_type n2)
    { return this->replace(pos1, n1, &str.m_data[pos2], str.m_limit(pos2, n2)); }

    basic_string& replace(size_type pos, size_type n1, const CharT* s, size_type n2);

    basic_string& replace(size_type pos, size_type n1, const CharT* s)
    {
    	return this->replace(pos, n1, s, strlen(s));
    }

    basic_string& replace(size_type pos, size_type n1, size_type n2, CharT c)
    { return m_replace_aux(pos, m_limit(pos, n1), n2, c); }

    basic_string& replace(iterator i1, iterator i2, const basic_string& str)
    { return this->replace(i1, i2, str.m_data, str.size()); }

    basic_string& replace(iterator i1, iterator i2, const CharT* s, size_type n)
    {
    	return this->replace(i1 - m_data, i2 - i1, s, n);
    }

    basic_string& replace(iterator i1, iterator i2, const CharT* s)
    {
    	return this->replace(i1, i2, s, strlen(s));
    }

    basic_string& replace(iterator i1, iterator i2, size_type n, CharT c)
    {
    	return m_replace_aux(i1 - m_data, i2 - i1, n, c);
    }

    template<class _InputIterator>
	basic_string& replace(iterator i1, iterator i2,
			_InputIterator k1, _InputIterator k2)
	{
		typedef typename ttl::__is_integer<_InputIterator>::__type _Integral;
		return m_replace_dispatch(i1 - m_data, i2 - i1, k1, k2, _Integral());
	}

    basic_string& replace(iterator i1, iterator i2, iterator k1, iterator k2)
    {
    	return this->replace(i1 - m_data, i2 - i1, k1, k2 - k1);
    }

    basic_string& replace(iterator i1, iterator i2, const_iterator k1, const_iterator k2)
    {
    	return this->replace(i1 - m_data, i2 - i1, k1, k2 - k1);
    }

    basic_string& replace(iterator i1, iterator i2, std::initializer_list<CharT> il)
    { return this->replace(i1, i2, il.begin(), il.end()); }


    size_type copy(CharT* s, size_type n, size_type pos = 0) const;

    void swap(basic_string& s);

    const CharT* c_str() const
    { return m_data; }

  	//---------------------------- search ------------------------------------//
    size_type find(const CharT* s, size_type pos, size_type n) const;

    size_type find(const basic_string& str, size_type pos = 0) const noexcept
    { return this->find(str.m_data, pos, str.size()); }

    size_type find(const CharT* s, size_type pos = 0) const
    {
    	return this->find(s, pos, strlen(s));
    }

    size_type find(CharT c, size_type pos = 0) const noexcept;

    size_type rfind(const basic_string& str, size_type pos = npos) const noexcept
    { return this->rfind(str.m_data, pos, str.size()); }

    size_type rfind(const CharT* s, size_type pos, size_type n) const;

    size_type rfind(const CharT* s, size_type pos = npos) const
    {
    	return this->rfind(s, pos, strlen(s));
    }

    size_type   rfind(CharT c, size_type pos = npos) const noexcept;

    size_type find_first_of(const basic_string& str, size_type pos = 0) const
	noexcept
    { return this->find_first_of(str.m_data, pos, str.size()); }

    size_type find_first_of(const CharT* s, size_type pos, size_type n) const;

    size_type find_first_of(const CharT* s, size_type pos = 0) const
    {
    	return this->find_first_of(s, pos, strlen(s));
    }

    size_type find_first_of(CharT c, size_type pos = 0) const noexcept
    { return this->find(c, pos); }

    size_type find_last_of(const basic_string& str, size_type pos = npos) const
	noexcept
    { return this->find_last_of(str.m_data, pos, str.size()); }

    size_type find_last_of(const CharT* s, size_type pos, size_type n) const;

    size_type find_last_of(const CharT* s, size_type pos = npos) const
    {
    	return this->find_last_of(s, pos, strlen(s));
    }

    size_type find_last_of(CharT c, size_type pos = npos) const noexcept
    { return this->rfind(c, pos); }

    size_type find_first_not_of(const basic_string& str, size_type pos = 0) const
	noexcept
    { return this->find_first_not_of(str.m_data, pos, str.size()); }

    size_type find_first_not_of(const CharT* s, size_type pos, size_type n) const;

    size_type find_first_not_of(const CharT* s, size_type pos = 0) const
    {
    	return this->find_first_not_of(s, pos, strlen(s));
    }

    size_type find_first_not_of(CharT c, size_type pos = 0) const noexcept;

    size_type find_last_not_of(const basic_string& str, size_type pos = npos) const
	noexcept { return this->find_last_not_of(str.m_data, pos, str.size()); }

    size_type find_last_not_of(const CharT* s, size_type pos, size_type n) const;

    size_type find_last_not_of(const CharT* s, size_type pos = npos) const
    {
    	return this->find_last_not_of(s, pos, strlen(s));
    }

    size_type find_last_not_of(CharT c, size_type pos = npos) const noexcept;

//    basic_string substr(size_type pos = 0, size_type n = npos) const
//    { return basic_string(*this, _M_check(pos, "basic_string::substr"), n); }

    int  compare(const basic_string& str) const
    {
    	pointer p1, p2;

    	p1 = m_data;
    	p2 = str.m_data;

    	if(p1 && p2)
    	{
    		int res;
			while(!(res = *p1 - *p2) && *p1  )
			{
				++p1;
				++p2;
			}
			return res;
    	}
    	return p1 - p2;
    }

    int compare(size_type pos, size_type n, const basic_string& str) const;

    int compare(size_type pos1, size_type n1, const basic_string& str,
	      size_type pos2, size_type n2) const;

    int compare(const CharT* s) const;

    int compare(size_type pos, size_type n1, const CharT* s) const;

    int compare(size_type pos, size_type n1, const CharT* s, size_type n2) const;


  	//-------------------------- depreciated ---------------------------------//
    basic_string substr( size_type pos, size_type n ) const
    {
    	return basic_string(*this, pos, n);
    }

	int atoi() const;
	long long atoll() const;
	int itoa(int num);

	size_type find_in_list(STR_LIST sl, size_type* dwRead) const;

	void free();

	int compare( const CharT* s, unsigned int n ) const
	{
		return (strncmp(m_data, s, n));
	}
    int start_with( const CharT* str ) const;
    int start_casewith( const CharT* str ) const;

	int format(const CharT *fmt, ...);
	int appendf(const CharT *fmt, ...);

};

template<typename CharT>
CharT basic_string<CharT>::dummy_char;

typedef basic_string<char> string;

template <typename CharT>
basic_string<CharT>&
basic_string<CharT>::m_replace_aux(size_type pos, size_type n1, size_type n2, CharT c)
{
	size_type sz = size();
	if(pos > sz)
	{
		TTL_THROW(EXCEPT_OUT_OF_RANGE);
	} else
	{
		sz -= pos;
		if(n1 > sz)
			n1 = sz;

		if(n2)
		{
			// we have something to replace with...
			size_type new_sz;

			sz -= n1; // size of rightmost (c)
			new_sz = pos + sz + n2;
			if((capacity() >= new_sz) && (string_head()->m_refcount == 1))
			{
				// work in place
				if(n2 != n1)
				{
					m_memmove(m_data+pos+n1, sz, pos+n2);
					m_set_size(new_sz);
				}

				m_memset(c, n2, pos);
			} else
			{
				pointer p = m_allocate(new_sz, new_sz);
  	  			if(p)
  	  			{
  	  				if(pos)
  	  					memcpy(p, m_data, pos*sizeof(CharT));

  	  				if(sz)
  	  					memcpy(p+pos+n2, m_data+pos+n1, sz*sizeof(CharT));
  					m_memset(c, n2, pos);

  	  			}
  			    m_dispose();
  			    m_data = p;
			}

		} else
		{
			// no replace
			if(n1)
				erase(pos, n1);
		}
	}

    return *this;
}

template <typename CharT>
basic_string<CharT>::basic_string(const basic_string& str) : m_data(str.m_data)
{
	m_grab();
}

template <typename CharT>
basic_string<CharT>::basic_string(const basic_string& str, size_t pos, size_t len)
:m_data(nullptr)
{
	size_type sz = str.size();
	if(pos > sz)
	{
		TTL_THROW(EXCEPT_OUT_OF_RANGE);
	} else
	{
		sz -= pos;
		if(len>sz)
			len = sz;

		if(len)
		{
			m_data = m_allocate(len, len);
			if(m_data)
				m_memcpy(str.c_str(), len, 0);
		}
	}
}

template <typename CharT>
basic_string<CharT>::basic_string (const char* s, size_t n)
:m_data(const_cast<pointer>(s))
{
	if (is_ram_ptr(m_data))
	{
		m_data = m_allocate(n, n);
		if(m_data)
			memcpy(m_data, s, n*sizeof(CharT));
	}
}

template <typename CharT>
basic_string<CharT>::basic_string(const CharT* s)
:m_data(const_cast<pointer>(s))
{
	if (is_ram_ptr(m_data))
	{
		size_t n = strlen(s);
		m_data = m_allocate(n, n);
		if(m_data)
			memcpy(m_data, s, n*sizeof(CharT));
	}
}

template <typename CharT>
basic_string<CharT>::basic_string(size_type n, CharT c) :m_data(m_construct(n, c))
{
}

template <typename CharT>
template <class InputIterator>
basic_string<CharT>::basic_string  (InputIterator first, InputIterator last)
:m_data(m_construct(first, last))
{
}

template <typename CharT>
basic_string<CharT>::basic_string(std::initializer_list<CharT> il)
:m_data(m_construct(il.begin(), il.end(), random_access_iterator_tag()))
{

}

template<typename CharT>
void basic_string<CharT>::resize(size_type n, CharT c)
{
	const size_type sz = this->size();
	if (sz < n)
		this->append(n - sz, c);
	else if (n < sz)
		this->erase(n);
}

template<typename CharT>
typename basic_string<CharT>::size_type
basic_string<CharT>::reserve(size_type res)
{
	size_type sz = this->size();

	if(res < sz)
		res = sz;

	return m_reserve(sz, res);
}

template<typename CharT>
CharT&  basic_string<CharT>::at(size_type n)
{
	pointer new_data;

	if(is_ram_ptr(m_data))
	{
		string_header* head = string_head();

		if(n > head->m_length)
		{
			TTL_THROW(EXCEPT_OUT_OF_RANGE);
			return (dummy_char);
		}
		if(head->m_refcount != 1 )
		{
			n = head->m_length;
  			new_data = m_allocate(n, n);
  			if(!new_data)
  				return (dummy_char);

  			memcpy(new_data, m_data, n*sizeof(CharT));
		    m_dispose();
		    m_data = new_data;
		}
	} else
	{
		size_type old_len = strlen(m_data);

		if(n > old_len)
		{
			TTL_THROW(EXCEPT_OUT_OF_RANGE);
			return (dummy_char);
		}

		new_data = m_allocate(old_len, old_len);
		if(!new_data)
			return (dummy_char);
		memcpy(new_data, m_data, old_len*sizeof(CharT));
		m_data = new_data;
	}
	return m_data[n];
}

template<typename CharT>
const CharT& basic_string<CharT>::at(size_type n) const
{
	if(n > size())
	{
		TTL_THROW(EXCEPT_OUT_OF_RANGE);
		return (dummy_char);
	}
	return m_data[n];
}


template<typename CharT>
basic_string<CharT>& basic_string<CharT>::append(const basic_string& str)
{
	m_append(str.m_data, str.size());
	return *this;
}

template<typename CharT>
basic_string<CharT>&
basic_string<CharT>::append(const basic_string& str, size_type pos, size_type n)
{
	if(n)
	{
		size_type sz = str.size();

		if(pos < sz)
		{
			if(n > sz-pos)
				n= sz-pos;

			m_append(sz, str.m_data + pos, n);
		}else
			if(pos > sz)
				TTL_THROW(EXCEPT_OUT_OF_RANGE);
	}
	return *this;
}

template<typename CharT>
basic_string<CharT>& basic_string<CharT>::append(const CharT* s, size_type n)
{
	if(n && s)
	{
		size_type sz = size();

		if(m_data <= s && s<= m_data+sz)
		{
			size_type off = s-m_data;
			if(m_reserve(sz, sz+n))
			{
				m_set_size(n +sz);
				m_memcpy(m_data+off, n, sz);
			}
		} else
		{
			m_append(sz, s, n);
		}
	}
	return *this;
}

template<typename CharT>
basic_string<CharT>& basic_string<CharT>::append(size_type n, CharT c)
{
	if(n)
	{
		size_type sz = size();

		if(m_reserve(sz, sz+n))
		{
			m_set_size(n +sz);
			m_memset(c, n, sz);
		}
	}
	return *this;
}

template<typename CharT>
basic_string<CharT>&
basic_string<CharT>::insert(size_type pos, const CharT* s, size_type n)
{
	if(n)
	{
		size_type sz = size();

		if(pos > sz || !s)
		{
			TTL_THROW(EXCEPT_OUT_OF_RANGE);
		} else
		{
			if(m_data <= s && s<= m_data+sz)
			{
				const size_type off = s - m_data;

				if(m_reserve(sz, sz+n))
				{
					this->m_set_size(sz+n);
					s = m_data + off;

					CharT* p = m_data + pos;
					m_memcpy(p, sz-pos, pos+n);
					if (s >= p)
					{
						m_memcpy(s+n, n, pos); // s moved +n
					}
					else
					{
						m_memcpy(s, n, pos);
					}
				}

			} else
			{
				if(m_reserve(sz, sz+n))
				{
					m_set_size(sz+n);
					m_memcpy(m_data + pos, sz-pos, pos+n);
					m_memcpy(s, n, pos);
				}
			}
		}
	}
	return *this;
}

template<typename CharT>
basic_string<CharT>& basic_string<CharT>::erase(size_type pos, size_type n)
{
	size_type sz = size();
	if(pos > sz)
	{
		TTL_THROW(EXCEPT_OUT_OF_RANGE);
	} else
	{
		if(n > sz-pos)
			n = sz-pos;
    	if(n)
    	{
    		if(m_reserve(sz, sz))
    		{
    			pointer dst, src;

        		sz -= pos+ n;	// chars to move
    			dst = m_data + pos;
    			src = dst+n;

    			for(n=0; n<sz; ++n)
    				dst[n] = src[n];

				m_set_size(pos+sz);
    		}
    	}
	}
	return *this;
}

/*
 * Legend
 * a -> pos
 * b -> n1
 * c -> size - pos - n1
 * x -> this is rom or referenced or no space
 * - -> this is ram, ref=1, space is OK
 *
 * 	a b c ? s
 *  0 0 0 * *	s			- replacing empty with s
 *  * 0 * *	0	this		- replace nothing
 *  * 1 * * 0	erase b
 *
 *  0 0 1 x	1	new(s+c)
 *  0 0 1 - 1	c >> n2, copy s
 *  1 0 0 x 1	new(a+s)
 *  1 0 0 - 1	copy s
 *  1 0 1 x 1	new(a,s,c)
 *  1 0 1 - 1 	c>> n2, copy s
 *
 *  0 1 1 x	1	new(s+c)
 *  0 1 1 - 1	c >> (n2-n1), copy s
 *  1 1 0 x 1	new(a+s)
 *  1 1 0 - 1	copy s
 *  1 1 1 x 1	new(a,s,c) , copy a,s c
 *  1 1 1 - 1 	c>> (n2-n1), copy s
 *
 */
template<typename CharT>
basic_string<CharT>&
basic_string<CharT>::replace(size_type pos, size_type n1, const CharT* s, size_type n2)
{
	size_type sz = size();
	if(pos > sz)
	{
		TTL_THROW(EXCEPT_OUT_OF_RANGE);
	} else
	{
		sz -= pos;
		if(n1 > sz)
			n1 = sz;

		if(n2 && s)
		{
			// we have something to replace with...
			size_type new_sz;
			pointer p;

			sz -= n1; // size of rightmost (c)
			new_sz = pos + sz + n2;
			if((capacity() >= new_sz) && (string_head()->m_refcount == 1))
			{
				// work in place
				if(n2 >= n1)
				{
					// expanding, so make room first and then copy
					// we have 				aaaaaabbbbcccc
					// it will become 		aaaaaabbbb   cccc
					// it will become 		aaaaaabbbbssscccc
					if(n2 != n1)
					{
						p = m_data+pos+n1;
						m_memcpy(p, sz, pos+n2);
						if((s >= p) && (s< p+sz))
						{
							s += n2-n1; // s was moved too
						}
					}
					m_memmove(s, n2, pos);
				}
				else
				{
					// shrinking we have 	aaaaaa1112222cccc
					// it will become 		aaaaaasss2222cccc
					// it will become 		aaaaaassscccc
					m_memmove(s, n2, pos);
					m_memmove(m_data + pos+n1, sz, pos+n2);
				}
				m_set_size(new_sz);
			} else
			{
  	  			p = m_allocate(new_sz, new_sz);
  	  			if(p)
  	  			{
  	  				if(pos)
  	  					memcpy(p, m_data, pos*sizeof(CharT));

  	  				memcpy(p + pos, s, n2*sizeof(CharT));

  	  				if(sz)
  	  					memcpy(p+pos+n2, m_data+pos+n1, sz*sizeof(CharT));

  	  			}
  			    m_dispose();
  			    m_data = p;
			}

		} else
		{
			// no replace
			if(n1)
				erase(pos, n1);
		}
	}
	return *this;
}

template<typename CharT>
typename basic_string<CharT>::size_type
basic_string<CharT>::copy(CharT* s, size_type n, size_type pos) const
{
	const_pointer p = m_data + pos;
	size_type i;

    n = m_limit(pos, n);

	if(s >= p)
	{
		//move to right
		for(i=n; i--; )
			s[i] = p[i];

	} else
	{
		for(i=0; i<n; ++i)
			s[i] = p[i];
	}

    return n;
}

template<typename CharT>
void basic_string<CharT>::swap(basic_string& s)
{
	pointer p = m_data;

	m_data  = s.m_data;
	s.m_data = p;
}


template<typename CharT>
typename basic_string<CharT>::size_type
basic_string<CharT>::find(const CharT* s, size_type pos, size_type n) const
{
    const size_type sz = this->size();

    if (n == 0)
    	return pos <= sz ? pos : npos;

    if (n <= sz)
	{
	  for (; pos <= sz - n; ++pos)
	    if (m_data[pos] == s[0] && memcmp(m_data + pos + 1, s + 1, n - 1) == 0)
	      return pos;
	}
    return npos;
}


template<typename CharT>
typename basic_string<CharT>::size_type
basic_string<CharT>::find(CharT c, size_type pos) const noexcept
{
    size_type sz = this->size();

    while(pos < sz)
    {
    	if(m_data[pos] == c)
    		return pos;
    	++pos;
    }
    return npos;
}

template<typename CharT>
typename basic_string<CharT>::size_type
basic_string<CharT>::rfind(const CharT* s, size_type pos, size_type n) const
{
	size_type sz = this->size();
	if (n <= sz)
	{
		sz -= n;
		if(pos > sz)
			pos = sz;

		if(n == 0)
			return pos;
		do
		{
			if (memcmp(m_data + pos, s, n) == 0)
				return pos;
		} while (pos-- > 0);
	}
	return npos;
}

template<typename CharT>
typename basic_string<CharT>::size_type
basic_string<CharT>::rfind(CharT c, size_type pos ) const noexcept
{
	size_type sz = this->size();
	if (sz)
	{

		if (pos > --sz)
			pos = sz;

		do
		{
			if (m_data[pos] == c)
				return pos;
		} while(pos--);
	}
	return npos;
}

template<typename CharT>
typename basic_string<CharT>::size_type
basic_string<CharT>::find_first_of(const CharT* s, size_type pos, size_type n) const
{
	if(n)
	{
		size_type sz = this->size();

		for (; pos < sz; ++pos)
		{
			if (m_find(s, n, m_data[pos]))
				return pos;
		}
	}
	return npos;
}

template<typename CharT>
typename basic_string<CharT>::size_type
basic_string<CharT>::find_last_of(const CharT* s, size_type pos, size_type n) const
{
	size_type sz = this->size();
	if (sz && n)
	{
		if (pos > --sz)
			pos = sz;
		do
		{
			if (m_find(s, n, m_data[pos]))
				return pos;
		} while (pos--);
	}
	return npos;
}

template<typename CharT>
typename basic_string<CharT>::size_type
basic_string<CharT>::find_first_not_of(const CharT* s, size_type pos, size_type n) const
{
	size_type sz = this->size();

	for (; pos < sz; ++pos)
	{
		if (!m_find(s, n, m_data[pos]))
			return pos;
	}
	return npos;
}

template<typename CharT>
typename basic_string<CharT>::size_type
basic_string<CharT>::find_first_not_of(CharT c, size_type pos) const noexcept
{
	size_type sz = this->size();

    while(pos < sz)
    {
		if (m_data[pos] == c)
			return pos;
		++pos;
    }
    return npos;
}

template<typename CharT>
typename basic_string<CharT>::size_type
basic_string<CharT>::find_last_not_of(const CharT* s, size_type pos, size_type n) const
{
	size_type sz = this->size();
	if (sz)
	{
		if (pos > --sz)
			pos = sz;
		do
		{
			if (!m_find(s, n, m_data[pos]))
				return pos;
		} while (pos--);
	}
    return npos;
}

template<typename CharT>
typename basic_string<CharT>::size_type
basic_string<CharT>::find_last_not_of(CharT c, size_type pos) const noexcept
{
    size_type sz = this->size();
	if (sz)
	{
		if (pos > --sz)
			pos = sz;
		do
		{
			if ( m_data[pos] != c)
				return pos;
		} while (pos--);
	}
    return npos;
}

template<typename CharT>
int basic_string<CharT>::compare(size_type pos, size_type n, const basic_string& str) const
{
    n = m_limit(pos, n);
    const size_type osz = str.size();

    int res = m_compare(m_data + pos, str.m_data, min(n, osz));
    if (!res)
    	res = n - osz;
    return res;
}

template<typename CharT>
int basic_string<CharT>::compare(size_type pos1, size_type n1, const basic_string& str,
      size_type pos2, size_type n2) const
{
	n1 = m_limit(pos1, n1);
	n2 = str.m_limit(pos2, n2);
	int res = m_compare(m_data + pos1, str.m_data + pos2, min(n1, n2));
	if (!res)
		res = n1 - n2;
	return res;
}

template<typename CharT>
int basic_string<CharT>::compare(const CharT* s) const
{
//	const size_type sz = this->size();
//	const size_type osz = strlen(s);
//
//	int res = m_compare(m_data, s, min(sz, osz));
//	if (!res)
//		res = sz - osz;
//	return res;
	return strcmp(m_data, s);
}


template<typename CharT>
int basic_string<CharT>::compare(size_type pos, size_type n1, const CharT* s) const
{
	n1 = m_limit(pos, n1);
	const size_type osz = strlen(s);

	int res = m_compare(m_data + pos, s, min(n1, osz));
	if (!res)
		res = n1 - osz;
	return res;
}

template<typename CharT>
int basic_string<CharT>::compare(size_type pos, size_type n1, const CharT* s, size_type n2) const
{
    n1 = m_limit(pos, n1);

    int res = m_compare(m_data + pos, s, min(n1, n2));
    if (!res)
    	res = n1 - n2;
    return res;
}

template<typename CharT>
int basic_string<CharT>::atoi() const
{
	int res=0;

	if(m_data)
	{
		tmos_sscanf(m_data, "%d", &res);
	}
	return (res);
}

template<typename CharT>
long long basic_string<CharT>::atoll() const
{
	long long res=0;

	if(m_data)
	{
		tmos_sscanf(m_data, "%ld", &res);
	}
	return (res);
}

template<typename CharT>
int basic_string<CharT>::itoa(int num)
{
	return format("%d", num);
}

template<typename CharT>
typename basic_string<CharT>::size_type
basic_string<CharT>::find_in_list(STR_LIST sl, size_type* dwRead) const
{
	const_pointer buf;
	buf = m_data;
	if(dwRead)
		buf += *dwRead;
	return (::find_in_list(buf, sl, dwRead));
}

template<typename CharT>
void basic_string<CharT>::free()
{
	m_dispose();
	m_data = const_cast<CharT*>("");
}

template<typename CharT>
int basic_string<CharT>::start_with( const CharT* str ) const
{
	unsigned int len;

	len = strlen(str);
	if(length() >= len)
	{
		if(!strncmp(c_str(), str, len))
			return len;
	}
	return 0;
}

template<typename CharT>
int basic_string<CharT>::start_casewith( const CharT* str ) const
{
	unsigned int len;

	len = strlen(str);
	if(length() >= len)
	{
		if(!strncasecmp(c_str(), str, len))
			return len;
	}
	return 0;
}

template<typename CharT>
int basic_string<CharT>::format(const CharT *fmt, ...)
{
	va_list args;
	int len;

	va_start(args, fmt);
	clear();
	len = tmos_vprintf_len(fmt, args);
	if(m_reserve(0, len))
	{
		len = tmos_vsprintf(m_data, fmt, args);
		m_set_size(len);
	} else
	{
		len = 0;
	}

	return len;
}

template<typename CharT>
int basic_string<CharT>::appendf(const CharT *fmt, ...)
{
	va_list args;
	int len1, len2;

	va_start(args, fmt);
	len1 = size();
	len2 = tmos_vprintf_len(fmt, args);	//Approximate size
	if(m_reserve(len1, len1+len2))
	{
		len2 = tmos_vsprintf(m_data + len1, fmt, args) + len1;
		m_set_size(len2);
	} else
	{
		len2 = 0;
	}

	return len2;
}

//--------------------------- operator + -------------------------------------//
template<typename CharT>
  basic_string<CharT>
  operator+(const basic_string<CharT>& lhs,  const basic_string<CharT>& rhs)
  {
    basic_string<CharT> str(lhs);
    str.append(rhs);
    return str;
  }

template<typename CharT>
  basic_string<CharT>
  operator+(const CharT* lhs, const basic_string<CharT>& rhs)
  {
    typedef basic_string<CharT> __string_type;
    typedef typename __string_type::size_type	  __size_type;
     __size_type l1 = strlen(lhs);
    __size_type sz = l1 + rhs.size();

    CharT* p = __string_type::m_allocate(sz, sz);
	if(p)
	{
		CharT* pp = p;

		sz -= l1;
		while(l1)
		{
			*pp++ = *lhs++;
			--l1;
		}

		lhs = rhs.c_str();
		while(sz--)
		{
			*pp++ = *lhs++;
		}
	}
    return static_cast<__string_type>(p);
  }

template<typename CharT>
  basic_string<CharT>
  operator+(CharT lhs, const basic_string<CharT>& rhs)
  {
    typedef basic_string<CharT> __string_type;
    typedef typename __string_type::size_type	  __size_type;
    __size_type sz = 1 + rhs.size();

    CharT* p = __string_type::m_allocate(sz, sz);

  	if(p)
  	{
		*p = lhs;

  		while(--sz)
  		{
  			p[sz] = rhs[sz-1];
  		}
  	}
    return static_cast<__string_type>(p);
  }

template<typename CharT>
  inline basic_string<CharT>
  operator+(const basic_string<CharT>& lhs,
	     const CharT* rhs)
  {
    basic_string<CharT> str(lhs);
    str.append(rhs);
    return str;
  }

template<typename CharT>
  inline basic_string<CharT>
  operator+(const basic_string<CharT>& lhs, CharT rhs)
  {
	typedef basic_string<CharT> __string_type;
	typedef typename __string_type::size_type __size_type;

	__string_type str(lhs);
	str.append(__size_type(1), rhs);
	return str;
  }

template<typename CharT>
  inline basic_string<CharT>
  operator+(basic_string<CharT>&& lhs, const basic_string<CharT>& rhs)
  { return ttl::move(lhs.append(rhs)); }

template<typename CharT>
  inline basic_string<CharT>
  operator+(const basic_string<CharT>& lhs, basic_string<CharT>&& rhs)
  { return ttl::move(rhs.insert(0, lhs)); }

template<typename CharT>
  inline basic_string<CharT>
  operator+(basic_string<CharT>&& lhs,  basic_string<CharT>&& rhs)
  {
	const auto sz = lhs.size() + rhs.size();
	const bool __cond = (sz > lhs.capacity() && sz <= rhs.capacity());
	return __cond ? ttl::move(rhs.insert(0, lhs)) : ttl::move(lhs.append(rhs));
  }

template<typename CharT>
  inline basic_string<CharT>
  operator+(const CharT* lhs,  basic_string<CharT>&& rhs)
  { return ttl::move(rhs.insert(0, lhs)); }

template<typename CharT>
  inline basic_string<CharT>
  operator+(CharT lhs,  basic_string<CharT>&& rhs)
  { return ttl::move(rhs.insert(0, 1, lhs)); }

template<typename CharT>
  inline basic_string<CharT>
  operator+(basic_string<CharT>&& lhs, const CharT* rhs)
  { return ttl::move(lhs.append(rhs)); }

template<typename CharT>
  inline basic_string<CharT>
  operator+(basic_string<CharT>&& lhs, CharT rhs)
  { return ttl::move(lhs.append(1, rhs)); }

//--------------------------- operator == ------------------------------------//

template<typename CharT>
  inline bool
  operator==(const basic_string<CharT>& lhs, const basic_string<CharT>& rhs)
  { return lhs.compare(rhs) == 0; }

template<typename CharT>
  inline bool
  operator==(const CharT* lhs, const basic_string<CharT>& rhs)
  { return rhs.compare(lhs) == 0; }

template<typename CharT>
  inline bool
  operator==(const basic_string<CharT>& lhs, const CharT* rhs)
  { return lhs.compare(rhs) == 0; }

//--------------------------- operator != ------------------------------------//
template<typename CharT>
  inline bool
  operator!=(const basic_string<CharT>& lhs, const basic_string<CharT>& rhs)
  { return !(lhs == rhs); }

template<typename CharT>
  inline bool
  operator!=(const CharT* lhs, const basic_string<CharT>& rhs)
  { return !(lhs == rhs); }

template<typename CharT>
  inline bool
  operator!=(const basic_string<CharT>& lhs, const CharT* rhs)
  { return !(lhs == rhs); }

//--------------------------- operator < -------------------------------------//
template<typename CharT>
  inline bool
  operator<(const basic_string<CharT>& lhs, const basic_string<CharT>& rhs)
  { return lhs.compare(rhs) < 0; }

template<typename CharT>
  inline bool
  operator<(const basic_string<CharT>& lhs, const CharT* rhs)
  { return lhs.compare(rhs) < 0; }

template<typename CharT>
  inline bool
  operator<(const CharT* lhs, const basic_string<CharT>& rhs)
  { return rhs.compare(lhs) > 0; }

//--------------------------- operator > -------------------------------------//
template<typename CharT>
  inline bool
  operator>(const basic_string<CharT>& lhs, const basic_string<CharT>& rhs)
  { return lhs.compare(rhs) > 0; }

template<typename CharT>
  inline bool
  operator>(const basic_string<CharT>& lhs, const CharT* rhs)
  { return lhs.compare(rhs) > 0; }

template<typename CharT>
  inline bool
  operator>(const CharT* lhs, const basic_string<CharT>& rhs)
  { return rhs.compare(lhs) < 0; }

//--------------------------- operator <= ------------------------------------//
template<typename CharT>
  inline bool
  operator<=(const basic_string<CharT>& lhs, const basic_string<CharT>& rhs)
  { return lhs.compare(rhs) <= 0; }

template<typename CharT>
  inline bool
  operator<=(const basic_string<CharT>& lhs, const CharT* rhs)
  { return lhs.compare(rhs) <= 0; }

template<typename CharT>
  inline bool
  operator<=(const CharT* lhs,  const basic_string<CharT>& rhs)
  { return rhs.compare(lhs) >= 0; }

//--------------------------- operator >= ------------------------------------//
template<typename CharT>
  inline bool
  operator>=(const basic_string<CharT>& lhs, const basic_string<CharT>& rhs)
  { return lhs.compare(rhs) >= 0; }

template<typename CharT>
  inline bool
  operator>=(const basic_string<CharT>& lhs, const CharT* rhs)
  { return lhs.compare(rhs) >= 0; }

template<typename CharT>
  inline bool
  operator>=(const CharT* lhs, const basic_string<CharT>& rhs)
  { return rhs.compare(lhs) <= 0; }


template<typename CharT>
  inline void
  swap(basic_string<CharT>& lhs, basic_string<CharT>& rhs)
  { lhs.swap(rhs); }

} // namespace ttl

typedef ttl::basic_string<char> string;


#endif /* TTL_STRING_H_ */
