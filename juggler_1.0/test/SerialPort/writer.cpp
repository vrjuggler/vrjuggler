#include <iostream>
#include <string.h>

#include <Utils/Port/vjSerialPort.h>
#include <Utils/Port/vjSerialTypes.h>


int
main (int argc, char* argv[]) {
    vjSerialPort* write_port;

    write_port = new vjSerialPort(argv[1]);

    write_port->setOpenWriteOnly();
    write_port->setOpenBlocking();

    if ( write_port->open() ) {
        char* buffer = "This is a test...\n";
        ssize_t bytes;

        std::cout << "Port opened\n";
        write_port->setUpdateAction(vjSerialTypes::NOW);
        write_port->setCharacterSize(8);
//        write_port->enableLocalAttach();
        bytes = write_port->write(buffer, strlen(buffer) + 1);
        write_port->flushQueue(vjSerialTypes::IO_QUEUES);
        std::cout << "Wrote " << bytes << " bytes to " << argv[1] << std::endl;
    }

    write_port->close();

    return 0;
}
