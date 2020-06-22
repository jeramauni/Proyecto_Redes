#pragma once
//Player class - Contains all the functionality of the CanarIO player
#include <iostream>

#include "XLDisplay.h"
#include "Vector2.h"
#include "Socket.h"

class Player
{
public:

    Player(Vector2 _position, uint16_t initial_size_, XLDisplay::XLColor _color);
    ~Player();
    
    void Update(XLDisplay* dpy);
    void Move(Vector2 new_Position);
    
    Vector2 position();
    XLDisplay::XLColor color();
    uint16_t size();

private:

    XLDisplay::XLColor color_;
    uint16_t size_;
    Vector2 position_;
};