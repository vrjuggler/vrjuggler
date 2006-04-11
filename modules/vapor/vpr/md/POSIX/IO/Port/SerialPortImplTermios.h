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
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

/**
 * vpr::SerialPort implementation using termios.
 * For more information about serial ports on Linux, refer to
 * http://www.ibiblio.org/mdw/HOWTO/Serial-HOWTO-3.html
 */
class SerialPortImplTermios
{
public:
   /**
    * Constructor.  This creates a file handle object connected to the given
    * port name and sets the update action to happen immediately.
    *
    * @pre None.
    * @post mHandle->getHandle() is allocated and connected to port_name, and
    *       the update action is set to vpr::SerialTypes::NOW.
    *
    * @param port_name The name of the serial port that will be accessed.
    */
   SerialPortImplTermios(const std::string& port_name);

   /**
    * Destructor.  If the file handle is non-NULL, its memory is released.
    *
    * @pre None.
    * @post If mHandle->getHandle() is non-NULL, its memory is released.
    */
   ~SerialPortImplTermios();

   // ========================================================================
   // vpr::BlockIO basics.
   // ========================================================================

   /**
    * Gets the name of this serial port.
    *
    * @pre None.
    * @post
    *
    * @return An object containing the name of the serial port.
    */
   const std::string& getName()
   {
      return mHandle->getName();
   }

