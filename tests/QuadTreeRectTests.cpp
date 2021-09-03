//
// Created by cbihan on 02/09/2021.
//

#include <catch2/catch.hpp>

#define private public
#include "QuadTree.hpp"
#include "Rect.hpp"
#include "TestsUtils.hpp"

TEST_CASE("Basic Use with Rect usages", "[QuadTree][Rect]")
{
	QuadTree::QuadTree<QuadTree::Tests::Rect> qT(0, 0, 100, 100);

	qT.maxElementsPerNode = 3;
	qT.maxDepth = 5;

	std::vector<std::shared_ptr<QuadTree::Tests::Rect>> rects {};

	rects.emplace_back(std::make_shared<QuadTree::Tests::Rect>(5, 0, 10, 0));
	rects.emplace_back(std::make_shared<QuadTree::Tests::Rect>(5, 12, 10, 1));
	rects.emplace_back(std::make_shared<QuadTree::Tests::Rect>(5, 24, 10, 2));
	rects.emplace_back(std::make_shared<QuadTree::Tests::Rect>(5, 36, 10, 3));
	rects.emplace_back(std::make_shared<QuadTree::Tests::Rect>(5, 48, 10, 4));
	rects.emplace_back(std::make_shared<QuadTree::Tests::Rect>(5, 60, 10, 5));

	for (auto &rect : rects) {
		qT.add(rect);
	}

	REQUIRE(getSizeNodes(qT) == 9);

	for (const auto &rect : rects) {
		auto neighbours = qT.getNeighbours(rect);

		switch (rect->uid) {
		case 0:
			REQUIRE(neighbours.size() == 2);
			for (const auto &value : {1, 2}) {
				REQUIRE(std::find_if(neighbours.begin(), neighbours.end(), [value](const auto &neighbour) {
					return neighbour->uid == value;
				}) != neighbours.end());
			}
			break;
		case 1:
			REQUIRE(neighbours.size() == 2);
			for (const auto &value : {0, 2}) {
				REQUIRE(std::find_if(neighbours.begin(), neighbours.end(), [value](const auto &neighbour) {
					return neighbour->uid == value;
				}) != neighbours.end());
			}
			break;
		case 2:
			REQUIRE(neighbours.size() == 4);
			for (const auto &value : {0, 1, 3, 4}) {
				REQUIRE(std::find_if(neighbours.begin(), neighbours.end(), [value](const auto &neighbour) {
					return neighbour->uid == value;
				}) != neighbours.end());
			}
			break;
		case 3:
			REQUIRE(neighbours.size() == 2);
			for (const auto &value : {4, 2}) {
				REQUIRE(std::find_if(neighbours.begin(), neighbours.end(), [value](const auto &neighbour) {
					return neighbour->uid == value;
				}) != neighbours.end());
			}
			break;
		case 4:
			REQUIRE(neighbours.size() == 3);
			for (const auto &value : {2, 3, 5}) {
				REQUIRE(std::find_if(neighbours.begin(), neighbours.end(), [value](const auto &neighbour) {
					return neighbour->uid == value;
				}) != neighbours.end());
			}
			break;
		case 5:
			REQUIRE(neighbours.size() == 1);
			REQUIRE(neighbours.front()->uid == 4);
			break;
		default:
			REQUIRE(false);
			break;
		}
	}
}