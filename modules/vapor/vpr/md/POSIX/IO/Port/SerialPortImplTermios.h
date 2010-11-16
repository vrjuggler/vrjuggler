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
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SERIAL_PORT_IMPL_TERMIOS_H_
#define _VPR_SERIAL_PORT_IMPL_TERMIOS_H_

#include <vpr/vprConfig.h>

#include <termios.h>
#include <string>
#include <vector>

#include <vpr/IO/Port/SerialTypes.h>
#include <vpr/IO/IOSys.h>
#include <vpr/md/POSIX/IO/FileHandleImplUNIX.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>


namespace vpr
{

/** \class SerialPortImplTermios SerialPortImplTermios.h vpr/IO/Port/SerialPort.h
 *
 * vpr::SerialPort implementation using termios.  This is used with
 * vpr::SerialPort_t<T> to create the typedef vpr::SerialPort.
 *
 * For more information about serial ports on Linux, refer to
 * http://www.ibiblio.org/mdw/HOWTO/Serial-HOWTO-3.html
 */
class VPR_CLASS_API SerialPortImplTermios
{
public:
   /**
    * Constructor.  This creates a file handle object connected to the given
    * port name and sets the update action to happen immediately.
    *
    * @post mHandle->getHandle() is allocated and connected to port_name, and
    *       the update action is set to vpr::SerialTypes::NOW.
    *
    * @param portName The name of the serial port that will be accessed.
    */
   SerialPortImplTermios(const std::string& portName);

   /**
    * Destructor.  If the file handle is non-NULL, its memory is released.
    *
    * @post If mHandle->getHandle() is non-NULL, its memory is released.
    */
   ~SerialPortImplTermios();

   // ========================================================================
   // vpr::BlockIO basics.
   // ========================================================================

   /**
    * Gets the name of this serial port.
    *
    * @return An object containing the name of the serial port.
    */
   const std::string& getName() const
   {
      return mHandle->getName();
   }

   /**
    * Sets the open flags so that the serial port is opened in read-only
    * mode.
    *
    * @post The open flags are updated so that when the port is opened, it is
    *       opened in read-only mode.  If the port is already open, this has
    *       no effect.
    */
   void setOpenReadOnly()
   {
      mHandle->setOpenReadOnly();
   }

   /**
    * Sets the open flags so that the serial port is opened in write-only
    * mode.
    *
    * @post The open flags are updated so that when the port is opened, it is
    *       opened in write-only mode.  If the port is already open, this has
    *       no effect.
    */
   void setOpenWriteOnly()
   {
      mHandle->setOpenWriteOnly();
   }

   /**
    * Sets the open flags so that the serial port is opened in read/write
    * mode.
    *
    * @post The open flags are updated so that when the port is opened, it is
    *       opened in read/write mode.  If the port is already open, this has
    *       no effect.
    */
   void setOpenReadWrite()
   {
      mHandle->setOpenReadWrite();
   }

   /**
    * Opens the serial port and initialize its flags.
    *
    * @pre This serial port is not already open.
    * @post An attempt is made to open the port.  If it is successful, the
    *       port's flags are initaialized to 0.  The resulting status is
    *       returned to the caller.
    *
    * @throws vpr::WouldBlockException if the serial port is in non-blocking
    *         mode and could not be opened yet.
    * @throws vpr::IOException if the serial port could not be opened.
    *
    * @see vpr::FileHandleImplUNIX::open()
    */
   void open();

   /**
    * Clears all of the serial port's flags
    *
    * @pre This serial port is already open.
    * @post An attempt is made to change the flags.  If it is successful, the
    *       port's flags are initaialized to 0.  The resulting status is
    *       returned to the caller.
    *
    * @throws vpr::IOException if all attributes could not be cleared.
    */
   void clearAll();

