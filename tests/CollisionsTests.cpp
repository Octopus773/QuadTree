//
// Created by cbihan on 18/08/2021.
//

#include <catch2/catch.hpp>
#include "Collisions.hpp"
#include "Rect.hpp"

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

TEST_CASE("Rect collision basic collision (2points in rect) horizontal", "[QuadTree][Collisions]")
{
	QuadTree::Tests::Rect rect1(0, 0, 100);
	QuadTree::Tests::Rect rect2(10, 90, 80);

	int axis = -1;
	REQUIRE(rect1.collide(rect2, axis));
	REQUIRE(axis == QuadTree::Tests::Rect::HorizontalAxis);

	REQUIRE(rect2.collide(rect1, axis));
	REQUIRE(axis == QuadTree::Tests::Rect::HorizontalAxis);
}

TEST_CASE("Rect collision basic collision (2points in rect) vertical", "[QuadTree][Collisions]")
{
	QuadTree::Tests::Rect rect1(0, 0, 100);
	QuadTree::Tests::Rect rect2(90, 10, 80);

	int axis = -1;
	REQUIRE(rect1.collide(rect2, axis));
	REQUIRE(axis == QuadTree::Tests::Rect::VerticalAxis);

	REQUIRE(rect2.collide(rect1, axis));
	REQUIRE(axis == QuadTree::Tests::Rect::VerticalAxis);
}

TEST_CASE("Rect collision basic no collision", "[QuadTree][Collisions]")
{
	QuadTree::Tests::Rect rect1(0, 0, 100);
	QuadTree::Tests::Rect rect2(10, 101, 80);

	int axis = -1;
	REQUIRE(!rect1.collide(rect2, axis));
	REQUIRE(!rect2.collide(rect1, axis));
}

TEST_CASE("Rect collision basic collision (1point in rect) vertical", "[QuadTree][Collisions]")
{
	QuadTree::Tests::Rect rect1(0, 0, 100);
	QuadTree::Tests::Rect rect2(90, 80, 80);

	int axis = -1;
	REQUIRE(rect1.collide(rect2, axis));
	REQUIRE(axis == QuadTree::Tests::Rect::VerticalAxis);

	REQUIRE(rect2.collide(rect1, axis));
	REQUIRE(axis == QuadTree::Tests::Rect::VerticalAxis);
}

TEST_CASE("Rect collision basic collision (1point in rect) horizontal", "[QuadTree][Collisions]")
{
	QuadTree::Tests::Rect rect1(0, 0, 100);
	QuadTree::Tests::Rect rect2(50, 80, 80);

	int axis = -1;
	REQUIRE(rect1.collide(rect2, axis));
	REQUIRE(axis == QuadTree::Tests::Rect::HorizontalAxis);

	REQUIRE(rect2.collide(rect1, axis));
	REQUIRE(axis == QuadTree::Tests::Rect::HorizontalAxis);
}

TEST_CASE("Rect collision same size basic collision (2points in rect) horizontal", "[QuadTree][Collisions]")
{
	QuadTree::Tests::Rect rect1(0, 0, 100);
	QuadTree::Tests::Rect rect2(0, 100, 100);

	int axis = -1;
	REQUIRE(rect1.collide(rect2, axis));
	REQUIRE(axis == QuadTree::Tests::Rect::HorizontalAxis);

	REQUIRE(rect2.collide(rect1, axis));
	REQUIRE(axis == QuadTree::Tests::Rect::HorizontalAxis);
}

TEST_CASE("Rect collision same size basic collision (2points in rect) vertical", "[QuadTree][Collisions]")
{
	QuadTree::Tests::Rect rect1(0, 0, 100);
	QuadTree::Tests::Rect rect2(100, 0, 100);

	int axis = -1;
	REQUIRE(rect1.collide(rect2, axis));
	REQUIRE(axis == QuadTree::Tests::Rect::VerticalAxis);

	REQUIRE(rect2.collide(rect1, axis));
	REQUIRE(axis == QuadTree::Tests::Rect::VerticalAxis);
}

TEST_CASE("Rect collision same size half basic collision (2points in rect) horizontal", "[QuadTree][Collisions]")
{
	QuadTree::Tests::Rect rect1(0, 0, 10);
	QuadTree::Tests::Rect rect2(0, 5, 10);

	int axis = -1;
	REQUIRE(rect1.collide(rect2, axis));
	REQUIRE(axis == QuadTree::Tests::Rect::HorizontalAxis);

	REQUIRE(rect2.collide(rect1, axis));
	REQUIRE(axis == QuadTree::Tests::Rect::HorizontalAxis);
}