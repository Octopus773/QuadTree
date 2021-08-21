//
// Created by cbihan on 20/08/2021.
//

#pragma once

#include <vector>
#include <list>
#include <memory>
#include "FreeList.hpp"
#include "QuadNode.hpp"

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
		FreeList<std::shared_ptr<T>> elements;

		// Stores all the element nodes in the quadtree.
		FreeList<QuadEltNode> elt_nodes;

		// Stores all the nodes in the quadtree. The first node in this
		// sequence is always the root.
		std::vector<QuadNode> nodes;

		// Stores the quadtree extents.
		int xmin;
		int ymin;
		int xmax;
		int ymax;

		// Stores the first free node in the quadtree to be reclaimed as 4
		// contiguous nodes at once. A value of -1 indicates that the free
		// list is empty, at which point we simply insert 4 nodes to the
		// back of the nodes array.
		int free_node = -1;

		// Stores the maximum depth allowed for the quadtree.
		unsigned int max_depth;
		unsigned int max_element_per_node;

		//! @brief Split a leaf node into children
		//! @note rect [0] xmin [1] ymin [2] xmax [3] ymax
		void split_leaf(QuadNode &leaf, const int rect[4]);

	public:

		explicit QuadTree(int x1, int y1, int x2, int y2);
	};

	template<typename T>
	QuadTree<T>::QuadTree(int x1, int y1, int x2, int y2)
		: xmin(x1),
		  ymin(y1),
		  xmax(x2),
		  ymax(y2),
		  max_depth(5),
		  max_element_per_node(8)
	{
	}

	template<typename T>
	void QuadTree<T>::split_leaf(QuadNode &leaf, const int rect[4])
	{
		auto &elementNodeIndex = leaf.firstChild;
		std::array<std::vector<int>, 4> indexes_to_link;
		std::vector<int> indexes_to_remove;

		const int childWidth = (rect[2] - rect[0]) >> 1;
		const int childHeight = (rect[3] - rect[1]) >> 1;

		do {
			auto &elementNode = this->elt_nodes[elementNodeIndex];
			auto &element = this->elements[elementNode.element];

			// top right
			if (element->collideRect({rect[0],
			                          rect[1],
			                          rect[0] + childWidth,
			                          rect[1] + childHeight})) {
				indexes_to_link[0].emplace_back(this->elt_nodes.insert({-1, elementNode.element}));
			}
			// top left
			if (element->collideRect({rect[0] + childWidth,
			                          rect[1],
			                          rect[0] + childWidth + childWidth,
			                          rect[1] + childHeight})) {
				indexes_to_link[1].emplace_back(this->elt_nodes.insert({-1, elementNode.element}));
			}
			// bottom right
			if (element->collideRect({rect[0],
			                          rect[1] + childHeight,
			                          rect[0] + childWidth,
			                          rect[1] + childHeight + childHeight})) {
				indexes_to_link[2].emplace_back(this->elt_nodes.insert({-1, elementNode.element}));
			}
			// bottom left
			if (element->collideRect({rect[0] + childWidth,
			                          rect[1] + childHeight,
			                          rect[0] + childWidth + childWidth,
			                          rect[1] + childHeight + childHeight})) {
				indexes_to_link[3].emplace_back(this->elt_nodes.insert({-1, elementNode.element}));
			}

			indexes_to_remove.emplace_back(elementNodeIndex);
			elementNodeIndex = elementNode.next;
		} while (elementNodeIndex != -1);

		for (const auto &indexes : indexes_to_link) {
			this->nodes.emplace_back({indexes.empty() ? -2 : indexes[0], indexes.size()});
			for (int i = 0; i < static_cast<int>(indexes.size()) - 1; i++) {
				this->elt_nodes[indexes[i]].next = indexes[i + 1];
			}
		}

		for (const auto &index : indexes_to_remove) {
			this->elt_nodes.erase(index);
		}

		leaf.firstChild = this->nodes.size() - 4;
		leaf.count = -1;
	}
}

