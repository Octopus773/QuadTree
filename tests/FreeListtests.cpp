//
// Created by cbihan on 24/08/2021.
//

#include <catch2/catch.hpp>

#define private public
#include "FreeList.hpp"

TEST_CASE("FreeList insert & operator[]", "[QuadTree][FreeList]")
{
	QuadTree::FreeList<int> list;

	for (int i = 0; i < 50; i++) {
		list.insert(i);
	}

	for (int i = 0; i < 50; i++) {
		REQUIRE(list[i] == i);
	}

}

TEST_CASE("FreeList remove normal use", "[QuadTree][FreeList]")
{
	QuadTree::FreeList<int> list;

	for (int i = 0; i < 50; i++) {
		list.insert(i);
	}

	list.remove(12);

	for (int i = 0; i < 50; i++) {
		if (i == 12) {
			continue;
		}
		REQUIRE(list[i] == i);
	}

}

TEST_CASE("FreeList remove and insert", "[QuadTree][FreeList]")
{
	QuadTree::FreeList<int> list;

	for (int i = 0; i < 26; i++) {
		list.insert(i);
	}

	for (int i = 0; i < 20; i++) {
		list.remove(i);
	}

	for (int i = 20; i < 26; i++) {
		REQUIRE(list[i] == i);
	}

	for (int i = 0; i < 26; i++) {
		list.insert(i);
	}

	for (int i = 0; i < 26 + 6; i++) {
		if (i >= 26) {
			CHECK(list[i] == i - 6);
		} else {
			CHECK(list[i] == i);
		}
	}

}

TEST_CASE("FreeList toVector", "[QuadTree][FreeList]")
{
	QuadTree::FreeList<int> list;

	for (int i = 0; i < 12; i++) {
		list.insert(i);
	}

	list.remove(3);
	list.remove(10);

	std::vector<int> vector = list.toVector();
	std::vector<int> ref{0, 1, 2, 4, 5, 6, 7, 8, 9, 11};

	CHECK(vector == ref);
}