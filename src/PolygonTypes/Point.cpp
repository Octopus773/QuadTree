//
// Created by cbihan on 06/08/2021.
//

#include "Point.hpp"

namespace QuadTree
{
	Point::Point(double hPos, double vPos, std::string name)
		: _horizontalPos(hPos),
		  _verticalPos(vPos),
		  _name(std::move(name))
	{
	}

	std::vector<std::pair<double, double>> Point::getPoints() const
	{
		return {{this->_horizontalPos, this->_verticalPos}};
	}

	APolygon *Point::clone() const
	{
		return new Point(this->_horizontalPos, this->_verticalPos);
	}

	std::string Point::getName() const
	{
		return this->_name;
	}
}