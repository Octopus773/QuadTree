//
// Created by cbihan on 18/08/2021.
//

#include <catch2/catch.hpp>
#include "Collisions.hpp"

using namespace QuadTree;

TEST_CASE("onSegment basic use", "[QuadTree][Collisions][onSegment]")
{
	CHECK(Collisions::onSegment({0, 0}, {5, 5}, {10, 10}));
	CHECK(Collisions::onSegment({1, 3.2}, {1, 4}, {1, 4.1}));
	CHECK(Collisions::onSegment({1, 3.2}, {1, 3.2}, {1, 3.1}));

	// not in
	CHECK(!Collisions::onSegment({1, 3.2}, {1, 4.2}, {1, 3.1}));
	CHECK(!Collisions::onSegment({0, 0}, {0, 15}, {0, 10}));
}

TEST_CASE("isPointInsideConvexPolygon functional tests", "[QuadTree][Collisions][isPointInsideConvexPolygon]")
{
	std::vector<std::pair<double, double>> polygon1 = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};
	std::pair<double, double> p = {20, 20};
	CHECK(!Collisions::isPointInsideConvexPolygon(polygon1, p));

	p = {5, 5};
	CHECK(Collisions::isPointInsideConvexPolygon(polygon1, p));

	std::vector<std::pair<double, double>> polygon2 = {{0, 0}, {5, 5}, {5, 0}};
	p = {3, 3};
	CHECK(Collisions::isPointInsideConvexPolygon(polygon2, p));

	p = {5, 1};
	CHECK(Collisions::isPointInsideConvexPolygon(polygon2, p));

	p = {8, 1};
	CHECK(!Collisions::isPointInsideConvexPolygon(polygon2, p));

	std::vector<std::pair<double, double>> polygon3 = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};
	p = {-1,10};
	CHECK(!Collisions::isPointInsideConvexPolygon(polygon3, p));
}

TEST_CASE("areOverlappingConvexPolygon normal use", "[QuadTree][Collisions][areOverlappingConvexPolygon]")
{

}