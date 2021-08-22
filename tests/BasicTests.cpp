//
// Created by cbihan on 05/08/2021.
//

#include <catch2/catch.hpp>

#define private public

#include "Point.hpp"
#include "QuadTree.hpp"


TEST_CASE("QuadTree Basic Use 10x10", "[QuadTree]")
{
	QuadTree::QuadTree<QuadTree::Tests::Point> qT(0, 0, 10, 10);

	qT.maxElementsPerNode = 3;

	std::vector<std::shared_ptr<QuadTree::Tests::Point>> points;

	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(4, 4, "0"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(7, 3, "1"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6.5, 6, "2"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6, 6.5, "3"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(8, 7, "4"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(9, 9, "5"));

	for (const auto &pt : points) {
		qT.add(pt);
	}

	for (const auto &pt : points) {
		auto neighbours = qT.getNeighbours(pt);

		if (pt->getName() == "0") {
			CHECK(neighbours.empty());
		} else if (pt->getName() == "1") {
			CHECK(neighbours.empty());
		} else if (pt->getName() == "2") {
			CHECK(neighbours.size() == 1);
			CHECK(neighbours[0]->getName() == "3");
		} else if (pt->getName() == "3") {
			CHECK(neighbours.size() == 1);
			CHECK(neighbours[0]->getName() == "2");
		} else if (pt->getName() == "4") {
			CHECK(neighbours.empty());
		} else if (pt->getName() == "5") {
			CHECK(neighbours.empty());
		}
	}
}

TEST_CASE("QuadTree Basic Use 510x510", "[QuadTree]")
{
	// dataset realized with
	// https://editor.p5js.org/Aslatas/sketches/bQwvi43Uf

	QuadTree::QuadTree<QuadTree::Tests::Point> qT(0, 0, 510, 510);

	qT.maxElementsPerNode = 4;

	std::vector<std::shared_ptr<QuadTree::Tests::Point>> points;

	//node name: 5
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(17.587791774952002, 95.93226289306212, "2"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(86.44172050933604, 1.9014910363196855, "22"));
	//node name: 6
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(182.7592770117985, 25.670905073871157, "3"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(243.6619951772354, 68.1650848511016, "15"));
	//node name: 7
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(50.8155727453303, 213.90748727490248, "7"));
	//node name: 8
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(190.54758139288384, 170.04255970715494, "14"));
	//node name: 13
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(357.5502623387069, 9.812098568075342, "4"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(361.5578227878114, 43.73370746298695, "18"));
	//node name: 14
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(438.5406048375477, 112.52959453775483, "16"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(390.31658295518815, 13.575343724123293, "19"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(419.1764369605108, 34.775507023212135, "27"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(424.03307536587937, 85.4499760955955, "28"));
	//node name: 15
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(265.9518772017447, 223.84625820210667, "11"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(279.21063259461755, 215.1177466015272, "12"));
	//node name: 16
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(463.3793267511266, 201.60987573235937, "20"));
	//node name: 17
	//node name: 21
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(140.97591539674264, 292.56875240437387, "21"));
	//node name: 22
	//node name: 23
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(150.40901829555656, 345.46205549112733, "0"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(170.92504306870342, 323.0685427595462, "25"));
	//node name: 24
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(194.21247306333086, 376.2844470618495, "5"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(204.46590948258952, 369.00173545475093, "26"));
	//node name: 19
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(101.52272893442452, 410.28761448299093, "23"));
	//node name: 20
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(147.76200732161158, 507.34858928203084, "9"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(144.52997918875272, 443.6795010217856, "13"));
	//node name: 9
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(345.26990553691786, 264.0505688799703, "1"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(302.2963430534768, 317.77778286415247, "8"));
	//node name: 10
	//node name: 11
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(298.50638999235247, 461.0012159917385, "6"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(355.29217137903663, 446.170498001138, "17"));
	//node name: 12
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(428.70133789213617, 503.32618496430536, "10"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(465.4831033338977, 454.1612878006692, "24"));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(391.4714586035678, 413.05645590410654, "29"));
	for (const auto &pt : points) {
		qT.add(pt);
	}

	for (const auto &pt : points) {
		auto neighbours = qT.getNeighbours(pt);

		if (pt->getName() == "0") {
			CHECK(neighbours.empty());
		} else if (pt->getName() == "1") {
			CHECK(neighbours.empty());
		} else if (pt->getName() == "2") {
			CHECK(neighbours.size() == 1);
			CHECK(neighbours[0]->getName() == "3");
		} else if (pt->getName() == "3") {
			CHECK(neighbours.size() == 1);
			CHECK(neighbours[0]->getName() == "2");
		} else if (pt->getName() == "4") {
			CHECK(neighbours.empty());
		} else if (pt->getName() == "5") {
			CHECK(neighbours.empty());
		}
	}
}