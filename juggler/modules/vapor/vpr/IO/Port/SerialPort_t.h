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

#ifndef _VPR_SERIAL_PORT_T_H_
#define _VPR_SERIAL_PORT_T_H_


namespace vpr {

// ----------------------------------------------------------------------------
//: Cross-platform serial port interface.
// ----------------------------------------------------------------------------
//!PUBLIC_API:
template<class RealSerialPortImpl>
class SerialPort_t : public Port {
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
    SerialPort_t (const std::string& port_name)
        : Port(port_name), m_sio_imp(port_name)
    {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    //: Destructor.  This does nothing.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    virtual ~SerialPort_t (void) {
        /* Do nothing. */ ;
    }

    // ========================================================================
    // Block I/O device interface.
    // ========================================================================

    // ------------------------------------------------------------------------
    //: Get the name of this serial port.
    //
    //! PRE: None.
    //! POST: A constant reference to the m_name object is returned to the
    //+       caller.
    //
    //! RETURNS: An object containing the name of this serial port.
    // ------------------------------------------------------------------------
    virtual const std::string&
    getName (void) {
        return m_sio_imp.getName();
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
    inline void
    setOpenBlocking (void) {
        m_sio_imp.setOpenBlocking();
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
    inline void
    setOpenNonBlocking (void) {
        m_sio_imp.setOpenNonBlocking();
    }

    // ------------------------------------------------------------------------
    //: Open the serial port.
    //
    //! PRE: The serial port is not already open.
    //! POST: An attempt is made to open the serial port.  The resulting
    //+       status is returned to the caller.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    open (void) {
        return m_sio_imp.open();
    }

    // ------------------------------------------------------------------------
    //: Close the serial port.
    //
    //! PRE: The serial port is open.
    //! POST: An attempt is made to close the serial port.  The resulting
    //+       status is returned to the caller.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    close (void) {
        return m_sio_imp.close();
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the serial port so that it is in blocking mode.
    //
    //! PRE: The port is open.
    //! POST: Processes will block when accessing the port.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    enableBlocking (void) {
        return m_sio_imp.enableBlocking();
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the serial port so that it is in non-blocking mode.
    //
    //! PRE: The port is open.
    //! POST: Processes will not block when accessing the port.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline virtual vpr::Status
    enableNonBlocking (void) {
        return m_sio_imp.enableNonBlocking();
    }

    // ------------------------------------------------------------------------
    //: Get the current blocking state for the serial port.
    //
    //! PRE: m_blocking is set correctly
    //! POST:
    //
    //! RETURNS: true  - The port is in blocking mode.
    //! RETURNS: false - The port is in non-blocking mode.
    // ------------------------------------------------------------------------
    inline bool
    getBlocking (void) const {
        return m_sio_imp.getBlocking();
    }

    // ------------------------------------------------------------------------
    //: Get the current non-blocking state for the serial port.
    //
    //! PRE: m_blocking is set correctly
    //! POST:
    //
    //! RETURNS: true  - The port is in non-blocking mode.
    //! RETURNS: false - The port is in blocking mode.
    // ------------------------------------------------------------------------
    inline bool
    getNonBlocking (void) const {
        return m_sio_imp.getNonBlocking();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual IOSys::Handle
    getHandle (void) {
        return m_sio_imp.getHandle();
    }

    // ========================================================================
    // Serial port interface.
    // ========================================================================

    // ------------------------------------------------------------------------
    //: Get the current update action.  This tells when updates to the serial
    //+ device take effect.
    //
    //! PRE: None.
    //! POST: The current update action is returned to the caller.
    //
    //! RETURNS: A vpr::SerialTypes::UpdateActionOption value stating when
    //+          updates take effect.
    // ------------------------------------------------------------------------
    inline SerialTypes::UpdateActionOption
    getUpdateAction (void) {
        return m_sio_imp.getUpdateAction();
    }

    // ------------------------------------------------------------------------
    //: Change the current update action to take place as described by the
    //+ given value.
    //
    //! PRE: None.
    //! POST: The update action is modified to use the new value.  The results
    //+       will be seen with the next device operation.
    //
    //! ARGS: action - The new update action value.
    // ------------------------------------------------------------------------
    inline void
    setUpdateAction (SerialTypes::UpdateActionOption action) {
        m_sio_imp.setUpdateAction(action);
    }

    // ------------------------------------------------------------------------
    //: Query the serial port for the minimum buffer size.  This is only
    //+ applicable in non-canonical mode.
    //
    //! PRE: The serial port is open.
    //! POST: The maximum buffer size is returned to the caller through the
    //+       by-reference argument.
    //
    //! ARGS: size - A reference to a vpr::Uint16 where the buffer size is
    //+              stored for return to the caller.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    getBufferSize (vpr::Uint16& size) {
        return m_sio_imp.getBufferSize(size);
    }

    // ------------------------------------------------------------------------
    //: Attempt to change the minimum buffer size to the given argument.  This
    //+ will change the minimum number of bytes required in the buffer before
    //+ a read() will return.  This is only applicable in non-canonical mode.
    //
    //! PRE: The serial port is open.
    //! POST: If the buffer size is usable on the port, the port attributes
    //+       are updated and success is returned.  Otherwise, a failure
    //+       status is returned.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    setBufferSize (const vpr::Uint8 size) {
        return m_sio_imp.setBufferSize(size);
    }

