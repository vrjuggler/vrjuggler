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

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <string.h>
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
vpr::ReturnStatus FileHandleImplUNIX::open()
{
   vpr::ReturnStatus status;

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
         status.setCode(vpr::ReturnStatus::WouldBlock);
         mOpen = true;
      }
      // Otherwise, report the error.
      else
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << "[vpr::FileHandleImplUNIX::open()] Could not open " << mName
            << ": " << strerror(errno) << std::endl << vprDEBUG_FLUSH;
         status.setCode(ReturnStatus::Fail);
         mOpen = false;
      }
   }
   // Otherwise, set mOpen to true.
   else
   {
      mOpen     = true;
      mBlocking = mOpenBlocking;
   }

   return status;
}

// Close the file handle.
vpr::ReturnStatus FileHandleImplUNIX::close()
{
   vpr::ReturnStatus status;

   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
      << "[vpr::FileHandleImplUNIX::close()] Closing file descriptor "
      << mFdesc << std::endl << vprDEBUG_FLUSH;

   if ( ::close(mFdesc) == -1 )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "[vpr::FileHandleImplUNIX::close()] Could not close " << mName
         << ": " << strerror(errno) << std::endl << vprDEBUG_FLUSH;
      status.setCode(ReturnStatus::Fail);
   }
   else
   {
      mFdesc = -1;
      mOpen  = false;
   }

   return status;
}

// Reconfigure the file handle so that it is in blocking mode.
vpr::ReturnStatus FileHandleImplUNIX::setBlocking(bool blocking)
{
   vpr::ReturnStatus retval;

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
         retval.setCode(ReturnStatus::Fail);
      }
      else
      {
         mBlocking = blocking;
      }
   }

   return retval;
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
vpr::ReturnStatus FileHandleImplUNIX::setAppend(bool append)
{
   int cur_flags, new_flags, retval;
   vpr::ReturnStatus status;

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
      status.setCode(ReturnStatus::Fail);
   }

   return status;
}

// Reconfigure the file handle so that writes are synchronous.
vpr::ReturnStatus FileHandleImplUNIX::setSynchronousWrite(bool sync)
{
   vpr::ReturnStatus status;
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
      status.setCode(vpr::ReturnStatus::Fail);
   }
#else
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << "[vpr::FileHandleImplUNIX::setSynchronousWrite()] Cannot enable "
      << (sync ? "synchronous" : "asynchronous")
      << " writes on this platform!\n" << vprDEBUG_FLUSH;
   status.setCode(vpr::ReturnStatus::Fail);
#endif

   return status;
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

vpr::ReturnStatus FileHandleImplUNIX::getReadBufferSize(vpr::Int32& buffer) const
{
   vpr::ReturnStatus status;

   if ( ioctl(mFdesc, FIONREAD, &buffer) == -1 )
   {
      status.setCode(vpr::ReturnStatus::Fail);
   }

   return status;
}

// ============================================================================
// Protected methods.
// ============================================================================

// Read the specified number of bytes from the file handle into the given
// bufer.
vpr::ReturnStatus FileHandleImplUNIX::read_i(void* buffer,
                                             const vpr::Uint32 length,
                                             vpr::Uint32& bytesRead,
                                             const vpr::Interval timeout)
{
   vpr::ReturnStatus status;

   status = isReadable(timeout);

   if ( status.success() )
   {
      ssize_t bytes;

      bytes = ::read(mFdesc, buffer, length);

      // Something went wrong while attempting to read from the file.
      if ( bytes < 0 )
      {
         bytesRead = 0;

         if ( errno == EAGAIN && ! mBlocking )
         {
            status.setCode(vpr::ReturnStatus::WouldBlock);
         }
         // If the error is EAGAIN and we are in non-blocking mode, we do not
         // bother to print the message.
         if ( ! (errno == EAGAIN && ! mBlocking) )
         {
            vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
               << "[vpr::FileHandleImplUNIX::read_i()] Error reading from "
               << mName << ": " << strerror(errno) << std::endl
               << vprDEBUG_FLUSH;
            status.setCode(ReturnStatus::Fail);
         }
      }
      // If 0 bytes were read or an error was returned, we print an error
      // message.
      else if ( bytes == 0 && errno != 0 )
      {
         // XXX: Failure status may not be exactly what we want to return.
         status.setCode(ReturnStatus::Fail);
         bytesRead = 0;
//     errno != ENOENT
         vprDEBUG(vprDBG_ERROR, vprDBG_WARNING_LVL)
            << "[vpr::FileHandleImplUNIX::read_i()] Nothing read from "
            << mName << ": " << strerror(errno) << std::endl << vprDEBUG_FLUSH;
      }
      else
      {
         bytesRead = bytes;
      }
   }
   else
   {
      bytesRead = 0;
   }

   return status;
}

