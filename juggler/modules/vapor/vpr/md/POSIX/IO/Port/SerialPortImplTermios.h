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

#ifndef _VPR_SERIAL_PORT_IMP_TERMIOS_H_
#define _VPR_SERIAL_PORT_IMP_TERMIOS_H_

#include <termios.h>
#include <string>
#include <vector>

#include <IO/Port/SerialPortImp.h>
#include <md/POSIX/FileHandleUNIX.h>
#include <IO/Port/SerialTypes.h>


namespace vpr {

// ----------------------------------------------------------------------------
//: vpr::SerialPort implementation using termios.
// ----------------------------------------------------------------------------
class SerialPortImpTermios : public SerialPortImp {
public:
    // ------------------------------------------------------------------------
    //: Constructor.  This creates a file handle object connected to the given
    //+ port name and sets the update action to happen immediately.
    //
    //! PRE: None.
    //! POST: m_handle is allocated and connected to port_name, and the update
    //+       action is set to vpr::SerialTypes::NOW.
    //
    //! ARGS: port_name - The name of the serial port that will be accessed.
    // ------------------------------------------------------------------------
    SerialPortImpTermios(const std::string& port_name);

    // ------------------------------------------------------------------------
    //: Destructor.  If the file handle is non-NULL, its memory is released.
    //
    //! PRE: None.
    //! POST: If m_handle is non-NULL, its memory is released.
    // ------------------------------------------------------------------------
    virtual ~SerialPortImpTermios(void);

    // ========================================================================
    // vpr::BlockIO overrides.
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
        m_handle->setOpenReadOnly();
    }

