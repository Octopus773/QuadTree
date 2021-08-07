//
// Created by cbihan on 08/08/2021.
//

#include <catch2/catch.hpp>

#include "Rect.hpp"
#include "Utils.hpp"

using namespace QuadTree;

TEST_CASE("Rect normal use", "[QuadTree][Rect][getHeight/getWidth]")
{
	Rect rect(-2.1, 4, 567.98, 23);

	CHECK(rect.getHeight() == 19);
	CHECK(rect.getWidth() == 570.08);

}

TEST_CASE("pointInRect normal use ", "[QuadTree][Utils][pointInRect]")
{
	Rect rect(0, 0, 10, 10);

	CHECK(Utils::pointInRect(rect, {5, 5}));
	CHECK(Utils::pointInRect(rect, {5, 0}));
	CHECK(Utils::pointInRect(rect, {10, 10}));
	CHECK(Utils::pointInRect(rect, {2.3, 7.898}));
	CHECK(!Utils::pointInRect(rect, {10.01, 10}));
	CHECK(!Utils::pointInRect(rect, {576, -456}));
	CHECK(Utils::pointInRect(rect, {-0, -0}));
	CHECK(!Utils::pointInRect(rect, {-2, 3}));
}