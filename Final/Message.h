#pragma once
#include "Serializable.h"
#include <stdexcept>
#include<string.h>

/**
 *  Mensaje del protocolo de la aplicación de Chat
 *
 *  +-------------------+
 *  | Tipo: uint8_t     | 0 (login), 1 (mensaje), 2 (logout)
 *  +-------------------+
 *  | Nick: char[8]     | Nick incluido el char terminación de cadena '\0'
 *  +-------------------+
 *  |                   |
 *  | Mensaje: char[80] | Mensaje incluido el char terminación de cadena '\0'
 *  |                   |
 *  +-------------------+
 *
 */

class Message : public Serializable
{
public:
    static const size_t MESSAGE_SIZE = sizeof(char) * 208 + sizeof(uint8_t);

    enum MessageType
    {
        LOGIN   = 0,
        MOVE = 1,
        LOGOUT  = 2,
        GAMEOVER = 3,
        DRAWPLAYER = 4,
        RENDERCALL = 5,
    };

    Message(){};
    
    Message(const std::string& n, const std::string& m):nick(n),message(m){};

    void to_bin();

    int from_bin(char * bobj);

    uint8_t type;

    std::string nick;
    std::string message;
};