//Canar.IO-server by
//Pablo Martín Navarro
//Jesús Ramos Rodríguez

#include "XLDisplay.h"

#include <unistd.h>
#include <iostream>


int main ()
{
    std::cout << "Starting server ... \n";

    XLDisplay::init(720, 480, "CanarIO-server");

    std::cout << "Waiting 5 seconds and closing ... \n";

    sleep(5);
    std::cout << "Closing ... \n";
    return 0;
}