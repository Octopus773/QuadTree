//
// Created by cbihan on 03/08/2021.
//

#include <vector>
#include <iostream>
#include "World.hpp"
#include "PolygonTypes.hpp"

int main(int ac, char **av)
{
	QuadTree::World world(20, 20);

	std::vector<QuadTree::Point *> points({
		                                      new QuadTree::Point{5, 5},
		                                      new QuadTree::Point{3, 3},
		                                      new QuadTree::Point{3, 3},
		                                      new QuadTree::Point{4, 3}
	                                      });

	for (const auto &point : points) {
		world.addPolygon(point);
	}

	std::cout << "end" << std::endl;

}