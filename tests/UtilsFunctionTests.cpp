//
// Created by cbihan on 08/08/2021.
//

#include <catch2/catch.hpp>
#include <array>

#define private public
#include "QuadTree.hpp"
#include "Point.hpp"

TEST_CASE("getQuadrant function all uses 0 0 100 100", "[QuadTree][getQuadrant]")
{

	std::array<double, 4> rect{0, 0, 100, 100};

	REQUIRE(QuadTree::QuadTree<QuadTree::Tests::Point>::_getQuadrant(rect, QuadTree::Quadrant::topLeft) == std::array<double, 4>{0, 0, 50, 50});
	REQUIRE(QuadTree::QuadTree<QuadTree::Tests::Point>::_getQuadrant(rect, QuadTree::Quadrant::topRight) == std::array<double, 4>{50, 0, 100, 50});
	REQUIRE(QuadTree::QuadTree<QuadTree::Tests::Point>::_getQuadrant(rect, QuadTree::Quadrant::bottomLeft) == std::array<double, 4>{0, 50, 50, 100});
	REQUIRE(QuadTree::QuadTree<QuadTree::Tests::Point>::_getQuadrant(rect, QuadTree::Quadrant::bottomRight) == std::array<double, 4>{50, 50, 100, 100});
}

TEST_CASE("getQuadrant function all uses 30 56 457 1078", "[QuadTree][getQuadrant]")
{

	std::array<double, 4> rect{30, 56, 457, 1078};

	REQUIRE(QuadTree::QuadTree<QuadTree::Tests::Point>::_getQuadrant(rect, QuadTree::Quadrant::topLeft) == std::array<double, 4>{30, 56, 243.5, 567});
	REQUIRE(QuadTree::QuadTree<QuadTree::Tests::Point>::_getQuadrant(rect, QuadTree::Quadrant::topRight) == std::array<double, 4>{243.5, 56, 457, 567});
	REQUIRE(QuadTree::QuadTree<QuadTree::Tests::Point>::_getQuadrant(rect, QuadTree::Quadrant::bottomLeft) == std::array<double, 4>{30, 567, 243.5, 1078});
	REQUIRE(QuadTree::QuadTree<QuadTree::Tests::Point>::_getQuadrant(rect, QuadTree::Quadrant::bottomRight) == std::array<double, 4>{243.5, 567, 457, 1078});
}