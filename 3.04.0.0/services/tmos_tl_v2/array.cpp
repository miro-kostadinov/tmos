/*
 * array.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: stanly
 */

#include <array.h>

template<class TYPE>
void array<TYPE>::ConstructElements(int from, int count)
{
	for(;from < count; from++)
	{
		data[from].TYPE();
	}
}

template<class TYPE>
array<TYPE>::array()
{
	data = NULL;
	size = max_size = grow_by = 0;
}

template<class TYPE>
void array<TYPE>::DestructElements(int from, int count)
{
	for(;from < count; from++)
	{
		data[from].~TYPE();
	}
}

template<class TYPE>
array<TYPE>::~array()
{
	if (this && data != NULL)
	{
		DestructElements(size);
		delete[] (uint8_t*)data;
	}
}

template<class TYPE>
void array<TYPE>::set_size(int new_size, int grow)
{
	if(!this || new_size < 0)
		return;

	if (grow != -1)
		grow_by = grow;  // set new size

	if (new_size == 0)
	{
		// shrink to nothing
		if (data != NULL)
		{
			DestructElements(size);
			delete[] (uint8_t*)data;
			data = NULL;
		}
		size = max_size = 0;
	}
	else if (data == NULL)
	{
		// create one with exact size
		data = (TYPE*) new char[new_size * sizeof(TYPE)];
		ConstructElements(0, new_size);
		size = max_size = new_size;
	}
	else if (new_size <= max_size)
	{
		// it fits
		if(new_size > size)
		{
			// initialize the new elements
			ConstructElements(size, new_size-size);
		}
		else if(size > new_size)
		{
			// destroy the old elements
			DestructElements(new_size, size-new_size);
		}
		size = new_size;
	}
	else
	{
		// otherwise, grow array
		grow = grow_by;
		if(grow == 0)
		{
			// heuristically determine growth when grow == 0
			grow = size / 8;
			grow = (grow < 4) ? 4 : ((grow > 1024) ? 1024 : grow);
		}
		int new_max;
		if (new_size < max_size + grow)
			new_max = max_size + grow;  // granularity
		else
			new_max = new_size;  // no slush

		if(new_max >= max_size)
		{
			data = tsk_realloc(data, new_max * sizeof(TYPE));
		}
		if(new_size > size && data)
			ConstructElements(size, new_size-size);

		// get rid of old stuff (note: no destructors called)
		size = new_size;
		max_size = new_max;
	}
}


template<class TYPE>
void array<TYPE>::FreeExtra()
{
	if(!this)
		return;

	if (size != max_size)
	{
		data = tsk_realloc(data, size * sizeof(TYPE));
		max_size = size;
	}
}

template<class TYPE>
void array<TYPE>::set_at_grow(int index, TYPE& element)
{
	ASSERT_VALID(this);
	ASSERT(index >= 0);

	if (index >= size)
		set_size(index+1, -1);
	data[index] = element;
}

template<class TYPE>
void array<TYPE>::insert_at(int index, TYPE& element, int count /*=1*/)
{
	if(!this || index < 0 || count < 0)
		return;

	if (index >= size)
	{
		// adding after the end of the array
		set_size(index + count, -1);   // grow so index is valid
	}
	else
	{
		// inserting in the middle of the array
		int prev_size = size;
		set_size(size + count, -1);  // grow it to new size
		// destroy intial data before copying over it
		DestructElements(prev_size, count);
		// shift old data up to fill gap
		memmove(&data[index+count], &data[index],
			(prev_size-index) * sizeof(TYPE));

		// re-init slots we copied from
		ConstructElements(index, count);
	}

	// insert new value in the gap
	if(index + count <= size)
	{
		while (count--)
			data[index++] = element;
	}
}

template<class TYPE>
void array<TYPE>::remove_at(int index, int count)
{
	if(!this || index < 0 || count < 0 || index + count > size)
		return;

	// just remove a range
	DestructElements(index, count);

	int cnt = size - (index + count);
	if (cnt)
		memmove(&data[index], &data[index + count], cnt * sizeof(TYPE));
	size -= count;
}

template<class TYPE>
void array<TYPE>::insert_at(int index, array* new_array)
{

	if(!this || !new_array || index < 0)
		return;
	if (new_array->get_size() > 0)
	{
		insert_at(index, new_array->get_at(0), new_array->get_size());
		for (int i = 0; i < new_array->get_size(); i++)
			set_at(index + i, new_array->get_at(i));
	}
}
