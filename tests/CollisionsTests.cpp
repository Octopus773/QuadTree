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