//
// Created by cbihan on 07/08/2021.
//

#pragma once

#include <utility>
#include "Rect.hpp"
#include "PolygonTypes/APolygon.hpp"

namespace QuadTree::Utils
{

	//! @brief This function give the Axis Aligned Bounding Box (AABB) for a given Polygon
	//! @param polygon The polygon
	//! @return The Axis Aligned Bounding Box (AABB) for the given polygon
	[[nodiscard]] Rect getAABB(const APolygon *polygon);

	//! @brief This function give the Axis Aligned Bounding Box (AABB) for a given Polygon
	//! @param polygonPoints All the points forming the polygon
	//! @return The Axis Aligned Bounding Box (AABB) for the given polygon
	[[nodiscard]] Rect getAABB(const std::vector<std::pair<double, double>> &polygonPoints);

}

