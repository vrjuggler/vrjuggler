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

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Enable BSD compatibility in sys/ioctl.h on Solaris.
#ifdef VPR_OS_Solaris
#  define BSD_COMP
#endif

#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>

#ifdef HAVE_SYS_FILE_H
#  include <sys/file.h>
#endif

#include <vpr/Util/Debug.h>
#include <vpr/md/POSIX/IO/FileHandleImplUNIX.h>
#include <vpr/IO/EOFException.h>
#include <vpr/IO/IOException.h>
#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/WouldBlockException.h>


namespace vpr
{

// ============================================================================
// Public methods.
// ============================================================================

// Constructor for unnamed file-based devices.  This initializes the member
// variables to reasonable defaults and stores the given file name for
// later use.
FileHandleImplUNIX::FileHandleImplUNIX()
   : mOpen(false)
   , mOpenBlocking(true)
   , mBlocking(true)
   , mFdesc(-1)
   , mOpenMode(O_RDWR)
{
   /* Do nothing. */ ;
}

// Constructor.  This initializes the member variables to reasonable defaults
// and stores the given file name for later use.
FileHandleImplUNIX::FileHandleImplUNIX(const std::string& file_name)
   : mName(file_name)
   , mOpen(false)
   , mOpenBlocking(true)
   , mBlocking(true)
   , mFdesc(-1)
   , mOpenMode(O_RDWR)
{
   /* Do nothing. */ ;
}

// Destructor.  If the file handle is in an open state, it is closed.
FileHandleImplUNIX::~FileHandleImplUNIX()
{
   if ( mOpen )
   {
      close();
   }
}

// Open the file handle.
void FileHandleImplUNIX::open()
{
   int open_flags(mOpenMode);

   if ( ! mOpenBlocking )
   {
      open_flags |= O_NONBLOCK;
   }

   mFdesc = ::open(mName.c_str(), open_flags);

   // If the file handle was not returned successfully, print an error
   // message explaining why.
   if ( mFdesc == -1 )
   {
      // If we are opening in non-blocking mode, we do not want to bomb out.
      if ( errno == EWOULDBLOCK && ! mOpenBlocking )
      {
         mOpen = true;
         throw WouldBlockException("Would block.", VPR_LOCATION);
      }
      // Otherwise, report the error.
      else
      {
         mOpen = false;
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << "[vpr::FileHandleImplUNIX::open()] Could not open " << mName
            << ": " << strerror(errno) << std::endl << vprDEBUG_FLUSH;

         throw IOException("[vpr::FileHandleImplUNIX::open()] Could not open "
            + mName + ": " + std::string(strerror(errno)), VPR_LOCATION);
      }
   }
   // Otherwise, set mOpen to true.
   else
   {
      mOpen     = true;
      mBlocking = mOpenBlocking;
   }
}

// Close the file handle.
void FileHandleImplUNIX::close()
{
   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
      << "[vpr::FileHandleImplUNIX::close()] Closing file descriptor "
      << mFdesc << std::endl << vprDEBUG_FLUSH;

   if ( ::close(mFdesc) == -1 )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "[vpr::FileHandleImplUNIX::close()] Could not close " << mName
         << ": " << strerror(errno) << std::endl << vprDEBUG_FLUSH;

      throw IOException("[vpr::FileHandleImplUNIX::close()] Could not close "
         + mName + ": " + std::string(strerror(errno)), VPR_LOCATION);
   }
   else
   {
      mFdesc = -1;
      mOpen  = false;
   }
}

