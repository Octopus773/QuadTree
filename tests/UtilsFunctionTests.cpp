//
// Created by cbihan on 08/08/2021.
//

#include <catch2/catch.hpp>
#include <PolygonTypes/Point.hpp>

#include "Rect.hpp"
#include "Utils.hpp"
#include "PolygonTypes/Point.hpp"

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

TEST_CASE("getAABB normal on Point", "[QuadTree][Rect][getAABB]")
{
	Rect rect(34, 89, 34, 89);

	Point point(34, 89);

	CHECK(Utils::getAABB(&point) == rect);
}

TEST_CASE("getAABB normal on square", "[QuadTree][Rect][getAABB]")
{
	Rect rect(0, 0, 10, 10);


	CHECK(Utils::getAABB({
		{0, 0},
		{10, 0},
		{10, 10},
		{0, 10}
	}) == rect);
}

TEST_CASE("getAABB normal on ascending line", "[QuadTree][Rect][getAABB]")
{
	Rect rect(3, -5, 23, 34);


	CHECK(Utils::getAABB({
		{3, -5},
		{23, 34}
	}) == rect);
}

TEST_CASE("getAABB normal on descending line", "[QuadTree][Rect][getAABB]")
{
	Rect rect(-8.4, 15, 2, 17);


	CHECK(Utils::getAABB({
		{-8.4, 17},
		{2, 15}
	}) == rect);
}