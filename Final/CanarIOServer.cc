#include "CanarIOServer.h"
#include "Message.h"

CanarIOServer::CanarIOServer(const char * s, const char * p): socket(s, p)
{
    socket.bind();
}

CanarIOServer::~CanarIOServer()
{
    feeding.clear();
    clients_player.clear();
}

//Movimiento de jugador-------------------------------------------------------------------------------
void CanarIOServer::move_msg(char key, Player* p)
{
    int player_velocity = (p)->velocity();
    Vector2 player_position = (p)->position();
    switch (key)
    {
     case 'w':
    case 'W':
        if(player_position.y - p->size() >= 0) player_position.y -= player_velocity;
        (p)->Move(player_position);
        break;
    case 's':
    case 'S':
        if(player_position.y + p->size() <= 480) player_position.y += player_velocity;
        (p)->Move(player_position);
        break;
     case 'a':
    case 'A':
        if(player_position.x - p->size() >= 0) player_position.x -= player_velocity;
        (p)->Move(player_position);
        break;
    case 'd':
    case 'D':
        if(player_position.x + p->size() <= 720) player_position.x += player_velocity;
        (p)->Move(player_position);
        break;
    default:
        break;
    }
}

//Actualiza la pantalla de los jugadores
void CanarIOServer::renderPlayers()
{
    for (auto it = clients_player.begin(); it != clients_player.end(); it++)
    {
        for (auto it3 = feeding.begin(); it3 != feeding.end(); it3++)
        {
            Vector2 pos = (*it3)->position();
            uint16_t size = (*it3)->size();
            XLDisplay::XLColor color = (*it3)->color();
            std::string s;
            s = std::to_string(pos.x) + '-' + std::to_string(pos.y) + '-' + std::to_string(size) + '-' + std::to_string(color);
            Message drawPlayer((*it)->nickname(), s);
            drawPlayer.type = Message::DRAWPLAYER;
            int t = socket.send(drawPlayer, *((*it)->socket()));
        }
        for (auto it2 = clients_player.begin(); it2 != clients_player.end(); it2++)
        {
            Vector2 pos = (*it2)->position();
            uint16_t size = (*it2)->size();
            XLDisplay::XLColor color = (*it2)->color();
            std::string s;
            s = std::to_string(pos.x) + '-' + std::to_string(pos.y) + '-' + std::to_string(size) + '-' + std::to_string(color);
            Message drawPlayer((*it)->nickname(), s);
            drawPlayer.type = Message::DRAWPLAYER;
            socket.send(drawPlayer, *((*it)->socket()));
        }
        Message renderPlayers((*it)->nickname(), " ");
        renderPlayers.type = Message::RENDERCALL;
        socket.send(renderPlayers, *((*it)->socket()));
    }
}

//Principal-------------------------------------------------------------------------------
void CanarIOServer::run()
{
    XLDisplay::init(720, 480, "CanarIO-server");
    XLDisplay *dpy;
    for(int i = 0; i < 20; i++)
    {
        //uint16_t food_size = std::rand() % 40 + 5;
        Vector2 food_pos = Vector2(std::rand() % (720 - 3*2) + 3, 
                                   std::rand() % (480 - 3*2) + 3);
        Player* food = new Player(food_pos, 3);
        feeding.push_back(food);
        food->Update(dpy);
    }
    dpy->flush();
    dpy->clear();
    while (true)
    {
        
        Socket* client_Socket = nullptr;
        Message message_Client;

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
                        Vector2 pos = Vector2((uint16_t)100, (uint16_t)200 * clients_player.size());
                        int random = std::rand() % 7;
                        if(random == 5) random++;
                        int random_Size = 20;
                        Player* new_Player = new Player(pos, (uint16_t)random_Size, (XLDisplay::XLColor)random, client_Socket, message_Client.nick);
                        clients_player.push_back(new_Player);
                        std::cout << "New client " << message_Client.nick <<  " logged succesfully\n";
                        std::cout << "Cantidad de usuarios: " << clients_player.size() << "\n";

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
                        std::cout << "Cantidad de usuarios: " << clients_player.size() << "\n";
                    }
                    else
                    {
                        std::cout << "LOGOUT FAIL: Client " << message_Client.nick <<  " is not logged \n";
                    }
                    break;
                }   

                case Message::MOVE:
                {
                    if(is_old_user)
                    {
                        char key = message_Client.message[0];
                        move_msg(key, *client_Player_Position);
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
                                //Si entra significa que ha perdido
                                dead_Players.push_back(it);
                                (*colliding)->ChangeSize(p->size() + (*colliding)->size());
                            }
                        }
                    }
                }
                for(auto food = feeding.begin(); food != feeding.end(); food++)
                {
                    if((*food)->IsColliding(p))
                    {
                        p->ChangeSize(p->size() + (*food)->size());
                        Vector2 food_pos = Vector2(std::rand() % (720 - 3*2) + 3, 
                                                    std::rand() % (480 - 3*2) + 3);
                        (*food)->Move(food_pos);
                    }
                }
                for(auto food = feeding.begin(); food != feeding.end(); food++)
                {
                    (*food)->Update(dpy);
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
            renderPlayers();
        }
        dpy->flush();
        dpy->clear();
    }
}