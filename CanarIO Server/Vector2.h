#pragma once
#include <inttypes.h>
#include <iostream>

class Vector2
{
    public:
    uint16_t x = 0;
    uint16_t y = 0;

    Vector2()   { x = 0; y = 0; }

    Vector2(uint16_t _x, uint16_t _y)
    {
        x = _x;
        y = _y;
    }
    
    ~Vector2() {}

    /*Vector2& operator = (const Vector2 &vec_new);
    {
        x = vec_new->x;
        y = vec_new->y;

        return *this;
    }
    
    bool operator == (const Vector2 &vec_1, const Vector2 &vec_2);
    {
        return (vec_1->x == vec_2->x && vec_1->y == vec_2->y)
    }

    ostream& operator << (ostream& os, const Vector2& v)
    {
        os << "{ " << v.x << ", " << v.y << "}";
        return os;
    }*/
};