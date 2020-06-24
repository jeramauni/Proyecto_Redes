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
        
        Socket* client_Socket = nullptr;
        Message message_Client;

        std::cout << "Cantidad de usuarios: " << clients_player.size() << "\n";

        int err = socket.recv(message_Client, &client_Socket);

        bool is_old_user = false;
        auto client_Player_Position = clients_player.begin();
        auto it_p = clients_player.begin();
        while( it_p != clients_player.end() && !is_old_user)
        {
            if(*((*it_p)->socket()) == *client_Socket)
            {
                is_old_user = true;
                client_Player_Position = it_p;
            }
            it_p++;
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
                        int random = std::rand() % 7;
                        int random_Size = std::rand() % 70 + 20;
                        Player* new_Player = new Player(pos, (uint16_t)random_Size, (XLDisplay::XLColor)random, client_Socket, message_Client.nick);
                        clients_player.push_back(new_Player);
                        std::cout << "New client " << message_Client.nick <<  " logged succesfully\n";
                    }
                    break;
                }   
                case Message::LOGOUT:
                {
                    if(is_old_user)
                    {
                        delete *client_Player_Position;
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
                        char key = message_Client.message[0];
                        int player_velocity = (*client_Player_Position)->velocity();
                        Vector2 player_position = (*client_Player_Position)->position();
                        switch (key)
                        {
                            case 'w':
                            case 'W':
                                player_position.y -= player_velocity;
                                (*client_Player_Position)->Move(player_position);                                                                
                                break;
                            case 's':
                            case 'S':
                                player_position.y += player_velocity;
                                (*client_Player_Position)->Move(player_position);                                                                
                                break;
                            case 'a':
                            case 'A':
                                player_position.x -= player_velocity;
                                (*client_Player_Position)->Move(player_position);                                                                
                                break;
                            case 'd':
                            case 'D':
                                player_position.x += player_velocity;
                                (*client_Player_Position)->Move(player_position);                                                                
                                break;
                            default:
                                break;
                        }
                        /*
                        //Futuro renderizado cliente
                        for(auto it = clients.begin(); it != clients.end(); it++)
                        {
                            //Comprobarción para ver si se pasan bien los mensajes
                            socket.send(message_Client, *(*it));
                           
                        } 
                        */
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
        //Update 
       if(clients_player.size() > 0)
        {
            std::vector<std::vector<Player*>::iterator> dead_Players;
            for(auto it = clients_player.begin(); it != clients_player.end(); it++)
            {
                Player* p = (*it);
                //No tiene sentido calcular colliding si solo hay un jugador
                if(clients_player.size() > 1)
                {
                    for(auto colliding = clients_player.begin(); colliding != clients_player.end(); colliding++)
                    {
                        if(!(*(p->socket()) == *((*colliding)->socket())))
                        {
                            if(p->IsColliding(*colliding))
                            {
                                dead_Players.push_back(it);
                                (*colliding)->ChangeSize(p->size() + (*colliding)->size());
                            }
                        }
                    }
                }
                p->Update(dpy);
            }
            
            for(auto dead_player = dead_Players.begin(); dead_player != dead_Players.end(); dead_player++)
            {
                std::vector<Player*>::iterator actual_dead = (*dead_player);
                Message gameOverMsg((*actual_dead)->nickname(), " ");
                gameOverMsg.type = Message::GAMEOVER;
                socket.send(gameOverMsg, *((*actual_dead)->socket()));
                delete *actual_dead;
                clients_player.erase(actual_dead);
            }
        }
        dpy->flush();
        dpy->clear();
    }
}