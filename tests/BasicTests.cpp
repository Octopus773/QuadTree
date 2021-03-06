//
// Created by cbihan on 05/08/2021.
//

#include <catch2/catch.hpp>
#include <stdexcept>

#define private public

#include "Point.hpp"
#include "QuadTree.hpp"
#include "TestsUtils.hpp"

TEST_CASE("QuadTree Basic Use 10x10", "[QuadTree]")
{
	QuadTree::QuadTree<QuadTree::Tests::Point> qT(0, 0, 10, 10);

	qT.maxElementsPerNode = 3;

	std::vector<std::shared_ptr<QuadTree::Tests::Point>> points;

	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(4, 4, 0));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(7, 3, 1));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6.5, 6, 2));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6, 6.5, 3));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(8, 7, 4));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(9, 9, 5));

	for (const auto &pt : points) {
		qT.add(pt);
	}

	for (const auto &pt : points) {
		auto neighbours = qT.getNeighbours(pt);

		switch (pt->getUID()) {
		case 0:
			CHECK(neighbours.empty());
			break;
		case 1:
			CHECK(neighbours.empty());
			break;
		case 2:
			CHECK(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 3);
			break;
		case 3:
			CHECK(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 2);
			break;
		case 4:
			CHECK(neighbours.empty());
			break;
		case 5:
			CHECK(neighbours.empty());
			break;
		default:
			CHECK(false);
			break;
		}
	}

	REQUIRE(getSizeNodes(qT) == 9);
	REQUIRE(qT._elements.size() == 6);


	qT.remove(points[3]);
	qT.remove(points[5]);

	for (const auto &pt : points) {
		std::vector<std::shared_ptr<QuadTree::Tests::Point>> neighbours;
		if (pt->getUID() == 3 || pt->getUID() == 5) {
			CHECK_THROWS_AS(neighbours = qT.getNeighbours(pt), std::runtime_error);
		} else {
			neighbours = qT.getNeighbours(pt);
		}

		switch (pt->getUID()) {
		case 0:
			CHECK(neighbours.empty());
			break;
		case 1:
			CHECK(neighbours.empty());
			break;
		case 2:
			CHECK(neighbours.empty());
			break;
		case 3:
			break;
		case 4:
			CHECK(neighbours.empty());
			break;
		case 5:
			break;
		default:
			CHECK(false);
			break;
		}
	}

	points[0]->horizontalPos = 7;
	points[0]->verticalPos = 2.5;

	qT.update(points[0]);

	for (const auto &pt : points) {
		std::vector<std::shared_ptr<QuadTree::Tests::Point>> neighbours;
		if (pt->getUID() == 3 || pt->getUID() == 5) {
			CHECK_THROWS_AS(neighbours = qT.getNeighbours(pt), std::runtime_error);
		} else {
			neighbours = qT.getNeighbours(pt);
		}

		switch (pt->getUID()) {
		case 0:
			CHECK(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 1);
			break;
		case 1:
			CHECK(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 0);
			break;
		case 2:
			CHECK(neighbours.empty());
			break;
		case 3:
			break;
		case 4:
			CHECK(neighbours.empty());
			break;
		case 5:
			break;
		default:
			CHECK(false);
			break;
			}
	}

	REQUIRE(qT._nodes.size() == 9);
	REQUIRE(qT._elements.size() == 4);

	qT.remove(points[2]);
	qT.remove(points[4]);

	qT.cleanup();

	REQUIRE(getSizeNodes(qT) == 5);
	REQUIRE(qT._elements.size() == 2);
}

