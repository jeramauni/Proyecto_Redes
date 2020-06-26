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
    CanarIOClient(const char * s, const char * p, const char * n):socket(s, p),
        nick(n), dpy(XLDisplay::display()){};

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

private:
    //Parsea el mensaje de renderizado   
    void parseDraw(Vector2& position_, uint16_t& size_, XLDisplay::XLColor& color_, std::string message);

    XLDisplay &dpy;
  
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

