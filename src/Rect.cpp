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

	Rect::Rect()
		: minVertical(0),
		  minHorizontal(0),
		  maxHorizontal(0),
		  maxVertical(0)
	{
	}

	std::ostream &operator<<(std::ostream &os, Rect const &rect)
	{
		os << '{' << rect.minHorizontal << ", " << rect.minVertical << "}, {" << rect.maxHorizontal << ", "
		<< rect.maxVertical << "}";
		return os;
	}
}