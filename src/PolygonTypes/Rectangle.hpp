//
// Created by cbihan on 18/08/2021.
//

#pragma once

#include <array>
#include "PolygonTypes/APolygon.hpp"

namespace QuadTree
{

	class Rectangle : public APolygon
	{
	public:
		//! @brief Containing points of the rectangle
		std::array<std::pair<double, double>, 4> points;

		//! @brief The name for user custom identification
		std::string name;

		//! @brief Return the name of the polygon
		//! @note User defined identifier
		//!       It allow users to identify more easily what polygon they have after the getNeighbour call for the QuadTree
		[[nodiscard]] std::string getName() const override;

		//! @brief Returns the points forming the polygon
		//! @return A vector of the points coordinates (horizontal, vertical) forming the polygon (coordinates are absolute)
		[[nodiscard]] std::vector<std::pair<double, double>> getPoints() const override;

		//! @brief Allow us to pass by copy APolygon Classes
		[[nodiscard]] APolygon *clone() const override;

		explicit Rectangle(std::array<std::pair<double, double>, 4> pts);
		Rectangle (const Rectangle &) = default;
		~Rectangle() override = default;
		Rectangle &operator=(const Rectangle &) = default;

	};
}


