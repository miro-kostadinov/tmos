/*
 * ttl_tree.h
 *
 *  Created on: Apr 9, 2013
 *      Author: miro
 *
 *	This file is modified version of tree.hh. See comments that follow:
 */


//	STL-like templated tree class.
//
// Copyright (C) 2001-2011 Kasper Peeters <kasper@phi-sci.com>
// Distributed under the GNU General Public License version 3.
//
// When used together with the htmlcxx library to create
// HTML::Node template instances, the GNU Lesser General Public
// version 2 applies. Special permission to use tree.hh under
// the LGPL for other projects can be requested from the author.

/** \mainpage tree.hh
    \author   Kasper Peeters
    \version  2.81
    \date     23-Aug-2011
    \see      http://tree.phi-sci.com/
    \see      http://tree.phi-sci.com/ChangeLog

   The tree.hh library for C++ provides an STL-like container class
   for n-ary trees, templated over the data stored at the
   nodes. Various types of iterators are provided (post-order,
   pre-order, and others). Where possible the access methods are
   compatible with the STL or alternative algorithms are
   available.
*/


#ifndef TTL_TREE_H_
#define TTL_TREE_H_

#include <ttl_config.h>
#include <ttl_iterator.h>
#include <ttl_function.h>
#include <ttl_algo.h>
#include <ttl_type_traits.h>
#include <ttl_move.h>

#include <ttl_string.h>

//#define assert(x) ASSERT(x)

namespace ttl
{

class tree_head
{
public:
	tree_head *parent;
	tree_head *first_child, *last_child;
	tree_head *prev_sibling, *next_sibling;

	tree_head(): parent(0), first_child(0), last_child(0),
			prev_sibling(0), next_sibling(0) {;}

	virtual ~tree_head(){}

	tree_head& operator=(const tree_head& other)
	{
		parent = other.parent;
		first_child = other.first_child;
		last_child = other.last_child;
		prev_sibling = other.prev_sibling;
		next_sibling = other.next_sibling;
		return *this;
	}

	tree_head* attach_to(tree_head* position);
	tree_head* replace(tree_head* position);
};

/// A node in the tree, combining links to other nodes as well as the actual data.
template<class T>
class tree_node_ : public tree_head
{ // size: 5*4=20 bytes (on 32 bit arch), can be reduced by 8.
public:
	T data;
	static int id()
	{
		return reinterpret_cast<int>(&id);
	}

	tree_node_() : tree_head() {TRACELN("node type\"%X\" ", id());}
	tree_node_(const T& val) : tree_head(), data(val){TRACELN("node \"%X\" ", id());}
	tree_node_(T&& val) : tree_head(), data(ttl::forward<T>(val)) {TRACELN("node \"%X\" ", id());}

	virtual ~tree_node_() {TRACELN1("~tree_node_");}

	virtual tree_node_<T> *clone() const
	{
		return new tree_node_<T>(this->data);
	}

	virtual void *construct_data_at(void *p) const
	{
		return new(p) T(this->data);
	}

	virtual int size()
	{
		return sizeof(T);
	}

	virtual void destroy_data()
	{
		if(ttl::has_trivial_destructor<T>::value)
			return;
		data.~T();
	}
};
// __attribute__((packed));

template<class T>
class tree
{
protected:
	typedef tree_node_<T> tree_node;
public:
	/// Value of the data stored at a node.
	typedef T value_type;

	class iterator_base;
	class pre_order_iterator;
	class post_order_iterator;
	class sibling_iterator;
	class leaf_iterator;

	/// tree constructors/ destructors
	tree()	{ head_initialise_(); }
	tree(const T& x) { head_initialise_(); set_head(x);}
	tree(const iterator_base& other)
	{
		head_initialise_();set_head(other); replace(begin(), other);
	}
	tree(const tree<T>& other) {head_initialise_(); copy_(other);}
	tree(tree<T>&& other)
	{
		head = other.head;
		feet = other.feet;
		other.head = nullptr;
		other.feet = nullptr;
	}

	~tree() {clear(); delete head; delete feet; }

	/// tree operators
	tree<T>& operator=(const tree<T>& other)
	{
		if (this != &other)
			copy_(other);
		return *this;
	}


	/// Base class for iterators, only pointers stored, no traversal logic.
	class iterator_base
	{
	public:
		typedef T 							value_type;
		typedef T* 							pointer;
		typedef T& 							reference;
		typedef size_t 						size_type;
		typedef ptrdiff_t 					difference_type;
		typedef bidirectional_iterator_tag 	iterator_category;

		iterator_base() :
			node(nullptr), skip_current_children_(false) {}
		iterator_base(tree_head *th) :
			node(th), skip_current_children_(false) {}

		T& operator*() const
			{ return reinterpret_cast<tree_node *>(node)->data;}
		T* operator->() const
			{ return &(reinterpret_cast<tree_node *>(node)->data);}

		bool operator!=(const iterator_base& other) const
		{
			if (other.node != this->node)
				return true;
			else
				return false;
		}

		bool operator==(const iterator_base& other) const
		{
			if (other.node == this->node)
				return true;
			else
				return false;
		}

		/// When called, the next increment/decrement skips children of this node.
		void skip_children(bool skip) 	{ skip_current_children_ = skip;}
		void skip_children() 			{ skip_current_children_ = true;}


		/// Number of children of the node pointed to by the iterator.
		unsigned int number_of_children() const;

		/// clone iterator
		inline tree_head * clone() const
		{
			return reinterpret_cast<tree_head*>(reinterpret_cast<tree_node*>(node)->clone());
		}
		/// construct iterator
		inline tree_head * construct_data_at(tree_head* to) const
		{
			reinterpret_cast<tree_node*>(node)->construct_data_at(
					static_cast<void*>(&reinterpret_cast<tree_node*>(to)->data));
			return to;
		}

		inline void destroy_data() const
		{
			reinterpret_cast<tree_node*>(node)->destroy_data();
		}

		inline int id() const
		{
			return reinterpret_cast<tree_node*>(node)->id();
		}

		inline int size() const
		{
			return reinterpret_cast<tree_node*>(node)->size();
		}

		sibling_iterator begin() const
		{
			if (node->first_child == 0)
				return end();
			sibling_iterator ret(node->first_child);
			ret.parent_ = this->node;
			return ret;
		}

		sibling_iterator end() const
		{
			sibling_iterator ret;
			ret.parent_ = node;
			return ret;
		}
		//----------- for debug only --------------------
		void dump() const;

		tree_head *node;
	protected:
		bool skip_current_children_;
	};

	/// Depth-first iterator, first accessing the node, then its children.
	class pre_order_iterator: public iterator_base
	{
	public:
		pre_order_iterator() :
			iterator_base() {}
		pre_order_iterator(tree_head *th) :
			iterator_base(th){};
		pre_order_iterator(const iterator_base& other) :
			iterator_base(other.node) {}
		pre_order_iterator(const sibling_iterator& other) :
			iterator_base(other.node)
		{
			if(this->node == 0)
			{
				if (other.range_last() != 0)
					this->node = other.range_last();
				else
					this->node = other.parent_;
				this->skip_children();
				++(*this);
			}
		}

		pre_order_iterator& operator++();
		pre_order_iterator& operator--();
		pre_order_iterator operator++(int);
		pre_order_iterator operator--(int);
		pre_order_iterator& operator+=(unsigned int);
		pre_order_iterator& operator-=(unsigned int);
	};

	/// Depth-first iterator, first accessing the children, then the node itself.
	class post_order_iterator: public iterator_base
	{
	public:
		post_order_iterator() :
			iterator_base()	{}
		post_order_iterator(const iterator_base& other) :
			iterator_base(other.node) {}
		post_order_iterator(const sibling_iterator& other) :
			iterator_base(other.node)
		{
			if(this->node == 0)
			{
				if (other.range_last() != 0)
					this->node = other.range_last();
				else
					this->node = other.parent_;
				this->skip_children();
				++(*this);
			}
		}


		post_order_iterator& operator++();
		post_order_iterator& operator--();
		post_order_iterator operator++(int);
		post_order_iterator operator--(int);
		post_order_iterator& operator+=(unsigned int);
		post_order_iterator& operator-=(unsigned int);

		/// Set iterator to the first child as deep as possible down the tree.
		void descend_all();
	};


	/// The default iterator types throughout the tree class.
	typedef pre_order_iterator iterator;

	/// Iterator which traverses only the nodes at a given depth from the root.
	class fixed_depth_iterator: public iterator_base
	{
	public:
		fixed_depth_iterator() :
				iterator_base(), top_node(0) {}
		fixed_depth_iterator(const iterator_base& other) :
				iterator_base(other.node), top_node(0) {}
		fixed_depth_iterator(const sibling_iterator& other) :
				iterator_base(other.node), top_node(0) {}
		fixed_depth_iterator(const fixed_depth_iterator& other) :
				iterator_base(other.node), top_node(other.top_node) {}

		bool operator==(const fixed_depth_iterator&) const;
		bool operator!=(const fixed_depth_iterator&) const;
		fixed_depth_iterator& operator++();
		fixed_depth_iterator& operator--();
		fixed_depth_iterator operator++(int);
		fixed_depth_iterator operator--(int);
		fixed_depth_iterator& operator+=(unsigned int);
		fixed_depth_iterator& operator-=(unsigned int);

