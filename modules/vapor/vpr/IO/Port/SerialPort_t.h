/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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


namespace vpr
{

/** \class SerialPort_t SerialPort_t.h vpr/IO/Port/SerialPort.h
 *
 * Cross-platform serial port interface.  The template parameter is the type
 * of the platform-specific serial port interface to use.  A type-specific
 * instantiation of this class is typedef'd to vpr::SerialPort.
 *
 * @param RealSerialPortImpl The type that serves as the true wrapper around
 *                           the platform-specific serial port implementation.
 *
 * @see vpr::SerialPortImplTermios, vpr::SerialPortImplWin32
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
    * @param portName The name of the serial port to be accessed through this
    *                 object.
    */
   SerialPort_t(const std::string& portName)
      : Port(portName)
      , mSioImpl(portName)
   {
      /* Do nothing. */ ;
   }

   /**
    * Destructor.  This does nothing.
    */
   virtual ~SerialPort_t()
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
    * @post A constant reference to the mName object is returned to the
    *       caller.
    *
    * @return An object containing the name of this serial port.
    */
   virtual const std::string& getName() const
   {
      return mSioImpl.getName();
   }

   /**
    * Opens the serial port.
    *
    * @pre The serial port is not already open.
    * @post An attempt is made to open the serial port.  The resulting status
    *       is returned to the caller.
    *
    * @throws vpr::WouldBlockException if the serial port is in non-blocking
    *         mode and could not be opened yet.
    * @throws vpr::IOException if the serial port could not be opened.
    *
    * @see vpr::FileHandleImplUNIX::open()
    */
   void open()
   {
      mSioImpl.open();
   }

   /**
    * Closes the serial port.
    *
    * @pre The serial port is open.
    * @post An attempt is made to close the serial port.  The resulting
    *       status is returned to the caller.
    *
    * @throws vpr::IOException if the file handle could not be closed.
    * @see vpr::FileHandleImplUNIX::close()
    */
   void close()
   {
      mSioImpl.close();
   }

   /**
    * Reconfigures the serial port so that it is in blocking or non-blocking
    * mode depending on the value of the parameter.  If the serial port has
    * not been opened yet, it will be opened in blocking or non-blocking mode
    * appropriately.
    *
    * @post Processes may block when accessing the port.
    *
    * @param blocking A value of true makes the serial port blocking.  A value
    *                 of false makes it non-blocking.
    *
    * @throws vpr::IOException if the blocking mode could not be set.
    * @see vpr::FileHandleImplUNIX::setBlocking()
    */
   void setBlocking(bool blocking)
   {
      mSioImpl.setBlocking(blocking);
   }

   /**
    * Clears all of the serial port settings
    *
    * @pre The port is open.
    * @post All of the serial port flags are set to 0 except VMIN=1
    *
    * @throws vpr::IOException if all attributes could not be cleared.
    */
   void clearAll()
   {
      mSioImpl.clearAll();
   }

   /**
    * Get the current blocking state for the serial port.
    *
    * @pre The blocking state is set correctly.
    *
    * @return true is returned if the port is in blocking mode.
    *         false is returned if the port is in non-blocking mode.
    */
   bool isBlocking() const
   {
      return mSioImpl.isBlocking();
   }

