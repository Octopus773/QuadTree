//
// Created by cbihan on 06/08/2021.
//

#include "APolygon.hpp"

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
}