   /**
    * Closes the serial port.
    *
    * @pre This serial port is open.
    * @post An attempt is made to close the port.  The resulting status is
    *       returned to the caller.
    *
    * @throws vpr::IOException if the file handle could not be closed.
    * @see vpr::FileHandleImplUNIX::close()
    */
   void close()
   {
      vprASSERT(mHandle->mFdesc != -1 && "The port may not be open");
      mHandle->close();
   }

   /**
    * Reconfigures the serial port so that it is in blocking or non-blocking
    * mode depending on the value of the argument.  If the serial port has
    * not been opened yet, it will be opened in blocking or non-blocking
    * mode appropriately when open() is called.
    *
    * @post Processes may block when accessing the port.
    *
    * @param blocking A value of true indicates that this port will use
    *                 blocking I/O.  A value of false indicates that this port
    *                 will use non-blocking I/O.
    *
    * @throws vpr::IOException if the blocking mode could not be set.
    * @see vpr::FileHandleImplUNIX::setBlocking()
    */
   void setBlocking(bool blocking)
   {
      mHandle->setBlocking(blocking);
   }

   /**
    * Get the current blocking state for the serial port.
    *
    * @pre mBlocking is set correctly
    *
    * @return \c true is returned if the port is in blocking mode.
    * @return \c false is returned if the port is in non-blocking mode.
    */
   bool isBlocking() const
   {
      return mHandle->isBlocking();
   }

   /**
    * Returns the contained handle.
    */
   vpr::IOSys::Handle getHandle() const
   {
      return mHandle->getHandle();
   }

   /**
    * Tests if the serial port is read-only.
    *
    * @pre This serial port is open.
    * @post The access mode is tested for read-only mode, and the result is
    *       returned to the caller.
    *
    * @return \c true is returned if the device is in read-only mode;
    * @return \c false is returned otherwise.
    */
   bool isReadOnly() const
   {
      vprASSERT(mHandle->mFdesc != -1 && "The port may not be open");
      return mHandle->isReadOnly();
   }

   /**
    * Tests if the serial port is write-only.
    *
    * @pre This serial port is open.
    * @post The access mode is tested for write-only mode, and the result is
    *       returned to the caller.
    *
    * @return \c true is returned if the device is in write-only mode.
    * @return \c false is returned otherwise.
    */
   bool isWriteOnly() const
   {
      vprASSERT(mHandle->mFdesc != -1 && "The port may not be open");
      return mHandle->isWriteOnly();
   }

   /**
    * Tests if the serial port is read/write.
    *
    * @pre This serial port is open.
    * @post The access mode is tested for read/write mode, and the result is
    *       returned to the caller.
    *
    * @return \c true is returned if the device is in read/write mode.
    * @return \c false is returned otherwise.
    */
   bool isReadWrite() const
   {
      vprASSERT(mHandle->mFdesc != -1 && "The port may not be open");
      return mHandle->isReadWrite();
   }

   // ========================================================================
   // VPR serial port interface implementation.
   // ========================================================================

   /**
    * Gets the current update action.  This tells when updates to the serial
    * device take effect.
    *
    * @post The current update action is returned to the caller.
    *
    * @return A vpr::SerialTypes::UpdateActionOption value stating when
    *         updates take effect.
    */
   vpr::SerialTypes::UpdateActionOption getUpdateAction() const;

   /**
    * Changes the current update action to take place as described by the
    * given value.
    *
    * @post The update action is modified to use the new value.  The results
    *       will be seen with the next device operation.
    *
    * @param action The new update action value.
    */
   void setUpdateAction(SerialTypes::UpdateActionOption action);

   /**
    * Queries the serial port for the minimum buffer size.  This is only
    * applicable in non-canonical mode.
    * The min buffer size determines how many bytes must be read before a read
    * can return.
    *
    * @pre The serial port is open.
    * @post The minimum buffer size is returned to the caller.
    *
    * @return The minimum buffer size is returned.
    *
    * @throws vpr::IOException if the buffer size could not be retrieved.
    *
    * @see getTimeout
    */
    vpr::Uint16 getMinInputSize() const;

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
   void setMinInputSize(const Uint8 size);

