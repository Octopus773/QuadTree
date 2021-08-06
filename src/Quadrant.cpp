//
// Created by cbihan on 06/08/2021.
//

#include "Quadrant.hpp"

namespace QuadTree
{
	Quadrant::Quadrant(double w, double h, double oH,
	                   double oV)
		: width(w),
		  height(h),
		  originHorizontal(oH),
		  originVertical(oV)
	{
	}
}