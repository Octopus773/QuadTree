//
// Created by cbihan on 03/08/2021.
//

#include <vector>
#include <iostream>
#include "World.hpp"
#include "PolygonTypes/Point.hpp"
#include "Collisions.hpp"

using namespace QuadTree::Collisions;
using namespace std;

int main(int ac, char **av)
{
	QuadTree::World world(10, 10);

	std::vector<QuadTree::Point *> points({
		                                      new QuadTree::Point{4, 4},
		                                      new QuadTree::Point{7, 3},
		                                      new QuadTree::Point{6.5, 6},
		                                      new QuadTree::Point{6, 6.5},
		                                      new QuadTree::Point{8, 7},
		                                      new QuadTree::Point{9, 9},
	                                      });

	for (const auto &point : points) {
		world.addPolygon(point);
	}

	std::vector<std::pair<double, double>> polygon1 = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};
	std::pair<double, double> p = {20, 20};
	isInside(polygon1, p) ? cout << "Yes \n": cout << "No \n";

	p = {5, 5};
	isInside(polygon1, p) ? cout << "Yes \n": cout << "No \n";

	vector<pair<double, double>> polygon2 = {{0, 0}, {5, 5}, {5, 0}};
	p = {3, 3};
	isInside(polygon2, p)? cout << "Yes \n": cout << "No \n";

	p = {5, 1};
	isInside(polygon2, p)? cout << "Yes \n": cout << "No \n";

	p = {8, 1};
	isInside(polygon2, p)? cout << "Yes \n": cout << "No \n";

	vector<pair<double, double>> polygon3 = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};
	p = {-1,10};
	isInside(polygon3, p)? cout << "Yes \n": cout << "No \n";

	return 0;
	std::cout << "end" << std::endl;

}