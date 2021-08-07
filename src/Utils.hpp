//
// Created by cbihan on 07/08/2021.
//

#pragma once

#include <utility>
#include "Rect.hpp"
#include "PolygonTypes/APolygon.hpp"

namespace QuadTree::Utils
{

	//! @brief Tells is a point is in a rectangle
	//! @param rect The rectangle
	//! @param pointPos The point position (horizontal, vertical)
	//! @return True if the point is in the rect otherwise false
	[[nodiscard]] bool pointInRect(const Rect &rect, std::pair<double, double> pointPos);

	//! @brief This function give the Axis Aligned Bounding Box (AABB) for a given Polygon
	//! @param polygon The polygon
	//! @return The Axis Aligned Bounding Box (AABB) for the given polygon
	[[nodiscard]] Rect getAABB(const APolygon *polygon);

	//! @brief This function give the Axis Aligned Bounding Box (AABB) for a given Polygon
	//! @param polygonPoints All the points forming the polygon
	//! @return The Axis Aligned Bounding Box (AABB) for the given polygon
	[[nodiscard]] Rect getAABB(const std::vector<std::pair<double, double>> &polygonPoints);

}

