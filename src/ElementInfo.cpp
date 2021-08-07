//
// Created by cbihan on 06/08/2021.
//

#include "ElementInfo.hpp"

namespace QuadTree
{

	ElementInfo::ElementInfo(APolygon *pol)
		: polygon(pol)
	{
	}

	ElementInfo::ElementInfo()
		: polygon(nullptr)
	{
	}
}