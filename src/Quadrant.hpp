//
// Created by cbihan on 06/08/2021.
//

#pragma once

#include <vector>

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

		//! @brief The width of the node
		double width;
		//! @brief The height of the node
		double height;
		//! @brief The base horizontal coordinate (top left corner)
		double originHorizontal;
		//! @brief Te base vertical coordinate (top left corner)
		double originVertical;

		Quadrant(double w,
		         double h,
		         double oH,
		         double oV);
	};

	inline bool operator==(const Quadrant &qn1, const Quadrant &qn2)
	{
		return qn1.width == qn2.width
		&& qn1.height == qn2.height
		&& qn1.originVertical == qn2.originVertical
		&& qn1.originHorizontal == qn2.originHorizontal;
	}
}