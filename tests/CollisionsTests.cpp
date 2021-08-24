//
// Created by cbihan on 18/08/2021.
//

#include <catch2/catch.hpp>
#include "Collisions.hpp"

using namespace QuadTree::Tests;

TEST_CASE("pointInRect", "[QuadTree][Collisions]")
{
	double rect[4] = {0, 0, 10, 10};
	CHECK(pointInRect(rect, {2.5, 2.5}));
	CHECK(pointInRect(rect, {0, 0}));
	CHECK(pointInRect(rect, {0, 1}));
	CHECK(!pointInRect(rect, {0, -1}));
	CHECK(pointInRect(rect, {10, 10}));
	CHECK(pointInRect(rect, {5, 5}));
	CHECK(pointInRect(rect, {3.4, 8.2}));

	CHECK(pointInRect(rect, {5, 0}));
	CHECK(pointInRect(rect, {2.3, 7.898}));
	CHECK(!pointInRect(rect, {10.01, 10}));
	CHECK(!pointInRect(rect, {576, -456}));
	CHECK(pointInRect(rect, {-0, -0}));
	CHECK(!pointInRect(rect, {-2, 3}));
}

