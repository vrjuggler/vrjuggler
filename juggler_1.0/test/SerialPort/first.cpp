#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <Utils/Port/vjSerialPort.h>
#include <Utils/Port/vjSerialTypes.h>


int
main (int argc, char* argv[]) {
    vjSerialPort* port;

    port = new vjSerialPort(argv[1]);

    port->setOpenReadWrite();
    port->setOpenBlocking();

    if ( port->open() ) {
        char read_buffer[10], write_buffer[10];
        int val;

        std::cerr << "Port opened\n";

        port->setUpdateAction(vjSerialTypes::NOW);
        port->setCharacterSize(8);
        port->enableRead();

        for ( int i = 0; i < 10; i++ ) {
            bzero((void*) &read_buffer, sizeof(read_buffer));
            port->read(read_buffer, sizeof(read_buffer));
            std::cerr << "Read '" << read_buffer << "'\n";

            val = atoi(read_buffer);
            val++;

            bzero((void*) &write_buffer, sizeof(write_buffer));
            sprintf(write_buffer, "%d", val);
            port->write(write_buffer, strlen(write_buffer) + 1);
            std::cerr << "Wrote '" << write_buffer << "'\n";
        }
    }

    return 0;
}
