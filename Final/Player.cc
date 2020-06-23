#include "Player.h"

Player::Player(Vector2 _position, uint16_t initial_size_, XLDisplay::XLColor _color)
{
    position_ = _position;
    size_ = initial_size_;
    color_ = _color;
}

Player::~Player()
{

}

void Player::Update(XLDisplay* dpy)
{
    dpy->set_color(color_);
    dpy->circle(position_.x, position_.y, size_);
}

Vector2 Player::position()
{
    return position_;
}

XLDisplay::XLColor Player::color()
{
    return color_;
}

uint16_t Player::size()
{
    return size_;
}

void Player::Move(Vector2 new_Position)
{
    position_ = new_Position;
}
