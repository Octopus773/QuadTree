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
		bool topLeft;
		bool topRight;
		bool bottomLeft;
		bool bottomRight;
		std::array<bool, 4> points{false};


		auto rectanglePts = rectToArray(*this);

		points[0] = pointInRect({rect.minHorizontal, rect.minVertical, rect.maxHorizontal, rect.maxVertical},
		                        rectanglePts[0]);
		points[1] = pointInRect({rect.minHorizontal, rect.minVertical, rect.maxHorizontal, rect.maxVertical},
		                        rectanglePts[1]);
		points[2] = pointInRect({rect.minHorizontal, rect.minVertical, rect.maxHorizontal, rect.maxVertical},
		                        rectanglePts[3]);
		points[3] = pointInRect({rect.minHorizontal, rect.minVertical, rect.maxHorizontal, rect.maxVertical},
		                        rectanglePts[2]);

		int count = 0;
		for (const auto &point : points) {
			if (point) {
				count++;
			}
		}

		switch (count) {
		case 1:
			break;
		case 2:
			if ((points[0] && points[1])
			    || (points[2] && points[3])) {
				axis = 2;
			} else {
				axis = 1;
			}
			return true;
		default: return false;
		}
		std::pair<double, double> thisRectPoint;
		std::pair<double, double> otherRectOppositePoint;

		if (points[0]) {
			thisRectPoint = rectanglePts[0];
			otherRectOppositePoint = {rect.maxHorizontal, rect.maxVertical};
		} else if (points[1]) {
			thisRectPoint = rectanglePts[1];
			otherRectOppositePoint = rectToArray(rect)[3];
		} else if (points[2]) {
			thisRectPoint = rectanglePts[3];
			otherRectOppositePoint = rectToArray(rect)[1];
		} else {
			thisRectPoint = rectanglePts[2];
			otherRectOppositePoint = {rect.minHorizontal, rect.minVertical};
		}
		double width = std::abs(thisRectPoint.first - otherRectOppositePoint.first);
		double height = std::abs(thisRectPoint.second - otherRectOppositePoint.second);
		axis = width == std::max(width, height) ? 2 : 1;
		return true;

			bool AIsRightOfB = this->getLeft() > rect.getRight();
		bool AIsLeftOfB = this->getRight() < rect.getLeft();
		bool AIsAboveB = this->getBottom() < rect.getTop();
		bool AIsBelowB = this->getTop() > rect.getBottom();

		if (AIsRightOfB || AIsLeftOfB) {
			axis = 1;
		} else {
			axis = 2;
		}
		return !(
			AIsRightOfB
			|| AIsLeftOfB
			|| AIsAboveB
			|| AIsBelowB
		);
	}
}