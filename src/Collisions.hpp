//
// Created by cbihan on 18/08/2021.
//

#pragma once

#include "ElementInfo.hpp"
#include "Rect.hpp"

namespace QuadTree::Collisions
{
	//! @brief Tells is a polygon overlaps a rectangle
	[[nodiscard]] bool isOverlapping(const ElementInfo &polygonInfo, const Rect &rectangle);

	//! @brief Tells is a convex polygon overlaps another convex polygon
	[[nodiscard]] inline bool isOverlappingConvex(const ElementInfo &polygonInfo1, const ElementInfo &polygonInfo2)
	{
		return std::any_of(polygonInfo1.polygon->getPoints().begin(), polygonInfo1.polygon->getPoints().end(),
		                   [](std::pair<double, double> point) {
			                   return true;
		                   });
	}

	//! @brief Tells is a rect and an another rect are overlapping
	[[nodiscard]] inline bool isOverlapping(const Rect &rect1, const Rect &rect2)
	{
		return rect1.minHorizontal > rect2.maxHorizontal || rect1.minVertical > rect2.maxVertical
		       || rect1.maxHorizontal < rect2.minHorizontal || rect1.maxVertical < rect2.minVertical;
	}

	//! @brief Tells is a point is in a axis aligned rectangle
	//! @param rect The rectangle
	//! @param pointPos The point position (horizontal, vertical)
	//! @return True if the point is in the rect otherwise false
	[[nodiscard]] inline bool pointInRect(const Rect &rect, std::pair<double, double> pointPos)
	{
		return pointPos.first >= rect.minHorizontal && pointPos.first <= rect.maxHorizontal
		       && pointPos.second >= rect.minVertical && pointPos.second <= rect.maxVertical;
	}

	//! @brief Given three collinear points p, q, r, the function checks if point q lies on line segment 'pr'
	//! @warning This function only works for collinear points !
	[[nodiscard]] inline bool
	onSegment(std::pair<double, double> p, std::pair<double, double> q, std::pair<double, double> r)
	{
		return q.first <= std::max(p.first, r.first) && q.first >= std::min(p.first, r.first) &&
		       q.second <= std::max(p.second, r.second) && q.second >= std::min(p.second, r.second);
	}

	// To find orientation of ordered triplet (p, q, r).
	// The function returns following values
	// 0 --> p, q and r are colinear
	// 1 --> Clockwise
	// 2 --> Counterclockwise
	[[nodiscard]] inline int
	orientation(std::pair<double, double> p, std::pair<double, double> q, std::pair<double, double> r)
	{
		int val = (q.second - p.second) * (r.first - q.first) -
		          (q.first - p.first) * (r.second - q.second);

		if (val == 0) return 0; // colinear
		return (val > 0) ? 1 : 2; // clock or counterclock wise
	}

	// The function that returns true if line segment 'p1q1'
	// and 'p2q2' intersect.
	bool doIntersect(std::pair<double, double> p1, std::pair<double, double> q1, std::pair<double, double> p2,
	                 std::pair<double, double> q2);

	// Returns true if the point p lies inside the polygon[] with n vertices
	bool isInside(const std::vector<std::pair<double, double>> &points, std::pair<double, double> p);

}