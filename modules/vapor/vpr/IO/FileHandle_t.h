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

#ifndef _VPR_FILE_HANDLE_BRIDGE_H_
#define _VPR_FILE_HANDLE_BRIDGE_H_

#include <vpr/vprConfig.h>

#include <string>
#include <vector>

#include <vpr/IO/BlockIO.h>
#include <vpr/Util/Interval.h>
#include <vpr/Util/ReturnStatus.h>


namespace vpr {

/**
 * Extension to the vpr::BlockIO interface defining a cross-platform file
 * handle interface.
 *
 * @author Patrick Hartling
 */
template<class RealFileHandleImpl>
class FileHandle_t : public BlockIO {
public:
    /**
     * Constructor.  This initializes the member variables to reasonable
     * defaults and stores the given file name for later use.
     *
     * @pre None.
     * @post All member variables are initialized including m_name that is
     *       assigned the string in file_name.
     *
     * @param file_name The name of the file to be handled.
     */
    FileHandle_t (const std::string& file_name)
        : BlockIO(file_name), m_open_mode(READ_WRITE), m_handle_impl(file_name)
    {
        /* Do nothing. */ ;
    }

    /**
     * Destructor.  This does nothing.
     *
     * @pre None.
     * @post None.
     */
    virtual ~FileHandle_t (void) {
        /* Do nothing. */ ;
    }

    /**
     * Gets the name of this file.
     *
     * @pre None.
     * @post A constant reference to the m_name object is returned to the
     *       caller.
     *
     * @return An object containing the name of this file.
     */
    virtual const std::string&
    getName (void) {
        return m_handle_impl.getName();
    }

    /**
     * Opens the file.
     *
     * @pre This file handle has not already been opened.
     * @post An attempt is made to open the file.  The resulting status is
     *       returned to the caller.  If opened successfully, this file is
     *       ready for use.
     *
     * @return <code>vpr::ReturnStatus::Succeed</code> is returned when the file was
     *         opened successfully.<br>
     *         <code>vpr::ReturnStatus::Fail</code> is returned if the file could
     *         not be opened for some reason.
     */
    virtual ReturnStatus
    open (void) {
        return m_handle_impl.open();
    }

    /**
     * Closes the file.  All pending operatings (as queued by the OS) are
     * completed.
     *
     * @pre This file handle is open.
     * @post An attempt is made to close the file.  The resulting status is
     *       returned to the caller.
     *
     * @return <code>vpr::ReturnStatus::Succeed</code> is returned if the file was
     *         closed successfully.<br>
     *         <code>vpr::ReturnStatus::Fail</code> is returned if the file could
     *         not be closed.
     */
    virtual ReturnStatus
    close (void) {
        return m_handle_impl.close();
    }

    /**
     * Reconfigures the file handle so that it is in blocking mode.
     *
     * @pre The file handle is open.
     * @post Processes may block when accessing the file.
     *
     * @return <code>vpr::ReturnStatus::Succeed</code> will be returned if the
     *         blocking mode was changed successfully.<br>
     *         <code>vpr::ReturnStatus::Fail</code> will be returned if the
     *         blocking mode could not be changed.
     */
    virtual ReturnStatus
    enableBlocking (void) {
        return m_handle_impl.enableBlocking();
    }

    /**
     * Reconfigures the file handle so that it is in non-blocking mode.
     *
     * @pre The file handle is open.
     * @post Processes will not block when accessing the file.
     *
     * @return <code>vpr::ReturnStatus::Succeed</code> will be returned if the
     *         blocking mode was changed successfully.<br>
     *         <code>vpr::ReturnStatus::Fail</code> will be returned if the
     *         blocking mode could not be changed.
     */
    virtual ReturnStatus
    enableNonBlocking (void) {
        return m_handle_impl.enableNonBlocking();
    }

    /**
     * Gets the current blocking state for the file.
     *
     * @pre <code>m_blocking</code> is set correctly
     * @post
     *
     * @return <code>true</code> is returned when the file is in blocking
     *         mode.<br>
     *         <code>false</code> is returned when the file is in non-blocking
     *         mode.
     */
    inline bool
    getBlocking (void) const {
        return m_handle_impl.getBlocking();
    }

    /**
     * Gets the current non-blocking state for the file.
     *
     * @pre <code>m_blocking</code> is set correctly
     * @post
     *
     * @return <code>true</code> is returned when the file is in non-blocking
     *         mode.<br>
     *         <code>false</code> is returned when the file is in blocking
     *         mode.
     */
    inline bool
    getNonBlocking (void) const {
        return m_handle_impl.getNonBlocking();
    }

    /**
     * Gets the <code>vpr::IOSys::Handle</code> object for this file.
     *
     * @return <code>vpr::IOSys::NullHandle</code> is returned if the file
     *         has no handle or if the handle could not be returned for some
     *         reason.
     */
    virtual IOSys::Handle
    getHandle (void) {
        return m_handle_impl.getHandle();
    }

    // ========================================================================
    // vpr::FileHandle API extensions.
    // ========================================================================

    /**
     * Sets the open flags so that the file handle is opened in read-only mode.
     *
     * @pre None.
     * @post The open flags are updated so that when the device is opened, it
     *       it is opened in read-only mode.  If the device is already open,
     *       this has no effect.
     */
    void
    setOpenReadOnly (void) {
        m_handle_impl.setOpenReadOnly();
    }

    /**
     * Sets the open flags so that the file handle is opened in write-only
     * mode.
     *
     * @pre None.
     * @post The open flags are updated so that when the device is opened, it
     *       is opened in write-only mode.  If the device is already open,
     *       this has no effect.
     */
    void
    setOpenWriteOnly (void) {
        m_handle_impl.setOpenWriteOnly();
    }

