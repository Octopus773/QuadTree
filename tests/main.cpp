//
// Created by cbihan on 03/08/2021.
//

#include <vector>
#include <iostream>
#include "World.hpp"
#include "PolygonTypes.hpp"

int main(int ac, char **av)
{
	QuadTree::World world(10, 10);

	std::vector<QuadTree::Point *> points({
		                                      new QuadTree::Point{4, 4},
		                                      new QuadTree::Point{7, 3},
		                                      new QuadTree::Point{6.5, 6},
		                                      new QuadTree::Point{6, 6.5}
	                                      });

	for (const auto &point : points) {
		world.addPolygon(point);
	}

	std::cout << "end" << std::endl;

}