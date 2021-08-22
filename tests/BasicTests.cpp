//
// Created by cbihan on 05/08/2021.
//

#include <catch2/catch.hpp>

#define private public

#include "Point.hpp"
#include "QuadTree.hpp"


TEST_CASE("QuadTree Basic Use", "[QuadTree]")
{
	QuadTree::QuadTree<QuadTree::Tests::Point> qT(0, 0, 10, 10);
}
