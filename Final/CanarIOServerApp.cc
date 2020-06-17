#include <unistd.h>
#include <signal.h>
#include "CanarIOServer.h"

int main(int argc, char **argv)
{
    sigset_t waitset;
    int      sig;

    CanarIOServer es(argv[1], argv[2]);
    
    es.do_messages();

    sigemptyset(&waitset);
    sigaddset(&waitset, SIGQUIT);

    sigwait(&waitset, &sig);

    return 0;
}
