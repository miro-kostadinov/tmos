/*
 * test.cpp
 *
 *  Created on: Mar 20, 2013
 *      Author: miro
 */
#include <brd_cfg.h>

#ifndef USE_TMOS_TL_V2
#define USE_TMOS_TL_V2 0
#endif

#ifndef USE_TMOS_TL_V3
#define USE_TMOS_TL_V3 0
#endif

#if USE_TMOS_TL_V2
//#include <iostream>
//#include <array>
//#include <vector>
//
//using namespace std;

//#undef min
//#undef max
#include <tl_array.h>
#include <tl_vector.h>
using namespace tmos;

//#include <tmos++.h>

register int *PMAIN_TASK asm("r10");
register int *CURRENT_TASK asm("r11");

//#include <ustl.h>
//using namespace ustl;


void test()
{
//	array<int, 5> c = {1,2,3,4,5};
//	vector <5> a;
//	reverse_iterator<>

//	tmos::array<int,4> myarray = {{4, 26, 80, 14}} ;
//
//	for(auto x : myarray)
//	{
//		TRACELN("x=%u", x);
//	}


	tmos::vector<int> myvector ={1,2,3, 4};


	for(auto x : myvector)
	{
		TRACELN("x=%u", x);
	}


	myvector.push_back(5);

	for(auto x : myvector)
	{
		TRACELN("x=%u", x);
	}

//	vector<int> v;
//	v.push_back (1);
//	v.emplace_back (2);
//	v[1] = 0;
//	v.erase (v.begin() + 1);
//	v.pop_back();
//	v.insert (v.begin(), 4);
//	v.resize (15);
}

#endif

#if USE_TMOS_TL_V3
void test()
{

}
#endif
