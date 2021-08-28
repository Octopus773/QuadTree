//
// Created by cbihan on 28/08/2021.
//

#pragma once

#include <iostream>
#include <string>
#include <array>

namespace QuadTree::Tests
{

	//! @brief A structure to represent an axis aligned rectangle (AABB)
	struct Rect
	{
		//! @brief name of the rect
		std::string name = "Rect";

		//! @brief uid of the rect
		int uid = -1;

		//! @brief The minimum Vertical point
		double minVertical;
		//! @brief The minimum Horizontal Point
		double minHorizontal;
		//! @brief The maximum Horizontal Point
		double maxHorizontal;
		//! @brief The maximum Vertical Point
		double maxVertical;

		//! @brief 0 x 1 y
		std::pair<double, double> velocity = {(rand() % 3) + 1, (rand() % 3) + 1};

		bool collide(const Rect &rect, int &axis);

		//! @brief get the width of the rect
		[[nodiscard]] inline double getWidth() const
		{
			return this->maxHorizontal - this->minHorizontal;
		}

		//! @brief get the height of the rect
		[[nodiscard]] inline double getHeight() const
		{
			return this->maxVertical - this->minVertical;
		}

		//! @brief get the left point of the rect
		[[nodiscard]] inline double getLeft() const
		{
			return this->minHorizontal;
		}

		//! @brief get the left point of the rect
		[[nodiscard]] inline double getRight() const
		{
			return this->maxHorizontal;
		}


		//! @brief get the left point of the rect
		[[nodiscard]] inline double getBottom() const
		{
			return this->maxVertical;
		}


		//! @brief get the left point of the rect
		[[nodiscard]] inline double getTop() const
		{
			return this->minVertical;
		}


		//! @brief Tells if the point collideRect the rectangle
		[[nodiscard]] bool collideRect(const std::array<double, 4> &rect);

		Rect();

		Rect(double h, double w, double size);

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