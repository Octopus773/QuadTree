//
// Created by cbihan on 05/08/2021.
//

#include "World.hpp"

#include <utility>
#include <functional>

namespace QuadTree
{

	World::World(double height, double width)
		: _width(width),
		  _height(height),
		  _maxPolygonPerDivision(3),
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
		this->population[polygon->getUID()] = std::make_pair(polygon, std::vector<std::reference_wrapper<QuadNode>>{});
		this->addPolygonInTree(this->rootNode, polygon->getUID());
	}

	void World::addPolygonInTree(QuadNode &node, unsigned int polygonUID)
	{
		if (node.children.empty()) {
			if (node.populationUIDs.size() >= this->_maxPolygonPerDivision) {
				this->splitLeaf(node);
				return this->addPolygonInTree(node, polygonUID);
			}
			node.populationUIDs.emplace_back(polygonUID);
			this->population[polygonUID].second.emplace_back(node);
			return;
		}
		auto &polygon = this->population.at(polygonUID);
		auto points = polygon.first->getPoints();
		int index = 0;
		if (points[0].first > node.originHorizontal + (node.width / 2)) {
			index++;
		}
		if (points[0].second > node.originVertical + (node.height / 2)) {
			index += 2;
		}
		this->addPolygonInTree(node.children.at(index), polygonUID);
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
			auto &polygon = this->population[uid];
			std::erase(polygon.second, std::ref(leaf));
			this->addPolygonInTree(leaf, uid);
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