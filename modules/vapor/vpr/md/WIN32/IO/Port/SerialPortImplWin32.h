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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _VPR_SERIAL_PORT_IMPL_WIN32_H_
#define _VPR_SERIAL_PORT_IMPL_WIN32_H_

#include <vpr/vprConfig.h>

#include <string>

#include <vpr/IO/IOSys.h>
#include <vpr/IO/IOException.h>
#include <vpr/IO/Port/SerialTypes.h>
#include <vpr/Util/Interval.h>
#include <vpr/Util/Debug.h>


namespace vpr
{

/** \class SerialPortImplWin32 SerialPortImplWin32.h vpr/IO/Port/SerialPort.h
 *
 * vpr::SerialPort implementation for Win32.  This can be used as a template
 * paramter to vpr::SerialPort_t<T> for the bridge implementation to create
 * the typedef vpr::SerialPort.
 */
class VPR_CLASS_API SerialPortImplWin32
{
public:
   /**
    * Constructor.  This creates a file handle object connected to the given
    * port name and sets the update action to happen immediately.
    *
    * @post mHandle is assigned a value and connected to \p portName, and the
    *       update action is set to vpr::SerialTypes::NOW.
    *
    * @param portName The name of the serial port that will be accessed.
    */
   SerialPortImplWin32(const std::string& portName);

   /**
    * Destructor.  If the serial port is still open, it is closed.
    *
    * @post The serial port is closed.
    */
   ~SerialPortImplWin32();

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
      return mName;
   }

   /**
    * Sets the open flags so that this serial port is opened in read-only
    * mode.
    *
    * @post The open flags are updated so that when the port is opened, it is
    *       opened in read-only mode.  If the port is already open, this has
    *       no effect.
    */
   void setOpenReadOnly()
   {
      mOpenFlag = GENERIC_READ;
   }

   /**
    * Sets the open flags so that this serial port is opened in write-only
    * mode.
    *
    * @post The open flags are updated so that when the port is opened, it is
    *       opened in write-only mode.  If the port is already open, this has
    *       no effect.
    */
   void setOpenWriteOnly()
   {
      mOpenFlag = GENERIC_WRITE;
   }

   /**
    * Sets the open flags so that this serial port is opened in read/write
    * mode.
    *
    * @post The open flags are updated so that when the port is opened, it is
    *       opened in read/write mode.  If the port is already open, this has
    *       no effect.
    */
   void setOpenReadWrite()
   {
      mOpenFlag = GENERIC_READ | GENERIC_WRITE;
   }

   /**
    * Opens this serial port and initializes its flags.
    *
    * @pre This serial port is not already open.
    * @post An attempt is made to open the port.  If it is successful, the
    *       port's flags are initaialized to 0.  The resulting status is
    *       returned to the caller.  If the port is opened, mOpen is set to
    *       true.
    *
    * @throw vpr::WouldBlockException If the serial port is in non-blocking
    *                                 mode and could not be opened yet.
    * @throw vpr::IOException         If the serial port could not be opened.
    */
   void open();

   /**
    * Closes this serial port.
    *
    * @pre The serial port is open.
    * @post An attempt is made to close the port.  The resulting status is
    *       returned to the caller.  If the port is closed, mOpen is set to
    *       false.
    *
    * @throw vpr::IOException If the file handle could not be closed.
    */
   void close();

   /**
    * Reconfigures this serial port so that it is in blocking or non-blocking
    * mode.  If the serial port has not been opened yet, it will be opened in
    * blocking or non-blocking mode when open() is called.
    *
    * @post Processes will block (or not) when accessing this port.
    *
    * @throw vpr::IOException If the blocking mode could not be set.
    */
   void setBlocking(bool flag) throw(IOException);

   /**
    * Get the current blocking state for the serial port.
    *
    * @pre \c mBlocking is set correctly
    *
    * @return \c true is returned if the port is in blocking mode.
    * @return \c false is returned if the port is in non-blocking mode.
    */
   bool isBlocking() const
   {
      return ! (mBlocking & FILE_FLAG_OVERLAPPED);
   }

   /**
    * Returns the contained handle.
    */
   vpr::IOSys::Handle getHandle() const
   {
#ifdef VPR_USE_NSPR
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "ERROR: Cannot get handle for Win32 file descriptor with NSPR!\n";
      return vpr::IOSys::NullHandle;
#else
      return mHandle;
#endif
   }

