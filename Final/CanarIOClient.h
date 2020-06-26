#include <string.h>
#include "Socket.h"
#include "XLDisplay.h"
#include "Vector2.h"

class CanarIOClient
{
public:
    /**
     * @param s dirección del servidor
     * @param p puerto del servidor
     * @param n nick del usuario
     */
    CanarIOClient(const char * s, const char * p, const char * n):socket(s, p), nick(n), dpy(XLDisplay::display())
    {
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 1;
        setsockopt(socket.sd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);       

    }

    /**
     *  Envía el mensaje de login al servidor
     */
    void login();

    /**
     *  Envía el mensaje de logout al servidor
     */
    void logout();

    /**
     *  Rutina principal para el Thread de E/S. Lee datos de STDIN (std::getline)
     *  y los envía por red vía el Socket.
     */
    void input_thread();

    /**
     *  Rutina del thread de Red. Recibe datos de la red y los "renderiza"
     *  en STDOUT
     */
    void net_thread();
    //Update
    void update_thread();

private:
    //Parsea el mensaje de renderizado   
    void parseDraw(Vector2& position_, uint16_t& size_, XLDisplay::XLColor& color_, std::string message);
    //Array de estado de WASD
    bool keys [4] = {1, 1, 1, 1};
    XLDisplay &dpy;
  
    //Actualiza el estado del array keys despues de un nuevo evento de teclado
    void updateKeys(char key);
    /**
     * Socket para comunicar con el servidor
     */
    Socket socket;
    bool live = true;
    /**
     * Nick del usuario
     */
    std::string nick;
};

