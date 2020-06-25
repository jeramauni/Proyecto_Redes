#include"CanarIOClient.h"
#include "Message.h"
void CanarIOClient::login()
{
    std::string msg;

    Message em(nick, msg);
    em.type = Message::LOGIN;

    int err = socket.send(em, socket);
    if (err == -1) {
        std::cout << "login send error \n";
    }
    std::cout << "send succeed: " << err << '\n';

}

void CanarIOClient::logout()
{
    std::string msg;

    Message em(nick, msg);
    em.type = Message::LOGOUT;

    socket.send(em, socket);
}

void CanarIOClient::input_thread()
{

    while (live)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
        std::string msg;
        char key = dpy.wait_key();

        msg = key;

        std::cout << msg <<"\n";

        if(msg == "exit" || msg == "logout" || msg == "q")
        {
            live = false;
        }
        else
        {
            Message em(nick, msg);
            em.type = Message::MOVE;

            socket.send(em, socket);
        }
        dpy.flush();
        dpy.clear();
    }
    logout();
}

void CanarIOClient::parseDraw(Vector2& position_, uint16_t& size_, XLDisplay::XLColor& color_, std::string message)
{
    int value = -1;
    int guion = 0;
    for(size_t i = 0; i < message.length(); i++)
    {
        char c = message[i];

        if (c == '-')
        {
            switch (guion)
            {
                case 0:
                    position_.x = (uint16_t)value;
                    break;
                case 1:
                    position_.y = (uint16_t)value;
                    break;
                case 2:
                    size_ = (uint16_t)value;
                    break;
                default:
                std::cerr << "Error on parsing drawing information\n";
                    break;
            }
            value = -1;
            guion++;
        }
        else
        {
            if (value == -1) 
            {
                value = c - '0';
            }
            else
            {
                value = value * 10 + c - '0';

            }
            
        }
    }
    color_= (XLDisplay::XLColor)value;
    std::cout << std::to_string(color_) << '\n';
}

void CanarIOClient::net_thread()
{

    while(true)
    {
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        Socket* server_Socket = nullptr;
        Message message_Server;      
        int tmp = socket.recv(message_Server, &server_Socket);
        switch (message_Server.type)
        {
            case Message::GAMEOVER:
                live = false;
                std::cout << "--GAME OVER-- \n" << message_Server.nick << " killed you \n";
        
                break;
            case Message::DRAWPLAYER:
            {
                Vector2 position_;
                uint16_t size_;
                XLDisplay::XLColor color_;
                parseDraw(position_, size_, color_, message_Server.message);  
                
                dpy.set_color(color_);
                dpy.circle(position_.x, position_.y, size_);
            }
                break;
            case Message::RENDERCALL:
            {    
                dpy.flush();
                dpy.clear();
            }
            default:
                break;
        }

    }
}