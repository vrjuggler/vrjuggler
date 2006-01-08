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

#ifndef _VPR_BLOCK_IO_H_
#define _VPR_BLOCK_IO_H_

#include <vpr/vprConfig.h>

#include <vector>
#include <string>
#include <string.h>

#include <vpr/Util/Assert.h>
#include <vpr/Util/Interval.h>
#include <vpr/IO/IOException.h>
#include <vpr/IO/IOSys.h>
#include <vpr/IO/Selector.h>

#include <vpr/IO/Stats/BaseIOStatsStrategy.h>


namespace vpr
{

/** \class BlockIO BlockIO.h vpr/IO/BlockIO.h
 *
 * Block-style (as opposed to streaming) I/O interface.
 */
class VPR_CLASS_API BlockIO
{
public:
   /**
    * Gets the name of this I/O device.  The significance of the name depends
    * on the specific device type.
    *
    * @pre None.
    * @post A constant reference to the \c mName object is returned to the
    *       caller.
    *
    * @return An object containing the name of this device.
    */
   virtual const std::string& getName() const
   {
      return mName;
   }

   /**
    * Opens the I/O device.
    *
    * @pre The device is not already open.
    * @post An attempt is made to open the I/O device.  The resulting status
    *       is returned to the caller.  If the I/O device is opened, \c mOpen
    *       is set to true.
    *
    * @throws vpr::WouldBlockException if the file handle is in non-blocking
    *         mode and could not be opened yet.
    * @throws vpr::IOException if the file handle could not be opened.
    */
   virtual void open() = 0;

   /**
    * Closes the I/O device.
    *
    * @pre The device is open.
    * @post An attempt is made to close the I/O device.  The resulting status
    *       is returned to the caller.  If the I/O device is closed,
    *       \c mOpen is set to false.
    *
    * @throws vpr::IOException if the file handle could not be closed.
    */
   virtual void close() = 0;

   /**
    * Gets the open state of this I/O device.
    *
    * @pre None.
    * @post The boolean value in \c mOpen is returned to the caller.
    *
    * @return \c true is returned if the device is open;
    *         \c false otherwise.
    */
   virtual bool isOpen() const
   {
      return mOpen;
   }

   /**
    * Reconfigures the I/O device so that it is in blocking or non-blocking
    * mode depending on the given parameter.  If the device has not been
    * opened yet, it will be opened in blocking or non-blocking mode
    * appropriately when open() is called.
    *
    * @post Processes will block when accessing the device.
    *
    * @param blocking A true value puts the I/O device into blocking mode.
    *                 A false value puts it into non-blocking mode.
    *
    * @throws vpr::IOException if the blocking mode could not be set.
    */
   virtual void setBlocking(bool blocking) = 0;

   /**
    * Gets the current blocking state for the I/O device.
    *
    * @pre \c mBlocking is set correctly.
    *
    * @return true is returned if the device is in blocking mode.
    *         Otherwise, false is returned.
    */
   virtual bool isBlocking() const
   {
      return mBlocking;
   }

   /**
    * Gets the vpr::IOSys::Handle object for this I/O device.
    *
    * @return vpr::IOSys::NullHandle is returned if the device
    *         has no handle or if the handle could not be returned for some
    *         reason.
    */
   virtual IOSys::Handle getHandle() const = 0;

   /**
    * Reads at most the specified number of bytes from the I/O device into
    * the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it from the
    *       device, and the number of bytes read successfully is returned to
    *       the caller via the \p bytesRead argument.
    *
    * @param buffer    A pointer to the buffer where the device's buffer
    *                  contents are to be stored.
    * @param length    The number of bytes to be read.
    * @param bytesRead The number of bytes read into the buffer.
    * @param timeout   The maximum amount of time to wait for data to be
    *                  available for reading.  This argument is optional
    *                  and defaults to vpr::Interval::NoTimeout.
    *
    * @throws vpr::WouldBlockException if the file is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    */
   void read(void* buffer, const vpr::Uint32 length,
             vpr::Uint32& bytesRead,
             const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException)
   {
      this->read_s(buffer, length, bytesRead, timeout);
   }

