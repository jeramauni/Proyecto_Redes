#include "Chat.h"
#include <vector>

void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    memcpy(_data, static_cast<void *>(&type), sizeof(uint8_t));
    _data += sizeof(uint8_t);

    memcpy(_data, static_cast<void *>((char*)nick.c_str()), sizeof(char) * 8);
    _data += sizeof(char) * 8;

    memcpy(_data, static_cast<void *>((char*)message.c_str()), sizeof(char) * 8);
    _data += sizeof(char) * 80;

    _data -= MESSAGE_SIZE;
}

int ChatMessage::from_bin(char * bobj)
{

    memcpy(static_cast<void *>(&type), bobj, sizeof(uint8_t));
    bobj += sizeof(uint8_t);

    char _nick [8];
    memcpy(static_cast<void *>(&_nick), bobj, sizeof(char) * 8);
    nick = _nick; 
    bobj += sizeof(char) * 8;
    
    char _message [80];
    memcpy(static_cast<void *>(&_message), bobj, sizeof(char) *80);
    message = _message; 
    bobj += sizeof(char) * 80;

    bobj -= MESSAGE_SIZE;
    return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::do_messages()
{
    XLDisplay::init(720, 480, "CanarIO-server");
    XLDisplay& dpy = XLDisplay::display();
    while (true)
    {
        
        Socket* client_Socket;
        ChatMessage message_Client;

        std::cout << "Cantidad de usuarios: " << clients.size() << "\n";

        int err = socket.recv(message_Client, client_Socket);

        bool is_old_user = false;
        auto client_position = clients.begin();
        auto it = clients.begin();
        //auto client_Player_Position = clients_player.begin();
        //auto it_p = clients_player.begin();
        while( it != clients.end() && !is_old_user)
        {
            std::cout << "123123d \n";
            if(*(*it) == *client_Socket)
            {
                std::cout << "Csdasd \n";
                is_old_user = true;
                client_position = it;
                //client_Player_Position = it_p;
            }
            std::cout << "1001213asd \n";
            //it_p++;
            it++;
        }

        if(err != -1)
        {
            switch (message_Client.type)
            {
                case ChatMessage::LOGIN:
                {
                    if(is_old_user)
                    {
                        std::cout << "LOGIN FAIL: Client " << message_Client.nick <<  " is already logged\n";
                    }
                    else {
                        Vector2 pos = Vector2((uint16_t)0, (uint16_t)200 * clients_player.size());
                        Player* new_Player = new Player(pos, (uint16_t)30, (uint8_t)2);
                        clients.push_back(client_Socket);
                        //clients_player.push_back(new_Player);
                        std::cout << "New client " << message_Client.nick <<  " logged succesfully\n";
                    }
                    break;
                }   
                case ChatMessage::LOGOUT:
                {
                    if(is_old_user)
                    {
                        delete *client_position;
                        //delete *client_Player_Position;
                        clients.erase(client_position);
                        //clients_player.erase(client_Player_Position);
                        std::cout << "Client " << message_Client.nick <<  " logged out succesfully\n";
                    }
                    else
                    {
                        std::cout << "LOGOUT FAIL: Client " << message_Client.nick <<  " is not logged \n";
                    }
                    break;
                }   

                case ChatMessage::MESSAGE:
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
        /*
        if(clients_player.size() > 0)
        {
            for(auto it = clients_player.begin(); it != clients_player.end(); ++it)
            {
                Player* p = (*it);
                switch (p->Color())
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
                dpy.circle(p->Position().x, p->Position().y, p->Size_());
            }
        }*/
        //dpy.flush();
    }
}

void ChatClient::login()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    int err = socket.send(em, socket);
    if (err == -1) {
        std::cout << "login send error \n";
    }
    std::cout << "send succeed: " << err << '\n';

}

void ChatClient::logout()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGOUT;

    socket.send(em, socket);
}

void ChatClient::input_thread()
{
    bool inChat = true;
    while (inChat)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
        std::string msg;
        std::getline(std::cin, msg);

        if(msg == "exit" || msg == "logout")
        {
            inChat = false;
            logout();
        } 
        else
        {
            ChatMessage em(nick, msg);
            em.type = ChatMessage::MESSAGE;

            socket.send(em, socket);
        }
    }
}

void ChatClient::net_thread()
{
    while(true)
    {
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        Socket* server_Socket;
        ChatMessage message_Server;
        int tmp = socket.recv(message_Server, server_Socket);
        if(tmp != -1)
        {
            std::cout << message_Server.nick << ": " << message_Server.message << "\n";
        }
    }
}