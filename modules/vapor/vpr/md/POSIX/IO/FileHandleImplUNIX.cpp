/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#ifdef __sun__
#include <sys/file.h>
#endif

#include <vpr/md/POSIX/IO/FileHandleUNIX.h>


extern int errno;

namespace vpr {

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor.  This initializes the member variables to reasonable defaults
// and stores the given file name for later use.
// ----------------------------------------------------------------------------
FileHandleUNIX::FileHandleUNIX (const std::string& file_name)
    : FileHandle(file_name), m_fdesc(-1)
{
    /* Do nothing. */ ;
}

// ----------------------------------------------------------------------------
// Destructor.  If the file handle is in an open state, it is closed.
// ----------------------------------------------------------------------------
FileHandleUNIX::~FileHandleUNIX () {
    if ( m_open ) {
        close();
    }
}

// ----------------------------------------------------------------------------
// Open the file handle.
// ----------------------------------------------------------------------------
Status
FileHandleUNIX::open () {
    int open_flags;
    Status status;

    open_flags = 0;

    switch ( m_open_mode ) {
      case BlockIO::READ_ONLY:
fprintf(stderr, "[vpr::FileHandleUNIX] Opening read-only\n");
        open_flags = O_RDONLY;
        break;
      case BlockIO::WRITE_ONLY:
fprintf(stderr, "[vpr::FileHandleUNIX] Opening write-only\n");
        open_flags = O_WRONLY;
        break;
      case BlockIO::READ_WRITE:
fprintf(stderr, "[vpr::FileHandleUNIX] Opening read/write\n");
        open_flags = O_RDWR;
        break;
    }

    if ( ! m_open_blocking ) {
fprintf(stderr, "[vpr::FileHandleUNIX] Opening non-blocking\n");
        open_flags |= O_NONBLOCK;
    }
    else {
fprintf(stderr, "[vpr::FileHandleUNIX] Opening blocking\n");
    }

    m_fdesc = ::open(m_name.c_str(), open_flags);

    // If the file handle was not returned successfully, print an error
    // message explaining why.
    if ( m_fdesc == -1 ) {
        fprintf(stderr, "[vpr::FileHandleUNIX] Could not open %s: %s\n",
                m_name.c_str(), strerror(errno));
        status.setCode(Status::Failure);
    }
    // Otherwise, set m_open to true.
    else {
        m_open = true;
    }

    return status;
}

// ----------------------------------------------------------------------------
// Close the file handle.
// ----------------------------------------------------------------------------
Status
FileHandleUNIX::close () {
    Status status;

    if ( ::close(m_fdesc) == -1 ) {
        fprintf(stderr,
                "[vpr::FileHandleUNIX] Could not close %s: %s\n",
                m_name.c_str(), strerror(errno));
        status.setCode(Status::Failure);
    }
    else {
        m_fdesc = -1;
        m_open  = false;
    }

    return status;
}

/*
// ----------------------------------------------------------------------------
// Test if the file is read-only.
// ----------------------------------------------------------------------------
bool
FileHandleUNIX::isReadOnly () {
    int flags, accmode;

    flags   = getFlags();
    accmode = flags & O_ACCMODE;

    return (accmode == O_RDONLY);
}

// ----------------------------------------------------------------------------
// Test if the file is write-only.
// ----------------------------------------------------------------------------
bool
FileHandleUNIX::isWriteOnly () {
    int flags, accmode;

    flags   = getFlags();
    accmode = flags & O_ACCMODE;

    return (accmode == O_WRONLY);
}

// ----------------------------------------------------------------------------
// Test if the file is read/write.
// ----------------------------------------------------------------------------
bool
FileHandleUNIX::isReadWrite () {
    int flags, accmode;

    flags   = getFlags();
    accmode = flags & O_ACCMODE;

    return (accmode == O_RDWR);
}
*/

// ----------------------------------------------------------------------------
// Reconfigure the file handle so that it is in blocking mode.
// ----------------------------------------------------------------------------
Status
FileHandleUNIX::enableBlocking () {
    int cur_flags, new_flags;
    Status retval;

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
        fprintf(stderr,
                "[vpr::FileHandleUNIX] Failed to set blocking on %s: %s\n",
                m_name.c_str(), strerror(errno));
        retval.setCode(Status::Failure);
    }
    else {
        m_blocking = true;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Reconfigure the file handle so that it is in non-blocking mode.
// ----------------------------------------------------------------------------
Status
FileHandleUNIX::enableNonBlocking () {
    int cur_flags, new_flags;
    Status retval;

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
        fprintf(stderr,
                "[vpr::FileHandleUNIX] Failed to set non-blocking on %s: %s\n",
                m_name.c_str(), strerror(errno));
        retval.setCode(Status::Failure);
    }
    else {
        m_blocking = false;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Reconfigure the file handle to be in append mode.
// ----------------------------------------------------------------------------
Status
FileHandleUNIX::enableAppend () {
    int cur_flags, new_flags, retval;
    Status status;

    // Get the current flags and set O_APPEND.
    cur_flags = getFlags();
    new_flags = cur_flags | O_APPEND;

    // Set the file descriptor to be blocking with the new flags.
    retval = setFlags(new_flags);

    if ( retval == -1 ) {
        fprintf(stderr,
                "[vpr::FileHandleUNIX] Failed to enable append mode on %s: %s\n",
                m_name.c_str(), strerror(errno));
        status.setCode(Status::Failure);
    }

    return status;
}

// ----------------------------------------------------------------------------
// Reconfigure the file handle so that it is not in append mode.
// ----------------------------------------------------------------------------
Status
FileHandleUNIX::disableAppend () {
    int cur_flags, new_flags, retval;
    Status status;

    // Get the current flags and mask O_APPEND.
    cur_flags = getFlags();
    new_flags = cur_flags & ~O_APPEND;

    // Set the file descriptor to be blocking with the new flags.
    retval = setFlags(new_flags);

    if ( retval == -1 ) {
        fprintf(stderr,
                "[vpr::FileHandleUNIX] Failed to disable append mode on %s: %s\n",
                m_name.c_str(), strerror(errno));
        status.setCode(Status::Failure);
    }

    return status;
}

// ----------------------------------------------------------------------------
// Reconfigure the file handle so that writes are synchronous.
// ----------------------------------------------------------------------------
Status
FileHandleUNIX::enableSynchronousWrite () {
    Status status;
#if ! defined(_POSIX_SOURCE) && defined(O_SYNC)
    int cur_flags, new_flags, retval;

    // Get the current flags and set O_SYNC.
    cur_flags = getFlags();
    new_flags = cur_flags | O_SYNC;

    // Set the file descriptor to be blocking with the new flags.
    retval = setFlags(new_flags);

    if ( retval == -1 ) {
        fprintf(stderr,
                "[vpr::FileHandleUNIX] Failed to enable synchronous writes on %s: %s\n",
                m_name.c_str(), strerror(errno));
        status.setCode(Status::Failure);
    }
#else
    fprintf(stderr,
            "[vpr::FileHandleUNIX] Cannot enable synchronous writes on this platform!\n");
    status.setCode(Status::Failure);
#endif

    return status;
}

// ----------------------------------------------------------------------------
// Reconfigure the file handle so that writes are asynchronous.
// ----------------------------------------------------------------------------
Status
FileHandleUNIX::enableAsynchronousWrite () {
    Status status;
#if ! defined(_POSIX_SOURCE) && defined(O_ASYNC)
    int cur_flags, new_flags, retval;

    // Get the current flags and set O_ASYNC.
    cur_flags = getFlags();
    new_flags = cur_flags | O_ASYNC;

    // Set the file descriptor to be blocking with the new flags.
    retval = setFlags(new_flags);

    if ( retval == -1 ) {
        fprintf(stderr,
                "[vpr::FileHandleUNIX] Failed to enable asynchronous writes on %s: %s\n",
                m_name.c_str(), strerror(errno));
        status.setCode(Status::Failure);
    }
#else
    fprintf(stderr,
            "[vpr::FileHandleUNIX] Cannot enable asynchronous writes on this platform!\n");
    status.setCode(Status::Failure);
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
Status
FileHandleUNIX::read_i (void* buffer, const size_t length, ssize_t& bytes_read)
{
    Status status;

    bytes_read = ::read(m_fdesc, buffer, length);

    // Something went wrong while attempting to read from the file.
    if ( bytes_read < 0 ) {
        if ( errno == EAGAIN && ! m_blocking ) {
            status.setCode(Status::WouldBlock);
        }
        // If the error is EAGAIN and we are in non-blocking mode, we do not
        // bother to print the message.
        if ( ! (errno == EAGAIN && ! m_blocking) ) {
            fprintf(stderr, "[vpr::FileHandleUNIX] Error reading from %s: %s\n",
                    m_name.c_str(), strerror(errno));
            status.setCode(Status::Failure);
        }

    }
    // If 0 bytes were read or an error was returned, we print an error
    // message.
    else if ( bytes_read == 0 && errno != 0 ) {
//    errno != ENOENT
        fprintf(stderr, "[vpr::FileHandleUNIX] Nothing read from %s: %s\n",
                m_name.c_str(), strerror(errno));
    }

    return status;
}

// ----------------------------------------------------------------------------
// Read exactly the specified number of bytes from the file handle into the
// given buffer.  This is baesd on the readn() function given on pages 51-2 of
// _Effective TCP/IP Programming_ by Jon D. Snader.
// ----------------------------------------------------------------------------
Status
FileHandleUNIX::readn_i (void* buffer, const size_t length,
                         ssize_t& bytes_read)
{
    size_t count;
    Status status;

    count = length;

    while ( count > 0 ) {
        bytes_read = ::read(m_fdesc, buffer, length);

        // Read error.
        if ( bytes_read < 0 ) {
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
        else if ( bytes_read == 0 ) {
            bytes_read = length - count;
        }
        else {
            buffer = (void*) ((char*) buffer + bytes_read);
            count  -= bytes_read;
        }
    }

    return status;
}

// ----------------------------------------------------------------------------
// Write the buffer to the file handle.
// ----------------------------------------------------------------------------
Status
FileHandleUNIX::write_i (const void* buffer, const size_t length,
                         ssize_t& bytes_written)
{
    Status status;

    bytes_written = ::write(m_fdesc, buffer, length);

    if ( bytes_written <= 0 ) {
        if ( errno == EAGAIN && ! m_blocking ) {
            status.setCode(Status::WouldBlock);
        }
        else {
            fprintf(stderr,
                    "[vpr::FileHandleUNIX] Error writing to %s: %s\n",
                    m_name.c_str(), strerror(errno));
            status.setCode(Status::Failure);
        }
    }

    return status;
}

// ----------------------------------------------------------------------------
// Get the current file handle flags.
// ----------------------------------------------------------------------------
int
FileHandleUNIX::getFlags () {
    return fcntl(m_fdesc, F_GETFL, 0);
}

// ----------------------------------------------------------------------------
// Overwrite the current file handle flags with the given value.
// ----------------------------------------------------------------------------
int
FileHandleUNIX::setFlags (const int flags) {
    return fcntl(m_fdesc, F_SETFL, flags);
}

}; // End of vpr namespace