    // ------------------------------------------------------------------------
    //: Set the open flags so that the serial port is opened in write-only
    //: mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the port is opened, it
    //+       is opened in write-only mode.  If the port is already open, this
    //+       has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenWriteOnly (void) {
        m_handle->setOpenWriteOnly();
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
        m_handle->setOpenReadWrite();
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
        m_handle->setOpenBlocking();
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
        m_handle->setOpenNonBlocking();
    }

    // ------------------------------------------------------------------------
    //: Open the serial port and initialize its flags.
    //
    //! PRE: The serial port is not already open.
    //! POST: An attempt is made to open the port.  If it is successful, the
    //+       port's flags are initaialized to 0.  The resulting status is
    //+       returned to the caller.  If the port is opened, m_open is set to
    //+       true.
    //
    //! RETURNS: true  - The serial port was opened successfully.
    //! RETURNS: false - The serial port could not be opened
    // ------------------------------------------------------------------------
    virtual bool open(void);

    // ------------------------------------------------------------------------
    //: Close the serial port.
    //
    //! PRE: The serial port is open.
    //! POST: An attempt is made to close the port.  The resulting status is
    //+       returned to the caller.  If the port is closed, m_open is set to
    //+       false.
    //
    //! RETURNS: true  - The serial port was closed successfully.
    //! RETURNS: false - The serial port could not be closed for some reason.
    // ------------------------------------------------------------------------
    inline virtual bool
    close (void) {
        bool retval;

        if ( (retval = m_handle->close()) ) {
            m_open = false;
        }

        return retval;
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the serial port so that it is in blocking mode.
    //
    //! PRE: The serial port is open.
    //! POST: Processes will block when accessing the port.
    //
    //! RETURNS: true  - The blocking mode was changed successfully.
    //! RETURNS: false - The blocking mode could not be changed for some
    //+                  reason.
    // ------------------------------------------------------------------------
    inline virtual bool
    enableBlocking (void) {
       return m_handle->enableBlocking();
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the serial port so that it is in non-blocking mode.
    //
    //! PRE: The serial port is open.
    //! POST: Processes will not block when accessing the port.
    //
    //! RETURNS:  0 - The blocking mode was changed successfully.
    //! RETURNS: -1 - The blocking mode could not be changed for some reason.
    // ------------------------------------------------------------------------
    inline virtual bool
    enableNonBlocking (void) {
       return m_handle->enableNonBlocking();
    }

    // ------------------------------------------------------------------------
    //! Read up to the specified number of bytes from the serial port into the
    //+ given buffer.
    //
    //! PRE: The port's file handle is valid, and the buffer is at least
    //+      length bytes long.
    //! POST: The given buffer has length bytes copied into it from the serial
    //+       port, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer where the data read from the
    //+                port is to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    read (void* buffer, const size_t length) {
        return m_handle->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    //! Read up to the specified number of bytes from the serial port into the
    //+ given buffer.
    //
    //! PRE: The port's file handle is valid, and the buffer is at least
    //+      length bytes long.
    //! POST: The given buffer has length bytes copied into it from the serial
    //+       port, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of unsigned chars)
    //+                where the data read from the port is to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    read (unsigned char* buffer, const size_t length) {
        return m_handle->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    //! Read up to the specified number of bytes from the serial port into the
    //+ given buffer.
    //
    //! PRE: The port's file handle is valid, and the buffer is at least
    //+      length bytes long.
    //! POST: The given buffer has length bytes copied into it from the serial
    //+       port, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of chars) where the
    //+                data read from the port is to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    read (char* buffer, const size_t length) {
        return m_handle->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    //! Read up to the specified number of bytes from the serial port into the
    //+ given buffer.
    //
    //! PRE: The port's file handle is valid, and the buffer is at least
    //+      length bytes long.
    //! POST: The given buffer has length bytes copied into it from the serial
    //+       port, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::string object) where
    //+                the data read from the port is to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    read (std::string& buffer, const size_t length) {
        return m_handle->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    //! Read up to the specified number of bytes from the serial port into the
    //+ given buffer.
    //
    //! PRE: The port's file handle is valid, and the buffer is at least
    //+      length bytes long.
    //! POST: The given buffer has length bytes copied into it from the serial
    //+       port, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (a vector of chars) where the
    //+                data read from the port is to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    read (std::vector<char>& buffer, const size_t length) {
        return m_handle->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port's file handle is valid, and the buffer is at least
    //+      length bytes long.
    //! POST: The given buffer has length bytes copied into it from the serial
    //+       port, and the number of bytes read successfully is returned to
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
        return m_handle->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port's file handle is valid, and the buffer is at least
    //+      length bytes long.
    //! POST: The given buffer has length bytes copied into it from the serial
    //+       port, and the number of bytes read successfully is returned to
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
    readn (unsigned char* buffer, const size_t length) {
        return m_handle->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port's file handle is valid, and the buffer is at least
    //+      length bytes long.
    //! POST: The given buffer has length bytes copied into it from the serial
    //+       port, and the number of bytes read successfully is returned to
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
        return m_handle->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port's file handle is valid, and the buffer is at least
    //+      length bytes long.
    //! POST: The given buffer has length bytes copied into it from the serial
    //+       port, and the number of bytes read successfully is returned to
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
        return m_handle->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port's file handle is valid, and the buffer is at least
    //+      length bytes long.
    //! POST: The given buffer has length bytes copied into it from the serial
    //+       port, and the number of bytes read successfully is returned to
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
        return m_handle->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Write the buffer to the serial port.
    //
    //! PRE: The port's file handle is valid.
    //! POST: The given buffer is written to the serial port, and the number
    //+       of bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer to be written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the serail
    //+                port.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    write (const void* buffer, const size_t length) {
        return m_handle->write(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Write the buffer to the serial port.
    //
    //! PRE: The port's file handle is valid.
    //! POST: The given buffer is written to the serial port, and the number
    //+       of bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of unsigned chars) to
    //+                be written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the serail
    //+                port.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    write (const unsigned char* buffer, const size_t length) {
        return m_handle->write(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Write the buffer to the serial port.
    //
    //! PRE: The port's file handle is valid.
    //! POST: The given buffer is written to the serial port, and the number
    //+       of bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of chars) to be
    //+                written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the serail
    //+                port.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    write (const char* buffer, const size_t length) {
        return m_handle->write(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Write the buffer to the serial port.
    //
    //! PRE: The port's file handle is valid.
    //! POST: The given buffer is written to the serial port, and the number
    //+       of bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::string object) to be
    //+                written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the serail
    //+                port.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    write (const std::string& buffer, const size_t length) {
        return m_handle->write(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Write the buffer to the serial port.
    //
    //! PRE: The port's file handle is valid.
    //! POST: The given buffer is written to the serial port, and the number
    //+       of bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (a vector of chars) to be
    //+                written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the serail
    //+                port.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    write (const std::vector<char>& buffer, const size_t length) {
        return m_handle->write(buffer, length);
    }

    // ========================================================================
    // vpr::SerialPortImp interface implementation.
    // ========================================================================

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual SerialTypes::UpdateActionOption getUpdateAction(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual void setUpdateAction(SerialTypes::UpdateActionOption action);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getLocalAttachState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableLocalAttach(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableLocalAttach(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual unsigned char getBufferSize(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int setBufferSize(const unsigned char size);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual unsigned char getTimeout(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int setTimeout(const unsigned char timeout);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual unsigned short getCharacterSize(void);

    // ------------------------------------------------------------------------
    //: Set the character size for both reading and writing on this port.
    //+ This size does not include the parity bit if any.  Possible values are
    //+ 5, 6, 7 or 8 bits per byte.
    // ------------------------------------------------------------------------
    virtual int setCharacterSize(const unsigned short bpb);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getReadState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableRead(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableRead(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual unsigned short getStopBits(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int setStopBits(const unsigned short num_bits);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getInputEchoState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableInputEcho(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableInputEcho(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getEraseEchoState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableEraseEcho(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableEraseEcho(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getKillEchoState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableKillEcho(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableKillEcho(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getNewlineEchoState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableNewlineEcho(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableNewlineEcho(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getCanonicalState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableCanonicalInput(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableCanonicalInput(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getCRTranslateState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableCRTranslation(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableCRTranslation(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getExtendedHandlingState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableExtendedHandling(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableExtendedHandling(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getCRIgnoreState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableCRIgnore(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableCRIgnore(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getBadByteIgnoreState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableBadByteIgnore(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableBadByteIgnore(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getParityCheckState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableParityCheck(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableParityCheck(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getSignalGenerateState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableSignalGeneration(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableSignalGeneration(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getBitStripState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableBitStripping(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableBitStripping(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getStartStopInputState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableStartStopInput(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableStartStopInput(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getStartStopOutputState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableStartStopOutput(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableStartStopOutput(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getNoFlushState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableNoFlush(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableNoFlush(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getParityGenerationState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableParityGeneration(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableParityGeneration(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool getParityErrorMarkingState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int enableParityErrorMarking(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int disableParityErrorMarking(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int getParity(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int setOddParity(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int setEvenParity(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int getInputBaudRate(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int setInputBaudRate(const int baudvoid);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int getOutputBaudRate(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int setOutputBaudRate(const int baudvoid);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int drainOutput(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int controlFlow(SerialTypes::FlowActionOption opt);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int flushQueue(SerialTypes::FlushQueueOption opt);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int sendBreak(const int duration);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual void setControlCharacter(const unsigned int index,
                                     const unsigned char value);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual unsigned char getControlCharacter(const unsigned int index);

protected:
    // Symbolic names for the four types of flags that can be set in the
    // termios struct.
    enum _term_flag {
        IFLAG,		// Input flag
        OFLAG,		// Output flag
        CFLAG,		// Control flag
        LFLAG		// Local flag
    };

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int getAttrs(struct termios* term);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int setAttrs(struct termios* term, const char* err_msg,
                         const bool print_sys_err = true);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int setAttrs(struct termios* term, const std::string& err_msg,
                         const bool print_sys_err = true);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getBit(const tcflag_t bit, _term_flag flag);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual int setBit(const tcflag_t bit, _term_flag flag, const bool enable,
                       const std::string& err_msg,
                       const bool print_sys_err = true);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    int baudToInt(const speed_t baud_rate);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    speed_t intToBaud(const int speed_int);

    FileHandleUNIX*	m_handle;    //: File handle for the serial port
    int			m_actions;
};

}; // End of vpr namespace


#endif	/* _VPR_SERIAL_PORT_IMP_TERMIOS_H_ */
