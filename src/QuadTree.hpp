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

	//! @brief USed to get the correct value from a const double rect[4] (passed as argument)
	enum Rect
	{
		//! @brief minimum horizontal
		minH = 0,
		//! @brief minimum vertical
		minV = 1,
		//! @brief maximum horizontal
		maxH = 2,
		//! @brief maximum vertical
		maxV = 3
	};

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

	//! @brief Represents an element node in the quadtree.
	struct QuadEltNode
	{
		//! @brief Points to the next element in the leaf node. A value of -1 indicates the end of the list.
		int next;

		//! @brief Stores the element index.
		int element;
	};

	template<typename T>
	class QuadTree
	{
		//! @brief Stores all the elements in the quadtree.
		FreeList<std::shared_ptr<T>> elements;

		//! @brief Stores all the element nodes in the quadtree.
		FreeList<QuadEltNode> elementNodes;

		//! @brief Stores all the nodes in the quadtree. The first node in this sequence is always the root.
		std::vector<QuadNode> nodes;

		//! @brief Stores the quadtree extents.
		double xmin;
		double ymin;
		double xmax;
		double ymax;

		// Stores the first free node in the quadtree to be reclaimed as 4
		// contiguous nodes at once. A value of -1 indicates that the free
		// list is empty, at which point we simply insert 4 nodes to the
		// back of the nodes array.
		int free_node = -1;

		//! @brief Split a leaf node into children
		//! @note rect [0] xmin [1] ymin [2] xmax [3] ymax
		void split_leaf(QuadNode &leaf, const std::array<double, 4> &rect);

		//! @brief creates the necessary QuadEltNode in the tree
		void addElementQuadNodeInTree(int elementIndex, QuadNode &node, const std::array<double, 4> &rect,
		                              unsigned int depth);

	public:

		//! @brief Stores the maximum depth allowed for the quadtree.
		//! @note The tree won't be recreated when changing this value
		unsigned int maxDepth;

		//! @brief Stores the maximum number of elements allowed in a quadrant
		//! @note The tree won't be recreated when changing this value
		unsigned int maxElementsPerNode;

		//! @brief Add an element in the tree
		void add(std::shared_ptr<T> element);


		//! @brief create a quadtree
		explicit QuadTree(double x1, double y1, double x2, double y2);
	};

	template<typename T>
	QuadTree<T>::QuadTree(double x1, double y1, double x2, double y2)
		:  nodes({{-2, 0}}),
		   xmin(x1),
		   ymin(y1),
		   xmax(x2),
		   ymax(y2),
		   maxDepth(5),
		   maxElementsPerNode(8)
	{
	}

	template<typename T>
	void QuadTree<T>::split_leaf(QuadNode &leaf, const std::array<double, 4> &rect)
	{
		auto &elementNodeIndex = leaf.firstChild;
		std::array<std::vector<int>, 4> indexes_to_link;
		std::vector<int> indexes_to_remove;

		const double childWidth = (rect[2] - rect[0]) / 2;
		const double childHeight = (rect[3] - rect[1]) / 2;

		do {
			auto &elementNode = this->elementNodes[elementNodeIndex];
			auto &element = this->elements[elementNode.element];

			// top right
			if (element->collideRect({rect[0],
			                          rect[1],
			                          rect[0] + childWidth,
			                          rect[1] + childHeight})) {
				indexes_to_link[0].emplace_back(this->elementNodes.insert({-1, elementNode.element}));
			}
			// top left
			if (element->collideRect({rect[0] + childWidth,
			                          rect[1],
			                          rect[0] + childWidth + childWidth,
			                          rect[1] + childHeight})) {
				indexes_to_link[1].emplace_back(this->elementNodes.insert({-1, elementNode.element}));
			}
			// bottom right
			if (element->collideRect({rect[0],
			                          rect[1] + childHeight,
			                          rect[0] + childWidth,
			                          rect[1] + childHeight + childHeight})) {
				indexes_to_link[2].emplace_back(this->elementNodes.insert({-1, elementNode.element}));
			}
			// bottom left
			if (element->collideRect({rect[0] + childWidth,
			                          rect[1] + childHeight,
			                          rect[0] + childWidth + childWidth,
			                          rect[1] + childHeight + childHeight})) {
				indexes_to_link[3].emplace_back(this->elementNodes.insert({-1, elementNode.element}));
			}

			indexes_to_remove.emplace_back(elementNodeIndex);
			elementNodeIndex = elementNode.next;
		} while (elementNodeIndex != -1);

		for (const auto &indexes : indexes_to_link) {
			this->nodes.emplace_back(indexes.empty() ? -2 : indexes[0], indexes.size());
			for (int i = 0; i < static_cast<int>(indexes.size()) - 1; i++) {
				this->elementNodes[indexes[i]].next = indexes[i + 1];
			}
		}

		for (const auto &index : indexes_to_remove) {
			this->elementNodes.erase(index);
		}

		leaf.firstChild = this->nodes.size() - 4;
		leaf.count = -1;
	}

	template<typename T>
	void QuadTree<T>::addElementQuadNodeInTree(int elementIndex, QuadNode &node, const std::array<double, 4> &rect,
	                                           unsigned int depth)
	{
		if (node.count == -1) {

			const double childWidth = (rect[2] - rect[0]) / 2;
			const double childHeight = (rect[3] - rect[1]) / 2;

			auto element = this->elements[elementIndex];


			// top right
			if (element->collideRect({rect[0],
			                          rect[1],
			                          rect[0] + childWidth,
			                          rect[1] + childHeight})) {
				this->addElementQuadNodeInTree(elementIndex,
				                               this->nodes[node.firstChild],
				                               {rect[0],
				                                rect[1],
				                                rect[0] + childWidth,
				                                rect[1] + childHeight},
				                               depth + 1);
			}
			// top left
			if (element->collideRect({rect[0] + childWidth,
			                          rect[1],
			                          rect[0] + childWidth + childWidth,
			                          rect[1] + childHeight})) {
				this->addElementQuadNodeInTree(elementIndex,
				                               this->nodes[node.firstChild + 1],
				                               {rect[0] + childWidth,
				                                rect[1],
				                                rect[0] + childWidth + childWidth,
				                                rect[1] + childHeight},
				                               depth + 1);
			}
			// bottom right
			if (element->collideRect({rect[0],
			                          rect[1] + childHeight,
			                          rect[0] + childWidth,
			                          rect[1] + childHeight + childHeight})) {
				this->addElementQuadNodeInTree(elementIndex,
				                               this->nodes[node.firstChild + 2],
				                               {rect[0],
				                                rect[1] + childHeight,
				                                rect[0] + childWidth,
				                                rect[1] + childHeight + childHeight},
				                               depth + 1);
			}
			// bottom left
			if (element->collideRect({rect[0] + childWidth,
			                          rect[1] + childHeight,
			                          rect[0] + childWidth + childWidth,
			                          rect[1] + childHeight + childHeight})) {
				this->addElementQuadNodeInTree(elementIndex,
				                               this->nodes[node.firstChild + 3],
				                               {rect[0] + childWidth,
				                                rect[1] + childHeight,
				                                rect[0] + childWidth + childWidth,
				                                rect[1] + childHeight + childHeight},
				                               depth + 1);
			}
			return;
		}

		int quadNodeIndex = node.firstChild;

		if (node.count == 0) {
			node.firstChild = this->elementNodes.insert({-1, elementIndex});
			node.count++;
			return;
		} else {
			while (true) {
				if (this->elementNodes[quadNodeIndex].next == -1) {
					this->elementNodes[quadNodeIndex].next = this->elementNodes.insert({-1, elementIndex});
					node.count++;
					break;
				}
				quadNodeIndex = this->elementNodes[quadNodeIndex].next;
			}
		}

		if (depth < this->maxDepth && static_cast<unsigned>(node.count) > this->maxElementsPerNode) {
			this->split_leaf(node, rect);
		}

	}

	template<typename T>
	void QuadTree<T>::add(std::shared_ptr<T> element)
	{
		int elementIndex = this->elements.insert(element);

		this->addElementQuadNodeInTree(elementIndex, this->nodes[0], {this->xmin, this->ymin, this->xmax, this->ymax},
		                               0);
	}
}