   /**
    * Gets the vpr::IOSys::Handle object for this serial port.
    *
    * @return vpr::IOSys::NullHandle is returned if the port has no handle or
    *         if the handle could not be returned for some reason.
    */
   virtual IOSys::Handle getHandle() const
   {
      return mSioImpl.getHandle();
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
   void setOpenReadOnly()
   {
      mSioImpl.setOpenReadOnly();
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
   void setOpenWriteOnly()
   {
      mSioImpl.setOpenWriteOnly();
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
   void setOpenReadWrite()
   {
      mSioImpl.setOpenReadWrite();
   }

   /**
    * Gets the current update action.  This tells when updates to the serial
    * device take effect.
    *
    * @pre None.
    * @post The current update action is returned to the caller.
    *
    * @return A vpr::SerialTypes::UpdateActionOption value stating when
    *         updates take effect.
    *
    * @see vpr::SerialTypes::UpdateActionOption
    */
   vpr::SerialTypes::UpdateActionOption getUpdateAction() const
   {
      return mSioImpl.getUpdateAction();
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
   void setUpdateAction(vpr::SerialTypes::UpdateActionOption action)
   {
      mSioImpl.setUpdateAction(action);
   }

   /**
    * Queries the serial port for the minimum buffer size.  This is only
    * applicable in non-canonical mode.  The minimum buffer size determines
    * how many bytes must be read before a read can return.
    *
    * @pre The serial port is open.
    * @post The minimum buffer size is returned to the caller.
    *
    * @return The size of the minimum buffer.
    *
    * @throws vpr::IOException if the buffer size could not be retrieved.
    *
    * @see getTimeout
    */
   vpr::Uint16 getMinInputSize() const
   {
      return mSioImpl.getMinInputSize();
   }

   /**
    * Attempts to change the minimum buffer size to the given argument.  This
    * will change the minimum number of bytes required in the buffer before
    * a read() call will return.  This is only applicable in non-canonical
    * mode.
    *
    * @pre The serial port is open.
    * @post If the buffer size is usable on the port, the port attributes are
    *       updated and success is returned.  Otherwise, a failure status is
    *       returned.
    *
    * @throws vpr::IOException if the buffer size could not be set successfully.
    *
    * @see setTimeout
    */
   void setMinInputSize(const vpr::Uint8 size)
   {
      mSioImpl.setMinInputSize(size);
   }

   /**
    * Gets the value of the timeout (in tenths of a second) to wait for data
    * to arrive.  This is only applicable in non-canonical mode.
    *
    * @pre The serial port is open.
    * @post The current timeout setting is returned to the caller.
    *
    * @return The timeout value is returned.
    *
    * @throws vpr::IOException if the timeout could not be retrieved.
    */
   vpr::Uint8 getTimeout() const
   {
      return mSioImpl.getTimeout();
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
    * @throws vpr::IOException if the timeout could not be set successfully.
    */
   void setTimeout(const vpr::Uint8 timeout_val)
   {
      mSioImpl.setTimeout(timeout_val);
   }

   /**
    * Gets the character size (the bits per byte).
    *
    * @pre The serial port is open.
    * @post The current character size is returned to the caller.
    *
    * @return A vpr::SerialTypes::CharacterSizeOption value is returned to
    *         indicate the character size.
    *
    * @throws vpr::IOException if the character size could be retrieved.
    *
    * @see vpr::SerialTypes::CharacterSizeOption
    */
   vpr::SerialTypes::CharacterSizeOption getCharacterSize() const
   {
      return mSioImpl.getCharacterSize();
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
    * @throws vpr::IOexception if character size could not be set.
    *
    * @see vpr::SerialTypes::CharacterSizeOption
    */
   void setCharacterSize(const vpr::SerialTypes::CharacterSizeOption bpb)
   {
      mSioImpl.setCharacterSize(bpb);
   }

   /**
    * Gets the current read state for the port.
    *
    * @pre The serial port is open.
    * @post The read state (either enabled or disabled) is returne to the
    *       caller.
    *
    * @return true is returned if bytes can be read from the device.  false is
    *         returned otherwise.
    * @throws vpr::IOException if read state could not be retrieved.
    */
   bool getReadState() const
   {
      return mSioImpl.getReadState();
   }

   /**
    * Changes the receiver so that bytes can or cannot be read from the port.
    *
    * @pre The serial port is open.
    * @post An attempt is made to make the device readable or unreadable.
    *
    * @param flag A value of true makes the port readable.  A value of false
    *             makes it unreadable.
    *
    * @throws vpr::IOException if read state could not be set.
    */
   void setRead(bool flag)
   {
      mSioImpl.setRead(flag);
   }

   /**
    * Gets the current local attach state for the port.
    *
    * @pre The serial port is open.
    * @post The CLOCAL state (either enabled or disabled) is returned to the
    *       caller.
    *
    * @return true is returned if local attachment is set.
    *         false is returned otherwise.
    *
    * @throws vpr::IOException if local attach state could not be retrieved.
    */
   bool getLocalAttachState() const
   {
      return mSioImpl.getLocalAttachState();
   }

   /**
    * Mark the device as locally attached or not.
    *
    * @pre This serial port is open.
    * @post The device has its local attachment state enabled or disabled.
    *
    * @param flag A value of true enables local attachment.
    *
    * @throws vpr::IOException if local attach state could not be set.
    */
   void setLocalAttach(bool flag)
   {
      return mSioImpl.setLocalAttach(flag);
   }

   /**
    * Gets the number of stop bits in use.  This will be either 1 or 2.
    *
    * @pre The serial port is open.
    * @post The number of stop bits currently in use is returned.
    *
    * @return The number of stop bits is returned.
    *
    * @throws vpr::IOException if stop bits could not be retrieved.
    */
   vpr::Uint8 getStopBits() const
   {
      return mSioImpl.getStopBits();
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
    * @throws vpr::IOException if stop bits could not be set.
    */
   void setStopBits(const vpr::Uint8 num_bits)
   {
      mSioImpl.setStopBits(num_bits);
   }

   /**
    * Queries the canonical input state of the serial port.  If canonical
    * mode is enabled, the characters \c EOF, \c EOL, \c EOL2, \c ERASE,
    * \c KILL, \c REPRINT, \c STATUS, and \c WERASE are enabled, and the input
    * characters are assembled into lines.  Otherwise, read requests are
    * satisfied directly from the input queue, and a read will not return
    * until the buffer is at its minimum capacity or the timeout has expired.
    *
    * @pre The serial port is open.
    * @post The current state of canonical input is returnd to the caller.
    *
    * @return true is returned if canonical input mode is enabled.
    *         Otherwise, false is returned.
    * @throws vpr::IOException if canonical state could not be retrieved.
    *
    * @see getMinInputSize
    * @see getTimeout
    */
   bool getCanonicalState() const
   {
      return mSioImpl.getCanonicalState();
   }

   /**
    * Enables canonical input.  See getCanonicalState() for more
    * information about what this means.
    *
    * @pre The serial port is open.
    * @post Canonical input is enabled or disabled.
    *
    * @throws vpr::IOException if canonical state could not be set.
    *
    * @see getCanonicalState
    */
   void setCanonicalInput(bool flag)
   {
      mSioImpl.setCanonicalInput(flag);
   }

   /**
    * Gets the current state of ignoring bytes with framing errors (other
    * than a BREAK) or parity errors.
    *
    * @pre The serial port is open.
    * @post The erroneous byte ignore state is returned to the caller.
    *
    * @throws vpr::IOException if bad byte ignore state could not
    *         be retrieved.
    */
   bool getBadByteIgnoreState() const
   {
      return mSioImpl.getBadByteIgnoreState();
   }

   /**
    * Enables or disables ignoring of received bytes with framing errors or
    * parity errors.
    *
    * @pre The serial port is open.
    * @post Erroneous byte ignoring is enabled or disabled.
    *
    * @param flag A value of true enables ignoring of bad bytes.  A value of
    *             false disables it.
    *
    * @throws vpr::IOException if bad bytes ignore could not be set.
    */
   void setBadByteIgnore(bool flag)
   {
      mSioImpl.setBadByteIgnore(flag);
   }

   /**
    * Gets the current state of ignoring BREAK bytes.
    *
    * @pre The serial port is open.
    * @post The BREAK byte ignore state is returned to the caller.
    *
    * @return true is returned if BREAK bytes are ignored.
    *         false is returned if BREAK bytes are not ignored.
    * @throws vpr::IOException if break byte ignore state could not
    *         be retrieved.
    */
   bool getBreakByteIgnoreState() const
   {
      return mSioImpl.getBreakByteIgnoreState();
   }

   /**
    * Enables or disables ignoring of received BREAK bytes.
    *
    * @pre The serial port is open.
    * @post BREAK byte ignoring is enabled or disabled.
    *
    * @param flag A value of true enables ignoring of BREAK bytes.  A value of
    *             false disables it.
    *
    * @throws vpr::IOException if break byte ignore state could not be set.
    */
   void setBreakByteIgnore(bool flag)
   {
      mSioImpl.setBreakByteIgnore(flag);
   }

   /**
    * Gets the state of parity checking for input.
    *
    * @pre The serial port is open.
    * @post The state of input parity checking is returned to the caller.
    *
    * @return true is returned if input parity checking is enabled.  false is
    *         returned if input parity checking is disabled.
    * @throws vpr::IOException if input parity check state
    *         could not be retrieved.
    */
   bool getInputParityCheckState() const
   {
      return mSioImpl.getInputParityCheckState();
   }

   /**
    * Enables or disables input parity checking.
    *
    * @pre The serial port is open.
    * @post Input parity checking is enabled or disabled.
    *
    * @param flag A value of true enables input parity checking.  A value of
    *             false disables it.
    *
    * @throws vpr::IOException if input parity check state could not be set.
    */
   void setInputParityCheck(bool flag)
   {
      mSioImpl.setInputParityCheck(flag);
   }

   /**
    * Gets the current state of bit stripping.  When enabled, input bytes are
    * stripped to seven bits.  Otherwise, all eight bits are processed.
    *
    * @pre The serial port is open.
    * @post The current bit stripping state is returned to the caller.
    *
    * @return true is returned if input bytes are stripped to seven bits.
    *         false is returned if input bytes are not stripped.
    * @throws vpr::IOException if bit strip state could not be retrieved.
    */
   bool getBitStripState() const
   {
      return mSioImpl.getBitStripState();
   }

   /**
    * Enables or disables stripping of input bytes to seven bits.
    *
    * @pre The serial port is open.
    * @post Input bit stripping is enabled or disabled.
    *
    * @param flag A value of true enables input byte stripping.  A value of
    *             false disables it.
    *
    * @throws vpr::IOException if bit strip state could not be set.
    */
   void setBitStripping(bool flag)
   {
      mSioImpl.setBitStripping(flag);
   }

   /**
    * Gets the state of start-stop input control.  When enabled, if the
    * terminal driver sees that the receive buffer is getting full, a STOP
    * command is sent to the serial port.  The device should respond by
    * halting its data flow.  When the input queue has been processed, a
    * START command is sent to the serial port which should cause it to
    * resume putting bytes onto the input queue.
    *
    * PRE: The serial port is open.
    * POST: The current state of start-stop input control is returned to the
    *       caller.
    *
    * @return true is returned if start-stop input control is enabled.  false
    *         is returned if start-stop input control is not enabled.
    * @throws vpr::IOException if start stop input state could not
    *         be retrieved.
    */
   bool getStartStopInputState() const
   {
      return mSioImpl.getStartStopInputState();
   }

   /**
    * Enables or disables start-stop input control.  See
    * getStartStopInputState() for more information.
    *
    * @pre The serial port is open.
    * @post Start-stop input control is enabled or disabled.
    *
    * @param flag A value of true enables start-stop input control.  A value
    *             of false disables it.
    *
    * @throws vpr::IOException if start stop input state could not be set.
    */
   void setStartStopInput(bool flag)
   {
      mSioImpl.setStartStopInput(flag);
   }

   /**
    * Gets the state of start-stop output control.  When enabled, when the
    * terminal driver receives a STOP command, output stops.  When it
    * receives a START command, output resumes.  If disabled, these
    * commands are read as normal characters.
    *
    * @pre The serial port is open.
    * @post The current state of start-stop output control is returned to
    *       the caller.
    *
    * @return true is returned if start-stop output control is enabled.  false
    *         is returned if start-stop output control is not enabled.
    * @throws vpr::IOException if start stop output state could not
    *         be retrieved.
    */
   bool getStartStopOutputState() const
   {
      return mSioImpl.getStartStopOutputState();
   }

   /**
    * Enables or disables start-stop output control.  See
    * getStartStopOutputState() for more information.
    *
    * @pre The serial port is open.
    * @post Start-stop output control is enabled or disabled.
    *
    * @param flag A value of true enables start-stop output control.  A value
    *             of false disables it.
    *
    * @throws vpr::IOException if start stop output state could not be set.
    *
    * @see getStartStopOutputState
    */
   void setStartStopOutput(bool flag)
   {
      mSioImpl.setStartStopOutput(flag);
   }

   /**
    * Gets the current state of parity generation for outgoing bytes and
    * parity checking for incoming bytes.
    *
    * @pre The serial port is open.
    * @post The current parity generation/checking state is returned to the
    *       caller.
    *
    * @return true is returned if parity generation and checking are enabled
    *         for outgoing and incoming bytes respectively.  false is returned
    *         if parity generation and checking are disabled.
    * @throws vpr::IOException if parity generation state could not
    *         be retrieved.
    */
   bool getParityGenerationState() const
   {
      return mSioImpl.getParityGenerationState();
   }

   /**
    * Enables or disables parity generation for outgoing bytes and parity
    * checking for incoming bytes.
    *
    * @pre The serial port is open.
    * @post Parity generation and checking are enabled or disabled.
    *
    * @param flag A value of true anables parity generation.  A value of true
    *             disables it.
    *
    * @throws vpr::IOException if partiy generation state could not be set.
    */
   void setParityGeneration(bool flag)
   {
      mSioImpl.setParityGeneration(flag);
   }

   /**
    * Returns the current state of parity error marking for this serial port.
    *
    * @pre This serial port is open.
    *
    * @return true is returned if parity error marking is enabled.  false is
    *         returned if parity error masking is disabled.
    * @throws vpr::IOException if parity error marking state
    *         could not be retrieved.
    */
   bool getParityErrorMarkingState() const
   {
      return mSioImpl.getParityErrorMarkingState();
   }

   /**
    * Enables or disables marking of bytes with parity errors or framing errors
    * (except BREAKs).  This is only active if input parity and framing error
    * reporting is enabled (see setInputParityCheck() for more information).
    * The mark is the three-byte sequence 0377 0 X where X is the byte
    * received in error.  If bit stripping is enabled, a valid 0377 byte is
    * passed as the two-byte sequence 0377 0377.
    *
    * @pre This serial port is open.
    * @post Bytes with an error are marked and passed on to the reader.
    *
    * @param flag A value of true enables parity error marking.  A value of
    *             false disables it.
    *
    * @throws vpr::IOException if parity error marking state could not be set.
    *
    * @see setInputParityCheck
    *
    * @note 0377 == 0xFF == 255
    */
   void setParityErrorMarking(bool flag)
   {
      mSioImpl.setParityErrorMarking(flag);
   }

   /**
    * Gets the current parity checking type (either odd or even).
    *
    * @pre The serial port is open.
    * @post The parity type is returned to the caller.
    *
    * @return A vpr::SerialTypes::ParityType value giving the parity
    *         checking being done.
    */
   vpr::SerialTypes::ParityType getParity() const
   {
      return mSioImpl.getParity();
   }

   /**
    * Enables even or odd parity.
    *
    * @pre The serial port is open.
    * @post Odd parity is enabled.
    *
    * @param type The type of parity to use.
    *
    * @throws vpr::IOException if parity could not be set.
    */
   void setParity(const vpr::SerialTypes::ParityType& type)
   {
      mSioImpl.setParity(type);
   }

   /**
    * Gets the current input baud setting.
    *
    * @pre The serial port is open.
    * @post The current input baud setting is returned to the caller.
    *
    * @return The current input baud setting is returned.
    *
    * @throws vpr::IOException if input baud rate could not be retrieved.
    */
   vpr::Uint32 getInputBaudRate() const
   {
      return mSioImpl.getInputBaudRate();
   }

   /**
    * Sets the current input baud setting.
    *
    * @pre The serial port is open, and the rate is a valid baud setting.
    * @post The input baud setting is updated to use the given value.
    *
    * @param baud The new input baud setting.
    *
    * @throws vpr::IOException if input baud rate could not be set.
    */
   void setInputBaudRate(const vpr::Uint32 baud)
   {
      mSioImpl.setInputBaudRate(baud);
   }

   /**
    * Gets the current output baud setting.
    *
    * @pre The serial port is open.
    * @post The current output baud setting is returned to the caller.
    *
    * @return The current output baud setting is returned.
    *
    * @throws vpr::IOException if ouput baud rate could not be retrieved.
    */
   vpr::Uint32 getOutputBaudRate() const
   {
      return mSioImpl.getOutputBaudRate();
   }

   /**
    * Sets the current output baud setting.
    *
    * @pre The serial port is open, and the rate is a valid baud setting.
    * @post The output baud setting is updated to use the given value.
    *
    * @param baud The new output baud setting.
    *
    * @throws vpr::IOException if ouput baud rate could not be set.
    */
   void setOutputBaudRate(const vpr::Uint32 baud)
   {
      mSioImpl.setOutputBaudRate(baud);
   }

   /**
    * Waits for all output to be transmitted.
    *
    * @throws vpr::IOException if failed to drain output.
    */
   void drainOutput()
   {
      mSioImpl.drainOutput();
   }

   /**
    * Alters the input or output flow control.  Based on the
    * vpr::SerialTypes::FlowActionOption argument, output can be suspended
    * and restarted or the terminal device can be told to stop or to resume
    * sending data.
    *
    * @pre The serial port is open.
    * @post Based on the argument value, the input or output flow control is
    *       updated.
    *
    * @param opt The flow control change.
    *
    * @throws vpr::IOException of control flow could not be changed.
    */
   void controlFlow(SerialTypes::FlowActionOption opt)
   {
      mSioImpl.controlFlow(opt);
   }

   /**
    * Returns the current enable state of hardware flow control.
    *
    * @pre This serial port is open.
    *
    * @return true is returned if hardware flow control is enabled.
    *         false is returned otherwise.
    * @throws vpr::IOException if hardware flow control state
    *         could not be retrieved.
    *
    * @see controlFlow
    */
   bool getHardwareFlowControlState() const
   {
      return mSioImpl.getHardwareFlowControlState();
   }

   /**
    * Attempts to enable "hardware" flow control.  While some documents
    * recommend the use of hardware flow control over the use of software
    * flow control, not all platforms implement hardware flow control.  Hence,
    * this may have no effect depending on the operating system.
    *
    * @throws vpr::IOException if hardware flow control state could not be set.
    *
    * @see controlFlow
    */
   void setHardwareFlowControl(bool flag)
   {
      mSioImpl.setHardwareFlowControl(flag);
   }

   /**
    * Discards either the input buffer (unread data received from the
    * terminal device) or the output buffer (data written but not yet
    * transmitted to the terminal device).  The argument tells which queue
    * (or queues) to flush.
    *
    * @pre The serial port is open.
    * @post Based on the argument, the appropriate queue is flushed of its
    *       current contents.
    *
    * @param queue The queue (or queues) to be flushed.
    *
    * @throws vpr::IOException if queue could not be flushed.
    */
   void flushQueue(SerialTypes::FlushQueueOption queue)
   {
      mSioImpl.flushQueue(queue);
   }

   /**
    * Transmit a continuous stream of zero bits for the given duration.  If
    * the argument is 0, the transmission will last between 0.25 and 0.5
    * seconds.  Otherwise, the duration specfies the number of seconds to
    * send the zero bit stream.
    *
    * @pre The serial port is open.
    * @post A stream of zero bits are sent to the serial port for the
    *       given duration.
    *
    * @param duration The length of time to send a zero bit stream to the
    *                 serial port.
    *
    * @throws vpr::IOException if break could not be sent.
    */
   void sendBreak(const int duration)
   {
      mSioImpl.sendBreak(duration);
   }

   /**
    * Return the status of the carrier detect signal.
    * @return - May be platform dependent, but will at least be as follows.
    *           0 - not high, 1 - high, -1 - Not supported
    *
    * @throws vpr::IOException if carrier detect state could not be retrieved.
    */
   int getCarrierDetect() const
   {
      return mSioImpl.getCarrierDetect();
   }

   /**
    * Returns the status of the data set ready line.
    * @return May be platform dependent, but will at least be as follows.
    *         0 - not high, 1 - high, -1 - Not supported
    *
    * @throws vpr::IOException if data set ready state could not be retrieved.
    */
   int getDataSetReady() const
   {
      return mSioImpl.getDataSetReady();
   }

   /**
    * Returns the status of the clear to send.
    * @return May be platform dependent, but will at least be as follows.
    *         0 - not high, 1 - high, -1 - Not supported
    *
    * @throws vpr::IOException if clear to send state could not be retrieved.
    */
   int getClearToSend() const
   {
      return mSioImpl.getRequestToSend();
   }

   /**
    * Returns the status of the ring indicator line.
    * @return May be platform dependent, but will at least be as follows.
    *         0 - not high, 1 - high, -1 - Not supported
    *
    * @throws vpr::IOException if ring indicator state could not be retrieved.
    */
   int getRingIndicator() const
   {
      return mSioImpl.getRingIndicator();
   }

   /** Sets the data terminal ready line.
    *
    * @throws vpr::IOException if data terminal ready state could
    *         not be set.
    */
   void setDataTerminalReady(bool val)
   {
      mSioImpl.setDataTerminalReady(val);
   }

   /** Sets the ready to send line.
    *
    * @throws vpr::IOException ifrequest to send state could
    *         not be set.
    */
   void setRequestToSend(bool val)
   {
      mSioImpl.setRequestToSend(val);
   }

protected:
   /**
    * Implementation of the read() template method.  This reads at most the
    * specified number of bytes from the serial port into the given buffer.
    *
    * @pre The port is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has length bytes copied into it from the port,
    *       and the number of bytes read successfully is returned to the
    *       caller.
    *
    * @param buffer  A pointer to the buffer where the port's buffer contents
    *                are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::WouldBlockException if the port is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    */
   virtual vpr::Uint32 read_i(void* buffer, const vpr::Uint32 length,
                              const vpr::Interval& timeout)
   {
      return mSioImpl.read_i(buffer, length, timeout);
   }

   /**
    * Implementation of the readn() template method.  This reads exactly the
    * specified number of bytes from the serial port into the given buffer.
    *
    * @pre The port is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it from the port,
    *       and the number of bytes read successfully is returned to the
    *       caller.
    *
    * @param buffer  A pointer to the buffer where the ports's buffer contents
    *                are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::EOFException if end of port or end of stream has been
    *         reached unexpectedly during input.
    * @throws vpr::IOException if an error ocured while reading.
    */
   virtual vpr::Uint32 readn_i(void* buffer, const vpr::Uint32 length,
                               const vpr::Interval& timeout)
   {
      return mSioImpl.readn_i(buffer, length, timeout);
   }

   /**
    * Implementation of the write() template method.  This writes the given
    * buffer to the serial port.
    *
    * @pre The port is open for writing.
    * @post The given buffer is written to the I/O port, and the number of
    *       bytes written successfully is returned to the caller.
    *
    * @param buffer  A pointer to the buffer to be written.
    * @param length  The length of the buffer.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for writing.
    *
    * @return The number of bytes written to the device is returned.
    *
    * @throws vpr::WouldBlockException if the handle is in non-blocking mode,
    *         and the write operation could not be completed.
    * @throws vpr::TimeoutException if the write could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the write operation failed.
    */
   virtual vpr::Uint32 write_i(const void* buffer, const vpr::Uint32 length,
                               const vpr::Interval& timeout)
   {
      return mSioImpl.write_i(buffer, length, timeout);
   }

   /// Platform-specific serial port implementation object
   RealSerialPortImpl mSioImpl;
};

} // End of vpr namespace


#endif  /* _VPR_SERIAL_PORT_T_H_ */
