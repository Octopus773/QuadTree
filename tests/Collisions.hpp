//
// Created by cbihan on 22/08/2021.
//

#pragma once


namespace QuadTree::Tests
{

	//! @brief Tells is a point is in a axis aligned rectangle
	//! @param rect The rectangle
	//! @param pointPos The point position (horizontal, vertical)
	//! @return True if the point is in the rect otherwise false
	[[nodiscard]] inline bool pointInRect(const double rect[4], std::pair<double, double> pointPos)
	{
		return pointPos.first >= rect[0] && pointPos.first <= rect[2]
		&& pointPos.second >= rect[1] && pointPos.second <= rect[3];
	}
}