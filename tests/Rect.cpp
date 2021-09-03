//
// Created by cbihan on 28/08/2021.
//

#include "Rect.hpp"
#include "Collisions.hpp"

namespace QuadTree::Tests
{
	Rect::Rect(double minHori, double minVert, double maxHori, double maxVert)
		: points({{
			          {minHori, minVert},
			          {maxHori, maxVert},
		          }})
	{
	}

	Rect::Rect()
		: points({{
			          {0, 0},
			          {0, 0},
		          }})
	{
	}

	Rect::Rect(double h, double w, double size, int UID)
		: uid(UID),
		  points({{
			          {w, h},
			          {w + size, h + size},
		          }})
	{
	}

	std::ostream &operator<<(std::ostream &os, Rect const &rect)
	{
		os << '{' << rect.points[0].first << ", " << rect.points[0].second << "}, {" << rect.points[1].first << ", "
		   << rect.points[1].second << "}";
		return os;
	}

	bool Rect::collideRect(const std::array<double, 4> &rect)
	{
		std::array<std::pair<double, double>, 4> pts{{
			                                             {this->points[0].first, this->points[0].second},
			                                             {this->points[0].first,
			                                              this->points[0].second + this->getHeight()},
			                                             {this->points[0].first + this->getWidth(),
			                                              this->points[0].second},
			                                             {this->points[1].first, this->points[1].second}
		                                             }};
		return std::all_of(pts.begin(), pts.end(), [&rect = std::as_const(rect)](auto pt) {
			return pointInRect(rect.data(), pt);
		});
	}

	bool Rect::collide(const Rect &rect1, const Rect &rect2, int &axis)
	{
		std::array<bool, 4> areCollidingPoints{false};

		auto rectanglePts = rectToArray(rect1);

		areCollidingPoints[0] = pointInRect(rect2.points, rectanglePts[0]);
		areCollidingPoints[1] = pointInRect(rect2.points, rectanglePts[1]);
		areCollidingPoints[2] = pointInRect(rect2.points, rectanglePts[3]);
		areCollidingPoints[3] = pointInRect(rect2.points, rectanglePts[2]);

		int count = 0;
		for (const auto &point: areCollidingPoints) {
			if (point) {
				count++;
			}
		}

		switch (count) {
		case 1: break;
		case 2:
			if ((areCollidingPoints[0] && areCollidingPoints[1])
			    || (areCollidingPoints[2] && areCollidingPoints[3])) {
				axis = 2;
			} else {
				axis = 1;
			}
			return true;
		default: return false;
		}
		std::pair<double, double> thisRectPoint;
		std::pair<double, double> otherRectOppositePoint;

		if (areCollidingPoints[0]) {
			thisRectPoint = rectanglePts[0];
			otherRectOppositePoint = {rect2.getRight(), rect2.getBottom()};
		} else if (areCollidingPoints[1]) {
			thisRectPoint = rectanglePts[1];
			otherRectOppositePoint = rectToArray(rect2)[3];
		} else if (areCollidingPoints[2]) {
			thisRectPoint = rectanglePts[3];
			otherRectOppositePoint = rectToArray(rect2)[1];
		} else {
			thisRectPoint = rectanglePts[2];
			otherRectOppositePoint = {rect2.getLeft(), rect2.getTop()};
		}
		double width = std::abs(thisRectPoint.first - otherRectOppositePoint.first);
		double height = std::abs(thisRectPoint.second - otherRectOppositePoint.second);
		axis = width == std::max(width, height) ? 2 : 1;
		return true;
	}

	bool Rect::collide(const Rect &rect, int &axis)
	{
		return this->collide(*this, rect, axis) || this->collide(rect, *this, axis);
	}
}