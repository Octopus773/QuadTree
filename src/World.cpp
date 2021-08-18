//
// Created by cbihan on 05/08/2021.
//

#include "World.hpp"

#include <functional>
#include "PolygonTypes/APolygon.hpp"
#include "Utils.hpp"
#include "Collisions.hpp"

namespace QuadTree
{

	World::World(double height, double width)
		: _maxPolygonPerDivision(3),
		  _rootNode(0, 0, width, height)
	{
	}

	void World::addPolygon(APolygon *polygon)
	{
		if (!polygon || polygon->getPoints().empty()) {
			return;
		}
		static unsigned int uuidInc = 0;
		polygon->setUID(uuidInc++);
		this->population.insert({polygon->getUID(), ElementInfo(polygon)});
		this->addPolygonInTree(this->_rootNode, this->population.at(polygon->getUID()));
	}

	void World::addPolygonInTree(Quadrant &node, ElementInfo &polygonInfo)
	{
		if (node.children.empty()) {
			if (node.populationUIDs.size() >= this->_maxPolygonPerDivision) {
				this->splitLeaf(node);
				return this->addPolygonInTree(node, polygonInfo);
			}
			node.populationUIDs.emplace_back(polygonInfo.polygon->getUID());
			polygonInfo.references.emplace_back(node);
			return;
		}

		for (auto &quadrant : node.children) {
			if (Collisions::pointInRect(quadrant.pos, polygonInfo.polygon->getPoints().front())) {
				this->addPolygonInTree(quadrant, polygonInfo);
			}
			/*if (Collisions::isOverlapping(quadrant.pos, polygonInfo.aabb)) {
				this->addPolygonInTree(quadrant, polygonInfo);
			}*/
		}
	}

	void World::splitLeaf(Quadrant &leaf)
	{
		if (!leaf.children.empty()) {
			return;
		}
		double midWidth = leaf.pos.getWidth() / 2;
		double midHeight = leaf.pos.getHeight() / 2;
		for (int i = 0; i < 4; i++) {
			auto quadMinVert = leaf.pos.minVertical + ((i > 1) * midHeight);
			auto quadMinHori = leaf.pos.minHorizontal + ((i & 1) * midWidth);
			leaf.children.emplace_back(quadMinHori,
			                           quadMinVert,
			                           quadMinHori + midWidth,
			                           quadMinVert + midHeight);
		}
		for (const auto &uid : leaf.populationUIDs) {
			auto &polygon = this->population.find(uid)->second;
			std::erase(polygon.references, std::ref(leaf));
			this->addPolygonInTree(leaf, this->population.at(uid));
		}
		leaf.populationUIDs.clear();
	}

	std::unordered_set<APolygon *> World::getNeighbours(unsigned int polygonUID)
	{
		std::vector<unsigned int> neighboursUIDs;
		std::unordered_set<APolygon *> neighbours;
		const auto &polygonRefs = this->population.at(polygonUID).references;
		for (const auto &ref : polygonRefs) {
			std::copy_if(ref.get().populationUIDs.begin(),
			             ref.get().populationUIDs.end(),
			             std::back_inserter(neighboursUIDs),
			             [polygonUID, &neighboursUIDs](const unsigned int poly) {
				             return polygonUID != poly
				                    && std::find(neighboursUIDs.begin(),
				                                 neighboursUIDs.end(),
				                                 poly) == neighboursUIDs.end();
			             });
		}
		for (const auto &neighbourUID : neighboursUIDs) {
			neighbours.insert(this->population.at(neighbourUID).polygon);
		}
		return neighbours;
	}

}