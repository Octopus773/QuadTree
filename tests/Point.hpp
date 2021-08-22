//
// Created by cbihan on 22/08/2021.
//

#pragma once

#include <string>

#include "Collisions.hpp"


namespace QuadTree::Tests
{

	class Point
	{
	private:
		double _horizontalPos;
		double _verticalPos;
		std::string _name;
	public:

		[[nodiscard]] std::string getName() const;


		//! @brief Tells if the point collideRect the rectangle
		[[nodiscard]] bool collideRect(const std::array<double, 4> &rect);

		Point(double hPos, double vPos, std::string name = "Point");

		Point(const Point &) = delete;


		~Point() = default;

		Point &operator=(const Point &) = delete;
	};
}