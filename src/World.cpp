//
// Created by cbihan on 05/08/2021.
//

#include "World.hpp"

#include <utility>

namespace QuadTree
{

	World::World(double height, double width)
		: _width(width),
		  _height(height),
		  _maxPolygonPerDivision(3),
		  _totalPopulation(0),
		  rootNode(width, height, 0, 0)
	{
	}

	void World::addPolygon(APolygon *polygon)
	{
		if (!polygon || polygon->getPoints().empty()) {
			return;
		}
		static unsigned int uuidInc = 0;
		polygon->setUID(uuidInc++);
		this->population[polygon->getUID()] = polygon;
		this->addPolygonInTree(this->rootNode, polygon);
	}

	void World::addPolygonInTree(QuadNode &node, APolygon *polygon)
	{
		if (node.children.empty()) {
			if (node.populationUIDs.size() >= this->_maxPolygonPerDivision) {
				this->splitLeaf(node);
				return this->addPolygonInTree(node, polygon);
			}
			node.populationUIDs.emplace_back(polygon->getUID());
			return;
		}
		auto points = polygon->getPoints();
		int index = 0;
		if (points[0].first > node.originHorizontal + (node.width / 2)) {
			index++;
		}
		if (points[0].second > node.originVertical + (node.height / 2)) {
			index += 2;
		}
		this->addPolygonInTree(node.children.at(index), polygon);
	}

	void World::splitLeaf(QuadNode &leaf)
	{
		if (!leaf.children.empty()) {
			return;
		}
		double midWidth = leaf.width / 2;
		double midHeight = leaf.height / 2;
		for (int i = 0; i < 4; i++) {
			leaf.children.emplace_back(midWidth, midHeight,
			                           leaf.originHorizontal + ((i & 1) * midWidth),
			                           leaf.originVertical + ((i > 1) * midHeight));
		}
		for (const auto &uid : leaf.populationUIDs) {
			this->addPolygonInTree(leaf, this->population.at(uid));
		}
		leaf.populationUIDs.clear();
	}

	QuadNode::QuadNode(double w, double h, double oH,
	                   double oV)
		: width(w),
		  height(h),
		  originHorizontal(oH),
		  originVertical(oV)
	{
	}
}