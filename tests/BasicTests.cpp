//
// Created by cbihan on 05/08/2021.
//

#include <catch2/catch.hpp>

#define private public

#include "Point.hpp"
#include "QuadTree.hpp"


TEST_CASE("QuadTree Basic Use", "[QuadTree]")
{
	QuadTree::QuadTree<QuadTree::Tests::Point> qT(0, 0, 10, 10);

	std::vector<std::shared_ptr<QuadTree::Tests::Point>> points;

	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(4, 4, "0"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(7, 3, "1"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6.5, 6, "2"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6, 6.5, "3"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(8, 7, "4"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(9, 9, "5"));

	for (const auto &pt : points) {
		qT.add(pt);
	}

	for (const auto &pt : points) {
		qT.getNeighbours(pt);
	}



}
