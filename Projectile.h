#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include<chrono>
#include<random>
#include<iostream>
/*
* Projectile is the base class for everything else. 
*/
class projectile {
protected:
	// Vertex array containing the vertices used by SFML to draw a shape on screen
	sf::VertexArray points;
	// Number of unique vertices in the array
	int number_of_vertices;
	// Side lengths of the smallest square that encloses all points
	int size;
	// Area of the shape generated by the vertex array
	float area = 0;
	// Velocity vector
	sf::Vector2f velocity;
public:
	sf::Vector2f getVelocity() {
		return velocity;
	}
	float getArea() {
		return area;
	}
	sf::VertexArray getPoints() {
		return points;
	}
	//Used to incremement the position of an object by its velocity vector every frame
	void movePoints(float k = 0.0167) {
		for (int i = 0; i < number_of_vertices + 1; i++) {
			points[i].position.x += velocity.x * k;
			points[i].position.y += velocity.y * k;
		}
	}
	//Used to change the position of an object (currently only used for when something goes off screen)
	void setPoints(int x, int y) {
		for (int i = 0; i < number_of_vertices + 1; i++) {
			points[i].position.x += x;
			points[i].position.y += y;
		}
	}
	int getNVertices() {
		return number_of_vertices;
	}
	int getSize() {
		return size;
	}
	/*
	* Shitty sorting algorithm. Designed to arrange the points in a vertex array such that SFML draws
	* a clean looking shape (no intersecting lines). Was originally supposed to work for a polygon with
	* any number of vertices, but it only seems to work reliably with vertex arrays with 4 or less unique
	* vertices.
	*/
	sf::VertexArray sort(sf::VertexArray array, int length) {
	a:
		for (int i = 0; i < length - 1; i++) {
			if (array[i].position.y < array[i + 1].position.y) {
				sf::Vector2f temp = array[i].position;
				array[i].position = array[i + 1].position;
				array[i + 1].position = temp;
				goto a;
			}
		}
	b:
		for (int i = 0; i < (length / 2) - 1; i++) {
			if (array[i].position.x > array[i + 1].position.x) {
				sf::Vector2f temp = array[i].position;
				array[i].position = array[i + 1].position;
				array[i + 1].position = temp;
				goto b;
			}
		}
	c:
		for (int i = (length) / 2; i < length; i++) {
			if (array[i].position.x < array[i + 1].position.x) {
				sf::Vector2f temp = array[i].position;
				array[i].position = array[i + 1].position;
				array[i + 1].position = temp;
				goto c;
			}
		}
		return array;
	}
	int generateRandomNumber(double lower, double upper) {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator(seed);
		std::uniform_real_distribution distribution(lower / 1000000, upper / 1000000);
		return int(distribution(generator) * 1000000);
	}
	sf::Vector2f generateRandomVector(int xlower, int xupper, int ylower, int yupper) {
		return sf::Vector2f(generateRandomNumber(xlower, xupper), generateRandomNumber(ylower, yupper));
	}
	sf::VertexArray generateRandomVertices(int length, int xlower, int xupper, int ylower, int yupper) {
		sf::VertexArray array(sf::LineStrip, length + 1);
		for (int i = 0; i < length; i++) {
			array[i].position = generateRandomVector(xlower, xupper, ylower, yupper);
		}
		return array;
	}
	// Pure virtual function. Serves no purpose but to make this an abstract class (rubric requirement).
	virtual void function() = 0;
	virtual ~projectile() {};
};
#endif