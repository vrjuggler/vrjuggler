/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SERIAL_PORT_T_H_
#define _VPR_SERIAL_PORT_T_H_


namespace vpr {

/**
 * Cross-platform serial port interface.
 *
 * @see vpr::SerialPortImplTermios
 * @see vpr::SerialPortImplWin32
 */
template<class RealSerialPortImpl>
class SerialPort_t : public Port
{
public:
   /**
    * Constructor.
    *
    * @pre None.
    * @post A serial port interface implementation object is created using
    *       the given port name.
    *
    * @param port_name The name of the serial port to be accessed through this
    *                  object.
    */
   SerialPort_t (const std::string& port_name)
      : Port(port_name), m_sio_imp(port_name)
   {
      /* Do nothing. */ ;
   }

   /**
    * Destructor.  This does nothing.
    *
    * @pre None.<br>
    * @post None.
    */
   virtual ~SerialPort_t (void)
   {
      /* Do nothing. */ ;
   }

   // ========================================================================
   // Block I/O device interface.
   // ========================================================================

   /**
    * Gets the name of the encapsulated serial port.
    *
    * @pre None.
    * @post A constant reference to the m_name object is returned to the
    *       caller.
    *
    * @return An object containing the name of this serial port.
    */
   virtual const std::string& getName (void)
   {
      return m_sio_imp.getName();
   }

   /**
    * Sets the blocking flags so that the serial port is opened in blocking
    * mode.
    *
    * @pre None.
    * @post The open flags are updated so that when the port is opened, it is
    *       opened in blocking mode.  If the port is already open, this has
    *       no effect.
    */
   void setOpenBlocking (void)
   {
      m_sio_imp.setOpenBlocking();
   }

   /**
    * Sets the blocking flags so that the serial port is opened in
    * non-blocking mode.
    *
    * @pre None.
    * @post The open flags are updated so that when the port is opened, it is
    *       opened in non-blocking mode.  If the port is already open, this
    *       has no effect.
    */
   void setOpenNonBlocking (void)
   {
      m_sio_imp.setOpenNonBlocking();
   }

   /**
    * Opens the serial port.
    *
    * @pre The serial port is not already open.
    * @post An attempt is made to open the serial port.  The resulting status
    *       is returned to the caller.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the serial port opened
    *         successfully.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus open (void)
   {
      return m_sio_imp.open();
   }

   /**
    * Closes the serial port.
    *
    * @pre The serial port is open.
    * @post An attempt is made to close the serial port.  The resulting
    *       status is returned to the caller.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the serial port is
    *         closed successfully.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus close (void)
   {
      return m_sio_imp.close();
   }

   /**
    * Reconfigures the serial port so that it is in blocking mode.
    *
    * @pre The port is open.
    * @post Processes may block when accessing the port.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the device's blocking
    *         mode is set to blocking.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus enableBlocking (void)
   {
      return m_sio_imp.enableBlocking();
   }

   /**
    * Reconfigures the serial port so that it is in non-blocking mode.
    *
    * @pre The port is open.
    * @post Processes will not block when accessing the port.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the device's blocking
    *         mode is set to non-blocking.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   virtual vpr::ReturnStatus enableNonBlocking (void)
   {
      return m_sio_imp.enableNonBlocking();
   }

   /**
    * Get the current blocking state for the serial port.
    *
    * @pre m_blocking is set correctly
    *
    * @return true is returned if the port is in blocking mode.<br>
    *         false is returned if the port is in non-blocking mode.
    */
   bool getBlocking (void) const
   {
      return m_sio_imp.getBlocking();
   }

   /**
    * Gets the current non-blocking state for the serial port.
    *
    * @pre <code>m_blocking</code> is set correctly<br>
    *
    * @return <code>true</code> is returned if the port is in non-blocking
    *         mode.   Otherwise, <code>false</code> is returned.
    */
   bool getNonBlocking (void) const
   {
      return m_sio_imp.getNonBlocking();
   }

