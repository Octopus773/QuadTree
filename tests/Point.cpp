//
// Created by cbihan on 22/08/2021.
//

#include "Point.hpp"
#include "Collisions.hpp"


namespace QuadTree::Tests
{

	Point::Point(double hPos, double vPos, std::string name)
		: _horizontalPos(hPos),
		  _verticalPos(vPos),
		  _name(std::move(name))
	{
	}

	std::string Point::getName() const
	{
		return this->_name;
	}

	bool Point::collideRect(const std::array<double, 4> &rect)
	{
		return pointInRect(rect.data(), {this->_horizontalPos, this->_verticalPos});
	}
}