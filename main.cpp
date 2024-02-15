#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include <SFML/Graphics.hpp>

const unsigned int kWindowWidth = 600;
const float kBoundaryWidth = 500;
const unsigned int kBottomMargin = 40;
const float kBlockWidth = 25;
const float kBallRadius = 12.5;
const int kMsPerTick = 20;

const struct {
    sf::Color mystic_mint{217, 232, 227};
    sf::Color nocturnal_expedition{17, 76, 90};
    sf::Color ocean_noir{23, 43, 54};
} kColorPalette;

struct Ball {
    sf::CircleShape shape;
    sf::Vector2f vel;
    unsigned int block_count;  // number of blocks whose color is same as the ball

    Ball(const sf::Vector2f& pos, const sf::Vector2f& vel, const sf::Color& color, unsigned int block_count) {
        this->shape.setRadius(kBallRadius);
        this->shape.setOrigin(kBallRadius, kBallRadius);
        this->shape.setPosition(pos);
        this->shape.setFillColor(color);
        this->vel = vel;
        this->block_count = block_count;
    }
};

inline sf::Color getOppoColor(sf::Color& color) {
    return color == kColorPalette.mystic_mint
        ? kColorPalette.nocturnal_expedition
        : kColorPalette.mystic_mint;
}

int main(void) {
    sf::RenderWindow window(sf::VideoMode(kWindowWidth, kWindowWidth + kBottomMargin), "Pong Wars", sf::Style::Titlebar | sf::Style::Close);
    sf::View view(sf::Vector2f(kBoundaryWidth / 2, (kBoundaryWidth + kBottomMargin) / 2), sf::Vector2f(kWindowWidth, kWindowWidth + kBottomMargin));
    window.setView(view);

    // background
    sf::VertexArray bg(sf::Quads, 4);
    float gap_width = (kWindowWidth - kBoundaryWidth) / 2.0;
    bg[0].position = sf::Vector2f(-gap_width, -gap_width);
    bg[1].position = sf::Vector2f(kBoundaryWidth + gap_width, -gap_width);
    bg[2].position = sf::Vector2f(kBoundaryWidth + gap_width, kBoundaryWidth + gap_width + kBottomMargin);
    bg[3].position = sf::Vector2f(-gap_width, kBoundaryWidth + gap_width + kBottomMargin);
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
    unsigned int block_num_per_color = block_num_per_dim * block_num_per_dim / 2;

    // balls
    std::vector<Ball> balls = {
        Ball({100, 20}, {-1, 1}, kColorPalette.nocturnal_expedition, block_num_per_color),
        Ball({350, 230}, {1, 1}, kColorPalette.mystic_mint, block_num_per_color),
    };

    // text
    sf::Font font;
    if (!font.loadFromFile("./ComicMono.ttf")) {
        std::cerr << "Failed to load the font!" << std::endl;
        return 1;
    }

    sf::Text info;
    info.setFont(font);
    info.setCharacterSize(16);
    info.setFillColor(kColorPalette.ocean_noir);
    info.setString("day xxx | night xxx");  // placeholder
    float text_x = (kBoundaryWidth - info.getGlobalBounds().getSize().x) / 2;
    info.setPosition(sf::Vector2f(text_x, kBoundaryWidth + kBottomMargin / 2.0));

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

        for (auto& ball : balls) {
            sf::Color ball_color = ball.shape.getFillColor();
            sf::Vector2f pos = ball.shape.getPosition();
            int i = pos.x / kBlockWidth;
            int j = pos.y / kBlockWidth;
            int left_side_i = (pos.x - kBallRadius) / kBlockWidth;
            int right_side_i = (pos.x + kBallRadius) / kBlockWidth;
            int top_side_j = (pos.y - kBallRadius) / kBlockWidth;
            int bottom_side_j = (pos.y + kBallRadius) / kBlockWidth;

            if (pos.x - kBallRadius < 0 || pos.x + kBallRadius >= kBoundaryWidth) {
                ball.vel.x *= -1;
            } else {
                auto& left_block = blocks[left_side_i][j];
                auto& right_block = blocks[right_side_i][j];

                if (left_block.getFillColor() == ball_color) {
                    left_block.setFillColor(getOppoColor(ball_color));
                    ball.vel.x *= -1;
                    ball.block_count--;
                }
                if (right_block.getFillColor() == ball_color) {
                    right_block.setFillColor(getOppoColor(ball_color));
                    ball.vel.x *= -1;
                    ball.block_count--;
                }
            }
            if (pos.y - kBallRadius < 0 || pos.y + kBallRadius >= kBoundaryWidth) {
                ball.vel.y *= -1;
            } else {
                auto& top_block = blocks[i][top_side_j];
                auto& bottom_block = blocks[i][bottom_side_j];

                if (top_block.getFillColor() == ball_color) {
                    top_block.setFillColor(getOppoColor(ball_color));
                    ball.vel.y *= -1;
                    ball.block_count--;
                }
                if (bottom_block.getFillColor() == ball_color) {
                    bottom_block.setFillColor(getOppoColor(ball_color));
                    ball.vel.y *= -1;
                    ball.block_count--;
                }
            }

            ball.shape.move(ball.vel.x, ball.vel.y);
        }

        std::stringstream ss;
        ss << "day " << std::setw(3) << balls[1].block_count << " | " << "night " << std::setw(3) << balls[0].block_count;
        info.setString(ss.str());

        window.draw(bg);
        for (int i = 0; i < block_num_per_dim; i++) {
            for (int j = 0; j < block_num_per_dim; j++) {
                window.draw(blocks[i][j]);
            }
        }
        for (auto& ball : balls) {
            window.draw(ball.shape);
        }
        window.draw(info);
        window.display();
    }

    return 0;
}
