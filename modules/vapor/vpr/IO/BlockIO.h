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

#ifndef _VPR_BLOCK_IO_H_
#define _VPR_BLOCK_IO_H_

#include <vpr/vprConfig.h>

#include <cstring>
#include <vector>
#include <string>
#include <cstring>

#include <vpr/Util/Assert.h>
#include <vpr/Util/Interval.h>
#include <vpr/IO/IOSys.h>


namespace vpr
{

class BaseIOStatsStrategy;

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
    *       the caller.
    *
    * @param buffer  A pointer to the buffer where the device's buffer
    *                contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::WouldBlockException if the file is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    */
   vpr::Uint32 read(void* buffer, const vpr::Uint32 length,
                    const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return this->read_s(buffer, length, timeout);
   }

   /**
    * Reads at most the specified number of bytes from the I/O device into
    * the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it from the
    *       device, and the number of bytes read successfully is returned to
    *       the caller.
    *
    * @param buffer  A reference to the buffer (a \p std::string object)
    *                where the device's buffer contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::WouldBlockException if the file is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    */
   vpr::Uint32 read(std::string& buffer, const vpr::Uint32 length,
                    const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      // Allocate the temporary buffer, zero it, and read in the current
      // buffer from the device.
      buffer.resize(length);
      std::memset(&buffer[0], '\0', buffer.size());
      return this->read(&buffer[0], buffer.size(), timeout);
   }

   /**
    * Reads at most the specified number of bytes from the I/O device into
    * the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it
    *       from the device, and the number of bytes read successfully is
    *       returned to the caller.
    *
    * @param buffer  A pointer to the buffer (a vector of vpr::Uint8 vlaues)
    *                where the device's buffer contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::WouldBlockException if the file is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    */
   vpr::Uint32 read(std::vector<vpr::Uint8>& buffer,
                    const vpr::Uint32 length,
                    const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      buffer.resize(length);

      // Allocate the temporary buffer, zero it, and read in the current
      // buffer from the device.
      std::memset(&buffer[0], '\0', buffer.size());

      const vpr::Uint32 bytes = this->read(&buffer[0], buffer.size(), timeout);

      // Size it down if needed, if (bytes == length), then resize() does
      // nothing.
      buffer.resize(bytes);

      return bytes;
   }

   /**
    * Reads exactly the specified number of bytes from the I/O device into
    * the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it from
    *       the device, and the number of bytes read successfully is returned
    *       to the caller.
    *
    * @param buffer  A pointer to the buffer where the device's buffer
    *                contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::EOFException if end of file or end of stream has been
    *         reached unexpectedly during input.
    * @throws vpr::IOException if an error ocured while reading.
    */
   vpr::Uint32 readn(void* buffer, const vpr::Uint32 length,
                     const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return this->readn_s(buffer, length, timeout);
   }

