//
// Created by cbihan on 05/08/2021.
//

#include "PolygonTypes.hpp"

namespace QuadTree
{

	unsigned int APolygon::getUID() const
	{
		return this->_uid;
	}

	void APolygon::setUID(unsigned int uid)
	{
		this->_uid = uid;
	}

	Point::Point(double hPos, double vPos)
		: _horizontalPos(hPos),
		  _verticalPos(vPos)
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
}