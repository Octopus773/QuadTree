//
// Created by cbihan on 22/08/2021.
//

#pragma once

#include <array>
#include "QuadTree.hpp"

namespace QuadTree::Tests
{

	//! @brief Tells is a point is in a axis aligned rectangle
	//! @param rect The rectangle
	//! @param pointPos The point position (horizontal, vertical)
	//! @return True if the point is in the rect otherwise false
	[[nodiscard]] inline bool pointInRect(const double rect[4], const std::pair<double, double> &pointPos)
	{
		return pointPos.first >= rect[0] && pointPos.first <= rect[2]
		       && pointPos.second >= rect[1] && pointPos.second <= rect[3];
	}

	//! @brief Tells is a point is in a axis aligned rectangle
	//! @param rect The rectangle
	//! @param pointPos The point position (horizontal, vertical)
	//! @return True if the point is in the rect otherwise false
	[[nodiscard]] inline bool pointInRect(const std::array<double, 4> &rect, const std::pair<double, double> &pointPos)
	{
		return pointPos.first >= rect[0] && pointPos.first <= rect[2]
		       && pointPos.second >= rect[1] && pointPos.second <= rect[3];
	}

	//! @brief Tells is a point is in a axis aligned rectangle
	//! @param rect The rectangle
	//! @param pointPos The point position (horizontal, vertical)
	//! @return True if the point is in the rect otherwise false
	[[nodiscard]] inline bool
	pointInRect(const std::array<std::pair<double, double>, 2> &rect, const std::pair<double, double> &pointPos)
	{
		return pointInRect({
			                   rect[0].first,
			                   rect[0].second,
			                   rect[1].first,
			                   rect[1].second
		                   }, pointPos);
	}
}