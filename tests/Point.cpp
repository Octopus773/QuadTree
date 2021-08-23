//
// Created by cbihan on 22/08/2021.
//

#include "Point.hpp"
#include "Collisions.hpp"


namespace QuadTree::Tests
{

	Point::Point(double hPos, double vPos, std::string name)
		: _name(std::move(name)),
		  _uid(-1),
		  horizontalPos(hPos),
		  verticalPos(vPos)
	{
	}

	std::string Point::getName() const
	{
		return this->_name;
	}

	bool Point::collideRect(const std::array<double, 4> &rect)
	{
		return pointInRect(rect.data(), {this->horizontalPos, this->verticalPos});
	}

	Point::Point(double hPos, double vPos, int uid)
		: _name("Point"),
		  _uid(uid),
		  horizontalPos(hPos),
		  verticalPos(vPos)
	{
	}

	int Point::getUID() const
	{
		return this->_uid;
	}
}