//
// Created by cbihan on 05/08/2021.
//

#include <catch2/catch.hpp>

#define private public
#include "World.hpp"
#include "PolygonTypes.hpp"

TEST_CASE("Normal use of quadtree points", "[QuadTree][Basic]")
{
	QuadTree::World world(10, 10);

	CHECK(world._width == 10);
	CHECK(world._height == 10);
	CHECK(world._maxPolygonPerDivision == 3);
	CHECK(world.rootNode.originHorizontal == 0);
	CHECK(world.rootNode.originVertical == 0);

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


	for (const auto &point : points) {
		delete point;
	}
}