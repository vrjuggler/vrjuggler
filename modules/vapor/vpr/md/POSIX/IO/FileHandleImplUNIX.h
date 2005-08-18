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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _VPR_FILE_HANDLE_IMPL_UNIX_H_
#define _VPR_FILE_HANDLE_IMPL_UNIX_H_

#include <vpr/vprConfig.h>

#include <string>
#include <vector>

#include <vpr/IO/EOFException.h>
#include <vpr/IO/IOException.h>
#include <vpr/IO/IOSys.h>
#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/WouldBlockException.h>
#include <vpr/Util/Interval.h>

namespace vpr
{

/** \class FileHandleImplUNIX FileHandleImplUNIX.h vpr/IO/FileHandle.h
 *
 * Wrapper around UNIX file descriptors.  This is used with
 * vpr::FileHandle_t<T> to create the typedef vpr::FileHandle.
 */
class FileHandleImplUNIX
{
public:
   /**
    * Constructor for unnamed file-based devices.  This initializes the member
    * variables to reasonable defaults and stores the given file name for
    * later use.
    *
    * @post All member variables are initialized except mName.
    */
   FileHandleImplUNIX();

   /**
    * Constructor.  This initializes the member variables to reasonable
    * defaults and stores the given file name for later use.
    *
    * @post All member variables are initialized including mName that is
    *       assigned the string in file_name.
    *
    * @param file_name The name of the file to be handled.
    */
   FileHandleImplUNIX(const std::string& file_name);

   /**
    * Destructor.  If the file handle is in an open state, it is closed.
    *
    * @post If the file handle is still open, it is closed.
    */
   ~FileHandleImplUNIX();

   // ========================================================================
   // vpr::BlockIO basics.
   // ========================================================================

   /**
    * Gets the name of this file.
    *
    * @post
    *
    * @return An object containing the name of this file.
    */
   const std::string& getName() const
   {
      return mName;
   }

   /**
    * Opens the file handle.
    *
    * @pre The file handle is not already open.
    * @post An attempt is made to open the file.  The resulting status is
    *       returned to the caller.  If the file is opened, mOpen is set to
    *       true.
    *
    * @throws vpr::WouldBlockException if the file handle is in non-blocking
    *         mode and could not be opened yet.
    * @throws vpr::IOException if the file handle could not be opened.
    */
   void open() throw (IOException);

   /**
    * Closes the file handle.
    *
    * @pre The file handle is open.
    * @post An attempt is made to close the file.  The resulting status is
    *       returned to the caller.  If the file is closed, mOpen is set to
    *       false.
    *
    * @throws vpr::IOException if the file handle could not be closed.
    */
   void close() throw (IOException);

   /**
    * Gets the open state of this file handle.
    *
    * @pre None.
    * @post The boolean value giving the open state is returned to the
    *       caller.
    *
    * @return true is returned if this file handle is open; false otherwise.
    */
   bool isOpen() const
   {
      return mOpen;
   }

   /**
    * Reconfigures the file handle so that it is in blocking or non-blocking
    * mode.  If this file handle has not been opened yet, it will be opened in
    * blocking or non-blocking mode appropriately when open() is called.
    *
    * @post Processes may block (or not) when accessing the file.
    *
    * @param blocking A value of true indicates that the file handle will use
    *                 blocking I/O.  A value of false indicates that it will
    *                 use non-blocking I/O.
    *
    * @throws vpr::IOException if the blocking mode could not be set.
    */
   void setBlocking(bool blocking) throw (IOException);

   /**
    * Returns the contained handle.
    */
   vpr::IOSys::Handle getHandle() const;

   /**
    * Gets the current blocking state for the file handle.
    *
    * @pre mBlocking is set correctly.
    *
    * @return true is returned if the file handle is in blocking mode.
    *         Otherwise, false is returned.
    */
   bool isBlocking() const
   {
      return mBlocking;
   }

   // ========================================================================
   // vpr::FileHandle implementation.
   // ========================================================================

   /**
    * Sets the open flags so that the I/O device is opened in read-only mode.
    *
    * @pre None.
    * @post The open flags are updated so that when the device is opened, it
    *       it is opened in read-only mode.  If the device is already open,
    *       this has no effect.
    */
   void setOpenReadOnly();

   /**
    * Sets the open flags so that the I/O device is opened in write-only mode.
    *
    * @pre None.
    * @post The open flags are updated so that when the device is opened, it
    *       is opened in write-only mode.  If the device is already open,
    *       this has no effect.
    */
   void setOpenWriteOnly();

   /**
    * Sets the open flags so that the I/O device is opened in read/write mode.
    *
    * @pre None.
    * @post The open flags are updated so that when the device is opened, it
    *       is opened in read/write mode.  If the device is already open,
    *       this has no effect.
    */
   void setOpenReadWrite();

