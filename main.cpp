#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include"Asteroid.h"
int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Asteroids prototype", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    asteroidArray testArray(500);
    for (int i = 0; i < 10; i++) {
        testArray.addAsteroid(4, 100, 200);
    }
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        testArray.checkBoundaries();
        testArray.checkCollisions();
        testArray.draw(&window);
        testArray.move();
        window.display();
    }
    return 0;
}