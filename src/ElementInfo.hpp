//
// Created by cbihan on 06/08/2021.
//

#pragma once

#include <functional>
#include <vector>
#include "PolygonTypes/APolygon.hpp"
#include "Rect.hpp"
#include "Quadrant.hpp"

namespace QuadTree
{

	struct ElementInfo
	{
		//! @brief The polygon
		APolygon *polygon;
		//! @brief a reference to each leaf node containing it's UID
		std::vector<std::reference_wrapper<Quadrant>> references;
		//! @brief The Axis aligned bounding box for the polygon
		Rect aabb;


		explicit ElementInfo(APolygon *pol);
		explicit ElementInfo();
		explicit ElementInfo(APolygon *polygon, Rect aabb);

		ElementInfo(const ElementInfo &) = default;
		~ElementInfo() = default;
		ElementInfo &operator=(const ElementInfo &) = default;
	};


}