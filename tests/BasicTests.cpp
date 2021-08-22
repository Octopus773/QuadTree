//
// Created by cbihan on 05/08/2021.
//

#include <catch2/catch.hpp>

#define private public

#include "QuadTree.hpp"
#include "Quadrant.hpp"
#include "PolygonTypes/Point.hpp"

TEST_CASE("Functional test Normal use of quadtree points", "[QuadTree][Basic]")
{
	QuadTree::QuadTree world(10, 10);

	CHECK(world._rootNode.pos.getWidth() == 10);
	CHECK(world._rootNode.pos.getHeight() == 10);
	CHECK(world._maxPolygonPerDivision == 3);
	CHECK(world._rootNode.pos.minHorizontal == 0);
	CHECK(world._rootNode.pos.minVertical == 0);

	std::vector<QuadTree::Point *> points({
		                                      new QuadTree::Point{4, 4, "0"},
		                                      new QuadTree::Point{7, 3, "1"},
		                                      new QuadTree::Point{6.5, 6, "2"},
		                                      new QuadTree::Point{6, 6.5, "3"},
		                                      new QuadTree::Point{8, 7, "4"},
		                                      new QuadTree::Point{9, 9, "5"},
	                                      });

	CHECK(points[0]->getPoints().size() == 1);
	CHECK(points[1]->getPoints()[0].second == 3);
	CHECK(points[1]->getPoints()[0].first == 7);

	for (const auto &point : points) {
		world.addPolygon(point);
	}
	CHECK(world.population.size() == 6);


	CHECK(world._rootNode.populationUIDs.empty());
	CHECK(world._rootNode.children.size() == 4);

	// point (4, 4) index: 0
	CHECK(world._rootNode.children[0].children.empty());
	CHECK(world._rootNode.children[0].populationUIDs.size() == 1);
	auto it = world._rootNode.children[0].populationUIDs.begin();
	CHECK(world.population[*it].polygon->getName() == "0");
	CHECK(world._rootNode.children[0].pos.minVertical == 0);
	CHECK(world._rootNode.children[0].pos.minHorizontal == 0);
	CHECK(world._rootNode.children[0].pos.maxHorizontal == 5);
	CHECK(world._rootNode.children[0].pos.maxVertical == 5);
	CHECK(world.population[0].references.size() == 1);
	CHECK(world.population[0].references[0] == world._rootNode.children[0]);
	CHECK(world.population[0].polygon == points[0]);

	// point (7, 3) inddex: 1
	CHECK(world._rootNode.children[1].children.empty());
	CHECK(world._rootNode.children[1].populationUIDs.size() == 1);
	it = world._rootNode.children[1].populationUIDs.begin();
	CHECK(world.population[*it].polygon->getName() == "1");
	CHECK(world._rootNode.children[1].pos.minVertical == 0);
	CHECK(world._rootNode.children[1].pos.minHorizontal == 5);
	CHECK(world._rootNode.children[1].pos.getWidth() == 5);
	CHECK(world._rootNode.children[1].pos.getHeight() == 5);
	CHECK(world.population[0].references.size() == 1);
	CHECK(world.population[1].references[0] == world._rootNode.children[1]);
	CHECK(world.population[1].polygon == points[1]);


	// points (6.5, 6); (6, 6.5); (8, 7); (9, 9)
	CHECK(world._rootNode.children[3].children.size() == 4);
	CHECK(world._rootNode.children[3].populationUIDs.empty());
	CHECK(world._rootNode.children[3].pos.minVertical == 5);
	CHECK(world._rootNode.children[3].pos.minHorizontal == 5);
	CHECK(world._rootNode.children[3].pos.getWidth() == 5);
	CHECK(world._rootNode.children[3].pos.getHeight() == 5);

	//      point (6.5, 6) index: 2 ; (6, 6.5) index: 3
	CHECK(world._rootNode.children[3].children[0].children.empty());
	CHECK(world._rootNode.children[3].children[0].populationUIDs.size() == 2);
	it = world._rootNode.children[3].children[0].populationUIDs.begin();
	CHECK(world.population[*it].polygon->getName() == "2");
	std::advance(it, 1);
	CHECK(world.population[*it].polygon->getName() == "3");
	CHECK(world._rootNode.children[3].children[0].pos.getWidth() == 2.5);
	CHECK(world._rootNode.children[3].children[0].pos.getHeight() == 2.5);
	CHECK(world._rootNode.children[3].children[0].pos.minHorizontal == 5);
	CHECK(world._rootNode.children[3].children[0].pos.minVertical == 5);

	CHECK(world.population[2].references.size() == 1);
	CHECK(world.population[2].references[0] == world._rootNode.children[3].children[0]);
	CHECK(world.population[2].polygon == points[2]);
	CHECK(world.population[3].references.size() == 1);
	CHECK(world.population[3].references[0] == world._rootNode.children[3].children[0]);
	CHECK(world.population[3].polygon == points[3]);



	//      point (8, 7) index: 4
	CHECK(world._rootNode.children[3].children[1].children.empty());
	CHECK(world._rootNode.children[3].children[1].populationUIDs.size() == 1);
	it = world._rootNode.children[3].children[1].populationUIDs.begin();
	CHECK(world.population[*it].polygon->getName() == "4");
	CHECK(world._rootNode.children[3].children[1].pos.getWidth() == 2.5);
	CHECK(world._rootNode.children[3].children[1].pos.getHeight() == 2.5);
	CHECK(world._rootNode.children[3].children[1].pos.minHorizontal == 7.5);
	CHECK(world._rootNode.children[3].children[1].pos.minVertical == 5);

	CHECK(world.population[4].references.size() == 1);
	CHECK(world.population[4].references[0] == world._rootNode.children[3].children[1]);
	CHECK(world.population[4].polygon == points[4]);

	//      check empty
	CHECK(world._rootNode.children[3].children[QuadTree::Qd::BottomLeft].children.empty());
	CHECK(world._rootNode.children[3].children[QuadTree::Qd::BottomLeft].populationUIDs.empty());
	CHECK(world._rootNode.children[3].children[QuadTree::Qd::BottomLeft].pos.getWidth() == 2.5);
	CHECK(world._rootNode.children[3].children[QuadTree::Qd::BottomLeft].pos.getHeight() == 2.5);
	CHECK(world._rootNode.children[3].children[QuadTree::Qd::BottomLeft].pos.minHorizontal == 5);
	CHECK(world._rootNode.children[3].children[QuadTree::Qd::BottomLeft].pos.minVertical == 7.5);


	//      point (9, 9) index: 5
	CHECK(world._rootNode.children[3].children[3].children.empty());
	CHECK(world._rootNode.children[3].children[3].populationUIDs.size() == 1);
	it = world._rootNode.children[3].children[3].populationUIDs.begin();
	CHECK(world.population[*it].polygon->getName() == "5");
	CHECK(world._rootNode.children[3].children[3].pos.getWidth() == 2.5);
	CHECK(world._rootNode.children[3].children[3].pos.getHeight() == 2.5);
	CHECK(world._rootNode.children[3].children[3].pos.minHorizontal == 7.5);
	CHECK(world._rootNode.children[3].children[3].pos.minVertical == 7.5);

	CHECK(world.population[5].references.size() == 1);
	CHECK(world.population[5].references[0] == world._rootNode.children[3].children[3]);
	CHECK(world.population[5].polygon == points[5]);

	CHECK(world.getNeighbours(5).empty());

	CHECK(world.getNeighbours(2).size() == 1);
	CHECK(*world.getNeighbours(2).begin() == points[3]);

	for (const auto &point : points) {
		delete point;
	}
}
