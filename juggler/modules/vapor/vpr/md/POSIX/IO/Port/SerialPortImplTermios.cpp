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

#include <md/POSIX/SerialPortImpTermios.h>


extern int errno;

namespace vpr {

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor.  This creates a file handle object connected to the given port
// name and sets the update action to happen immediately.
// ----------------------------------------------------------------------------
SerialPortImpTermios::SerialPortImpTermios (const std::string& port_name)
    : SerialPortImp(port_name), m_handle(NULL)
{
    m_handle = new FileHandleUNIX(port_name);
    setUpdateAction(SerialTypes::NOW);
}

// ----------------------------------------------------------------------------
// Destructor.  If the file handle is non-NULL, its memory is released.
// ----------------------------------------------------------------------------
SerialPortImpTermios::~SerialPortImpTermios () {
    if ( m_handle != NULL ) {
        delete m_handle;
    }
}

// ----------------------------------------------------------------------------
// Open the serial port and initialize its flags.
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::open () {
    m_open = m_handle->open();

    // If the serial port could not be opened, print an error message.
    if ( ! m_open ) {
        fprintf(stderr,
                "[vpr::SerialPortImpTermios] Could not open serial port %s: %s\n",
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
SerialTypes::UpdateActionOption
SerialPortImpTermios::getUpdateAction () {
    SerialTypes::UpdateActionOption action;

    switch (m_actions) {
      case TCSANOW:
        action = SerialTypes::NOW;
        break;
      case TCSADRAIN:
        action = SerialTypes::DRAIN;
        break;
      case TCSAFLUSH:
        action = SerialTypes::FLUSH;
        break;
    }

    return action;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void
SerialPortImpTermios::setUpdateAction (SerialTypes::UpdateActionOption action)
{
    switch (action) {
      case SerialTypes::NOW:
        m_actions = TCSANOW;
        break;
      case SerialTypes::DRAIN:
        m_actions = TCSADRAIN;
        break;
      case SerialTypes::FLUSH:
        m_actions = TCSAFLUSH;
        break;
    }
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getLocalAttachState () {
    return getBit(CLOCAL, SerialPortImpTermios::CFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableLocalAttach () {
    return setBit(CLOCAL, SerialPortImpTermios::CFLAG, true,
                  "Could not enable local attachment");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableLocalAttach () {
    return setBit(CLOCAL, SerialPortImpTermios::CFLAG, true,
                  "Could not disable local attachment");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
unsigned char
SerialPortImpTermios::getBufferSize () {
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
SerialPortImpTermios::setBufferSize (const unsigned char size) {
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
SerialPortImpTermios::getTimeout () {
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
SerialPortImpTermios::setTimeout (const unsigned char timeout) {
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
SerialPortImpTermios::getCharacterSize () {
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
SerialPortImpTermios::setCharacterSize (const unsigned short bpb) {
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
                    "[vpr::SerialPortImpTermios] Could not set character size "
                    " to %hu on port %s\n", bpb, m_name.c_str());
            break;
        }

        retval = setAttrs(&term, "Could not set character size");
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getReadState () {
    return getBit(CREAD, SerialPortImpTermios::CFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableRead () {
    return setBit(CREAD, SerialPortImpTermios::CFLAG, true,
                  "Could not enable reading");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableRead () {
    return setBit(CREAD, SerialPortImpTermios::CFLAG, false,
                  "Could not disable reading");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
unsigned short
SerialPortImpTermios::getStopBits () {
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
SerialPortImpTermios::setStopBits (const unsigned short num_bits) {
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
                    "[vpr::SerialPortImpTermios] Stop bits may only be set to "
                    "1 or 2 on port %s\n", m_name.c_str());
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
SerialPortImpTermios::getInputEchoState () {
    return getBit(ECHO, SerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableInputEcho () {
    return setBit(ECHO, SerialPortImpTermios::LFLAG, true,
                  "Could not enable input echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableInputEcho () {
    return setBit(ECHO, SerialPortImpTermios::LFLAG, false,
                  "Could not disable input echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getEraseEchoState () {
    return getBit(ECHOE, SerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableEraseEcho () {
    return setBit(ECHOE, SerialPortImpTermios::LFLAG, true,
                  "Could not enable erase echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableEraseEcho () {
    return setBit(ECHOE, SerialPortImpTermios::LFLAG, false,
                  "Could not disable erase echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getKillEchoState () {
    return getBit(ECHOK, SerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableKillEcho () {
    return setBit(ECHOK, SerialPortImpTermios::LFLAG, true,
                  "Could not enable kill echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableKillEcho () {
    return setBit(ECHOK, SerialPortImpTermios::LFLAG, false,
                  "Could not disable kill echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getNewlineEchoState () {
    return getBit(ECHONL, SerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableNewlineEcho () {
    return setBit(ECHONL, SerialPortImpTermios::LFLAG, true,
           "Could not enable newline echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableNewlineEcho () {
    return setBit(ECHONL, SerialPortImpTermios::LFLAG, false,
           "Could not disable newline echo");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getCanonicalState () {
    return getBit(ICANON, SerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableCanonicalInput () {
    return setBit(ICANON, SerialPortImpTermios::LFLAG, true,
           "Could not enable canonical input mode");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableCanonicalInput () {
    return setBit(ICANON, SerialPortImpTermios::LFLAG, false,
           "Could not disable canonical input mode");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getCRTranslateState () {
    return getBit(ICRNL, SerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableCRTranslation () {
    return setBit(ICRNL, SerialPortImpTermios::IFLAG, true,
                  "Could not enable carriage return translation mode");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableCRTranslation () {
    return setBit(ICRNL, SerialPortImpTermios::IFLAG, false,
                  "Could not disable carriage return translation mode");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getExtendedHandlingState () {
    return getBit(IEXTEN, SerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableExtendedHandling () {
    return setBit(IEXTEN, SerialPortImpTermios::LFLAG, true,
                  "Could not enable extended character handling mode");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableExtendedHandling () {
    return setBit(IEXTEN, SerialPortImpTermios::LFLAG, false,
                  "Could not disable extended character handling mode");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getCRIgnoreState () {
    return getBit(IGNCR, SerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableCRIgnore () {
    return setBit(IGNCR, SerialPortImpTermios::IFLAG, true,
                  "Could not enable carriage return ignoring");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableCRIgnore () {
    return setBit(IGNCR, SerialPortImpTermios::IFLAG, false,
                  "Could not disable carriage return ignoring");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getBadByteIgnoreState () {
    return getBit(IGNPAR, SerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableBadByteIgnore () {
    return setBit(IGNPAR, SerialPortImpTermios::IFLAG, true,
                  "Could not enable bad byte ignoring");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableBadByteIgnore () {
    return setBit(IGNPAR, SerialPortImpTermios::IFLAG, false,
                  "Could not disable bad byte ignoring");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getParityCheckState () {
    return getBit(IGNPAR, SerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableParityCheck () {
    return setBit(INPCK, SerialPortImpTermios::IFLAG, true,
                  "Could not enable parity checking");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableParityCheck () {
    return setBit(INPCK, SerialPortImpTermios::IFLAG, false,
                  "Could not disable parity checking");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getSignalGenerateState () {
    return getBit(ISIG, SerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableSignalGeneration () {
    return setBit(ISIG, SerialPortImpTermios::LFLAG, true,
                  "Could not enable signal generation for input characters");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableSignalGeneration () {
    return setBit(ISIG, SerialPortImpTermios::LFLAG, false,
                  "Could not disable signal generation for input characters");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getBitStripState () {
    return getBit(ISTRIP, SerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableBitStripping () {
    return setBit(ISTRIP, SerialPortImpTermios::IFLAG, true,
                  "Could not enable bit stripping from eight to seven bits");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableBitStripping () {
    return setBit(ISTRIP, SerialPortImpTermios::IFLAG, false,
                  "Could not disable bit stripping");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getStartStopInputState () {
    return getBit(IXOFF, SerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableStartStopInput () {
    return setBit(IXOFF, SerialPortImpTermios::IFLAG, true,
                  "Could not enable start-stop input control");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableStartStopInput () {
    return setBit(IXOFF, SerialPortImpTermios::IFLAG, false,
                  "Could not disable start-stop input control");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getStartStopOutputState () {
    return getBit(IXON, SerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableStartStopOutput () {
    return setBit(IXON, SerialPortImpTermios::IFLAG, true,
                  "Could not enable start-stop output control");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableStartStopOutput () {
    return setBit(IXON, SerialPortImpTermios::IFLAG, false,
                  "Could not disable start-stop output control");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getNoFlushState () {
    return getBit(NOFLSH, SerialPortImpTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableNoFlush () {
    return setBit(NOFLSH, SerialPortImpTermios::LFLAG, true,
                  "Could not enable no flushing at signal generation");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableNoFlush () {
    return setBit(NOFLSH, SerialPortImpTermios::LFLAG, false,
                  "Could not disable no flushing at signal generation");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getParityGenerationState () {
    return getBit(PARENB, SerialPortImpTermios::CFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableParityGeneration () {
    return setBit(PARENB, SerialPortImpTermios::CFLAG, true,
                  "Could not enable parity generation on outgoing characters");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableParityGeneration () {
    return setBit(PARENB, SerialPortImpTermios::CFLAG, false,
                  "Could not disable parity generation on outgoing characters");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getParityErrorMarkingState () {
    return getBit(PARMRK, SerialPortImpTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::enableParityErrorMarking () {
    return setBit(PARMRK, SerialPortImpTermios::IFLAG, true,
                  "Could not enable parity error marking");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::disableParityErrorMarking () {
    return setBit(PARMRK, SerialPortImpTermios::IFLAG, false,
                  "Could not disable parity error marking");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::getParity () {
    int retval;

    // Odd parity if PARODD is set.
    if ( getBit(PARODD, SerialPortImpTermios::CFLAG) ) {
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
SerialPortImpTermios::setOddParity () {
    return setBit(PARODD, SerialPortImpTermios::CFLAG, true,
                  "Could not set odd parity");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::setEvenParity () {
    return setBit(PARODD, SerialPortImpTermios::CFLAG, false,
                  "Could not set even parity");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::getInputBaudRate () {
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
SerialPortImpTermios::setInputBaudRate (const int baud) {
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
SerialPortImpTermios::getOutputBaudRate () {
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
SerialPortImpTermios::setOutputBaudRate (const int baud) {
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
SerialPortImpTermios::drainOutput () {
    int retval;

    retval = tcdrain(m_handle->m_fdesc);

    if ( retval == -1 ) {
        fprintf(stderr,
                "[vpr::SerialPortImpTermios] Failed to drain output on port %s: %s\n",
                m_name.c_str(), strerror(errno));
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::controlFlow (SerialTypes::FlowActionOption opt) {
    int action, retval;

    switch (opt) {
      case SerialTypes::OUTPUT_OFF:
        action = TCOOFF;
        break;
      case SerialTypes::OUTPUT_ON:
        action = TCOON;
        break;
      case SerialTypes::INPUT_OFF:
        action = TCIOFF;
        break;
      case SerialTypes::INPUT_ON:
        action = TCION;
        break;
    }

    retval = tcflow(m_handle->m_fdesc, action);

    if ( retval == -1 ) {
        fprintf(stderr,
                "[vpr::SerialPortImpTermios] Failed to alter flow control on "
                "port %s: %s\n", m_name.c_str(), strerror(errno));
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::flushQueue (SerialTypes::FlushQueueOption opt) {
    int queue, retval;

    switch (opt) {
      case SerialTypes::INPUT_QUEUE:
        queue = TCIFLUSH;
        break;
      case SerialTypes::OUTPUT_QUEUE:
        queue = TCOFLUSH;
        break;
      case SerialTypes::IO_QUEUES:
        queue = TCIOFLUSH;
        break;
    }

    retval = tcflush(m_handle->m_fdesc, queue);

    if ( retval == -1 ) {
        std::string queue_name;

        switch (opt) {
          case SerialTypes::INPUT_QUEUE:
            queue_name = "input queue";
            break;
          case SerialTypes::OUTPUT_QUEUE:
            queue_name = "output queue";
            break;
          case SerialTypes::IO_QUEUES:
            queue_name = "input and output queues";
            break;
        }

        fprintf(stderr,
                "[vpr::SerialPortImpTermios] Failed to flush %s on port %s: %s\n",
                queue_name.c_str(), m_name.c_str(), strerror(errno));
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::sendBreak (const int duration) {
    int retval;

    retval = tcsendbreak(m_handle->m_fdesc, duration);

    if ( retval == -1 ) {
        fprintf(stderr,
                "[vpr::SerialPortImpTermios] Failed to send break on port %s: %s\n",
                m_name.c_str(), strerror(errno));
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void
SerialPortImpTermios::setControlCharacter (const unsigned int index,
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
                    "[vpr::SerialPortImpTermios] Index %u too large for "
                    "control character array\n", index);
        }
    }
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
unsigned char
SerialPortImpTermios::getControlCharacter (const unsigned int index) {
    struct termios term;
    unsigned char retval;

    retval = 0;

    if ( getAttrs(&term) != -1 ) {
        if ( index < NCCS ) {
            retval = term.c_cc[index];
        }
        else {
            fprintf(stderr,
                    "[vpr::SerialPortImpTermios] Index %u too large for "
                    "control character array\n", index);
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
SerialPortImpTermios::getAttrs (struct termios* term) {
    int retval;

    retval = tcgetattr(m_handle->m_fdesc, term);

    if ( retval == -1 ) {
        fprintf(stderr,
                "[vpr::SerialPortImpTermios] Could not get attributes for port %s: %s\n",
                m_name.c_str(), strerror(errno));
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int
SerialPortImpTermios::setAttrs (struct termios* term, const char* err_msg,
                               const bool print_sys_err)
{
    int retval;

fprintf(stderr, "[vpr::SerialPortImpTermios] term->c_iflag: %d\n",
        term->c_iflag);
fprintf(stderr, "[vpr::SerialPortImpTermios] term->c_oflag: %d\n",
        term->c_oflag);
fprintf(stderr, "[vpr::SerialPortImpTermios] term->c_cflag: %d\n",
        term->c_cflag);
fprintf(stderr, "[vpr::SerialPortImpTermios] term->c_lflag: %d\n",
        term->c_lflag);
fprintf(stderr, "[vpr::SerialPortImpTermios] term->c_ispeed: %d\n",
        cfgetispeed(term));
fprintf(stderr, "[vpr::SerialPortImpTermios] term->c_ospeed: %d\n\n",
        cfgetospeed(term));
fprintf(stderr, "[vpr::SerialPortImpTermios] term->c_cc[]:");
for ( int i = 0; i < NCCS; i++ ) {
    fprintf(stderr, " %u", (unsigned int) term->c_cc[i]);
}
fprintf(stderr, "\n");

    retval = tcsetattr(m_handle->m_fdesc, m_actions, term);

    if ( retval == -1 ) {
        fprintf(stderr, "[vpr::SerialPortImpTermios] %s (port '%s')", err_msg,
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
SerialPortImpTermios::setAttrs (struct termios* term,
                                const std::string& err_msg,
                                const bool print_sys_err)
{
    return setAttrs(term, err_msg.c_str(), print_sys_err);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImpTermios::getBit (const tcflag_t bit,
                              SerialPortImpTermios::_term_flag flag)
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
SerialPortImpTermios::setBit (const tcflag_t bit,
                              SerialPortImpTermios::_term_flag flag,
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
SerialPortImpTermios::baudToInt (const speed_t baud_rate) {
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
SerialPortImpTermios::intToBaud (const int speed_int) {
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

}; // End of vpr namespace
