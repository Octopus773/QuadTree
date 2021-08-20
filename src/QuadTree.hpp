//
// Created by cbihan on 20/08/2021.
//

#pragma once

#include <vector>
#include <list>
#include "FreeList.hpp"
#include "QuadNode.hpp"

namespace QuadTree
{

	// Represents an element in the quadtree.
	struct QuadElt
	{
		// Stores the ID for the element (can be used to
		// refer to external data).
		int id;

		// Stores the rectangle for the element.
		int x1, y1, x2, y2;
	};

	// Represents an element node in the quadtree.
	struct QuadEltNode
	{
		// Points to the next element in the leaf node. A value of -1
		// indicates the end of the list.
		int next;

		// Stores the element index.
		int element;
	};

	template<typename T>
	class QuadTree
	{
		// Stores all the elements in the quadtree.
		FreeList<QuadElt> elts;

		// Stores all the element nodes in the quadtree.
		FreeList<QuadEltNode> elt_nodes;

		// Stores all the nodes in the quadtree. The first node in this
		// sequence is always the root.
		std::vector<QuadNode> nodes;

		// Stores the quadtree extents.
		//QuadCRect root_rect;

		// Stores the first free node in the quadtree to be reclaimed as 4
		// contiguous nodes at once. A value of -1 indicates that the free
		// list is empty, at which point we simply insert 4 nodes to the
		// back of the nodes array.
		int free_node;

		// Stores the maximum depth allowed for the quadtree.
		int max_depth;
	};

	static std::list<QuadNode> find_leaves(const Quadtree& tree, const QuadNode& root, const int rect[4])
	{
		std::list<QuadNode> leaves, to_process;
		to_process.push_back(root);
		while (!to_process.empty())
		{
			const auto nd = to_process.back();
			to_process.pop_back();



			// If this node is a leaf, insert it to the list.
			if (tree.nodes[nd.index].count != -1)
				leaves.push_back(nd);
			else
			{
				// Otherwise push the children that intersect the rectangle.
				const int mx = nd.crect[0], my = nd.crect[1];
				const int hx = nd.crect[2] >> 1, hy = nd.crect[3] >> 1;
				const int fc = tree.nodes[nd.index].first_child;
				const int l = mx-hx, t = my-hx, r = mx+hx, b = my+hy;

				if (rect[1] <= my)
				{
					if (rect[0] <= mx)
						to_process.push_back(child_data(l,t, hx, hy, fc+0, nd.depth+1));
					if (rect[2] > mx)
						to_process.push_back(child_data(r,t, hx, hy, fc+1, nd.depth+1));
				}
				if (rect[3] > my)
				{
					if (rect[0] <= mx)
						to_process.push_back(child_data(l,b, hx, hy, fc+2, nd.depth+1));
					if (rect[2] > mx)
						to_process.push_back(child_data(r,b, hx, hy, fc+3, nd.depth+1));
				}
			}
		}
		return leaves;
	}
}

