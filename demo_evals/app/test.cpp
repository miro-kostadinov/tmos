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




#if USE_TMOS_TL_V1
#include <tmos.h>

#include <vector>


struct my_stru
{
	unsigned int data;

	my_stru() :data(0)
	{
		TRACELN("constr=%x", this);
	}

	my_stru(const my_stru& s) :data(s.data)
	{
		TRACELN("constr&=%x", this);
	}

	~my_stru()
	{
		TRACELN("desstr=%x", this);
	}
};

//#include <ext/new_allocator.h>
void std::__throw_bad_alloc()
{
	TRACELN1("BAD ALLOC!\r\n");
	while(1)
	{

	}
}

void test()
{
//	std::vector<int> myvector ={1,2,3, 4};
//
//
//	for(auto x : myvector)
//	{
//		TRACELN("x=%u", x);
//	}

	std::vector<my_stru> myvector2(5);



}
#endif





#if USE_TMOS_TL_V2
#include <tmos.h>
#include <ttl_string.h>
#include <ttl_tree.h>
using namespace ttl;

//#include <ext/new_allocator.h>
//void std::__throw_bad_alloc()
//{
//	TRACELN1("BAD ALLOC!\r\n");
//	while(1)
//	{
//
//	}
//}

typedef basic_string<char> string;

void print_tree(tree<string>& tr, tree<string>::iterator loc, int level)
{
	while (loc != tr.end())
	{
		TRACELN1("");
		for (int i = 0; i < tr.depth(loc); ++i)
			TRACE1(" ");
		TRACE("%s", loc->c_str());
//		print_tree(tr, loc.begin(), level+1);
		++loc;
	}

}

struct base
{
	int base_data;
};

struct derived: public base
{
	int derived_data;
};

struct base_xml
{
	base_xml();
	virtual ~base_xml();
	virtual string get_name();
	virtual int get_type();
	virtual void print(tree<base_xml>::iterator pos)
	{

	}
};

struct base_xml_tag: public base_xml
{
	string name;
};

#define XML_TAB 5

struct xml_table: base_xml_tag
{
	int columns;

	//...
	void tab_proc(tree<base_xml>::iterator pos)
	{

	}
	virtual int get_type()
	{
		return XML_TAB;
	};
};

void table_process(tree<base_xml>::iterator pos)
{
	xml_table& table = static_cast<xml_table&>(*pos);


}

void blah()
{
	tree<base_xml> wml;

	wml.insert(wml.begin(), xml_table());

	for(tree<base_xml>::iterator it = wml.begin(); it != wml.end(); it++)
	{
		auto& x = *it;

		switch(x.get_type())
		{
		case XML_TAB:
			static_cast<xml_table&>(x).tab_proc(it);
			break;
		}
		x.get_name();

	}
}


/*
struct ttree_node
{


};

struct ttree_iterator;

struct ttree
{
	ttree_node* node;

	ttree():node(0){};

	ttree(ttree_node*t):node(t){};

	ttree_iterator begin() ;

	///insert...
	void insert();
};

struct ttree_iterator
{
	ttree_node* node;

	ttree_iterator(ttree_node*t): node(t) {};

	ttree* operator->() {return (ttree*)this;};

	// operator ++, ---...
};

void test21312()
{
	ttree tr;

	ttree_iterator it = tr.begin();

	tr.insert();
	it->insert();
}


*/
void test()
{
//	string s;
//	const char* ptr = "Aaa";
//
//	s = "1234567";
//	s.insert(2, ptr, 10);
//
//
//	TRACELN("s(%u) = ", s.size());
//
//	for(auto const& c: s )
//		TRACE("%c", c);

//	 tree<string> tr;
//	tree<string>::iterator top, one, two, loc, banana;
//
//	top = tr.begin();
//	one = tr.insert(top, "one");
//	two = tr.append_child(one, "two");
//	tr.append_child(two, "apple");
//	banana = tr.append_child(two, "banana");
//	tr.append_child(banana, "cherry");
//	tr.append_child(two, "peach");
//	tr.append_child(one, "three");
//
//	print_tree(tr, tr.begin(), 0);

	tree<base> tr;

	derived my;

	my.base_data = 1;
	my.derived_data = 2;

	tr.insert(tr.begin(), my);

	base& r = *tr.begin();
	TRACELN("bd=%u", r.base_data);




//	loc = find(tr.begin(), tr.end(), "two");
//	if (loc != tr.end())
//	{
//		tree<string>::sibling_iterator sib = tr.begin(loc);
//		while (sib != tr.end(loc))
//		{
//			TRACE("%s\r\n", sib->c_str());
//			++sib;
//		}
//		TRACELN("--\r\n");
//		tree<string>::iterator sib2 = tr.begin(loc);
//		tree<string>::iterator end2 = tr.end(loc);
//		while (sib2 != end2)
//		{
//			for (int i = 0; i < tr.depth(sib2) - 2; ++i)
//				TRACE1(" ");
//			TRACE("%s\r\n", sib2->c_str());
//			++sib2;
//		}
//	}


}


#endif