   /**
    * Gets the value of the timeout (in tenths of a second) to wait for data
    * to arrive.  This is only applicable in non-canonical mode.
    *
    * @pre This serial port is open.
    * @post The current timeout setting is returned to the caller.
    *
    * @return The timout value is returned.
    *
    * @throws vpr::IOException if the timeout could not be retrieved.
    *
    * @see Refer to page 353 of <I>Advanced Programming in the UNIX
    *      Environment</I> for more details.
    */
   vpr::Uint8 getTimeout() const;

   /**
    * Sets the value of the timeout to wait for data to arrive.  The value
    * given must be in tenths of a second.  This is only applicable in
    * non-canonical mode.
    *
    * @pre This serial port is open.
    * @post The timeout interval is updated to use the given value.
    *
    * @param timeout The new timeout value measured in tenths of a second.
    *
    * @throws vpr::IOException if the timeout could not be set successfully.
    *
    * @see Refer to page 353 of <I>Advanced Programming in the UNIX
    *      Environment</I> for more details.
    */
   void setTimeout(const Uint8 timeout);

   /**
    * Gets the character size (the bits per byte).
    *
    * @pre This serial port is open.
    * @post The current character size is returned to the caller.
    *
    * @return A vpr::SerialTypes::CharacterSizeOption value is returned to
    *         indicate the character size.
    *
    * @throws vpr::IOException if the character size could be retrieved.
    */
   vpr::SerialTypes::CharacterSizeOption getCharacterSize() const;

   /**
    * Sets the current character size (the bits per byte) to the size in the
    * given value.  This is used for both reding and writing, and the size
    * does not include the parity bit (if any).
    *
    * @pre This serial port is open.
    * @post The bits per byte for the serial port is updated.
    *
    * @param bpb The new character size.
    *
    * @throws vpr::IOexception if character size could not be set.
    */
   void setCharacterSize(const vpr::SerialTypes::CharacterSizeOption bpb);

   /**
    * Gets the current read state for the port.
    *
    * @pre This serial port is open.
    * @post The read state (either enabled or disabled) is returne to the
    *       caller.
    *
    * @return true is returned if bytes can be read from the device.  false is
    *         returned otherwise.
    * @throws vpr::IOException if read state could not be retrieved.
    */
   bool getReadState() const;

   /**
    * Changes the receiver so that bytes can (or cannot) be read from the port.
    *
    * @pre This serial port is open.
    * @post An attempt is made to make the device readable or unreadable.
    *
    * @param flag A value of true indicates that reading will be allowed.
    *             A value of false prevents reading from the port.
    *
    * @throws vpr::IOException if read state could not be set.
    */
   void setRead(bool flag);

   /**
    * Get the local attach state.  That is, test if the device is attached
    * locally.
    *
    * @pre The serial port is open.
    * @post The local attach state is returned to the caller.
    *
    * @return \c true is returned if the device is attached locally.
    * @return \c false is returned if the device is not attached locally, and
    *         opening the device will usually block until there is a response.
    *
    * @throws vpr::IOException if local attach state could not be retrieved.
    */
   bool getLocalAttachState() const;

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
   void setLocalAttach(bool flag);

   /**
    * Gets the number of stop bits in use.  This will be either 1 or 2.
    *
    * @pre This serial port is open.
    * @post The number of stop bits currently in use is returned.
    *
    * @return The number of stop bits currently in use is returned.
    *
    * @throws vpr::IOException if stop bits could not be retrieved.
    */
   vpr::Uint8 getStopBits() const;

   /**
    * Sets the number of stop bits to use.  The value must be either 1 or 2.
    *
    * @pre This serial port is open, and the given number of stop bits is
    *       either 1 or 2.
    * @post The number of stop bits is updated to use the given value.
    *
    * @param num_bits The number of stop bits to use.
    *
    * @throws vpr::IOException if stop bits could not be set.
    */
   void setStopBits(const Uint8 num_bits);