TEST_CASE("QuadTree Basic Use 510x510", "[QuadTree]")
{
	// dataset realized with
	// https://editor.p5js.org/Aslatas/sketches/bQwvi43Uf
	// modified code in ressources/datacreator.js

	QuadTree::QuadTree<QuadTree::Tests::Point> qT(0, 0, 510, 510);

	qT.maxElementsPerNode = 4;

	std::vector<std::shared_ptr<QuadTree::Tests::Point>> points;

	//node name: 5
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(17.587791774952002, 95.93226289306212, 2));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(86.44172050933604, 1.9014910363196855, 22));
	//node name: 6
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(182.7592770117985, 25.670905073871157, 3));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(243.6619951772354, 68.1650848511016, 15));
	//node name: 7
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(50.8155727453303, 213.90748727490248, 7));
	//node name: 8
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(190.54758139288384, 170.04255970715494, 14));
	//node name: 9
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(345.26990553691786, 264.0505688799703, 1));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(302.2963430534768, 317.77778286415247, 8));
	//node name: 10
	//node name: 11
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(298.50638999235247, 461.0012159917385, 6));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(355.29217137903663, 446.170498001138, 17));
	//node name: 12
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(428.70133789213617, 503.32618496430536, 10));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(465.4831033338977, 454.1612878006692, 24));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(391.4714586035678, 413.05645590410654, 29));
	//node name: 13
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(357.5502623387069, 9.812098568075342, 4));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(361.5578227878114, 43.73370746298695, 18));
	//node name: 14
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(438.5406048375477, 112.52959453775483, 16));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(390.31658295518815, 13.575343724123293, 19));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(419.1764369605108, 34.775507023212135, 27));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(424.03307536587937, 85.4499760955955, 28));
	//node name: 15
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(265.9518772017447, 223.84625820210667, 11));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(279.21063259461755, 215.1177466015272, 12));
	//node name: 16
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(463.3793267511266, 201.60987573235937, 20));
	//node name: 17
	//node name: 19
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(101.52272893442452, 410.28761448299093, 23));
	//node name: 20
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(147.76200732161158, 507.34858928203084, 9));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(144.52997918875272, 443.6795010217856, 13));
	//node name: 21
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(140.97591539674264, 292.56875240437387, 21));
	//node name: 22
	//node name: 23
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(150.40901829555656, 345.46205549112733, 0));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(170.92504306870342, 323.0685427595462, 25));
	//node name: 24
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(194.21247306333086, 376.2844470618495, 5));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(204.46590948258952, 369.00173545475093, 26));

	for (const auto &pt : points) {
		qT.add(pt);
	}

	for (const auto &pt : points) {
		const auto neighbours = qT.getNeighbours(pt);


		switch (pt->getUID()) {
			// node 5
		case 2:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 22);
			break;
		case 22:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 2);
			break;
			// node 6
		case 3:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 15);
			break;
		case 15:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 3);
			break;
			// node 7
		case 7:
			CHECK(neighbours.empty());
			break;
		// node 8
		case 14:
			CHECK(neighbours.empty());
			break;
		// node 9
		case 1:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 8);
			break;
		case 8:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 1);
			break;
			// node 11
		case 6:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 17);
			break;
		case 17:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 6);
			break;
			// node 12
		case 10:
			CHECK(neighbours.size() == 2);
			for (const auto &value : {24, 29}) {
				CHECK(std::find_if(neighbours.begin(), neighbours.end(), [value](const auto &neighbour) {
					return neighbour->getUID() == value;
				}) != neighbours.end());
			}
			break;
		case 24:
			CHECK(neighbours.size() == 2);
			for (const auto &value : {10, 29}) {
				CHECK(std::find_if(neighbours.begin(), neighbours.end(), [value](const auto &neighbour) {
					return neighbour->getUID() == value;
				}) != neighbours.end());
			}
			break;
		case 29:
			CHECK(neighbours.size() == 2);
			for (const auto &value : {24, 10}) {
				CHECK(std::find_if(neighbours.begin(), neighbours.end(), [value](const auto &neighbour) {
					return neighbour->getUID() == value;
				}) != neighbours.end());
			}
			break;
			// node 13
		case 4:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 18);
			break;
		case 18:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 4);
			break;
			// node 14
		case 16:
			CHECK(neighbours.size() == 3);
			for (const auto &value : {19, 28, 27}) {
				CHECK(std::find_if(neighbours.begin(), neighbours.end(), [value](const auto &neighbour) {
					return neighbour->getUID() == value;
				}) != neighbours.end());
			}
			break;
		case 19:
			CHECK(neighbours.size() == 3);
			for (const auto &value : {16, 27, 28}) {
				CHECK(std::find_if(neighbours.begin(), neighbours.end(), [value](const auto &neighbour) {
					return neighbour->getUID() == value;
				}) != neighbours.end());
			}
			break;
		case 27:
			CHECK(neighbours.size() == 3);
			for (const auto &value : {16, 19, 28}) {
				CHECK(std::find_if(neighbours.begin(), neighbours.end(), [value](const auto &neighbour) {
					return neighbour->getUID() == value;
				}) != neighbours.end());
			}
			break;
		case 28:
			CHECK(neighbours.size() == 3);
			for (const auto &value : {16, 19, 27}) {
				CHECK(std::find_if(neighbours.begin(), neighbours.end(), [value](const auto &neighbour) {
					return neighbour->getUID() == value;
				}) != neighbours.end());
			}
			break;
			// node 15
		case 11:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 12);
			break;
		case 12:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 11);
			break;
			// node 16
		case 20:
			CHECK(neighbours.empty());
			break;
			// node 19
		case 23:
			CHECK(neighbours.empty());
			break;
			// node 20
		case 9:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 13);
			break;
		case 13:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 9);
			break;
			// node 21
		case 21:
			CHECK(neighbours.empty());
			break;
			// node 23
		case 0:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 25);
			break;
		case 25:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 0);
			break;
			// node 24
		case 5:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 26);
			break;
		case 26:
			REQUIRE(neighbours.size() == 1);
			CHECK(neighbours[0]->getUID() == 5);
			break;
		default:
			// shouldn't go here
			CHECK(false);
			break;
		}

	}
}