// Reconfigure the file handle so that it is in blocking mode.
void FileHandleImplUNIX::setBlocking(bool blocking)
{
   if ( ! mOpen )
   {
      mOpenBlocking = blocking;
   }
   else
   {
      int cur_flags, new_flags;

      // Get the current flags.
      cur_flags = getFlags();

      if ( blocking )
      {
#ifdef _SGI_SOURCE
         // On IRIX, mask FNONBLK and FNDELAY.  We mask FNDELAY to ensure that
         // it is not set by the operating system at some level.
         new_flags = cur_flags & ~(FNONBLK | FNDELAY);
#else
         // On everything else, mask O_NONBLOCK and O_NDELAY.  We mask O_NDELAY
         // to ensure that it is not set by the operating system at some level.
         new_flags = cur_flags & ~(O_NONBLOCK | O_NDELAY);
#endif
      }
      else
      {
#ifdef _SVR4_SOURCE
         // On SysV, set FNONBLK.  We do not set FNDELAY because it just adds
         // confusion.  FNONBLK is preferred.
         new_flags = cur_flags | FNONBLK;
#else
         // On everything else, set O_NONBLOCK.  We do not set O_NDELAY because
         // it just adds confusion.  O_NONBLOCK is preferred.
         new_flags = cur_flags | O_NONBLOCK;
#endif
      }

      // Set the file descriptor to be blocking with the new flags.
      if ( setFlags(new_flags) == -1 )
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << "[vpr::FileHandleImplUNIX::setBlocking()] Failed to set "
            << (blocking ? "blocking" : "non-blocking") << " state on "
            << mName << ": " << strerror(errno) << std::endl << vprDEBUG_FLUSH;

         throw IOException("[vpr::FileHandleImplUNIX::setBlocking()] Failed to set "
            + std::string(blocking ? "blocking" : "non-blocking") + " state on "
            + mName + ": " + std::string(strerror(errno)), VPR_LOCATION);
      }
      else
      {
         mBlocking = blocking;
      }
   }
}

vpr::IOSys::Handle FileHandleImplUNIX::getHandle() const
{
#ifdef VPR_USE_NSPR
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
       << "ERROR: Cannot get handle for UNIX file descriptor with NSPR!\n";
   return vpr::IOSys::NullHandle;
#else
   return mFdesc;
#endif
}

void FileHandleImplUNIX::setOpenReadOnly()
{
   mOpenMode = O_RDONLY;
}

void FileHandleImplUNIX::setOpenWriteOnly()
{
   mOpenMode = O_WRONLY;
}

void FileHandleImplUNIX::setOpenReadWrite()
{
   mOpenMode = O_RDWR;
}

// Reconfigure the file handle to be in append mode.
void FileHandleImplUNIX::setAppend(bool append)
{
   int cur_flags, new_flags, retval;

   // Get the current flags.
   cur_flags = getFlags();

   // Set O_APPEND.
   if ( append )
   {
      new_flags = cur_flags | O_APPEND;
   }
   // Clear O_APPEND.
   else
   {
      new_flags = cur_flags & ~O_APPEND;
   }

   // Set the file descriptor to be blocking with the new flags.
   retval = setFlags(new_flags);

   if ( retval == -1 )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << "[vpr::FileHandleImplUNIX::setAppend()] Failed to "
         << (append ? "enable" : "disable") << " append mode on "
         << mName << ": " << strerror(errno) << std::endl << vprDEBUG_FLUSH;

      throw IOException("[vpr::FileHandleImplUNIX::setAppend()] Failed to "
         + std::string(append ? "enable" : "disable") + " append mode on "
         + mName + ": " + std::string(strerror(errno)), VPR_LOCATION);
   }
}

// Reconfigure the file handle so that writes are synchronous.
void FileHandleImplUNIX::setSynchronousWrite(bool sync)
{
#if ! defined(_POSIX_SOURCE) && defined(O_SYNC) && defined(O_ASYNC)
   int cur_flags, new_flags, retval;

   // Get the current flags.
   cur_flags = getFlags();

   // Synchronous writes.
   if ( sync )
   {
      new_flags = cur_flags | O_SYNC;
   }
   // Asynchronous writes.
   else
   {
      new_flags = cur_flags | O_ASYNC;
   }

   // Set the file descriptor to be blocking with the new flags.
   retval = setFlags(new_flags);

   if ( retval == -1 )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << "[vpr::FileHandleImplUNIX::setSynchronousWrite()] Failed to enable "
         << (sync ? "synchronous" : "asynchronous") << " writes on "
         << mName << ": " << strerror(errno) << std::endl << vprDEBUG_FLUSH;

      std::ostringstream msg_stream;
      msg_stream << "Failed to enable "
                 << (sync ? "synchronous" : "asynchronous") << " writes on "
                 << mName << ": " << strerror(errno);
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }
#else
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << "[vpr::FileHandleImplUNIX::setSynchronousWrite()] Cannot enable "
      << (sync ? "synchronous" : "asynchronous")
      << " writes on this platform!\n" << vprDEBUG_FLUSH;

   std::ostringstream msg_stream;
   msg_stream << "Cannot enable " << (sync ? "synchronous" : "asynchronous")
              << " writes on this platform!";
   throw IOException(msg_stream.str(), VPR_LOCATION);
