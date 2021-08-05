//
// Created by cbihan on 05/08/2021.
//

#include "PopulationTypes.hpp"

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

	Point::Point(unsigned int hPos, unsigned int vPos)
		: _horizontalPos(hPos),
		  _verticalPos(vPos)
	{
	}

	std::vector<std::pair<unsigned int, unsigned int>> Point::getPoints() const
	{
		return {{this->_horizontalPos, this->_verticalPos}};
	}
}