		tree_head *top_node;
	};

	/// Iterator which traverses only the nodes which are siblings of each other.
	class sibling_iterator: public iterator_base
	{
	public:
		sibling_iterator() : iterator_base()
			{ set_parent_(); }
		sibling_iterator(tree_head *th) : iterator_base(th)
			{ set_parent_(); }
		sibling_iterator(const iterator_base& other) : iterator_base(other.node)
			{ set_parent_(); }
		sibling_iterator(const sibling_iterator& other) :
				iterator_base(other), parent_(other.parent_) {}

		sibling_iterator& operator++();
		sibling_iterator& operator--();
		sibling_iterator operator++(int);
		sibling_iterator operator--(int);
		sibling_iterator& operator+=(unsigned int);
		sibling_iterator& operator-=(unsigned int);

		tree_node *range_first() const;
		tree_node *range_last() const;

		tree_head *parent_;
	private:
		void set_parent_()
		{
			parent_ = 0;
			if(this->node)
				parent_ =this->node->parent;
		}
	};

	/// Iterator which traverses only the leaves.
	class leaf_iterator: public iterator_base
	{
	public:
		leaf_iterator();
		leaf_iterator(tree_node *, tree_node *top = 0);
		leaf_iterator(const sibling_iterator&);
		leaf_iterator(const iterator_base&);

		bool operator==(const leaf_iterator&) const;
		bool operator!=(const leaf_iterator&) const;
		leaf_iterator& operator++();
		leaf_iterator& operator--();
		leaf_iterator operator++(int);
		leaf_iterator operator--(int);
		leaf_iterator& operator+=(unsigned int);
		leaf_iterator& operator-=(unsigned int);
	private:
		tree_node *top_node;
	};

	/// Return iterator to the beginning of the tree.
	inline pre_order_iterator begin() const;
	/// Return iterator to the end of the tree.
	inline pre_order_iterator end() const;
	/// Return post-order iterator to the beginning of the tree.
	post_order_iterator begin_post() const;
	/// Return post-order end iterator of the tree.
	post_order_iterator end_post() const;
	/// Return fixed-depth iterator to the first node at a given depth from the given iterator.
	fixed_depth_iterator begin_fixed(const iterator_base&, unsigned int) const;
	/// Return fixed-depth end iterator.
	fixed_depth_iterator end_fixed(const iterator_base&, unsigned int) const;

	/// Return sibling iterator to the first child of given node.
	sibling_iterator begin(const iterator_base&) const;
	/// Return sibling end iterator for children of given node.
	sibling_iterator end(const iterator_base&) const;
	/// Return leaf iterator to the first leaf of the tree.
	leaf_iterator begin_leaf() const;
	/// Return leaf end iterator for entire tree.
	leaf_iterator end_leaf() const;
	/// Return leaf iterator to the first leaf of the subtree at the given node.
	leaf_iterator begin_leaf(const iterator_base& top) const;
	/// Return leaf end iterator for the subtree at the given node.
	leaf_iterator end_leaf(const iterator_base& top) const;

	/// Return iterator to the parent of a node.
	template<typename iter> static iter parent(iter);
	/// Return iterator to the previous sibling of a node.
	template<typename iter> iter previous_sibling(iter) const;
	/// Return iterator to the next sibling of a node.
	template<typename iter> iter next_sibling(iter) const;
	/// Return iterator to the next node at a given depth.
	template<typename iter> iter next_at_same_depth(iter) const;

	/// Erase all nodes of the tree.
	void clear();
	/// Erase element at position pointed to by iterator, return incremented iterator.
	template<typename iter> iter erase(iter);
	/// Erase all children of the node pointed to by iterator.
	void erase_children(const iterator_base&);

	/// Insert empty node as last/first child of node pointed to by position.
	template<typename iter> iter append_child(iter position) const;
	template<typename iter> iter prepend_child(iter position);
	/// Insert node as last/first child of node pointed to by position.
	template<typename iter, typename Tr>
	iter append_child(iter position, const Tr& x);
	template<typename iter, typename Tr>
	iter append_child(iter position, Tr&& x);
	template<typename iter, typename Tr>
	iter prepend_child(iter position, const Tr& x);
	template<typename iter, typename Tr>
	iter prepend_child(iter position, Tr&& x);
	/// Append the node (plus its children) at other_position as last/first child of position.
	template<typename iter>
	iter append_child(iter position, iter other);
	template<typename iter>
	iter prepend_child(iter position, iter other);
	/// Append the nodes in the from-to range (plus their children) as last/first children of position.
	template<typename iter>
	iter append_children(iter position, sibling_iterator from,
			const sibling_iterator& to);
	template<typename iter>
	iter prepend_children(iter position, sibling_iterator from,
			const sibling_iterator& to);

	/// Short-hand to insert topmost node in otherwise empty tree.
	template<typename Tr>
	pre_order_iterator set_head(const Tr& x);
	template<typename Tr>
	pre_order_iterator set_head(Tr&& x);

	/// Insert (move) node as previous sibling of node pointed to by position.
	template<typename iter, typename Tr>
	iter insert(iter position, Tr&& x, ttl::false_type);
	/// Specialization of previous member.
	template<typename iter, typename Tr> // iter - sibling_iterator
	iter insert(iter position, Tr&& x, ttl::true_type);

	template<typename iter, typename Tr>
	iter insert(iter position, Tr&& x)
	{
		return insert(position, ttl::forward<Tr>(x),
				typename ttl::is_base_of<sibling_iterator, iter>::type());

	}
	/// Insert node as previous sibling of node pointed to by position.
	template<typename iter, typename Tr>
	iter insert(iter position, const Tr& x, ttl::false_type);
	/// Specialization of previous member.
	template<typename iter, typename Tr> // iter - sibling_iterator
	iter insert(iter position, const Tr& x, ttl::true_type);

	template<typename iter, typename Tr>
	iter insert(iter position, const Tr& x)
	{
		return insert(position, x,
				typename ttl::is_base_of<sibling_iterator, iter>::type());

	}

	/// Insert node (with children) pointed to by subtree as previous sibling of node pointed to by position.
	template<typename iter>
	iter insert_subtree(iter position, const iterator_base& subtr);
	/// Insert node as next sibling of node pointed to by position.
	template<typename iter, typename Tr>
	iter insert_after(iter position, const Tr& x);
	template<typename iter, typename Tr>
	iter insert_after(iter position, Tr&& x);
	/// Insert node (with children) pointed to by subtree as next sibling of node pointed to by position.
	template<typename iter>
	iter insert_subtree_after(iter position, const iterator_base& subtr);
//Replace
	/// Replace node at 'position' with other node (keeping same children); 'position' becomes invalid.
	template<typename iter, typename Tr>
	iter replace(iter position, const Tr& x, ttl::false_type);
	/// Replace node at 'position' with subtree starting at 'from' (do not erase subtree at 'from'); see above.
	template<typename iter, typename Tr>
	iter replace(iter position, const Tr& from, ttl::true_type);

	template<typename iter, typename Tr>
	iter replace(iter position, const Tr& x)
	{
		TRACELN1("replace lval");
		return replace(position, x,
				typename ttl::is_base_of<iterator_base,
						typename ttl::remove_reference<Tr>::type>::type());
	}

	template<typename iter, typename Tr>
	iter replace(iter position, Tr&& x, ttl::false_type);

	template<typename iter, typename Tr>
	iter replace(iter position, Tr&& x)
	{
		TRACELN1("replace rval");
		return replace(position, ttl::forward<Tr>(x),
				typename ttl::is_base_of<iterator_base,
						typename ttl::remove_reference<Tr>::type>::type());
	}

	/// Replace string of siblings (plus their children) with copy of a new string (with children); see above
	sibling_iterator replace(sibling_iterator orig_begin,
			const sibling_iterator& orig_end, sibling_iterator new_begin,
			const sibling_iterator& new_end);
//
	/// Move all children of node at 'position' to be siblings, returns position.
	template<typename iter> iter flatten(iter position);
	/// Move nodes in range to be children of 'position'.
	template<typename iter>
	iter reparent(iter position, sibling_iterator bg, const sibling_iterator& en);
	/// Move all child nodes of 'from' to be children of 'position'.
	template<typename iter>
	iter reparent(iter position, iter from);

	/// Replace node with a new node, making the old node a child of the new node.
	template<typename iter, typename Tr>
	iter wrap(iter position, const Tr& x);

	/// Move 'source' node (plus its children) to become the next sibling of 'target'.
	template<typename
	iter> iter move_after(iter target, iter source) const;
	/// Move 'source' node (plus its children) to become the previous sibling of 'target'.
	template<typename iter>
	iter move_before(iter target, iter source) const;

	sibling_iterator move_before(sibling_iterator target, sibling_iterator source) const;
	/// Move 'source' node (plus its children) to become the node at 'target' (erasing the node at 'target').
	template<typename iter>
	iter move_ontop(iter target, iter source);

	/// Merge with other tree, creating new branches and leaves only if they are not already present.
	void merge(sibling_iterator, sibling_iterator, sibling_iterator,
			const sibling_iterator&, bool duplicate_leaves = false);


	/// Compare two ranges of nodes (compares nodes as well as tree structure).
	template<typename iter>
	bool equal(const iter& one_, const iter& two, const iter& three_) const;