   /**
    * Reads at most the specified number of bytes from the I/O device into
    * the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it from the
    *       device, and the number of bytes read successfully is returned to
    *       the caller via the \p bytesRead argument.
    *
    * @param buffer    A reference to the buffer (a \p std::string object)
    *                  where the device's buffer contents are to be stored.
    * @param length    The number of bytes to be read.
    * @param bytesRead The number of bytes read into the buffer.
    * @param timeout   The maximum amount of time to wait for data to be
    *                  available for reading.  This argument is optional
    *                  and defaults to vpr::Interval::NoTimeout.
    *
    * @throws vpr::WouldBlockException if the file is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    */
   void read(std::string& buffer, const vpr::Uint32 length,
             vpr::Uint32& bytesRead,
             const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException)
   {
      // Allocate the temporary buffer, zero it, and read in the current
      // buffer from the device.
      buffer.resize(length);
      memset(&buffer[0], '\0', buffer.size());
      this->read(&buffer[0], buffer.size(), bytesRead, timeout);
   }

   /**
    * Reads at most the specified number of bytes from the I/O device into
    * the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it
    *       from the device, and the number of bytes read successfully is
    *       returned to the caller via the \p bytesRead argument.
    *
    * @param buffer    A pointer to the buffer (a vector of vpr::Uint8
    *                  values) where the device's buffer contents are to be
    *                  stored.
    * @param length    The number of bytes to be read.
    * @param bytesRead The number of bytes read into the buffer.
    * @param timeout   The maximum amount of time to wait for data to be
    *                  available for reading.  This argument is optional
    *                  and defaults to vpr::Interval::NoTimeout.
    *
    * @throws vpr::WouldBlockException if the file is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    */
   void read(std::vector<vpr::Uint8>& buffer,
             const vpr::Uint32 length, vpr::Uint32& bytesRead,
             const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException)
   {
      buffer.resize(length);

      // Allocate the temporary buffer, zero it, and read in the current
      // buffer from the device.
      memset(&buffer[0], '\0', buffer.size());
      
      this->read(&buffer[0], buffer.size(), bytesRead, timeout);

      // size it down if needed, if (bytesRead == length), then resize does
      // nothing...
      buffer.resize(bytesRead);
   }

   /**
    * Reads exactly the specified number of bytes from the I/O device into
    * the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it from
    *       the device, and the number of bytes read successfully is returned
    *       to the caller via the \p bytesRead parameter.
    *
    * @param buffer    A pointer to the buffer where the device's buffer
    *                  contents are to be stored.
    * @param length    The number of bytes to be read.
    * @param bytesRead The number of bytes read into the buffer.
    * @param timeout   The maximum amount of time to wait for data to be
    *                  available for reading.  This argument is optional
    *                  and defaults to vpr::Interval::NoTimeout.
    *
    * @throws vpr::EOFException if end of file or end of stream has been
    *         reached unexpectedly during input.
    * @throws vpr::IOException if an error ocured while reading.
    */
   void readn(void* buffer, const vpr::Uint32 length,
              vpr::Uint32& bytesRead,
              const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      this->readn_s(buffer, length, bytesRead, timeout);
   }