   /**
    * Tests if this serial port is read-only.
    *
    * @pre This serial port is open.
    * @post The access mode is tested for read-only mode, and the result is
    *       returned to the caller.
    *
    * @return \c true is returned if the device is in read-only mode.
    * @return \c false is returned otherwise.
    */
   bool isReadOnly() const
   {
      return GENERIC_READ == mOpenFlag;
   }

   /**
    * Tests if the serial port is write-only.
    *
    * @pre The serial port is open.
    * @post The access mode is tested for write-only mode, and the result is
    *       returned to the caller.
    *
    * @return \c true is returned if the device is in write-only mode.
    * @return \c false is returned otherwise.
    */
   bool isWriteOnly() const
   {
      return GENERIC_WRITE == mOpenFlag;
   }

   /**
    * Tests if the serial port is read/write.
    *
    * @pre The serial port is open.
    * @post The access mode is tested for read/write mode, and the result is
    *       returned to the caller.
    *
    * @return \c true is returned if the device is in read/write mode;
    * @return \c false is returned otherwise.
    */
   bool isReadWrite() const
   {
      return(GENERIC_READ | GENERIC_WRITE) == mOpenFlag;
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
   void setUpdateAction(vpr::SerialTypes::UpdateActionOption action);

   /**
    * Clears all of this serial port's flags.
    *
    * @pre This serial port is already open.
    * @post An attempt is made to change the flags.  If it is successful, the
    *       port's flags are initaialized to 0.  The resulting status is
    *       returned to the caller.
    *
    * @throw vpr::IOException If all attributes could not be cleared.
    */
   void clearAll();

   /**
    * Queries the serial port for the maximum buffer size.
    *
    * @pre The serial port is open.
    * @post The maximum buffer size is returned to the caller through the
    *       by-reference argument.
    *
    * @param size A reference to a vpr::Uint8 where the buffer size is
    *             stored for return to the caller.
    *
    * @throw vpr::IOException If the buffer size could not be retrieved.
    */
   void getMinInputSize(vpr::Uint16& size) const;

   /**
    * Attempts to change the buffer size to the given argument.
    *
    * @pre The serial port is open.
    * @post If the buffer size is usable on the port, the port attributes
    *       are updated and success is returned.  Otherwise, a failure
    *       status is returned.
    *
    * @param size The new size for the buffer.
    *
    * @throw vpr::IOException If the buffer size could not be set
    *                         successfully.
    */
   void setMinInputSize(const vpr::Uint8 size);

   /**
    * Gets the value of the timeout (in tenths of a second) to wait for data
    * to arrive.  This is only applicable in non-canonical mode.
    *
    * @pre The serial port is open.
    * @post The current timeout setting is returned to the caller in the
    *       by-reference argument.
    *
    * @param timeout A reference to a vpr::Uint8 to be used as storage for
    *                the timeout value.
    *
    * @throw vpr::IOException If the timeout could not be retrieved.
    */
   void getTimeout(vpr::Uint8& timeout) const;

   /**
    * Sets the value of the timeout to wait for data to arrive.  The value
    * given must be in tenths of a second.  This is only applicable in
    * non-canonical mode.
    *
    * @pre The serial port is open.
    * @post The timeout interval is updated to use the given value.
    *
    * @param timeout The new timeout value measured in tenths of a second.
    *
    * @throw vpr::IOException If the timeout could not be set successfully.
    */
   void setTimeout(const vpr::Uint8 timeout);

   /**
    * Gets the character size (the bits per byte).
    *
    * @pre The serial port is open.
    * @post The current character size is returned to the caller in the
    *       by-reference argument.
    *
    * @param size A reference to a vpr::SerialTypes::CharacterSizeOption
    *             object to be used as storage for the character size
    *             value.
    *
    * @throw vpr::IOException If the character size could not be retrieved.
    */
   void getCharacterSize(vpr::SerialTypes::CharacterSizeOption& size) const;

   /**
    * Sets the current character size (the bits per byte) to the size in the
    * given value.  This is used for both reding and writing, and the size
    * does not include the parity bit (if any).
    *
    * @pre The serial port is open.
    * @post The bits per byte for the serial port is updated.
    *
    * @param bpb  The new character size.
    *
    * @throw vpr::IOexception If character size could not be set.
    */
   void setCharacterSize(const vpr::SerialTypes::CharacterSizeOption bpb);

   /**
    * Gets the current read state for the port.
    *
    * @pre The serial port is open.
    * @post The read state (either enabled or disabled) is returne to the
    *       caller.
    *
    * @return \c true is returned if bytes can be read from the device.
    */
   bool getReadState() const
   {
      return true;
   }

   /**
    * Enables the receiver so that bytes can be read from the port.
    *
    * @pre The serial port is open.
    * @post An attempt is made to make the device readable.
    *
    * @throw vpr::IOException If read state could not be set.
    */
   void setRead(bool flag)
   {
      /* Do nothing. */ ;
   }

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
    * @throw vpr::IOException If local attach state could not be retrieved.
    */
   bool getLocalAttachState() const;

   /**
    * Mark the device as locally attached or not.
    *
    * @pre This serial port is open.
    * @post The device has its local attachment state enabled.
    *
    * @throw vpr::IOException If local attach state could not be set.
    */
   void setLocalAttach(bool flag);

   /**
    * Get the current state of ignoring BREAK bytes.
    *
    * @pre This serial port is open.
    * @post The BREAK byte ignore state is returned to the caller.
    *
    * @return \c true is returned if bad bytes are ignored.
    * @return \c false is returned if bad bytes are not ignored.
    *
    * @throw vpr::IOException If break byte ignore state could not
    *                         be retrieved.
    */
   bool getBreakByteIgnoreState() const;

   /**
    * Enable ignoring of received BREAK bytes or not.
    *
    * @pre This serial port is open.
    * @post BREAK byte ignoring is enabled.
    *
    * @throw vpr::IOException If break byte ignore state could not be set.
    */
   void setBreakByteIgnore(bool flag);

   /**
    * Gets the number of stop bits in use.  This will be either 1 or 2.
    *
    * @pre The serial port is open.
    * @post The number of stop bits currently in use is returned via the
    *       by-reference argument.
    *
    * @param numBits A reference to a vpr::Uint8 that will be used to
    *                store the number of stop bits.
    *
    * @throw vpr::IOException If stop bits could not be retrieved.
    */
   void getStopBits(vpr::Uint8& numBits) const;

   /**
    * Sets the number of stop bits to use.  The value must be either 1 or 2.
    *
    * @pre The serial port is open, and the given number of stop bits is
    *      either 1 or 2.
    * @post The number of stop bits is updated to use the given value.
    *
    * @param numBits The number of stop bits to use.
    *
    * @throw vpr::IOException if stop bits could not be set.
    */
   void setStopBits(const vpr::Uint8 numBits);

   /**
    * Queries the canonical input state of the serial port.  If canonical mode
    * is enabled, the characters EOF, EOL, EOL2, ERASE, KILL, REPRINT,
    * STATUS, and WERASE are enabled, and the input characters are
    * assembled into lines.  Otherwise, read requests are satisfied directly
    * from the input queue, and a read will not return until the buffer is
    * at its minimum capacity or the timeout has expired.  See
    * getMinInputSize() and getTimeout() for more information.
    *
    * @pre The serial port is open.
    * @post The current state of canonical input is returnd to the caller.
    *
    * @return \c true if canonical input is enabled; \c false otherwise.
    *
    * @throw vpr::IOException If canonical state could not be retrieved.
    */
   bool getCanonicalState() const;

   /**
    * Enables or disables canonical input.  See getCanonicalState() for more
    * information about what this means.
    *
    * @pre The serial port is open.
    * @post Canonical input is enabled.
    *
    * @throw vpr::IOException If canonical state could not be set.
    *
    * @see getCanonicalState
    */
   void setCanonicalInput(bool flag);

   /**
    * Gets the current state of ignoring bytes with framing errors (other
    * than a BREAK) or parity errors.
    *
    * @pre The serial port is open.
    * @post The erroneous byte ignore state is returned to the caller.
    *
    * @return \c true if bad bytes are ignored; \c false otherwise.
    *
    * @throw vpr::IOException If bad byte ignore state could not be retrieved.
    */
   bool getBadByteIgnoreState() const;

   /**
    * Enables or disables ignoring of received bytes with framing errors or
    * parity errors.
    *
    * @pre The serial port is open.
    * @post Erroneous byte ignoring is enabled.
    *
    * @throw vpr::IOException If bad bytes ignore could not be set.
    */
   void setBadByteIgnore(bool flag);

   /**
    * Gets the state of parity checking for input.
    *
    * @pre The serial port is open.
    * @post The state of input parity checking is returned to the caller.
    *
    * @return \c true if input parity checking is enabled; \c false otherwise.
    *
    * @throw vpr::IOException If input parity check state could not be
    *                         retrieved.
    */
   bool getInputParityCheckState() const;

   /**
    * Enables or disables input parity checking.
    *
    * @pre The serial port is open.
    * @post Input parity checking is enabled.
    *
    * @throw vpr::IOException If input parity check state could not be set.
    */
   void setInputParityCheck(bool flag);

   /**
    * Gets the current state of bit stripping.  When enabled, input bytes are
    * stripped to seven bits.  Otherwise, all eight bits are processed.
    *
    * @pre The serial port is open.
    * @post The current bit stripping state is returned to the caller.
    *
    * @return \c true if input bytes are stripped to seven bits.
    * @return \c false is returned if input bytes are not stripped.
    *
    * @throw vpr::IOException If bit strip state could not be retrieved.
    */
   bool getBitStripState() const;

   /**
    * Enables or disables stripping of input bytes to seven bits.
    *
    * @pre The serial port is open.
    * @post Input bit stripping is enabled.
    *
    * @throw vpr::IOException If bit strip state could not be set.
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
    * @pre The serial port is open.
    * @post The current state of start-stop input control is returned to the
    *        caller.
    *
    * @return \c true is returned if start-stop input control is enabled.
    * @return \c false is returned otherwise.
    *
    * @throw vpr::IOException If start-stop input state could not be retrieved.
    */
   bool getStartStopInputState() const;

   /**
    * Enables or disables start-stop input control.  See
    * getStartStopInputState() for more information.
    *
    * @pre The serial port is open.
    * @post Start-stop input control is enabled.
    *
    * @throw vpr::IOException If start-stop input state could not be set.
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
    * @pre The serial port is open.
    * @post The current state of start-stop output control is returned to
    *        the caller.
    *
    * @return \c true is returned if start-stop output control is enabled
    * @return \c false is returned otherwise.
    *
    * @throw vpr::IOException If start-stop output state could not be
    *                         retrieved.
    */
   bool getStartStopOutputState() const;

   /**
    * Enables or disables start-stop output control.  See
    * getStartStopOutputState() for more information.
    *
    * @pre The serial port is open.
    * @post Start-stop output control is ensabled.
    *
    * @throw vpr::IOException if start-stop output state could not be set.
    *
    * @see getStartStopOutputState
    */
   void setStartStopOutput(bool flag);

   /**
    * Gets the current state of parity generation for outgoing bytes and
    * parity checking for incoming bytes.
    *
    * @pre The serial port is open.
    * @post The current parity generation/checking state is returned to the
    *        caller.
    *
    * @return \c true if parity generation and checking are enabled for
    *         outgoing and incoming bytes respectively.
    * @return \c false is returned if parity generation and checking are
    *         disabled.
    *
    * @throw vpr::IOException If parity generation state could not be
    *                         retrieved.
    */
   bool getParityGenerationState() const;

   /**
    * Enables or disables parity generation for outgoing bytes and parity
    * checking for incoming bytes.
    *
    * @pre This serial port is open.
    * @post Parity generation and checking are enabled.
    *
    * @throw vpr::IOException If partiy generation state could not be set.
    */
   void setParityGeneration(bool flag);

   /**
    * Returns the current state of parity error marking for this serial port.
    *
    * @pre This serial port is open.
    *
    * @return \c true is returned if parity error marking is enabled.
    * @return \c false is returned if parity error masking is disabled.
    *
    * @throw vpr::IOException If parity error marking state could not be
    *                         retrieved.
    */
   bool getParityErrorMarkingState() const;

   /**
    * Enables marking of bytes with parity errors or framing errors (except
    * BREAKs).  This is only active if input parity and framing error
    * reporting is enabled (see setInputParityCheck() for more
    * information).  The mark is the three-byte sequence \377 \0 X where X
    * is the byte received in error.  If bit stripping is enabled, a valid
    * \377 byte is passed as the two-byte sequence \377 \377.
    *
    * @pre This serial port is open.
    * @post Bytes with an error are marked and passed on to the reader.
    *
    * @throw vpr::IOException If parity error marking state could not be set.
    *
    * @see setInputParityCheck
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
   SerialTypes::ParityType getParity() const;

   /**
    * Enables odd or even parity.
    *
    * @pre This serial port is open.
    * @post Odd or even parity is enabled.
    *
    * @throw vpr::IOException If parity could not be set.
    */
   void setParity(const SerialTypes::ParityType& type);

   /**
    * Gets the current input baud rate.
    *
    * @pre This serial port is open.
    * @post The current input baud rate is returned to the caller via the
    *        by-reference parameter.
    *
    * @param rate A reference to a vpr::Uint32 to be used as storage for
    *             the current input baud rate.
    *
    * @throw vpr::IOException If input baud rate could not be retrieved.
    */
   void getInputBaudRate(vpr::Uint32& rate) const;

   /**
    * Sets the current input baud rate.
    *
    * @pre This serial port is open, and the rate is a valid baud rate.
    * @post The input baud rate is updated to use the given value.
    *
    * @param rate The new input baud rate.
    *
    * @throw vpr::IOException If input baud rate could not be set.
    */
   void setInputBaudRate(const vpr::Uint32& rate);

   /**
    * Gets the current output baud rate.
    *
    * @pre This serial port is open.
    * @post The current output baud rate is returned to the caller via the
    *        by-reference parameter.
    *
    * @param rate A reference to a vpr::Uint32 to be used as storage for
    *             the current output baud rate.
    *
    * @throw vpr::IOException If ouput baud rate could not be retrieved.
    */
   void getOutputBaudRate(vpr::Uint32& rate) const;

   /**
    * Sets the current output baud rate.
    *
    * @pre This serial port is open, and the rate is a valid baud rate.
    * @post The output baud rate is updated to use the given value.
    *
    * @param rate The new output baud rate.
    *
    * @throw vpr::IOException if ouput baud rate could not be set.
    */
   void setOutputBaudRate(const vpr::Uint32& rate);

   /**
    * Waits for all output to be transmitted.
    *
    * @pre This serial port is open.
    * @post All output is transmitted.
    *
    * @throw vpr::IOException If failed to drain output.
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
    * @return \c true is returned if hardware flow control is enabled.
    * @return \c false is returned otherwise.
    *
    * @throw vpr::IOException If hardware flow control state could not be
    *                         retrieved.
    *
    * @see controlFlow
    */
   bool getHardwareFlowControlState() const;

   /**
    * Attempts to enable "hardware" flow control.  While some documents
    * recommend the use of hardware flow control over the use of software
    * flow control, not all platforms implement hardware flow control.  Hence,
    * this may have no effect depending on the operating system.
    *
    * @throw vpr::IOException If hardware flow control state could not be set.
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
    * @throw vpr::IOException If hardware flow control state could not be set.
    */
   void flushQueue(vpr::SerialTypes::FlushQueueOption queue);

   /**
    * Transmits a continuous stream of zero bits for the given duration.  If
    * the argument is 0, the transmission will last between 0.25 and 0.5
    * seconds.  Otherwise, the duration specfies the number of seconds to
    * send the zero bit stream.
    *
    * @pre The serial device is open.
    * @post A stream of zero bits are sent to the serial device for the
    *       given duration.
    *
    * @param duration The length of time to send a zero bit stream to the
    *                 serial device.
    *
    * @throw vpr::IOException If break could not be sent.
    */
   void sendBreak(const vpr::Int32 duration);

   /**
    * Returns the status of the carrier detect signal.
    *
    * @return May be platform dependent, but will at least be as follows:
    *         0 - not high, 1 - high, -1 - Not supported.
    *
    * @throw vpr::IOException If carrier detect state could not be retrieved.
    */
   int getCarrierDetect() const
   {
      return -1;
   }

   /**
    * Returns the status of the data set ready line.
    *
    * @return May be platform dependent, but will at least be as follows:
    *         0 - not high, 1 - high, -1 - Not supported.
    *
    * @throw vpr::IOException If data set ready state could not be retrieved.
    */
   int getDataSetReady() const
   {
      return -1;
   }

   /**
    * Returns the status of the clear to send.
    *
    * @return May be platform dependent, but will at least be as follows:
    *         0 - not high, 1 - high, -1 - Not supported.
    *
    * @throw vpr::IOException If clear to send state could not be retrieved.
    */
   int getClearToSend() const
   {
      return -1;
   }

   /**
    * Returns the status of the ring indicator line.
    *
    * @return May be platform dependent, but will at least be as follows:
    *         0 - not high, 1 - high, -1 - Not supported.
    *
    * @throw vpr::IOException If ring indicator state could not be retrieved.
    */
   int getRingIndicator() const
   {
      return -1;
   }

   /**
    * Sets or clears the data terminal ready line.
    *
    * @param val A boolean parameter indicating whether to set (true) or
    *            clear (false) the data terminal ready line.
    *
    * @throw vpr::IOException If the data terminal ready state could not be
    *                         set.
    */
   void setDataTerminalReady(bool val);

   /**
    * Sets or clears the ready-to-send line.
    *
    * @param val A boolean parameter indicating whether to set (true) or
    *            clear (false) the ready-to-send line.
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
    *       caller via the \p bytesRead parameter.
    *
    * @param buffer    A pointer to the buffer where the port's buffer
    *                  contents are to be stored.
    * @param length    The number of bytes to be read.
    * @param bytesRead The number of bytes read into the buffer.
    * @param timeout   The maximum amount of time to wait for data to be
    *                  available for reading.  This argument is optional and
    *                  defaults to vpr::Interval::NoTimeout.
    *
    * @throw vpr::WouldBlockException If the port is in non-blocking mode,
    *                                 and there is no data to read.
    * @throw vpr::TimeoutException    If the read could not begin within the
    *                                 timeout interval.
    * @throw vpr::IOException         If the read operation failed.
    */
   void read_i(void* buffer, const vpr::Uint32 length, vpr::Uint32& bytesRead,
               const vpr::Interval timeout = vpr::Interval::NoTimeout);

   /**
    * Implementation of the readn() template method.  This reads exactly the
    * specified number of bytes from the serial port into the given buffer.
    *
    * @pre The port is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it from the port,
    *       and the number of bytes read successfully is returned to the caller
    *       via the \p bytesRead parameter.
    *
    * @param buffer    A pointer to the buffer where the ports's buffer
    *                  contents are to be stored.
    * @param length    The number of bytes to be read.
    * @param bytesRead The number of bytes read into the buffer.
    * @param timeout   The maximum amount of time to wait for data to be
    *                  available for reading.  This argument is optional and
    *                  defaults to vpr::Interval::NoTimeout.
    *
    * @throw vpr::EOFException If end of port or end of stream has been
    *                          reached unexpectedly during input.
    * @throw vpr::IOException  If an error ocured while reading.
    */
   void readn_i(void* buffer, const vpr::Uint32 length, vpr::Uint32& bytesRead,
                const vpr::Interval timeout = vpr::Interval::NoTimeout);

   /**
    * Implementation of the write() template method.  This writes the given
    * buffer to the serial port.
    *
    * @pre The port is open for writing.
    * @post The given buffer is written to the I/O port, and the number
    *       of bytes written successfully is returned to the caller via the
    *       \p bytesWritten parameter.
    *
    * @param buffer       A pointer to the buffer to be written.
    * @param length       The length of the buffer.
    * @param bytesWritten The number of bytes written to the port.
    * @param timeout      The maximum amount of time to wait for data to be
    *                     available for writing.  This argument is optional
    *                     and defaults to vpr::Interval::NoTimeout.
    *
    * @throw vpr::WouldBlockException If the handle is in non-blocking mode,
    *                                 and the write operation could not be
    *                                 completed.
    * @throw vpr::TimeoutException    If the write could not begin within the
    *                                 timeout interval.
    * @throw vpr::IOException         If the write operation failed.
    */
   void write_i(const void* buffer, const vpr::Uint32 length,
                vpr::Uint32& bytesWritten,
                const vpr::Interval timeout = vpr::Interval::NoTimeout);

protected:
   HANDLE mHandle;     /**< handle to communication file */
   DWORD mOpenFlag;    /**< flag to specify how to open comm port */
   std::string mName;  /**< name of communication device */
   bool mOpen;
   DWORD mBlocking;    /**< flag for blocking I/O */
   bool mParityMark;   /**< flag for parity marking */
   vpr::Uint8 mCurrentTimeout;
};

} // End of vpr namespace


#endif  /* _VPR_SERIAL_PORT_IMPL_WIN32_H_ */
