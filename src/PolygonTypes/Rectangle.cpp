//
// Created by cbihan on 18/08/2021.
//

#include "Rectangle.hpp"

#include <utility>

namespace QuadTree
{

	std::string Rectangle::getName() const
	{
		return this->name;
	}

	std::vector<std::pair<double, double>> Rectangle::getPoints() const
	{
		return {this->points.begin(), this->points.end()};
	}

	APolygon *Rectangle::clone() const
	{
		return new Rectangle(*this);
	}

	Rectangle::Rectangle(std::array<std::pair<double, double>, 4> pts, std::string rectName)
		: points(std::move(pts)),
		  name(std::move(rectName))
	{
	}

	void Rectangle::moveTo(std::pair<double, double> newMin)
	{
		for (auto &point : points) {
			point.second += newMin.second;
			point.first += newMin.first;
		}
	}
}