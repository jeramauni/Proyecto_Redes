#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "Serializable.h"
#include "Socket.h"
#include "Player.h"

class CanarIOServer
{
public:
    CanarIOServer(const char * s, const char * p);
    ~CanarIOServer();
    /**
     *  Thread principal del servidor recive mensajes en el socket y
     *  lo distribuye a los clientes. Mantiene actualizada la lista de clientes
     */
    void do_messages();

private:
    /**
     *  Lista de clientes conectados al servidor de Chat, representados por
     *  su socket
     */
    std::vector<Socket *> clients;
    std::vector<Player *> clients_player;
    /**
     * Socket del servidor
     */
    Socket socket;
};