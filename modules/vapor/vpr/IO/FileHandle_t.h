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
 * but WITHOUT ANY WARRANTY; without even the impllied warranty of
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

#ifndef _VPR_FILE_HANDLE_BRIDGE_H_
#define _VPR_FILE_HANDLE_BRIDGE_H_

#include <vpr/vprConfig.h>

#include <string>
#include <vector>

#include <vpr/IO/BlockIO.h>
#include <vpr/Util/Interval.h>
#include <vpr/Util/Status.h>


namespace vpr {

// ----------------------------------------------------------------------------
//: Extension to the vpr::BlockIO interface defining a cross-platform file
//+ handle interface.
// ----------------------------------------------------------------------------
//!PUBLIC_API:
template<class RealFileHandleImpl>
class FileHandle_t : public BlockIO {
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
    FileHandle_t (const std::string& file_name)
        : BlockIO(file_name), m_handle_impl(file_name)
    {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    //: Destructor.  This does nothing.
    //
    //! PRE:  None.
    //! POST: None.
    // ------------------------------------------------------------------------
    virtual ~FileHandle_t (void) {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    //: Get the name of this file.
    //
    //! PRE: None.
    //! POST: A constant reference to the m_name object is returned to the
    //+       caller.
    //
    //! RETURNS: An object containing the name of this file.
    // ------------------------------------------------------------------------
    virtual const std::string&
    getName (void) {
        return m_handle_impl.getName();
    }

    // ------------------------------------------------------------------------
    //: Open the file handle.
    //
    //! PRE: The file handle is not already open.
    //! POST: An attempt is made to open the file.  The resulting status is
    //+       returned to the caller.
    //
    //! RETURNS: vpr::Status::Success - The file handle was opened
    //+                                 successfully.
    //! RETURNS: vpr::Status::Failure - The file handle could not be opened
    //+                                 for some reason.
    // ------------------------------------------------------------------------
    virtual Status
    open (void) {
        return m_handle_impl.open();
    }

    // ------------------------------------------------------------------------
    //: Close the file handle.
    //
    //! PRE: The file handle is open.
    //! POST: An attempt is made to close the file.  The resulting status is
    //+       returned to the caller.
    //
    //! RETURNS: vpr::Status::Success - The file handle was closed
    //+                                 successfully.
    //! RETURNS: vpr::Status::Failure - The file handle could not be closed
    //+                                 for some reason.
    // ------------------------------------------------------------------------
    virtual Status
    close (void) {
        return m_handle_impl.close();
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle so that it is in blocking mode.
    //
    //! PRE: The file handle is open.
    //! POST: Processes will block when accessing the file.
    //
    //! RETURNS: vpr::Status::Success - The blocking mode was changed
    //+                                 successfully.
    //! RETURNS: vpr::Status::Failure - The blocking mode could not be changed
    //+                                 for some reason.
    // ------------------------------------------------------------------------
    virtual Status
    enableBlocking (void) {
        return m_handle_impl.enableBlocking();
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle so that it is in non-blocking mode.
    //
    //! PRE: The file handle is open.
    //! POST: Processes will not block when accessing the file.
    //
    //! RETURNS: vpr::Status::Success - The blocking mode was changed
    //+                                 successfully.
    //! RETURNS: vpr::Status::Failure - The blocking mode could not be changed
    //+                                 for some reason.
    // ------------------------------------------------------------------------
    virtual Status
    enableNonBlocking (void) {
        return m_handle_impl.enableNonBlocking();
    }

    // ------------------------------------------------------------------------
    //: Get the current blocking state for the file.
    //
    //! PRE: m_blocking is set correctly
    //! POST:
    //
    //! RETURNS: true  - The file is in blocking mode.
    //! RETURNS: false - The file is in non-blocking mode.
    // ------------------------------------------------------------------------
    inline bool
    getBlocking (void) const {
        return m_handle_impl.getBlocking();
    }

    // ------------------------------------------------------------------------
    //: Get the current non-blocking state for the file.
    //
    //! PRE: m_blocking is set correctly
    //! POST:
    //
    //! RETURNS: true  - The file is in non-blocking mode.
    //! RETURNS: false - The file is in blocking mode.
    // ------------------------------------------------------------------------
    inline bool
    getNonBlocking (void) const {
        return m_handle_impl.getNonBlocking();
    }

    // ------------------------------------------------------------------------
    //! RETURNS: vpr::IOSys::NullHandle
    // ------------------------------------------------------------------------
    virtual IOSys::Handle
    getHandle (void) {
        return m_handle_impl.getHandle();
    }

    // ========================================================================
    // vpr::FileHandle API extensions.
    // ========================================================================

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle to be in append mode.
    //
    //! PRE: The file handle is open.
    //! POST: The file handle's write mode is set to append.
    //
    //! RETURNS: vpr::Status::Success - The write mode was changed
    //+                                 successfully.
    //! RETURNS: vpr::Status::Failure - The write mode could not be changed
    //+                                 for some reason.
    // ------------------------------------------------------------------------
    inline Status
    enableAppend (void) {
        return m_handle_impl.enableAppend();
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle so that it is not in append mode.
    //
    //! PRE: The file handle is open.
    //! POST: The file handle's write mode is set so that writes are appended.
    //
    //! RETURNS: vpr::Status::Success - The write mode was changed
    //+                                 successfully.
    //! RETURNS: vpr::Status::Failure - The write mode could not be changed
    //+                                 for some reason.
    // ------------------------------------------------------------------------
    inline Status
    disableAppend (void) {
        return m_handle_impl.disableAppend();
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle so that writes are synchronous.
    //
    //! PRE: The file handle is open.
    //! POST: Writes are performed synchronously.
    //
    //! RETURNS: vpr::Status::Success - The write mode was changed
    //+                                 successfully.
    //! RETURNS: vpr::Status::Failure - The write mode could not be changed
    //+                                 for some reason.
    // ------------------------------------------------------------------------
    inline Status
    enableSynchronousWrite (void) {
        return m_handle_impl.enableSynchronousWrite();
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle so that writes are asynchronous.
    //
    //! PRE: The file handle is open.
    //! POST: Writes are performed asynchronously.
    //
    //! RETURNS: vpr::Status::Success - The write mode was changed
    //+                                 successfully.
    //! RETURNS: vpr::Status::Failure - The write mode could not be changed
    //+                                 for some reason.
    // ------------------------------------------------------------------------
    inline Status
    enableAsynchronousWrite (void) {
        return m_handle_impl.enableAsynchronousWrite();
    }

protected:
    // ------------------------------------------------------------------------
    //: Read at most the specified number of bytes from the file handle into
    //+ the given buffer.
    //
    //! PRE: The file handle implementation object is valid, and the buffer is
    //+      at least length bytes long.
    //! POST: The given buffer has length bytes copied into it from the file
    //+       bufffer, and the number of bytes read successfully is returned
    //+       to the caller.
    //
    //! ARGS: buffer     - A pointer to the buffer where the file's buffer
    //+                    contents are to be stored.
    //! ARGS: length     - The number of bytes to be read.
    //! ARGS: bytes_read - A reference to a variable where the number of bytes
    //+                    successfully read from the file will be stored.
    //+                    The value will be -1 if an error occurred.
    //! ARGS: timeout    - The amount of time to wait before returning to the
    //+                    caller.  This argument is optional and defaults to
    //+                    vpr::Interval::NoTimeout.
    //
    //! RETURNS: vpr::Status::Success    - The read operation completed
    //+                                    successfully.
    //! RETURNS: vpr::Status::Failure    - The read operation failed.
    //! RETURNS: vpr::Status::InProgress - The handle is in non-blocking mode,
    //+                                    and the read operation is in
    //+                                    progress.
    // ------------------------------------------------------------------------
    virtual Status
    read_i (void* buffer, const size_t length, ssize_t& bytes_read,
            const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_handle_impl.read(buffer, length, bytes_read, timeout);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the file into the
    //+ given buffer.
    //
    //! PRE: The handle implementation object is valid, and the buffer is at
    //+      least length bytes long.
    //! POST: The given buffer has length bytes copied into it from the file
    //+       buffer, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer     - A pointer to the buffer where the file's buffer
    //+                    contents are to be stored.
    //! ARGS: length     - The number of bytes to be read.
    //! ARGS: bytes_read - A reference to a variable where the number of bytes
    //+                    successfully read from the file will be stored.
    //+                    The value will be -1 if an error occurred.
    //! ARGS: timeout    - The amount of time to wait before returning to the
    //+                    caller.  This argument is optional and defaults to
    //+                    vpr::Interval::NoTimeout.
    //
    //! RETURNS: vpr::Status::Success - The read operation completed
    //+                                 successfully.
    //! RETURNS: vpr::Status::Failure - The read operation failed.
    // ------------------------------------------------------------------------
    virtual Status
    readn_i (void* buffer, const size_t length, ssize_t& bytes_read,
             const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_handle_impl.readn(buffer, length, bytes_read, timeout);
    }

    // ------------------------------------------------------------------------
    //: Write the given buffer to the file.
    //
    //! PRE: The handle implementation object is valid.
    //! POST: The given buffer is written to the file, and the number of
    //+       bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer        - A pointer to the buffer to be written.
    //! ARGS: length        - The length of the buffer.
    //! ARGS: bytes_written - A reference to a variable where the number of
    //+                       bytes successfully written to the file will be
    //+                       stored.  The value will be -1 if an error
    //+                       occurred.
    //! ARGS: timeout       - The amount of time to wait before returning to the
    //+                       caller.  This argument is optional and defaults to
    //+                       vpr::Interval::NoTimeout.
    //
    //! RETURNS: vpr::Status::Success    - The write operation completed
    //+                                    successfully.
    //! RETURNS: vpr::Status::Failure    - The write operation failed.
    //! RETURNS: vpr::Status::InProgress - The handle is in non-blocking mode,
    //+                                    and the write operation is in
    //+                                    progress.
    // ------------------------------------------------------------------------
    virtual Status
    write_i (const void* buffer, const size_t length, ssize_t& bytes_written,
             const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_handle_impl.write(buffer, length, bytes_written, timeout);
    }

    RealFileHandleImpl m_handle_impl; //: Platform-specific file hanlde impll
};

} // End of vpr namespace


#endif	/* _VPR_FILE_HANDLE_H_ */
