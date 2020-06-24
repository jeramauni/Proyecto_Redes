#include "Player.h"
#include <cmath>

Player::Player(Vector2 _position, uint16_t initial_size_, XLDisplay::XLColor _color,
                 Socket* initial_socket_, std::string initial_nickName_)
{
    position_ = _position;
    size_ = initial_size_;
    color_ = _color;
    socket_ = initial_socket_;
    nickname_ = initial_nickName_;
}

Player::~Player()
{

}

void Player::Update(XLDisplay* dpy)
{
    dpy->set_color(color_);
    dpy->circle(position_.x, position_.y, size_);
}

bool Player::IsColliding(Player* other_)
{
    int colision_zone = (size_ / 4) + (other_->size() / 4);
    std::cout << colision_zone << "\n";
    if(std::abs(position_.x - other_->position().x) < colision_zone &&
       std::abs(position_.y - other_->position().y) < colision_zone)
    {
        //Si los dos miden lo mismo es más justo que simplemente no pase nada
        if(size_ == other_->size()) return false;
        else if(size_ > other_->size()) return false;
        else return true;
    }
    else return false;
}

int Player::velocity()
{
    return velocity_;
}

std::string Player::nickname()
{
    return nickname_;
}

Vector2 Player::position()
{
    return position_;
}

Socket* Player::socket()
{
    return socket_;
}

XLDisplay::XLColor Player::color()
{
    return color_;
}

uint16_t Player::size()
{
    return size_;
}

void Player::ChangeVelocity(int new_velocity)
{
    velocity_ = new_velocity;
}

void Player::ChangeSize(uint16_t new_Size)
{
    size_ = new_Size;
}

void Player::Move(Vector2 new_Position)
{
    position_ = new_Position;
}
