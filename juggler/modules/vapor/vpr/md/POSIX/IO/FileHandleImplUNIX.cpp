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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>

#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif

#include <vpr/Util/Debug.h>
#include <vpr/md/POSIX/IO/FileHandleImplUNIX.h>


namespace vpr {

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor.  This initializes the member variables to reasonable defaults
// and stores the given file name for later use.
// ----------------------------------------------------------------------------
FileHandleImplUNIX::FileHandleImplUNIX (const std::string& file_name)
    : m_name(file_name), m_open(false), m_open_blocking(true),
      m_blocking(true), m_fdesc(-1), m_open_mode(O_RDWR)
{
    /* Do nothing. */ ;
}

// ----------------------------------------------------------------------------
// Destructor.  If the file handle is in an open state, it is closed.
// ----------------------------------------------------------------------------
FileHandleImplUNIX::~FileHandleImplUNIX () {
    if ( m_open ) {
        close();
    }
}

// ----------------------------------------------------------------------------
// Open the file handle.
// ----------------------------------------------------------------------------
ReturnStatus
FileHandleImplUNIX::open () {
    int open_flags;
    ReturnStatus status;

    open_flags = m_open_mode;

    if ( ! m_open_blocking ) {
        open_flags |= O_NONBLOCK;
    }

    m_fdesc = ::open(m_name.c_str(), open_flags);

    // If the file handle was not returned successfully, print an error
    // message explaining why.
    if ( m_fdesc == -1 ) {
        // If we are opening in non-blocking mode, we do not want to bomb out.
        if ( errno == EWOULDBLOCK && ! m_open_blocking ) {
            status.setCode(vpr::ReturnStatus::WouldBlock);
            m_open = true;
        }
        // Otherwise, report the error.
        else {
            vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
                << "[vpr::FileHandleImplUNIX] Could not open " << m_name << ": "
                << strerror(errno) << std::endl << vprDEBUG_FLUSH;
            status.setCode(ReturnStatus::Fail);
            m_open = false;
        }
    }
    // Otherwise, set m_open to true.
    else {
        m_open     = true;
        m_blocking = m_open_blocking;
    }

    return status;
}

// ----------------------------------------------------------------------------
// Close the file handle.
// ----------------------------------------------------------------------------
ReturnStatus
FileHandleImplUNIX::close () {
    ReturnStatus status;

    if ( ::close(m_fdesc) == -1 ) {
        vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "[vpr::FileHandleImplUNIX] Could not close " << m_name << ": "
            << strerror(errno) << std::endl << vprDEBUG_FLUSH;
        status.setCode(ReturnStatus::Fail);
    }
    else {
        m_fdesc = -1;
        m_open  = false;
    }

    return status;
}

// ----------------------------------------------------------------------------
// Reconfigure the file handle so that it is in blocking mode.
// ----------------------------------------------------------------------------
ReturnStatus
FileHandleImplUNIX::enableBlocking () {
    int cur_flags, new_flags;
    ReturnStatus retval;

    // Get the current flags.
    cur_flags = getFlags();

#ifdef _SGI_SOURCE
    // On IRIX, mask FNONBLK and FNDELAY.  We mask FNDELAY to ensure that it
    // is not set by the operating system at some level.
    new_flags = cur_flags & ~(FNONBLK | FNDELAY);
#else
    // On everything else, mask O_NONBLOCK and O_NDELAY.  We mask O_NDELAY to
    // ensure that it is not set by the operating system at some level.
    new_flags = cur_flags & ~(O_NONBLOCK | O_NDELAY);
#endif

    // Set the file descriptor to be blocking with the new flags.
    if ( setFlags(new_flags) == -1 ) {
        vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << "[vpr::FileHandleImplUNIX] Failed to set blocking on " << m_name
            << ": " << strerror(errno) << std::endl << vprDEBUG_FLUSH;
        retval.setCode(ReturnStatus::Fail);
    }
    else {
        m_blocking = true;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Reconfigure the file handle so that it is in non-blocking mode.
// ----------------------------------------------------------------------------
ReturnStatus
FileHandleImplUNIX::enableNonBlocking () {
    int cur_flags, new_flags;
    ReturnStatus retval;

    // Get the current flags.
    cur_flags = getFlags();

#ifdef _SVR4_SOURCE
    // On SysV, set FNONBLK.  We do not set FNDELAY because it just adds
    // confusion.  FNONBLK is preferred.
    new_flags = cur_flags | FNONBLK;
#else
    // On everything else, set O_NONBLOCK.  We do not set O_NDELAY because
    // it just adds confusion.  O_NONBLOCK is preferred.
    new_flags = cur_flags | O_NONBLOCK;
#endif

    // Set the file descriptor to be non-blocking with the new flags.
    if ( setFlags(new_flags) == -1 ) {
        vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << "[vpr::FileHandleImplUNIX] Failed to set non-blocking on "
            << m_name << ": " << strerror(errno) << std::endl
            << vprDEBUG_FLUSH;
        retval.setCode(ReturnStatus::Fail);
    }
    else {
        m_blocking = false;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Reconfigure the file handle to be in append mode.
// ----------------------------------------------------------------------------
ReturnStatus
FileHandleImplUNIX::enableAppend () {
    int cur_flags, new_flags, retval;
    ReturnStatus status;

    // Get the current flags and set O_APPEND.
    cur_flags = getFlags();
    new_flags = cur_flags | O_APPEND;

    // Set the file descriptor to be blocking with the new flags.
    retval = setFlags(new_flags);

    if ( retval == -1 ) {
        vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << "[vpr::FileHandleImplUNIX] Failed to enable append mode on "
            << m_name << ": " << strerror(errno) << std::endl
            << vprDEBUG_FLUSH;
        status.setCode(ReturnStatus::Fail);
    }

    return status;
}

// ----------------------------------------------------------------------------
// Reconfigure the file handle so that it is not in append mode.
// ----------------------------------------------------------------------------
ReturnStatus
FileHandleImplUNIX::disableAppend () {
    int cur_flags, new_flags, retval;
    ReturnStatus status;

    // Get the current flags and mask O_APPEND.
    cur_flags = getFlags();
    new_flags = cur_flags & ~O_APPEND;

    // Set the file descriptor to be blocking with the new flags.
    retval = setFlags(new_flags);

    if ( retval == -1 ) {
        vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << "[vpr::FileHandleImplUNIX] Failed to disable append mode on "
            << m_name << ": " << strerror(errno) << std::endl
            << vprDEBUG_FLUSH;
        status.setCode(ReturnStatus::Fail);
    }

    return status;
}

// ----------------------------------------------------------------------------
// Reconfigure the file handle so that writes are synchronous.
// ----------------------------------------------------------------------------
ReturnStatus
FileHandleImplUNIX::enableSynchronousWrite () {
    ReturnStatus status;
#if ! defined(_POSIX_SOURCE) && defined(O_SYNC)
    int cur_flags, new_flags, retval;

    // Get the current flags and set O_SYNC.
    cur_flags = getFlags();
    new_flags = cur_flags | O_SYNC;

    // Set the file descriptor to be blocking with the new flags.
    retval = setFlags(new_flags);

    if ( retval == -1 ) {
        vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << "[vpr::FileHandleImplUNIX] Failed to enable synchronous writes on "
            << m_name << ": " << strerror(errno) << std::endl
            << vprDEBUG_FLUSH;
        status.setCode(ReturnStatus::Fail);
    }
#else
    vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
        << "[vpr::FileHandleImplUNIX] Cannot enable synchronous writes on this platform!\n"
        << vprDEBUG_FLUSH;
    status.setCode(ReturnStatus::Fail);
#endif

    return status;
}

// ----------------------------------------------------------------------------
// Reconfigure the file handle so that writes are asynchronous.
// ----------------------------------------------------------------------------
ReturnStatus
FileHandleImplUNIX::enableAsynchronousWrite () {
    ReturnStatus status;
#if ! defined(_POSIX_SOURCE) && defined(O_ASYNC)
    int cur_flags, new_flags, retval;

    // Get the current flags and set O_ASYNC.
    cur_flags = getFlags();
    new_flags = cur_flags | O_ASYNC;

    // Set the file descriptor to be blocking with the new flags.
    retval = setFlags(new_flags);

    if ( retval == -1 ) {
        vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << "[vpr::FileHandleImplUNIX] Failed to enable asynchronous writes on "
            << m_name << ": " << strerror(errno) << std::endl
            << vprDEBUG_FLUSH;
        status.setCode(ReturnStatus::Fail);
    }
#else
    vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
        << "[vpr::FileHandleImplUNIX] Cannot enable asynchronous writes on this platform!\n"
        << vprDEBUG_FLUSH;
    status.setCode(ReturnStatus::Fail);
#endif

    return status;
}

// ============================================================================
// Protected methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Read the specified number of bytes from the file handle into the given
// bufer.
// ----------------------------------------------------------------------------
ReturnStatus
FileHandleImplUNIX::read_i (void* buffer, const vpr::Uint32 length,
                            vpr::Uint32& bytes_read,
                            const vpr::Interval timeout)
{
    ReturnStatus status;

    status = isReadable(timeout);

    if ( status.success() ) {
        ssize_t bytes;

        bytes = ::read(m_fdesc, buffer, length);

        // Something went wrong while attempting to read from the file.
        if ( bytes < 0 ) {
            bytes_read = 0;

            if ( errno == EAGAIN && ! m_blocking ) {
                status.setCode(vpr::ReturnStatus::WouldBlock);
            }
            // If the error is EAGAIN and we are in non-blocking mode, we do not
            // bother to print the message.
            if ( ! (errno == EAGAIN && ! m_blocking) ) {
                vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
                    << "[vpr::FileHandleImplUNIX] Error reading from "
                    << m_name << ": " << strerror(errno) << std::endl
                    << vprDEBUG_FLUSH;
                status.setCode(ReturnStatus::Fail);
            }
        }
        // If 0 bytes were read or an error was returned, we print an error
        // message.
        else if ( bytes == 0 && errno != 0 ) {
            bytes_read = 0;
//        errno != ENOENT
            vprDEBUG(vprDBG_ERROR, vprDBG_WARNING_LVL)
                << "[vpr::FileHandleImplUNIX] Nothing read from " << m_name
                << ": " << strerror(errno) << std::endl << vprDEBUG_FLUSH;
        }
        else {
            bytes_read = bytes;
        }
    }

    return status;
}

// ----------------------------------------------------------------------------
// Read exactly the specified number of bytes from the file handle into the
// given buffer.  This is baesd on the readn() function given on pages 51-2 of
// _Effective TCP/IP Programming_ by Jon D. Snader.
// ----------------------------------------------------------------------------
ReturnStatus
FileHandleImplUNIX::readn_i (void* buffer, const vpr::Uint32 length,
                             vpr::Uint32& bytes_read,
                             const vpr::Interval timeout)
{
    size_t count;
    ssize_t bytes;
    ReturnStatus status;

    if(vpr::Interval::NoTimeout != timeout)
       vprDEBUG(vprDBG_ALL,vprDBG_WARNING_LVL) << "Timeout not supported\n" << vprDEBUG_FLUSH;

    count = length;

    while ( count > 0 ) {
        bytes = ::read(m_fdesc, buffer, length);

        // Read error.
        if ( bytes < 0 ) {
            // Restart the read process if we were interrupted by the OS.
            if ( errno == EINTR ) {
                continue;
            }
            // Otherwise, we have an error situation, so return the value
            // returned by read(2).
            else  {
                break;
            }
        }
        // May have read EOF, so return bytes read so far.
        else if ( bytes == 0 ) {
            bytes = length - count;
        }
        else {
            buffer = (void*) ((char*) buffer + bytes);
            count  -= bytes;
        }
    }

    bytes_read = bytes;

    return status;
}

// ----------------------------------------------------------------------------
// Write the buffer to the file handle.
// ----------------------------------------------------------------------------
ReturnStatus
FileHandleImplUNIX::write_i (const void* buffer, const vpr::Uint32 length,
                             vpr::Uint32& bytes_written,
                             const vpr::Interval timeout)
{
    vpr::ReturnStatus status;

    status = isWriteable(timeout);

    if ( status.success() ) {
        ssize_t bytes;

        bytes = ::write(m_fdesc, buffer, length);

        if ( bytes <= 0 ) {
            bytes_written = 0;

            if ( errno == EAGAIN && ! m_blocking ) {
                status.setCode(vpr::ReturnStatus::WouldBlock);
            }
            else {
                vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
                    << "[vpr::FileHandleImplUNIX] Error writing to " << m_name
                    << ": " << strerror(errno) << std::endl << vprDEBUG_FLUSH;
                status.setCode(ReturnStatus::Fail);
            }
        }
        else {
            bytes_written = bytes;
        }
    }

    return status;
}

// ----------------------------------------------------------------------------
// Get the current file handle flags.
// ----------------------------------------------------------------------------
int
FileHandleImplUNIX::getFlags () {
    return fcntl(m_fdesc, F_GETFL, 0);
}

// ----------------------------------------------------------------------------
// Overwrite the current file handle flags with the given value.
// ----------------------------------------------------------------------------
int
FileHandleImplUNIX::setFlags (const int flags) {
    return fcntl(m_fdesc, F_SETFL, flags);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
ReturnStatus
FileHandleImplUNIX::isReadable (const vpr::Interval timeout) {
    vpr::ReturnStatus ready;
    fd_set read_set;
    int num_events;
    struct timeval timeout_obj;

    if ( m_fdesc == -1 )
    {
       ready.setCode(vpr::ReturnStatus::Fail);
    }
    else
    {
       if ( timeout == vpr::Interval::NoWait ) {
           timeout_obj.tv_sec  = 0;
           timeout_obj.tv_usec = 0;
       }
       else {
           if ( timeout.msec() >= 1000 ) {
               timeout_obj.tv_sec  = timeout.msec() / 1000;
               timeout_obj.tv_usec = (timeout.msec() % 1000) * 1000000;
           }
           else {
               timeout_obj.tv_sec  = 0;
               timeout_obj.tv_usec = timeout.msec() * 1000;
           }
       }

       FD_ZERO(&read_set);
       FD_SET(m_fdesc, &read_set);

       num_events = select(m_fdesc + 1, &read_set, NULL, NULL,
                           (timeout != vpr::Interval::NoTimeout) ? &timeout_obj :
                                                                   NULL);

       if ( num_events == 0 ) {
           if ( ! FD_ISSET(m_fdesc, &read_set) ) {
               ready.setCode(vpr::ReturnStatus::Timeout);
           }
       }
       else if ( num_events < 0 ) {
           ready.setCode(vpr::ReturnStatus::Fail);
       }
    }

    return ready;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
ReturnStatus
FileHandleImplUNIX::isWriteable (const vpr::Interval timeout) {
    vpr::ReturnStatus ready;
    fd_set write_set;
    int num_events;
    struct timeval timeout_obj;

    if ( m_fdesc == -1 )
    {
       ready.setCode(vpr::ReturnStatus::Fail);
    }
    else
    {
       if ( timeout == vpr::Interval::NoWait ) {
           timeout_obj.tv_sec  = 0;
           timeout_obj.tv_usec = 0;
       }
       else {
           if ( timeout.msec() >= 1000 ) {
               timeout_obj.tv_sec  = timeout.msec() / 1000;
               timeout_obj.tv_usec = (timeout.msec() % 1000) * 1000000;
           }
           else {
               timeout_obj.tv_sec  = 0;
               timeout_obj.tv_usec = timeout.msec() * 1000;
           }
       }

       FD_ZERO(&write_set);
       FD_SET(m_fdesc, &write_set);

       num_events = select(m_fdesc + 1, NULL, &write_set, NULL,
                           (timeout != vpr::Interval::NoTimeout) ? &timeout_obj :
                                                                   NULL);

       if ( num_events == 0 ) {
           if ( ! FD_ISSET(m_fdesc, &write_set) ) {
               ready.setCode(vpr::ReturnStatus::Timeout);
           }
       }
       else if ( num_events < 0 ) {
           ready.setCode(vpr::ReturnStatus::Fail);
       }
    }

    return ready;
}

}; // End of vpr namespace
