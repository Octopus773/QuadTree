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

		//! @brief Value of the axis param for collide functions
		static constexpr int HorizontalAxis = 1;
		//! @brief Value of the axis param for collide functions
		static constexpr int VerticalAxis = 2;

		//! @brief name of the rect
		std::string name = "Rect";

		//! @brief uid of the rect
		int uid = -1;

		bool isCollided = false;

		//! @note first x second y
		std::array<std::pair<double, double> , 2> points;

	/*	//! @brief The minimum Vertical point
		double minVertical;
		//! @brief The minimum Horizontal Point
		double minHorizontal;
		//! @brief The maximum Horizontal Point
		double maxHorizontal;
		//! @brief The maximum Vertical Point
		double maxVertical;
*/
		//! @brief 0 x 1 y
		std::pair<double, double> velocity = {(rand() % 3) + 1, (rand() % 3) + 1};
		//std::pair<double, double> velocity = {1, 0};

		//! @brief Tells if the rect is colliding and the axis will be set to 1 if it's the x axis and 2 if it's y axis
		bool collide(const Rect &rect, int &axis);

		static bool collide(const Rect &rect1, const Rect &rect2, int &axis);

		//! @brief get the width of the rect
		[[nodiscard]] inline double getWidth() const
		{
			return this->points[1].first - this->points[0].first;
		}

		//! @brief get the height of the rect
		[[nodiscard]] inline double getHeight() const
		{
			return this->points[1].second - this->points[0].second;
		}

		//! @brief get the left point of the rect
		[[nodiscard]] inline double getLeft() const
		{
			return this->points[0].first;
		}

		//! @brief get the left point of the rect
		[[nodiscard]] inline double getRight() const
		{
			return this->points[1].first;
		}


		//! @brief get the left point of the rect
		[[nodiscard]] inline double getBottom() const
		{
			return this->points[1].second;
		}


		//! @brief get the left point of the rect
		[[nodiscard]] inline double getTop() const
		{
			return this->points[0].second;
		}


		//! @brief Tells if the point collideRect the rectangle
		[[nodiscard]] bool collideRect(const std::array<double, 4> &rect);

		Rect();

		Rect(double h, double w, double size, int UID = 0);

		Rect(double minHori,
		     double minVert,
		     double maxHori,
		     double maxVert);
	};

	inline bool operator==(const Rect &rect1, const Rect &rect2)
	{
		return rect1.points == rect2.points;
	}

	// for debug purpuses (expansion of custom structs with catch2 lib)
	std::ostream &operator<<(std::ostream &os, Rect const &rect);

	//! @brief gives an array of the 4 rectangle points from a rect
	inline std::array<std::pair<double, double>, 4> rectToArray(const Rect &rect)
	{
		return {{
			        rect.points[0],
			        {rect.points[0].first + rect.getWidth(), rect.points[0].second},
			        rect.points[1],
			        {rect.points[0].first, rect.points[0].second + rect.getHeight()}
		        }};
	}

	//! @brief gives an array of the 4 rectangle points from a rect
	inline std::array<std::pair<double, double>, 4> rectToArray(const std::array<double, 4> &rect)
	{
		return {{
			        {rect[0], rect[1]},
			        {rect[2], rect[1]},
			        {rect[2], rect[3]},
			        {rect[0], rect[3]}
		        }};
	}


}