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


		//! @brief Tells if the point collide the rectangle
		[[nodiscard]] bool collide(const double rect[4]);

		Point(double hPos, double vPos, std::string name = "Point");

		Point(const Point &) = delete;


		~Point() = default;

		Point &operator=(const Point &) = delete;
	};
}