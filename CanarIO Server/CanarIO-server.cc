//Canar.IO-server by
//Pablo Martín Navarro
//Jesús Ramos Rodríguez

#include <unistd.h>
#include <iostream>
#include "Player.h"

int main ()
{
    std::cout << "Starting server ... \n";

    XLDisplay::init(720, 480, "CanarIO-server");
    Vector2 pos = Vector2((uint16_t)150, (uint16_t)200);
    
    Player p1 = Player(pos, (uint16_t)50, (uint8_t)2);
    p1.DebugInfo();
    std::cout << "Waiting 5 seconds and closing ... \n";

    sleep(5);
    std::cout << "Closing ... \n";
    return 0;
}