#endif
}

bool FileHandleImplUNIX::isReadOnly() const
{
   return (mOpenMode == O_RDONLY);
}

bool FileHandleImplUNIX::isWriteOnly() const
{
   return (mOpenMode == O_WRONLY);
}

bool FileHandleImplUNIX::isReadWrite() const
{
   return (mOpenMode == O_RDWR);
}

vpr::Int32 FileHandleImplUNIX::getReadBufferSize() const
{
   vpr::Int32 buffer(0);

   if ( ioctl(mFdesc, FIONREAD, &buffer) == -1 )
   {
      throw IOException("vpr::FileHandleImplUNIX::getReadBufferSize() Cannot get buffer size.",
         VPR_LOCATION);
   }

   return buffer;
}

// ============================================================================
// Protected methods.
// ============================================================================

// Read the specified number of bytes from the file handle into the given
// bufer.
vpr::Uint32 FileHandleImplUNIX::read_i(void* buffer, const vpr::Uint32 length,
                                       const vpr::Interval& timeout)
{
   vpr::Uint32 bytes_read(0);

   // If not readable within timeout interval throw exception.
   if ( ! isReadable(timeout) )
   {
      std::ostringstream msg_stream;
      msg_stream << "Timeout occured while trying to read from " << mName;
      throw TimeoutException(msg_stream.str(), VPR_LOCATION);
   }

   const ssize_t bytes = ::read(mFdesc, buffer, length);

   // Error: Something went wrong while attempting to read from the file.
   if ( bytes < 0 )
   {
      if ( errno == EAGAIN && ! mBlocking )
      {
         throw WouldBlockException("Would block while reading.", VPR_LOCATION);
      }
      else  // "real" error, so throw IO Exception
      {
         std::ostringstream msg_stream;
         msg_stream << "Error reading from " << mName << ":"
                    << strerror(errno);
         throw IOException(msg_stream.str(), VPR_LOCATION);
      }
   }
   // If 0 bytes were read and an error was returned, we throw.
   // Note: If bytes == 0 and no error (and socket) then that means the other
   //       side shut down cleanly.
   else if ( 0 == bytes && 0 != errno )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_WARNING_LVL)
         << "[vpr::FileHandleImplUNIX::read_i()] Nothing read from "
         << mName << ": " << strerror(errno) << std::endl << vprDEBUG_FLUSH;

      // XXX: Failure status may not be exactly what we want to return.
      std::ostringstream msg_stream;
      msg_stream << "Nothing read from " << mName << ": " << strerror(errno);
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }
   else
   {
      bytes_read = bytes;
   }

   return bytes_read;
}

