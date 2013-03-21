/*
 * array.h
 *
 *  Created on: Mar 13, 2013
 *      Author: stanly
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <tmos.h>

// array<TYPE>
namespace tmos
{












template<class TYPE>
class array
{
// Implementation
protected:
	TYPE* data;   	// the actual array of data
	int size;     	// # of elements (upperBound - 1)
	int max_size;  	// max allocated
	int grow_by;    // grow amount


public:
// Construction
	array();
	~array();

// Attributes
	int get_size() const { return size; }
	int get_upper_bound() const { return size-1; }
	void set_size(int new_size, int grow = -1);

// Operations
	// Clean up
	void FreeExtra();
	void erase() { set_size(0, -1); }


	// Accessing elements
	TYPE get_at(int index) const
	{
		if(index >= 0 && index < size)
			return data[index];
		return *(TYPE *)NULL;
	}
	void set_at(int index, TYPE& element)
	{
		if(index >= 0 && index < size)
			data[index] = element;
	}
	TYPE& element_at(int index)
	{
		if(index >= 0 && index < size)
			return data[index];
		return *(TYPE *)NULL;
	}

	// Direct Access to the element data (may return NULL)
	const TYPE* get_data() const { return (const TYPE*)data;}
	TYPE* get_data() { return (TYPE*)data;}

	// Potentially growing the array
	void set_at_grow(int index, TYPE& element);

	int add(TYPE& element)
	{
		int index = size;
		set_at_grow(index, element);
		return index;
	}

	// Operations that move elements around
	void insert_at(int index, TYPE& element, int count = 1);
	void remove_at(int index, int count = 1);
	void insert_at(int index, array* new_array);

	// overloaded operator helpers
	TYPE operator[](int index) const { return get_at(index); }
	TYPE& operator[](int index) { return element_at(index); }
private:
	void DestructElements(int from, int count);
	void ConstructElements(int from, int count);

};
}

#endif /* ARRAY_H_ */
