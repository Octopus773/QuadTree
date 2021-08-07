//
// Created by cbihan on 05/08/2021.
//

#include <catch2/catch.hpp>

#define private public

#include "World.hpp"
#include "Quadrant.hpp"
#include "PolygonTypes/Point.hpp"

TEST_CASE("Functional test Normal use of quadtree points", "[QuadTree][Basic]")
{
	QuadTree::World world(10, 10);

	CHECK(world._width == 10);
	CHECK(world._height == 10);
	CHECK(world._maxPolygonPerDivision == 3);
	CHECK(world.rootNode.minHorizontal == 0);
	CHECK(world.rootNode.minVertical == 0);

	std::vector<QuadTree::Point *> points({
		                                      new QuadTree::Point{4, 4},
		                                      new QuadTree::Point{7, 3},
		                                      new QuadTree::Point{6.5, 6},
		                                      new QuadTree::Point{6, 6.5},
		                                      new QuadTree::Point{8, 7},
		                                      new QuadTree::Point{9, 9},
	                                      });

	CHECK(points[0]->getPoints().size() == 1);
	CHECK(points[1]->getPoints()[0].second == 3);
	CHECK(points[1]->getPoints()[0].first == 7);

	for (const auto &point : points) {
		world.addPolygon(point);
	}
	CHECK(world.population.size() == 6);


	CHECK(world.rootNode.populationUIDs.empty());
	CHECK(world.rootNode.children.size() == 4);

	// point (4, 4) index: 0
	CHECK(world.rootNode.children[0].children.empty());
	CHECK(world.rootNode.children[0].populationUIDs.size() == 1);
	CHECK(world.rootNode.children[0].populationUIDs[0] == 0);
	CHECK(world.rootNode.children[0].minVertical == 0);
	CHECK(world.rootNode.children[0].minHorizontal == 0);
	CHECK(world.rootNode.children[0].maxHorizontal == 5);
	CHECK(world.rootNode.children[0].maxVertical == 5);
	CHECK(world.population[0].references.size() == 1);
	CHECK(world.population[0].references[0] == world.rootNode.children[0]);
	CHECK(world.population[0].polygon == points[0]);

	// point (7, 3) inddex: 1
	CHECK(world.rootNode.children[1].children.empty());
	CHECK(world.rootNode.children[1].populationUIDs.size() == 1);
	CHECK(world.rootNode.children[1].populationUIDs[0] == 1);
	CHECK(world.rootNode.children[1].minVertical == 0);
	CHECK(world.rootNode.children[1].minHorizontal == 5);
	CHECK(world.rootNode.children[1].maxHorizontal == 5);
	CHECK(world.rootNode.children[1].maxVertical == 5);
	CHECK(world.population[0].references.size() == 1);
	CHECK(world.population[1].references[0] == world.rootNode.children[1]);
	CHECK(world.population[1].polygon == points[1]);


	// points (6.5, 6); (6, 6.5); (8, 7); (9, 9)
	CHECK(world.rootNode.children[3].children.size() == 4);
	CHECK(world.rootNode.children[3].populationUIDs.empty());
	CHECK(world.rootNode.children[3].minVertical == 5);
	CHECK(world.rootNode.children[3].minHorizontal == 5);
	CHECK(world.rootNode.children[3].maxHorizontal == 5);
	CHECK(world.rootNode.children[3].maxVertical == 5);

	//      point (6.5, 6) index: 2 ; (6, 6.5) index: 3
	CHECK(world.rootNode.children[3].children[0].children.empty());
	CHECK(world.rootNode.children[3].children[0].populationUIDs.size() == 2);
	CHECK(world.rootNode.children[3].children[0].populationUIDs[0] == 2);
	CHECK(world.rootNode.children[3].children[0].populationUIDs[1] == 3);
	CHECK(world.rootNode.children[3].children[0].maxHorizontal == 2.5);
	CHECK(world.rootNode.children[3].children[0].maxVertical == 2.5);
	CHECK(world.rootNode.children[3].children[0].minHorizontal == 5);
	CHECK(world.rootNode.children[3].children[0].minVertical == 5);

	CHECK(world.population[2].references.size() == 1);
	CHECK(world.population[2].references[0] == world.rootNode.children[3].children[0]);
	CHECK(world.population[2].polygon == points[2]);
	CHECK(world.population[3].references.size() == 1);
	CHECK(world.population[3].references[0] == world.rootNode.children[3].children[0]);
	CHECK(world.population[3].polygon == points[3]);



	//      point (8, 7) index: 4
	CHECK(world.rootNode.children[3].children[1].children.empty());
	CHECK(world.rootNode.children[3].children[1].populationUIDs.size() == 1);
	CHECK(world.rootNode.children[3].children[1].populationUIDs[0] == 4);
	CHECK(world.rootNode.children[3].children[1].maxHorizontal == 2.5);
	CHECK(world.rootNode.children[3].children[1].maxVertical == 2.5);
	CHECK(world.rootNode.children[3].children[1].minHorizontal == 7.5);
	CHECK(world.rootNode.children[3].children[1].minVertical == 5);

	CHECK(world.population[4].references.size() == 1);
	CHECK(world.population[4].references[0] == world.rootNode.children[3].children[1]);
	CHECK(world.population[4].polygon == points[4]);

	//      check empty
	CHECK(world.rootNode.children[3].children[QuadTree::Qd::BottomLeft].children.empty());
	CHECK(world.rootNode.children[3].children[QuadTree::Qd::BottomLeft].populationUIDs.empty());
	CHECK(world.rootNode.children[3].children[QuadTree::Qd::BottomLeft].maxHorizontal == 2.5);
	CHECK(world.rootNode.children[3].children[QuadTree::Qd::BottomLeft].maxVertical == 2.5);
	CHECK(world.rootNode.children[3].children[QuadTree::Qd::BottomLeft].minHorizontal == 5);
	CHECK(world.rootNode.children[3].children[QuadTree::Qd::BottomLeft].minVertical == 7.5);


	//      point (9, 9) index: 5
	CHECK(world.rootNode.children[3].children[3].children.empty());
	CHECK(world.rootNode.children[3].children[3].populationUIDs.size() == 1);
	CHECK(world.rootNode.children[3].children[3].populationUIDs[0] == 5);
	CHECK(world.rootNode.children[3].children[3].maxHorizontal == 2.5);
	CHECK(world.rootNode.children[3].children[3].maxVertical == 2.5);
	CHECK(world.rootNode.children[3].children[3].minHorizontal == 7.5);
	CHECK(world.rootNode.children[3].children[3].minVertical == 7.5);

	CHECK(world.population[5].references.size() == 1);
	CHECK(world.population[5].references[0] == world.rootNode.children[3].children[3]);
	CHECK(world.population[5].polygon == points[5]);

	CHECK(world.getNeighbours(5).empty());

	CHECK(world.getNeighbours(2).size() == 1);
	CHECK(*world.getNeighbours(2).begin() == points[3]);

	for (const auto &point : points) {
		delete point;
	}
}

TEST_CASE("Check init point and QuadNode", "[QuadTree][init]")
{
	auto point = new QuadTree::Point{7, 3};
	QuadTree::Quadrant qn(10, 15, 0, 5);


	CHECK(qn.children.empty());
	CHECK(qn.populationUIDs.empty());
	CHECK(qn.minVertical == 5);
	CHECK(qn.minHorizontal == 0);
	CHECK(qn.maxHorizontal == 15);
	CHECK(qn.maxVertical == 10);

	point->setUID(678087);
	CHECK(point->getUID() == 678087);
	CHECK(point->getPoints().size() == 1);
	CHECK(point->getPoints()[0].first == 7);
	CHECK(point->getPoints()[0].second == 3);

	delete point;
}