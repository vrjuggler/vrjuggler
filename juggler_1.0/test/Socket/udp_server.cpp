#include <iostream>
#include <Utils/Socket/vjSocketDatagram.h>
#include <Utils/Socket/vjInetAddr.h>


int
main (int argc, char* argv[]) {
    unsigned short port = 5432;

    if ( argc == 2 ) {
        port = (unsigned short) atoi(argv[1]);
    }

    vjSocketDatagram sock(port);
    int status;

    if ( sock.open() && sock.bind() ) {
        char recv_buf[32];
        char send_buf[] = "Hello there!";
        ssize_t bytes;

        while ( 1 ) {
            vjInetAddr addr;

            bytes = sock.recvfrom(recv_buf, sizeof(recv_buf), 0, addr);

            if ( bytes != -1 ) {
                std::cout << "Read '" << recv_buf << "' (" << bytes
                          << " bytes) from " << addr.getAddressString()
                          << std::endl;

                sock.sendto(send_buf, sizeof(send_buf), 0, addr);
            }
        }

        status = 0;
    }
    else {
        status = 1;
    }

    return status;
}
