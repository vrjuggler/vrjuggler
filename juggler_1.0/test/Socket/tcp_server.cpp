#include <stdlib.h>
#include <Utils/Socket/vjSocketStream.h>


int
main (int argc, char* argv[]) {
    unsigned short port = 5432;

    if ( argc == 2 ) {
        port = (unsigned short) atoi(argv[1]);
    }

    vjSocketStream sock(port);
    int status;

    if ( sock.openServer() ) {
        vjSocketStream* client_sock;
        char buffer[] = "Hello there!";

        while ( 1 ) {
            client_sock = sock.accept();
            client_sock->send(buffer, sizeof(buffer));
            delete client_sock;
        }

        status = 0;
    }
    else {
        status = 1;
    }

    return status;
}
