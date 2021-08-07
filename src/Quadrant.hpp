//
// Created by cbihan on 06/08/2021.
//

#pragma once

#include <vector>
#include "Rect.hpp"

namespace QuadTree
{

	class Quadrant
	{
	public:
		//! @brief Child nodes
		//! @note child at index 0 is top left,
		//!                      1 is top right,
		//!                      2 is bottom left,
		//!                      3 is bottom right
		std::vector<Quadrant> children;
		//! @brief Contains the UUID of each element contained int this QuadNode
		//! @note Only leaves nodes contains UIDs
		std::vector<unsigned int> populationUIDs;
		//! @brief The area covered by the Quadrant
		Rect pos;

		Quadrant(double minHori,
		         double minVert,
		         double maxHori,
		         double maxVert);
	};

	inline bool operator==(const Quadrant &qn1, const Quadrant &qn2)
	{
		return qn1.pos == qn2.pos;
	}
}