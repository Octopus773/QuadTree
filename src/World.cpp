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

	World::World(double height, double width, unsigned int maxPolygonPerDivision, unsigned int maxDepth)
		: _maxPolygonPerDivision(maxPolygonPerDivision),
		  _maxDepth(maxDepth),
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

	void World::addPolygonInTree(Quadrant &node, ElementInfo &polygonInfo, unsigned int depth)
	{
		if (node.children.empty()) {
			if (depth < this->_maxDepth && node.populationUIDs.size() >= this->_maxPolygonPerDivision) {
				this->splitLeaf(node, depth);
				return this->addPolygonInTree(node, polygonInfo, depth + 1);
			}
			node.populationUIDs.emplace_back(polygonInfo.polygon->getUID());
			polygonInfo.references.emplace_back(node);
			return;
		}

		for (auto &quadrant : node.children) {
			if (Collisions::isOverlapping(quadrant.pos, polygonInfo.aabb)) {
				this->addPolygonInTree(quadrant, polygonInfo, depth + 1);
			}
		}
	}

	void World::splitLeaf(Quadrant &leaf, unsigned int depth)
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
			this->addPolygonInTree(leaf, this->population.at(uid), depth + 1);
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

	void World::removePolygonInTree(ElementInfo &polygonInfo)
	{
		auto uid = polygonInfo.polygon->getUID();
		for (auto &reference : polygonInfo.references) {
			auto &populationUIDs = reference.get().populationUIDs;
			populationUIDs.erase(std::remove(populationUIDs.begin(), populationUIDs.end(), uid), populationUIDs.end());
		}
		polygonInfo.references.clear();
	}

	void World::removePolygon(APolygon *polygon)
	{
		if (!this->population.contains(polygon->getUID())) {
			return;
		}
		auto &individual = this->population[polygon->getUID()];
		this->removePolygonInTree(individual);
		this->population.erase(polygon->getUID());
	}

	void World::updatePolygon(APolygon *polygon)
	{
		if (!this->population.contains(polygon->getUID())) {
			return;
		}
		this->removePolygonInTree(this->population[polygon->getUID()]);
		this->population[polygon->getUID()].aabb = Utils::getAABB(polygon);
		this->addPolygonInTree(this->_rootNode, this->population[polygon->getUID()]);
	}

	void World::setMaxPolygonPerDivision(unsigned int maxPolygons)
	{
		this->_maxPolygonPerDivision = maxPolygons;
	}

	unsigned int World::getMaxPolygonPerDivision() const
	{
		return this->_maxPolygonPerDivision;
	}

	void World::setMaxDepth(unsigned int maxDepth)
	{
		this->_maxDepth = maxDepth;
	}

	unsigned int World::getMaxDepth() const
	{
		return this->_maxDepth;
	}

}