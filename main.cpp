#include <SFML/Graphics.hpp>

const int kMsPerTick = 20;

int main(void) {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Pong Wars");
    sf::Clock clock;
    sf::CircleShape ball;

    ball.setPosition(0, 0);
    ball.setRadius(10);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        if (clock.getElapsedTime().asMilliseconds() % kMsPerTick == 0) {
            ball.move(1, 1);
        }

        window.draw(ball);
        window.display();
    }

    return 0;
}
