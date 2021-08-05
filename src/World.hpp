//
// Created by cbihan on 05/08/2021.
//

#pragma once

#include <vector>
#include <map>
#include "PolygonTypes.hpp"

namespace QuadTree
{

	class QuadNode
	{
	public:
		//! @brief Child nodes
		//! @note child at index 0 is top left,
		//!                      1 is top right,
		//!                      2 is bottom left,
		//!                      3 is bottom right
		std::vector<QuadNode> children;
		//! @brief Contains the UUID of each element contained int this QuadNode
		//! @note Only leaves nodes contains UIDs
		std::vector<unsigned int> populationUIDs;

		//! @brief The width of the node
		double width;
		//! @brief The height of the node
		double height;
		//! @brief The base horizontal coordinate (top left corner)
		double originHorizontal;
		//! @brief Te base vertical coordinate (top left corner)
		double originVertical;

		QuadNode(double w,
		         double h,
		         double oH,
		         double oV);


	};

	class World
	{
	private:
		//! @brief The width of the world
		double _width;
		//! @brief The height of the world
		double _height;
		//! @brief The maximum number of polygon per division
		double _maxPolygonPerDivision;
		//! @brief The total number of polygon in the world
		double _totalPopulation;
		//! @brief The map containing all of the world polygons (we access them by their UID)
		std::map<unsigned int, APolygon *> population;
		//! @brief The root node for the quadtree
		QuadNode rootNode;

		//! @brief add a polygon in the current tree
		//! @param node The node we want to add the polygon (it will be attached to it's children if it needs to)
		//! @param polygon The polygon to add
		void addPolygonInTree(QuadNode &node, APolygon *polygon);

		//! @brief Transform a leaf into a parent of leaves and dispatch it's current polygons
		//! @param leaf The leaf to transform
		void splitLeaf(QuadNode &leaf);

	public:
		//! @brief Add a polygon to the world
		void addPolygon(APolygon *polygon);

		World(double height, double width);

		World(const World &) = delete;

		~World() = default;

		World &operator=(const World &) = delete;
	};
}