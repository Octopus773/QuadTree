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
		return getAABB(polygon->getPoints());
	}

	Rect getAABB(const std::vector<std::pair<double, double>> &polygonPoints)
	{
		switch (polygonPoints.size()) {
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
				polygonPoints.front().first,
				polygonPoints.front().second,
				polygonPoints.front().first,
				polygonPoints.front().second
			};
		default:
			double maxHorizontal = -std::numeric_limits<double>::infinity();
			double maxVertical = -std::numeric_limits<double>::infinity();
			double minHorizontal = std::numeric_limits<double>::infinity();
			double minVertical = std::numeric_limits<double>::infinity();

			for (const auto &point : polygonPoints) {
				maxHorizontal = std::max(point.first, maxHorizontal);
				minHorizontal = std::min(point.first, minHorizontal);
				maxVertical = std::max(point.second, maxVertical);
				minVertical = std::min(point.second, minVertical);
			}
			return {minHorizontal, minVertical, maxHorizontal, maxVertical};
		}
	}
}