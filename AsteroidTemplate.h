#ifndef ASTEROIDTEMPLATE_H
#define ASTEROIDTEMPLATE_H
#include"Projectile.h"
class asteroidTemplate : public projectile {
public:
	void virtual fragment(projectile* pArray, sf::VertexArray array, int length, sf::Vertex x, sf::Vertex y) {};
};
#endif
