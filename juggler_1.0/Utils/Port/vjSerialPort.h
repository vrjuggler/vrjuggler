/*************** <auto-copyright.pl BEGIN do not edit this line> ************** *
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

#ifndef _VJ_SERIAL_PORT_H_
#define _VJ_SERIAL_PORT_H_

#include <Utils/Port/vjPort.h>
#include <Utils/Port/vjSerialTypes.h>
#include <Utils/Port/vjSerialPortImp.h>
#include <Utils/Port/vjSerialPortFactory.h>


// ----------------------------------------------------------------------------
//: Cross-platform serial port interface.
// ----------------------------------------------------------------------------
//!PUBLIC_API:
class vjSerialPort : public vjPort {
public:
    // ------------------------------------------------------------------------
    //: Constructor.
    //
    //! PRE: None.
    //! POST: A serial port interface implementation object is created using
    //+       the given port name.
    //
    //! ARGS: port_name - The name of the serial port to be accessed by this
    //+                   object.
    // ------------------------------------------------------------------------
    vjSerialPort (const std::string& port_name)
        : vjPort(port_name), m_sio_imp(NULL)
    {
        vjSerialPortFactory fac;
        m_sio_imp = fac.create(port_name);
    }

    // ------------------------------------------------------------------------
    //: Destructor.
    //
    //! PRE: None.
    //! POST: The memory allocated to m_sio_imp in the constructor is
    //+       released.
    // ------------------------------------------------------------------------
    virtual ~vjSerialPort (void) {
        if ( m_sio_imp != NULL ) {
            delete m_sio_imp;
        }
    }

    // ========================================================================
    // Block I/O device interface.
    // ========================================================================

    // ------------------------------------------------------------------------
    //: Set the open flags so that the serial port is opened in read-only
    //+ mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the port is opened, it
    //+       is opened in read-only mode.  If the port is already open, this
    //+       has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenReadOnly (void) {
        m_sio_imp->setOpenReadOnly();
    }

    // ------------------------------------------------------------------------
    //: Set the open flags so that the serial port is opened in write-only
    //+ mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the port is opened, it
    //+       is opened in write-only mode.  If the port is already open, this
    //+       has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenWriteOnly (void) {
        m_sio_imp->setOpenWriteOnly();
    }

    // ------------------------------------------------------------------------
    //: Set the open flags so that the serial port is opened in read/write
    //+ mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the port is opened, it
    //+       is opened in read/write mode.  If the port is already open, this
    //+       has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenReadWrite (void) {
        m_sio_imp->setOpenReadWrite();
    }

    // ------------------------------------------------------------------------
    //: Set the blocking flags so that the serial port is opened in blocking
    //+ mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the port is opened, it
    //+       is opened in blocking mode.  If the port is already open, this
    //+       has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenBlocking (void) {
        m_sio_imp->setOpenBlocking();
    }

    // ------------------------------------------------------------------------
    //: Set the blocking flags so that the serial port is opened in
    //+ non-blocking mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the port is opened, it
    //+       is opened in non-blocking mode.  If the port is already open,
    //+       this has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenNonBlocking (void) {
        m_sio_imp->setOpenNonBlocking();
    }

    // ------------------------------------------------------------------------
    //: Open the serial port.
    //
    //! PRE: The serial port is not already open.
    //! POST: An attempt is made to open the serial port.  The resulting
    //+       status is returned to the caller.
    //
    //! RETURNS: true  - The serial port was opened successfully.
    //! RETURNS: false - The serial port could not be opened for some reason.
    // ------------------------------------------------------------------------
    inline virtual bool
    open (void) {
        return m_sio_imp->open();
    }

    // ------------------------------------------------------------------------
    //: Close the serial port.
    //
    //! PRE: The serial port is open.
    //! POST: An attempt is made to close the serial port.  The resulting
    //+       status is returned to the caller.
    //
    //! RETURNS: true  - The serial port was closed successfully.
    //! RETURNS: false - The serial port could not be closed for some reason.
    // ------------------------------------------------------------------------
    inline virtual bool
    close (void) {
        return m_sio_imp->close();
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the serial port so that it is in blocking mode.
    //
    //! PRE: The port is open.
    //! POST: Processes will block when accessing the port.
    //
    //! RETURNS: true  - The blocking mode was changed successfully.
    //! RETURNS: false - The blocking mode could not be changed for some
    //+                  reason.
    // ------------------------------------------------------------------------
    inline virtual bool
    enableBlocking (void) {
        return m_sio_imp->enableBlocking();
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the serial port so that it is in non-blocking mode.
    //
    //! PRE: The port is open.
    //! POST: Processes will not block when accessing the port.
    //
    //! RETURNS: true  - The blocking mode was changed successfully.
    //! RETURNS: false - The blocking mode could not be changed for some
    //+                  reason.
    // ------------------------------------------------------------------------
    inline virtual bool
    enableNonBlocking (void) {
        return m_sio_imp->enableNonBlocking();
    }

    // ------------------------------------------------------------------------
    //: Read at most the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port implementation object is valid, and the buffer is at
    //+     least length bytes long.
    //! POST: The given buffer has length bytes copied into it from the port's
    //+       buffer, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer where the port's buffer
    //+                contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    read (void* buffer, const size_t length) {
        return m_sio_imp->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Read at most the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port implementation object is valid, and the buffer is at
    //+      least length bytes long.
    //! POST: The given buffer has length bytes copied into it from the port's
    //+       buffer, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of unsigned chars)
    //+                where the port's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    read (unsigned char* buffer, const size_t length) {
        return m_sio_imp->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Read at most the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port implementation object is valid, and the buffer is at
    //+      least length bytes long.
    //! POST: The given buffer has length bytes copied into it from the port's
    //+       buffer, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of chars) where the
    //+                port's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    read (char* buffer, const size_t length) {
        return m_sio_imp->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Read at most the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port implementation object is valid, and the buffer is at
    //+      least length bytes long.
    //! POST: The given buffer has length bytes copied into it from the port's
    //+       buffer, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::string object) where
    //+                the port's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.  This is optional and
    //+                can be determined from the length of the string object
    //+                if not specified.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    read (std::string& buffer, const size_t length = 0) {
        return m_sio_imp->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Read at most the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port implementation object is valid, and the buffer is at
    //+      least length bytes long.
    //! POST: The given buffer has length bytes copied into it from the port's
    //+       buffer, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (a vector of chars) where the
    //+                port's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.  This is optional and
    //+                can be determined from the length of the vector if not
    //+                specified.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    read (std::vector<char>& buffer, const size_t length = 0) {
        return m_sio_imp->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port implementation object is valid, and the buffer is at
    //+       least length bytes long.
    //! POST: The given buffer has length bytes copied into it from the port's
    //+       buffer, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer where the port's buffer
    //+                contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    readn (void* buffer, const size_t length) {
        return m_sio_imp->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port implementation object is valid, and the buffer is at
    //+      least length bytes long.
    //! POST: The given buffer has length bytes copied into it from the port's
    //+       buffer, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of unsigned chars)
    //+                where the port's buffer contents are to be stored.
    //! ARG:S length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    readn (unsigned char* buffer, const size_t length) {
        return m_sio_imp->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port implementation object is valid, and the buffer is at
    //+      least length bytes long.
    //! POST: The given buffer has length bytes copied into it from the port's
    //+       buffer, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of chars) where the
    //+                port's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    readn (char* buffer, const size_t length) {
        return m_sio_imp->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port implementation object is valid, and the buffer is at
    //+      least length bytes long.
    //! POST: The given buffer has length bytes copied into it from the port's
    //+       buffer, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::string object) where
    //+                the port's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.  This is optional and
    //+                can be determined from the length of the string object
    //+                if not specified.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    readn (std::string& buffer, const size_t length = 0) {
        return m_sio_imp->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port implementation object is valid, and the buffer is at
    //+      least length bytes long.
    //! POST: The given buffer has length bytes copied into it from the port's
    //+       buffer, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (a vector of chars) where the
    //+                port's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.  This is optional and
    //+                can be determined from the length of the vector if not
    //+                specified.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    readn (std::vector<char>& buffer, const size_t length = 0) {
        return m_sio_imp->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Write the buffer to the serial port.
    //
    //! PRE: The port implementation object is valid.
    //! POST: The given buffer is written to the serial port, and the number
    //+       of bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer to be written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    write (const void* buffer, const size_t length) {
        return m_sio_imp->write(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Write the buffer to the serial port.
    //
    //! PRE: The port implementation object is valid.
    //! POST: The given buffer is written to the serial port, and the number
    //+       of bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of unsigned chars) to
    //+                be written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    write (const unsigned char* buffer, const size_t length) {
        return m_sio_imp->write(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Write the buffer to the serial port.
    //
    //! PRE: The port implementation object is valid.
    //! POST: The given buffer is written to the serial port, and the number
    //+       of bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of chars) to be
    //+                written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    write (const char* buffer, const size_t length) {
        return m_sio_imp->write(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Write the buffer to the serial port.
    //
    //! PRE: The port implementation object is valid.
    //! POST: The given buffer is written to the serial port, and the number
    //+       of bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::string object) to be
    //+                written.
    //! ARGS: length - The length of the buffer.  This is optional and can be
    //+                determined from the length of the string object if not
    //+                specified.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    write (const std::string& buffer, const size_t length = 0) {
        return m_sio_imp->write(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Write the buffer to the serial port.
    //
    //! PRE: The port implementation object is valid.
    //! POST: The given buffer is written to the serial port, and the number
    //+       of bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (a vector of chars) to be
    //+                written.
    //! ARGS: length - The length of the buffer.  This is optional and can be
    //+                determined from the length of the vector if not
    //+                specified.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    write (const std::vector<char>& buffer, const size_t length = 0) {
        return m_sio_imp->write(buffer, length);
    }

    // ========================================================================
    // Serial port interface.
    // ========================================================================

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    drainOutput (void) {
        return m_sio_imp->drainOutput();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    controlFlow (vjSerialTypes::FlowActionOption opt) {
        return m_sio_imp->controlFlow(opt);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    flushQueue (vjSerialTypes::FlushQueueOption opt) {
        return m_sio_imp->flushQueue(opt);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    sendBreak (const int duration) {
        return m_sio_imp->sendBreak(duration);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual vjSerialTypes::UpdateActionOption
    getUpdateAction (void) {
        return m_sio_imp->getUpdateAction();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual void
    setUpdateAction (vjSerialTypes::UpdateActionOption action) {
        m_sio_imp->setUpdateAction(action);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual unsigned char
    getBufferSize (void) {
        return m_sio_imp->getBufferSize();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    setBufferSize (const unsigned char size) {
        return m_sio_imp->setBufferSize(size);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual unsigned char
    getTimeout () {
        return m_sio_imp->getTimeout();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    setTimeout (const unsigned char timeout_val) {
        return m_sio_imp->setTimeout(timeout_val);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual unsigned short
    getCharacterSize (void) {
        return m_sio_imp->getCharacterSize();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    setCharacterSize (const unsigned short bpb) {
        return m_sio_imp->setCharacterSize(bpb);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual void
    setControlCharacter (const unsigned int index, const unsigned char value) {
        m_sio_imp->setControlCharacter(index, value);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual unsigned char
    getControlCharacter (const unsigned int index) {
        return m_sio_imp->getControlCharacter(index);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual unsigned short
    getStopBits (void) {
        return m_sio_imp->getStopBits();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    setStopBits (const unsigned short num_bits) {
        return m_sio_imp->setStopBits(num_bits);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    getParity (void) {
        return m_sio_imp->getParity();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    setOddParity (void) {
        return m_sio_imp->setOddParity();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    setEvenParity (void) {
        return m_sio_imp->setEvenParity();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    getInputBaudRate (void) {
        return m_sio_imp->getInputBaudRate();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    setInputBaudRate (const int baud) {
        return m_sio_imp->setInputBaudRate(baud);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    getOutputBaudRate (void) {
        return m_sio_imp->getOutputBaudRate();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    setOutputBaudRate (const int baud) {
        return m_sio_imp->setOutputBaudRate(baud);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getLocalAttachState (void) {
        return m_sio_imp->getLocalAttachState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableLocalAttach (void) {
        return m_sio_imp->enableLocalAttach();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableLocalAttach (void) {
        return m_sio_imp->disableLocalAttach();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getReadState (void) {
        return m_sio_imp->getReadState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableRead (void) {
        return m_sio_imp->enableRead();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableRead (void) {
        return m_sio_imp->disableRead();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getInputEchoState (void) {
        return m_sio_imp->getInputEchoState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableInputEcho (void) {
        return m_sio_imp->enableInputEcho();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableInputEcho (void) {
        return m_sio_imp->disableInputEcho();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getEraseEchoState (void) {
        return m_sio_imp->getEraseEchoState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableEraseEcho (void) {
        return m_sio_imp->enableEraseEcho();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableEraseEcho (void) {
        return m_sio_imp->disableEraseEcho();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getKillEchoState (void) {
        return m_sio_imp->getKillEchoState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableKillEcho (void) {
        return m_sio_imp->enableKillEcho();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableKillEcho (void) {
        return m_sio_imp->disableKillEcho();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getNewlineEchoState (void) {
        return m_sio_imp->getNewlineEchoState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableNewlineEcho (void) {
        return m_sio_imp->enableNewlineEcho();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableNewlineEcho (void) {
        return m_sio_imp->disableNewlineEcho();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getCanonicalState (void) {
        return m_sio_imp->getCanonicalState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableCanonicalInput (void) {
        return m_sio_imp->enableCanonicalInput();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableCanonicalInput (void) {
        return m_sio_imp->disableCanonicalInput();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getCRTranslateState (void) {
        return m_sio_imp->getCRTranslateState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableCRTranslation (void) {
        return m_sio_imp->enableCRTranslation();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableCRTranslation (void) {
        return m_sio_imp->disableCRTranslation();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getExtendedHandlingState (void) {
        return m_sio_imp->getExtendedHandlingState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableExtendedHandling (void) {
        return m_sio_imp->enableExtendedHandling();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableExtendedHandling (void) {
        return m_sio_imp->disableExtendedHandling();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getCRIgnoreState (void) {
        return m_sio_imp->getCRIgnoreState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableCRIgnore (void) {
        return m_sio_imp->enableCRIgnore();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableCRIgnore (void) {
        return m_sio_imp->disableCRIgnore();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getBadByteIgnoreState (void) {
        return m_sio_imp->getBadByteIgnoreState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableBadByteIgnore (void) {
        return m_sio_imp->enableBadByteIgnore();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableBadByteIgnore (void) {
        return m_sio_imp->disableBadByteIgnore();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getParityCheckState (void) {
        return m_sio_imp->getParityCheckState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableParityCheck (void) {
        return m_sio_imp->enableParityCheck();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableParityCheck (void) {
        return m_sio_imp->disableParityCheck();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getSignalGenerateState (void) {
        return m_sio_imp->getSignalGenerateState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableSignalGeneration (void) {
        return m_sio_imp->enableSignalGeneration();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableSignalGeneration (void) {
        return m_sio_imp->disableSignalGeneration();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getBitStripState (void) {
        return m_sio_imp->getBitStripState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableBitStripping (void) {
        return m_sio_imp->enableBitStripping();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableBitStripping (void) {
        return m_sio_imp->disableBitStripping();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getStartStopInputState (void) {
        return m_sio_imp->getStartStopInputState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableStartStopInput (void) {
        return m_sio_imp->enableStartStopInput();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableStartStopInput (void) {
        return m_sio_imp->disableStartStopInput();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getStartStopOutputState (void) {
        return m_sio_imp->getStartStopOutputState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableStartStopOutput (void) {
        return m_sio_imp->enableStartStopOutput();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableStartStopOutput (void) {
        return m_sio_imp->disableStartStopOutput();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getNoFlushState (void) {
        return m_sio_imp->getNoFlushState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableNoFlush (void) {
        return m_sio_imp->enableNoFlush();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableNoFlush (void) {
        return m_sio_imp->disableNoFlush();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getParityGenerationState (void) {
        return m_sio_imp->getParityGenerationState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableParityGeneration (void) {
        return m_sio_imp->enableParityGeneration();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableParityGeneration (void) {
        return m_sio_imp->disableParityGeneration();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    getParityErrorMarkingState (void) {
        return m_sio_imp->getParityErrorMarkingState();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    enableParityErrorMarking (void) {
        return m_sio_imp->enableParityErrorMarking();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual int
    disableParityErrorMarking (void) {
        return m_sio_imp->disableParityErrorMarking();
    }

protected:
    vjSerialPortImp* m_sio_imp;    //: Platform-specific serial port
				   //+ implementation object
};


#endif	/* _VJ_SERIAL_PORT_H_ */