    /**
     * Sets the open flags so that the file handle is opened in read/write
     * mode.
     *
     * @pre None.
     * @post The open flags are updated so that when the device is opened, it
     *       is opened in read/write mode.  If the device is already open,
     *       this has no effect.
     */
    void
    setOpenReadWrite (void) {
        m_handle_impl.setOpenReadWrite();
    }

    /**
     * Reconfigures the file handle to be in append mode.
     *
     * @pre The file handle is open.
     * @post The file handle's write mode is set to append.
     *
     * @return <code>vpr::ReturnStatus::Succeed</code> is returned if the write mode
     *         was changed successfully.<br>
     *         <code>vpr::ReturnStatus::Fail</code> is returned otherwise.
     */
    inline ReturnStatus
    enableAppend (void) {
        return m_handle_impl.enableAppend();
    }

    /**
     * Reconfigures the file handle so that it is not in append mode.
     *
     * @pre The file handle is open.
     * @post The file handle's write mode is set so that writes are
     *              appended.
     *
     * @return <code>vpr::ReturnStatus::Succeed</code> is returned if the write mode
     *         was changed successfully.
     *         <code>vpr::ReturnStatus::Fail</code> is returned otherwise.
     */
    inline ReturnStatus
    disableAppend (void) {
        return m_handle_impl.disableAppend();
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle so that writes are synchronous.
    //
    //! PRE: The file handle is open.
    //! POST: Writes are performed synchronously.
    //
    //! RETURNS: vpr::ReturnStatus::Succeed - The write mode was changed
    //+                                 successfully.
    //! RETURNS: vpr::ReturnStatus::Fail - The write mode could not be changed
    //+                                 for some reason.
    // ------------------------------------------------------------------------
    inline ReturnStatus
    enableSynchronousWrite (void) {
        return m_handle_impl.enableSynchronousWrite();
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle so that writes are asynchronous.
    //
    //! PRE: The file handle is open.
    //! POST: Writes are performed asynchronously.
    //
    //! RETURNS: vpr::ReturnStatus::Succeed - The write mode was changed
    //+                                 successfully.
    //! RETURNS: vpr::ReturnStatus::Fail - The write mode could not be changed
    //+                                 for some reason.
    // ------------------------------------------------------------------------
    inline ReturnStatus
    enableAsynchronousWrite (void) {
        return m_handle_impl.enableAsynchronousWrite();
    }

    /**
     * Tests if the file handle is read-only.
     *
     * @pre The file handle is open.
     * @post The access mode is tested for read-only mode, and the result is
     *       returned to the caller.
     *
     * @return <code>true</code> is returned if the device is in read-only
     *         mode; <code>false</code> otherwise.
     */
    inline bool
    isReadOnly (void) {
        return m_handle_impl.isReadOnly();
    }

    /**
     * Tests if the file handle is write-only.
     *
     * @pre The file handle is open.
     * @post The access mode is tested for write-only mode, and the result is
     *       returned to the caller.
     *
     * @return <code>true</code> is returned if the device is in write-only
     *         mode; <code>false</code> otherwise.
     */
    inline bool
    isWriteOnly (void) {
        return m_handle_impl.isWriteOnly();
    }

    /**
     * Tests if the file handle is read/write.
     *
     * @pre The file handle is open.
     * @post The access mode is tested for read/write mode, and the result is
     *       returned to the caller.
     *
     * @return <code>true</code> is returned if the device is in read/write
     *         mode; <code>false</code> otherwise.
     */
    inline bool
    isReadWrite (void) {
        return m_handle_impl.isReadWrite();;
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
    //! RETURNS: vpr::ReturnStatus::Succeed    - The read operation completed
    //+                                    successfully.
    //! RETURNS: vpr::ReturnStatus::Fail    - The read operation failed.
    //! RETURNS: vpr::ReturnStatus::WouldBlock - The handle is in non-blocking mode,
    //+                                    and there is no data to read.
    // ------------------------------------------------------------------------
    virtual ReturnStatus
    read_i (void* buffer, const vpr::Uint32 length, vpr::Uint32& bytes_read,
            const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_handle_impl.read_i(buffer, length, bytes_read, timeout);
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
    //! RETURNS: vpr::ReturnStatus::Succeed - The read operation completed
    //+                                 successfully.
    //! RETURNS: vpr::ReturnStatus::Fail - The read operation failed.
    // ------------------------------------------------------------------------
    virtual ReturnStatus
    readn_i (void* buffer, const vpr::Uint32 length, vpr::Uint32& bytes_read,
             const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_handle_impl.readn_i(buffer, length, bytes_read, timeout);
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
    //! RETURNS: vpr::ReturnStatus::Succeed    - The write operation completed
    //+                                    successfully.
    //! RETURNS: vpr::ReturnStatus::Fail    - The write operation failed.
    //! RETURNS: vpr::ReturnStatus::WouldBlock - The handle is in non-blocking mode,
    //+                                    and the write operation could not
    //+                                    be completed.
    // ------------------------------------------------------------------------
    virtual ReturnStatus
    write_i (const void* buffer, const vpr::Uint32 length,
             vpr::Uint32& bytes_written,
             const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_handle_impl.write_i(buffer, length, bytes_written, timeout);
    }

    /// Platform-specific file hanlde impll
    RealFileHandleImpl m_handle_impl;
};

} // End of vpr namespace


#endif	/* _VPR_FILE_HANDLE_H_ */
