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

#include <vpr/md/POSIX/IO/Port/SerialPortImplTermios.h>


extern int errno;

namespace vpr {

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor.  This creates a file handle object connected to the given port
// name and sets the update action to happen immediately.
// ----------------------------------------------------------------------------
SerialPortImplTermios::SerialPortImplTermios (const std::string& port_name) {
    m_handle = new FileHandleImplUNIX(port_name);
    setUpdateAction(SerialTypes::NOW);
}

// ----------------------------------------------------------------------------
// Destructor.  If the file handle is non-NULL, its memory is released.
// ----------------------------------------------------------------------------
SerialPortImplTermios::~SerialPortImplTermios () {
    if ( m_handle != NULL ) {
        delete m_handle;
    }
}

// ----------------------------------------------------------------------------
// Open the serial port and initialize its flags.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::open () {
    Status status;

    status = m_handle->open();

    // If the serial port could not be opened, print an error message.
    if ( status == vpr::Status::Failure ) {
        fprintf(stderr,
                "[vpr::SerialPortImplTermios] Could not open serial port %s: %s\n",
                getName().c_str(), strerror(errno));
    }
    // Otherwise, initialize the serial port's flags.
    else {
        struct termios term;

        if ( getAttrs(&term).success() ) {
            // Initialize all the flags to 0.
            term.c_iflag = term.c_oflag = term.c_cflag = term.c_lflag = 0;

            // Initialize the minimum buffer size to 1 and the timeout to 0.
            term.c_cc[VMIN]  = 1;
            term.c_cc[VTIME] = 0;

            // If we cannot set the initialized attribute flags on the port,
            // then we are not considered open.
            if ( ! setAttrs(&term, "Could not initialize flags").success() ) {
                status.setCode(Status::Failure);
            }
        }
    }

    return status;
}

