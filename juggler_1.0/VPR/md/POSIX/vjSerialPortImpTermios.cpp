/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <errno.h>

#include <VPR/md/POSIX/vjSerialPortImpTermios.h>


extern int errno;

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor.  This creates a file handle object connected to the given port
// name and sets the update action to happen immediately.
// ----------------------------------------------------------------------------
vjSerialPortImpTermios::vjSerialPortImpTermios (const std::string& port_name)
    : vjSerialPortImp(port_name), m_handle(NULL)
{
    m_handle = new vjFileHandleUNIX(port_name);
    setUpdateAction(vjSerialTypes::NOW);
}

// ----------------------------------------------------------------------------
// Destructor.  If the file handle is non-NULL, its memory is released.
// ----------------------------------------------------------------------------
vjSerialPortImpTermios::~vjSerialPortImpTermios () {
    if ( m_handle != NULL ) {
        delete m_handle;
    }
}

// ----------------------------------------------------------------------------
// Open the serial port and initialize its flags.
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::open () {
    m_open = m_handle->open();

    // If the serial port could not be opened, print an error message.
    if ( ! m_open ) {
        fprintf(stderr,
                "[vjSerialPortImpTermios] Could not open serial port %s: %s\n",
                m_name.c_str(), strerror(errno));
    }
    // Otherwise, initialize the serial port's flags.
    else {
        struct termios term;

        if ( getAttrs(&term) != -1 ) {
            // Initialize all the flags to 0.
            term.c_iflag = term.c_oflag = term.c_cflag = term.c_lflag = 0;

            // Initialize the minimum buffer size to 1 and the timeout to 0.
            term.c_cc[VMIN]  = 1;
            term.c_cc[VTIME] = 0;

            // If we cannot set the initialized attribute flags on the port,
            // then we are not considered open.
            if ( setAttrs(&term, "Could not initialize flags") == -1 ) {
                m_open = false;
            }
        }
    }

    return m_open;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
vjSerialTypes::UpdateActionOption
vjSerialPortImpTermios::getUpdateAction () {
    vjSerialTypes::UpdateActionOption action;

    switch (m_actions) {
      case TCSANOW:
        action = vjSerialTypes::NOW;
        break;
      case TCSADRAIN:
        action = vjSerialTypes::DRAIN;
        break;
      case TCSAFLUSH:
        action = vjSerialTypes::FLUSH;
        break;
    }

    return action;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void
vjSerialPortImpTermios::setUpdateAction (vjSerialTypes::UpdateActionOption action)
{
    switch (action) {
      case vjSerialTypes::NOW:
        m_actions = TCSANOW;
        break;
      case vjSerialTypes::DRAIN:
        m_actions = TCSADRAIN;
        break;
      case vjSerialTypes::FLUSH:
        m_actions = TCSAFLUSH;
        break;
    }
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getLocalAttachState () {
    return getBit(CLOCAL, vjSerialPortImpTermios::CFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableLocalAttach () {
    return setBit(CLOCAL, vjSerialPortImpTermios::CFLAG, true,
                  "Could not enable local attachment");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableLocalAttach () {
    return setBit(CLOCAL, vjSerialPortImpTermios::CFLAG, true,
                  "Could not disable local attachment");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
unsigned char
vjSerialPortImpTermios::getBufferSize () {
    unsigned char retval;
    struct termios term;

    if ( getAttrs(&term) != 0 ) {
        retval = 0;
    }
    else {
        retval = term.c_cc[VMIN];
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::setBufferSize (const unsigned char size) {
    int retval;
    struct termios term;

    if ( getAttrs(&term) != 0 ) {
        retval = -1;
    }
    else {
        term.c_cc[VMIN] = size;
        retval = setAttrs(&term, "Could not set minimum buffer size");
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
unsigned char
vjSerialPortImpTermios::getTimeout () {
    unsigned char retval;
    struct termios term;

    if ( getAttrs(&term) != 0 ) {
        retval = 0;
    }
    else {
        retval = term.c_cc[VTIME];
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::setTimeout (const unsigned char timeout) {
    int retval;
    struct termios term;

    if ( getAttrs(&term) != 0 ) {
        retval = -1;
    }
    else {
        term.c_cc[VTIME] = timeout;
        retval = setAttrs(&term, "Could not set minimum buffer size");
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
unsigned short
vjSerialPortImpTermios::getCharacterSize () {
    unsigned short retval;
    struct termios term;

    if ( getAttrs(&term) != 0 ) {
        retval = 0;
    }
    else {
        retval = term.c_cflag & CSIZE;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::setCharacterSize (const unsigned short bpb) {
    struct termios term;
    int retval;

    if ( getAttrs(&term) != 0 ) {
        retval = -1;
    }
    else {
        term.c_cflag &= ~CSIZE;	// Zero out the bits

        // Set the character size based on the given bits-per-byte value.
        switch (bpb) {
          // 5 bits/byte.
          case 5:
            term.c_cflag |= CS5;
            break;
          // 6 bits/byte.
          case 6:
            term.c_cflag |= CS6;
            break;
          // 7 bits/byte.
          case 7:
            term.c_cflag |= CS7;
            break;
          // 8 bits/byte.
          case 8:
            term.c_cflag |= CS8;
            break;
          // Unrecognized value!
          default:
            fprintf(stderr,
                    "[vjSerialPortImpTermios] Could not set character size to "
                    "%hu on port %s\n", bpb, m_name.c_str());
            break;
        }

        retval = setAttrs(&term, "Could not set character size");
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getReadState () {
    return getBit(CREAD, vjSerialPortImpTermios::CFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableRead () {
    return setBit(CREAD, vjSerialPortImpTermios::CFLAG, true,
                  "Could not enable reading");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableRead () {
    return setBit(CREAD, vjSerialPortImpTermios::CFLAG, false,
                  "Could not disable reading");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
unsigned short
vjSerialPortImpTermios::getStopBits () {
    struct termios term;
    unsigned short retval;

    if ( getAttrs(&term) != 0 ) {
        retval = 0;
    }
    else {
        switch (term.c_cflag & CSTOPB) {
          case 0:
            retval = 1;
            break;
          case 1:
            retval = 2;
            break;
        }
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::setStopBits (const unsigned short num_bits) {
    struct termios term;
    int retval;

    if ( (retval = getAttrs(&term)) == 0 ) {
        std::string msg;

        switch (num_bits) {
          case 1:
            term.c_cflag &= ~CSTOPB;
            break;
          case 2:
            term.c_cflag |= CSTOPB;
            break;
          default:
            fprintf(stderr,
                    "[vjSerialPortImpTermios] Stop bits may only be set to 1 "
                    "or 2 on port %s\n", m_name.c_str());
            break;
        }

        // Construct the error message to send to setAttrs().
        msg = "Could set not stop bits to ";
        msg += num_bits;

        retval = setAttrs(&term, msg);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getInputEchoState () {
    return getBit(ECHO, vjSerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableInputEcho () {
    return setBit(ECHO, vjSerialPortImpTermios::LFLAG, true,
                  "Could not enable input echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableInputEcho () {
    return setBit(ECHO, vjSerialPortImpTermios::LFLAG, false,
                  "Could not disable input echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getEraseEchoState () {
    return getBit(ECHOE, vjSerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableEraseEcho () {
    return setBit(ECHOE, vjSerialPortImpTermios::LFLAG, true,
                  "Could not enable erase echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableEraseEcho () {
    return setBit(ECHOE, vjSerialPortImpTermios::LFLAG, false,
                  "Could not disable erase echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getKillEchoState () {
    return getBit(ECHOK, vjSerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableKillEcho () {
    return setBit(ECHOK, vjSerialPortImpTermios::LFLAG, true,
                  "Could not enable kill echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableKillEcho () {
    return setBit(ECHOK, vjSerialPortImpTermios::LFLAG, false,
                  "Could not disable kill echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getNewlineEchoState () {
    return getBit(ECHONL, vjSerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableNewlineEcho () {
    return setBit(ECHONL, vjSerialPortImpTermios::LFLAG, true,
           "Could not enable newline echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableNewlineEcho () {
    return setBit(ECHONL, vjSerialPortImpTermios::LFLAG, false,
           "Could not disable newline echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getCanonicalState () {
    return getBit(ICANON, vjSerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableCanonicalInput () {
    return setBit(ICANON, vjSerialPortImpTermios::LFLAG, true,
           "Could not enable canonical input mode");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableCanonicalInput () {
    return setBit(ICANON, vjSerialPortImpTermios::LFLAG, false,
           "Could not disable canonical input mode");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getCRTranslateState () {
    return getBit(ICRNL, vjSerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableCRTranslation () {
    return setBit(ICRNL, vjSerialPortImpTermios::IFLAG, true,
                  "Could not enable carriage return translation mode");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableCRTranslation () {
    return setBit(ICRNL, vjSerialPortImpTermios::IFLAG, false,
                  "Could not disable carriage return translation mode");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getExtendedHandlingState () {
    return getBit(IEXTEN, vjSerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableExtendedHandling () {
    return setBit(IEXTEN, vjSerialPortImpTermios::LFLAG, true,
                  "Could not enable extended character handling mode");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableExtendedHandling () {
    return setBit(IEXTEN, vjSerialPortImpTermios::LFLAG, false,
                  "Could not disable extended character handling mode");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getCRIgnoreState () {
    return getBit(IGNCR, vjSerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableCRIgnore () {
    return setBit(IGNCR, vjSerialPortImpTermios::IFLAG, true,
                  "Could not enable carriage return ignoring");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableCRIgnore () {
    return setBit(IGNCR, vjSerialPortImpTermios::IFLAG, false,
                  "Could not disable carriage return ignoring");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getBadByteIgnoreState () {
    return getBit(IGNPAR, vjSerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableBadByteIgnore () {
    return setBit(IGNPAR, vjSerialPortImpTermios::IFLAG, true,
                  "Could not enable bad byte ignoring");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableBadByteIgnore () {
    return setBit(IGNPAR, vjSerialPortImpTermios::IFLAG, false,
                  "Could not disable bad byte ignoring");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getParityCheckState () {
    return getBit(IGNPAR, vjSerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableParityCheck () {
    return setBit(INPCK, vjSerialPortImpTermios::IFLAG, true,
                  "Could not enable parity checking");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableParityCheck () {
    return setBit(INPCK, vjSerialPortImpTermios::IFLAG, false,
                  "Could not disable parity checking");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getSignalGenerateState () {
    return getBit(ISIG, vjSerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableSignalGeneration () {
    return setBit(ISIG, vjSerialPortImpTermios::LFLAG, true,
                  "Could not enable signal generation for input characters");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableSignalGeneration () {
    return setBit(ISIG, vjSerialPortImpTermios::LFLAG, false,
                  "Could not disable signal generation for input characters");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getBitStripState () {
    return getBit(ISTRIP, vjSerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableBitStripping () {
    return setBit(ISTRIP, vjSerialPortImpTermios::IFLAG, true,
                  "Could not enable bit stripping from eight to seven bits");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableBitStripping () {
    return setBit(ISTRIP, vjSerialPortImpTermios::IFLAG, false,
                  "Could not disable bit stripping");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getStartStopInputState () {
    return getBit(IXOFF, vjSerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableStartStopInput () {
    return setBit(IXOFF, vjSerialPortImpTermios::IFLAG, true,
                  "Could not enable start-stop input control");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableStartStopInput () {
    return setBit(IXOFF, vjSerialPortImpTermios::IFLAG, false,
                  "Could not disable start-stop input control");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getStartStopOutputState () {
    return getBit(IXON, vjSerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableStartStopOutput () {
    return setBit(IXON, vjSerialPortImpTermios::IFLAG, true,
                  "Could not enable start-stop output control");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableStartStopOutput () {
    return setBit(IXON, vjSerialPortImpTermios::IFLAG, false,
                  "Could not disable start-stop output control");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getNoFlushState () {
    return getBit(NOFLSH, vjSerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableNoFlush () {
    return setBit(NOFLSH, vjSerialPortImpTermios::LFLAG, true,
                  "Could not enable no flushing at signal generation");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableNoFlush () {
    return setBit(NOFLSH, vjSerialPortImpTermios::LFLAG, false,
                  "Could not disable no flushing at signal generation");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getParityGenerationState () {
    return getBit(PARENB, vjSerialPortImpTermios::CFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableParityGeneration () {
    return setBit(PARENB, vjSerialPortImpTermios::CFLAG, true,
                  "Could not enable parity generation on outgoing characters");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableParityGeneration () {
    return setBit(PARENB, vjSerialPortImpTermios::CFLAG, false,
                  "Could not disable parity generation on outgoing characters");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getParityErrorMarkingState () {
    return getBit(PARMRK, vjSerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::enableParityErrorMarking () {
    return setBit(PARMRK, vjSerialPortImpTermios::IFLAG, true,
                  "Could not enable parity error marking");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::disableParityErrorMarking () {
    return setBit(PARMRK, vjSerialPortImpTermios::IFLAG, false,
                  "Could not disable parity error marking");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::getParity () {
    int retval;

    // Odd parity if PARODD is set.
    if ( getBit(PARODD, vjSerialPortImpTermios::CFLAG) ) {
        retval = 1;
    }
    // Even parity if PARODD is not set.
    else {
        retval = 0;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::setOddParity () {
    return setBit(PARODD, vjSerialPortImpTermios::CFLAG, true,
                  "Could not set odd parity");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::setEvenParity () {
    return setBit(PARODD, vjSerialPortImpTermios::CFLAG, false,
                  "Could not set even parity");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::getInputBaudRate () {
    struct termios term;
    int retval;

    if ( (retval = getAttrs(&term)) == 0 ) {
        speed_t baud_rate;

        baud_rate = cfgetispeed(&term);
        retval = baudToInt(baud_rate);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::setInputBaudRate (const int baud) {
    struct termios term;
    int retval;

    if ( (retval = getAttrs(&term)) == 0 ) {
        speed_t new_rate;

        new_rate = intToBaud(baud);

fprintf(stderr, "Setting input baud rate to %d\n", new_rate);

        if ( cfsetispeed(&term, new_rate) == -1 ) {
            fprintf(stderr,
                    "Failed to set the input baud rate to %u on port %s: %s\n",
                    baud, m_name.c_str(), strerror(errno));
            retval = -1;
        }
        else {
            std::string msg;

            msg     = "Failed to set the input baud rate to ";
            msg    += baud;
            retval  = setAttrs(&term, msg);
        }
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::getOutputBaudRate () {
    struct termios term;
    int retval;

    if ( (retval = getAttrs(&term)) == 0 ) {
        speed_t baud_rate;

        baud_rate = cfgetospeed(&term);
        retval = baudToInt(baud_rate);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::setOutputBaudRate (const int baud) {
    struct termios term;
    int retval;

    if ( (retval = getAttrs(&term)) == 0 ) {
        speed_t new_rate;

        new_rate = intToBaud(baud);

fprintf(stderr, "Setting output baud rate to %d\n", new_rate);

        if ( cfsetospeed(&term, new_rate) == -1 ) {
            fprintf(stderr,
                    "Failed to set the output baud rate to %u on port %s: %s\n",
                    baud, m_name.c_str(), strerror(errno));
            retval = -1;
        }
        else {
            std::string msg;

            msg     = "Failed to set the output baud rate to ";
            msg    += baud;
            retval  = setAttrs(&term, msg);
        }
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::drainOutput () {
    int retval;

    retval = tcdrain(m_handle->m_fdesc);

    if ( retval == -1 ) {
        fprintf(stderr,
                "[vjSerialPortImpTermios] Failed to drain output on port %s: %s\n",
                m_name.c_str(), strerror(errno));
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::controlFlow (vjSerialTypes::FlowActionOption opt) {
    int action, retval;

    switch (opt) {
      case vjSerialTypes::OUTPUT_OFF:
        action = TCOOFF;
        break;
      case vjSerialTypes::OUTPUT_ON:
        action = TCOON;
        break;
      case vjSerialTypes::INPUT_OFF:
        action = TCIOFF;
        break;
      case vjSerialTypes::INPUT_ON:
        action = TCION;
        break;
    }

    retval = tcflow(m_handle->m_fdesc, action);

    if ( retval == -1 ) {
        fprintf(stderr,
                "[vjSerialPortImpTermios] Failed to alter flow control on "
                "port %s: %s\n", m_name.c_str(), strerror(errno));
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::flushQueue (vjSerialTypes::FlushQueueOption opt) {
    int queue, retval;

    switch (opt) {
      case vjSerialTypes::INPUT_QUEUE:
        queue = TCIFLUSH;
        break;
      case vjSerialTypes::OUTPUT_QUEUE:
        queue = TCOFLUSH;
        break;
      case vjSerialTypes::IO_QUEUES:
        queue = TCIOFLUSH;
        break;
    }

    retval = tcflush(m_handle->m_fdesc, queue);

    if ( retval == -1 ) {
        std::string queue_name;

        switch (opt) {
          case vjSerialTypes::INPUT_QUEUE:
            queue_name = "input queue";
            break;
          case vjSerialTypes::OUTPUT_QUEUE:
            queue_name = "output queue";
            break;
          case vjSerialTypes::IO_QUEUES:
            queue_name = "input and output queues";
            break;
        }

        fprintf(stderr,
                "[vjSerialPortImpTermios] Failed to flush %s on port %s: %s\n",
                queue_name.c_str(), m_name.c_str(), strerror(errno));
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::sendBreak (const int duration) {
    int retval;

    retval = tcsendbreak(m_handle->m_fdesc, duration);

    if ( retval == -1 ) {
        fprintf(stderr,
                "[vjSerialPortImpTermios] Failed to send break on port %s: %s\n",
                m_name.c_str(), strerror(errno));
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void
vjSerialPortImpTermios::setControlCharacter (const unsigned int index,
                                          const unsigned char value)
{
    struct termios term;

    if ( getAttrs(&term) == 0 ) {
        if ( index < NCCS ) {
            std::string msg;

            term.c_cc[index] = value;

            msg  = "Could not set control character ";
            msg += index;
            msg += " to ";
            msg += value;
            setAttrs(&term, msg);
        }
        else {
            fprintf(stderr,
                    "[vjSerialPortImpTermios] Index %u too large for control "
                    "character array\n", index);
        }
    }
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
unsigned char
vjSerialPortImpTermios::getControlCharacter (const unsigned int index) {
    struct termios term;
    unsigned char retval;

    retval = 0;

    if ( getAttrs(&term) != -1 ) {
        if ( index < NCCS ) {
            retval = term.c_cc[index];
        }
        else {
            fprintf(stderr,
                    "[vjSerialPortImpTermios] Index %u too large for control "
                    "character array\n", index);
        }
    }

    return retval;
}

// ============================================================================
// Protected methods.
// ============================================================================

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::getAttrs (struct termios* term) {
    int retval;

    retval = tcgetattr(m_handle->m_fdesc, term);

    if ( retval == -1 ) {
        fprintf(stderr,
                "[vjSerialPortImpTermios] Could not get attributes for port %s: %s\n",
                m_name.c_str(), strerror(errno));
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::setAttrs (struct termios* term, const char* err_msg,
                               const bool print_sys_err)
{
    int retval;

fprintf(stderr, "[vjSerialPortImpTermios] term->c_iflag: %d\n", term->c_iflag);
fprintf(stderr, "[vjSerialPortImpTermios] term->c_oflag: %d\n", term->c_oflag);
fprintf(stderr, "[vjSerialPortImpTermios] term->c_cflag: %d\n", term->c_cflag);
fprintf(stderr, "[vjSerialPortImpTermios] term->c_lflag: %d\n", term->c_lflag);
fprintf(stderr, "[vjSerialPortImpTermios] term->c_ispeed: %d\n",
        cfgetispeed(term));
fprintf(stderr, "[vjSerialPortImpTermios] term->c_ospeed: %d\n\n",
        cfgetospeed(term));
fprintf(stderr, "[vjSerialPortImpTermios] term->c_cc[]:");
for ( int i = 0; i < NCCS; i++ ) {
    fprintf(stderr, " %u", (unsigned int) term->c_cc[i]);
}
fprintf(stderr, "\n");

    retval = tcsetattr(m_handle->m_fdesc, m_actions, term);

    if ( retval == -1 ) {
        fprintf(stderr, "[vjSerialPortImpTermios] %s (port '%s')", err_msg,
                m_name.c_str());

        if ( print_sys_err ) {
            fprintf(stderr, ": %s", strerror(errno));
        }

        fprintf(stderr, "\n");
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::setAttrs (struct termios* term,
                               const std::string& err_msg,
                               const bool print_sys_err)
{
    return setAttrs(term, err_msg.c_str(), print_sys_err);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
vjSerialPortImpTermios::getBit (const tcflag_t bit,
                             vjSerialPortImpTermios::_term_flag flag)
{
    struct termios term;
    bool retval;

    if ( getAttrs(&term) == -1 ) {
        retval = false;
    }
    else {
        switch (flag) {
          case IFLAG:
            retval = ((term.c_iflag & bit) != 0);
          case OFLAG:
            retval = ((term.c_oflag & bit) != 0);
          case CFLAG:
            retval = ((term.c_cflag & bit) != 0);
          case LFLAG:
            retval = ((term.c_lflag & bit) != 0);
        }
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::setBit (const tcflag_t bit,
                             vjSerialPortImpTermios::_term_flag flag,
                             const bool enable,
                             const std::string& err_msg,
                             const bool print_sys_err)
{
    struct termios term;
    int retval;

    if ( (retval = getAttrs(&term)) == 0 ) {
        // Set the bit for this port.
        if ( enable ) {
            switch (flag) {
              case IFLAG:
                term.c_iflag |= bit;
                break;
              case OFLAG:
                term.c_oflag |= bit;
                break;
              case CFLAG:
                term.c_cflag |= bit;
                break;
              case LFLAG:
                term.c_lflag |= bit;
                break;
            }
        }
        // Reset the bit for this port.
        else {
            switch (flag) {
              case IFLAG:
                term.c_iflag &= ~bit;
                break;
              case OFLAG:
                term.c_oflag &= ~bit;
                break;
              case CFLAG:
                term.c_cflag &= ~bit;
                break;
              case LFLAG:
                term.c_lflag &= ~bit;
                break;
            }
        }

        retval = setAttrs(&term, err_msg, print_sys_err);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
vjSerialPortImpTermios::baudToInt (const speed_t baud_rate) {
    int retval;

    switch (baud_rate) {
      case B50:
        retval = 50;
        break;
      case B75:
        retval = 75;
        break;
      case B110:
        retval = 110;
        break;
      case B134:
        retval = 134;
        break;
      case B150:
        retval = 150;
        break;
      case B200:
        retval = 200;
        break;
      case B300:
        retval = 300;
        break;
      case B600:
        retval = 600;
        break;
      case B1200:
        retval = 1200;
        break;
      case B1800:
        retval = 1800;
        break;
      case B2400:
        retval = 2400;
        break;
      case B4800:
        retval = 4800;
        break;
      case B9600:
        retval = 9600;
        break;
      case B19200:
        retval = 19200;
        break;
      case B38400:
        retval = 38400;
        break;
      default:
        retval = baud_rate;
        break;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
speed_t
vjSerialPortImpTermios::intToBaud (const int speed_int) {
    speed_t rate;

    switch (speed_int) {
      case 50:
        rate = B50;
        break;
      case 75:
        rate = B75;
        break;
      case 110:
        rate = B110;
        break;
      case 134:
        rate = B134;
        break;
      case 150:
        rate = B150;
        break;
      case 200:
        rate = B200;
        break;
      case 300:
        rate = B300;
        break;
      case 600:
        rate = B600;
        break;
      case 1200:
        rate = B1200;
        break;
      case 1800:
        rate = B1800;
        break;
      case 2400:
        rate = B2400;
        break;
      case 4800:
        rate = B4800;
        break;
      case 9600:
        rate = B9600;
        break;
      case 19200:
        rate = B19200;
        break;
      case 38400:
        rate = B38400;
        break;
      default:
        rate = speed_int;
        break;
    }

    return rate;
}
