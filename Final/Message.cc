#include "Message.h"

void Message::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    memcpy(_data, static_cast<void *>(&type), sizeof(uint8_t));
    _data += sizeof(uint8_t);

    memcpy(_data, static_cast<void *>((char*)nick.c_str()), sizeof(char) * 8);
    _data += sizeof(char) * 8;

    memcpy(_data, static_cast<void *>((char*)message.c_str()), sizeof(char) * 200);
    _data += sizeof(char) * 200;

    _data -= MESSAGE_SIZE;
}

int Message::from_bin(char * bobj)
{

    memcpy(static_cast<void *>(&type), bobj, sizeof(uint8_t));
    bobj += sizeof(uint8_t);

    char _nick [8];
    memcpy(static_cast<void *>(&_nick), bobj, sizeof(char) * 8);
    nick = _nick; 
    bobj += sizeof(char) * 8;
    
    char _message [200];
    memcpy(static_cast<void *>(&_message), bobj, sizeof(char) * 200);
    message = _message; 
    bobj += sizeof(char) * 200;

    bobj -= MESSAGE_SIZE;
    return 0;
}
