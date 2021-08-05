//
// Created by cbihan on 05/08/2021.
//

#pragma once

#include <vector>
#include <map>
#include "PopulationTypes.hpp"

namespace QuadTree
{

	class QuadNode
	{
	public:
		std::vector<QuadNode> children;
		//! @brief Contains the UUID of each element contained int this QuadNode
		//! @note Only leaves nodes contains UIDs
		std::vector<unsigned int> populationUIDs;

		[[nodiscard]] bool isLeave() const;

	};

	class World
	{
	private:
		unsigned int _width;
		unsigned int _height;
		unsigned int _maxPopulationPerDivision;
		unsigned int _totalPopulation;
		std::map<unsigned int, APolygon> population;

		QuadNode rootNode;

		void addPolygonInTree(APolygon &polygon);
	public:

		void addPolygon(APolygon polygon);

		World(unsigned int height, unsigned  int width);
		World(const World &) = delete;
		~World() = default;
		World &operator=(const World &) = delete;
	};
}