   /**
    * Queries the canonical input state of the serial port.  If canonical mode
    * is enabled, the characters EOF, EOL, EOL2, ERASE, KILL, REPRINT,
    * STATUS, and WERASE are enabled, and the input characters are
    * assembled into lines.  Otherwise, read requests are satisfied directly
    * from the input queue, and a read will not return until the buffer is
    * at its minimum capacity or the timeout has expired.  See
    * getMinInputSize() and getTimeout() for more information.
    *
    * @pre This serial port is open.
    * @post The current state of canonical input is returnd to the caller.
    *
    * @return \c true is returned if canonical input is enabled.
    * @return \c false is returned otherwise.
    * @throws vpr::IOException if canonical state could not be retrieved.
    */
   bool getCanonicalState() const;

   /**
    * Enables or disables canonical input.  See getCanonicalState() for more
    * information about what this means.
    *
    * @pre This serial port is open.
    * @post Canonical input is enabled or disabled.
    *
    * @param flag A value of true enables canonical input.  Passing false
    *             disables canonical input.
    *
    * @throws vpr::IOException if canonical state could not be set.
    *
    * @see getCanonicalState
    */
   void setCanonicalInput(bool flag);

   /**
    * Gets the current state of ignoring bytes with framing errors (other
    * than a BREAK) or parity errors.
    *
    * @pre This serial port is open.
    * @post The erroneous byte ignore state is returned to the caller.
    *
    * @return \c true is returned if bad bytes are ignored.
    * @return \c false is returned otherwise.
    * @throws vpr::IOException if bad byte ignore state could not
    *         be retrieved.
    */
   bool getBadByteIgnoreState() const;

   /**
    * Enables or disables ignoring of received bytes with framing errors or
    * parity errors.
    *
    * @pre This serial port is open.
    * @post Erroneous byte ignoring is enabled or disabled.
    *
    * @param flag A value of true enables ignoring of bad bytes.  Passing false
    *             disables bad byte ignoring.
    *
    * @throws vpr::IOException if bad bytes ignore could not be set.
    */
   void setBadByteIgnore(bool flag);

   /**
    * Gets the current state of ignoring BREAK bytes
    *
    * @pre This serial port is open.
    * @post The BREAK byte ignore state is returned to the caller.
    *
    * @return \c true is returned if bad bytes are ignored.
    * @return \c false is retunred otherwise.
    *
    * @throws vpr::IOException if break byte ignore state could not
    *         be retrieved.
    */
   bool getBreakByteIgnoreState() const;

   /**
    * Enables or disables ignoring of received BREAK bytes
    *
    * @pre This serial port is open.
    * @post BREAK byte ignoring is enabled or disabled.
    *
    * @param flag A value of true enables ignoring of BREAK bytes.  Passing
    *             false disables BREAK byte ignoring.
    *
    * @throws vpr::IOException if break byte ignore state could not be set.
    */
   void setBreakByteIgnore(bool flag);

   /**
    * Gets the state of parity checking for input.
    *
    * @pre This serial port is open.
    * @post The state of input parity checking is returned to the caller.
    *
    * @return \c true is returned if input parity checking is enabled
    * @return \c false is returned otherwise.
    *
    * @throws vpr::IOException if input parity check state
    *         could not be retrieved.
    */
   bool getInputParityCheckState() const;

   /**
    * Enables or disables input parity checking.
    *
    * @pre This serial port is open.
    * @post Input parity checking is enabled or disabled.
    *
    * @param flag A value of true enables input parity checking.  Passing
    *             false disables input parity checking.
    *
    * @throws vpr::IOException if input parity check state could not be set.
    */
   void setInputParityCheck(bool flag);