   /**
    * Gets the vpr::IOSys::Handle object for this serial port.
    *
    * @return vpr::IOSys::NullHandle is returned if the port has no handle or
    *         if the handle could not be returned for some reason.
    */
   virtual IOSys::Handle getHandle (void)
   {
      return m_sio_imp.getHandle();
   }

   // ========================================================================
   // Serial port interface.
   // ========================================================================

   /**
    * Sets the open flags so that the serial port is opened in read-only mode.
    *
    * @pre None.
    * @post The open flags are updated so that when the port is opened, it
    *       is opened in read-only mode.  If the port is already open, this
    *       has no effect.
    */
   void setOpenReadOnly (void)
   {
      m_sio_imp.setOpenReadOnly();
   }

   /**
    * Sets the open flags so that the serial port is opened in write-only
    * mode.
    *
    * @pre None.
    * @post The open flags are updated so that when the port is opened, it
    *       is opened in write-only mode.  If the port is already open,
    *       this has no effect.
    */
   void setOpenWriteOnly (void)
   {
      m_sio_imp.setOpenWriteOnly();
   }

   /**
    * Sets the open flags so that the serial port is opened in read/write
    * mode.
    *
    * @pre None.
    * @post The open flags are updated so that when the port is opened, it
    *       is opened in read/write mode.  If the port is already open,
    *       this has no effect.
    */
   void setOpenReadWrite (void)
   {
      m_sio_imp.setOpenReadWrite();
   }

   /**
    * Gets the current update action.  This tells when updates to the serial
    * device take effect.
    *
    * @pre None.
    * @post The current update action is returned to the caller.
    *
    * @return A <code>vpr::SerialTypes::UpdateActionOption</code> value
    *         stating when updates take effect.
    *
    * @see vpr::SerialTypes::UpdateActionOption
    */
   vpr::SerialTypes::UpdateActionOption getUpdateAction (void)
   {
      return m_sio_imp.getUpdateAction();
   }

   /**
    * Changes the current update action to take place as described by the
    * given value.
    *
    * @pre None.
    * @post The update action is modified to use the new value.  The results
    *       will be seen with the next device operation.
    *
    * @param action The new update action value.
    *
    * @see vpr::SerialTypes::UpdateActionOption
    */
   void setUpdateAction (vpr::SerialTypes::UpdateActionOption action)
   {
      m_sio_imp.setUpdateAction(action);
   }

   /**
    * Queries the serial port for the minimum buffer size.  This is only
    * applicable in non-canonical mode.
    *
    * @pre The serial port is open.
    * @post The maximum buffer size is returned to the caller through the
    *       by-reference argument.
    *
    * @param size A reference to a vpr::Uint16 where the buffer size is
    *             stored for return to the caller.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the buffer size was
    *         retrieved successfully.  vpr::ReturnStatus::Fail is returned
    *         otherwise.
    */
   vpr::ReturnStatus getBufferSize (vpr::Uint16& size)
   {
      return m_sio_imp.getBufferSize(size);
   }

   /**
    * Attempts to change the minimum buffer size to the given argument.  This
    * will change the minimum number of bytes required in the buffer before
    * a <code>read</code> call will return.  This is only applicable in
    * non-canonical mode.
    *
    * @pre The serial port is open.
    * @post If the buffer size is usable on the port, the port attributes are
    *       updated and success is returned.  Otherwise, a failure status is
    *       returned.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the buffer size was set
    *         successfully.  vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus setBufferSize (const vpr::Uint8 size)
   {
      return m_sio_imp.setBufferSize(size);
   }

   /**
    * Gets the value of the timeout (in tenths of a second) to wait for data
    * to arrive.  This is only applicable in non-canonical mode.
    *
    * @pre The serial port is open.
    * @post The current timeout setting is returned to the caller in the
    *       by-reference argument.
    *
    * @param timeout A reference to a <code>vpr::Uint8</code> to be used as
    *                storage for the timeout value.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the timeout length was
    *         retrieved successfully.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus getTimeout (vpr::Uint8& timeout)
   {
      return m_sio_imp.getTimeout(timeout);
   }

   /**
    * Sets the value of the timeout to wait for data to arrive.  The value
    * given must be in tenths of a second.  This is only applicable in
    * non-canonical mode.
    *
    * @pre The serial port is open.
    * @post The timeout interval is updated to use the given value.
    *
    * @param timeout_val The new timeout value measured in tenths of a
    *                    second.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the timeout length was
    *         updated successfully.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus setTimeout (const vpr::Uint8 timeout_val)
   {
      return m_sio_imp.setTimeout(timeout_val);
   }

   /**
    * Gets the character size (the bits per byte).
    *
    * @pre The serial port is open.
    * @post The current character size is returned to the caller in the
    *       by-reference argument.
    *
    * @param size A reference to a vpr::SerialTypes::CharacterSizeOption
    *             object to be used as storage for the character size value.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the character size was
    *         retrieved successfully.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    *
    * @see vpr::SerialTypes::CharacterSizeOption
    */
   vpr::ReturnStatus getCharacterSize (vpr::SerialTypes::CharacterSizeOption& size)
   {
      return m_sio_imp.getCharacterSize(size);
   }

