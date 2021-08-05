//
// Created by cbihan on 06/08/2021.
//

#pragma once

#include <vector>
#include <string>

namespace QuadTree
{
	class APolygon
	{
	private:
		//! @brief The UID holder
		unsigned int _uid = 0;
	public:

		//! @brief Gives the current UID (used by the quadtree)
		[[nodiscard]] unsigned int getUID() const;

		//! @brief set the UID
		//! @warning You should not call this function unless you know what you're doing (UID is used for the Quadtree internally)
		void setUID(unsigned int uid);

		//! @brief Return the name of the polygon
		//! @note User defined identifier
		//!       It allow users to identify more easily what polygon they have after the getNeighbour call for the QuadTree
		[[nodiscard]] virtual std::string getName() const = 0;

		//! @brief Returns the points forming the polygon
		//! @return A vector of the points coordinates (horizontal, vertical) forming the polygon (coordinates are absolute)
		[[nodiscard]] virtual std::vector<std::pair<double, double>> getPoints() const = 0;

		//! @brief Allow us to pass by copy APolygon Classes
		[[nodiscard]] virtual APolygon *clone() const = 0;

		//! @brief A virtual dtor
		virtual ~APolygon() = default;
	};

};


