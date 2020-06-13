#include "Player.h"

Player::Player(Vector2 position_, uint16_t initial_size_, uint8_t color_)
{
    position = position_;
    size = initial_size_;
    color = color_;
}

Player::~Player()
{

}

void Player::DebugInfo()
{
    std::cout << "Position: (" << position.x << ", " << position.y << ")\n";
    std::cout << "Size: " << size << '\n';
    std::cout << "Color: " << color << '\n';
}