   /**
    * Reads exactly the specified number of bytes from the I/O device into
    * the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it from
    *       the device, and the number of bytes read successfully is returned
    *       to the caller via the \p bytesRead parameter.
    *
    * @param buffer    A reference to the buffer (a \c std::string object)
    *                  where the device's buffer contents are to be stored.
    * @param length    The number of bytes to be read.
    * @param bytesRead The number of bytes read into the buffer.
    * @param timeout   The maximum amount of time to wait for data to be
    *                  available for reading.  This argument is optional
    *                  and defaults to vpr::Interval::NoTimeout.
    *
    * @throws vpr::EOFException if end of file or end of stream has been
    *         reached unexpectedly during input.
    * @throws vpr::IOException if an error ocured while reading.
    */
   void readn(std::string& buffer, const vpr::Uint32 length,
              vpr::Uint32& bytesRead,
              const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      // Allocate the temporary buffer, zero it, and read in the current
      // buffer from the device.
      buffer.resize(length);
      memset(&buffer[0], '\0', buffer.size());
      this->readn(&buffer[0], buffer.size(), bytesRead, timeout);

      buffer.resize(bytesRead);
   }

   /**
    * Reads exactly the specified number of bytes from the I/O device into
    * the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it from the
    *       device, and the number of bytes read successfully is returned to
    *       the caller.
    *
    * @param buffer    A pointer to the buffer (a vector of vpr::Uint8
    *                  values) where the device's buffer contents are to be
    *                  stored.
    * @param length    The number of bytes to be read.
    * @param bytesRead The number of bytes read into the buffer.
    * @param timeout   The maximum amount of time to wait for data to be
    *                  available for reading.  This argument is optional
    *                  and defaults to vpr::Interval::NoTimeout.
    *
    * @throws vpr::EOFException if end of file or end of stream has been
    *         reached unexpectedly during input.
    * @throws vpr::IOException if an error ocured while reading.
    */
   void readn(std::vector<vpr::Uint8>& buffer,
              const vpr::Uint32 length, vpr::Uint32& bytesRead,
              const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      // Allocate the temporary buffer, zero it, and read in the current
      // buffer from the device.
      
      buffer.resize(length);
      memset(&buffer[0], '\0', buffer.size());
      this->readn(&buffer[0], buffer.size(), bytesRead, timeout);
      
      buffer.resize(bytesRead);
   }

   /** Return the number of avaiable bytes for reading. */
   virtual vpr::Uint32 availableBytes() const
   {
      return 0;
   }

   /**
    * Write the buffer to the I/O device.
    *
    * @pre The device is open for writing.
    * @post The given buffer is written to the I/O device, and the number of
    *       bytes written successfully is returned to the caller via the
    *       \p bytesWritten parameter.
    *
    * @param buffer       A pointer to the buffer to be written.
    * @param length       The length of the buffer.
    * @param bytesWritten The number of bytes written to the device.
    * @param timeout      The maximum amount of time to wait for data to be
    *                     available for writing.  This argument is optional
    *                     and defaults to vpr::Interval::NoTimeout.
    *
    * @throws vpr::WouldBlockException if the handle is in non-blocking mode,
    *         and the write operation could not be completed.
    * @throws vpr::TimeoutException if the write could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the file handle write operation failed.
    */
   void write(const void* buffer, const vpr::Uint32 length,
              vpr::Uint32& bytesWritten,
              const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      this->write_s(buffer, length, bytesWritten,timeout);
   }

   /**
    * Writes the buffer to the I/O device.
    *
    * @pre The device is open for writing.
    * @post The given buffer is written to the I/O device, and the number of
    *       bytes written successfully is returned to the caller via the
    *       \p bytesWritten parameter.
    *
    * @param buffer       A reference to the buffer (a \c std::string object)
    *                     to be written.
    * @param length       The length of the buffer.
    * @param bytesWritten The number of bytes written to the device.
    * @param timeout      The maximum amount of time to wait for data to be
    *                     available for writing.  This argument is optional
    *                     and defaults to vpr::Interval::NoTimeout.
    *
    * @throws vpr::WouldBlockException if the handle is in non-blocking mode,
    *         and the write operation could not be completed.
    * @throws vpr::TimeoutException if the write could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the file handle write operation failed.
    */
   void write(const std::string& buffer, const vpr::Uint32 length,
              vpr::Uint32& bytesWritten,
              const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      vprASSERT(length <= buffer.size() && "length was bigger than the data given");
      this->write(buffer.c_str(), length, bytesWritten,timeout);
   }

