#include"CanarIOClient.h"
#include "Message.h"

const Vector2 WINDOW_SIZE = Vector2(720, 480);

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

        if(msg == "q")
        {
            live = false;
        }
        else
        {
            Message em(nick, msg);
            em.type = Message::MOVE;

            socket.send(em, socket);
        }     
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
}

void CanarIOClient::net_thread()
{
    bool endGame = false;
    Socket* server_Socket = nullptr;
    Message message_Server;   
    while(true)
    {
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"   
        if(!endGame) int tmp = socket.recv(message_Server, &server_Socket);
        switch (message_Server.type)
        {
            case Message::DRAWPLAYER:
            {
                Vector2 position_;
                uint16_t size_;
                XLDisplay::XLColor color_;
                parseDraw(position_, size_, color_, message_Server.message);  
                
                dpy.set_color(color_);
                dpy.circle(position_.x, position_.y, size_);
                if(color_ != XLDisplay::XLColor::GREEN)
                {
                    dpy.set_color(XLDisplay::BLACK);
                    dpy.text(position_.x - (message_Server.nick.size() * 5), position_.y + ((size_/2)-15), message_Server.nick);
                    dpy.text(position_.x - 10, position_.y + (size_/2), std::to_string(size_));
                }
                
            }
            break;
            case Message::RENDERCALL:
            {
                dpy.flush();
                dpy.clear();
            }
            break;
            case Message::WIN:
            {   
                dpy.set_color(XLDisplay::GREEN);
                dpy.circle(WINDOW_SIZE.x / 2, WINDOW_SIZE.y / 2, 300);

                dpy.set_color(XLDisplay::BLACK);
                // W
                XPoint ptsW[] = {{110,95},{160,385},{210,285},{260,385},{310, 95}};
                dpy.lines(ptsW, 5);
                // I
                dpy.line(330, 95, 430, 95);
                dpy.line(370, 95, 370, 385);
                dpy.line(330, 385, 430, 385);
                // N
                XPoint ptsI[] = {{450,385},{450,95},{550,385},{550,95}};
                dpy.lines(ptsI, 4);
                //Mensaje
                dpy.text(290, 450, "-Press Q to exit-");
                endGame = true;
            }
            break;
            case Message::LOSE:
            {    
                dpy.set_color(XLDisplay::RED);
                dpy.circle(360, 240, 300);

                dpy.set_color(XLDisplay::BLACK);
                // L
                dpy.line(110, 95, 110, 385);
                dpy.line(110, 385, 210, 385);
                // O
                XPoint ptsO[] = {{230,95},{230,385},{330,385},{330,95},{230,95}};
                dpy.lines(ptsO, 5);
                // S
                XPoint ptsS[] = {{450,95},{350,95},{350,240},{450,240},{450,385},{350,385}};
                dpy.lines(ptsS, 6);
                // E
                XPoint ptsE[] = {{570,95},{470,95},{470,385},{570,385}};
                dpy.line(470, 240, 570, 240);
                dpy.lines(ptsE, 4);
                //Mensaje
                dpy.text(290, 450, "-Press Q to exit-");
                endGame = true;
            }
            break;
            default:
                break;
        }

    }
}