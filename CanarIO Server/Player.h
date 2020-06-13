#pragma once
//Player class - Contains all the functionality of the CanarIO player

#include "XLDisplay.h"
#include <iostream>
#include "Vector2.h"

class Player
{
public:
    Player(Vector2 position_, uint16_t initial_size_, uint8_t color_);
    ~Player();

    void DebugInfo(); 

private:
    uint8_t color;
    uint16_t size;
    Vector2 position;
};