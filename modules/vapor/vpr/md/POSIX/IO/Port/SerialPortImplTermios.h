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

#include <IO/Port/Port.h>
#include <IO/Port/SerialTypes.h>
#include <md/POSIX/FileHandleUNIX.h>


namespace vpr {

// ----------------------------------------------------------------------------
//: vpr::SerialPort implementation using termios.
// ----------------------------------------------------------------------------
class SerialPortImpTermios : public Port {
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
    virtual Status open(void);

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
    inline virtual Status
    close (void) {
        Status retval;

        retval = m_handle->close();
        m_open = (retval.success() ? false : true);

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
    inline virtual Status
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
    inline virtual Status
    enableNonBlocking (void) {
       return m_handle->enableNonBlocking();
    }

    // ========================================================================
    // vpr::SerialPortImp interface implementation.
    // ========================================================================

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    SerialTypes::UpdateActionOption getUpdateAction(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    void setUpdateAction(SerialTypes::UpdateActionOption action);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getLocalAttachState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status enableLocalAttach(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status disableLocalAttach(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status getBufferSize(Uint8& size);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status setBufferSize(const Uint8 size);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status getTimeout(Uint8& timeout);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status setTimeout(const Uint8 timeout);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status getCharacterSize(SerialTypes::CharacterSizeOption& size);

    // ------------------------------------------------------------------------
    //: Set the character size for both reading and writing on this port.
    //+ This size does not include the parity bit if any.
    // ------------------------------------------------------------------------
    Status setCharacterSize(const SerialTypes::CharacterSizeOption bpb);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getReadState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status enableRead(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status disableRead(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status getStopBits(Uint8& num_bits);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status setStopBits(const Uint8 num_bits);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getInputEchoState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status enableInputEcho(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status disableInputEcho(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getCanonicalState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status enableCanonicalInput(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status disableCanonicalInput(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getCRTranslateState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status enableCRTranslation(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status disableCRTranslation(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getCRIgnoreState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status enableCRIgnore(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status disableCRIgnore(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getBadByteIgnoreState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status enableBadByteIgnore(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status disableBadByteIgnore(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getParityCheckState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status enableParityCheck(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status disableParityCheck(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getSignalGenerateState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status enableSignalGeneration(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status disableSignalGeneration(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getBitStripState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status enableBitStripping(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status disableBitStripping(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getStartStopInputState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status enableStartStopInput(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status disableStartStopInput(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getStartStopOutputState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status enableStartStopOutput(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status disableStartStopOutput(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getNoFlushState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status enableNoFlush(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status disableNoFlush(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getParityGenerationState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status enableParityGeneration(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status disableParityGeneration(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getParityErrorMarkingState(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status enableParityErrorMarking(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status disableParityErrorMarking(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Int8 getParity(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status setOddParity(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status setEvenParity(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status getInputBaudRate(Int32& rate);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status setInputBaudRate(const Int32 rate);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status getOutputBaudRate(Int32& rate);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status setOutputBaudRate(const Int32 rate);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status drainOutput(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status controlFlow(SerialTypes::FlowActionOption opt);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status flushQueue(SerialTypes::FlushQueueOption opt);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status sendBreak(const Int32 duration);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    void setControlCharacter(const Uint32 index, const Uint8 value);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Uint8 getControlCharacter(const Uint32 index);

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
    virtual Status
    read_i (void* buffer, const size_t length, ssize_t& bytes_read) {
        return m_handle->read(buffer, length, bytes_read);
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
    virtual Status
    readn_i (void* buffer, const size_t length, ssize_t& bytes_read) {
        return m_handle->readn(buffer, length, bytes_read);
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
    virtual Status
    write_i (const void* buffer, const size_t length, ssize_t& bytes_written) {
        return m_handle->write(buffer, length, bytes_written);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status getAttrs(struct termios* term);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status setAttrs(struct termios* term, const char* err_msg,
                    const bool print_sys_err = true);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status setAttrs(struct termios* term, const std::string& err_msg,
                    const bool print_sys_err = true);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getBit(const tcflag_t bit, _term_flag flag);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status setBit(const tcflag_t bit, _term_flag flag, const bool enable,
                  const std::string& err_msg, const bool print_sys_err = true);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Int32 baudToInt(const speed_t baud_rate);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    speed_t intToBaud(const Int32 speed_int);

    FileHandleUNIX*	m_handle;    //: File handle for the serial port
    int			m_actions;
};

}; // End of vpr namespace


#endif	/* _VPR_SERIAL_PORT_IMP_TERMIOS_H_ */
