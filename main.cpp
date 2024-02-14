#include <SFML/Graphics.hpp>

const unsigned int kWinWidth = 640;
const int kMsPerTick = 20;
const float kBoundRadius = 250;
const float kBallRadius = 10;

int main(void) {
    sf::RenderWindow window(sf::VideoMode(kWinWidth, kWinWidth), "Pong Wars");
    sf::View view(sf::Vector2f(0, 0), sf::Vector2f(kWinWidth, kWinWidth));
    window.setView(view);

    sf::RectangleShape boundary;
    boundary.setSize(sf::Vector2f(kBoundRadius * 2, kBoundRadius * 2));
    boundary.setPosition(-kBoundRadius, -kBoundRadius);
    boundary.setFillColor(sf::Color::Transparent);
    boundary.setOutlineThickness(2);
    boundary.setOutlineColor(sf::Color::White);

    sf::CircleShape ball;
    ball.setPosition(100, -20);
    ball.setRadius(kBallRadius);
    ball.setOrigin(kBallRadius, kBallRadius);

    sf::Vector2f ball_vel(1, 1);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        if (clock.getElapsedTime().asMilliseconds() % kMsPerTick == 0) {
            sf::Vector2f pos = ball.getPosition();

            if (pos.x + kBallRadius > kBoundRadius || pos.x - kBallRadius < -kBoundRadius) {
                ball_vel.x *= -1;
            }
            if (pos.y + kBallRadius > kBoundRadius || pos.y - kBallRadius < -kBoundRadius) {
                ball_vel.y *= -1;
            }

            ball.move(ball_vel.x, ball_vel.y);
        }

        window.draw(boundary);
        window.draw(ball);
        window.display();
    }

    return 0;
}
