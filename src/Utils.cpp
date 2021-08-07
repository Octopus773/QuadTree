//
// Created by cbihan on 07/08/2021.
//

#include "Utils.hpp"

namespace QuadTree::Utils
{

	bool pointInRect(const Rect &rect, std::pair<double, double> pointPos)
	{
		return pointPos.first >= rect.minHorizontal && pointPos.first <= rect.maxHorizontal
		       && pointPos.second >= rect.minVertical && pointPos.second <= rect.maxVertical;
	}
}