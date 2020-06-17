#pragma once
#include <inttypes.h>
#include <iostream>

class Vector2
{
public:
    uint16_t x = 0;
    uint16_t y = 0;

    Vector2();

    Vector2(uint16_t _x, uint16_t _y);

    ~Vector2();

    inline Vector2& operator = (const Vector2& vec_)
    {
        x = vec_.x;
        y = vec_.y;

        return *this;
    };

    inline bool operator== (const Vector2& vec_)
    {
        return (vec_.x == this->x && vec_.y == this->y);
    };
};