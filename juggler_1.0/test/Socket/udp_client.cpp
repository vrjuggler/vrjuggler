#include <stdio.h>
#include <VPR/IO/Socket/vjSocketDatagram.h>


int
main (int argc, char* argv[]) {
    vjSocketDatagram sock(argv[1], (unsigned short) atoi(argv[2]));

    if ( sock.open() ) {
        char buffer[40];

        if ( sock.connect() ) {
            ssize_t bytes;

            bytes = sock.recv(buffer, 40);

            if ( bytes > 0 ) {
                printf("Read %d bytes from server\n", bytes);
                printf("    Got '%s'\n", buffer);
            }

            sock.close();
        }
    }
}
