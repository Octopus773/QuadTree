//
// Created by cbihan on 06/08/2021.
//

#include "ElementInfo.hpp"
#include "Utils.hpp"

namespace QuadTree
{

	ElementInfo::ElementInfo(APolygon *pol)
		: polygon(pol),
		  aabb(Utils::getAABB(pol))
	{
	}

	ElementInfo::ElementInfo()
		: polygon(nullptr)
	{
	}

	ElementInfo::ElementInfo(APolygon *pol, Rect boundingBox)
		: polygon(pol),
		  aabb(boundingBox)
	{
	}
}