//
// Created by cbihan on 05/08/2021.
//

#pragma once

#include <vector>
#include <map>
#include <functional>
#include "PolygonTypes.hpp"

namespace QuadTree
{

	enum Sq {
		TopLeft = 0,
		TopRight = 1,
		BottomLeft = 2,
		BottomRight = 3
	};

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

	inline bool operator==(const QuadNode &qn1, const QuadNode &qn2)
	{
		return qn1.width == qn2.width
		       && qn1.height == qn2.height
		       && qn1.originVertical == qn2.originVertical
		       && qn1.originHorizontal == qn2.originHorizontal;
	}

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
		std::map<unsigned int, std::pair<APolygon *, std::vector<std::reference_wrapper<QuadNode>>>> population;
		//! @brief The root node for the quadtree
		QuadNode rootNode;

		//! @brief add a polygon in the current tree
		//! @param node The node we want to add the polygon (it will be attached to it's children if it needs to)
		//! @param polygon The polygon to add
		void addPolygonInTree(QuadNode &node, unsigned int polygonUID);

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