	template<typename iter, class BinaryPredicate>
	bool equal(const iter& one_, const iter& two, const iter& three_,
			BinaryPredicate) const;

	template<typename iter>
	bool equal_subtree(const iter& one_, const iter& two_) const;

	template<typename iter, class BinaryPredicate>
	bool equal_subtree(const iter& one_, const iter& two_, BinaryPredicate) const;

	/// Extract a new tree formed by the range of siblings plus all their children.
	tree subtree(sibling_iterator from, sibling_iterator to) const;
	void subtree(tree&, sibling_iterator from, sibling_iterator to) const;

	/// Exchange the node (plus subtree) with its sibling node (do nothing if no sibling present).
	void swap(sibling_iterator it);

	/// Exchange two nodes (plus subtrees)
	void swap(iterator, iterator);

	/// Count the total number of nodes.
	size_t size() const;
	/// Count the total number of nodes below the indicated node (plus one).
	size_t size(const iterator_base&) const;
	/// Check if tree is empty.
	bool empty() const;
	/// Compute the depth to the root or to a fixed other iterator.
	static int depth(const iterator_base&);
	static int depth(const iterator_base&, const iterator_base&);
	/// Determine the maximal depth of the tree. An empty tree has max_depth=-1.
	int max_depth() const;
	/// Determine the maximal depth of the tree with top node at the given position.
	int max_depth(const iterator_base&) const;
	/// Count the number of children of node at position.
	static unsigned int number_of_children(const iterator_base&);
	/// Count the number of siblings (left and right) of node at iterator. Total nodes at this level is +1.
	unsigned int number_of_siblings(const iterator_base&) const;
	/// Determine whether node at position is in the subtrees with root in the range.
	bool is_in_subtree(const iterator_base& it,
			const iterator_base& bg, const iterator_base& en) const;
	/// Determine whether the iterator is an 'end' iterator and thus not actually pointing to a node.
	bool is_valid(const iterator_base&) const;

	/// Determine the index of a node in the range of siblings to which it belongs.
	unsigned int index(sibling_iterator it) const;
	/// Inverse of 'index': return the n-th child of the node at position.
	static sibling_iterator child(const iterator_base& it, unsigned int);
	/// Return iterator to the sibling indicated by index
	sibling_iterator sibling(const iterator_base& it, unsigned int);

	/// For debugging only: verify internal consistency by inspecting all pointers in the tree
	/// (which will also trigger a valgrind error in case something got corrupted).
	void debug_verify_consistency() const;
	void dump() const;

	/// Comparator class for iterators (compares pointer values; why doesn't this work automatically?)
	class iterator_base_less
	{
	public:
		bool operator()(
				const typename tree<T>::iterator_base& one,
				const typename tree<T>::iterator_base& two) const
		{
			return one.node < two.node;
		}
	};

	tree_head *head, *feet; // head/feet are always dummy; if an iterator points to them it is invalid

private:
	template<typename Tr>
	tree_head* m_create_new_node(const Tr& x);
	template<typename Tr>
	tree_head* m_create_new_node(Tr&& x);
	template<class iter>
	iter m_append_child_node(iter position, tree_head* node) const;
	template<class iter>
	iter m_prepend_child_node(iter position, tree_head* node) const;
//	template<class iter>
	tree_head* m_copy_children(tree_head* it, const tree_head* subtr);
	bool m_is_valid_node(tree_head* node) const
	{
		if(node && node != head && node != feet)
			return true;
		TTL_THROW(EXCEPT_OUT_OF_RANGE);
		return false;
	}
//	template<typename Tr>
//
	void head_initialise_()
	{
		head = new tree_head;
		feet = new tree_head;
		head->next_sibling = feet; //head;
		feet->prev_sibling = head;
	}


	void copy_(const tree<T>& other);