   /**
    * Writes the buffer to the I/O device.
    *
    * @pre The device is open for writing.
    * @post The given buffer is written to the I/O device, and the number of
    *       bytes written successfully is returned to the caller via the
    *       \p bytesWritten parameter.
    *
    * @param buffer       A pointer to the buffer (a vector of vpr::Uint8
    *                     values) to be written.
    * @param length       The length of the buffer.
    * @param bytesWritten The number of bytes written to the device.
    * @param timeout      The maximum amount of time to wait for data to be
    *                     available for writing.  This argument is optional
    *                     and defaults to vpr::Interval::NoTimeout.
    *
    * @throws vpr::WouldBlockException if the handle is in non-blocking mode,
    *         and the write operation could not be completed.
    * @throws vpr::TimeoutException if the write could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the file handle write operation failed.
    */
   void write(const std::vector<vpr::Uint8>& buffer,
              const vpr::Uint32 length,
              vpr::Uint32& bytesWritten,
              const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      vprASSERT(length <= buffer.size() && "length was bigger than the data given");
      this->write(&buffer[0], length, bytesWritten,timeout);
   }

   /**
    * Tests if reading from this I/O device will block.
    *
    * @pre getHandle() returns a valid vpr::IOSys::Handle object.
    * @post
    *
    * @param timeout Give a timeout period to wait for the device to
    *                unblock for reading.  This is optional and defaults to
    *                vpr::Interval::NoWait which will simply poll the device
    *                and return immediately.
    *
    * @return \c true is returned if reading will block;
    *         \c false otherwise.
    */
   bool isReadBlocked(const vpr::Interval& timeout = vpr::Interval::NoWait);

   /**
    * Tests if writing to this I/O device will block.
    *
    * @pre getHandle() returns a valid vpr::IOSys::Handle object.
    * @post
    *
    * @param timeout Give a timeout period to wait for the device to
    *                unblock for writing.  This is optional and defaults to
    *                vpr::Interval::NoWait which will simply poll the device
    *                and return immediately.
    *
    * @return \c true is returned if writing will block;
    *         \c false otherwise.
    */
   bool isWriteBlocked(const vpr::Interval& timeout = vpr::Interval::NoWait);

   /**
    * Sets the IO stats strategy to use.
    */
   void setIOStatStrategy(vpr::BaseIOStatsStrategy* strat)
   {
      mStatsStrategy = strat;
      if(mStatsStrategy != NULL)              // If we have a non-NULL strategy
      {
         mStatsStrategy->setRealObject(this); // Tell it about us
      }
   }

   /**
    * Gets the current IO stats strategy.
    */
   vpr::BaseIOStatsStrategy* getIOStatStrategy() const
   {
      return mStatsStrategy;
   }

protected:
   /**
    * Default constructor.  This is provided for subclasses where a "named"
    * device does not make sense.
    *
    * @pre None.
    * @post The open mode is set to blocking; the open state is set to false;
    *       and the blocking mode for reads and writes is set to true.
    */
   BlockIO();

   /**
    * Constructor.
    *
    * @pre None.
    * @post The name object is copied into \c mName; the open mode is set to
    *       blocking; the open state is set to \c false; and the blocking mode
    *       for reads and writes is set to \c true.
    *
    * @param name The name for this device.
    */
   BlockIO(const std::string& name);

   /**
    * Copy constructor.
    *
    * @param other A constant reference to another vpr::BlockIO object used
    *              as the source for the copy.
    */
   BlockIO(const BlockIO& other);

   /**
    * Destructor.
    *
    * @pre None.
    * @post None.
    */
   virtual ~BlockIO();

   /**
    * Read strategy.
    */
   virtual void read_s(void* buffer, const vpr::Uint32 length,
                       vpr::Uint32& bytesRead,
                       const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException);

