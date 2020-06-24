#pragma once
//Player class - Contains all the functionality of the CanarIO player
#include <iostream>

#include "XLDisplay.h"
#include "Vector2.h"
#include "Socket.h"

class Player
{
public:

    Player(Vector2 _position, uint16_t initial_size_, XLDisplay::XLColor _color,
                 Socket* initial_socket_, std::string initial_nickName_);
    
    Player(Vector2 _position, uint16_t initial_size_);

    ~Player();
    
    void Update(XLDisplay* dpy);
    void Move(Vector2 new_Position);
    void ChangeVelocity(int new_velocity);
    void ChangeSize(uint16_t new_Size);


    std::string nickname();
    bool IsColliding(Player* other_);
    int velocity();
    Socket* socket();
    Vector2 position();
    XLDisplay::XLColor color();
    uint16_t size();

private:

    std::string nickname_ = "null";
    int velocity_ = 5;
    XLDisplay::XLColor color_ = XLDisplay::XLColor::GREEN;
    Socket* socket_ =  nullptr;
    uint16_t size_ = 1;
    Vector2 position_ = Vector2(10, 10);
};