   /**
    * Sets the open flags so that the serial port is opened in read-only
    * mode.
    *
    * @pre None.
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
    * @pre None.
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
    * @pre None.
    * @post The open flags are updated so that when the port is opened, it is
    *       opened in read/write mode.  If the port is already open, this has
    *       no effect.
    */
   void setOpenReadWrite()
   {
      mHandle->setOpenReadWrite();
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
   void setOpenBlocking()
   {
      mHandle->setOpenBlocking();
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
   void setOpenNonBlocking()
   {
      mHandle->setOpenNonBlocking();
   }

   /**
    * Opens the serial port and initialize its flags.
    *
    * @pre This serial port is not already open.
    * @post An attempt is made to open the port.  If it is successful, the
    *       port's flags are initaialized to 0.  The resulting status is
    *       returned to the caller.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the serial port was
    *         opened successfully.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus open();

   /**
    * Clears all of the serial port's flags
    *
    * @pre This serial port is already open.
    * @post An attempt is made to change the flags.  If it is successful, the
    *       port's flags are initaialized to 0.  The resulting status is
    *       returned to the caller.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the serial port was
    *         opened successfully.
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus clearAll();


   /**
    * Closes the serial port.
    *
    * @pre This serial port is open.
    * @post An attempt is made to close the port.  The resulting status is
    *       returned to the caller.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the serial port was
    *         closed successfully.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus close()
   {
      vprASSERT(mHandle->mFdesc != -1 && "The port may not be open");
      return mHandle->close();
   }

   /**
    * Reconfigures the serial port so that it is in blocking mode.
    *
    * @pre This serial port is open.
    * @post Processes may block when accessing the port.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the blocking mode was
    *         changed successfully; vpr::ReturnStatus::Fail otherwise.
    */
   vpr::ReturnStatus enableBlocking()
   {
      vprASSERT(mHandle->mFdesc != -1 && "The port may not be open");
      return mHandle->enableBlocking();
   }

   /**
    * Reconfigures the serial port so that it is in non-blocking mode.
    *
    * @pre This serial port is open.
    * @post Processes will not block when accessing the port.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the blocking mode was
    *         changed successfully; vpr::ReturnStatus::Fail otherwise.
    */
   vpr::ReturnStatus enableNonBlocking()
   {
      vprASSERT(mHandle->mFdesc != -1 && "The port may not be open");
      return mHandle->enableNonBlocking();
   }

   /**
    * Get the current blocking state for the serial port.
    *
    * @pre mBlocking is set correctly
    *
    * @return true is returned if the port is in blocking mode.<br>
    *         false is returned if the port is in non-blocking mode.
    */
   bool getBlocking() const
   {
      return mHandle->getBlocking();
   }

   /**
    * Gets the current non-blocking state for the serial port.
    *
    * @pre mBlocking is set correctly.
    *
    * @return <code>true</code> is returned if the port is in non-blocking
    *         mode.   Otherwise, <code>false</code> is returned.
    */
   bool getNonBlocking() const
   {
      return mHandle->getNonBlocking();
   }

   /**
    * Returns the contained handle.
    */
   vpr::IOSys::Handle getHandle()
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
    * @return <code>true</code> is returned if the device is in read-only
    *         mode; <code>false</code> otherwise.
    */
   bool isReadOnly()
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
    * @return <code>true</code> is returned if the device is in write-only
    *         mode; <code>false</code> otherwise.
    */
   bool isWriteOnly()
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
    * @return <code>true</code> is returned if the device is in read/write
    *         mode; <code>false</code> otherwise.
    */
   bool isReadWrite()
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
   vpr::SerialTypes::UpdateActionOption getUpdateAction();

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
    * Queries the serial port for the maximum buffer size.
    *
    * @pre This serial port is open.
    * @post The maximum buffer size is returned to the caller through the
    *       by-reference argument.
    *
    * @param size A reference to a vpr::Uint8 where the buffer size is
    *             stored for return to the caller.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the buffer size was
    *         retrieved successfully; vpr::ReturnStatus::Fail otherwise.
    */
   vpr::ReturnStatus getBufferSize(Uint16& size);

   /**
    * Attempts to change the buffer size to the given argument.
    *
    * @pre This serial port is open.
    * @post If the buffer size is usable on the port, the port attributes
    *       are updated and success is returned.  Otherwise, a failure
    *       status is returned.
    *
    * @param size The new size for the buffer.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the buffer size was set
    *         successfully; vpr::ReturnStatus::Fail otherwise.
    */
   vpr::ReturnStatus setBufferSize(const Uint8 size);

   /**
    * Gets the value of the timeout (in tenths of a second) to wait for data
    * to arrive.  This is only applicable in non-canonical mode.
    *
    * @pre This serial port is open.
    * @post The current timeout setting is returned to the caller in the
    *       by-reference argument.
    *
    * @param timeout A reference to a vpr::Uint8 to be used as storage for
    *                the timeout value.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the timeout was requested
    *         successfully; vpr::ReturnStatus::Fail otherwise.
    *
    * @see Refer to page 353 of <I>Advanced Programming in the UNIX
    *      Environment</I> for more details.
    */
   vpr::ReturnStatus getTimeout(Uint8& timeout);

   /**
    * Sets the value of the timeout to wait for data to arrive.  The value
    * given must be in tenths of a second.  This is only applicable in
    * non-canonical mode.
    *
    * @pre This serial port is open.
    * @post The timeout interval is updated to use the given value.
    *
    * @param timeout_val The new timeout value measured in tenths of a
    *                    second.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the timeout was set
    *         successfully; vpr::ReturnStatus::Fail otherwise.
    *
    * @see Refer to page 353 of <I>Advanced Programming in the UNIX
    *      Environment</I> for more details.
    */
   vpr::ReturnStatus setTimeout(const Uint8 timeout);

   /**
    * Gets the character size (the bits per byte).
    *
    * @pre This serial port is open.
    * @post The current character size is returned to the caller in the
    *       by-reference argument.
    *
    * @param size A reference to a vpr::SerialTypes::CharacterSizeOption
    *             object to be used as storage for the character size
    *             value.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus getCharacterSize(SerialTypes::CharacterSizeOption& size);

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
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setCharacterSize(const vpr::SerialTypes::CharacterSizeOption bpb);

   /**
    * Gets the current read state for the port.
    *
    * @pre This serial port is open.
    * @post The read state (either enabled or disabled) is returne to the
    *       caller.
    *
    * @return true is returned if bytes can be read from the device.  false is
    *         returned otherwise.
    */
   bool getReadState();

   /**
    * Enables the receiver so that bytes can be read from the port.
    *
    * @pre This serial port is open.
    * @post An attempt is made to make the device readable.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus enableRead();

   /**
    * Disables the receiver so that bytes cannot be read from the port.
    *
    * @pre This serial port is open.
    * @post An attempt is made to make the device unreadable.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus disableRead();

   /**
    * Get the local attach state.  That is, test if the device is attached
    * locally.
    *
    * @pre The serial port is open.
    * @post The local attach state is returned to the caller.
    * @return true  - The device is attached locally.
    *         false - The device is not attached locally, and opening the
    *                 device will usually block until there is a response.
    */
   bool getLocalAttachState();

   /**
    * Mark the device as locally attached.
    *
    * @pre This serial port is open.
    * @post The device has its local attachment state enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus enableLocalAttach();

   /**
    * Mark the device as not locally attached.
    *
    * @pre This serial port is open.
    * @post The device has its local attachment state disabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus disableLocalAttach();

   /**
    * Gets the number of stop bits in use.  This will be either 1 or 2.
    *
    * @pre This serial port is open.
    * @post The number of stop bits currently in use is returned via the
    *       by-reference argument.
    *
    * @param num_bits A reference to a vpr::Uint8 that will be used to
    *                 store the number of stop bits.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus getStopBits(Uint8& num_bits);

   /**
    * Sets the number of stop bits to use.  The value must be either 1 or 2.
    *
    * @pre This serial port is open, and the given number of stop bits is
    *       either 1 or 2.
    * @post The number of stop bits is updated to use the given value.
    *
    * @param num_bits The number of stop bits to use.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setStopBits(const Uint8 num_bits);

   /**
    * Queries the canonical input state of the serial port.  If canonical mode
    * is enabled, the characters EOF, EOL, EOL2, ERASE, KILL, REPRINT,
    * STATUS, and WERASE are enabled, and the input characters are
    * assembled into lines.  Otherwise, read requests are satisfied directly
    * from the input queue, and a read will not return until the buffer is
    * at its minimum capacity or the timeout has expired.  See
    * getBufferSize() and getTimeout() for more information.
    *
    * @pre This serial port is open.
    * @post The current state of canonical input is returnd to the caller.
    *
    * @return true if canonical input is enabled; false otherwise.
    */
   bool getCanonicalState();

   /**
    * Enables canonical input.  See getCanonicalState() for more information
    * about what this means.
    *
    * @pre This serial port is open.
    * @post Canonical input is enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus enableCanonicalInput();

   /**
    * Disables canonical input.  See getCanonicalState() for more information
    * about what this means.
    *
    * @pre This serial port is open.
    * @post Canonical input is disabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus disableCanonicalInput();

   /**
    * Gets the current state of ignoring bytes with framing errors (other
    * than a BREAK) or parity errors.
    *
    * @pre This serial port is open.
    * @post The erroneous byte ignore state is returned to the caller.
    *
    * @return true if bad bytes are ignored; false otherwise.
    */
   bool getBadByteIgnoreState();

   /**
    * Enables ignoring of received bytes with framing errors or parity
    * errors.
    *
    * @pre This serial port is open.
    * @post Erroneous byte ignoring is enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus enableBadByteIgnore();

   /**
    * Disables ignoring of received bytes with framing errors or parity
    * errors.
    *
    * @pre This serial port is open.
    * @post Erroneous byte ignoring is disabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus disableBadByteIgnore();

   /**
    * Gets the current state of ignoring BREAK bytes
    *
    * @pre This serial port is open.
    * @post The BREAK byte ignore state is returned to the caller.
    *
    * @return true if bad bytes are ignored; false otherwise.
    */
   bool getBreakByteIgnoreState();

   /**
    * Enables ignoring of received BREAK bytes
    *
    * @pre This serial port is open.
    * @post BREAK byte ignoring is enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus enableBreakByteIgnore();

   /**
    * Disables ignoring of received BREAK bytes
    *
    * @pre This serial port is open.
    * @post BREAK byte ignoring is disabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus disableBreakByteIgnore();

   /**
    * Gets the state of parity checking for input.
    *
    * @pre This serial port is open.
    * @post The state of input parity checking is returned to the caller.
    *
    * @return true if input parity checking is enabled; false otherwise.
    */
   bool getInputParityCheckState();

   /**
    * Enables input parity checking.
    *
    * @pre This serial port is open.
    * @post Input parity checking is enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus enableInputParityCheck();

   /**
    * Disables input parity checking.
    *
    * @pre This serial port is open.
    * @post Input parity checking is disabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus disableInputParityCheck();

   /**
    * Gets the current state of bit stripping.  When enabled, input bytes are
    * stripped to seven bits.  Otherwise, all eight bits are processed.
    *
    * @pre This serial port is open.
    * @post The current bit stripping state is returned to the caller.
    *
    * @return true if input bytes are stripped to seven bits.  false is
    *         returned if input bytes are not stripped.
    */
   bool getBitStripState();

   /**
    * Enables stripping of input bytes to seven bits.
    *
    * @pre This serial port is open.
    * @post Input bit stripping is enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus enableBitStripping();

   /**
    * Disables stripping of input bytes to seven bits.
    *
    * @pre This serial port is open.
    * @post Input bit stripping is disabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus disableBitStripping();

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
    * @return true if start-stop input control is enabled; false otherwise.
    */
   bool getStartStopInputState();

   /**
    * Enables start-stop input control.  See getStartStopInputState() for
    * more information.
    *
    * @pre This serial port is open.
    * @post Start-stop input control is enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus enableStartStopInput();

   /**
    * Disables start-stop input control.  See getStartStopInputState() for
    * more information.
    *
    * @pre This serial port is open.
    * @post Start-stop input control is disabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus disableStartStopInput();

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
    * @return true if start-stop output control is enabled; false otherwise.
    */
   bool getStartStopOutputState();

   /**
    * Enables start-stop input control.  See getStartStopInputState() for
    * more information.
    *
    * @pre This serial port is open.
    * @post Start-stop input control is ensabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus enableStartStopOutput();

   /**
    * Disables start-stop output control.  See getStartStopOutputState() for
    * more information.
    *
    * @pre This serial port is open.
    * @post Start-stop output control is disabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus disableStartStopOutput();

   /**
    * Gets the current state of parity generation for outgoing bytes and
    * parity checking for incoming bytes.
    *
    * @pre This serial port is open.
    * @post The current parity generation/checking state is returned to the
    *        caller.
    *
    * @return true if parity generation and checking are enabled for
    *         outgoing and incoming bytes respectively.  false is returned
    *         if parity generation and checking are disabled.
    */
   bool getParityGenerationState();

   /**
    * Enables parity generation for outgoing bytes and parity checking for
    * incoming bytes.
    *
    * @pre This serial port is open.
    * @post Parity generation and checking are enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus enableParityGeneration();

   /**
    * Disables parity generation for outgoing bytes and parity checking for
    * incoming bytes.
    *
    * @pre This serial port is open.
    * @post Parity generation and checking are disabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus disableParityGeneration();

   /**
    * Returns the current state of parity error marking for this serial port.
    *
    * @pre This serial port is open.
    *
    * @return true is returned if parity error marking is enabled.  false is
    *         returned if parity error masking is disabled.
    */
   bool getParityErrorMarkingState();

   /**
    * Enables marking of bytes with parity errors or framing errors (except
    * BREAKs).  This is only active if input parity and framing error
    * reporting is enabled (see enableInputParityCheck() for more
    * information).  The mark is the three-byte sequence \377 \0 X where X
    * is the byte received in error.  If bit stripping is enabled, a valid
    * \377 byte is passed as the two-byte sequence \377 \377.
    *
    * @pre This serial port is open.
    * @post Bytes with an error are marked and passed on to the reader.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    *
    * @see enableInputParityCheck
    */
   vpr::ReturnStatus enableParityErrorMarking();

   /**
    * Disables parity error and framing error marking.
    *
    * @pre This serial port is open.
    * @post Parity error marking is disabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    *
    * @see enableParityErrorMarking
    */
   vpr::ReturnStatus disableParityErrorMarking();

   /**
    * Gets the current parity checking type (either odd or even).
    *
    * @pre This serial port is open.
    * @post The parity type is returned to the caller.
    *
    * @return A vpr::SerialTypes::ParityType value giving the parity
    *         checking being done.
    */
   vpr::SerialTypes::ParityType getParity();

   /**
    * Enables odd parity.
    *
    * @pre This serial port is open.
    * @post Odd parity is enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setOddParity();

   /**
    * Enable even parity.
    *
    * @pre This serial port is open.
    * @post Even parity is enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setEvenParity();

   /**
    * Gets the current input baud rate.
    *
    * @pre This serial port is open.
    * @post The current input baud rate is returned to the caller via the
    *       by-reference parameter.
    *
    * @param rate A reference to a vpr::Uint32 to be used as storage for
    *             the current input baud rate.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus getInputBaudRate(vpr::Uint32& rate);

   /**
    * Sets the current input baud rate.
    *
    * @pre This serial port is open, and the rate is a valid baud rate.
    * @post The input baud rate is updated to use the given value.
    *
    * @param rate The new input baud rate.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setInputBaudRate(const vpr::Uint32 rate);

   /**
    * Gets the current output baud rate.
    *
    * @pre This serial port is open.
    * @post The current output baud rate is returned to the caller via the
    *       by-reference parameter.
    *
    * @param rate A reference to a vpr::Uint32 to be used as storage for
    *              the current output baud rate.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus getOutputBaudRate(vpr::Uint32& rate);

   /**
    * Sets the current output baud rate.
    *
    * @pre This serial port is open, and the rate is a valid baud rate.
    * @post The output baud rate is updated to use the given value.
    *
    * @param rate The new output baud rate.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus setOutputBaudRate(const Uint32 rate);

   /**
    * Waits for all output to be transmitted.
    *
    * @pre This serial port is open.
    * @post All output is transmitted.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus drainOutput();

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
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus controlFlow(vpr::SerialTypes::FlowActionOption opt);

   /**
    * Returns the current enable state of hardware flow control.
    *
    * @pre This serial port is open.
    *
    * @return true is returned if hardware flow control is enabled.
    *         false is returned otherwise.
    *
    * @see controlFlow
    */
   bool getHardwareFlowControlState();

   /**
    * Attempts to enable "hardware" flow control.  While some documents
    * recommend the use of hardware flow control over the use of software
    * flow control, not all platforms implement hardware flow control.  Hence,
    * this may have no effect depending on the operating system.
    *
    * @pre This serial port is open.
    * @post If suppported by the operating system, hardware flow control is
    *       enabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    *
    * @see controlFlow
    */
   vpr::ReturnStatus enableHardwareFlowControl();

   /**
    * Attempts to disable "hardware" flow control.  While some documents
    * recommend the use of hardware flow control over the use of software
    * flow control, not all platforms implement hardware flow control.  Hence,
    * this may have no effect depending on the operating system.
    *
    * @pre This serial port is open.
    * @post If suppported by the operating system, hardware flow control is
    *       disabled.
    *
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    *
    * @see controlFlow
    */
   vpr::ReturnStatus disableHardwareFlowControl();

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
    * @return A vpr::ReturnStatus object describing the results of the
    *         operation.
    */
   vpr::ReturnStatus flushQueue(vpr::SerialTypes::FlushQueueOption queue);

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
    * @return A vpr::ReturnStatus object describing the results of the operation.
    */
   vpr::ReturnStatus sendBreak(const Int32 duration);

    /**
   * Return the status of the carrier detect signal.
   * @return - May be platform dependent, but will at least be as follows.
   *           0 - not high, 1 - high, -1 - Not supported
   */
   int getCarrierDetect();

   /**
   * Return the status of the data set ready line.
   * @return - May be platform dependent, but will at least be as follows.
   *           0 - not high, 1 - high, -1 - Not supported
   */
   int getDataSetReady();

   /**
   * Return the status of the clear to send.
   * @return - May be platform dependent, but will at least be as follows.
   *           0 - not high, 1 - high, -1 - Not supported
   */
   int getClearToSend();

   /**
   * Return the status of the ring indicator line.
   * @return - May be platform dependent, but will at least be as follows.
   *           0 - not high, 1 - high, -1 - Not supported
   */
   int getRingIndicator();

   /** Set the data terminal ready line. */
   vpr::ReturnStatus setDataTerminalReady(bool val);

   /** Set the request to send line */
   vpr::ReturnStatus setRequestToSend(bool val);


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
    *                   defaults to vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock if the port is in non-blocking mode,
    *         and there is no data to read.<br>
    *         vpr::ReturnStatus::Timeout is returned if the read could not begin
    *         within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the read operation failed.
    */
   vpr::ReturnStatus read_i(void* buffer, const vpr::Uint32 length,
                            vpr::Uint32& bytes_read,
                            const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      vprASSERT(mHandle->mFdesc != -1 && "The port may not be open");
      return mHandle->read_i(buffer, length, bytes_read, timeout);
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
    *                   defaults to vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock if the port is in non-blocking mode,
    *         and there is no data to read.<br>
    *         vpr::ReturnStatus::Timeout is returned if the read could not begin
    *         within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the read operation failed.
    */
   vpr::ReturnStatus readn_i(void* buffer, const vpr::Uint32 length,
                             vpr::Uint32& bytes_read,
                             const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      vprASSERT(mHandle->mFdesc != -1 && "The port may not be open");
      return mHandle->readn_i(buffer, length, bytes_read, timeout);
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
    *         non-blocking mode, and the write operation could not be
    *         completed.<br>
    *         vpr::ReturnStatus::Timeout is returned if the write could not
    *         begin within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the write operation
    *         failed.
    */
   vpr::ReturnStatus write_i(const void* buffer, const vpr::Uint32 length,
                             vpr::Uint32& bytes_written,
                             const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      vprASSERT(mHandle->mFdesc != -1 && "The port may not be open");
      return mHandle->write_i(buffer, length, bytes_written, timeout);
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
    */
   void setControlCharacter(const vpr::Uint32 index, const vpr::Uint8 value);

   /**
    * Gets the value of the control character at the given index.
    */
   vpr::Uint8 getControlCharacter(const vpr::Uint32 index);

   /**
    * Gets the termios structure for this serial port handle.
    */
   vpr::ReturnStatus getAttrs(struct termios* term);

   /**
    * Tries to set the attributes for this serial port handle using the given
    * termios structure.  If it fails, the given error message will be
    * printed.
    */
   vpr::ReturnStatus setAttrs(struct termios* term, const char* err_msg,
                              const bool print_sys_err = true);

   /**
    * Tries to set the attributes for this serial port handle using the given
    * termios structure.  If it fails, the given error message will be
    * printed.
    */
   vpr::ReturnStatus setAttrs(struct termios* term, const std::string& err_msg,
                              const bool print_sys_err = true);

   /**
    * Gets the value (either on or off) for the named bit.
    */
   bool getBit(const tcflag_t bit, TermFlag flag);

   /**
    * Attempts to enable or disable the named bit.  If it fails, the given
    * error message is printed.
    */
   vpr::ReturnStatus setBit(const tcflag_t bit, TermFlag flag,
                            const bool enable, const std::string& err_msg,
                            const bool print_sys_err = true);

   /** Helper for setting line status flags */
   vpr::ReturnStatus setLineFlag(Uint8 flag, bool val);

   /** Helper for getting lines status flags */
   bool getLineFlag(Uint8 flag);

   /**
    * Converts a termios baud rate to its corresponding integer value.
    */
   vpr::Uint32 baudToInt(const speed_t baud_rate);

   /**
    * Converts an integer baud rate to the corresponding termios rate
    * constant.
    */
   speed_t intToBaud(const vpr::Uint32 speed_int);

   FileHandleImplUNIX* mHandle;    /**< File handle for the serial port */
   int                 mActions;
};

} // End of vpr namespace


#endif  /* _VPR_SERIAL_PORT_IMPL_TERMIOS_H_ */
