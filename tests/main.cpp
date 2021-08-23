//
// Created by cbihan on 03/08/2021.
//

#include <vector>
#include <iostream>
#include <chrono>

#define private public

#include "QuadTree.hpp"
#include "Point.hpp"

using namespace std;

int main(int ac, char **av)
{
	QuadTree::QuadTree<QuadTree::Tests::Point> qT(0, 0, 10, 10);

	qT.maxElementsPerNode = 3;

	std::vector<std::shared_ptr<QuadTree::Tests::Point>> points;

	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(4, 4, 0));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(7, 3, 1));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6.5, 6, 2));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6, 6.5, 3));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(8, 7, 4));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(9, 9, 5));

	for (const auto &pt : points) {
		qT.add(pt);
	}

	qT.remove(points[3]);
	qT.remove(points[5]);

}