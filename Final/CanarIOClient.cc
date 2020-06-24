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
    XLDisplay::init(80, 80, "CanarIO-client");
    XLDisplay *dpy;
    while (live)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
        std::string msg;

        char key = dpy->wait_key();

        msg = key;

        std::cout << msg <<"\n";

        if(msg == "exit" || msg == "logout" || msg == "q")
        {
            live = false;
        }
        else
        {
            Message em(nick, msg);
            em.type = Message::MESSAGE;

            socket.send(em, socket);
        }
        dpy->flush();
        dpy->clear();
    }
    logout();
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
        if(message_Server.type == Message::GAMEOVER)
        {
            live = false;
            std::cout << "--GAME OVER-- \n" << message_Server.nick << " kill you \n";
        }
        else
        {
            if(tmp != -1)
            {
                std::cout << message_Server.nick << ": " << message_Server.message << "\n";
            }
        }
    }
}