// Read exactly the specified number of bytes from the file handle into the
// given buffer.  This is baesd on the readn() function given on pages 51-2 of
// _Effective TCP/IP Programming_ by Jon D. Snader.
vpr::ReturnStatus FileHandleImplUNIX::readn_i(void* buffer,
                                              const vpr::Uint32 buffer_size,
                                              vpr::Uint32& bytesRead,
                                              const vpr::Interval timeout)
{
   size_t bytes_left;
   ssize_t bytes;
   vpr::ReturnStatus status;

   if ( vpr::Interval::NoTimeout != timeout )
   {
      vprDEBUG(vprDBG_ALL,vprDBG_WARNING_LVL) << "Timeout not supported\n"
                                              << vprDEBUG_FLUSH;
   }

   bytesRead = 0;
   bytes_left = buffer_size;

   while ( bytes_left > 0 )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
         << "[vpr::FileHandleImplUNIX::readn_i()] Reading " << bytes_left
         << " bytes from file handle " << mFdesc << std::endl
         << vprDEBUG_FLUSH;

      bytes = ::read(mFdesc, buffer, bytes_left);

      vprDEBUG_NEXT(vprDBG_ALL, vprDBG_HVERB_LVL)
         << "Read " << bytes << " bytes from file handle " << mFdesc
         << std::endl << vprDEBUG_FLUSH;

      // Read error.
      if ( bytes < 0 )
      {
         // Restart the read process if we were interrupted by the OS.
         if ( errno == EINTR )
         {
            continue;
         }
         // Otherwise, we have an error situation, so return failure status.
         else
         {
            status.setCode(ReturnStatus::Fail);
            bytesRead = 0;
            return status;
         }
      }
      // We have read EOF, so there is nothing more to read.  At this point,
      // bytesRead contains an accurate count of the bytes read so far
      // (posisbly less than buffer_size).
      else if ( bytes == 0 )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
            << "[vpr::FileHandleImplUNIX::readn_i()] Read EOF with "
            << bytes_left << " bytes left to read from file handle "
            << mFdesc << " and " << bytesRead << " bytes read in total."
            << std::endl << vprDEBUG_FLUSH;

         return status;
      }
      else
      {
         buffer = (void*) ((char*) buffer + bytes);
         bytes_left -= bytes;
         bytesRead  += bytes;
      }
   }

   return status;
}

// Write the buffer to the file handle.
vpr::ReturnStatus FileHandleImplUNIX::write_i(const void* buffer,
                                              const vpr::Uint32 length,
                                              vpr::Uint32& bytesWritten,
                                              const vpr::Interval timeout)
{
   vpr::ReturnStatus status;

   status = isWriteable(timeout);

   if ( status.success() )
   {
      ssize_t bytes;

      bytes = ::write(mFdesc, buffer, length);

      if ( bytes <= 0 )
      {
         bytesWritten = 0;

         if ( errno == EAGAIN && ! mBlocking )
         {
            status.setCode(vpr::ReturnStatus::WouldBlock);
         }
         else
         {
            vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
               << "[vpr::FileHandleImplUNIX::write_i()] Error writing to "
               << mName << ": " << strerror(errno) << std::endl
               << vprDEBUG_FLUSH;
            status.setCode(ReturnStatus::Fail);
         }
      }
      else
      {
         bytesWritten = bytes;
      }
   }

   return status;
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

vpr::ReturnStatus FileHandleImplUNIX::isReadable(const vpr::Interval timeout) const
{
   vpr::ReturnStatus ready;
   fd_set read_set;
   int num_events;
   struct timeval timeout_obj;

   if ( mFdesc == -1 )
   {
      ready.setCode(vpr::ReturnStatus::Fail);
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
         if ( ! FD_ISSET(mFdesc, &read_set) )
         {
            ready.setCode(vpr::ReturnStatus::Timeout);
         }
      }
      else if ( num_events < 0 )
      {
         ready.setCode(vpr::ReturnStatus::Fail);
      }
   }

   return ready;
}

vpr::ReturnStatus FileHandleImplUNIX::isWriteable(const vpr::Interval timeout) const
{
   vpr::ReturnStatus ready;
   fd_set write_set;
   int num_events;
   struct timeval timeout_obj;

   if ( mFdesc == -1 )
   {
      ready.setCode(vpr::ReturnStatus::Fail);
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

      num_events = select(mFdesc + 1, NULL, &write_set, NULL,
                          (timeout != vpr::Interval::NoTimeout) ? &timeout_obj :
                                                                  NULL);

      if ( num_events == 0 )
      {
         if ( ! FD_ISSET(mFdesc, &write_set) )
         {
            ready.setCode(vpr::ReturnStatus::Timeout);
         }
      }
      else if ( num_events < 0 )
      {
         ready.setCode(vpr::ReturnStatus::Fail);
      }
   }

   return ready;
}

} // End of vpr namespace
