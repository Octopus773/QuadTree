//
// Created by cbihan on 07/08/2021.
//

#pragma once

#include <utility>
#include "Rect.hpp"

namespace QuadTree::Utils
{

	//! @brief Tells is a point is in a rectangle
	//! @param rect The rectangle
	//! @param pointPos The point position (horizontal, vertical)
	//! @return True if the point is in the rect otherwise false
	[[nodiscard]] bool pointInRect(const Rect &rect, std::pair<double, double> pointPos);

}

