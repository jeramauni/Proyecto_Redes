#include "CanarIOServer.h"
#include "Message.h"

CanarIOServer::CanarIOServer(const char * s, const char * p): socket(s, p)
{
    socket.bind();
}

CanarIOServer::~CanarIOServer()
{
    clients.clear();
    clients_player.clear();
}

void CanarIOServer::do_messages()
{
    XLDisplay::init(720, 480, "CanarIO-server");
    XLDisplay *dpy;
    while (true)
    {
        
        Socket* client_Socket;
        Message message_Client;

        std::cout << "Cantidad de usuarios: " << clients_player.size() << "\n";

        int err = socket.recv(message_Client, client_Socket);

        bool is_old_user = false;
        auto client_position = clients.begin();
        auto it = clients.begin();
        auto client_Player_Position = clients_player.begin();
        auto it_p = clients_player.begin();
        while( it != clients.end() && !is_old_user)
        {
            if(*(*it) == *client_Socket)
            {
                is_old_user = true;
                client_position = it;
                client_Player_Position = it_p;
            }
            it_p++;
            it++;
        }

        if(err != -1)
        {
            switch (message_Client.type)
            {
                case Message::LOGIN:
                {
                    if(is_old_user)
                    {
                        std::cout << "LOGIN FAIL: Client " << message_Client.nick <<  " is already logged\n";
                    }
                    else {
                        Vector2 pos = Vector2((uint16_t)0, (uint16_t)200 * clients_player.size());
                        Player* new_Player = new Player(pos, (uint16_t)30, (uint8_t)2);
                        clients.push_back(client_Socket);
                        clients_player.push_back(new_Player);
                        std::cout << "New client " << message_Client.nick <<  " logged succesfully\n";
                    }
                    break;
                }   
                case Message::LOGOUT:
                {
                    if(is_old_user)
                    {
                        delete *client_position;
                        delete *client_Player_Position;
                        clients.erase(client_position);
                        clients_player.erase(client_Player_Position);
                        std::cout << "Client " << message_Client.nick <<  " logged out succesfully\n";
                    }
                    else
                    {
                        std::cout << "LOGOUT FAIL: Client " << message_Client.nick <<  " is not logged \n";
                    }
                    break;
                }   

                case Message::MESSAGE:
                {
                    if(is_old_user)
                    {
                        std::cout << "Is not an old user \n";
                        for(auto it = clients.begin(); it != clients.end(); it++)
                        {
                            if(!(*(*it) == *client_Socket)) 
                            {
                                socket.send(message_Client, *(*it));
                            }
                        }
                        std::cout << "Client " << message_Client.nick <<  " says "<< message_Client.message <<" \n";
                    }
                    else
                    {
                        std::cout << "MESSAGE FAIL: Client " << message_Client.nick <<  " is not logged \n";
                    }
                    break;
                }  

                default:
                std::cout << "Default \n";
                    break;
            }
        }
        if(clients_player.size() > 0)
        {
            for(auto it = clients_player.begin(); it != clients_player.end(); ++it)
            {
                Player* p = (*it);
                switch (p->Color())
                {
                    case 0:
                        dpy->set_color(XLDisplay::RED);
                        break;
                    case 1:
                        dpy->set_color(XLDisplay::BROWN);
                        break;
                    case 2:
                        dpy->set_color(XLDisplay::BLUE);
                        break;
                    case 3:
                        dpy->set_color(XLDisplay::GREEN);
                        break;
                    case 4:
                        dpy->set_color(XLDisplay::WHITE);
                        break;
                    case 5:
                        dpy->set_color(XLDisplay::BLACK);
                        break;
                    case 6:
                        dpy->set_color(XLDisplay::RED);
                        break;                   
                    default:
                        break;
                }
                dpy->circle(p->Position().x, p->Position().y, p->Size_());
            }
        }
        dpy->flush();
        dpy->clear();
    }
}