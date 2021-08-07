//
// Created by cbihan on 06/08/2021.
//

#include "Quadrant.hpp"

namespace QuadTree
{

	Quadrant::Quadrant(double minHori, double minVert, double maxHori, double maxVert)
		: pos(minHori, minVert, maxHori, maxVert)
	{
	}
}