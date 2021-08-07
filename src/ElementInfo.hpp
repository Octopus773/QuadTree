//
// Created by cbihan on 06/08/2021.
//

#pragma once

#include <functional>
#include <vector>
#include "PolygonTypes/APolygon.hpp"
#include "Quadrant.hpp"

namespace QuadTree
{

	struct ElementInfo
	{
		APolygon *polygon;

		std::vector<std::reference_wrapper<Quadrant>> references;

		explicit ElementInfo(APolygon *pol);
		explicit ElementInfo();
		ElementInfo(const ElementInfo &) = default;
		~ElementInfo() = default;
		ElementInfo &operator=(const ElementInfo &) = default;
	};


}