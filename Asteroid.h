#ifndef ASTEROID_H
#define ASTEROID_H
#include"projectile.h"
#include<iostream>
class asteroid : public projectile {
public:
	/* 
	* Constructor #1: randomly generated asteroid with set velocity whose vertices lie within a certain
	* x and y range.
	*/
	asteroid(int vertices, int xMin, int xMax, int yMin, int yMax, int xVelocity, int yVelocity) {
		if ((xMax - xMin) > (yMax - yMin)) {
			this->size = xMax - xMin;
		}
		else {
			this->size = yMax - yMin;
		}
		int minimum_area = 0.4 * (xMax - xMin)*(yMax - yMin);
	restart:
		do {
			points = sort(generateRandomVertices(vertices, xMin, xMax, yMin, yMax), vertices);
		} while (points[vertices - 2].position.y < points[vertices - 1].position.y && points[vertices].position.y < points[vertices - 1].position.y);
		points[vertices] = points[0];
		area = calculateArea(points, vertices);
		if (area < minimum_area) {
			goto restart;
		}
		number_of_vertices = vertices;
		velocity.x = xVelocity;
		velocity.y = yVelocity;
	}
	// Constructor 2: asteroid with random shape and position, set size and speed.
	asteroid(int vertices, int size, int speed) {
		this->size = size;
		size = size / 2;
		int minimum_area = 0.8 * size * size;
		sf::Vector2f position = generateRandomVector(0, 1920, 0, 1080);
		sf::Vector2f target = generateRandomVector(0, 1920, 0, 1080);
	restart:
		do {
			points = sort(generateRandomVertices(vertices, position.x - size, position.x + size, position.y - size, position.y + size), vertices);
		} while (points[vertices - 2].position.y < points[vertices - 1].position.y && points[vertices].position.y < points[vertices - 1].position.y);
		points[vertices] = points[0];
		area = calculateArea(points, vertices);
		if (area < minimum_area) {
			goto restart;
		}
		number_of_vertices = vertices;
		velocity = sf::Vector2f(target.x - position.x, target.y - position.y);
		int magnitude = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
		velocity.x = speed * velocity.x / magnitude;
		velocity.y = speed * velocity.y / magnitude;
	}
	// Constructor 3: asteroid with set points and velocity.
	asteroid(sf::VertexArray array, int length, sf::Vector2f velocity) {
		points = array;
		this->velocity = velocity;
		number_of_vertices = length;
		area = calculateArea(array, length);
	}
	// Constructor 4: default constructor used as a placeholder in the array.
	asteroid() {
		area = 0;
	}
	// Function used to calculate the area of an asteroid.
	float calculateArea(sf::VertexArray points, int length) {
		float sum = 0;
		for (int i = 0; i < length; i++) {
			sum += (points[i].position.y + points[i + 1].position.y) * (points[i].position.x - points[i + 1].position.x);
		}
		return -0.5 * sum;
	}
	// Overriding pure virtual function from projectile.
	virtual void function() {};
	~asteroid() {};
};
//Asteroid array used to manage on screen objects. Inherits from asteroid to reuse some functions.
class asteroidArray : public asteroid {
private:
	// Asteroid array.
	asteroid* array;
	// Number of asteroids in the array.
	int size;
public:
	// Functions used to find maximums/minimums.
	int xMax(sf::VertexArray array, int size) {
		int max = array[0].position.x;
		for (int i = 1; i < size; i++) {
			if (array[i].position.x > max) {
				max = array[i].position.x;
			}
		}
		return max;
	}
	int yMax(sf::VertexArray array, int size) {
		int max = array[0].position.y;
		for (int i = 1; i < size; i++) {
			if (array[i].position.y > max) {
				max = array[i].position.y;
			}
		}
		return max;
	}
	int xMin(sf::VertexArray array, int size) {
		int min = array[0].position.x;
		for (int i = 1; i < size; i++) {
			if (array[i].position.x < min) {
				min = array[i].position.x;
			}
		}
		return min;
	}
	int yMin(sf::VertexArray array, int size) {
		int min = array[0].position.y;
		for (int i = 1; i < size; i++) {
			if (array[i].position.y < min) {
				min = array[i].position.y;
			}
		}
		return min;
	}
	// Constructor.
	asteroidArray(int size) {
		this->size = size;
		array = new asteroid[size];
		for (int i = 0; i < size; i++) {
			asteroid placeholder;
			array[i] = placeholder;
		}
	}
	// Increments all objects by their velocities.
	void move() {
		for (int i = 0; i < size; i++) {
			if (array[i].getArea() != 0) {
				array[i].movePoints();
			}
		}
	}
	// Functions to add asteroids to the array using different asteroid constructors.
	void addAsteroid(int vertices, int size, int speed) {
		for (int i = 0; i < size; i++) {
			if (array[i].getArea() == 0) {
				array[i].~asteroid();
				asteroid newAsteroid(vertices, size, speed);
				array[i] = newAsteroid;
				return;
			}
		}
	}
	void addAsteroid(sf::VertexArray vertices, int length, sf::Vector2f velocity) {
		for (int i = 0; i < size; i++) {
			if (array[i].getArea() == 0) {
				array[i].~asteroid();
				asteroid newAsteroid(vertices, length, velocity);
				array[i] = newAsteroid;
				return;
			}
		}
	}
	void addAsteroid(int vertices, int xMin, int xMax, int yMin, int yMax, int xVelocity, int yVelocity) {
		for (int i = 0; i < size; i++) {
			if (array[i].getArea() == 0) {
				array[i].~asteroid();
				asteroid newAsteroid(vertices, xMin, xMax, yMin, yMax, xVelocity, yVelocity);
				array[i] = newAsteroid;
				return;
			}
		}
	}
	// Removes asteroid at index i.
	void remove(int i) {
		array[i].~asteroid();
		asteroid placeholder;
		array[i] = placeholder;
	}
	// Draws asteroids in the array on screen.
	void draw(sf::RenderWindow* window) {
		for (int i = 0; i < size; i++) {
			if (array[i].getArea() != 0) {
				window->draw(array[i].getPoints());
			}
		}
	}
	// Repositions asteroids once they go off screen.
	void checkBoundaries() {
		for (int j = 0; j < size; j++) {
			if (array[j].getArea() != 0) {
				for (int i = 0; i < array[j].getNVertices(); i++) {
					if (array[j].getPoints()[i].position.x >= 1920 + array[j].getSize()) {
						array[j].setPoints(-1 * (1920 + array[j].getSize() - 1), 0);
					}
					else if (array[j].getPoints()[i].position.x <= 0 - 1 * array[j].getSize()) {
						array[j].setPoints(1920 + array[j].getSize() - 1 , 0);
					}
					else if (array[j].getPoints()[i].position.y >= 1080 + 1 * array[j].getSize()) {
						array[j].setPoints(0, -1 * (1080 + array[j].getSize() - 1));
					}
					else if (array[j].getPoints()[i].position.y <= 0 - 1 * array[j].getSize()) {
;						array[j].setPoints(0, 1080 + array[j].getSize() - 1);
					}
				}
			}
		}
	}
	// Detects collisions between asteroids and handles the destruction physics.
	void checkCollisions() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (i != j && array[i].getArea() != 0 && array[j].getArea() != 0) {
					for (int k = 0; k < array[j].getNVertices(); k++) {
						sf::VertexArray testArray = sf::VertexArray(sf::LineStrip, 6);
						for (int l = 0; l < array[i].getNVertices(); l++) {
							testArray[l] = array[i].getPoints()[l];
						}
						testArray[4] = array[j].getPoints()[k];
						testArray = sort(testArray, 5);
						testArray[5] = testArray[0];
						if (xMin(testArray, 5) == xMin(array[i].getPoints(), 4) && xMax(testArray, 5) == xMax(array[i].getPoints(), 4) && yMax(testArray, 5) == yMax(array[i].getPoints(), 4) && yMin(testArray, 5) == yMin(array[i].getPoints(), 4)) {
							if (calculateArea(testArray, 5) < array[i].getArea() && calculateArea(testArray, 5) > 0) {
								if (array[i].getArea() > 500) {
									int magnitude = sqrt(array[i].getVelocity().x * array[i].getVelocity().x + array[i].getVelocity().y * array[i].getVelocity().y);
									int modifiedMagnitude = 0.5 * magnitude;
									sf::VertexArray asteroidFragment(sf::LineStrip, 8);
									for (int z = 0; z < array[i].getNVertices(); z++) {
										asteroidFragment[z] = array[i].getPoints()[z];
									}
									for (int z = 0; z < array[j].getNVertices(); z++) {
										asteroidFragment[z + array[i].getNVertices()] = array[j].getPoints()[z];
									}
									int xMidpoint = xMin(asteroidFragment, 8) + (xMax(asteroidFragment, 8) - xMin(asteroidFragment, 8)) / 2;
									int yMidpoint = yMin(asteroidFragment, 8) + (yMax(asteroidFragment, 8) - yMin(asteroidFragment, 8)) / 2;
									addAsteroid(4,xMin(asteroidFragment, 8), xMidpoint, yMin(asteroidFragment, 8), yMidpoint, -modifiedMagnitude, -modifiedMagnitude);
									addAsteroid(4, xMidpoint, xMax(asteroidFragment,8), yMin(asteroidFragment, 8), yMidpoint, modifiedMagnitude, -modifiedMagnitude);
									addAsteroid(4, xMin(asteroidFragment, 8), xMidpoint, yMidpoint, yMax(asteroidFragment,8), -modifiedMagnitude, modifiedMagnitude);
									addAsteroid(4, xMidpoint, xMax(asteroidFragment,8), yMidpoint, yMax(asteroidFragment, 8), modifiedMagnitude, modifiedMagnitude);
									remove(i);
									remove(j);
								}
								else {
									remove(i);
									remove(j);
								}
							}
						}
					}
				}
			}
		}
	}
};
#endif