// Read exactly the specified number of bytes from the file handle into the
// given buffer.  This is based on the readn() function given on pages 51-2 of
// _Effective TCP/IP Programming_ by Jon D. Snader.
vpr::Uint32 FileHandleImplUNIX::readn_i(void* buffer,
                                        const vpr::Uint32 buffer_size,
                                        const vpr::Interval& timeout)
{
   if ( vpr::Interval::NoTimeout != timeout )
   {
      vprDEBUG(vprDBG_ALL,vprDBG_WARNING_LVL) << "Timeout not supported\n"
                                              << vprDEBUG_FLUSH;
      //TODO: InvalidArgumentException instead, but this will require
      //      adding it as an acceptable exception to throw.
      throw IOException("Timeout not supported by readn.", VPR_LOCATION);
   }

   vpr::Uint32 bytes_read(0);
   size_t bytes_left = buffer_size;

   while ( bytes_left > 0 )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
         << "[vpr::FileHandleImplUNIX::readn_i()] Reading " << bytes_left
         << " bytes from file handle " << mFdesc << std::endl
         << vprDEBUG_FLUSH;

      const ssize_t bytes = ::read(mFdesc, buffer, bytes_left);

      vprDEBUG_NEXT(vprDBG_ALL, vprDBG_HVERB_LVL)
         << "Read " << bytes << " bytes from file handle " << mFdesc
         << std::endl << vprDEBUG_FLUSH;

      // Read error.
      if ( bytes < 0 )
      {
         // Restart the read process if we were interrupted by the OS.
         if ( EINTR == errno )
         {
            continue;
         }
         // Restart the read process if socket is non-blocking and no
         // data was immediately available.
         else if ( EAGAIN == errno )
         {
            continue;
         }
         // Otherwise, we have an error situation, so return failure status.
         else
         {
            std::ostringstream msg_stream;
            msg_stream << "Error reading from " << mName << ": "
                       << strerror(errno);
            throw IOException(msg_stream.str(), VPR_LOCATION);
         }
      }
      // We have read EOF, so there is nothing more to read.  At this point,
      // bytes_read contains an accurate count of the bytes read so far
      // (posisbly less than buffer_size).
      else if ( bytes == 0 )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
            << "[vpr::FileHandleImplUNIX::readn_i()] Read EOF with "
            << bytes_left << " bytes left to read from file handle "
            << mFdesc << " and " << bytes_read << " bytes read in total."
            << std::endl << vprDEBUG_FLUSH;

         std::ostringstream ss;
         ss << "Read EOF with " << bytes_left
            << " bytes left to read from file handle " << mFdesc << " and "
            << bytes_read << " bytes read in total.";
         throw EOFException(ss.str(), VPR_LOCATION);
      }
      else
      {
         buffer = (void*) ((char*) buffer + bytes);
         bytes_left -= bytes;
         bytes_read += bytes;
      }
   }

   return bytes_read;
}

// Write the buffer to the file handle.
vpr::Uint32 FileHandleImplUNIX::write_i(const void* buffer,
                                        const vpr::Uint32 length,
                                        const vpr::Interval& timeout)
{
   vpr::Uint32 bytes_written(0);

   // If not writable within timeout interval throw exception.
   if ( ! isWriteable(timeout) )
   {
      std::ostringstream msg_stream;
      msg_stream << "Timeout occured while trying to write to: " << mName;
      throw TimeoutException(msg_stream.str(), VPR_LOCATION);
   }

   const ssize_t bytes = ::write(mFdesc, buffer, length);

   if ( bytes <= 0 )
   {
      if ( errno == EAGAIN && ! mBlocking )
      {
         throw WouldBlockException("Would block writing.", VPR_LOCATION);
      }
      else
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << "[vpr::FileHandleImplUNIX::write_i()] Error writing to "
            << mName << ": " << strerror(errno) << std::endl
            << vprDEBUG_FLUSH;

         std::ostringstream msg_stream;
         msg_stream << "Error writing to " << mName << strerror(errno);
         throw IOException(msg_stream.str(), VPR_LOCATION);
      }
   }
   else
   {
      bytes_written = bytes;
   }

   return bytes_written;
}

vpr::Uint32 FileHandleImplUNIX::availableBytes() const
{
   int result;

   if ( ioctl(mFdesc, FIONREAD, &result) < 0 )
   {
      result = 0;
   }

   return result;
}

// Get the current file handle flags.
int FileHandleImplUNIX::getFlags() const
{
   return fcntl(mFdesc, F_GETFL, 0);
}

// Overwrite the current file handle flags with the given value.
int FileHandleImplUNIX::setFlags(const int flags)
{
   return fcntl(mFdesc, F_SETFL, flags);
}

