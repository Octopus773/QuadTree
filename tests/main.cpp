//
// Created by cbihan on 03/08/2021.
//

#include <vector>
#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "PolygonTypes/Point.hpp"
#include "PolygonTypes/Rectangle.hpp"
#include "Collisions.hpp"

#define private public

#include "World.hpp"

using namespace QuadTree::Collisions;
using namespace std;

void drawRect(sf::RenderWindow &window, std::array<std::pair<double, double>, 4> points, sf::Color color = sf::Color::White)
{
	sf::Vertex line[]{
		sf::Vertex(sf::Vector2f(points[0].first, points[0].second)),
		sf::Vertex(sf::Vector2f(points[1].first, points[1].second))
	};
	line[0].color = color;
	line[1].color = color;
	window.draw(line, 2, sf::Lines);
	line[0] = line[1];
	line[1] = {sf::Vector2f(points[2].first, points[2].second), color};
	window.draw(line, 2, sf::Lines);
	line[0] = line[1];
	line[1] = {sf::Vector2f(points[3].first, points[3].second), color};
	window.draw(line, 2, sf::Lines);
	line[0] = line[1];
	line[1] = {sf::Vector2f(points[0].first, points[0].second), color};
	window.draw(line, 2, sf::Lines);
}

inline std::array<std::pair<double, double>, 4> rectToArray(const QuadTree::Rect &rect)
{
	return {{
		{rect.minHorizontal, rect.minVertical},
		{rect.minHorizontal + rect.getWidth(), rect.minVertical},
		{rect.maxHorizontal, rect.maxVertical},
		{rect.minHorizontal, rect.minVertical + rect.getHeight()}
	}};
}

void drawQuadTree(sf::RenderWindow &window, QuadTree::Quadrant &quadrant)
{
	auto rect = quadrant.pos;
	/*rect.maxVertical *= 100;
	rect.maxHorizontal *= 100;
	rect.minHorizontal *= 100;
	rect.minVertical *= 100;*/
	drawRect(window, rectToArray(rect));
	for (auto &child : quadrant.children) {
		drawQuadTree(window, child);
	}
}

void drawPolygons(sf::RenderWindow &window, QuadTree::World &world)
{
	int idx = 0;
	for (const auto &poly : world.population) {
		auto pts = poly.second.polygon->getPoints();
		idx = 0;
		if (pts.size() == 1) {
			sf::CircleShape circle(5);
			circle.setPosition(pts[0].first, pts[0].second);
			window.draw(circle);
			continue;
		}
		sf::ConvexShape convex;
		convex.setPointCount(pts.size());
		for (const auto &point : pts) {
			convex.setPoint(idx++, sf::Vector2f(point.first, point.second));
		}
		window.draw(convex);
		drawRect(window, rectToArray(poly.second.aabb), sf::Color::Red);
	}
}

int main(int ac, char **av)
{
	std::srand(std::time(nullptr));
	QuadTree::World world(1000, 1000);
	world._maxPolygonPerDivision = 5;

	std::vector<QuadTree::APolygon *> points({
		                                         /* new QuadTree::Point{4, 4},
												  new QuadTree::Point{7, 3},
												  new QuadTree::Point{6.5, 6},
												  new QuadTree::Point{6, 6.5},
												  new QuadTree::Point{8, 7},
												  new QuadTree::Point{9, 9},*/
		                                         //new QuadTree::Rectangle({{{4.5, 4.5}, {4, 6}, {6, 6}, {6, 4}}}),
	                                         });
/*	points.reserve(50);
	for (int i = 0; i < 50; i++) {
		points.emplace_back(new QuadTree::Rectangle({{{0, 0}, {0, 5}, {5, 5}, {5, 0}}}));
	}*/

	for (const auto &point : points) {
		world.addPolygon(point);
	}
	sf::RenderWindow window(sf::VideoMode(1010, 1010), "QuadTree superior!");
	sf::Vertex line[]{
		sf::Vertex(sf::Vector2f(10, 10)),
		sf::Vertex(sf::Vector2f(150, 150))
	};

	auto clock = std::chrono::steady_clock::now();


	while (window.isOpen()) {

		sf::Event event{};
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		auto now = std::chrono::steady_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - clock).count() > 900) {
			clock = now;
			auto rectD = new QuadTree::Rectangle({{{0, 0}, {0, 5}, {5, 5}, {5, 0}}});
			rectD->moveTo({std::rand() % 950, std::rand() % 950});
			for (auto &pt : rectD->points) {
				pt.first += std::rand() % 10;
				pt.second += std::rand() % 10;
			}
			world.addPolygon(rectD);
		}
		window.clear();
		drawQuadTree(window, world._rootNode);
		drawPolygons(window, world);

		//window.draw(line, 2, sf::Lines);
		window.display();
	}


	std::cout << "end" << std::endl;

}