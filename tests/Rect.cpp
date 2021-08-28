//
// Created by cbihan on 28/08/2021.
//

#include "Rect.hpp"
#include "Collisions.hpp"

namespace QuadTree::Tests
{
	Rect::Rect(double minHori, double minVert, double maxHori, double maxVert)
		: minVertical(minVert),
		  minHorizontal(minHori),
		  maxHorizontal(maxHori),
		  maxVertical(maxVert)
	{
	}

	Rect::Rect()
		: minVertical(0),
		  minHorizontal(0),
		  maxHorizontal(0),
		  maxVertical(0)
	{
	}

	Rect::Rect(double h, double w, double size)
		: minVertical(w),
		  minHorizontal(h),
		  maxHorizontal(h + size),
		  maxVertical(w + size)
	{
	}

	std::ostream &operator<<(std::ostream &os, Rect const &rect)
	{
		os << '{' << rect.minHorizontal << ", " << rect.minVertical << "}, {" << rect.maxHorizontal << ", "
		   << rect.maxVertical << "}";
		return os;
	}

	bool Rect::collideRect(const std::array<double, 4> &rect)
	{
		std::array<std::pair<double, double>, 4> pts{{
			                                             {this->minHorizontal, this->minVertical},
			                                             {this->minHorizontal, this->minVertical + this->getHeight()},
			                                             {this->minHorizontal + this->getWidth(), this->minVertical},
			                                             {this->maxHorizontal, this->maxVertical}
		                                             }};
		return std::all_of(pts.begin(), pts.end(), [&rect = std::as_const(rect)](auto pt) {
			return pointInRect(rect.data(), pt);
		});
	}

	bool Rect::collide(const Rect &rect, int &axis)
	{
		bool ARightOfB = this->getLeft() > rect.getRight();
		bool AIsLeftOfB = this->getRight() < rect.getLeft();
		bool AIsAboveB = this->getBottom() < rect.getTop();
		bool AIsBelowB = this->getTop() > rect.getBottom();

		if (ARightOfB || AIsLeftOfB) {
			axis = 1;
		} else {
			axis = 2;
		}
		return !(
			ARightOfB
			|| AIsLeftOfB
			|| AIsAboveB
			|| AIsBelowB
		);
	}
}