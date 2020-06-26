#include <unistd.h>
#include "CanarIOClient.h"
#include <string>

extern "C" void * _client_thread(void *arg)
{
    CanarIOClient * server = static_cast<CanarIOClient *>(arg);

    server->net_thread();

    return 0;
}

int main(int argc, char **argv)
{
    std::string nick = argv[3];
    if (nick.size() < 8) {
            
    XLDisplay::init(720, 480, "CanarIO-client");
    CanarIOClient ec(argv[1], argv[2], argv[3]);

    pthread_attr_t attr;
    pthread_t id;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&id, &attr, _client_thread, static_cast<void *>(&ec));
    
    std::cout << "Logging in...\n";
    ec.login();

    ec.input_thread();
    } else {
        std::cout << "Please, use a nickname with 7 or less characters\n";
    }
}

