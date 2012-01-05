/*
 * mqueue.h
 *
 *  Created on: 30.04.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef MQUEUE_H_
#define MQUEUE_H_

#include <tmos.h>

/** Message queue class **/
template <class T, int max_size>
class mqueue
{
	unsigned short 	size;
	unsigned short	in2;
	unsigned short	in;
	unsigned short	out;
	T 				items[max_size];

public:
	mqueue(): size(max_size), in2(1), in(0), out(0) {};

	/** checks if the queue is empty **/
	bool empty()
	{
		return (in == out);
	}

	/** checks if the queue is full **/
	bool full()
	{
		return (in2 == out);
	}

	/** Push item in the queue **/
	bool push(T item)
	{
		if(in2 != out)
		{
			items[in] = item;
			in = in2++;
			if(in2 == size)
				in2=0;
			return (true);
		}
		return (false);
	}

	/** pop item from the queue **/
	bool pop(T& item)
	{
		if(in != out)
		{
			item = items[out++];
			if(out == size)
				out=0;
			return (true);
		}
		return (false);
	}
};

/** fixed size message queue
 *
 * size_mask must be order of 2 -1
 */

template <class T, int size_mask>
class fmqueue
{
	unsigned short	in;
	unsigned short	out;
	T 				items[size_mask+1];

public:
	fmqueue(): in(0), out(0) {};

	/** checks if the queue is empty **/
	bool empty()
	{
		return (in == out);
	}

	/** checks if the queue is full **/
	bool full()
	{
		return (((in+1) & size_mask) == out);
	}

	/** Push item in the queue **/
	bool push(T item)
	{
		unsigned in2;
		in2 = (in+1) & size_mask;
		if(in2 != out)
		{
			items[in] = item;
			in = in2;
			return (true);
		}
		return (false);
	}

	/** pop item from the queue **/
	bool pop(T& item)
	{
		if(in != out)
		{
			item = items[out++];
			out &= size_mask;
			return (true);
		}
		return (false);
	}

	/** top element in the queue **/
	T top()
	{
		if(in != out)
			return (items[out]);

		return (NULL);
	}
};


#endif /* MQUEUE_H_ */