    // ------------------------------------------------------------------------
    //: Get the value of the timeout (in tenths of a second) to wait for data
    //+ to arrive.  This is only applicable in non-canonical mode.
    //
    //! PRE: The serial port is open.
    //! POST: The current timeout setting is returned to the caller in the
    //+       by-reference argument.
    //
    //! ARGS: timeout - A reference to a vpr::Uint8 to be used as storage for
    //+                 the timeout value.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    getTimeout (vpr::Uint8& timeout) {
        return m_sio_imp.getTimeout(timeout);
    }

    // ------------------------------------------------------------------------
    //: Set the value of the timeout to wait for data to arrive.  The value
    //+ given must be in tenths of a second.  This is only applicable in
    //+ non-canonical mode.
    //
    //! PRE: The serial port is open.
    //! POST: The timeout interval is updated to use the given value.
    //
    //! ARGS: timeout_val - The new timeout value measured in tenths of a
    //+                     second.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    setTimeout (const vpr::Uint8 timeout_val) {
        return m_sio_imp.setTimeout(timeout_val);
    }

    // ------------------------------------------------------------------------
    //: Get the character size (the bits per byte).
    //
    //! PRE: The serial port is open.
    //! POST: The current character size is returned to the caller in the
    //+       by-reference argument.
    //
    //! ARGS: size - A reference to a vpr::SerialTypes::CharacterSizeOption
    //+              object to be used as storage for the character size
    //+              value.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    getCharacterSize (SerialTypes::CharacterSizeOption& size) {
        return m_sio_imp.getCharacterSize(size);
    }

    // ------------------------------------------------------------------------
    //: Set the current character size (the bits per byte) to the size in the
    //+ given value.  This is used for both reding and writing, and the size
    //+ does not include the parity bit (if any).
    //
    //! PRE: The serial port is open.
    //! POST: The bits per byte for the serial port is updated.
    //
    //! ARGS: bpb - The new character size.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    setCharacterSize (const SerialTypes::CharacterSizeOption bpb) {
        return m_sio_imp.setCharacterSize(bpb);
    }

    // ------------------------------------------------------------------------
    //: Get the number of stop bits in use.  This will be either 1 or 2.
    //
    //! PRE: The serial port is open.
    //! POST: The number of stop bits currently in use is returned via the
    //+       by-reference argument.
    //
    //! ARGS: num_bits - A reference to a vpr::Uint8 that will be used to
    //+                  store the number of stop bits.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    getStopBits (vpr::Uint8& num_bits) {
        return m_sio_imp.getStopBits(num_bits);
    }

    // ------------------------------------------------------------------------
    //: Set the number of stop bits to use.  The value must be either 1 or 2.
    //
    //! PRE: The serial port is open, and the given number of stop bits is
    //+      either 1 or 2.
    //! POST: The number of stop bits is updated to use the given value.
    //
    //! ARGS: num_bits - The number of stop bits to use.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    setStopBits (const vpr::Uint8 num_bits) {
        return m_sio_imp.setStopBits(num_bits);
    }

    // ------------------------------------------------------------------------
    //: Query the canonical input state of the serial port.  If canonical mode
    //+ is enabled, the characters EOF, EOL, EOL2, ERASE, KILL, REPRINT,
    //+ STATUS, and WERASE are enabled, and the input characters are
    //+ assembled into lines.  Otherwise, read requests are satisfied directly
    //+ from the input queue, and a read will not return until the buffer is
    //+ at its minimum capacity or the timeout has expired.  See
    //+ getBufferSize() and getTimeout() for more information.
    //
    //! PRE: The serial port is open.
    //! POST: The current state of canonical input is returnd to the caller.
    //
    //! RETURNS: true  - Canonical input is enabled.
    //! RETURNS: false - Canonical input is disabled.
    // ------------------------------------------------------------------------
    inline bool
    getCanonicalState (void) {
        return m_sio_imp.getCanonicalState();
    }

    // ------------------------------------------------------------------------
    //: Enable canonical input.  See getCanonicalState() for more information
    //+ about what this means.
    //
    //! PRE: The serial port is open.
    //! POST: Canonical input is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    enableCanonicalInput (void) {
        return m_sio_imp.enableCanonicalInput();
    }

    // ------------------------------------------------------------------------
    //: Disable canonical input.  See getCanonicalState() for more information
    //+ about what this means.
    //
    //! PRE: The serial port is open.
    //! POST: Canonical input is disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    disableCanonicalInput (void) {
        return m_sio_imp.disableCanonicalInput();
    }

    // ------------------------------------------------------------------------
    //: Get the current state of ignoring bytes with framing errors (other
    //+ than a BREAK) or parity errors.
    //
    //! PRE: The serial port is open.
    //! POST: The erroneous byte ignore state is returned to the caller.
    //
    //! RETURNS: true  - Bad bytes are ignored.
    //! RETURNS: false - Bad bytes are not ignored.
    // ------------------------------------------------------------------------
    inline bool
    getBadByteIgnoreState (void) {
        return m_sio_imp.getBadByteIgnoreState();
    }

    // ------------------------------------------------------------------------
    //: Enable ignoring of received bytes with framing errors or parity
    //+ errors.
    //
    //! PRE: The serial port is open.
    //! POST: Erroneous byte ignoring is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    enableBadByteIgnore (void) {
        return m_sio_imp.enableBadByteIgnore();
    }

    // ------------------------------------------------------------------------
    //: Disable ignoring of received bytes with framing errors or parity
    //+ errors.
    //
    //! PRE: The serial port is open.
    //! POST: Erroneous byte ignoring is disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    disableBadByteIgnore (void) {
        return m_sio_imp.disableBadByteIgnore();
    }

    // ------------------------------------------------------------------------
    //: Get the state of parity checking for input.
    //
    //! PRE: The serial port is open.
    //! POST: The state of input parity checking is returned to the caller.
    //
    //! RETURNS: true  - Input parity checking is enabled.
    //! RETURNS: false - Input parity checking is disabled.
    // ------------------------------------------------------------------------
    inline bool
    getInputParityCheckState (void) {
        return m_sio_imp.getInputParityCheckState();
    }

    // ------------------------------------------------------------------------
    //: Enable input parity checking.
    //
    //! PRE: The serial port is open.
    //! POST: Input parity checking is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    enableInputParityCheck (void) {
        return m_sio_imp.enableInputParityCheck();
    }

    // ------------------------------------------------------------------------
    //: Disable input parity checking.
    //
    //! PRE: The serial port is open.
    //! POST: Input parity checking is disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    disableInputParityCheck (void) {
        return m_sio_imp.disableInputParityCheck();
    }

    // ------------------------------------------------------------------------
    //: Get the current state of bit stripping.  When enabled, input bytes are
    //+ stripped to seven bits.  Otherwise, all eight bits are processed.
    //
    //! PRE: The serial port is open.
    //! POST: The current bit stripping state is returned to the caller.
    //
    //! RETURNS: true  - Input bytes are stripped to seven bits.
    //! RETURNS: false - Input bytes are not stripped.
    // ------------------------------------------------------------------------
    inline bool
    getBitStripState (void) {
        return m_sio_imp.getBitStripState();
    }

    // ------------------------------------------------------------------------
    //: Enable stripping of input bytes to seven bits.
    //
    //! PRE: The serial port is open.
    //! POST: Input bit stripping is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    enableBitStripping (void) {
        return m_sio_imp.enableBitStripping();
    }

    // ------------------------------------------------------------------------
    //: Disable stripping of input bytes to seven bits.
    //
    //! PRE: The serial port is open.
    //! POST: Input bit stripping is disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    disableBitStripping (void) {
        return m_sio_imp.disableBitStripping();
    }

    // ------------------------------------------------------------------------
    //: Get the state of start-stop input control.  When enabled, if the
    //+ terminal driver sees that the receive buffer is getting full, a STOP
    //+ command is sent to the serial device.  The device should respond by
    //+ halting its data flow.  When the input queue has been processed, a
    //+ START command is sent to the serial device which should cause it to
    //+ resume putting bytes onto the input queue.
    //
    //! PRE: The serial port is open.
    //! POST: The current state of start-stop input control is returned to the
    //+       caller.
    //
    //! RETURNS: true  - Start-stop input control is enabled.
    //! RETURNS: true  - Start-stop input control is not enabled.
    // ------------------------------------------------------------------------
    inline bool
    getStartStopInputState (void) {
        return m_sio_imp.getStartStopInputState();
    }

    // ------------------------------------------------------------------------
    //: Enable start-stop input control.  See getStartStopInputState() for
    //+ more information.
    //
    //! PRE: The serial port is open.
    //! POST: Start-stop input control is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    enableStartStopInput (void) {
        return m_sio_imp.enableStartStopInput();
    }

    // ------------------------------------------------------------------------
    //: Disable start-stop input control.  See getStartStopInputState() for
    //+ more information.
    //
    //! PRE: The serial port is open.
    //! POST: Start-stop input control is disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    disableStartStopInput (void) {
        return m_sio_imp.disableStartStopInput();
    }

    // ------------------------------------------------------------------------
    //: Get the state of start-stop output control.  When enabled, when the
    //+ terminal driver receives a STOP command, output stops.  When it
    //+ receives a START command, output resumes.  If disabled, these
    //+ commands are read as normal characters.
    //
    //! PRE: The serial port is open.
    //! POST: The current state of start-stop output control is returned to
    //+       the caller.
    //
    //! RETURNS: true  - Start-stop output control is enabled.
    //! RETURNS: true  - Start-stop output control is not enabled.
    // ------------------------------------------------------------------------
    inline bool
    getStartStopOutputState (void) {
        return m_sio_imp.getStartStopOutputState();
    }

    // ------------------------------------------------------------------------
    //: Disable start-stop input control.  See getStartStopInputState() for
    //+ more information.
    //
    //! PRE: The serial port is open.
    //! POST: Start-stop input control is disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    enableStartStopOutput (void) {
        return m_sio_imp.enableStartStopOutput();
    }

    // ------------------------------------------------------------------------
    //: Disable start-stop output control.  See getStartStopOutputState() for
    //+ more information.
    //
    //! PRE: The serial port is open.
    //! POST: Start-stop output control is disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    disableStartStopOutput (void) {
        return m_sio_imp.disableStartStopOutput();
    }

    // ------------------------------------------------------------------------
    //: Get the current state of parity generation for outgoing bytes and
    //+ parity checking for incoming bytes.
    //
    //! PRE: The serial port is open.
    //! POST: The current parity generation/checking state is returned to the
    //+       caller.
    //
    //! RETURNS: true  - Parity generation and checking are enabled for
    //+                  outgoing and incoming bytes respectively.
    //! RETURNS: false - Parity generation and checking are disabled.
    // ------------------------------------------------------------------------
    inline bool
    getParityGenerationState (void) {
        return m_sio_imp.getParityGenerationState();
    }

    // ------------------------------------------------------------------------
    //: Enable parity generation for outgoing bytes and parity checking for
    //+ incoming bytes.
    //
    //! PRE: The serial port is open.
    //! POST: Parity generation and checking are enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    enableParityGeneration (void) {
        return m_sio_imp.enableParityGeneration();
    }

    // ------------------------------------------------------------------------
    //: Disable parity generation for outgoing bytes and parity checking for
    //+ incoming bytes.
    //
    //! PRE: The serial port is open.
    //! POST: Parity generation and checking are disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    disableParityGeneration (void) {
        return m_sio_imp.disableParityGeneration();
    }

    // ------------------------------------------------------------------------
    //: Enable marking of bytes with parity errors or framing errors (except
    //+ BREAKs).  This is only active if input parity and framing error
    //+ reporting is enabled (see getInputParityCheckState() for more
    //+ information).  The mark is the three-byte sequence \377 \0 X where X
    //+ is the byte received in error.  If bit stripping is enabled, a valid
    //+ \377 byte is passed as the two-byte sequence \377 \377.
    //
    //! PRE: The serial port is open.
    //! POST: Bytes with an error are marked and passed on to the reader.
    //
    //! RETURNS: true  - Parity error marking is enabled.
    //! RETURNS: false - Parity error marking is disabled.
    // ------------------------------------------------------------------------
    inline bool
    getParityErrorMarkingState (void) {
        return m_sio_imp.getParityErrorMarkingState();
    }

    // ------------------------------------------------------------------------
    //: Enable parity error and framing error marking.
    //
    //! PRE: The serial port is open.
    //! POST: Parity error marking is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    enableParityErrorMarking (void) {
        return m_sio_imp.enableParityErrorMarking();
    }

    // ------------------------------------------------------------------------
    //: Disable parity error and framing error marking.
    //
    //! PRE: The serial port is open.
    //! POST: Parity error marking is disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    disableParityErrorMarking (void) {
        return m_sio_imp.disableParityErrorMarking();
    }

    // ------------------------------------------------------------------------
    //: Get the current parity checking type (either odd or even).
    //
    //! PRE: The serial port is open.
    //! POST: The parity type is returned to the caller.
    //
    //! RETURNS: A vpr::SerialTypes::ParityType value giving the parity
    //+          checking being done.
    // ------------------------------------------------------------------------
    inline vpr::SerialTypes::ParityType
    getParity (void) {
        return m_sio_imp.getParity();
    }

    // ------------------------------------------------------------------------
    //: Enable odd parity.
    //
    //! PRE: The serial port is open.
    //! POST: Odd parity is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    setOddParity (void) {
        return m_sio_imp.setOddParity();
    }

    // ------------------------------------------------------------------------
    //: Enable even parity.
    //
    //! PRE: The serial port is open.
    //! POST: Even parity is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    setEvenParity (void) {
        return m_sio_imp.setEvenParity();
    }

    // ------------------------------------------------------------------------
    //: Get the current input baud rate.
    //
    //! PRE: The serial port is open.
    //! POST: The current input baud rate is returned to the caller via the
    //+       by-reference parameter.
    //
    //! ARGS: rate - A reference to a vpr::Uint32 to be used as storage for
    //+              the current input baud rate.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    getInputBaudRate (vpr::Uint32& rate) {
        return m_sio_imp.getInputBaudRate(rate);
    }

    // ------------------------------------------------------------------------
    //: Set the current input baud rate.
    //
    //! PRE: The serial port is open, and the rate is a valid baud rate.
    //! POST: The input baud rate is updated to use the given value.
    //
    //! ARGS: rate - The new input baud rate.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    setInputBaudRate (const vpr::Uint32 baud) {
        return m_sio_imp.setInputBaudRate(baud);
    }

    // ------------------------------------------------------------------------
    //: Get the current output baud rate.
    //
    //! PRE: The serial port is open.
    //! POST: The current output baud rate is returned to the caller via the
    //+       by-reference parameter.
    //
    //! ARGS: rate - A reference to a vpr::Uint32 to be used as storage for
    //+              the current output baud rate.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    getOutputBaudRate (vpr::Uint32& rate) {
        return m_sio_imp.getOutputBaudRate(rate);
    }

    // ------------------------------------------------------------------------
    //: Set the current output baud rate.
    //
    //! PRE: The serial port is open, and the rate is a valid baud rate.
    //! POST: The output baud rate is updated to use the given value.
    //
    //! ARGS: rate - The new output baud rate.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    setOutputBaudRate (const vpr::Uint32 baud) {
        return m_sio_imp.setOutputBaudRate(baud);
    }

    // ------------------------------------------------------------------------
    //: Wait for all output to be transmitted.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    drainOutput (void) {
        return m_sio_imp.drainOutput();
    }

    // ------------------------------------------------------------------------
    //: Alter the input or output flow control.  Based on the
    //+ vpr::SerialTypes::FlowActionOption argument, output can be suspended
    //+ and restarted or the terminal device can be told to stop or to resume
    //+ sending data.
    //
    //! PRE: The serial device is open.
    //! POST: Based on the argument value, the input or output flow control is
    //+       updated.
    //
    //! ARGS: opt - The flow control change.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    controlFlow (SerialTypes::FlowActionOption opt) {
        return m_sio_imp.controlFlow(opt);
    }

    // ------------------------------------------------------------------------
    //: Discard either the input buffer (unread data received from the
    //+ terminal device) or the output buffer (data written but not yet
    //+ transmitted to the terminal device).  The argument tells which queue
    //+ (or queues) to flush.
    //
    //! PRE: The serial device is open.
    //! POST: Based on the argument, the appropriate queue is flushed of its
    //+       current contents.
    //
    //! ARGS: queue - The queue (or queues) to be flushed.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    flushQueue (SerialTypes::FlushQueueOption queue) {
        return m_sio_imp.flushQueue(queue);
    }

    // ------------------------------------------------------------------------
    //: Transmit a continuous stream of zero bits for the given duration.  If
    //+ the argument is 0, the transmission will last between 0.25 and 0.5
    //+ seconds.  Otherwise, the duration specfies the number of seconds to
    //+ send the zero bit stream.
    //
    //! PRE: The serial device is open.
    //! POST: A stream of zero bits are sent to the serial device for the
    //+       given duration.
    //
    //! ARGS: duration - The length of time to send a zero bit stream to the
    //+                  serial device.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    inline vpr::Status
    sendBreak (const int duration) {
        return m_sio_imp.sendBreak(duration);
    }

