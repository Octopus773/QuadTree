//
// Created by cbihan on 05/08/2021.
//

#pragma once

#include <vector>
#include <unordered_set>
#include <map>
#include <functional>
#include "PolygonTypes/APolygon.hpp"
#include "Quadrant.hpp"
#include "ElementInfo.hpp"

namespace QuadTree
{

	enum Qd
	{
		TopLeft = 0,
		TopRight = 1,
		BottomLeft = 2,
		BottomRight = 3
	};

	class World
	{
	private:

		//! @brief The maximum number of polygon per division
		//! @warning The leaf node can have more member if it has reached the maxDepth
		unsigned int _maxPolygonPerDivision;
		//! @brief Set the maximum depth for leaf nodes
		//! @note Prevent the quadtree from crashing if more than this->_maxPolygonPerDivision are undividable (like the same polygon)
		unsigned int _maxDepth;
		//! @brief The map containing all of the world polygons (we access them by their UID)
		std::map<unsigned int, ElementInfo> population;
		//! @brief The root node for the quadtree
		Quadrant _rootNode;

		//! @brief add a polygon in the current tree
		//! @param node The node we want to add the polygon (it will be attached to it's children if it needs to)
		//! @param polygonInfo All the infos of the polygon (ElementInfo should be stored in population class member)
		//! @param depth The current depth of the node it allows to check with maxDepth parameter
		void addPolygonInTree(Quadrant &node, ElementInfo &polygonInfo, unsigned int depth = 0);

		//! @brief Remove the polygon in the tree
		//! @param polygonInfo The element to remove
		void removePolygonInTree(ElementInfo &polygonInfo);

		//! @brief Transform a leaf into a parent of leaves and dispatch it's current polygons
		//! @param leaf The leaf to transform
		//! @param depth The depth of the leaf in the tree
		void splitLeaf(Quadrant &leaf, unsigned int depth);

	public:

		//! @brief Set the number max of Polygons per Division
		//! @param maxPolygons The number max of polygons
		//! @warning The leaf node can have more member if it has reached the maxDepth
		void setMaxPolygonPerDivision(unsigned int maxPolygons);

		//! @brief Get the current number mox of polygons per division
		[[nodiscard]] unsigned int getMaxPolygonPerDivision() const;

		//! @brief Set the max depth for a leaf node
		//! @param maxDepth The maximum depth (0 = root node)
		//! @note Setting a new maxDepth wont reload the quadtree
		void setMaxDepth(unsigned int maxDepth);

		//! @brief Get the current maxDepth
		[[nodiscard]] unsigned int getMaxDepth() const;

		//! @brief Add a polygon to the world
		//! @param polygon The polygon to add
		void addPolygon(APolygon *polygon);

		//! @brief Remove completely a polygon from the world
		//! @param polygon The polygon to remove
		void removePolygon(APolygon *polygon);

		//! @brief Update the polygon in the tree
		//! @param polygon The polygon to update
		//! @note Call this function if you changed the result of polygon->getPoints();
		void updatePolygon(APolygon *polygon);


		std::unordered_set<APolygon *> getNeighbours(unsigned int polygonUID);

		World(double height, double width, unsigned int maxPolygonPerDivision = 3, unsigned int maxDepth = 5);

		World(const World &) = delete;

		~World() = default;

		World &operator=(const World &) = delete;
	};
}