   /**
    * Reconfigures the file handle to be in append mode or not.
    *
    * @pre The file handle is open.
    * @post The file handle's write mode is set to append or not.
    *
    * @param flag A value of true indicates that this file handle will use
    *             append mode for writing.  A value of false indicates that
    *             it will not.
    *
    * @throws vpr::IOException if the write mode could not be changed
    *         for some reason.
    */
   void setAppend(bool flag) throw (IOException);

   /**
    * Reconfigures the file handle so that writes are synchronous or
    * asynchronous depending on the value of the parameter.
    *
    * @pre The file handle is open.
    * @post Writes are performed synchronously.
    *
    * @param flag A value of true indicates that synchronous writes will be
    *             used.  A value of false indicates that asynchronous writes
    *             will be used.
    *
    * @throws vpr::IOException if the write mode could not be changed
    *         for some reason.
    */
   void setSynchronousWrite(bool flag) throw (IOException);

   /**
    * Tests if the I/O device is read-only.
    *
    * @pre The I/O device is open.
    * @post The access mode is tested for read-only mode, and the result is
    *       returned to the caller.
    *
    * @return true is returned if the device is in read-only mode; false
    *         otherwise.
    */
   bool isReadOnly() const;

   /**
    * Tests if the I/O device is write-only.
    *
    * @pre The I/O device is open.
    * @post The access mode is tested for write-only mode, and the result is
    *       returned to the caller.
    *
    * @return true is returned if the device is in write-only mode; false
    *         otherwise.
    */
   bool isWriteOnly() const;

   /**
    * Tests if the I/O device is read/write.
    *
    * @pre The I/O device is open.
    * @post The access mode is tested for read/write mode, and the result is
    *       returned to the caller.
    *
    * @return true is returned if the device is in read/write mode; false
    *         otherwise.
    */
   bool isReadWrite() const;

   /**
    * Queries the amount of data currently in the read buffer.
    *
    * @pre The file descriptor is valid.
    * @post The buffer size is returned via the by-reference parameter.
    * @throws vpr::IOException if read buffer size could not be found.
    */
   void getReadBufferSize(vpr::Int32& buffer) const throw (IOException);

   /**
    * Implementation of the read template method.  This reads at most the
    * specified number of bytes from the file into the given buffer.
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
    * @throws vpr::WouldBlockException if the file is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    */
   void read_i(void* buffer, const vpr::Uint32 length, vpr::Uint32& bytesRead,
               const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException);

   /**
    * Implementation of the readn template method.  This reads exactly the
    * specified number of bytes from the file into the given buffer.
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
   void readn_i(void* buffer, const vpr::Uint32 length,
                vpr::Uint32& bytesRead,
                const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException);

   /**
    * Implementation of the write template method.  This writes the buffer to
    * the file.
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
    * @throws vpr::IOException if the write operation failed.
    */
   void write_i(const void* buffer, const vpr::Uint32 length,
                vpr::Uint32& bytesWritten,
                const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException);

   /**
    * Returns the number of bytes available for reading in the receive
    * buffer.
    *
    * @return A value greater than 0 is returned if there are bytes to be
    *         read.  If there is nothing to read or an error occurred, 0 is
    *         returned.
    */
   vpr::Uint32 availableBytes() const;

protected:
   // Friends.
   friend class SerialPortImplTermios;
   friend class SocketDatagramImplBSD;
   friend class SocketImplBSD;
   friend class SocketStreamImplBSD;

   /**
    * Gets the current file handle flags.
    *
    * @pre The file handle is open.
    * @post The current flags for the handle are returned to the caller.
    *
    * @return A value larger than -1 is returned giving the current flags for
    *         the file handle.
    * @return -1 is returned if the current flags could not be requested.
    */
   int getFlags() const;

   /**
    * Overwrites the current file handle flags with the given value.
    *
    * @pre The file handle is open.
    * @post The flags for the file handle are overwritten with the new
    *       value.
    *
    * @return 0 is returned if the flags were updated successfully.
    * @return -1 is returned if the current flags could not be overwritten.
    */
   int setFlags(const int flags);

   /**
    * Tests if the file handle is ready for reading within the timeout
    * period.
    *
    * @throws IOException if error occured while querying read state.
    */
   bool isReadable(const vpr::Interval timeout) const throw (IOException);

   /**
    * Tests if the file handle is ready for writing within the timeout
    * period.
    * 
    * @throws IOException if error occured while querying write state.
    */
   bool isWriteable(const vpr::Interval timeout) const throw (IOException);

   std::string mName;           /**< The name of this file */
   bool        mOpen;           /**< Open state of this file */
   bool        mOpenBlocking;
   bool        mBlocking;       /**< Blocking state of this file */

   int mFdesc;      /**< File descriptor */
   int mOpenMode;   /**< The open mode of the device */
};

} // End of vpr namespace


#endif  /* _VPR_FILE_HANDLE_IMPL_UNIX_H_ */