   /**
    * Gets the current state of bit stripping.  When enabled, input bytes are
    * stripped to seven bits.  Otherwise, all eight bits are processed.
    *
    * @pre This serial port is open.
    * @post The current bit stripping state is returned to the caller.
    *
    * @return \c true is returned if input bytes are stripped to seven bits.
    * @return \c false is returned if input bytes are not stripped.
    * @throws vpr::IOException if bit strip state could not be retrieved.
    */
   bool getBitStripState() const;

   /**
    * Enables or disables stripping of input bytes to seven bits.
    *
    * @pre This serial port is open.
    * @post Input bit stripping is enabled or disabled.
    *
    * @param flag A value of true enables input byte stripping.  Passing
    *             false disables input byte stripping.
    *
    * @throws vpr::IOException if bit strip state could not be set.
    */
   void setBitStripping(bool flag);

   /**
    * Gets the state of start-stop input control.  When enabled, if the
    * terminal driver sees that the receive buffer is getting full, a STOP
    * command is sent to the serial device.  The device should respond by
    * halting its data flow.  When the input queue has been processed, a
    * START command is sent to the serial device which should cause it to
    * resume putting bytes onto the input queue.
    *
    * @pre This serial port is open.
    * @post The current state of start-stop input control is returned to the
    *        caller.
    *
    * @return \c true is returned if start-stop input control is enabled.
    * @return \c false is returned otherwise.
    * @throws vpr::IOException if start-stop input state could not
    *         be retrieved.
    */
   bool getStartStopInputState() const;

   /**
    * Enables or disables start-stop input control.  See
    * getStartStopInputState() for more information.
    *
    * @pre This serial port is open.
    * @post Start-stop input control is enabled or disabled.
    *
    * @param flag A value of true enables start-stop input control.  A value
    *             of false disables it.
    *
    * @throws vpr::IOException if start-stop input state could not be set.
    *
    * @see getStartStopInputState
    */
   void setStartStopInput(bool flag);

   /**
    * Gets the state of start-stop output control.  When enabled, when the
    * terminal driver receives a STOP command, output stops.  When it
    * receives a START command, output resumes.  If disabled, these
    * commands are read as normal characters.
    *
    * @pre This serial port is open.
    * @post The current state of start-stop output control is returned to
    *        the caller.
    *
    * @return \c true is returned if start-stop output control is enabled;
    * @return \c false is returned otherwise.
    * @throws vpr::IOException if start-stop output state could not
    *         be retrieved.
    */
   bool getStartStopOutputState() const;

   /**
    * Enables or disables start-stop output control.  See
    * getStartStopOutputState() for more information.
    *
    * @pre This serial port is open.
    * @post Start-stop output control is enabled or disabled.
    *
    * @param flag A value of true enables start-stop output control.  A value
    *             of false disables it.
    *
    * @throws vpr::IOException if start-stop output state could not be set.
    *
    * @see getStartStopOutputState
    */
   void setStartStopOutput(bool flag);

   /**
    * Gets the current state of parity generation for outgoing bytes and
    * parity checking for incoming bytes.
    *
    * @pre This serial port is open.
    * @post The current parity generation/checking state is returned to the
    *        caller.
    *
    * @return \c true is returned if parity generation and checking are enabled
    *         for outgoing and incoming bytes respectively.
    * @return \c false is returned if parity generation and checking are
    *         disabled.
    * @throws vpr::IOException if parity generation state could not
    *         be retrieved.
    */
   bool getParityGenerationState() const;

   /**
    * Enables or disables parity generation for outgoing bytes and parity
    * checking for incoming bytes.
    *
    * @pre This serial port is open.
    * @post Parity generation and checking are enabled or disabled.
    *
    * @param flag A value of true enables parity generation.  A value of
    *             false disables it.
    *
    * @throws vpr::IOException if partiy generation state could not be set.
    */
   void setParityGeneration(bool flag);

   /**
    * Returns the current state of parity error marking for this serial port.
    *
    * @pre This serial port is open.
    *
    * @return \c true is returned if parity error marking is enabled.
    * @return \c false is returned if parity error masking is disabled.
    * @throws vpr::IOException if parity error marking state
    *         could not be retrieved.
    */
   bool getParityErrorMarkingState() const;

