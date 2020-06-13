//Canar.IO-server by
//Pablo Martín Navarro
//Jesús Ramos Rodríguez

#include <unistd.h>
#include <iostream>
#include <list> 
#include <iterator> 
#include "Player.h"

void draw_players()
{

}
int main ()
{
    std::cout << "Starting server ... \n";

    XLDisplay::init(720, 480, "CanarIO-server");
    Vector2 pos = Vector2((uint16_t)0, (uint16_t)200);
    std::list <Player> players;
    Player p1 = Player(pos, (uint16_t)30, (uint8_t)2);
    players.push_back(p1);
    pos = Vector2((uint16_t)0, (uint16_t)400);
    Player p2 = Player(pos, (uint16_t)40, (uint8_t)3);
    players.push_back(p2);
    p1.DebugInfo();
    //std::cout << "Waiting 5 seconds and closing ... \n";
    int i = 0;
    char k = 's';
    while(true)
    {
        XLDisplay& dpy = XLDisplay::display();
        std::list<Player>::iterator it;
        for(it = players.begin(); it != players.end(); ++it)
        {
            Player p = (*it);
        switch (p.Color())
        {
            case 0:
                dpy.set_color(XLDisplay::RED);
                break;
            case 1:
                dpy.set_color(XLDisplay::BROWN);
                break;
            case 2:
                dpy.set_color(XLDisplay::BLUE);
                break;
            case 3:
                dpy.set_color(XLDisplay::GREEN);
                break;
            case 4:
                dpy.set_color(XLDisplay::WHITE);
                break;
            case 5:
                dpy.set_color(XLDisplay::BLACK);
                break;
            case 6:
                dpy.set_color(XLDisplay::RED);
                break;                   
            default:
                break;
        }
        
        dpy.circle(p.Position().x + i, p.Position().y, p.Size_());
        }

        i++;
        if(i >= 720) i = 0;
        dpy.flush();

        //k = dpy.wait_key();

        usleep(500);

        dpy.clear();
    }
    //sleep(5);
    std::cout << "Closing ... \n";
    return 0;
}