   /**
    * Read strategy.
    */
   virtual void readn_s(void* buffer, const vpr::Uint32 length,
                        vpr::Uint32& bytesRead,
                        const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException);

   /**
    * Write strategy.
    */
   virtual void write_s(const void* buffer,
                        const vpr::Uint32 length,
                        vpr::Uint32& bytesWritten,
                        const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException);

   /**
    * Implementation of the read() template method.  This reads at most the
    * specified number of bytes from the I/O device into the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has length bytes copied into it from the device,
    *       and the number of bytes read successfully is returned to the
    *       caller via the \p bytesRead parameter.
    *
    * @param buffer    A pointer to the buffer where the device's buffer
    *                  contents are to be stored.
    * @param length    The number of bytes to be read.
    * @param bytesRead The number of bytes read into the buffer.
    * @param timeout   The maximum amount of time to wait for data to be
    *                  available for reading.  This argument is optional and
    *                  defaults to vpr::Interval::NoTimeout.
    *
    * @throws vpr::WouldBlockException if the file is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    */
   virtual void read_i(void* buffer, const vpr::Uint32 length,
                       vpr::Uint32& bytesRead,
                       const vpr::Interval timeout = vpr::Interval::NoTimeout) = 0;

   /**
    * Implementation of the readn() template method.  This reads exactly the
    * specified number of bytes from the I/O device into the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it from the
    *       device, and the number of bytes read successfully is returned to
    *       the caller via the \p bytesRead parameter.
    *
    * @param buffer    A pointer to the buffer where the device's buffer
    *                  contents are to be stored.
    * @param length    The number of bytes to be read.
    * @param bytesRead The number of bytes read into the buffer.
    * @param timeout   The maximum amount of time to wait for data to be
    *                  available for reading.  This argument is optional and
    *                  defaults to vpr::Interval::NoTimeout.
    *
    * @throws vpr::EOFException if end of file or end of stream has been
    *         reached unexpectedly during input.
    * @throws vpr::IOException if an error ocured while reading.
    */
   virtual void readn_i(void* buffer, const vpr::Uint32 length,
                        vpr::Uint32& bytesRead,
                        const vpr::Interval timeout = vpr::Interval::NoTimeout) = 0;

   /**
    * Implementation of the write() template method.  This writes the given
    * buffer to the I/O device.
    *
    * @pre The device is open for writing.
    * @post The given buffer is written to the I/O device, and the number
    *       of bytes written successfully is returned to the caller via the
    *       \p bytesWritten parameter.
    *
    * @param buffer       A pointer to the buffer to be written.
    * @param length       The length of the buffer.
    * @param bytesWritten The number of bytes written to the device.
    * @param timeout      The maximum amount of time to wait for data to be
    *                     available for writing.  This argument is optional
    *                     and defaults to vpr::Interval::NoTimeout.
    *
    * @throws vpr::WouldBlockException if the handle is in non-blocking mode,
    *         and the write operation could not be completed.
    * @throws vpr::TimeoutException if the write could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the file handle write operation failed.
    */
   virtual void write_i(const void* buffer,
                        const vpr::Uint32 length,
                        vpr::Uint32& bytesWritten,
                        const vpr::Interval timeout = vpr::Interval::NoTimeout) = 0;

   // Friends
   friend class vpr::BaseIOStatsStrategy;    // Need it to be able to call the protected read_i, readn_i, and write_i memebers

protected:
   /// The name of the I/O device.
   std::string mName;

   /// Flag telling if the device is open.
   bool mOpen;

   /// Flag telling if blocking for reads and writes is enabled.
   bool mBlocking;

   /// Perf monitor.
   vpr::BaseIOStatsStrategy* mStatsStrategy;
};

} // End of vpr namespace


#endif   /* _VPR_BLOCK_IO_H_ */