TEST_CASE("QuadTree cleanup test with empty tree", "[QuadTree][cleanup]")
{
	QuadTree::QuadTree<QuadTree::Tests::Point> qT(0, 0, 10, 10);

	qT.maxElementsPerNode = 3;

	std::vector<std::shared_ptr<QuadTree::Tests::Point>> points;

	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(4, 4, 0));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(7, 3, 1));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6.5, 6, 2));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6, 6.5, 3));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(8, 7, 4));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(9, 9, 5));

	for (auto &pt : points) {
		qT.add(pt);
	}
	REQUIRE(getSizeNodes(qT) == 9);

	for (auto &pt : points) {
		qT.remove(pt);
	}
	REQUIRE(getSizeNodes(qT) == 9);
	qT.cleanup();
	REQUIRE(getSizeNodes(qT) == 5);
	qT.cleanup();
	REQUIRE(getSizeNodes(qT) == 1);
}

TEST_CASE("QuadTree cleanup test with not empty tree", "[QuadTree][cleanup]")
{
	QuadTree::QuadTree<QuadTree::Tests::Point> qT(0, 0, 10, 10);

	qT.maxElementsPerNode = 3;

	std::vector<std::shared_ptr<QuadTree::Tests::Point>> points;

	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(4, 4, 0));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(7, 3, 1));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6.5, 6, 2));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6, 6.5, 3));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(8, 7, 4));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(9, 9, 5));

	for (auto &pt : points) {
		qT.add(pt);
	}
	REQUIRE(getSizeNodes(qT) == 9);

	qT.remove(points[2]);
	qT.remove(points[3]);

	REQUIRE(getSizeNodes(qT) == 9);
	qT.cleanup();
	REQUIRE(getSizeNodes(qT) == 9);
	qT.cleanup();
	REQUIRE(getSizeNodes(qT) == 9);
}

