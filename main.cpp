#include <vector>

#include <SFML/Graphics.hpp>

const unsigned int kWindowWidth = 640;
const float kBoundaryWidth = 500;
const float kBlockWidth = 20;
const float kBallRadius = 10;
const int kMsPerTick = 20;

const struct {
    sf::Color mystic_mint{217, 232, 227};
    sf::Color nocturnal_expedition{17, 76, 90};
    sf::Color ocean_noir{23, 43, 54};
} kColorPalette;

int main(void) {
    sf::RenderWindow window(sf::VideoMode(kWindowWidth, kWindowWidth), "Pong Wars");
    sf::View view(sf::Vector2f(kBoundaryWidth / 2.0, kBoundaryWidth / 2.0), sf::Vector2f(kWindowWidth, kWindowWidth));
    window.setView(view);

    // background
    sf::VertexArray bg(sf::Quads, 4);
    float gap_width = (kWindowWidth - kBoundaryWidth) / 2.0;
    bg[0].position = sf::Vector2f(-gap_width, -gap_width);
    bg[1].position = sf::Vector2f(kBoundaryWidth + gap_width, -gap_width);
    bg[2].position = sf::Vector2f(kBoundaryWidth + gap_width, kBoundaryWidth + gap_width);
    bg[3].position = sf::Vector2f(-gap_width, kBoundaryWidth + gap_width);
    bg[0].color = kColorPalette.ocean_noir;
    bg[1].color = kColorPalette.ocean_noir;
    bg[2].color = kColorPalette.mystic_mint;
    bg[3].color = kColorPalette.mystic_mint;

    // blocks
    int block_num_per_dim = kBoundaryWidth / kBlockWidth;
    std::vector<std::vector<sf::RectangleShape>> blocks(
        block_num_per_dim,
        std::vector<sf::RectangleShape>(block_num_per_dim)
    );
    for (int i = 0; i < block_num_per_dim; i++) {
        for (int j = 0; j < block_num_per_dim; j++) {
            auto& block = blocks[i][j];

            block.setSize(sf::Vector2f(kBlockWidth, kBlockWidth));
            block.setPosition(i * kBlockWidth, j * kBlockWidth);

            if (i < block_num_per_dim / 2) {
                block.setFillColor(kColorPalette.mystic_mint);
            } else {
                block.setFillColor(kColorPalette.nocturnal_expedition);
            }
        }
    }

    // ball
    sf::CircleShape ball;
    ball.setPosition(350, 230);
    ball.setRadius(kBallRadius);
    ball.setOrigin(kBallRadius, kBallRadius);
    ball.setFillColor(kColorPalette.mystic_mint);

    sf::Vector2f ball_vel(1, 1);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (clock.getElapsedTime().asMilliseconds() % kMsPerTick != 0) {
            continue;
        }

        sf::Vector2f pos = ball.getPosition();
        int i = pos.x / kBlockWidth;
        int j = pos.y / kBlockWidth;
        int left_side_i = (pos.x - kBallRadius) / kBlockWidth;
        int right_side_i = (pos.x + kBallRadius) / kBlockWidth;
        int top_side_j = (pos.y - kBallRadius) / kBlockWidth;
        int bottom_side_j = (pos.y + kBallRadius) / kBlockWidth;

        if (pos.x - kBallRadius < 0 || pos.x + kBallRadius >= kBoundaryWidth) {
            ball_vel.x *= -1;
        } else {
            auto& left_block = blocks[left_side_i][j];
            auto& right_block = blocks[right_side_i][j];

            if (left_block.getFillColor() == kColorPalette.mystic_mint) {
                left_block.setFillColor(kColorPalette.nocturnal_expedition);
                ball_vel.x *= -1;
            }
            if (right_block.getFillColor() == kColorPalette.mystic_mint) {
                right_block.setFillColor(kColorPalette.nocturnal_expedition);
                ball_vel.x *= -1;
            }
        }
        if (pos.y - kBallRadius < 0 || pos.y + kBallRadius >= kBoundaryWidth) {
            ball_vel.y *= -1;
        } else {
            auto& top_block = blocks[i][top_side_j];
            auto& bottom_block = blocks[i][bottom_side_j];

            if (top_block.getFillColor() == kColorPalette.mystic_mint) {
                top_block.setFillColor(kColorPalette.nocturnal_expedition);
                ball_vel.y *= -1;
            }
            if (bottom_block.getFillColor() == kColorPalette.mystic_mint) {
                bottom_block.setFillColor(kColorPalette.nocturnal_expedition);
                ball_vel.y *= -1;
            }
        }

        ball.move(ball_vel.x, ball_vel.y);

        window.draw(bg);
        for (int i = 0; i < block_num_per_dim; i++) {
            for (int j = 0; j < block_num_per_dim; j++) {
                window.draw(blocks[i][j]);
            }
        }
        window.draw(ball);
        window.display();
    }

    return 0;
}
