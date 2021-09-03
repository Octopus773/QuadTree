//
// Created by cbihan on 02/09/2021.
//

#pragma once

#include "QuadTree.hpp"

//! @brief Return the size of the _nodes internal member
//! @param qT the quadtree
template<typename T>
size_t getSizeNodes(QuadTree::QuadTree<T> &qT)
{
	size_t sizeFree = 0;
	int freeIndex = qT._firstFreeNode;

	while (freeIndex != QuadTree::QuadTree<T>::EndOfList) {
		sizeFree++;
		freeIndex = qT._nodes[freeIndex].firstChild;
	}
	return qT._nodes.size() - sizeFree;
}

