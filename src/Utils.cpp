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

	Rect getAABB(const APolygon *polygon)
	{
		auto points = polygon->getPoints();

		switch (points.size()) {
		case 0:
			//TODO throw an exception
			return {
				std::numeric_limits<double>::infinity(),
				std::numeric_limits<double>::infinity(),
				std::numeric_limits<double>::infinity(),
				std::numeric_limits<double>::infinity()
			};
		case 1:
			return {
				points.front().first,
				points.front().second,
				points.front().first,
				points.front().second
			};
		default:
			double maxHorizontal = 0;
			double maxVertical = 0;
			double minHorizontal = 0;
			double minVertical = 0;

			for (const auto &point : points) {
				maxHorizontal = std::max(point.first, maxHorizontal);
				minHorizontal = std::min(point.first, minHorizontal);
				maxVertical = std::max(point.second, maxVertical);
				minVertical = std::min(point.second, minVertical);
			}
			return {minHorizontal, minVertical, maxHorizontal, maxVertical};
		}
	}
}