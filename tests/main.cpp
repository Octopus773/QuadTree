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
		                                      new QuadTree::Point{6, 6.5},
		                                      new QuadTree::Point{8, 7},
		                                      new QuadTree::Point{9, 9},
	                                      });

	for (const auto &point : points) {
		world.addPolygon(point);
	}



	(world.getNeighbours(5).empty());

	(world.getNeighbours(2).size() == 1);
	(*world.getNeighbours(2).begin() == points[3]);
	std::cout << "end" << std::endl;

}