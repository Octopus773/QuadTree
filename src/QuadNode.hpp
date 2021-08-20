//
// Created by cbihan on 20/08/2021.
//

#pragma once

#include <cstdint>

namespace QuadTree
{
	//! @brief Represents a node in the quadtree.
	struct QuadNode
	{
		//! @brief Points to the first child if this node is a branch or the first element if this node is a leaf.
		int32_t firstChild;

		//! @brief Stores the number of elements in the leaf or -1 if it this node is not a leaf.
		int32_t count;
	};

	static QuadNodeList find_leaves(const Quadtree& tree, const QuadNodeData& root, const int rect[4])
	{
		QuadNodeList leaves, to_process;
		to_process.push_back(root);
		while (to_process.size() > 0)
		{
			const QuadNodeData nd = to_process.pop_back();

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


