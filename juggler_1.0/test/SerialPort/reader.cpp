#include <iostream>

#include <VPR/IO/Port/vjSerialPort.h>


int
main (int argc, char* argv[]) {
    vjSerialPort* read_port;

    read_port = new vjSerialPort(argv[1]);

    read_port->setOpenReadOnly();
    read_port->setOpenBlocking();

    if ( read_port->open() ) {
        char buffer[80];

//        read_port->setUpdateAction(vjSerialIO::NOW);
//        read_port->enableLocalAttach();
        read_port->setCharacterSize(8);
        read_port->enableRead();
//        read_port->flushQueue(vjSerialIO::INPUT_QUEUE);
        std::cout << "Port opened\n";
        read_port->read(buffer, sizeof(buffer));
        std::cout << "Read '" << buffer << "'\n";
    }

    return 0;
}
