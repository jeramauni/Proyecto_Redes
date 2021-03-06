#include "Socket.h"

#include <string.h>
#include "Serializable.h"

Socket::Socket(const char * address, const char * port):sd(-1)
{
    struct addrinfo hints;
    struct addrinfo * res;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int rc = getaddrinfo(address, port, &hints, &res);

    if ( rc != 0 )
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
    }

    // res contiene la representación como sockaddr de dirección + puerto

    sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    sa = *res->ai_addr;
    sa_len = res->ai_addrlen;
    
    freeaddrinfo(res);
}

int Socket::recv(Serializable &obj, Socket **sock)
{
    struct sockaddr sa;
    socklen_t sa_len = sizeof(struct sockaddr);

    char buffer[MAX_MESSAGE_SIZE];

    ssize_t bytes = ::recvfrom(sd, buffer, MAX_MESSAGE_SIZE, 0, &sa, &sa_len);

    if ( bytes <= 0 )
    {
        return -1;
    }

    if ( sock != nullptr )
    {
        *sock = new Socket(&sa, sa_len);
    }

    obj.from_bin(buffer);

    return 0;
}

int Socket::send(Serializable& obj, const Socket& sock)
{
    obj.to_bin();
    int bytes = sendto(sd, obj.data(), obj.size(), 0, &sock.sa, sock.sa_len);
    if(bytes < 0) 
    {
        return -1;
    }
    else  
    {
        return 0;
    }
}

bool operator== (const Socket &s1, const Socket &s2)
{
    //Comparar los campos sin_family, sin_addr.s_addr y sin_port
    //de la estructura sockaddr_in de los Sockets s1 y s2
    //Retornar false si alguno difiere
    struct sockaddr_in * socket_f = (struct sockaddr_in *) &s1.sa;
    struct sockaddr_in * socket_s = (struct sockaddr_in *) &s2.sa;
    
    if(socket_f->sin_family == socket_s->sin_family)
    {
        if(socket_f->sin_addr.s_addr == socket_s->sin_addr.s_addr)
        {
            if(socket_f->sin_port == socket_s->sin_port)
            {
                return true;
            }
        }
    }
    return false;
};

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;

    return os;
};
