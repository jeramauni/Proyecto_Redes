#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <iterator>

#include "Serializable.h"
#include "Socket.h"
#include "Player.h"

class CanarIOServer
{
public:
    CanarIOServer(const char * s, const char * p);
    ~CanarIOServer();
    //Thread principal del servidor
    void run();
    //Procesamiento mensajes
    void move_msg(bool (&keys) [4], Player* p);
private:
void renderPlayers();
    /**
     *  Lista de clientes conectados al servidor de Chat, representados por
     *  su socket
     */
    std::vector<Player *> feeding;
    std::vector<Player *> clients_player;
    /**
     * Socket del servidor
     */
    Socket socket;
};