	/// Comparator class for two nodes of a tree (used for sorting and searching).
	template<class StrictWeakOrdering>
	class compare_nodes
	{
	public:
		compare_nodes(StrictWeakOrdering comp) : comp_(comp) {}
		bool operator()(const tree_node *a, const tree_node *b)
		{
			return comp_(a->data, b->data);
		}
	private:
		StrictWeakOrdering comp_;
	};
};


// Tree


template<class T>
template<class Tr>
tree_head* tree<T>::m_create_new_node(const Tr& x)
{
	if(is_base_of<iterator_base, typename ttl::remove_reference<Tr>::type>::value)
		// create from iterator
		return reinterpret_cast<const iterator_base*>(&x)->clone();
	// create from node data
	return new tree_node_<typename ttl::remove_reference<Tr>::type>(x);
}

template<class T>
template<class Tr>
tree_head*  tree<T>::m_create_new_node(Tr&& x)
{
	if(is_base_of<iterator_base, typename ttl::remove_reference<Tr>::type>::value)
		// create from iterator
		return reinterpret_cast<const iterator_base*>(&x)->clone();
	// create from node data
	return new tree_node_<typename ttl::remove_reference<Tr>::type>(ttl::forward<Tr>(x));
}


template<class T>
void tree<T>::copy_(const tree<T>& other)
{
	clear();
	pre_order_iterator it = other.begin(), to = begin();
	while (it != other.end())
	{
		to = insert(to, it);
		it.skip_children();
		++it;
	}
	dump();
	to = begin();
	it = other.begin();
	while (it != other.end())
	{
		to = replace(to, it);
		to.skip_children();
		it.skip_children();
		++to;
		++it;
	}
}

template<class T>
void tree<T>::clear()
{
	if (head)
		while (head->next_sibling != feet)
			erase(pre_order_iterator(head->next_sibling));
}

template<class T>
void tree<T>::erase_children(const iterator_base& it)
{
	if (it.node == 0)
		return;

	tree_head *cur = it.node->first_child;
	tree_head *prev = 0;

	while (cur != 0)
	{
		prev = cur;
		cur = cur->next_sibling;
		erase_children(pre_order_iterator(prev));
		delete prev;
	}
	it.node->first_child = 0;
	it.node->last_child = 0;
//	std::cout << "exit" << std::endl;
}

template<class T>
template<class iter>
iter tree<T>::erase(iter it)
{
	tree_head *cur = it.node;

	if(cur == head)
	{
		clear();
		return head->next_sibling;
	}

	iter ret = it;
	ret.skip_children();
	++ret;
	erase_children(it);
	if (cur->prev_sibling == 0)
	{
		cur->parent->first_child = cur->next_sibling;
	}
	else
	{
		cur->prev_sibling->next_sibling = cur->next_sibling;
	}
	if (cur->next_sibling == 0)
	{
		cur->parent->last_child = cur->prev_sibling;
	}
	else
	{
		cur->next_sibling->prev_sibling = cur->prev_sibling;
	}

	delete cur;

	return ret;
}

template<class T>
typename tree<T>::pre_order_iterator tree<T>::begin() const
{
	return pre_order_iterator(head->next_sibling);
}

template<class T>
typename tree<T>::pre_order_iterator tree<T>::end() const
{
	return pre_order_iterator(feet);
}


template<class T>
typename tree<T>::post_order_iterator tree<T>::begin_post() const
{
	tree_head *tmp = head->next_sibling;
	if (tmp != feet)
	{
		while (tmp->first_child)
			tmp = tmp->first_child;
	}
	return post_order_iterator(tmp);
}

template<class T>
typename tree<T>::post_order_iterator tree<T>::end_post() const
{
	return post_order_iterator(feet);
}

template<class T>
typename tree<T>::fixed_depth_iterator tree<T>::begin_fixed(
		const iterator_base& pos, unsigned int dp) const
{
	typename tree<T>::fixed_depth_iterator ret;
	ret.top_node = pos.node;

	tree_head *tmp = pos.node;
	unsigned int curdepth = 0;
	while (curdepth < dp)
	{ // go down one level
		while (tmp->first_child == 0)
		{
			if (tmp->next_sibling == 0)
			{
				// try to walk up and then right again
				do
				{
					if (tmp == ret.top_node)
					{
						TTL_THROW(EXCEPT_OUT_OF_RANGE);
						return ret;
					}

					tmp = tmp->parent;
					if (tmp == 0)
					{
						TTL_THROW(EXCEPT_OUT_OF_RANGE);
						return ret;
					}
					--curdepth;
				} while (tmp->next_sibling == 0);
			}
			tmp = tmp->next_sibling;
		}
		tmp = tmp->first_child;
		++curdepth;
	}

	ret.node = tmp;
	return ret;
}

template<class T>
typename tree<T>::fixed_depth_iterator tree<T>::end_fixed(
		const iterator_base& pos, unsigned int dp) const
{
	//assert(1 == 0); // FIXME: not correct yet: use is_valid() as a temporary workaround
	tree_head *tmp = pos.node;
	unsigned int curdepth = 1;
	while (curdepth < dp)
	{ // go down one level
		while (tmp->first_child == 0)
		{
			tmp = tmp->next_sibling;
			if (tmp == 0)
			{
				TTL_THROW(EXCEPT_OUT_OF_RANGE);
				return tmp;
			}
		}
		tmp = tmp->first_child;
		++curdepth;
	}
	return tmp;
}

template<class T>
typename tree<T>::sibling_iterator tree<T>::begin(const iterator_base& pos) const
{
	if(pos.node != 0)
	{
		if (pos.node->first_child == 0)
		{
			return end(pos);
		}
		return pos.node->first_child;
	}
	TTL_THROW(EXCEPT_OUT_OF_RANGE);
	return pos;
}

template<class T>
typename tree<T>::sibling_iterator tree<T>::end(const iterator_base& pos) const
{
	sibling_iterator ret;
	ret.parent_ = pos.node;
	return ret;
}

template<class T>
typename tree<T>::leaf_iterator tree<T>::begin_leaf() const
{
	tree_head *tmp = head->next_sibling;
	if (tmp != feet)
	{
		while (tmp->first_child)
			tmp = tmp->first_child;
	}
	return leaf_iterator(tmp);
}

template<class T>
typename tree<T>::leaf_iterator tree<T>::end_leaf() const
{
	return leaf_iterator(feet);
}

template<class T>
typename tree<T>::leaf_iterator tree<T>::begin_leaf(
		const iterator_base& top) const
{
	tree_head *tmp = top.node;
	while (tmp->first_child)
		tmp = tmp->first_child;
	return leaf_iterator(tmp, top.node);
}

template<class T>
typename tree<T>::leaf_iterator tree<T>::end_leaf(
		const iterator_base& top) const
{
	return leaf_iterator(top.node, top.node);
}

template<class T>
template<typename iter>
iter tree<T>::parent(iter position)
{
	if(position.node != 0)
		return iter(position.node->parent);
	TTL_THROW(EXCEPT_OUT_OF_RANGE);
	return position;
}

template<class T>
template<typename iter>
iter tree<T>::previous_sibling(iter position) const
{
	if(position.node != 0)
		return iter(position.node->prev_sibling);
	TTL_THROW(EXCEPT_OUT_OF_RANGE);
	return position;
}

template<class T>
template<typename iter>
iter tree<T>::next_sibling(iter position) const
{
	if(position.node != 0)
		return iter(position.node->next_sibling);
	TTL_THROW(EXCEPT_OUT_OF_RANGE);
	return position;
}

template<class T>
template<typename iter>
iter tree<T>::next_at_same_depth(iter position) const
{
	// We make use of a temporary fixed_depth iterator to implement this.

	typename tree<T>::fixed_depth_iterator tmp(position.node);

	++tmp;
	return iter(tmp);
}

template<class T>
template<typename iter>
iter tree<T>::append_child(iter position) const
{
	if(!m_is_valid_node(position.node))
		return position;
	return m_append_child_node(position, new tree_node_<T>);
}

template<class T>
template<class iter, typename Tr>
iter tree<T>::append_child(iter position, const Tr& x)
{
	if(!m_is_valid_node(position.node))
		return position;
	return m_append_child_node(position, m_create_new_node(x));
}

template<class T>
template<class iter, typename Tr>
iter tree<T>::append_child(iter position, Tr&& x)
{
	if(!m_is_valid_node(position.node))
		return position;
	return m_append_child_node(position, m_create_new_node(forward<Tr>(x)));
}

template<class T>
template<class iter>
iter tree<T>::append_child(iter position, iter other)
{
	if(!m_is_valid_node(position.node))
		return position;

	sibling_iterator aargh = m_append_child_node(position, other.clone());
	return replace(aargh, other);
}

template<class T>
template<class iter>
iter tree<T>::m_append_child_node(iter position, tree_head* node) const
{
	node->parent = position.node;
	if (position.node->last_child != 0)
	{
		position.node->last_child->next_sibling = node;
	}
	else
	{
		position.node->first_child = node;
	}
	node->prev_sibling = position.node->last_child;
	position.node->last_child = node;
	node->next_sibling = 0;
	return node;
}

template<class T>
template<typename iter>
iter tree<T>::prepend_child(iter position)
{
	if(!m_is_valid_node(position.node))
		return position;
	return m_prepend_child_node(position, new tree_node_<T>);
}

template<class T>
template<class iter, typename Tr>
iter tree<T>::prepend_child(iter position, const Tr& x)
{
	if(!m_is_valid_node(position.node))
		return position;
	return m_prepend_child_node(position, m_create_new_node(x));
}

template<class T>
template<class iter, typename Tr>
iter tree<T>::prepend_child(iter position, Tr&& x)
{
	if(!m_is_valid_node(position.node))
		return position;
	return m_prepend_child_node(position, m_create_new_node(forward<Tr>(x)));
}

template<class T>
template<class iter>
iter tree<T>::prepend_child(iter position, iter other)
{
	if(!m_is_valid_node(position.node))
		return position;
	sibling_iterator aargh = prepend_child(position, other.clone());
	return replace(aargh, other);
}

template<class T>
template<class iter>
iter tree<T>::m_prepend_child_node(iter position, tree_head* node) const
{
	node->parent = position.node;
	if (position.node->first_child != 0)
	{
		position.node->first_child->prev_sibling = node;
	}
	else
	{
		position.node->last_child = node;
	}
	node->next_sibling = position.node->first_child;
	position.node->first_child = node;
	node->prev_sibling = 0;
	return node;
}


// appen_children(...)
template<class T>
template<class iter>
iter tree<T>::append_children(iter position, sibling_iterator from,
		const sibling_iterator& to)
{
	if(!m_is_valid_node(position.node))
		return position;

	iter ret = from;

	while (from != to)
	{
		insert_subtree(position.end(), from);
		++from;
	}
	return ret;
}

template<class T>
template<class iter>
iter tree<T>::prepend_children(iter position, sibling_iterator from,
		const sibling_iterator& to)
{
	if(!m_is_valid_node(position.node))
		return position;

	iter ret = from;
	if(from != to)
	{
		position = insert_subtree(position.begin(), from++);
		while (from != to)
		{
			position = insert_subtree_after(position, from);
			++from;
		}
	}
	return ret;
}

template<class T>
template<class Tr>
typename tree<T>::pre_order_iterator tree<T>::set_head(const Tr& x)
{
	if(head->next_sibling != feet)
		clear();
	return insert(iterator(feet), x);
}
template<class T>
template<class Tr>
typename tree<T>::pre_order_iterator tree<T>::set_head(Tr&& x)
{
	if(head->next_sibling != feet)
		clear();
	return insert(iterator(feet), ttl::forward<Tr>(x));
}

template<class T>
template<class iter, class Tr >
iter tree<T>::insert(iter position, Tr&& x, ttl::false_type)
{
	if (position.node == 0)
		position.node = feet; // Backward compatibility: when calling insert on a null node,
							  // insert before the feet.
	return  m_create_new_node(ttl::forward<Tr>(x))->attach_to(position.node);
}

// sibling iterator specialization
template<class T>
template<typename iter, typename Tr>
iter tree<T>::insert(iter position, Tr&& x, ttl::true_type)
{
	tree_head* tmp = m_create_new_node(ttl::forward<Tr>(x));
	if (position.node == 0)
	{ // iterator points to end of a subtree
		tmp->parent = position.parent_;
		tmp->prev_sibling = position.range_last();
		tmp->parent->last_child = tmp;
		return tmp->attach_to(nullptr);
	}
	return tmp->attach_to(position.node);
}

template<class T>
template<class iter, class Tr>
iter tree<T>::insert(iter position, const Tr& x, ttl::false_type)
{
	if (position.node == 0)
		position.node = feet; // Backward compatibility: when calling insert on a null node,
							  // insert before the feet.
	return m_create_new_node(x)->attach_to(position.node);
}

// sibling iterator specialization
template<class T>
template<typename iter, typename Tr>
iter tree<T>::insert(iter position, const Tr& x, ttl::true_type)
{
	tree_head* tmp = m_create_new_node(x);
	if (position.node == 0)
	{ // iterator points to end of a subtree
		tmp->parent = position.parent_;
		tmp->prev_sibling = position.range_last();
		tmp->parent->last_child = tmp;
		return tmp->attach_to(nullptr);
	}
	return tmp->attach_to(position.node);
}

template<class T>
template<class iter, typename Tr>
iter tree<T>::insert_after(iter position, const Tr& x)
{
	if(!position.node || position.node == feet)
		return this->insert(position, x);

	tree_head* tmp = m_create_new_node(x);
	tmp->parent = position.node->parent;
	tmp->prev_sibling = position.node;
	tmp->next_sibling = position.node->next_sibling;
	position.node->next_sibling = tmp;

	if (tmp->next_sibling == 0)
	{
		if (tmp->parent) // when inserting nodes at the head, there is no parent
			tmp->parent->last_child = tmp;
	}
	else
	{
		tmp->next_sibling->prev_sibling = tmp;
	}
	return tmp;
}

template<class T>
template<class iter, typename Tr>
iter tree<T>::insert_after(iter position, Tr&& x)
{
	if(!position.node || position.node == feet)
		return this->insert(position, forward<Tr>(x));

	tree_head* tmp = m_create_new_node(forward<Tr>(x));
	tmp->parent = position.node->parent;
	tmp->prev_sibling = position.node;
	tmp->next_sibling = position.node->next_sibling;
	position.node->next_sibling = tmp;

	if (tmp->next_sibling == 0)
	{
		if (tmp->parent) // when inserting nodes at the head, there is no parent
			tmp->parent->last_child = tmp;
	}
	else
	{
		tmp->next_sibling->prev_sibling = tmp;
	}
	return tmp;
}

template<class T>
template<class iter>
iter tree<T>::insert_subtree(iter position, const iterator_base& subtr)
{
	return m_copy_children(insert(position, subtr).node, subtr.node);
}

template<class T>
tree_head* tree<T>::m_copy_children(tree_head* it, const tree_head* subtr)
{
	const tree_head *current_from = subtr;
	const tree_head *start_from = subtr;

	// only at this stage can we fix 'last'
	tree_head* last = subtr->next_sibling;

	pre_order_iterator toit(it);
	// copy all children
	do
	{
		if(current_from != 0)
		{
			if (current_from->first_child != 0)
			{
				current_from = current_from->first_child;
				toit = m_append_child_node(toit, reinterpret_cast<const tree_node*>(current_from)->clone());
			}
			else
			{
				while (current_from->next_sibling == 0 && current_from != start_from)
				{
					current_from = current_from->parent;
					toit = parent(toit);
					if(current_from == nullptr)
					{
						TTL_THROW(EXCEPT_OUT_OF_RANGE);
						return it;
					}
				}
				current_from = current_from->next_sibling;
				if (current_from != last)
				{
					toit = m_append_child_node(parent(toit), reinterpret_cast<const tree_node*>(current_from)->clone());
				}
			}
		}
		else
		{
			TTL_THROW(EXCEPT_OUT_OF_RANGE);
			break;
		}
	} while (current_from != last);

	return it;
}

template<class T>
template<class iter>
iter tree<T>::insert_subtree_after(iter position, const iterator_base& subtr)
{
	return m_copy_children(insert_after(position, subtr).node, subtr.node);
}

template<class T>
template<class iter, typename Tr>
iter tree<T>::replace(iter position, const Tr& x, ttl::false_type)
{
	tree_head* current = position.node;
	if(current == nullptr || current == head)
	{
		TTL_THROW(EXCEPT_OUT_OF_RANGE);
		return position;
	}

	if(current == feet)
		return insert(position, x);

	if(position.size()== sizeof(Tr))
	{
		position.destroy_data();
		::new(static_cast<void*>(&reinterpret_cast<tree_node*>(current)->data)) Tr(x);
		return position;
	}

	tree_head* tmp = m_create_new_node(x);
	tmp->replace(current);

	iterator from = begin(current);
	iterator to = end(current);

	tmp->first_child = from.node;
	tmp->last_child = to.node;
	while(from != to )
	{
		from.node->parent = tmp;
		from++;
	}
	delete current;

	return tmp;
}

template<class T>
template<class iter, class Tr>
iter tree<T>::replace(iter position, const Tr& from, ttl::true_type)
{
	tree_head *current = position.node;
	if(current == nullptr || current == head)
	{
		TTL_THROW(EXCEPT_OUT_OF_RANGE);
		return position;
	}

	if(current == feet )
		return insert(position, from);

	tree_head* tmp;
	erase_children(position);
	if(position.size() == from.size())
	{
		position.destroy_data();
		tmp = from.construct_data_at(current);
	}
	else
	{
		tmp = m_create_new_node(from);
		tmp->replace(current);
		delete current;
	}
	return m_copy_children(tmp, from.node);
}

template<class T>
template<class iter, typename Tr>
iter tree<T>::replace(iter position, Tr&& x, ttl::false_type)
{
	tree_head* current = position.node;
	if(current == nullptr || current == head)
	{
		TTL_THROW(EXCEPT_OUT_OF_RANGE);
		return position;
	}

	if(current == feet )
		return insert(position, ttl::forward<Tr>(x));

	if(position.size()== sizeof(Tr))
	{
		position.destroy_data();
		::new (static_cast<void*>(&reinterpret_cast<tree_node*>(current)->data))
				typename ttl::remove_reference<Tr>::type(ttl::forward<Tr>(x));
		return position;
	}

	tree_head* tmp = m_create_new_node(ttl::forward<Tr>(x));
	tmp->replace(current);

	iterator from = begin(current);
	iterator to = end(current);

	tmp->first_child = from.node;
	tmp->last_child = to.node;
	while(from != to )
	{
		from.node->parent = tmp;
		from++;
	}
	delete current;

	return tmp;
}

template<class T>
typename tree<T>::sibling_iterator tree<T>::replace(
		sibling_iterator orig_begin, const sibling_iterator& orig_end,
		sibling_iterator new_begin, const sibling_iterator& new_end)
{
	tree_node *orig_first = orig_begin.node;
	tree_node *new_first = new_begin.node;
	tree_node *orig_last = orig_first;
	while ((++orig_begin) != orig_end)
		orig_last = orig_last->next_sibling;
	tree_node *new_last = new_first;
	while ((++new_begin) != new_end)
		new_last = new_last->next_sibling;

	// insert all siblings in new_first..new_last before orig_first
	bool first = true;
	pre_order_iterator ret;
	while (1 == 1)
	{
		pre_order_iterator tt = insert_subtree(pre_order_iterator(orig_first),
				pre_order_iterator(new_first));
		if (first)
		{
			ret = tt;
			first = false;
		}
		if (new_first == new_last)
			break;
		new_first = new_first->next_sibling;
	}

	// erase old range of siblings
	bool last = false;
	tree_node *next = orig_first;
	while (1 == 1)
	{
		if (next == orig_last)
			last = true;
		next = next->next_sibling;
		erase((pre_order_iterator) orig_first);
		if (last)
			break;
		orig_first = next;
	}
	return ret;
}

template<class T>
template<typename iter>
iter tree<T>::flatten(iter position)
{
	if (position.node->first_child == 0)
		return position;

	tree_head *tmp = position.node->first_child;
	while (tmp)
	{
		tmp->parent = position.node->parent;
		tmp = tmp->next_sibling;
	}
	if (position.node->next_sibling)
	{
		position.node->last_child->next_sibling = position.node->next_sibling;
		position.node->next_sibling->prev_sibling = position.node->last_child;
	}
	else
	{
		position.node->parent->last_child = position.node->last_child;
	}
	position.node->next_sibling = position.node->first_child;
	position.node->next_sibling->prev_sibling = position.node;
	position.node->first_child = 0;
	position.node->last_child = 0;

	return position;
}

template<class T>
template<typename iter>
iter tree<T>::reparent(iter position, sibling_iterator bg,
		const sibling_iterator& en)
{
	tree_head *first = bg.node;
	tree_head *last = first;

	if(first == position.node)
		return position;

	if (bg == en)
		return bg;
	// determine last node
	while ((++bg) != en)
	{
		last = last->next_sibling;
	}
	// move subtree
	if (first->prev_sibling == 0)
	{
		first->parent->first_child = last->next_sibling;
	}
	else
	{
		first->prev_sibling->next_sibling = last->next_sibling;
	}
	if (last->next_sibling == 0)
	{
		last->parent->last_child = first->prev_sibling;
	}
	else
	{
		last->next_sibling->prev_sibling = first->prev_sibling;
	}
	if (position.node->first_child == 0)
	{
		position.node->first_child = first;
		position.node->last_child = last;
		first->prev_sibling = 0;
	}
	else
	{
		position.node->last_child->next_sibling = first;
		first->prev_sibling = position.node->last_child;
		position.node->last_child = last;
	}
	last->next_sibling = 0;

	tree_head *pos = first;
	for (;;)
	{
		pos->parent = position.node;
		if (pos == last)
			break;
		pos = pos->next_sibling;
	}

	return first;
}

template<class T>
template<typename iter> iter tree<T>::reparent(iter position, iter from)
{
	if (from.node->first_child == 0)
		return position;
	return reparent(position, from.node->first_child, end(from));
}

template<class T>
template<typename iter, typename Tr> iter tree<T>::wrap(iter position, const Tr& x)
{
	if(position.node == 0)
		return position;
	sibling_iterator fr = position, to = position;
	++to;
	iter ret = insert(position, x);
	reparent(ret, fr, to);
	return ret;
}

template<class T>
template<typename iter> iter tree<T>::move_after(iter target, iter source) const
{
	tree_head *dst = target.node;
	tree_head *src = source.node;

	if(dst == nullptr || src == nullptr)
		return source;

	if (dst == src)
		return source;
	if (dst->next_sibling)
		if (dst->next_sibling == src) // already in the right spot
			return source;

	// take src out of the tree
	if (src->prev_sibling != 0)
		src->prev_sibling->next_sibling = src->next_sibling;
	else
		src->parent->first_child = src->next_sibling;
	if (src->next_sibling != 0)
		src->next_sibling->prev_sibling = src->prev_sibling;
	else
		src->parent->last_child = src->prev_sibling;

	// connect it to the new point
	if (dst->next_sibling != 0)
		dst->next_sibling->prev_sibling = src;
	else
		dst->parent->last_child = src;
	src->next_sibling = dst->next_sibling;
	dst->next_sibling = src;
	src->prev_sibling = dst;
	src->parent = dst->parent;
	return src;
}

template<class T>
template<typename iter> iter tree<T>::move_before(iter target, iter source) const
{
	tree_head *dst = target.node;
	tree_head *src = source.node;

	if(dst == nullptr || src == nullptr)
		return source;

	if (dst == src)
		return source;
	if (dst->prev_sibling)
		if (dst->prev_sibling == src) // already in the right spot
			return source;

	// take src out of the tree
	if (src->prev_sibling != 0)
		src->prev_sibling->next_sibling = src->next_sibling;
	else
		src->parent->first_child = src->next_sibling;
	if (src->next_sibling != 0)
		src->next_sibling->prev_sibling = src->prev_sibling;
	else
		src->parent->last_child = src->prev_sibling;

	// connect it to the new point
	if (dst->prev_sibling != 0)
		dst->prev_sibling->next_sibling = src;
	else
		dst->parent->first_child = src;
	src->prev_sibling = dst->prev_sibling;
	dst->prev_sibling = src;
	src->next_sibling = dst;
	src->parent = dst->parent;
	return src;
}

// specialisation for sibling_iterators
template<class T>
typename tree<T>::sibling_iterator tree<T>::move_before(
		sibling_iterator target, sibling_iterator source) const
{
	tree_head *dst = target.node;
	tree_head *src = source.node;
	tree_head *dst_prev_sibling;
	if (dst == 0)
	{ // must then be an end iterator
		dst_prev_sibling = target.parent_->last_child;
		if(dst_prev_sibling == nullptr)
		{
			TTL_THROW(EXCEPT_OUT_OF_RANGE);
			return source;
		}
	}
	else
		dst_prev_sibling = dst->prev_sibling;
	if(src == nullptr)
	{
		TTL_THROW(EXCEPT_OUT_OF_RANGE);
		return source;
	}

	if (dst == src)
		return source;
	if (dst_prev_sibling)
		if (dst_prev_sibling == src) // already in the right spot
			return source;

	// take src out of the tree
	if (src->prev_sibling != 0)
		src->prev_sibling->next_sibling = src->next_sibling;
	else
		src->parent->first_child = src->next_sibling;
	if (src->next_sibling != 0)
		src->next_sibling->prev_sibling = src->prev_sibling;
	else
		src->parent->last_child = src->prev_sibling;

	// connect it to the new point
	if (dst_prev_sibling != 0)
		dst_prev_sibling->next_sibling = src;
	else
		target.parent_->first_child = src;
	src->prev_sibling = dst_prev_sibling;
	if (dst)
	{
		dst->prev_sibling = src;
		src->parent = dst->parent;
	}
	src->next_sibling = dst;
	return src;
}

template<class T>
template<typename iter> iter tree<T>::move_ontop(iter target, iter source)
{
	tree_head *dst = target.node;
	tree_head *src = source.node;

	if(dst == nullptr || src == nullptr)
		return source;
	if (dst == src)
		return source;

	// remember connection points
	tree_head *b_prev_sibling = dst->prev_sibling;
	tree_head *b_next_sibling = dst->next_sibling;
	tree_head *b_parent = dst->parent;

	// remove target
	erase(target);

	// take src out of the tree
	if (src->prev_sibling != 0)
		src->prev_sibling->next_sibling = src->next_sibling;
	else
		src->parent->first_child = src->next_sibling;
	if (src->next_sibling != 0)
		src->next_sibling->prev_sibling = src->prev_sibling;
	else
		src->parent->last_child = src->prev_sibling;

	// connect it to the new point
	if (b_prev_sibling != 0)
		b_prev_sibling->next_sibling = src;
	else
		b_parent->first_child = src;
	if (b_next_sibling != 0)
		b_next_sibling->prev_sibling = src;
	else
		b_parent->last_child = src;
	src->prev_sibling = b_prev_sibling;
	src->next_sibling = b_next_sibling;
	src->parent = b_parent;
	return src;
}

template<class T>
void tree<T>::merge(sibling_iterator to1, sibling_iterator to2,
		sibling_iterator from1, const sibling_iterator& from2, bool duplicate_leaves)
{
	sibling_iterator fnd;
	while (from1 != from2)
	{
		if ((fnd = ttl::find(to1, to2, (*from1))) != to2)
		{ // element found
			if (from1.begin() == from1.end())
			{ // full depth reached
				if (duplicate_leaves)
					append_child(parent(to1), (*from1));
			}
			else
			{ // descend further
				merge(fnd.begin(), fnd.end(), from1.begin(), from1.end(),
						duplicate_leaves);
			}
		}
		else
		{ // element missing
			insert_subtree(to2, from1);
		}
		++from1;
	}
}


template<class T>
template<typename iter>
bool tree<T>::equal(const iter& one_, const iter& two, const iter& three_) const
{
	ttl::equal_to<T> comp;
	return equal(one_, two, three_, comp);
}

template<class T>
template<typename iter>
bool tree<T>::equal_subtree(const iter& one_, const iter& two_) const
{
	ttl::equal_to<T> comp;
	return equal_subtree(one_, two_, comp);
}

template<class T>
template<typename iter, class BinaryPredicate>
bool tree<T>::equal(const iter& one_, const iter& two, const iter& three_,
		BinaryPredicate fun) const
{
	pre_order_iterator one(one_), three(three_);

//	if(one==two && is_valid(three) && three.number_of_children()!=0)
//		return false;
	while (one != two && is_valid(three))
	{
		if (!fun(*one, *three))
			return false;
		if (one.number_of_children() != three.number_of_children())
			return false;
		++one;
		++three;
	}
	return true;
}

template<class T>
template<typename iter, class BinaryPredicate>
bool tree<T>::equal_subtree(const iter& one_, const iter& two_,
		BinaryPredicate fun) const
{
	pre_order_iterator one(one_), two(two_);

	if (!fun(*one, *two))
		return false;
	if (number_of_children(one) != number_of_children(two))
		return false;
	return equal(begin(one), end(one), begin(two), fun);
}

template<class T>
tree<T> tree<T>::subtree(sibling_iterator from, sibling_iterator to) const
{
	tree tmp;
	tmp.set_head(from);
	tmp.replace(tmp.begin(), tmp.end(), from, to);
	return tmp;
}

template<class T>
void tree<T>::subtree(tree& tmp, sibling_iterator from,
		sibling_iterator to) const
{
	tmp.set_head(from);
	tmp.replace(tmp.begin(), tmp.end(), from, to);
}

template<class T>
size_t tree<T>::size() const
{
	size_t i = 0;
	pre_order_iterator it = begin(), eit = end();
	while (it != eit)
	{
		++i;
		++it;
	}
	return i;
}

template<class T>
size_t tree<T>::size(const iterator_base& top) const
{
	size_t i = 0;
	pre_order_iterator it = top, eit = top;
	eit.skip_children();
	++eit;
	while (it != eit)
	{
		++i;
		++it;
	}
	return i;
}

template<class T>
bool tree<T>::empty() const
{
	return (head->next_sibling == feet);
}

template<class T>
int tree<T>::depth(const iterator_base& it)
{
	tree_head* pos = it.node;
	if(pos == nullptr)
		return -1;
	int ret = 0;
	while (pos->parent != 0)
	{
		pos = pos->parent;
		++ret;
	}
	return ret;
}

template<class T>
int tree<T>::depth(const iterator_base& it, const iterator_base& root)
{
	tree_head* pos = it.node;
	if(pos == nullptr)
		return -1;

	int ret = 0;
	while (pos->parent != 0 && pos != root.node)
	{
		pos = pos->parent;
		++ret;
	}
	return ret;
}

template<class T>
int tree<T>::max_depth() const
{
	int maxd = -1;
	for (tree_head *it = head->next_sibling; it != feet; it = it->next_sibling)
		maxd = max(maxd, max_depth(it));

	return maxd;
}

template<class T>
int tree<T>::max_depth(const iterator_base& pos) const
{
	tree_head *tmp = pos.node;

	if (tmp == 0 || tmp == head || tmp == feet)
		return -1;

	int curdepth = 0, maxdepth = 0;
	while (true)
	{ // try to walk the bottom of the tree
		while (tmp->first_child == 0)
		{
			if (tmp == pos.node)
				return maxdepth;
			if (tmp->next_sibling == 0)
			{
				// try to walk up and then right again
				do
				{
					tmp = tmp->parent;
					if (tmp == 0)
						return maxdepth;
					--curdepth;
				} while (tmp->next_sibling == 0);
			}
			if (tmp == pos.node)
				return maxdepth;
			tmp = tmp->next_sibling;
		}
		tmp = tmp->first_child;
		++curdepth;
		maxdepth = max(curdepth, maxdepth);
	}
}

template<class T>
unsigned int tree<T>::number_of_children(const iterator_base& it)
{
	tree_head *pos = it.node;
	if (pos == nullptr || (pos=pos->first_child) == nullptr)
		return 0;

	unsigned int ret = 1;
	while ((pos = pos->next_sibling))
		++ret;
	return ret;
}

template<class T>
unsigned int tree<T>::number_of_siblings(const iterator_base& it) const
{
	tree_head *pos = it.node;
	unsigned int ret = 0;
	if(pos)
	{
		// count forward
			while (pos->next_sibling && pos->next_sibling != head
					&& pos->next_sibling != feet)
			{
				++ret;
				pos = pos->next_sibling;
			}
		// count backward
			pos = it.node;
			while (pos->prev_sibling && pos->prev_sibling != head
					&& pos->prev_sibling != feet)
			{
				++ret;
				pos = pos->prev_sibling;
			}
	}
	return ret;
}

template<class T>
void tree<T>::swap(sibling_iterator it)
{
	tree_head *nxt = it.node;
	if (nxt &&(nxt=nxt->next_sibling))
	{
		if (it.node->prev_sibling)
			it.node->prev_sibling->next_sibling = nxt;
		else
			it.node->parent->first_child = nxt;
		nxt->prev_sibling = it.node->prev_sibling;
		tree_head *nxtnxt = nxt->next_sibling;
		if (nxtnxt)
			nxtnxt->prev_sibling = it.node;
		else
			it.node->parent->last_child = it.node;
		nxt->next_sibling = it.node;
		it.node->prev_sibling = nxt;
		it.node->next_sibling = nxtnxt;
	}
}

template<class T>
void tree<T>::swap(iterator one, iterator two)
{
// if one and two are adjacent siblings, use the sibling swap
	if (one.node->next_sibling == two.node)
		swap(one);
	else if (two.node->next_sibling == one.node)
		swap(two);
	else
	{
		tree_head *nxt1 = one.node->next_sibling;
		tree_head *nxt2 = two.node->next_sibling;
		tree_head *pre1 = one.node->prev_sibling;
		tree_head *pre2 = two.node->prev_sibling;
		tree_head *par1 = one.node->parent;
		tree_head *par2 = two.node->parent;

		// reconnect
		one.node->parent = par2;
		one.node->next_sibling = nxt2;
		if (nxt2)
			nxt2->prev_sibling = one.node;
		else
			par2->last_child = one.node;
		one.node->prev_sibling = pre2;
		if (pre2)
			pre2->next_sibling = one.node;
		else
			par2->first_child = one.node;

		two.node->parent = par1;
		two.node->next_sibling = nxt1;
		if (nxt1)
			nxt1->prev_sibling = two.node;
		else
			par1->last_child = two.node;
		two.node->prev_sibling = pre1;
		if (pre1)
			pre1->next_sibling = two.node;
		else
			par1->first_child = two.node;
	}
}

// template <class BinaryPredicate>
// tree<T>::iterator tree<T>::find_subtree(
// 	sibling_iterator subfrom, sibling_iterator subto, iterator from, iterator to,
// 	BinaryPredicate fun) const
// 	{
// 	assert(1==0); // this routine is not finished yet.
// 	while(from!=to) {
// 		if(fun(*subfrom, *from)) {
//
// 			}
// 		}
// 	return to;
// 	}

template<class T>
bool tree<T>::is_in_subtree(const iterator_base& it, const iterator_base& bg,
		const iterator_base& en) const
{
// FIXME: this should be optimised.
	pre_order_iterator tmp = bg;
	while (tmp != en)
	{
		if (tmp == it)
			return true;
		++tmp;
	}
	return false;
}

template<class T>
bool tree<T>::is_valid(const iterator_base& it) const
{
	if (it.node == 0 || it.node == feet || it.node == head)
		return false;
	else
		return true;
}

template<class T>
unsigned int tree<T>::index(sibling_iterator it) const
{
	unsigned int ind = 0;
	if(it.node)
	{
		if (it.node->parent == 0)
		{
			while (it.node->prev_sibling != head)
			{
				it.node = it.node->prev_sibling;
				++ind;
			}
		}
		else
		{
			while (it.node->prev_sibling != 0)
			{
				it.node = it.node->prev_sibling;
				++ind;
			}
		}
	}
	return ind;
}

template<class T>
typename tree<T>::sibling_iterator tree<T>::sibling(const iterator_base& it,
		unsigned int num)
{
	tree_head *tmp=nullptr;
	if(it.node)
	{
		if (it.node->parent == 0)
		{
			tmp = head->next_sibling;
			while (num)
			{
				if(tmp == nullptr)
					break;
				tmp = tmp->next_sibling;
				--num;
			}
		}
		else
		{
			tmp = it.node->parent->first_child;
			while (num)
			{
				if(tmp == nullptr)
					break;
				tmp = tmp->next_sibling;
				--num;
			}
		}
	}
	return tmp;
}


template<class T>
void tree<T>::dump() const
{
	iterator it = begin();
	while (it != end())
	{
		it++.dump();
		tsk_sleep(5);
	}

}

template<class T>
typename tree<T>::sibling_iterator tree<T>::child(const iterator_base& it,
		unsigned int num)
{
	tree_head *tmp = it.node;
	if(tmp && (tmp=tmp->first_child))
		while (num--)
		{
			if(tmp == nullptr)
				break;
			tmp = tmp->next_sibling;
		}
	return tmp;
}

// Iterator base
template<class T>
unsigned int tree<T>::iterator_base::number_of_children() const
{
	tree_head *pos = node->first_child;
	if (pos == nullptr ||(pos=pos->first_child)== nullptr )
		return 0;

	unsigned int ret = 1;
	while (pos != node->last_child)
	{
		++ret;
		pos = pos->next_sibling;
	}
	return ret;
}

template<class T>
typename tree<T>::pre_order_iterator& tree<T>::pre_order_iterator::operator++()
{
	if(this->node)
	{
		if (!this->skip_current_children_ && this->node->first_child != 0)
		{
			this->node = this->node->first_child;
		}
		else
		{
			this->skip_current_children_ = false;
			while (this->node->next_sibling == 0)
			{
				this->node = this->node->parent;
				if (this->node == 0)
					return *this;
			}
			this->node = this->node->next_sibling;
		}
	}
	return *this;
}

template<class T>
typename tree<T>::pre_order_iterator& tree<T>::pre_order_iterator::operator--()
{
	if(this->node)
	{
		if (this->node->prev_sibling)
		{
			this->node = this->node->prev_sibling;
			while (this->node->last_child)
				this->node = this->node->last_child;
		}
		else
		{
			this->node = this->node->parent;
			if (this->node == 0)
				return *this;
		}
	}
	return *this;
}

template<class T>
typename tree<T>::pre_order_iterator tree<T>::pre_order_iterator::operator++(
		int)
{
	pre_order_iterator copy = *this;
	++(*this);
	return copy;
}

template<class T>
typename tree<T>::pre_order_iterator tree<T>::pre_order_iterator::operator--(
		int)
{
	pre_order_iterator copy = *this;
	--(*this);
	return copy;
}

template<class T>
typename tree<T>::pre_order_iterator& tree<T>::pre_order_iterator::operator+=(
		unsigned int num)
{
	while (num > 0)
	{
		++(*this);
		--num;
	}
	return (*this);
}

template<class T>
typename tree<T>::pre_order_iterator& tree<T>::pre_order_iterator::operator-=(
		unsigned int num)
{
	while (num > 0)
	{
		--(*this);
		--num;
	}
	return (*this);
}

// Post-order iterator


template<class T>
typename tree<T>::post_order_iterator& tree<T>::post_order_iterator::operator++()
{
	if(this->node)
	{
		if (this->node->next_sibling == 0)
		{
			this->node = this->node->parent;
			this->skip_current_children_ = false;
		}
		else
		{
			this->node = this->node->next_sibling;
			if (this->skip_current_children_)
			{
				this->skip_current_children_ = false;
			}
			else
			{
				while (this->node->first_child)
					this->node = this->node->first_child;
			}
		}
	}
	return *this;
}

template<class T>
typename tree<T>::post_order_iterator& tree<T>::post_order_iterator::operator--()
{
	if(this->node)
	{
		if (this->skip_current_children_ || this->node->last_child == 0)
		{
			this->skip_current_children_ = false;
			while (this->node->prev_sibling == 0)
				this->node = this->node->parent;
			this->node = this->node->prev_sibling;
		}
		else
		{
			this->node = this->node->last_child;
		}
	}
	return *this;
}

template<class T>
typename tree<T>::post_order_iterator tree<T>::post_order_iterator::operator++(
		int)
{
	post_order_iterator copy = *this;
	++(*this);
	return copy;
}

template<class T>
typename tree<T>::post_order_iterator tree<T>::post_order_iterator::operator--(
		int)
{
	post_order_iterator copy = *this;
	--(*this);
	return copy;
}

template<class T>
typename tree<T>::post_order_iterator& tree<T>::post_order_iterator::operator+=(
		unsigned int num)
{
	while (num > 0)
	{
		++(*this);
		--num;
	}
	return (*this);
}

template<class T>
typename tree<T>::post_order_iterator& tree<T>::post_order_iterator::operator-=(
		unsigned int num)
{
	while (num > 0)
	{
		--(*this);
		--num;
	}
	return (*this);
}

template<class T>
void tree<T>::post_order_iterator::descend_all()
{
	if(this->node)
	{
		while (this->node->first_child)
			this->node = this->node->first_child;
	}
}

// Fixed depth iterator


template<class T>
bool tree<T>::fixed_depth_iterator::operator==(
		const fixed_depth_iterator& other) const
{
	if (other.node == this->node && other.top_node == top_node)
		return true;
	else
		return false;
}

template<class T>
bool tree<T>::fixed_depth_iterator::operator!=(
		const fixed_depth_iterator& other) const
{
	if (other.node != this->node || other.top_node != top_node)
		return true;
	else
		return false;
}

template<class T>
typename tree<T>::fixed_depth_iterator& tree<T>::fixed_depth_iterator::operator++()
{
	if(this->node)
	{
		if (this->node->next_sibling)
		{
			this->node = this->node->next_sibling;
		}
		else
		{
			int relative_depth = 0;
			upper: do
			{
				if (this->node == this->top_node)
				{
					this->node = 0; // FIXME: return a proper fixed_depth end iterator once implemented
					return *this;
				}
				this->node = this->node->parent;
				if (this->node == 0)
					return *this;
				--relative_depth;
			} while (this->node->next_sibling == 0);
			lower: this->node = this->node->next_sibling;
			while (this->node->first_child == 0)
			{
				if (this->node->next_sibling == 0)
					goto upper;
				this->node = this->node->next_sibling;
				if (this->node == 0)
					return *this;
			}
			while (relative_depth < 0 && this->node->first_child != 0)
			{
				this->node = this->node->first_child;
				++relative_depth;
			}
			if (relative_depth < 0)
			{
				if (this->node->next_sibling == 0)
					goto upper;
				else
					goto lower;
			}
		}
	}
	return *this;
}

template<class T>
typename tree<T>::fixed_depth_iterator& tree<T>::fixed_depth_iterator::operator--()
{

	if(this->node)
	{
		if (this->node->prev_sibling)
		{
			this->node = this->node->prev_sibling;
		}
		else
		{
			int relative_depth = 0;

			upper: do
			{
				if (this->node == this->top_node)
				{
					this->node = 0;
					return *this;
				}
				this->node = this->node->parent;
				if (this->node == 0)
					return *this;
				--relative_depth;
			} while (this->node->prev_sibling == 0);

			lower: this->node = this->node->prev_sibling;
			while (this->node->last_child == 0)
			{
				if (this->node->prev_sibling == 0)
					goto upper;
				this->node = this->node->prev_sibling;
				if (this->node == 0)
					return *this;
			}
			while (relative_depth < 0 && this->node->last_child != 0)
			{
				this->node = this->node->last_child;
				++relative_depth;
			}

			if (relative_depth < 0)
			{
				if (this->node->prev_sibling == 0)
					goto upper;
				else
					goto lower;
			}
		}
	}
	return *this;

//
//
//	assert(this->node!=0);
//	if(this->node->prev_sibling!=0) {
//		this->node=this->node->prev_sibling;
//		assert(this->node!=0);
//		if(this->node->parent==0 && this->node->prev_sibling==0) // head element
//			this->node=0;
//		}
//	else {
//		tree_node *par=this->node->parent;
//		do {
//			par=par->prev_sibling;
//			if(par==0) { // FIXME: need to keep track of this!
//				this->node=0;
//				return *this;
//				}
//			} while(par->last_child==0);
//		this->node=par->last_child;
//		}
//	return *this;
}

template<class T>
typename tree<T>::fixed_depth_iterator tree<T>::fixed_depth_iterator::operator++(
		int)
{
	fixed_depth_iterator copy = *this;
	++(*this);
	return copy;
}

template<class T>
typename tree<T>::fixed_depth_iterator tree<T>::fixed_depth_iterator::operator--(
		int)
{
	fixed_depth_iterator copy = *this;
	--(*this);
	return copy;
}

template<class T>
typename tree<T>::fixed_depth_iterator& tree<T>::fixed_depth_iterator::operator-=(
		unsigned int num)
{
	while (num > 0)
	{
		--(*this);
		--(num);
	}
	return (*this);
}

template<class T>
typename tree<T>::fixed_depth_iterator& tree<T>::fixed_depth_iterator::operator+=(
		unsigned int num)
{
	while (num > 0)
	{
		++(*this);
		--(num);
	}
	return *this;
}

// Sibling iterator

template<class T>
typename tree<T>::sibling_iterator& tree<T>::sibling_iterator::operator++()
{
	if (this->node)
		this->node = this->node->next_sibling;
	return *this;
}

template<class T>
typename tree<T>::sibling_iterator& tree<T>::sibling_iterator::operator--()
{
	if (this->node)
		this->node = this->node->prev_sibling;
	else
	{
		if(parent_)
			this->node = parent_->last_child;
	}
	return *this;
}

template<class T>
typename tree<T>::sibling_iterator tree<T>::sibling_iterator::operator++(int)
{
	sibling_iterator copy = *this;
	++(*this);
	return copy;
}

template<class T>
typename tree<T>::sibling_iterator tree<T>::sibling_iterator::operator--(int)
{
	sibling_iterator copy = *this;
	--(*this);
	return copy;
}

template<class T>
typename tree<T>::sibling_iterator& tree<T>::sibling_iterator::operator+=(
		unsigned int num)
{
	while (num > 0)
	{
		++(*this);
		--num;
	}
	return (*this);
}

template<class T>
typename tree<T>::sibling_iterator& tree<T>::sibling_iterator::operator-=(
		unsigned int num)
{
	while (num > 0)
	{
		--(*this);
		--num;
	}
	return (*this);
}

template<class T>
typename tree<T>::tree_node *tree<T>::sibling_iterator::range_first() const
{
	tree_node *tmp = reinterpret_cast<tree_node*>(parent_->first_child);
	return tmp;
}

template<class T>
typename tree<T>::tree_node *tree<T>::sibling_iterator::range_last() const
{
	return reinterpret_cast<tree_node*>(parent_->last_child);
}

// Leaf iterator

template<class T>
tree<T>::leaf_iterator::leaf_iterator() :
		iterator_base(), top_node(0)
{
}

template<class T>
tree<T>::leaf_iterator::leaf_iterator(tree_node *tn, tree_node *top) :
		iterator_base(tn), top_node(top)
{
}

template<class T>
tree<T>::leaf_iterator::leaf_iterator(const iterator_base &other) :
		iterator_base(other.node), top_node(0)
{
}

template<class T>
tree<T>::leaf_iterator::leaf_iterator(const sibling_iterator& other) :
		iterator_base(other.node), top_node(0)
{
	if (this->node == 0)
	{
		if (other.range_last() != 0)
			this->node = other.range_last();
		else
			this->node = other.parent_;
		++(*this);
	}
}

template<class T>
typename tree<T>::leaf_iterator& tree<T>::leaf_iterator::operator++()
{
	if(this->node)
	{
		if (this->node->first_child != 0)
		{ // current node is no longer leaf (children got added)
			while (this->node->first_child)
				this->node = this->node->first_child;
		}
		else
		{
			while (this->node->next_sibling == 0)
			{
				if (this->node->parent == 0)
					return *this;
				this->node = this->node->parent;
				if (top_node != 0 && this->node == top_node)
					return *this;
			}
			this->node = this->node->next_sibling;
			while (this->node->first_child)
				this->node = this->node->first_child;
		}
	}
	return *this;
}

template<class T>
typename tree<T>::leaf_iterator& tree<T>::leaf_iterator::operator--()
{
	if(this->node)
	{
		while (this->node->prev_sibling == 0)
		{
			if (this->node->parent == 0)
				return *this;
			this->node = this->node->parent;
			if (top_node != 0 && this->node == top_node)
				return *this;
		}
		this->node = this->node->prev_sibling;
		while (this->node->last_child)
			this->node = this->node->last_child;
	}
	return *this;
}

template<class T>
typename tree<T>::leaf_iterator tree<T>::leaf_iterator::operator++(int)
{
	leaf_iterator copy = *this;
	++(*this);
	return copy;
}

template<class T>
typename tree<T>::leaf_iterator tree<T>::leaf_iterator::operator--(int)
{
	leaf_iterator copy = *this;
	--(*this);
	return copy;
}

template<class T>
typename tree<T>::leaf_iterator& tree<T>::leaf_iterator::operator+=(
		unsigned int num)
{
	while (num > 0)
	{
		++(*this);
		--num;
	}
	return (*this);
}

template<class T>
typename tree<T>::leaf_iterator& tree<T>::leaf_iterator::operator-=(
		unsigned int num)
{
	while (num > 0)
	{
		--(*this);
		--num;
	}
	return (*this);
}

//--------- debug iterators ----------------------
template<class T>
void tree<T>::iterator_base::dump(void) const
{
	basic_string<char> str;
	if(node)
	{
		str= "\r\n|";
		for (int i = 0; i < depth(*this); ++i)
			str +="  ";
		str +="  ";
		TRACE1(str.c_str());
		TRACE("node:%X id:%X", node, id());
		TRACE(" _vptr:%X, ", reinterpret_cast<int *>(node)[0]);
		TRACE1(str.c_str());
		TRACE("{ _vptr=%X, ", reinterpret_cast<int *>(&static_cast<tree_node_<T>*>(node)->data)[0]);
		TRACE("data=%d }", reinterpret_cast<int *>(&static_cast<tree_node_<T>*>(node)->data)[1]);
		TRACE1(str.c_str());
		TRACE1("parent:");
		if(node->parent)
		{
			TRACE("%X:",node->parent);
		}
		else
			TRACE1("null");
		TRACE1(str.c_str());
		TRACE1("child first:");
		if(node->first_child)
		{
			TRACE("%X =", node->first_child);
		}
		else
			TRACE1("null");
		TRACE1(str.c_str());
		TRACE1("child last:");
		if(node->last_child)
		{
			TRACE("%X =", node->last_child);
		}
		else
			TRACE1("null");
		TRACE1(str.c_str());
		TRACE1("sibling prev:");
		if(node->prev_sibling)
		{
			TRACE("%X =", node->prev_sibling);
		}
		else
			TRACE1("null");
		TRACE1(str.c_str());
		TRACE1("sibling next:");
		if(node->next_sibling)
		{
			TRACE("%X =", node->next_sibling);
		}
		else
			TRACE1("null");
		TRACE1(str.c_str());

	}
	else
		TRACELN1("|node:null");

}

} // namespace ttl
//#undef assert

#endif /* TTL_TREE_H_ */
