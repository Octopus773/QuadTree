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

	rects.emplace_back(std::make_shared<QuadTree::Tests::Rect>(10, 0, 10, 0));
	rects.emplace_back(std::make_shared<QuadTree::Tests::Rect>(10, 12, 10, 1));
	rects.emplace_back(std::make_shared<QuadTree::Tests::Rect>(10, 24, 10, 2));
	rects.emplace_back(std::make_shared<QuadTree::Tests::Rect>(10, 36, 10, 3));
	rects.emplace_back(std::make_shared<QuadTree::Tests::Rect>(10, 48, 10, 4));
	rects.emplace_back(std::make_shared<QuadTree::Tests::Rect>(10, 60, 10, 5));

	for (auto &rect : rects) {
		qT.add(rect);
	}

	//REQUIRE(QuadTree::Tests::getSizeNodes(qT) == 9);
}