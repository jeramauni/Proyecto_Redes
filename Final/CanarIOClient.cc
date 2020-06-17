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
            Message em(nick, msg);
            em.type = Message::MESSAGE;

            socket.send(em, socket);
        }
    }
}

void CanarIOClient::net_thread()
{
    while(true)
    {
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        Socket* server_Socket;
        Message message_Server;
        int tmp = socket.recv(message_Server, server_Socket);
        if(tmp != -1)
        {
            std::cout << message_Server.nick << ": " << message_Server.message << "\n";
        }
    }
}