protected:
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
    //! ARGS: buffer     - A pointer to the buffer where the port's buffer
    //+                    contents are to be stored.
    //! ARGS: length     - The number of bytes to be read.
    //! ARGS: bytes_read - The number of bytes read into the buffer.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    virtual vpr::Status
    read_i (void* buffer, const size_t length,
            ssize_t& bytes_read, const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_sio_imp.read(buffer, length, bytes_read, timeout);
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
    //! ARGS: buffer     - A pointer to the buffer where the port's buffer
    //+                    contents are to be stored.
    //! ARGS: length     - The number of bytes to be read.
    //! ARGS: bytes_read - The number of bytes read into the buffer.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    virtual vpr::Status
    readn_i (void* buffer, const size_t length,
             ssize_t& bytes_read, const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_sio_imp.readn(buffer, length, bytes_read, timeout);
    }

    // ------------------------------------------------------------------------
    //: Write the buffer to the serial port.
    //
    //! PRE: The port implementation object is valid.
    //! POST: The given buffer is written to the serial port, and the number
    //+       of bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer        - A pointer to the buffer to be written.
    //! ARGS: length        - The length of the buffer.
    //! ARGS: bytes_written - The number of bytes written to the device.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    virtual vpr::Status
    write_i (const void* buffer, const size_t length,
             ssize_t& bytes_written, const vpr::Interval timeout = vpr::Interval::NoTimeout) {
        return m_sio_imp.write(buffer, length, bytes_written, timeout);
    }

    RealSerialPortImpl m_sio_imp;    //: Platform-specific serial port
                                     //+ implementation object
};

}; // End of vpr namespace


#endif	/* _VPR_SERIAL_PORT_T_H_ */