TEST_CASE("QuadTree reCreation test 6 -> 4 points", "[QuadTree][reCreation]")
{
	QuadTree::QuadTree<QuadTree::Tests::Point> qT(0, 0, 10, 10);

	qT.maxElementsPerNode = 3;

	std::vector<std::shared_ptr<QuadTree::Tests::Point>> points;

	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(4, 4, 0));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(7, 3, 1));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6.5, 6, 2));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6, 6.5, 3));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(8, 7, 4));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(9, 9, 5));

	for (auto &pt : points) {
		qT.add(pt);
	}
	REQUIRE(getSizeNodes(qT) == 9);

	qT.remove(points[2]);
	qT.remove(points[3]);

	qT.reCreate();

	for (const auto &pt : points) {
		std::vector<std::shared_ptr<QuadTree::Tests::Point>> neighbours;
		if (pt->getUID() == 2 || pt->getUID() == 3) {
			CHECK_THROWS_AS(neighbours = qT.getNeighbours(pt), std::runtime_error);
		} else {
			neighbours = qT.getNeighbours(pt);
		}

		switch (pt->getUID()) {
		case 0:
			CHECK(neighbours.empty());
			break;
		case 1:
			CHECK(neighbours.empty());
			break;
		case 2:
		case 3:
			break;
		case 4:
			REQUIRE(neighbours.size() == 1);
			REQUIRE(neighbours[0]->getUID() == 5);
			break;
		case 5:
			REQUIRE(neighbours.size() == 1);
			REQUIRE(neighbours[0]->getUID() == 4);
			break;
		default:
			CHECK(false);
			break;
		}
	}
}

TEST_CASE("QuadTree reCreation test 6 -> 3 points", "[QuadTree][reCreation]")
{
	QuadTree::QuadTree<QuadTree::Tests::Point> qT(0, 0, 10, 10);

	qT.maxElementsPerNode = 3;

	std::vector<std::shared_ptr<QuadTree::Tests::Point>> points;

	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(4, 4, 0));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(7, 3, 1));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6.5, 6, 2));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(6, 6.5, 3));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(8, 7, 4));
	points.emplace_back(std::make_shared<QuadTree::Tests::Point>(9, 9, 5));

	for (auto &pt : points) {
		qT.add(pt);
	}

	qT.remove(points[2]);
	qT.remove(points[3]);
	qT.remove(points[0]);

	qT.reCreate();

	for (const auto &pt : points) {
		std::vector<std::shared_ptr<QuadTree::Tests::Point>> neighbours;
		if (pt->getUID() == 2 || pt->getUID() == 3 || pt->getUID() == 0) {
			CHECK_THROWS_AS(neighbours = qT.getNeighbours(pt), std::runtime_error);
		} else {
			neighbours = qT.getNeighbours(pt);
		}

		switch (pt->getUID()) {
		case 1:
			REQUIRE(neighbours.size() == 2);
			for (const auto &value : {4, 5}) {
				REQUIRE(std::find_if(neighbours.begin(), neighbours.end(), [value](const auto &neighbour) {
					return neighbour->getUID() == value;
				}) != neighbours.end());
			}
			break;
		case 0:
		case 2:
		case 3:
			break;
		case 4:
			REQUIRE(neighbours.size() == 2);
			for (const auto &value : {1, 5}) {
				REQUIRE(std::find_if(neighbours.begin(), neighbours.end(), [value](const auto &neighbour) {
					return neighbour->getUID() == value;
				}) != neighbours.end());
			}
			break;
		case 5:
			REQUIRE(neighbours.size() == 2);
			for (const auto &value : {4, 1}) {
				REQUIRE(std::find_if(neighbours.begin(), neighbours.end(), [value](const auto &neighbour) {
					return neighbour->getUID() == value;
				}) != neighbours.end());
			}
			break;
		default:
			REQUIRE(false);
			break;
		}
	}
}

TEST_CASE("_allocNodes & _freeNodes", "[QuadTree][Memory]")
{
	QuadTree::QuadTree<QuadTree::Tests::Point> qT(0, 0, 10, 10);

	REQUIRE(getSizeNodes(qT) == 1);
	REQUIRE(qT._firstFreeNode == -1);

	int index = qT._allocNodes();

	REQUIRE(index == 1);
	REQUIRE(getSizeNodes(qT) == 5);
	REQUIRE(qT._firstFreeNode == -1);

	index = qT._allocNodes();

	REQUIRE(index == 5);
	REQUIRE(getSizeNodes(qT) == 9);
	REQUIRE(qT._firstFreeNode == -1);

	qT._freeNodes(1);
	REQUIRE(qT._firstFreeNode == 1);
	REQUIRE(getSizeNodes(qT) == 5);

	index = qT._allocNodes();

	REQUIRE(index == 1);
	REQUIRE(getSizeNodes(qT) == 9);
	REQUIRE(qT._firstFreeNode == -1);


}