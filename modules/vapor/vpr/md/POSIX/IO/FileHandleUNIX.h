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

#ifndef _VPR_FILE_HANDLE_UNIX_H_
#define _VPR_FILE_HANDLE_UNIX_H_

#include <sys/types.h>
#include <string>
#include <vector>

#include <IO/FileHandle.h>


namespace vpr {

class FileHandleUNIX : public FileHandle {
public:
    // ------------------------------------------------------------------------
    //: Constructor.  This initializes the member variables to reasonable
    //+ defaults and stores the given file name for later use.
    //
    //! PRE: None.
    //! POST: All member variables are initialized including m_name that is
    //+       assigned the string in file_name.
    //
    //! ARGS: file_name - The name of the file to be handled.
    // ------------------------------------------------------------------------
    FileHandleUNIX(const std::string& file_name);

    // ------------------------------------------------------------------------
    //: Destructor.  If the file handle is in an open state, it is closed.
    //
    //! PRE: None.
    //! POST: If the file handle is still open, it is closed.
    // ------------------------------------------------------------------------
    virtual ~FileHandleUNIX(void);

    // ========================================================================
    // vpr::BlockIO implementation.
    // ========================================================================

    // ------------------------------------------------------------------------
    //: Open the file handle.
    //
    //! PRE: The file handle is not already open.
    //! POST: An attempt is made to open the file.  The resulting status is
    //+       returned to the caller.  If the file is opened, m_open is set to
    //+       true.
    //
    //! RETURNS: true  - The file handle was opened successfully.
    //! RETURNS: false - The file handle could not be opened for some reason.
    // ------------------------------------------------------------------------
    virtual bool open(void);

    // ------------------------------------------------------------------------
    //: Close the file handle.
    //
    //! PRE: The file handle is open.
    //! POST: An attempt is made to close the file.  The resulting status is
    //+       returned to the caller.  If the file is closed, m_open is set to
    //+       false.
    //
    //! RETURNS: true  - The file handle was closed successfully.
    //! RETURNS: false - The file handle could not be closed for some reason.
    //+                  An error message is printed explaining what went
    //+                  wrong.
    // ------------------------------------------------------------------------
    virtual bool close(void);

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle so that it is in blocking mode.
    //
    //! PRE: The file handle is open.
    //! POST: Processes will block when accessing the file.
    //
    //! RETURNS: true  - The blocking mode was changed successfully.
    //! RETURNS: false - The blocking mode could not be changed for some
    //+                  reason.
    // ------------------------------------------------------------------------
    virtual bool enableBlocking(void);

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle so that it is in non-blocking mode.
    //
    //! PRE: The file handle is open.
    //! POST: Processes will not block when accessing the file.
    //
    //! RETURNS: true  - The blocking mode was changed successfully.
    //! RETURNS: false - The blocking mode could not be changed for some
    //+                  reason.
    // ------------------------------------------------------------------------
    virtual bool enableNonBlocking(void);

    // ------------------------------------------------------------------------
    //! Read the specified number of bytes from the file handle into the given
    //+ buffer.
    //
    //! PRE: The file descriptor is valid, and the buffer is at least length
    //+       bytes long.
    //! POST: The given buffer has length bytes copied into it from the file
    //+       handle, and the number of bytes read successfully is returned
    //+       to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer where the file contents are to
    //+                be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the file
    //+                handle.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t read(void* buffer, const size_t length);

    // ------------------------------------------------------------------------
    //! Read the specified number of bytes from the file handle into the given
    //+ buffer.
    //
    //! PRE: The file descriptor is valid, and the buffer is at least length
    //+       bytes long.
    //! POST: The given buffer has length bytes copied into it from the file
    //+       handle, and the number of bytes read successfully is returned
    //+       to the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::string object) where
    //+                the file contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the file
    //+                handle.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t read(std::string& buffer, const size_t length = 0);

    // ------------------------------------------------------------------------
    //! Read the specified number of bytes from the file handle into the given
    //+ buffer.
    //
    //! PRE: The file descriptor is valid, and the buffer is at least length
    //+       bytes long.
    //! POST: The given buffer has length bytes copied into it from the file
    //+       handle, and the number of bytes read successfully is returned
    //+       to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (a vector of chars) where the
    //+                file contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the file
    //+                handle.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t read(std::vector<char>& buffer, const size_t length = 0);