// ----------------------------------------------------------------------------
// Get the current update action.  This tells when updates to the serial
// device take effect.
// ----------------------------------------------------------------------------
SerialTypes::UpdateActionOption
SerialPortImplTermios::getUpdateAction () {
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
// Change the current update action to take place as described by the given
// value.
// ----------------------------------------------------------------------------
void
SerialPortImplTermios::setUpdateAction (SerialTypes::UpdateActionOption action)
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
// Query the serial port for the maximum buffer size.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::getBufferSize (Uint16& size) {
    Status retval;
    struct termios term;

    if ( (retval = getAttrs(&term)).success() ) {
        size = (Uint16) term.c_cc[VMIN];
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Attempt to change the buffer size to the given argument.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::setBufferSize (const Uint8 size) {
    Status retval;
    struct termios term;

    if ( (retval = getAttrs(&term)).success() ) {
        term.c_cc[VMIN] = size;
        retval = setAttrs(&term, "Could not set minimum buffer size");
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Get the value of the timeout (in tenths of a second) to wait for data to
// arrive.  This is only applicable in non-canonical mode.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::getTimeout (Uint8& timeout) {
    Status retval;
    struct termios term;

    if ( (retval = getAttrs(&term)).success() ) {
        timeout = term.c_cc[VTIME];
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Set the value of the timeout to wait for data to arrive.  The value given
// must be in tenths of a second.  This is only applicable in non-canonical
// mode.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::setTimeout (const Uint8 timeout) {
    Status retval;
    struct termios term;

    if ( (retval = getAttrs(&term)).success() ) {
        term.c_cc[VTIME] = timeout;
        retval = setAttrs(&term, "Could not set minimum buffer size");
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Get the character size (the bits per byte).
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::getCharacterSize (SerialTypes::CharacterSizeOption& size)
{
    Status retval;
    struct termios term;

    if ( (retval = getAttrs(&term)).success() ) {
        switch (term.c_cflag & CSIZE) {
          case CS5:
            size = SerialTypes::CS_BITS_5;
            break;
          case CS6:
            size = SerialTypes::CS_BITS_6;
            break;
          case CS7:
            size = SerialTypes::CS_BITS_7;
            break;
          case CS8:
            size = SerialTypes::CS_BITS_8;
            break;
        }
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Set the current character size (the bits per byte) to the size in the given
// value.  This is used for both reding and writing, and the size does not
// include the parity bit (if any).
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::setCharacterSize (const SerialTypes::CharacterSizeOption bpb)
{
    struct termios term;
    Status retval;

    if ( (retval = getAttrs(&term)).success() ) {
        term.c_cflag &= ~CSIZE;	// Zero out the bits

        // Set the character size based on the given bits-per-byte value.
        switch (bpb) {
          // 5 bits/byte.
          case SerialTypes::CS_BITS_5:
            term.c_cflag |= CS5;
            break;
          // 6 bits/byte.
          case SerialTypes::CS_BITS_6:
            term.c_cflag |= CS6;
            break;
          // 7 bits/byte.
          case SerialTypes::CS_BITS_7:
            term.c_cflag |= CS7;
            break;
          // 8 bits/byte.
          case SerialTypes::CS_BITS_8:
            term.c_cflag |= CS8;
            break;
        }

        retval = setAttrs(&term, "Could not set character size");
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Get the number of stop bits in use.  This will be either 1 or 2.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::getStopBits (Uint8& num_bits) {
    struct termios term;
    Status retval;

    if ( (retval = getAttrs(&term)).success() ) {
        switch (term.c_cflag & CSTOPB) {
          case 0:
            num_bits = 1;
            break;
          case 1:
            num_bits = 2;
            break;
        }
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Set the number of stop bits to use.  The value must be either 1 or 2.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::setStopBits (const Uint8 num_bits) {
    struct termios term;
    Status retval;

    if ( (retval = getAttrs(&term)).success() ) {
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
                    "[vpr::SerialPortImplTermios] Stop bits may only be set to "
                    "1 or 2 on port %s\n", getName().c_str());
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
// Query the canonical input state of the serial port.  If canonical mode is
// enabled, the characters EOF, EOL, EOL2, ERASE, KILL, REPRINT, STATUS, and
// WERASE are enabled, and the input characters are assembled into lines.
// Otherwise, read requests are satisfied directly from the input queue, and a
// read will not return until the buffer is at its minimum capacity or the
// timeout has expired.  See getBufferSize() and getTimeout() for more
// information.
// ----------------------------------------------------------------------------
bool
SerialPortImplTermios::getCanonicalState () {
    return getBit(ICANON, SerialPortImplTermios::LFLAG);
}

// ----------------------------------------------------------------------------
// Enable canonical input.  See getCanonicalState() for more information about
// what this means.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::enableCanonicalInput () {
    return setBit(ICANON, SerialPortImplTermios::LFLAG, true,
           "Could not enable canonical input mode");
}

// ----------------------------------------------------------------------------
// Disable canonical input.  See getCanonicalState() for more information
// about what this means.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::disableCanonicalInput () {
    return setBit(ICANON, SerialPortImplTermios::LFLAG, false,
           "Could not disable canonical input mode");
}

// ----------------------------------------------------------------------------
// Get the current state of ignoring bytes with framing errors (other than a
// BREAK) or parity errors.
// ----------------------------------------------------------------------------
bool
SerialPortImplTermios::getBadByteIgnoreState () {
    return getBit(IGNPAR, SerialPortImplTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// Enable ignoring of received bytes with framing errors or parity errors.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::enableBadByteIgnore () {
    return setBit(IGNPAR, SerialPortImplTermios::IFLAG, true,
                  "Could not enable bad byte ignoring");
}

// ----------------------------------------------------------------------------
// Disable ignoring of received bytes with framing errors or parity errors.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::disableBadByteIgnore () {
    return setBit(IGNPAR, SerialPortImplTermios::IFLAG, false,
                  "Could not disable bad byte ignoring");
}

// ----------------------------------------------------------------------------
// Get the state of parity checking for input.
// ----------------------------------------------------------------------------
bool
SerialPortImplTermios::getInputParityCheckState () {
    return getBit(IGNPAR, SerialPortImplTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// Enable input parity checking.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::enableInputParityCheck () {
    return setBit(INPCK, SerialPortImplTermios::IFLAG, true,
                  "Could not enable input parity checking");
}

// ----------------------------------------------------------------------------
// Disable input parity checking.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::disableInputParityCheck () {
    return setBit(INPCK, SerialPortImplTermios::IFLAG, false,
                  "Could not disable input parity checking");
}

// ----------------------------------------------------------------------------
// Get the current state of bit stripping.  When enabled, input bytes are
// stripped to seven bits.  Otherwise, all eight bits are processed.
// ----------------------------------------------------------------------------
bool
SerialPortImplTermios::getBitStripState () {
    return getBit(ISTRIP, SerialPortImplTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// Enable stripping of input bytes to seven bits.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::enableBitStripping () {
    return setBit(ISTRIP, SerialPortImplTermios::IFLAG, true,
                  "Could not enable bit stripping from eight to seven bits");
}

// ----------------------------------------------------------------------------
// Disable stripping of input bytes to seven bits.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::disableBitStripping () {
    return setBit(ISTRIP, SerialPortImplTermios::IFLAG, false,
                  "Could not disable bit stripping");
}

// ----------------------------------------------------------------------------
// Get the state of start-stop input control.  When enabled, if the terminal
// driver sees that the receive buffer is getting full, a STOP command is sent
// to the serial device.  The device should respond by halting its data flow.
// When the input queue has been processed, a START command is sent to the
// serial device which should cause it to resume putting bytes onto the input
// queue.
// ----------------------------------------------------------------------------
bool
SerialPortImplTermios::getStartStopInputState () {
    return getBit(IXOFF, SerialPortImplTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// Enable start-stop input control.  See getStartStopInputState() for more
// information.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::enableStartStopInput () {
    return setBit(IXOFF, SerialPortImplTermios::IFLAG, true,
                  "Could not enable start-stop input control");
}

// ----------------------------------------------------------------------------
// Disable start-stop input control.  See getStartStopInputState() for more
// information.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::disableStartStopInput () {
    return setBit(IXOFF, SerialPortImplTermios::IFLAG, false,
                  "Could not disable start-stop input control");
}

// ----------------------------------------------------------------------------
// Get the state of start-stop output control.  When enabled, when the
// terminal driver receives a STOP command, output stops.  When it receives a
// START command, output resumes.  If disabled, these/ commands are read as
// normal characters.
// ----------------------------------------------------------------------------
bool
SerialPortImplTermios::getStartStopOutputState () {
    return getBit(IXON, SerialPortImplTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// Enable start-stop input control.  See getStartStopInputState() for more
// information.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::enableStartStopOutput () {
    return setBit(IXON, SerialPortImplTermios::IFLAG, true,
                  "Could not enable start-stop output control");
}

// ----------------------------------------------------------------------------
// Disable start-stop input control.  See getStartStopInputState() for more
// information.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::disableStartStopOutput () {
    return setBit(IXON, SerialPortImplTermios::IFLAG, false,
                  "Could not disable start-stop output control");
}

// ----------------------------------------------------------------------------
// Get the current state of parity generation for outgoing bytes and parity
// checking for incoming bytes.
// ----------------------------------------------------------------------------
bool
SerialPortImplTermios::getParityGenerationState () {
    return getBit(PARENB, SerialPortImplTermios::CFLAG);
}

// ----------------------------------------------------------------------------
// Enable parity generation for outgoing bytes and parity checking for
// incoming bytes.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::enableParityGeneration () {
    return setBit(PARENB, SerialPortImplTermios::CFLAG, true,
                  "Could not enable parity generation on outgoing characters");
}

// ----------------------------------------------------------------------------
// Disable parity generation for outgoing bytes and parity checking for
// incoming bytes.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::disableParityGeneration () {
    return setBit(PARENB, SerialPortImplTermios::CFLAG, false,
                  "Could not disable parity generation on outgoing characters");
}

// ----------------------------------------------------------------------------
// Enable marking of bytes with parity errors or framing errors (except
// BREAKs).  This is only active if input parity and framing error reporting
// is enabled (see getInputParityCheckState() for more information).  The mark
// is the three-byte sequence \377 \0 X where X is the byte received in error.
// If bit stripping is enabled, a valid \377 byte is passed as the two-byte
// sequence \377 \377.
// ----------------------------------------------------------------------------
bool
SerialPortImplTermios::getParityErrorMarkingState () {
    return getBit(PARMRK, SerialPortImplTermios::IFLAG);
}

// ----------------------------------------------------------------------------
// Enable parity error and framing error marking.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::enableParityErrorMarking () {
    return setBit(PARMRK, SerialPortImplTermios::IFLAG, true,
                  "Could not enable parity error marking");
}

// ----------------------------------------------------------------------------
// Disable parity error and framing error marking.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::disableParityErrorMarking () {
    return setBit(PARMRK, SerialPortImplTermios::IFLAG, false,
                  "Could not disable parity error marking");
}

// ----------------------------------------------------------------------------
// Get the current parity checking type (either odd or even).
// ----------------------------------------------------------------------------
SerialTypes::ParityType
SerialPortImplTermios::getParity () {
    SerialTypes::ParityType retval;

    // Odd parity if PARODD is set.
    if ( getBit(PARODD, SerialPortImplTermios::CFLAG) ) {
        retval = SerialTypes::PORT_PARITY_ODD;
    }
    // Even parity if PARODD is not set.
    else {
        retval = SerialTypes::PORT_PARITY_EVEN;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Enable odd parity.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::setOddParity () {
    return setBit(PARODD, SerialPortImplTermios::CFLAG, true,
                  "Could not set odd parity");
}

// ----------------------------------------------------------------------------
// Enable even parity.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::setEvenParity () {
    return setBit(PARODD, SerialPortImplTermios::CFLAG, false,
                  "Could not set even parity");
}

// ----------------------------------------------------------------------------
// Get the current input baud rate.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::getInputBaudRate (Uint32& rate) {
    struct termios term;
    Status retval;

    if ( (retval = getAttrs(&term)).success() ) {
        speed_t baud_rate;

        baud_rate = cfgetispeed(&term);
        rate      = baudToInt(baud_rate);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Set the current input baud rate.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::setInputBaudRate (const Uint32 baud) {
    struct termios term;
    Status retval;

    if ( (retval = getAttrs(&term)).success() ) {
        speed_t new_rate;

        new_rate = intToBaud(baud);

fprintf(stderr, "Setting input baud rate to %d\n", new_rate);

        if ( cfsetispeed(&term, new_rate) == -1 ) {
            fprintf(stderr,
                    "Failed to set the input baud rate to %u on port %s: %s\n",
                    baud, getName().c_str(), strerror(errno));
            retval.setCode(Status::Failure);
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
// Get the current output baud rate.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::getOutputBaudRate (Uint32& rate) {
    struct termios term;
    Status retval;

    if ( (retval = getAttrs(&term)).success() ) {
        speed_t baud_rate;

        baud_rate = cfgetospeed(&term);
        rate      = baudToInt(baud_rate);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Set the current output baud rate.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::setOutputBaudRate (const Uint32 baud) {
    struct termios term;
    Status retval;

    if ( (retval = getAttrs(&term)).success() ) {
        speed_t new_rate;

        new_rate = intToBaud(baud);

fprintf(stderr, "Setting output baud rate to %d\n", new_rate);

        if ( cfsetospeed(&term, new_rate) == -1 ) {
            fprintf(stderr,
                    "Failed to set the output baud rate to %u on port %s: %s\n",
                    baud, getName().c_str(), strerror(errno));
            retval.setCode(Status::Failure);
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
// Wait for all output to be transmitted.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::drainOutput () {
    Status retval;

    if ( tcdrain(m_handle->m_fdesc) == -1 ) {
        fprintf(stderr,
                "[vpr::SerialPortImplTermios] Failed to drain output on port %s: %s\n",
                getName().c_str(), strerror(errno));
        retval.setCode(Status::Failure);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Alter the input or output flow control.  Based on the
// vpr::SerialTypes::FlowActionOption argument, output can be suspended and
// restarted or the terminal device can be told to stop or to resume sending
// data.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::controlFlow (SerialTypes::FlowActionOption opt) {
    int action;
    Status retval;

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

    if ( tcflow(m_handle->m_fdesc, action) == -1 ) {
        fprintf(stderr,
                "[vpr::SerialPortImplTermios] Failed to alter flow control on "
                "port %s: %s\n", getName().c_str(), strerror(errno));
        retval.setCode(Status::Failure);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Discard either the input buffer (unread data received from the terminal
// device) or the output buffer (data written but not yet transmitted to the
// terminal device).  The argument tells which queue (or queues) to flush.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::flushQueue (SerialTypes::FlushQueueOption vpr_queue) {
    int queue;
    Status retval;

    switch (vpr_queue) {
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

    if ( tcflush(m_handle->m_fdesc, queue) == -1 ) {
        std::string queue_name;

        switch (vpr_queue) {
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
                "[vpr::SerialPortImplTermios] Failed to flush %s on port %s: %s\n",
                queue_name.c_str(), getName().c_str(), strerror(errno));
        retval.setCode(Status::Failure);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Transmit a continuous stream of zero bits for the given duration.  If the
// argument is 0, the transmission will last between 0.25 and 0.5 seconds.
// Otherwise, the duration specfies the number of seconds to send the zero bit
// stream.
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::sendBreak (const Int32 duration) {
    Status retval;

    if ( tcsendbreak(m_handle->m_fdesc, duration) == -1 ) {
        fprintf(stderr,
                "[vpr::SerialPortImplTermios] Failed to send break on port %s: %s\n",
                getName().c_str(), strerror(errno));
        retval.setCode(Status::Failure);
    }

    return retval;
}

// ============================================================================
// Protected methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Set the control character at the given index to the given value.
// ----------------------------------------------------------------------------
void
SerialPortImplTermios::setControlCharacter (const Uint32 index,
                                           const Uint8 value)
{
    struct termios term;

    if ( getAttrs(&term).success() ) {
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
                    "[vpr::SerialPortImplTermios] Index %u too large for "
                    "control character array\n", index);
        }
    }
}

// ----------------------------------------------------------------------------
// Get the value of the control character at the given index.
// ----------------------------------------------------------------------------
Uint8
SerialPortImplTermios::getControlCharacter (const Uint32 index) {
    struct termios term;
    Uint8 retval;

    retval = 0;

    if ( getAttrs(&term).success() ) {
        if ( index < NCCS ) {
            retval = term.c_cc[index];
        }
        else {
            fprintf(stderr,
                    "[vpr::SerialPortImplTermios] Index %u too large for "
                    "control character array\n", index);
        }
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::getAttrs (struct termios* term) {
    Status retval;

    if ( tcgetattr(m_handle->m_fdesc, term) == -1 ) {
        fprintf(stderr,
                "[vpr::SerialPortImplTermios] Could not get attributes for port %s: %s\n",
                getName().c_str(), strerror(errno));
        retval.setCode(Status::Failure);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::setAttrs (struct termios* term, const char* err_msg,
                                 const bool print_sys_err)
{
    Status retval;

fprintf(stderr, "[vpr::SerialPortImplTermios] term->c_iflag: %d\n",
        term->c_iflag);
fprintf(stderr, "[vpr::SerialPortImplTermios] term->c_oflag: %d\n",
        term->c_oflag);
fprintf(stderr, "[vpr::SerialPortImplTermios] term->c_cflag: %d\n",
        term->c_cflag);
fprintf(stderr, "[vpr::SerialPortImplTermios] term->c_lflag: %d\n",
        term->c_lflag);
fprintf(stderr, "[vpr::SerialPortImplTermios] term->c_ispeed: %d\n",
        cfgetispeed(term));
fprintf(stderr, "[vpr::SerialPortImplTermios] term->c_ospeed: %d\n\n",
        cfgetospeed(term));
fprintf(stderr, "[vpr::SerialPortImplTermios] term->c_cc[]:");
for ( int i = 0; i < NCCS; i++ ) {
    fprintf(stderr, " %u", (unsigned int) term->c_cc[i]);
}
fprintf(stderr, "\n");

    if ( tcsetattr(m_handle->m_fdesc, m_actions, term) == -1 ) {
        fprintf(stderr, "[vpr::SerialPortImplTermios] %s (port '%s')", err_msg,
                getName().c_str());

        if ( print_sys_err ) {
            fprintf(stderr, ": %s", strerror(errno));
        }

        fprintf(stderr, "\n");

        retval.setCode(Status::Failure);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SerialPortImplTermios::setAttrs (struct termios* term,
                                 const std::string& err_msg,
                                 const bool print_sys_err)
{
    return setAttrs(term, err_msg.c_str(), print_sys_err);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool
SerialPortImplTermios::getBit (const tcflag_t bit,
                               SerialPortImplTermios::_term_flag flag)
{
    struct termios term;
    bool retval;

    if ( ! getAttrs(&term).success() ) {
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
Status
SerialPortImplTermios::setBit (const tcflag_t bit,
                               SerialPortImplTermios::_term_flag flag,
                               const bool enable,
                               const std::string& err_msg,
                               const bool print_sys_err)
{
    struct termios term;
    Status retval;

    if ( (retval = getAttrs(&term)).success() ) {
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
// Convert a termios baud rate to its corresponding integer value.
// ----------------------------------------------------------------------------
Uint32
SerialPortImplTermios::baudToInt (const speed_t baud_rate) {
    Uint32 retval;

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
// Convert an integer baud rate to the corresponding termios rate constant.
// ----------------------------------------------------------------------------
speed_t
SerialPortImplTermios::intToBaud (const Uint32 speed_int) {
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