bool FileHandleImplUNIX::isReadable(const vpr::Interval timeout) const
{
   fd_set read_set;
   int num_events;
   struct timeval timeout_obj;

   if ( mFdesc == -1 )
   {
      throw IOException("Invalid file handle.", VPR_LOCATION);
   }
   else
   {
      if ( timeout == vpr::Interval::NoWait )
      {
         timeout_obj.tv_sec  = 0;
         timeout_obj.tv_usec = 0;
      }
      else
      {
         if ( timeout.msec() >= 1000 )
         {
            timeout_obj.tv_sec  = timeout.msec() / 1000;
            timeout_obj.tv_usec = (timeout.msec() % 1000) * 1000000;
         }
         else
         {
            timeout_obj.tv_sec  = 0;
            timeout_obj.tv_usec = timeout.msec() * 1000;
         }
      }

      FD_ZERO(&read_set);
      FD_SET(mFdesc, &read_set);

      num_events = select(mFdesc + 1, &read_set, NULL, NULL,
                          (timeout != vpr::Interval::NoTimeout) ? &timeout_obj :
                                                                  NULL);

      if ( num_events == 0 )
      {
         // Added in revision 1.33
         // If timeout is vpr::Interval::NoWait(NULL) the file handle may be ready for
         // reading even though select returned 0. This is vauge in the documentation
         // because it says that calling select with a timeout of NULL will return
         // immediately.
         if ( ! FD_ISSET(mFdesc, &read_set) )
         {
            return false;
         }
      }
      else if ( num_events < 0 )
      {
         // TODO: Test additional select errors.
         // EBADF  An invalid file descriptor was given in one of the sets.
         // EINTR  A non blocked signal was caught.
         // EINVAL n is negative or the value contained within timeout is invalid.
         // ENOMEM select was unable to allocate memory for internal tables.
         throw IOException("Error while testing if file handle is readable.", VPR_LOCATION);
      }
   }
   return true;
}

bool FileHandleImplUNIX::isWriteable(const vpr::Interval timeout) const
{
   fd_set write_set;
   int num_events;
   struct timeval timeout_obj;

   if ( mFdesc == -1 )
   {
      throw IOException("Invalid file handle.", VPR_LOCATION);
   }
   else
   {
      if ( timeout == vpr::Interval::NoWait )
      {
         timeout_obj.tv_sec  = 0;
         timeout_obj.tv_usec = 0;
      }
      else
      {
         if ( timeout.msec() >= 1000 )
         {
            timeout_obj.tv_sec  = timeout.msec() / 1000;
            timeout_obj.tv_usec = (timeout.msec() % 1000) * 1000000;
         }
         else
         {
            timeout_obj.tv_sec  = 0;
            timeout_obj.tv_usec = timeout.msec() * 1000;
         }
      }

      FD_ZERO(&write_set);
      FD_SET(mFdesc, &write_set);

      // Watch to see if a write will block. Timeout is an upper bound on the amount of time
      // elapsed before select returns. It may be zero, causing select to return
      // immediately. (This is useful for polling.) If timeout is NULL (no timeout),
      // select can block indefinitely.
      num_events = select(mFdesc + 1, NULL, &write_set, NULL,
                          (timeout != vpr::Interval::NoTimeout) ? &timeout_obj :
                                                                  NULL);

      if ( num_events == 0 )
      {
         // Added in revision 1.33
         // If timeout is vpr::Interval::NoWait(NULL) the file handle may be ready for
         // writing even though select returned 0. This is vauge in the documentation
         // because it says that calling select with a timeout of NULL will return
         // immediately.
         if ( ! FD_ISSET(mFdesc, &write_set) )
         {
            return false;
         }
      }
      else if ( num_events < 0 )
      {
         // TODO: Test additional select errors.
         // EBADF  An invalid file descriptor was given in one of the sets.
         // EINTR  A non blocked signal was caught.
         // EINVAL n is negative or the value contained within timeout is invalid.
         // ENOMEM select was unable to allocate memory for internal tables.
         throw IOException("Error while testing if file handle is writeable.", VPR_LOCATION);
      }
   }
   return true;
}

} // End of vpr namespace
