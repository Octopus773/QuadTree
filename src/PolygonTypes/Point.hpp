//
// Created by cbihan on 06/08/2021.
//

#pragma once

#include <string>
#include <vector>
#include "PolygonTypes/APolygon.hpp"

namespace QuadTree
{
	class Point : public APolygon
		{
		private:
			double _horizontalPos;
			double _verticalPos;
			std::string _name;
		public:

			//! @brief Returns the points forming the polygon
			//! @return A vector of the points coordinates (horizontal, vertical) forming the polygon (coordinates are absolute)
			[[nodiscard]] std::vector<std::pair<double, double>> getPoints() const override;

			//! @brief Allow us to pass by copy Point Classes
			[[nodiscard]] APolygon *clone() const override;

			[[nodiscard]] std::string getName() const override;

			Point(double hPos, double vPos, std::string name = "Point");

			Point(const Point &) = delete;

			~Point() override = default;

			Point &operator=(const Point &) = delete;
		};
};


