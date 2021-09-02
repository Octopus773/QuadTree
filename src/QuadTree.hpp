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

		//! @brief When this value is encountered in traversing a singly list it means it's the end of the list
		static constexpr int EndOfList = -1;

		//! @brief If the count member of a QuadNode as this value it means it's a branch
		//! @note Branches are not leaves
		static constexpr int BranchIdentifier = -1;

		//! @brief Stores all the elements in the quadtree.
		FreeList<std::shared_ptr<T>> _elements;

		//! @brief Stores all the element nodes in the quadtree.
		FreeList<QuadEltNode> _elementNodes;

		//! @brief Stores all the nodes in the quadtree. The first node in this sequence is always the root.
		std::vector<QuadNode> _nodes;

		// Stores the first free node in the quadtree to be reclaimed as 4
		// contiguous nodes at once. A value of -1 indicates that the free
		// list is empty, at which point we simply insert 4 nodes to the
		// back of the nodes array.
		//! @brief variale used for keeping track of the firstFreeNode in the node vector
		//! @warning You should not update this variable, you should use the _allocNodes and _freeNode functions
		int _firstFreeNode = EndOfList;

		//! @brief Split a leaf node into children
		//! @note rect [0] xmin [1] ymin [2] xmax [3] ymax
		void _splitLeaf(int leafIndex, const std::array<double, 4> &rect);

		//! @brief creates the necessary QuadEltNode in the tree
		void _addElementInTree(int elementIndex, int nodeIndex, const std::array<double, 4> &rect,
		                       unsigned int depth);


		//! @brief Finds and return all the leaves nodes which an element is present in them
		[[nodiscard]] std::vector<int>
		_findLeaves(int elementIndex, int nodeIndex, const std::array<double, 4> &rect) const;

		//! @brief Finds and remove all QuadNodes referencing to the element index
		void _removeElementInTree(int elementIndex, int nodeIndex, const std::array<double, 4> &rect);

		//! @brief Returns the index pointing to 4 continuous nodes
		//! @note You should always use this function to create new nodes
		[[nodiscard]] int _allocNodes();

		//! @brief Free the space taken by the 4 continuous nodes pointing by the given index
		//! @note You should always use this function to free nodes
		void _freeNodes(int firstNodeIndex);

		//! @brief Free the space taken by the 1 node pointing by the given index
		//! @note You should always use this function to free a single node
		void _freeNode(int nodeIndex);

		//! @brief Will reset the node list. Keep only the root
		//! @note Used for recreating the tree we're clearing the list but the vector size doesn't change (used for fast reset)
		void _resetNodes();

		void _splitLeafRec(int leafIndex, const std::array<double, 4> &rect, unsigned int depth);

	public:

		//! @brief The rect of the quadtree
		const std::array<double, 4> _rootRect;

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
		//! @note You should onlythis->nodes.size() call this function if the results of the collideRect method of the object will be different (updating element position)
		void update(std::shared_ptr<T> element);

		//! @brief Clears the empty leaf nodes to shrink the tree
		void cleanup();

		//! @brief Reconstruct the tree
		//! @note This method should be faster for updating the tree than calling .update for each element if you're updating more than half of the elements
		void reCreate();

		//! @brief create a quadtree
		explicit QuadTree(double x1, double y1, double x2, double y2);
	};

	template<typename T>
	QuadTree<T>::QuadTree(double x1, double y1, double x2, double y2)
		:  _nodes({{-2, 0}}),
		   _rootRect({x1, y1, x2, y2}),
		   maxDepth(5),
		   maxElementsPerNode(8)
	{
	}

	template<typename T>
	void QuadTree<T>::_splitLeaf(int leafIndex, const std::array<double, 4> &rect)
	{
		auto &elementNodeIndex = this->_nodes[leafIndex].firstChild;
		std::array<std::vector<int>, 4> indexes_to_link;
		std::vector<int> indexes_to_remove;

		const double childWidth = (rect[2] - rect[0]) / 2;
		const double childHeight = (rect[3] - rect[1]) / 2;

		do {
			const auto elementNode = this->_elementNodes[elementNodeIndex];
			const auto &element = this->_elements[elementNode.element];

			// top right
			if (element->collideRect({rect[0],
			                          rect[1],
			                          rect[0] + childWidth,
			                          rect[1] + childHeight})) {
				indexes_to_link[0].emplace_back(this->_elementNodes.insert({EndOfList, elementNode.element}));
			}
			// top left
			if (element->collideRect({rect[0] + childWidth,
			                          rect[1],
			                          rect[0] + childWidth + childWidth,
			                          rect[1] + childHeight})) {
				indexes_to_link[1].emplace_back(this->_elementNodes.insert({EndOfList, elementNode.element}));
			}
			// bottom right
			if (element->collideRect({rect[0],
			                          rect[1] + childHeight,
			                          rect[0] + childWidth,
			                          rect[1] + childHeight + childHeight})) {
				indexes_to_link[2].emplace_back(this->_elementNodes.insert({EndOfList, elementNode.element}));
			}
			// bottom left
			if (element->collideRect({rect[0] + childWidth,
			                          rect[1] + childHeight,
			                          rect[0] + childWidth + childWidth,
			                          rect[1] + childHeight + childHeight})) {
				indexes_to_link[3].emplace_back(this->_elementNodes.insert({EndOfList, elementNode.element}));
			}

			indexes_to_remove.push_back(elementNodeIndex);
			elementNodeIndex = elementNode.next;
		} while (elementNodeIndex != EndOfList);

		int nodeIndex = this->_allocNodes();
		for (const auto &indexes : indexes_to_link) {
			this->_nodes[nodeIndex++] = {indexes.empty() ? -2 : indexes[0], static_cast<int>(indexes.size())};
			for (int i = 0; i < static_cast<int>(indexes.size()) - 1; i++) {
				this->_elementNodes[indexes[i]].next = indexes[i + 1];
			}
		}
		nodeIndex -= 4;

		for (const auto &index : indexes_to_remove) {
			this->_elementNodes.remove(index);
		}

		this->_nodes[leafIndex].firstChild = nodeIndex;
		this->_nodes[leafIndex].count = BranchIdentifier;
	}

	template<typename T>
	void QuadTree<T>::_splitLeafRec(int leafIndex, const std::array<double, 4> &rect, unsigned int depth)
	{
		this->_splitLeaf(leafIndex, rect);

		depth++;

		for (int i = this->_nodes[leafIndex].firstChild; i < this->_nodes[leafIndex].firstChild + 4; i++) {
			if (depth < this->maxDepth && static_cast<unsigned>(this->_nodes[i].count) > this->maxElementsPerNode) {
				this->_splitLeafRec(i, rect, depth);
			}
		}
	}

	template<typename T>
	void QuadTree<T>::_addElementInTree(int elementIndex, int nodeIndex, const std::array<double, 4> &rect,
	                                    unsigned int depth)
	{
		if (this->_nodes[nodeIndex].count == BranchIdentifier) {

			const double childWidth = (rect[2] - rect[0]) / 2;
			const double childHeight = (rect[3] - rect[1]) / 2;

			const auto &element = this->_elements[elementIndex];


			// top right
			if (element->collideRect({rect[0],
			                          rect[1],
			                          rect[0] + childWidth,
			                          rect[1] + childHeight})) {
				this->_addElementInTree(elementIndex,
				                        this->_nodes[nodeIndex].firstChild,
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
				this->_addElementInTree(elementIndex,
				                        this->_nodes[nodeIndex].firstChild + 1,
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
				this->_addElementInTree(elementIndex,
				                        this->_nodes[nodeIndex].firstChild + 2,
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
				this->_addElementInTree(elementIndex,
				                        this->_nodes[nodeIndex].firstChild + 3,
				                        {rect[0] + childWidth,
				                         rect[1] + childHeight,
				                         rect[0] + childWidth + childWidth,
				                         rect[1] + childHeight + childHeight},
				                        depth + 1);
			}
			return;
		}

		int quadNodeIndex = this->_nodes[nodeIndex].firstChild;

		if (this->_nodes[nodeIndex].count == 0) {
			this->_nodes[nodeIndex].firstChild = this->_elementNodes.insert({EndOfList, elementIndex});
			this->_nodes[nodeIndex].count++;
			return;
		} else {
			while (true) {
				if (this->_elementNodes[quadNodeIndex].next == EndOfList) {
					this->_elementNodes[quadNodeIndex].next = this->_elementNodes.insert({EndOfList, elementIndex});
					this->_nodes[nodeIndex].count++;
					break;
				}
				quadNodeIndex = this->_elementNodes[quadNodeIndex].next;
			}
		}

		if (depth < this->maxDepth && static_cast<unsigned>(this->_nodes[nodeIndex].count) > this->maxElementsPerNode) {
			this->_splitLeafRec(nodeIndex, rect, depth);
		}
	}

	template<typename T>
	void QuadTree<T>::add(std::shared_ptr<T> element)
	{
		int elementIndex = this->_elements.insert(element);

		this->_addElementInTree(elementIndex, RootNodeIndex, this->_rootRect, 0);
	}

	template<typename T>
	std::vector<std::shared_ptr<T>> QuadTree<T>::getNeighbours(const std::shared_ptr<T> &element) const
	{
		int index = this->_elements.findIndex(element);

		if (index == EndOfList) {
			throw std::runtime_error("element not found in tree");
		}

		std::vector<int> leavesIndexes = this->_findLeaves(index, 0, this->_rootRect);
		std::vector<int> neighboursIndexes;

		for (const auto &leaveIndex : leavesIndexes) {
			const auto &leaf = this->_nodes[leaveIndex];
			int elementNodeIndex = leaf.firstChild;
			for (int i = 0; i < leaf.count; i++) {
				neighboursIndexes.emplace_back(this->_elementNodes[elementNodeIndex].element);
				elementNodeIndex = this->_elementNodes[elementNodeIndex].next;
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
		neighbours.reserve(neighboursIndexes.size());
		for (const auto &neighbourIndex : neighboursIndexes) {
			neighbours.emplace_back(this->_elements[neighbourIndex]);
		}
		return neighbours;
	}

	template<typename T>
	std::vector<int> QuadTree<T>::_findLeaves(int elementIndex, int nodeIndex, const std::array<double, 4> &rect) const
	{
		std::vector<int> leavesIndexes;
		auto &node = this->_nodes[nodeIndex];

		if (node.count != BranchIdentifier) {
			leavesIndexes.emplace_back(nodeIndex);
			return leavesIndexes;
		}

		const double childWidth = (rect[2] - rect[0]) / 2;
		const double childHeight = (rect[3] - rect[1]) / 2;

		const auto &element = this->_elements[elementIndex];


		// top right
		if (element->collideRect({rect[0],
		                          rect[1],
		                          rect[0] + childWidth,
		                          rect[1] + childHeight})) {
			auto leavesFound = this->_findLeaves(elementIndex,
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
			auto leavesFound = this->_findLeaves(elementIndex,
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
			auto leavesFound = this->_findLeaves(elementIndex,
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
			auto leavesFound = this->_findLeaves(elementIndex,
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
		int elementIndex = this->_elements.findIndex(element);

		this->_removeElementInTree(elementIndex, RootNodeIndex, this->_rootRect);
		this->_elements.remove(elementIndex);
	}

	template<typename T>
	void QuadTree<T>::_removeElementInTree(int elementIndex, int nodeIndex, const std::array<double, 4> &rect)
	{
		std::vector<int> leavesIndexes = this->_findLeaves(elementIndex, nodeIndex, rect);

		for (const auto &leafIndex : leavesIndexes) {
			auto &leaf = this->_nodes[leafIndex];
			int elementNodeIndex = leaf.firstChild;
			int prevElementNodeIndex = elementNodeIndex;

			for (int i = 0; i < leaf.count; i++) {
				if (this->_elementNodes[elementNodeIndex].element == elementIndex) {
					if (i == 0) {
						leaf.firstChild = this->_elementNodes[elementNodeIndex].next;
					} else {
						this->_elementNodes[prevElementNodeIndex].next = this->_elementNodes[elementNodeIndex].next;
					}
					leaf.count--;
					this->_elementNodes.remove(elementNodeIndex);
					break;
				}
				prevElementNodeIndex = elementNodeIndex;
				elementNodeIndex = this->_elementNodes[elementNodeIndex].next;
			}
		}
	}

	template<typename T>
	void QuadTree<T>::update(std::shared_ptr<T> element)
	{
		int elementIndex = this->_elements.findIndex(element);
		this->_removeElementInTree(elementIndex, RootNodeIndex, this->_rootRect);
		this->_addElementInTree(elementIndex, RootNodeIndex, this->_rootRect, 0);
	}

	template<typename T>
	void QuadTree<T>::cleanup()
	{
		std::vector<int> to_process;
		// Only process the root if it's not a leaf.
		if (this->_nodes[RootNodeIndex].count == BranchIdentifier)
			to_process.push_back(RootNodeIndex);

		while (!to_process.empty()) {
			const int node_index = to_process.back();
			to_process.pop_back();
			QuadNode &node = this->_nodes[node_index];

			// Loop through the children.
			int num_empty_leaves = 0;
			for (int j = 0; j < 4; ++j) {
				const int childIndex = node.firstChild + j;
				const QuadNode &child = this->_nodes[childIndex];

				// Increment empty leaf count if the child is an empty
				// leaf. Otherwise if the child is a branch, add it to
				// the stack to be processed in the next iteration.
				if (child.count == 0) {
					++num_empty_leaves;
				}
				else if (child.count == BranchIdentifier) {
					to_process.push_back(childIndex);
				}
			}

			// If all the children were empty leaves, remove them and
			// make this node the new empty leaf.
			if (num_empty_leaves == 4) {
				// Push all 4 children to the free list.
				this->_freeNodes(node.firstChild);

				// Make this node the new empty leaf.
				node.firstChild = EndOfList;
				node.count = 0;
			}
		}
	}

	template<typename T>
	int QuadTree<T>::_allocNodes()
	{
		if (this->_firstFreeNode == EndOfList) {
			for (int i = 0; i < 4; i++) {
				this->_nodes.emplace_back(-2, 0);
			}
			return static_cast<int>(this->_nodes.size()) - 4;
		}
		int tmp = this->_firstFreeNode;
		for (int i = 0; i < 4; i++) {
			this->_firstFreeNode = this->_nodes[this->_firstFreeNode].firstChild;
		}
		return tmp;
	}

	template<typename T>
	void QuadTree<T>::_freeNodes(int firstNodeIndex)
	{
		firstNodeIndex += 3;
		for (int i = 0; i < 4; i++) {
			this->_freeNode(firstNodeIndex--);
		}
	}

	template<typename T>
	void QuadTree<T>::_freeNode(int nodeIndex)
	{
		this->_nodes[nodeIndex].firstChild = this->_firstFreeNode;
		this->_firstFreeNode = nodeIndex;
	}

	template<typename T>
	void QuadTree<T>::reCreate()
	{
		this->_elementNodes.reset();
		this->_resetNodes();

		this->_elements.forEach([this](const std::shared_ptr<T> &, int elementIndex) {
			this->_addElementInTree(elementIndex, RootNodeIndex, this->_rootRect, 0);
			return true;
		});

	}

	template<typename T>
	void QuadTree<T>::_resetNodes()
	{
		int nodeSize = this->_nodes.size();

		this->_nodes[0].count = 0;
		if (nodeSize < 2) {
			return;
		}
		this->_firstFreeNode = 1;
		for (int i = 1; i < nodeSize; i++) {
			this->_nodes[i].firstChild = i == nodeSize - 1 ? EndOfList : i + 1;
		}
	}
}

