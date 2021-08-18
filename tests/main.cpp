//
// Created by cbihan on 03/08/2021.
//

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "PolygonTypes/Point.hpp"
#include "Collisions.hpp"
#define private public
#include "World.hpp"

using namespace QuadTree::Collisions;
using namespace std;

void drawQuadTree(sf::RenderWindow &window, QuadTree::Quadrant &quadrant)
{
	auto rect = quadrant.pos;
	rect.maxVertical *= 100;
	rect.maxHorizontal *= 100;
	rect.minHorizontal *= 100;
	rect.minVertical *= 100;
	sf::Vertex line[] {
		sf::Vertex(sf::Vector2f(rect.minHorizontal, rect.minVertical)),
		sf::Vertex(sf::Vector2f(rect.minHorizontal, rect.minVertical  + rect.getHeight()))
	};
	window.draw(line, 2, sf::Lines);
	line[0] = line[1];
	line[1] = sf::Vector2f(rect.minHorizontal + rect.getWidth(), rect.minVertical + rect.getHeight());
	window.draw(line, 2, sf::Lines);
	line[0] = line[1];
	line[1] = sf::Vector2f(rect.minHorizontal + rect.getWidth(), rect.minVertical);
	window.draw(line, 2, sf::Lines);
	line[0] = line[1];
	line[1] = sf::Vector2f(rect.minHorizontal, rect.minVertical);
	window.draw(line, 2, sf::Lines);
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
			sf::CircleShape circle(10);
			circle.setPosition(pts[0].first * 100, pts[0].second * 100);
			window.draw(circle);
			continue;
		}
		sf::ConvexShape convex;
		convex.setPointCount(pts.size());
		for (const auto &point : pts) {
			convex.setPoint(idx++, sf::Vector2f(point.first * 100, point.second * 100));
		}
		window.draw(convex);
	}
}

int main(int ac, char **av)
{
	QuadTree::World world(10, 10);

	std::vector<QuadTree::Point *> points({
		                                      new QuadTree::Point{4, 4},
		                                      new QuadTree::Point{7, 3},
		                                      new QuadTree::Point{6.5, 6},
		                                      new QuadTree::Point{6, 6.5},
		                                      new QuadTree::Point{8, 7},
		                                      new QuadTree::Point{9, 9},
	                                      });

	for (const auto &point : points) {
		world.addPolygon(point);
	}
	sf::RenderWindow window(sf::VideoMode(1010, 1010), "QuadTree superior!");
	sf::Vertex line[] {
		sf::Vertex(sf::Vector2f(10, 10)),
		sf::Vertex(sf::Vector2f(150, 150))
	};



	while (window.isOpen())
	{

		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		drawQuadTree(window, world._rootNode);
		drawPolygons(window, world);

		//window.draw(line, 2, sf::Lines);
		window.display();
	}


	std::cout << "end" << std::endl;

}