//
// Created by cbihan on 07/08/2021.
//

#pragma once

#include <iostream>


namespace QuadTree
{

	//! @brief A structure to represent an axis aligned rectangle
	struct Rect
	{
		//! @brief The minimum Vertical point
		double minVertical;
		//! @brief The minimum Horizontal Point
		double minHorizontal;
		//! @brief The maximum Horizontal Point
		double maxHorizontal;
		//! @brief The maximum Vertical Point
		double maxVertical;

		[[nodiscard]] double getWidth() const;

		[[nodiscard]] double getHeight() const;

		Rect();

		Rect(double minHori,
		     double minVert,
		     double maxHori,
		     double maxVert);
	};

	inline bool operator==(const Rect &rect1, const Rect &rect2)
	{
		return rect1.maxVertical == rect2.maxVertical
		       && rect1.maxHorizontal == rect2.maxHorizontal
		       && rect1.minVertical == rect2.minVertical
		       && rect1.minHorizontal == rect2.minHorizontal;
	}

	// for debug purpuses (expansion of custom structs with catch2 lib)
	std::ostream &operator<<(std::ostream &os, Rect const &rect);


}