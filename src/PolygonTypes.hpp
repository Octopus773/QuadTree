//
// Created by cbihan on 05/08/2021.
//

#pragma once

#include <vector>

namespace QuadTree
{
	class APolygon
	{
	private:
		unsigned int _uid = 0;
	public:

		[[nodiscard]] unsigned int getUID() const;

		void setUID(unsigned int uid);

		//! @brief Returns the points forming the polygon
		//! @return A vector of the points coordinates (horizontal, vertical) forming the polygon (coordinates are absolute)
		[[nodiscard]] virtual std::vector<std::pair<unsigned int, unsigned int>> getPoints() const = 0;

		//! @brief Allow us to pass by copy APolygon Classes
		[[nodiscard]] virtual APolygon *clone() const = 0;

		virtual ~APolygon() = default;
	};

	class Point : public APolygon
	{
	private:
		unsigned int _horizontalPos;
		unsigned int _verticalPos;
	public:

		//! @brief Returns the points forming the polygon
		//! @return A vector of the points coordinates (horizontal, vertical) forming the polygon (coordinates are absolute)
		[[nodiscard]] std::vector<std::pair<unsigned int, unsigned int>> getPoints() const override;

		//! @brief Allow us to pass by copy Point Classes
		[[nodiscard]] APolygon *clone() const override;

		Point(unsigned int hPos, unsigned int vPos);

		Point(const Point &) = delete;

		~Point() override = default;

		Point &operator=(const Point &) = delete;
	};
}