   /**
    * Sets the current character size (the bits per byte) to the size in the
    * given value.  This is used for both reding and writing, and the size
    * does not include the parity bit (if any).
    *
    * @pre The serial port is open.
    * @post The bits per byte for the serial port is updated.
    *
    * @param bpb The new character size.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the character size was
    *         updated successfully. vpr::ReturnStatus::Fail is is returned
    *         otherwise.
    *
    * @see vpr::SerialTypes::CharacterSizeOption
    */
   vpr::ReturnStatus setCharacterSize (const vpr::SerialTypes::CharacterSizeOption bpb)
   {
      return m_sio_imp.setCharacterSize(bpb);
   }

   /**
    * Gets the current read state for the port.
    *
    * @pre The serial port is open.
    * @post The read state (either enabled or disabled) is returne to the
    *       caller.
    *
    * @return true is returned if bytes can be read from the device.<br>
    *         false is returned otherwise.
    */
   bool getReadState (void)
   {
      return m_sio_imp.getReadState();
   }

   /**
    * Enables the receiver so that bytes can be read from the port.
    *
    * @pre The serial port is open.
    * @post An attempt is made to make the device readable.
    *
    * @return vpr::ReturnStatus::Succeed is returned if reading was enabled.
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus enableRead (void)
   {
      return m_sio_imp.enableRead();
   }

   /**
    * Disables the receiver so that bytes cannot be read from the port.
    *
    * @pre The serial port is open.
    * @post An attempt is made to make the device unreadable.
    *
    * @return vpr::ReturnStatus::Succeed is returned if reading was disabled.
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus disableRead (void)
   {
      return m_sio_imp.disableRead();
   }

   /**
    * Gets the number of stop bits in use.  This will be either 1 or 2.
    *
    * @pre The serial port is open.
    * @post The number of stop bits currently in use is returned via the
    *       by-reference argument.
    *
    * @param num_bits A reference to a vpr::Uint8 that will be
    *                 used to store the number of stop bits.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the number of
    *         stop bits was retrieved successfully.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus getStopBits (vpr::Uint8& num_bits)
   {
      return m_sio_imp.getStopBits(num_bits);
   }

   /**
    * Sets the number of stop bits to use.  The value must be either 1 or 2.
    *
    * @pre The serial port is open, and the given number of stop bits is
    *      either 1 or 2.
    * @post The number of stop bits is updated to use the given value.
    *
    * @param num_bits The number of stop bits to use.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the number of stop bits
    *         was updated successfully.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus setStopBits (const vpr::Uint8 num_bits)
   {
      return m_sio_imp.setStopBits(num_bits);
   }

   /**
    * Queries the canonical input state of the serial port.  If canonical
    * mode is enabled, the characters <code>EOF</code>, <code>EOL</code>,
    * <code>EOL2</code>, <code>ERASE</code>, <code>KILL</code>,
    * <code>REPRINT</code>, <code>STATUS</code>, and <code>WERASE</code> are
    * enabled, and the input characters are assembled into lines.  Otherwise,
    * read requests are satisfied directly from the input queue, and a read
    * will not return until the buffer is at its minimum capacity or the
    * timeout has expired.
    *
    * @pre The serial port is open.
    * @post The current state of canonical input is returnd to the caller.
    *
    * @return <code>true</code> is returned if canonical input mode is
    *         enabled.  Otherwise, <code>false</code> is returned.
    *
    * @see getBufferSize
    * @see getTimeout
    */
   bool getCanonicalState (void)
   {
      return m_sio_imp.getCanonicalState();
   }

