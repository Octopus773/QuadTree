//
// Created by cbihan on 03/08/2021.
//

#include <vector>
#include <memory>
#include <iostream>
#include <chrono>

#include <SFML/Graphics.hpp>


#include "QuadTree.hpp"
#include "Point.hpp"
#include "Rect.hpp"

#define ELT_SPEED 10

#define WORLD_MIN_H 0
#define WORLD_MIN_V 0
#define WORLD_MAX_H 1000
#define WORLD_MAX_V 1000


void drawRect(sf::RenderWindow &window, const std::array<std::pair<double, double>, 4> &points, sf::Color color = sf::Color::White)
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


int main(int ac, char **av)
{
	srand(std::time(nullptr));
	QuadTree::QuadTree<QuadTree::Tests::Rect> qT(0, 0, 1000, 1000);

	qT.maxElementsPerNode = 3;


	int incY = 0;
	int incX = 0;
	std::vector<std::shared_ptr<QuadTree::Tests::Rect>> rects {};

	int size = 10;

	for (int i = 0; i < 10; i++) {
		rects.emplace_back(std::make_shared<QuadTree::Tests::Rect>((size + 3) * incY, (size + 3) * incX++, size));

		if (((size + 3) * incX) + size > WORLD_MAX_H) {
			incX = 0;
			incY++;
		}
	}
	rects[0]->isCollided = true;

	for (auto &rect : rects) {
		qT.add(rect);
	}

	sf::RenderWindow window(sf::VideoMode(1010, 1010), "QuadTree superior!");

	auto clock = std::chrono::steady_clock::now();


	while (window.isOpen()) {

		sf::Event event{};
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		auto now = std::chrono::steady_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - clock).count() > 10) {
			clock = now;
			for (auto &rect : rects) {
				rect->points[0].first += rect->velocity.first;
				rect->points[0].second += rect->velocity.second;
				rect->points[1].second += rect->velocity.second;
				rect->points[1].first += rect->velocity.first;

				if (rect->getBottom() > WORLD_MAX_V || rect->getTop() < WORLD_MIN_V) {
					rect->velocity.second *= -1;
				}
				if (rect->getRight() > WORLD_MAX_H || rect->getLeft() < WORLD_MIN_H) {
					rect->velocity.first *= -1;
				}

				qT.update(rect);

			}
			//qT.reCreate();

			for (auto &rect : rects) {
				auto neighbours = qT.getNeighbours(rect);

				for (const auto &neighbour : neighbours) {
					int axis = 0;
					//rect->isCollided = false;
					if (rect->collide(*neighbour, axis)) {
					//	rect->isCollided = true;
					//	continue;
						if (axis == QuadTree::Tests::Rect::HorizontalAxis) {
							rect->velocity.first *= -1;
						} else {
							rect->velocity.second *= -1;
						}
					}
				}
			}



		}
		window.clear();
		for (auto &rect : rects) {
			drawRect(window, rectToArray(*rect), rect->isCollided ? sf::Color::Red : sf::Color::Yellow);

		}


		window.display();
	}


}