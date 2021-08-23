//
// Created by cbihan on 22/08/2021.
//

#pragma once

#include <string>

#include "Collisions.hpp"


namespace QuadTree::Tests
{

	//! @brief Point class used to test the quadtree
	class Point
	{
	private:

		std::string _name;
		int _uid;
	public:

		double horizontalPos;
		double verticalPos;

		[[nodiscard]] std::string getName() const;

		[[nodiscard]] int getUID() const;


		//! @brief Tells if the point collideRect the rectangle
		[[nodiscard]] bool collideRect(const std::array<double, 4> &rect);

		Point(double hPos, double vPos, std::string name = "Point");

		Point(double hPos, double vPos, int uid);

		Point(const Point &) = delete;


		~Point() = default;

		Point &operator=(const Point &) = delete;
	};
}