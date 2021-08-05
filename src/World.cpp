//
// Created by cbihan on 05/08/2021.
//

#include "World.hpp"

#include <utility>

namespace QuadTree
{

	World::World(unsigned int height, unsigned int width)
		: _width(width),
		  _height(height),
		  _maxPopulationPerDivision(10),
		  _totalPopulation(0)
	{
	}

	void World::addPolygon(APolygon polygon)
	{
		static unsigned int uuidInc = 0;
		polygon.setUID(uuidInc++);
		this->population[polygon.getUID()] = std::move(polygon);
	}

	void World::addPolygonInTree(APolygon &polygon)
	{
	}

	bool QuadNode::isLeave() const
	{
		return this->children.empty();
	}
}