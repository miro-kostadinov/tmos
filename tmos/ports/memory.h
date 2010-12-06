//==========================================================================
// This file is part of TMOS Operating System (ThumbOS).
// Copyright (C) 2007-2008 Eltrade Ltd.
//
// TMOS is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3 or (at your option) any later version.
//
// TMOS is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
//==========================================================================
//
// Author(s):     stanly
// Date:          18.11.2010
// Description:   memory.h
//
//==========================================================================

#ifndef MEMORY_H_
#define MEMORY_H_

template<class Y> struct auto_ptr_ref
{
	Y* yp;
	auto_ptr_ref(Y* rhs) :
		yp(rhs)
	{
	}
};

template<class T>
class auto_ptr
{
private:
	T* ap; //refers to the actual owned object (if any)
public:
	typedef T element_type;
	//constructor
	explicit auto_ptr(T* ptr = 0):
		ap(ptr)
	{
	}

	//copy constructors (with implicit conversion)
	//- note: nonconstant parameter
	auto_ptr(auto_ptr& rhs) :
		ap(rhs. release())
	{
	}
	template<class Y>
	auto_ptr(auto_ptr<Y>& rhs) :
		ap(rhs.release())
	{
	}

	//assignments (with implicit conversion)
	//- note: nonconstant parameter
	auto_ptr& operator=(auto_ptr& rhs)
	{
		reset(rhs.release());
		return *this;
	}
	template<class Y>
	auto_ptr& operator=(auto_ptr<Y>& rhs)
	{
		reset(rhs.release());
		return *this;
	}

	//destructor
	~auto_ptr()
	{
		delete ap;
	}

	//value access
	T* get() const
	{
		return ap;
	}
	T& operator*() const
	{
		return *ap;
	}
	T* operator->() const
	{
		return ap;
	}

	//release ownership
	T* release()
	{
		T* tmp(ap);
		ap = 0;
		return tmp;
	}

	//reset value
	void reset(T* ptr = 0)
	{
		if (ap != ptr)
		{
			delete ap;
			ap = ptr;
		}
	}

	/* special conversions with auxiliary type to enable copies
	 and assignments
	 */
	auto_ptr(auto_ptr_ref<T> rhs) :
		ap(rhs.yp)
	{
	}
	auto_ptr& operator=(auto_ptr_ref<T> rhs)
	{ //new
		reset(rhs.yp);
		return *this;
	}
	template<class Y>
	operator auto_ptr_ref<Y>()
	{
		return auto_ptr_ref<Y> (release());
	}
	template<class Y>
	operator auto_ptr<Y>()
	{
		return auto_ptr<Y> (release());
	}
};


#endif /* MEMORY_H_ */