   /**
    * Enables canonical input.  See <code>getCanonicalState</code> for more
    * information about what this means.
    *
    * @pre The serial port is open.
    * @post Canonical input is enabled.
    *
    * @return vpr::ReturnStatus::Succeed is returned if canonical input was
    *         enabled.  Otherwise, vpr::ReturnStatus::Fail is returned.
    *
    * @see getCanonicalState
    */
   vpr::ReturnStatus enableCanonicalInput (void)
   {
      return m_sio_imp.enableCanonicalInput();
   }

   /**
    * Disables canonical input.  See <code>getCanonicalState</code> for more
    * information about what this means.
    *
    * @pre The serial port is open.
    * @post Canonical input is disabled.
    *
    * @return vpr::ReturnStatus::Succeed is returned if canonical input was
    *         disabled.  Otherwise, vpr::ReturnStatus::Fail is returned.
    *
    * @see getCanonicalState
    */
   vpr::ReturnStatus disableCanonicalInput (void)
   {
      return m_sio_imp.disableCanonicalInput();
   }

   /**
    * Gets the current state of ignoring bytes with framing errors (other
    * than a <code>BREAK</code>) or parity errors.
    *
    * @pre The serial port is open.
    * @post The erroneous byte ignore state is returned to the caller.
    *
    * @return <code>true</code> is returned if bad bytes are ignored.<br>
    *         <code>false</code> is returned if bad bytes are not ignored.
    */
   bool getBadByteIgnoreState (void)
   {
      return m_sio_imp.getBadByteIgnoreState();
   }

