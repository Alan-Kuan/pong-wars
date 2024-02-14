#include <SFML/Graphics.hpp>

int main(void) {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Pong Wars");

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    return 0;
}
