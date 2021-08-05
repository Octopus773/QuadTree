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

	// point (4, 4) index: 0
	CHECK(world.rootNode.children[0].children.empty());
	CHECK(world.rootNode.children[0].populationUIDs.size() == 1);
	CHECK(world.rootNode.children[0].populationUIDs[0] == 0);
	CHECK(world.rootNode.children[0].originVertical == 0);
	CHECK(world.rootNode.children[0].originHorizontal == 0);
	CHECK(world.rootNode.children[0].height == 5);
	CHECK(world.rootNode.children[0].width == 5);

	// point (7, 3) inddex: 1
	CHECK(world.rootNode.children[1].children.empty());
	CHECK(world.rootNode.children[1].populationUIDs.size() == 1);
	CHECK(world.rootNode.children[1].populationUIDs[0] == 1);
	CHECK(world.rootNode.children[1].originVertical == 0);
	CHECK(world.rootNode.children[1].originHorizontal == 5);
	CHECK(world.rootNode.children[1].height == 5);
	CHECK(world.rootNode.children[1].width == 5);

	// points (6.5, 6); (6, 6.5); (8, 7); (9, 9)
	CHECK(world.rootNode.children[3].children.size() == 4);
	CHECK(world.rootNode.children[3].populationUIDs.empty());
	CHECK(world.rootNode.children[3].originVertical == 5);
	CHECK(world.rootNode.children[3].originHorizontal == 5);
	CHECK(world.rootNode.children[3].height == 5);
	CHECK(world.rootNode.children[3].width == 5);

	//      point (6.5, 6) index: 2 ; (6, 6.5) index: 3
	CHECK(world.rootNode.children[3].children[0].children.empty());
	CHECK(world.rootNode.children[3].children[0].populationUIDs.size() == 2);
	CHECK(world.rootNode.children[3].children[0].populationUIDs[0] == 2);
	CHECK(world.rootNode.children[3].children[0].populationUIDs[1] == 3);
	CHECK(world.rootNode.children[3].children[0].height == 2.5);
	CHECK(world.rootNode.children[3].children[0].width == 2.5);
	CHECK(world.rootNode.children[3].children[0].originHorizontal == 5);
	CHECK(world.rootNode.children[3].children[0].originVertical == 5);

	//      point (8, 7) index: 4
	CHECK(world.rootNode.children[3].children[1].children.empty());
	CHECK(world.rootNode.children[3].children[1].populationUIDs.size() == 1);
	CHECK(world.rootNode.children[3].children[1].populationUIDs[0] == 4);
	CHECK(world.rootNode.children[3].children[1].height == 2.5);
	CHECK(world.rootNode.children[3].children[1].width == 2.5);
	CHECK(world.rootNode.children[3].children[1].originHorizontal == 7.5);
	CHECK(world.rootNode.children[3].children[1].originVertical == 5);

	//      check empty
	CHECK(world.rootNode.children[3].children[QuadTree::Sq::BottomLeft].children.empty());
	CHECK(world.rootNode.children[3].children[QuadTree::Sq::BottomLeft].populationUIDs.empty());
	CHECK(world.rootNode.children[3].children[QuadTree::Sq::BottomLeft].height == 2.5);
	CHECK(world.rootNode.children[3].children[QuadTree::Sq::BottomLeft].width == 2.5);
	CHECK(world.rootNode.children[3].children[QuadTree::Sq::BottomLeft].originHorizontal == 5);
	CHECK(world.rootNode.children[3].children[QuadTree::Sq::BottomLeft].originVertical == 7.5);


	//      point (9, 9) index: 5
	CHECK(world.rootNode.children[3].children[3].children.empty());
	CHECK(world.rootNode.children[3].children[3].populationUIDs.size() == 1);
	CHECK(world.rootNode.children[3].children[3].populationUIDs[0] == 5);
	CHECK(world.rootNode.children[3].children[3].height == 2.5);
	CHECK(world.rootNode.children[3].children[3].width == 2.5);
	CHECK(world.rootNode.children[3].children[3].originHorizontal == 7.5);
	CHECK(world.rootNode.children[3].children[3].originVertical == 7.5);

	for (const auto &point : points) {
		delete point;
	}
}