    // ------------------------------------------------------------------------
    //! Read exactly the specified number of bytes from the file handle into
    //+ the given buffer.
    //
    //! PRE: The file descriptor is valid, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the file
    //+       handle, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer where the file contents are to
    //                 be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t readn(void* buffer, const size_t length);

    // ------------------------------------------------------------------------
    //! Read exactly the specified number of bytes from the file handle into
    //+ the given buffer.
    //
    //! PRE: The file descriptor is valid, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the file
    //+       handle, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A reference to the buffer where the file contents are
    //                 to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t readn(std::string& buffer, const size_t length = 0);

    // ------------------------------------------------------------------------
    //! Read exactly the specified number of bytes from the file handle into
    //+ the given buffer.
    //
    //! PRE: The file descriptor is valid, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the file
    //+       handle, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer where the file contents are to
    //                 be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t readn(std::vector<char>& buffer, const size_t length = 0);

    // ------------------------------------------------------------------------
    //: Write the buffer to the file handle.
    //
    //! PRE: The file descriptor is valid.
    //! POST: The given buffer is written to the file handle, and the number
    //+       of bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer to be written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the file
    //+                handle.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    virtual ssize_t write(const void* buffer, const size_t length);

    // ------------------------------------------------------------------------
    //: Write the buffer to the file handle.
    //
    //! PRE: The file descriptor is valid.
    //! POST: The given buffer is written to the file handle, and the number
    //+       of bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::string object) to be
    //+                written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the file
    //+                handle.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    virtual ssize_t write(const std::string& buffer, const size_t length = 0);

    // ------------------------------------------------------------------------
    //: Write the buffer to the file handle.
    //
    //! PRE: The file descriptor is valid.
    //! POST: The given buffer is written to the file handle, and the number
    //+       of bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (a vector of chars) to be
    //+                written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the file
    //+                handle.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    virtual ssize_t write(const std::vector<char>& buffer,
                          const size_t length = 0);

    // ========================================================================
    // vpr::FileHandle implementation.
    // ========================================================================

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle to be in append mode.
    //
    //! PRE: The file handle is open.
    //! POST: The file handle's write mode is set to append.
    //
    //! RETURNS: true  - The write mode was changed successfully.
    //! RETURNS: false - The write mode could not be changed for some reason.
    // ------------------------------------------------------------------------
    virtual bool enableAppend(void);

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle so that it is not in append mode.
    //
    //! PRE: The file handle is open.
    //! POST: The file handle's write mode is set so that writes are appended.
    //
    //! RETURNS: true  - The write mode was changed successfully.
    //! RETURNS: false - The write mode could not be changed for some reason.
    // ------------------------------------------------------------------------
    virtual bool disableAppend(void);

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle so that writes are synchronous.
    //
    //! PRE: The file handle is open.
    //! POST: Writes are performed synchronously.
    //
    //! RETURNS: true  - The write mode was changed successfully.
    //! RETURNS: false - The write mode could not be changed for some reason.
    // ------------------------------------------------------------------------
    virtual bool enableSynchronousWrite(void);

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle so that writes are asynchronous.
    //
    //! PRE: The file handle is open.
    //! POST: Writes are performed asynchronously.
    //
    //! RETURNS: true  - The write mode was changed successfully.
    //! RETURNS: false - The write mode could not be changed for some reason.
    // ------------------------------------------------------------------------
    virtual bool enableAsynchronousWrite(void);

protected:
    // Friends.
    friend class SerialPortImpTermios;
    friend class SocketDatagramImpBSD;
    friend class SocketImpBSD;
    friend class SocketStreamImpBSD;

    // ------------------------------------------------------------------------
    //: Get the current file handle flags.
    //
    //! PRE: The file handle is open.
    //! POST: The current flags for the handle are returned to the caller.
    //
    //! RETURNS: >-1 - The current flags for the file handle.
    //! RETURNS:  -1 - The current flags could not be requested.
    // ------------------------------------------------------------------------
    int getFlags(void);

    // ------------------------------------------------------------------------
    //: Overwrite the current file handle flags with the given value.
    //
    //! PRE: The file handle is open.
    //! POST: The flags for the file handle are overwritten with the new
    //+       value.
    //
    //! RETURNS:  0 - The flags were updated successfully.
    //! RETURNS: -1 - The current flags could not be overwritten.
    // ------------------------------------------------------------------------
    int setFlags(const int flags);

    int m_fdesc;    //: File descriptor
};

}; // End of vpr namespace


#endif	/* _VPR_FILE_HANDLE_UNIX_H_ */