   /**
    * Enables or disables marking of bytes with parity errors or framing errors
    * (except BREAKs).  This is only active if input parity and framing error
    * reporting is enabled (see setInputParityCheck() for more
    * information).  The mark is the three-byte sequence 0377 0 X where X
    * is the byte received in error.  If bit stripping is enabled, a valid
    * 0377 byte is passed as the two-byte sequence 0377 0377.
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
   void setParityErrorMarking(bool flag);

   /**
    * Gets the current parity checking type (either odd or even).
    *
    * @pre This serial port is open.
    * @post The parity type is returned to the caller.
    *
    * @return A vpr::SerialTypes::ParityType value giving the parity
    *         checking being done.
    */
   vpr::SerialTypes::ParityType getParity() const;

   /**
    * Enables odd or even parity depending on the value of the parameter.
    *
    * @pre This serial port is open.
    * @post Odd or even parity is enabled.
    *
    * @param type The type of parity to use.
    *
    * @throws vpr::IOException if parity could not be set.
    */
   void setParity(const vpr::SerialTypes::ParityType& type);

   /**
    * Gets the current input baud setting.
    *
    * @pre This serial port is open.
    * @post The current input baud setting is returned to the caller.
    *
    * @return The current input baud setting is returned.
    *
    * @throws vpr::IOException if input baud setting could not be retrieved.
    */
   vpr::Uint32 getInputBaudRate() const;

   /**
    * Sets the current input baud setting.
    *
    * @pre This serial port is open, and the rate is a valid baud setting.
    * @post The input baud setting is updated to use the given value.
    *
    * @param rate The new input baud setting.
    *
    * @throws vpr::IOException if input baud setting could not be set.
    */
   void setInputBaudRate(const vpr::Uint32& rate);

   /**
    * Gets the current output baud setting.
    *
    * @pre This serial port is open.
    * @post The current output baud setting is returned to the caller.
    *
    * @return The current output baud setting is returned.
    *
    * @throws vpr::IOException if ouput baud setting could not be retrieved.
    */
   vpr::Uint32 getOutputBaudRate() const;

   /**
    * Sets the current output baud setting.
    *
    * @pre This serial port is open, and the rate is a valid baud setting.
    * @post The output baud setting is updated to use the given value.
    *
    * @param rate The new output baud setting.
    *
    * @throws vpr::IOException if ouput baud setting could not be set.
    */
   void setOutputBaudRate(const vpr::Uint32& rate);

   /**
    * Waits for all output to be transmitted.
    *
    * @pre This serial port is open.
    * @post All output is transmitted.
    *
    * @throws vpr::IOException if failed to drain output.
    */
   void drainOutput();

   /**
    * Alters the input or output flow control.  Based on the
    * vpr::SerialTypes::FlowActionOption argument, output can be suspended
    * and restarted or the terminal device can be told to stop or to resume
    * sending data.
    *
    * @pre This serial device is open.
    * @post Based on the argument value, the input or output flow control is
    *       updated.
    *
    * @param opt The flow control change.
    *
    * @throw vpr::IOException If control flow could not be changed.
    */
   void controlFlow(vpr::SerialTypes::FlowActionOption opt);

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
   bool getHardwareFlowControlState() const;

   /**
    * Attempts to enable or disable "hardware" flow control.  While some
    * documents recommend the use of hardware flow control over the use of
    * software flow control, not all platforms implement hardware flow control.
    * Hence, this may have no effect depending on the operating system.
    *
    * @pre This serial port is open.
    * @post If suppported by the operating system, hardware flow control is
    *       enabled.
    *
    * @throws vpr::IOException if hardware flow control state could not be set.
    *
    * @see controlFlow
    */
   void setHardwareFlowControl(bool flag);

