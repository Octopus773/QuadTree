//
// Created by cbihan on 07/08/2021.
//

#include "Rect.hpp"

namespace QuadTree
{
	Rect::Rect(double minHori, double minVert, double maxHori, double maxVert)
		: minVertical(minVert),
		  minHorizontal(minHori),
		  maxHorizontal(maxHori),
		  maxVertical(maxVert)
	{
	}

	double Rect::getWidth() const
	{
		return this->maxHorizontal - this->minHorizontal;
	}

	double Rect::getHeight() const
	{
		return this->maxVertical - this->minVertical;
	}
}