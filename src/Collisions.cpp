//
// Created by cbihan on 18/08/2021.
//

#include "Collisions.hpp"


namespace QuadTree::Collisions
{
	bool isOverlapping(const ElementInfo &polygonInfo, const Rect &rectangle)
	{
		if (polygonInfo.polygon->getPoints().size() == 1) {
			return pointInRect(rectangle, polygonInfo.polygon->getPoints()[0]);
		}
		return isOverlapping(polygonInfo.aabb, rectangle);
	}

	bool isInside(const std::vector<std::pair<double, double>> &points, std::pair<double, double> p)
	{
		// There must be at least 3 vertices in polygon[]
		if (points.size() < 3) return false;

		// Create a point for line segment from p to infinite
		std::pair<double, double> extreme = {10000, p.second};

		// Count intersections of the above line with sides of polygon
		int count = 0;
		for (auto i = points.begin(); i != points.end(); i++) {
			if (doIntersect(*i, *(i + 1), p, extreme)) {
				// If the point 'p' is colinear with line segment 'i-next',
				// then check if it lies on segment. If it lies, return true,
				// otherwise false
				if (orientation(*i, p, *(i + 1)) == 0)
					return onSegment(*i, p, *(i + 1));

				count++;
			}
		}
		// Return true if count is odd, false otherwise
		return count & 1;
	}

	bool doIntersect(std::pair<double, double> p1, std::pair<double, double> q1, std::pair<double, double> p2,
	                 std::pair<double, double> q2)
	{
		// Find the four orientations needed for general and
		// special cases
		int o1 = orientation(p1, q1, p2);
		int o2 = orientation(p1, q1, q2);
		int o3 = orientation(p2, q2, p1);
		int o4 = orientation(p2, q2, q1);

		// General case
		if (o1 != o2 && o3 != o4)
			return true;

		// Special Cases
		// p1, q1 and p2 are colinear and p2 lies on segment p1q1
		if (o1 == 0 && onSegment(p1, p2, q1)) return true;

		// p1, q1 and p2 are colinear and q2 lies on segment p1q1
		if (o2 == 0 && onSegment(p1, q2, q1)) return true;

		// p2, q2 and p1 are colinear and p1 lies on segment p2q2
		if (o3 == 0 && onSegment(p2, p1, q2)) return true;

		// p2, q2 and q1 are colinear and q1 lies on segment p2q2
		if (o4 == 0 && onSegment(p2, q1, q2)) return true;

		return false; // Doesn't fall in any of the above cases
	}

}