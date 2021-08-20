//
// Created by cbihan on 20/08/2021.
//

#pragma once

#include <vector>

namespace QuadTree
{

	template<typename T>
	class QuadTree
	{
		// Stores all the elements in the quadtree.
		FreeList <QuadElt> elts;

		// Stores all the element nodes in the quadtree.
		FreeList <QuadEltNode> elt_nodes;

		// Stores all the nodes in the quadtree. The first node in this
		// sequence is always the root.
		std::vector <QuadNode> nodes;

		// Stores the quadtree extents.
		QuadCRect root_rect;

		// Stores the first free node in the quadtree to be reclaimed as 4
		// contiguous nodes at once. A value of -1 indicates that the free
		// list is empty, at which point we simply insert 4 nodes to the
		// back of the nodes array.
		int free_node;

		// Stores the maximum depth allowed for the quadtree.
		int max_depth;
	};
}

