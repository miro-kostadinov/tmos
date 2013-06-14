/*
 * ttl_tree.cpp
 *
 *  Created on: 24.04.2013
 *      Author: stanly
 */

#include <tmos.h>
#include <ttl_tree.h>
using namespace ttl;


tree_head* tree_head::attach_to(tree_head* position)
{
	next_sibling = position;
	if (position)
	{
		parent = position->parent;
		prev_sibling = position->prev_sibling;
		position->prev_sibling = this;
	}

	if (prev_sibling == 0)
	{
		if (parent) // when inserting nodes at the head, there is no parent
			parent->first_child = this;
	}
	else
		prev_sibling->next_sibling = this;
	return this;
}

tree_head* tree_head::replace(tree_head* position)
{
	if (position->prev_sibling == 0)
	{
		if (position->parent != 0)
			position->parent->first_child = this;
	}
	else
	{
		position->prev_sibling->next_sibling = this;
	}
	prev_sibling = position->prev_sibling;
	if (position->next_sibling == 0)
	{
		if (position->parent != 0)
			position->parent->last_child = this;
	}
	else
	{
		position->next_sibling->prev_sibling = this;
	}
	next_sibling = position->next_sibling;
	parent = position->parent;
	return this;
}
