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

	enum Sq {
		TopLeft = 0,
		TopRight = 1,
		BottomLeft = 2,
		BottomRight = 3
	};

	class World
	{
	private:
		//! @brief The width of the world
		double _width;
		//! @brief The height of the world
		double _height;
		//! @brief The maximum number of polygon per division
		unsigned int _maxPolygonPerDivision;
		//! @brief The map containing all of the world polygons (we access them by their UID)
		std::map<unsigned int, ElementInfo> population;
		//! @brief The root node for the quadtree
		Quadrant rootNode;

		//! @brief add a polygon in the current tree
		//! @param node The node we want to add the polygon (it will be attached to it's children if it needs to)
		//! @param polygon The polygon to add
		void addPolygonInTree(Quadrant &node, unsigned int polygonUID);

		//! @brief Transform a leaf into a parent of leaves and dispatch it's current polygons
		//! @param leaf The leaf to transform
		void splitLeaf(Quadrant &leaf);

	public:
		//! @brief Add a polygon to the world
		void addPolygon(APolygon *polygon);


		std::unordered_set<APolygon *> getNeighbours(unsigned int polygonUID);

		World(double height, double width);

		World(const World &) = delete;

		~World() = default;

		World &operator=(const World &) = delete;
	};
}