   /**
    * Enables ignoring of received bytes with framing errors or parity
    * errors.
    *
    * @pre The serial port is open.
    * @post Erroneous byte ignoring is enabled.
    *
    * @return vpr::ReturnStatus::Succeed is returned if bad byte ignoring is
    *         enabled.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus enableBadByteIgnore (void)
   {
      return m_sio_imp.enableBadByteIgnore();
   }

   // ------------------------------------------------------------------------
   //: Disable ignoring of received bytes with framing errors or parity
   //+ errors.
   //
   //! PRE: The serial port is open.
   //! POST: Erroneous byte ignoring is disabled.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus disableBadByteIgnore (void)
   {
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
   bool getInputParityCheckState (void)
   {
      return m_sio_imp.getInputParityCheckState();
   }

   // ------------------------------------------------------------------------
   //: Enable input parity checking.
   //
   //! PRE: The serial port is open.
   //! POST: Input parity checking is enabled.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus enableInputParityCheck (void)
   {
      return m_sio_imp.enableInputParityCheck();
   }

   // ------------------------------------------------------------------------
   //: Disable input parity checking.
   //
   //! PRE: The serial port is open.
   //! POST: Input parity checking is disabled.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus disableInputParityCheck (void)
   {
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
   bool getBitStripState (void)
   {
      return m_sio_imp.getBitStripState();
   }

   // ------------------------------------------------------------------------
   //: Enable stripping of input bytes to seven bits.
   //
   //! PRE: The serial port is open.
   //! POST: Input bit stripping is enabled.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus enableBitStripping (void)
   {
      return m_sio_imp.enableBitStripping();
   }

   // ------------------------------------------------------------------------
   //: Disable stripping of input bytes to seven bits.
   //
   //! PRE: The serial port is open.
   //! POST: Input bit stripping is disabled.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus disableBitStripping (void)
   {
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
   bool getStartStopInputState (void)
   {
      return m_sio_imp.getStartStopInputState();
   }

   // ------------------------------------------------------------------------
   //: Enable start-stop input control.  See getStartStopInputState() for
   //+ more information.
   //
   //! PRE: The serial port is open.
   //! POST: Start-stop input control is enabled.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus enableStartStopInput (void)
   {
      return m_sio_imp.enableStartStopInput();
   }

   // ------------------------------------------------------------------------
   //: Disable start-stop input control.  See getStartStopInputState() for
   //+ more information.
   //
   //! PRE: The serial port is open.
   //! POST: Start-stop input control is disabled.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus disableStartStopInput (void)
   {
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
   bool getStartStopOutputState (void)
   {
      return m_sio_imp.getStartStopOutputState();
   }

   // ------------------------------------------------------------------------
   //: Disable start-stop input control.  See getStartStopInputState() for
   //+ more information.
   //
   //! PRE: The serial port is open.
   //! POST: Start-stop input control is disabled.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus enableStartStopOutput (void)
   {
      return m_sio_imp.enableStartStopOutput();
   }

   // ------------------------------------------------------------------------
   //: Disable start-stop output control.  See getStartStopOutputState() for
   //+ more information.
   //
   //! PRE: The serial port is open.
   //! POST: Start-stop output control is disabled.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus disableStartStopOutput (void)
   {
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
   bool getParityGenerationState (void)
   {
      return m_sio_imp.getParityGenerationState();
   }

   // ------------------------------------------------------------------------
   //: Enable parity generation for outgoing bytes and parity checking for
   //+ incoming bytes.
   //
   //! PRE: The serial port is open.
   //! POST: Parity generation and checking are enabled.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus enableParityGeneration (void)
   {
      return m_sio_imp.enableParityGeneration();
   }

   // ------------------------------------------------------------------------
   //: Disable parity generation for outgoing bytes and parity checking for
   //+ incoming bytes.
   //
   //! PRE: The serial port is open.
   //! POST: Parity generation and checking are disabled.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus disableParityGeneration (void)
   {
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
   bool getParityErrorMarkingState (void)
   {
      return m_sio_imp.getParityErrorMarkingState();
   }

   // ------------------------------------------------------------------------
   //: Enable parity error and framing error marking.
   //
   //! PRE: The serial port is open.
   //! POST: Parity error marking is enabled.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus enableParityErrorMarking (void)
   {
      return m_sio_imp.enableParityErrorMarking();
   }

   // ------------------------------------------------------------------------
   //: Disable parity error and framing error marking.
   //
   //! PRE: The serial port is open.
   //! POST: Parity error marking is disabled.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus disableParityErrorMarking (void)
   {
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
   vpr::SerialTypes::ParityType getParity (void)
   {
      return m_sio_imp.getParity();
   }

   // ------------------------------------------------------------------------
   //: Enable odd parity.
   //
   //! PRE: The serial port is open.
   //! POST: Odd parity is enabled.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus setOddParity (void)
   {
      return m_sio_imp.setOddParity();
   }

   // ------------------------------------------------------------------------
   //: Enable even parity.
   //
   //! PRE: The serial port is open.
   //! POST: Even parity is enabled.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus setEvenParity (void)
   {
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
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus getInputBaudRate (vpr::Uint32& rate)
   {
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
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus setInputBaudRate (const vpr::Uint32 baud)
   {
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
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus getOutputBaudRate (vpr::Uint32& rate)
   {
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
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus setOutputBaudRate (const vpr::Uint32 baud)
   {
      return m_sio_imp.setOutputBaudRate(baud);
   }

   // ------------------------------------------------------------------------
   //: Wait for all output to be transmitted.
   //
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus drainOutput (void)
   {
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
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus controlFlow (SerialTypes::FlowActionOption opt)
   {
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
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus flushQueue (SerialTypes::FlushQueueOption queue)
   {
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
   //! RETURNS: A vpr::ReturnStatus object describing the results of the operation.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus sendBreak (const int duration)
   {
      return m_sio_imp.sendBreak(duration);
   }

protected:
   /**
    * Implementation of the <code>read</code> template method.  This reads at
    * most the specified number of bytes from the serial port into the given
    * buffer.
    *
    * @pre The port is open for reading, and the buffer is at least
    *      <code>length</code> bytes long.
    * @post The given buffer has length bytes copied into it from the port,
    *       and the number of bytes read successfully is returned to the
    *       caller via the <code>bytes_read</code> parameter.
    *
    * @param buffer     A pointer to the buffer where the port's buffer
    *                   contents are to be stored.
    * @param length     The number of bytes to be read.
    * @param bytes_read The number of bytes read into the buffer.
    * @param timeout    The maximum amount of time to wait for data to be
    *                   available for reading.  This argument is optional and
    *                   defaults to <code>vpr::Interval::NoTimeout</code>
    *
    * @return <code>vpr::ReturnStatus::Succeed</code> is returned if the read
    *         operation completed successfully.<br>
    *         <code>vpr::ReturnStatus::Succeed</code> is returned if the read
    *         operation failed.<br>
    *         <code>vpr::ReturnStatus::WouldBlock</code> if the port is in
    *         non-blocking mode, and there is no data to be read.<br>
    *         <code>vpr::ReturnStatus::Timeout</code> is returned if the read
    *         could not begin within the timeout interval.
    */
   virtual vpr::ReturnStatus read_i (void* buffer, const vpr::Uint32 length,
                                     vpr::Uint32& bytes_read,
                                     const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return m_sio_imp.read_i(buffer, length, bytes_read, timeout);
   }

