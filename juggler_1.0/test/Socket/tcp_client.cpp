#include <stdio.h>
#include <VPR/IO/Socket/vjSocketStream.h>


int
main (int argc, char* argv[]) {
    vjSocketStream* sock;

    if ( argc == 2 ) {
        sock = new vjSocketStream(argv[1]);
    }
    else if ( argc == 3 ) {
        sock = new vjSocketStream(argv[1], (unsigned short) atoi(argv[2]));
    }
    else {
        return 1;
    }

    if ( sock->open() ) {
        char buffer[40];

        if ( sock->connect() ) {
            ssize_t bytes;

            bytes = sock->recv(buffer, 40);

            if ( bytes > 0 ) {
                printf("Read %d bytes from server\n", bytes);
                printf("    Got '%s'\n", buffer);
            }

            sock->close();
        }
    }

    delete sock;

    return 0;
}