   /**
    * Discards either the input buffer (unread data received from the
    * terminal device) or the output buffer (data written but not yet
    * transmitted to the terminal device).  The argument tells which queue
    * (or queues) to flush.
    *
    * @pre This serial device is open.
    * @post Based on the argument, the appropriate queue is flushed of its
    *       current contents.
    *
    * @param queue The queue (or queues) to be flushed.
    *
    * @throws vpr::IOException if queue could not be flushed.
    */
   void flushQueue(vpr::SerialTypes::FlushQueueOption queue);

   /**
    * Transmits a continuous stream of zero bits for the given duration.  If
    * the argument is 0, the transmission will last between 0.25 and 0.5
    * seconds.  Otherwise, the duration specfies the number of seconds to
    * send the zero bit stream.
    *
    * @pre This serial device is open.
    * @post A stream of zero bits are sent to the serial device for the
    *       given duration.
    *
    * @param duration The length of time to send a zero bit stream to the
    *                  serial device.
    *
    * @throws vpr::IOException if break could not be sent.
    */
   void sendBreak(const vpr::Int32 duration);

   /**
    * Returns the status of the carrier detect signal.
    *
    * @return May be platform dependent, but will at least be as follows.
    *         0 - not high, 1 - high, -1 - Not supported.
    *
    * @throws vpr::IOException if carrier detect state could not be retrieved.
    */
   int getCarrierDetect() const;

   /**
    * Returns the status of the data set ready line.
    *
    * @return May be platform dependent, but will at least be as follows.
    *         0 - not high, 1 - high, -1 - Not supported.
    *
    * @throws vpr::IOException if data set ready state could not be retrieved.
    */
   int getDataSetReady() const;

   /**
    * Returns the status of the clear to send.
    *
    * @return May be platform dependent, but will at least be as follows.
    *         0 - not high, 1 - high, -1 - Not supported.
    *
    * @throws vpr::IOException if clear to send state could not be retrieved.
    */
   int getClearToSend() const;

   /**
    * Returns the status of the ring indicator line.
    *
    * @return May be platform dependent, but will at least be as follows.
    *         0 - not high, 1 - high, -1 - Not supported.
    *
    * @throws vpr::IOException if ring indicator state could not be retrieved.
    */
   int getRingIndicator() const;

   /**
    * Sets or clears the data terminal ready line.
    *
    * @param val A boolean parameter indicating whether to set (true) or
    *            clear (false) the data terminal ready line.
    *
    * @throw vpr::IOException If data terminal ready state could not be set.
    */
   void setDataTerminalReady(bool val);

