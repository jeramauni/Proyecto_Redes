//Canar.IO-client by
//Pablo Martín Navarro
//Jesús Ramos Rodríguez

#include "XLDisplay.h"

#include <unistd.h>
#include <iostream>


int main ()
{
    std::cout << "Starting client ... \n";

    XLDisplay::init(720, 480, "CanarIO-client");

    std::cout << "Waiting 5 seconds and closing ... \n";

    sleep(5);
    std::cout << "Closing ... \n";
    return 0;
}