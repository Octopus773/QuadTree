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
}