   /**
    * Sets or clears the request-to-send line.
    *
    * @param val A boolean parameter indicating whether to set (true) or
    *            clear (false) the request-to-send line.
    *
    * @throw vpr::IOException If request to send state could not be set.
    */
   void setRequestToSend(bool val);

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
    * @param buffer  A pointer to the buffer where the port's buffer
    *                contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::WouldBlockException if the port is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    *
    * @see vpr::FileHandleImplUNIX::read_i()
    */
   vpr::Uint32 read_i(void* buffer, const vpr::Uint32 length,
                      const vpr::Interval& timeout = vpr::Interval::NoTimeout)
   {
      vprASSERT(mHandle->mFdesc != -1 && "The port may not be open");
      return mHandle->read_i(buffer, length, timeout);
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
    * @param buffer  A pointer to the buffer where the ports's buffer
    *                contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::EOFException if end of port or end of stream has been
    *         reached unexpectedly during input.
    * @throws vpr::IOException if an error ocured while reading.
    *
    * @see vpr::FileHandleImplUNIX::readn_i()
    */
   vpr::Uint32 readn_i(void* buffer, const vpr::Uint32 length,
                       const vpr::Interval& timeout = vpr::Interval::NoTimeout)
   {
      vprASSERT(mHandle->mFdesc != -1 && "The port may not be open");
      return mHandle->readn_i(buffer, length, timeout);
   }

   /**
    * Implementation of the write() template method.  This writes the given
    * buffer to the serial port.
    *
    * @pre The port is open for writing.
    * @post The given buffer is written to the I/O port, and the number
    *       of bytes written successfully is returned to the caller.
    *
    * @param buffer  A pointer to the buffer to be written.
    * @param length  The length of the buffer.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for writing.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes written to the device is returned.
    *
    * @throws vpr::WouldBlockException if the handle is in non-blocking mode,
    *         and the write operation could not be completed.
    * @throws vpr::TimeoutException if the write could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the write operation failed.
    *
    * @see vpr::FileHandleImplUNIX::write_i()
    */
   vpr::Uint32 write_i(const void* buffer, const vpr::Uint32 length,
                       const vpr::Interval& timeout = vpr::Interval::NoTimeout)
   {
      vprASSERT(mHandle->mFdesc != -1 && "The port may not be open");
      return mHandle->write_i(buffer, length, timeout);
   }

protected:
   /**
    * Symbolic names for the four types of flags that can be set in the
    * termios struct.
    */
   enum TermFlag
   {
      IFLAG,      /**< Input flag */
      OFLAG,      /**< Output flag */
      CFLAG,      /**< Control flag */
      LFLAG       /**< Local flag */
   };

   /**
    * Sets the control character at the given index to the given value.
    *
    * @throws vpr::IOException if index is too large for control character array.
    */
   void setControlCharacter(const vpr::Uint32 index, const vpr::Uint8 value);

   /**
    * Gets the value of the control character at the given index.
    *
    * @throws vpr::IOException if index is too large for control character array.
    */
   vpr::Uint8 getControlCharacter(const vpr::Uint32 index) const;

   /**
    * Gets the termios structure for this serial port handle.
    *
    * @throws vpr::IOException if error occured while getting attributes.
    */
   void getAttrs(struct termios* term) const;

   /**
    * Tries to set the attributes for this serial port handle using the given
    * termios structure.  If it fails, the given error message will be
    * printed.
    *
    * @throws vpr::IOException if error occured while setting attributes.
    */
   void setAttrs(struct termios* term, const char* errMsg,
                 const bool printSysErr = true);

   /**
    * Tries to set the attributes for this serial port handle using the given
    * termios structure.  If it fails, the given error message will be
    * printed.
    *
    * @throws vpr::IOException if error occured while setting attributes.
    */
   void setAttrs(struct termios* term, const std::string& errMsg,
                 const bool printSysErr = true);

   /**
    * Gets the value (either on or off) for the named bit.
    *
    * @throws vpr::IOException if named bit could now be retrieved.
    */
   bool getBit(const tcflag_t bit, TermFlag flag) const;

   /**
    * Attempts to enable or disable the named bit.  If it fails, the given
    * error message is printed.
    *
    * @throws vpr::IOException if error occured while attempting
    *         to set the named bit.
    */
   void setBit(const tcflag_t bit, TermFlag flag,
               bool enable, const std::string& errMsg,
               const bool printSysErr = true);

   /** Helper for setting line status flags
    * @throws vpr::IOException if error occured when setting
    *         line flag.
    */
   void setLineFlag(Uint16 flag, bool val);

   /** Helper for getting lines status flags
    * @throws vpr::IOException if error occured when retrieving
    *         line flag.
    */
   bool getLineFlag(Uint16 flag) const;

   /**
    * Converts a termios baud setting to its corresponding integer value.
    */
   vpr::Uint32 baudToInt(const speed_t baud_rate) const;

   /**
    * Converts an integer baud setting to the corresponding termios rate
    * constant.
    */
   speed_t intToBaud(const vpr::Uint32 speed_int) const;

   FileHandleImplUNIX* mHandle;    /**< File handle for the serial port */
   int                 mActions;
};

} // End of vpr namespace


#endif  /* _VPR_SERIAL_PORT_IMPL_TERMIOS_H_ */