   /**
    * Reads exactly the specified number of bytes from the I/O device into
    * the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it from
    *       the device, and the number of bytes read successfully is returned
    *       to the caller.
    *
    * @param buffer  A reference to the buffer (a \c std::string object)
    *                where the device's buffer contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::EOFException if end of file or end of stream has been
    *         reached unexpectedly during input.
    * @throws vpr::IOException if an error ocured while reading.
    */
   vpr::Uint32 readn(std::string& buffer, const vpr::Uint32 length,
                     const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      // Allocate the temporary buffer, zero it, and read in the current
      // buffer from the device.
      buffer.resize(length);
      std::memset(&buffer[0], '\0', buffer.size());
      const vpr::Uint32 bytes = this->readn(&buffer[0], buffer.size(),
                                            timeout);

      buffer.resize(bytes);

      return bytes;
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
    * @param buffer  A pointer to the buffer (a vector of vpr::Uint8 values)
    *                where the device's buffer contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::EOFException if end of file or end of stream has been
    *         reached unexpectedly during input.
    * @throws vpr::IOException if an error ocured while reading.
    */
   vpr::Uint32 readn(std::vector<vpr::Uint8>& buffer,
                     const vpr::Uint32 length,
                     const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      // Allocate the temporary buffer, zero it, and read in the current
      // buffer from the device.

      buffer.resize(length);
      std::memset(&buffer[0], '\0', buffer.size());
      const vpr::Uint32 bytes = this->readn(&buffer[0], buffer.size(),
                                            timeout);

      buffer.resize(bytes);

      return bytes;
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
    *       bytes written successfully is returned to the caller.
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
    * @throws vpr::IOException if the file handle write operation failed.
    */
   vpr::Uint32 write(const void* buffer, const vpr::Uint32 length,
                     const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return this->write_s(buffer, length, timeout);
   }

   /**
    * Writes the buffer to the I/O device.
    *
    * @pre The device is open for writing.
    * @post The given buffer is written to the I/O device, and the number of
    *       bytes written successfully is returned to the caller.
    *
    * @param buffer  A reference to the buffer (a \c std::string object) to be
    *                written.
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
    * @throws vpr::IOException if the file handle write operation failed.
    */
   vpr::Uint32 write(const std::string& buffer, const vpr::Uint32 length,
                     const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      vprASSERT(length <= buffer.size() && "length was bigger than the data given");
      return this->write(buffer.c_str(), length, timeout);
   }

   /**
    * Writes the buffer to the I/O device.
    *
    * @pre The device is open for writing.
    * @post The given buffer is written to the I/O device, and the number of
    *       bytes written successfully is returned to the caller.
    *
    * @param buffer  A pointer to the buffer (a vector of vpr::Uint8 values) to
    *                be written.
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
    * @throws vpr::IOException if the file handle write operation failed.
    */
   vpr::Uint32 write(const std::vector<vpr::Uint8>& buffer,
                     const vpr::Uint32 length,
                     const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      vprASSERT(length <= buffer.size() && "length was bigger than the data given");
      return this->write(&buffer[0], length, timeout);
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
   void setIOStatStrategy(vpr::BaseIOStatsStrategy* strat);

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
    * @throws vpr::IOException if the read operation failed.
    */
   virtual vpr::Uint32 read_s(void* buffer, const vpr::Uint32 length,
                              const vpr::Interval& timeout);

   /**
    * Read strategy.
    * @throws vpr::IOException if the read operation failed.
    */
   virtual vpr::Uint32 readn_s(void* buffer, const vpr::Uint32 length,
                               const vpr::Interval& timeout);

   /**
    * Write strategy.
    * @throws vpr::IOException if the operation failed.
    */
   virtual vpr::Uint32 write_s(const void* buffer, const vpr::Uint32 length,
                               const vpr::Interval& timeout);

   /**
    * Implementation of the read() template method.  This reads at most the
    * specified number of bytes from the I/O device into the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has length bytes copied into it from the device,
    *       and the number of bytes read successfully is returned to the
    *       caller.
    *
    * @param buffer  A pointer to the buffer where the device's buffer
    *                contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::WouldBlockException if the file is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    */
   virtual vpr::Uint32 read_i(void* buffer, const vpr::Uint32 length,
                              const vpr::Interval& timeout) = 0;

   /**
    * Implementation of the readn() template method.  This reads exactly the
    * specified number of bytes from the I/O device into the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it from the
    *       device, and the number of bytes read successfully is returned to
    *       the caller.
    *
    * @param buffer  A pointer to the buffer where the device's buffer
    *                contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::EOFException if end of file or end of stream has been
    *         reached unexpectedly during input.
    * @throws vpr::IOException if an error ocured while reading.
    */
   virtual vpr::Uint32 readn_i(void* buffer, const vpr::Uint32 length,
                               const vpr::Interval& timeout) = 0;

   /**
    * Implementation of the write() template method.  This writes the given
    * buffer to the I/O device.
    *
    * @pre The device is open for writing.
    * @post The given buffer is written to the I/O device, and the number
    *       of bytes written successfully is returned to the caller.
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
    * @throws vpr::IOException if the file handle write operation failed.
    */
   virtual vpr::Uint32 write_i(const void* buffer, const vpr::Uint32 length,
                               const vpr::Interval& timeout) = 0;

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