   /**
    * Implementation of the <code>readn</code> template method.  This reads
    * exactly the specified number of bytes from the serial port into the
    * given buffer.
    *
    * @pre The port is open for reading, and the buffer is at least
    *      <code>length</code> bytes long.
    * @post The given buffer has <code>length</code> bytes copied into
    *       it from the port, and the number of bytes read successfully
    *       is returned to the caller via the <code>bytes_read</code>
    *       parameter.
    *
    * @param buffer     A pointer to the buffer where the ports's buffer
    *                   contents are to be stored.
    * @param length     The number of bytes to be read.
    * @param bytes_read The number of bytes read into the buffer.
    * @param timeout    The maximum amount of time to wait for data to be
    *                   available for reading.  This argument is optional and
    *                   defaults to <code>vpr::Interval::NoTimeout</code>
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock if the port is in non-blocking
    *         mode, and there is no data to be read.<br>
    *         vpr::ReturnStatus::Timeout is returned if the read could not
    *         begin within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the read operation failed.
    */
   virtual vpr::ReturnStatus readn_i (void* buffer, const vpr::Uint32 length,
                                      vpr::Uint32& bytes_read,
                                      const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return m_sio_imp.readn_i(buffer, length, bytes_read, timeout);
   }

   /**
    * Implementation of the <code>write</code> template method.  This writes
    * the given buffer to the serial port.
    *
    * @pre The port is open for writing.
    * @post The given buffer is written to the I/O port, and the number
    *       of bytes written successfully is returned to the caller via the
    *       <code>bytes_written</code> parameter.
    *
    * @param buffer        A pointer to the buffer to be written.
    * @param length        The length of the buffer.
    * @param bytes_written The number of bytes written to the port.
    * @param timeout       The maximum amount of time to wait for data to be
    *                      available for writing.  This argument is optional
    *                      and defaults to vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the write operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock is returned if the handle is in
    *         non-blocking mode, and the write operation could not
    *         complete.<br>
    *         vpr::ReturnStatus::Timeout is returned if the write could not
    *         begin within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the write operation
    *         failed.
    */
   virtual vpr::ReturnStatus write_i (const void* buffer,
                                      const vpr::Uint32 length,
                                      vpr::Uint32& bytes_written,
                                      const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return m_sio_imp.write_i(buffer, length, bytes_written, timeout);
   }

   /// Platform-specific serial port implementation object
   RealSerialPortImpl m_sio_imp;
};

}; // End of vpr namespace


#endif	/* _VPR_SERIAL_PORT_T_H_ */
