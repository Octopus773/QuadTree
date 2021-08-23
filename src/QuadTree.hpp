//
// Created by cbihan on 20/08/2021.
//

#pragma once

#include <vector>
#include <list>
#include <algorithm>
#include <memory>
#include <unordered_set>
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
	private:
		//! @brief The index Of the root QuadNode in the nodes vector
		static constexpr int RootNodeIndex = 0;

		//! @brief Stores all the elements in the quadtree.
		FreeList<std::shared_ptr<T>> elements;

		//! @brief Stores all the element nodes in the quadtree.
		FreeList<QuadEltNode> elementNodes;

		//! @brief Stores all the nodes in the quadtree. The first node in this sequence is always the root.
		std::vector<QuadNode> nodes;

		//! @brief Stores the quadtree extents.
		double _xMin;
		double _yMin;
		double _xMax;
		double _yMax;

		//! @brief The rect of the quadtree
		const std::array<double, 4> _rootRect;

		// Stores the first free node in the quadtree to be reclaimed as 4
		// contiguous nodes at once. A value of -1 indicates that the free
		// list is empty, at which point we simply insert 4 nodes to the
		// back of the nodes array.
		int free_node = -1;

		//! @brief Split a leaf node into children
		//! @note rect [0] xmin [1] ymin [2] xmax [3] ymax
		void split_leaf(int leafIndex, const std::array<double, 4> &rect);

		//! @brief creates the necessary QuadEltNode in the tree
		void addElementInTree(int elementIndex, int nodeIndex, const std::array<double, 4> &rect,
		                      unsigned int depth);


		//! @brief Finds and return all the leaves nodes which an element is present in them
		[[nodiscard]] std::vector<int>
		findLeaves(int elementIndex, int nodeIndex, const std::array<double, 4> &rect) const;

		//! @brief Finds and remove all QuadNodes referencing to the element index
		void removeElementInTree(int elementIndex, int nodeIndex, const std::array<double, 4> &rect);

	public:


		//! @brief Get all the elements in the same quadNode of the given element
		[[nodiscard]] std::vector<std::shared_ptr<T>> getNeighbours(const std::shared_ptr<T> &element) const;

		//! @brief Stores the maximum depth allowed for the quadtree.
		//! @note The tree won't be recreated when changing this value
		unsigned int maxDepth;

		//! @brief Stores the maximum number of elements allowed in a quadrant
		//! @note The tree won't be recreated when changing this value
		unsigned int maxElementsPerNode;

		//! @brief Add an element in the tree
		void add(std::shared_ptr<T> element);

		//! @brief Remove the element from the quadTree completely
		void remove(std::shared_ptr<T> element);

		//! @brief Update the position of the element in the tree
		//! @note You should only call this function if the results of the collideRect method of the object will be different (updating element position)
		void update(std::shared_ptr<T> element);

		//! @brief create a quadtree
		explicit QuadTree(double x1, double y1, double x2, double y2);
	};

	template<typename T>
	QuadTree<T>::QuadTree(double x1, double y1, double x2, double y2)
		:  nodes({{-2, 0}}),
		   _xMin(x1),
		   _yMin(y1),
		   _xMax(x2),
		   _yMax(y2),
		   _rootRect({x1, y1, x2, y2}),
		   maxDepth(5),
		   maxElementsPerNode(8)
	{
	}

	template<typename T>
	void QuadTree<T>::split_leaf(int leafIndex, const std::array<double, 4> &rect)
	{
		auto &elementNodeIndex = this->nodes[leafIndex].firstChild;
		std::array<std::vector<int>, 4> indexes_to_link;
		std::vector<int> indexes_to_remove;

		const double childWidth = (rect[2] - rect[0]) / 2;
		const double childHeight = (rect[3] - rect[1]) / 2;

		do {
			const auto elementNode = this->elementNodes[elementNodeIndex];
			const auto &element = this->elements[elementNode.element];

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

			indexes_to_remove.push_back(elementNodeIndex);
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

		this->nodes[leafIndex].firstChild = this->nodes.size() - 4;
		this->nodes[leafIndex].count = -1;
	}

	template<typename T>
	void QuadTree<T>::addElementInTree(int elementIndex, int nodeIndex, const std::array<double, 4> &rect,
	                                   unsigned int depth)
	{
		if (this->nodes[nodeIndex].count == -1) {

			const double childWidth = (rect[2] - rect[0]) / 2;
			const double childHeight = (rect[3] - rect[1]) / 2;

			const auto &element = this->elements[elementIndex];


			// top right
			if (element->collideRect({rect[0],
			                          rect[1],
			                          rect[0] + childWidth,
			                          rect[1] + childHeight})) {
				this->addElementInTree(elementIndex,
				                       this->nodes[nodeIndex].firstChild,
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
				this->addElementInTree(elementIndex,
				                       this->nodes[nodeIndex].firstChild + 1,
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
				this->addElementInTree(elementIndex,
				                       this->nodes[nodeIndex].firstChild + 2,
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
				this->addElementInTree(elementIndex,
				                       this->nodes[nodeIndex].firstChild + 3,
				                       {rect[0] + childWidth,
				                        rect[1] + childHeight,
				                        rect[0] + childWidth + childWidth,
				                        rect[1] + childHeight + childHeight},
				                       depth + 1);
			}
			return;
		}

		int quadNodeIndex = this->nodes[nodeIndex].firstChild;

		if (this->nodes[nodeIndex].count == 0) {
			this->nodes[nodeIndex].firstChild = this->elementNodes.insert({-1, elementIndex});
			this->nodes[nodeIndex].count++;
			return;
		} else {
			while (true) {
				if (this->elementNodes[quadNodeIndex].next == -1) {
					this->elementNodes[quadNodeIndex].next = this->elementNodes.insert({-1, elementIndex});
					this->nodes[nodeIndex].count++;
					break;
				}
				quadNodeIndex = this->elementNodes[quadNodeIndex].next;
			}
		}

		if (depth < this->maxDepth && static_cast<unsigned>(this->nodes[nodeIndex].count) > this->maxElementsPerNode) {
			this->split_leaf(nodeIndex, rect);
		}

	}

	template<typename T>
	void QuadTree<T>::add(std::shared_ptr<T> element)
	{
		int elementIndex = this->elements.insert(element);

		this->addElementInTree(elementIndex, RootNodeIndex, this->_rootRect, 0);
	}

	template<typename T>
	std::vector<std::shared_ptr<T>> QuadTree<T>::getNeighbours(const std::shared_ptr<T> &element) const
	{
		int index = this->elements.findIndex(element);

		if (index == -1) {
			throw std::runtime_error("element not found in tree");
		}

		std::vector<int> leavesIndexes = this->findLeaves(index, 0, this->_rootRect);
		std::vector<int> neighboursIndexes;

		for (const auto &leaveIndex : leavesIndexes) {
			const auto &leaf = this->nodes[leaveIndex];
			int elementNodeIndex = leaf.firstChild;
			for (int i = 0; i < leaf.count; i++) {
				neighboursIndexes.emplace_back(this->elementNodes[elementNodeIndex].element);
				elementNodeIndex = this->elementNodes[elementNodeIndex].next;
			}
		}

		// eliminates duplicates
		std::unordered_set<int> s;
		for (int i : neighboursIndexes)
			s.insert(i);
		// removes itself from the neighbours index
		s.erase(index);
		neighboursIndexes.assign(s.begin(), s.end());
		std::sort(neighboursIndexes.begin(), neighboursIndexes.end());


		std::vector<std::shared_ptr<T>> neighbours;
		for (const auto &neighbourIndex: neighboursIndexes) {
			neighbours.emplace_back(this->elements[neighbourIndex]);
		}
		return neighbours;
	}

	template<typename T>
	std::vector<int> QuadTree<T>::findLeaves(int elementIndex, int nodeIndex, const std::array<double, 4> &rect) const
	{
		std::vector<int> leavesIndexes;
		auto &node = this->nodes[nodeIndex];

		if (node.count != -1) {
			leavesIndexes.emplace_back(nodeIndex);
			return leavesIndexes;
		}

		const double childWidth = (rect[2] - rect[0]) / 2;
		const double childHeight = (rect[3] - rect[1]) / 2;

		const auto &element = this->elements[elementIndex];


		// top right
		if (element->collideRect({rect[0],
		                          rect[1],
		                          rect[0] + childWidth,
		                          rect[1] + childHeight})) {
			auto leavesFound = this->findLeaves(elementIndex,
			                                    node.firstChild,
			                                    {rect[0],
			                                     rect[1],
			                                     rect[0] + childWidth,
			                                     rect[1] + childHeight});
			leavesIndexes.insert(leavesIndexes.end(), leavesFound.begin(), leavesFound.end());
		}
		// top left
		if (element->collideRect({rect[0] + childWidth,
		                          rect[1],
		                          rect[0] + childWidth + childWidth,
		                          rect[1] + childHeight})) {
			auto leavesFound = this->findLeaves(elementIndex,
			                                    node.firstChild + 1,
			                                    {rect[0] + childWidth,
			                                     rect[1],
			                                     rect[0] + childWidth + childWidth,
			                                     rect[1] + childHeight});
			leavesIndexes.insert(leavesIndexes.end(), leavesFound.begin(), leavesFound.end());
		}
		// bottom right
		if (element->collideRect({rect[0],
		                          rect[1] + childHeight,
		                          rect[0] + childWidth,
		                          rect[1] + childHeight + childHeight})) {
			auto leavesFound = this->findLeaves(elementIndex,
			                                    node.firstChild + 2,
			                                    {rect[0],
			                                     rect[1] + childHeight,
			                                     rect[0] + childWidth,
			                                     rect[1] + childHeight + childHeight});
			leavesIndexes.insert(leavesIndexes.end(), leavesFound.begin(), leavesFound.end());
		}
		// bottom left
		if (element->collideRect({rect[0] + childWidth,
		                          rect[1] + childHeight,
		                          rect[0] + childWidth + childWidth,
		                          rect[1] + childHeight + childHeight})) {
			auto leavesFound = this->findLeaves(elementIndex,
			                                    node.firstChild + 3,
			                                    {rect[0] + childWidth,
			                                     rect[1] + childHeight,
			                                     rect[0] + childWidth + childWidth,
			                                     rect[1] + childHeight + childHeight});
			leavesIndexes.insert(leavesIndexes.end(), leavesFound.begin(), leavesFound.end());
		}
		return leavesIndexes;
	}

	template<typename T>
	void QuadTree<T>::remove(std::shared_ptr<T> element)
	{
		int elementIndex = this->elements.findIndex(element);

		this->removeElementInTree(elementIndex, RootNodeIndex, this->_rootRect);
		this->elements.erase(elementIndex);
	}

	template<typename T>
	void QuadTree<T>::removeElementInTree(int elementIndex, int nodeIndex, const std::array<double, 4> &rect)
	{
		std::vector<int> leavesIndexes = this->findLeaves(elementIndex, nodeIndex, rect);

		for (const auto &leafIndex : leavesIndexes) {
			auto &leaf = this->nodes[leafIndex];
			int elementNodeIndex = leaf.firstChild;
			int prevElementNodeIndex = elementNodeIndex;

			for (int i = 0; i < leaf.count; i++) {
				if (this->elementNodes[elementNodeIndex].element == elementIndex) {
					if (i == 0)  {
						leaf.firstChild = this->elementNodes[elementNodeIndex].next;
					} else {
						this->elementNodes[prevElementNodeIndex].next = this->elementNodes[elementNodeIndex].next;
					}
					leaf.count--;
					this->elementNodes.erase(elementNodeIndex);
					break;
				}
				prevElementNodeIndex = elementNodeIndex;
				elementNodeIndex = this->elementNodes[elementNodeIndex].next;
			}
		}
	}

	template<typename T>
	void QuadTree<T>::update(std::shared_ptr<T> element)
	{
		int elementIndex = this->elements.findIndex(element);
		this->removeElementInTree(elementIndex, RootNodeIndex, this->_rootRect);
		this->addElementInTree(elementIndex, RootNodeIndex, this->_rootRect, 0);
	}
}

