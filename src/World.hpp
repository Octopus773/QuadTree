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

		unsigned int width;
		unsigned int height;
		unsigned int originHorizontal;
		unsigned int originVertical;

		QuadNode(unsigned int w,
		         unsigned int h,
		         unsigned int oH,
		         unsigned int oV);


	};

	class World
	{
	private:
		unsigned int _width;
		unsigned int _height;
		unsigned int _maxPopulationPerDivision;
		unsigned int _totalPopulation;
		std::map<unsigned int, APolygon *> population;

		QuadNode rootNode;

		void addPolygonInTree(QuadNode &node, APolygon *polygon);

		void splitLeaf(QuadNode &leaf);

	public:

		void addPolygon(APolygon *polygon);

		World(unsigned int height, unsigned int width);

		World(const World &) = delete;

		~World() = default;

		World &operator=(const World &) = delete;
	};
}