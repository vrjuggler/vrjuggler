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

#ifndef _VPR_BLOCK_IO_H_
#define _VPR_BLOCK_IO_H_

#include <vpr/vprConfig.h>

#include <vector>
#include <string>
#include <string.h>

#ifdef VPR_OS_FreeBSD
#include <sys/types.h>
#endif

#include <vpr/Util/Assert.h>
#include <vpr/Util/Status.h>
#include <vpr/Util/Interval.h>
#include <vpr/IO/IOSys.h>


namespace vpr {

/**
 * Block-style (as opposed to streaming) I/O interface.
 *
 * @author Patrick Hartling
 */
class VPR_CLASS_API BlockIO {
public:
    enum _open_mode {
        READ_ONLY,        /**< Open read-only */
        WRITE_ONLY,       /**< Open write-only */
        READ_WRITE        /**< Open read/write */
    };

    /**
     * Gets the name of this I/O device.  The significance of the name depends
     * on the specific device type.
     *
     * <b>PRE:</b> None.<br>
     * <b>POST:</b> A constant reference to the <code>m_name</code> object is
     *              returned to the caller.
     *
     * @return An object containing the name of this device.
     */
    virtual const std::string&
    getName (void) {
        return m_name;
    }

    /**
     * Sets the open flags so that the I/O device is opened in read-only mode.
     *
     * <b>PRE:</b> None.<br>
     * <b>POST:</b> The open flags are updated so that when the device is
     *              opened, it is opened in read-only mode.  If the device is
     *              already open, this has no effect.
     */
    virtual void
    setOpenReadOnly (void) {
        m_open_mode = READ_ONLY;
    }

    /**
     * Sets the open flags so that the I/O device is opened in write-only mode.
     *
     * <b>PRE:</b> None.<br>
     * <b>POST:</b> The open flags are updated so that when the device is
     *              opened, it is opened in write-only mode.  If the device is
     *              already open, this has no effect.
     */
    virtual void
    setOpenWriteOnly (void) {
        m_open_mode = WRITE_ONLY;
    }

    /**
     * Sets the open flags so that the I/O device is opened in read/write mode.
     *
     * <b>PRE:</b> None.<br>
     * <b>POST:</b> The open flags are updated so that when the device is
     *              opened, it is opened in read/write mode.  If the device is
     *              already open, this has no effect.
     */
    virtual void
    setOpenReadWrite (void) {
        m_open_mode = READ_WRITE;
    }

    /**
     * Sets the blocking flags so that the I/O device is opened in blocking
     * mode.
     *
     * <b>PRE:</b> None.<br>
     * <b>POST:</b> The open flags are updated so that when the device is
     *              opened, it is opened in blocking mode.  If the device is
     *              already open, this has no effect.
     */
    virtual void
    setOpenBlocking (void) {
        m_open_blocking = true;
    }

    /**
     * Sets the blocking flags so that the I/O device is opened in
     * non-blocking mode.
     *
     * <b>PRE:</b> None.<br>
     * <b>POST:</b> The open flags are updated so that when the device is
     *              opened, it is opened in non-blocking mode.  If the device
     *              is already open, this has no effect.
     */
    virtual void
    setOpenNonBlocking (void) {
        m_open_blocking = false;
    }

    /**
     * Opens the I/O device.
     *
     * <b>PRE:</b> The device is not already open.<br>
     * <b>POST:</b> An attempt is made to open the I/O device.  The resulting
     *              status is returned to the caller.  If the I/O device is
     *              opened, <code>m_open</code> is set to true.
     *
     * @return <code>vpr::Status::Success</code> is returned if the device is
     *         opened successfully.<br>
     *         <code>vpr::Status::Failure</code> is returned otherwise.
     */
    virtual Status open(void) = 0;

    /**
     * Closes the I/O device.
     *
     * <b>PRE:</b> The device is open.<br>
     * <b>POST:</b> An attempt is made to close the I/O device.  The resulting
     *              status is returned to the caller.  If the I/O device is
     *              closed, <code>m_open</code> is set to false.
     *
     * @return <code>vpr::Status::Success</code> is returned if the device is
     *         closed successfully.<br>
     *         <code>vpr::Status::Failure</code> is returned otherwise.
     */
    virtual Status close(void) = 0;

    /**
     * Gets the open state of this I/O device.
     *
     * <b>PRE:</b> None.<br>
     * <b>POST:</b> The boolean value in m_open is returned to the caller.
     *
     * @return <code>true</code> is returned if the device is open;
     *         <code>false</code> otherwise.
     */
    inline virtual bool
    isOpen (void) {
        return m_open;
    }

    /**
     * Reconfigures the I/O device so that it is in blocking mode.
     *
     * <b>PRE:</b> The device is open.<br>
     * <b>POST:</b> Processes will block when accessing the device.
     *
     * @return <code>vpr::Status::Success</code> is returned if the device's
     *         blocking mode is set to blocking.<br>
     *         <code>vpr::Status::Failure</code> is returned otherwise.
     */
    virtual Status enableBlocking(void) = 0;

    /**
     * Reconfigures the I/O device so that it is in non-blocking mode.
     *
     * <b>PRE:</b> The device is open.<br>
     * <b>POST:</b> Processes will not block when accessing the device.
     *
     * @return <code>vpr::Status::Success</code> is returned if the device's
     *         blocking mode is set to non-blocking.<br>
     *         <code>vpr::Status::Failure</code> is returned otherwise.
     */
    virtual Status enableNonBlocking(void) = 0;

    /**
     * Gets the current blocking state for the I/O device.
     *
     * <b>PRE:</b> <code>m_blocking</code> is set correctly<br>
     * <b>POST:</b>
     *
     * @return <code>true</code> is returned if the device is in blocking mode.
     *         Otherwise, <code>false</code> is returned.
     */
    virtual bool
    getBlocking (void) const {
        return m_blocking;
    }

    /**
     * Gets the current non-blocking state for the I/O device.
     *
     * <b>PRE:</b> m_blocking is set correctly<br>
     * <b>POST:</b>
     *
     * @return <code>true</code> is returned if the device is in non-blocking
     *         mode.  Otherwise, <code>false</code> is returned.
     */
    virtual bool
    getNonBlocking (void) const {
        return (!m_blocking);
    }

    /**
     * Gets the <code>vpr::IOSys::Handle</code> object for this I/O device.
     *
     * @return <code>vpr::IOSys::NullHandle</code> is returned if the device
     *         has no handle or if the handle could not be returned for some
     *         reason.
     *
     */
    virtual IOSys::Handle getHandle(void) = 0;

    /**
     * Reads at most the specified number of bytes from the I/O device into
     * the given buffer.
     *
     * <b>PRE:</b> The device is open for reading, and the buffer is at least
     *             <code>length</code> bytes long.<br>
     * <b>POST:</b> The given buffer has <code>length</code> bytes copied into
     *              it from the device, and the number of bytes read
     *              successfully is returned to the caller via the
     *              <code>bytes_read</code> argument.
     *
     * @param buffer     A pointer to the buffer where the device's buffer
     *                   contents are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to
     *                   <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the read
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Success</code> is returned if the read
     *         operation failed.<br>
     *         <code>vpr::Status::InProgress</code> if the device is in
     *         non-blocking mode, and the read operation is in progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the read
     *         could not begin within the timeout interval.
     */
    Status
    read (void* buffer, const size_t length, ssize_t& bytes_read,
          const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return this->read_i(buffer, length, bytes_read, timeout);
    }

    /**
     * Reads at most the specified number of bytes from the I/O device into
     * the given buffer.
     *
     * <b>PRE:</b> The device is open for reading, and the buffer is at least
     *             <code>length</code> bytes long.<br>
     * <b>POST:</b> The given buffer has <code>length</code> bytes copied into
     *              it from the device, and the number of bytes read
     *              successfully is returned to the caller via the
     *              <code>bytes_read</code> argument.
     *
     * @param buffer     A reference to the buffer (a <code>std::string</code>
     *                   object) where the device's buffer contents are to be
     *                   stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the read
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Success</code> is returned if the read
     *         operation failed.<br>
     *         <code>vpr::Status::InProgress</code> if the device is in
     *         non-blocking mode, and the read operation is in progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the read
     *         could not begin within the timeout interval.
     */
    Status
    read (std::string& buffer, const size_t length, ssize_t& bytes_read,
          const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       Status status;

       // Allocate the temporary buffer, zero it, and read in the current
       // buffer from the device.
       buffer.resize( length );
       memset( &buffer[0], '\0', buffer.size() );
       status = this->read( &buffer[0], buffer.size(), bytes_read, timeout);

       return status;
    }

    /**
     * Reads at most the specified number of bytes from the I/O device into
     * the given buffer.
     *
     * <b>PRE:</b> The device is open for reading, and the buffer is at least
     *             <code>length</code> bytes long.<br>
     * <b>POST:</b> The given buffer has <code>length</code> bytes copied into
     *              it from the device, and the number of bytes read
     *              successfully is returned to the caller via the
     *              <code>bytes_read</code> argument.
     *
     * @param buffer     A pointer to the buffer (a vector of
     *                   <code>char</code>s) where the device's buffer
     *                   contents are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the read
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Success</code> is returned if the read
     *         operation failed.<br>
     *         <code>vpr::Status::InProgress</code> if the device is in
     *         non-blocking mode, and the read operation is in progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the read
     *         could not begin within the timeout interval.
     */
    Status
    read (std::vector<vpr::Uint8>& buffer, const size_t length,
          ssize_t& bytes_read,
          const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       Status status;
       buffer.resize( length );

       // Allocate the temporary buffer, zero it, and read in the current
       // buffer from the device.
       memset( &buffer[0], '\0', buffer.size() );
       status = this->read( &buffer[0], buffer.size(), bytes_read, timeout);

       // size it down if needed, if (bytes_read==length),
       // then resize does nothing...
       if (bytes_read >= 0)
       {
          buffer.resize( bytes_read );
       }

       return status;
    }

    /**
     * Reads exactly the specified number of bytes from the I/O device into
     * the given buffer.
     *
     * <b>PRE:</b> The device is open for reading, and the buffer is at least
     *             <code>length</code> bytes long.<br>
     * <b>POST:</b> The given buffer has <code>length</code> bytes copied into
     *              it from the device, and the number of bytes read
     *              successfully is returned to the caller via the
     *              <code>bytes_read</code> parameter.
     *
     * @param buffer     A pointer to the buffer where the device's buffer
     *                   contents are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the read
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Success</code> is returned if the read
     *         operation failed.<br>
     *         <code>vpr::Status::InProgress</code> if the device is in
     *         non-blocking mode, and the read operation is in progress.
     */
    Status
    readn (void* buffer, const size_t length, ssize_t& bytes_read,
           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return this->readn_i(buffer, length, bytes_read, timeout);
    }

    /**
     * Reads exactly the specified number of bytes from the I/O device into
     * the given buffer.
     *
     * <b>PRE:</b> The device is open for reading, and the buffer is at least
     *             <code>length</code> bytes long.<br>
     * <b>POST:</b> The given buffer has <code>length</code> bytes copied into
     *              it from the device, and the number of bytes read
     *              successfully is returned to the caller via the
     *              <code>bytes_read</code> parameter.
     *
     * @param buffer     A reference to the buffer (a <code>std::string</code>
     *                   object) where the device's buffer contents are to be
     *                   stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the read
     *         operation completed successfully.
     *         <code>vpr::Status::Success</code> is returned if the read
     *         operation failed.  <code>vpr::Status::InProgress</code> if the
     *         device is in non-blocking mode, and the read operation is in
     *         progress.
     */
    Status
    readn (std::string& buffer, const size_t length, ssize_t& bytes_read,
           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       Status status;

       // Allocate the temporary buffer, zero it, and read in the current
       // buffer from the device.
       buffer.resize( length );
       memset( &buffer[0], '\0', buffer.size() );
       status = this->readn( &buffer[0], buffer.size(), bytes_read, timeout);

       // If anything was read into temp_buf, copy it into buffer.
       if ( bytes_read >= 0 ) {
           buffer.resize( bytes_read );
       }

       return status;
    }

    /**
     * Reads exactly the specified number of bytes from the I/O device into
     * the given buffer.
     *
     * <b>PRE:</b> The device is open for reading, and the buffer is at least
     *             <code>length</code> bytes long.<br>
     * <b>POST:</b> The given buffer has <code>length</code> bytes copied into
     *              it from the device, and the number of bytes read
     *              successfully is returned to the caller.
     *
     * @param buffer     A pointer to the buffer (a vector of
     *                   <code>char</code>s) where the device's buffer contents
     *                   are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the read
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Success</code> is returned if the read
     *         operation failed.<br>
     *         <code>vpr::Status::InProgress</code> if the device is in
     *         non-blocking mode, and the read operation is in progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the read
     *         could not begin within the timeout interval.
     */
    Status
    readn (std::vector<vpr::Uint8>& buffer, const size_t length,
           ssize_t& bytes_read,
           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        Status status;

        // Allocate the temporary buffer, zero it, and read in the current
        // buffer from the device.
        buffer.resize( length );
        memset( &buffer[0], '\0', buffer.size() );
        status = this->readn( &buffer[0], buffer.size(), bytes_read, timeout );

        // If anything was read into temp_buf, copy it into buffer.
        if ( bytes_read >= 0 ) {
           buffer.resize(bytes_read);
        }

        return status;
    }

    /**
     * Write the buffer to the I/O device.
     *
     * <b>PRE:</b> The device is open for writing.<br>
     * <b>POST:</b> The given buffer is written to the I/O device, and the
     *              number of bytes written successfully is returned to the
     *              caller via the <code>bytes_written</code> parameter.
     *
     * @param buffer        A pointer to the buffer to be written.
     * @param length        The length of the buffer.
     * @param bytes_written The number of bytes written to the device.
     * @param timeout       The maximum amount of time to wait for data to be
     *                      available for writing.  This argument is optional
     *                      and defaults to
     *                      <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the write
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Failure</code> is returned if the write
     *         operation failed.
     *         <code>vpr::Status::InProgress</code> is returned if the handle
     *         is in non-blocking mode, and the write operation is in
     *         progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the write
     *         could not begin within the timeout interval.
     */
    Status
    write (const void* buffer, const size_t length, ssize_t& bytes_written,
           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return this->write_i(buffer, length, bytes_written,timeout);
    }

    /**
     * Writes the buffer to the I/O device.
     *
     * <b>PRE:</b> The device is open for writing.<br>
     * <b>POST:</b> The given buffer is written to the I/O device, and the
     *              number of bytes written successfully is returned to the
     *              caller via the <code>bytes_written</code> parameter.
     *
     * @param buffer        A reference to the buffer (a std::string object)
     *                      to be written.
     * @param length        The length of the buffer.
     * @param bytes_written The number of bytes written to the device.
     * @param timeout       The maximum amount of time to wait for data to be
     *                      available for writing.  This argument is optional
     *                      and defaults to
     *                      <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the write
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Failure</code> is returned if the write
     *         operation failed.
     *         <code>vpr::Status::InProgress</code> is returned if the handle
     *         is in non-blocking mode, and the write operation is in
     *         progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the write
     *         could not begin within the timeout interval.
     */
    Status
    write (const std::string& buffer, const size_t length,
           ssize_t& bytes_written,
           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       vprASSERT( length <= buffer.size() && "length was bigger than the data given" );
       return this->write(buffer.c_str(), length, bytes_written,timeout);
    }

    /**
     * Writes the buffer to the I/O device.
     *
     * <b>PRE:</b> The device is open for writing.<br>
     * <b>POST:</b> The given buffer is written to the I/O device, and the
     *              number of bytes written successfully is returned to the
     *              caller via the <code>bytes_written</code> parameter.
     *
     * @param buffer        A pointer to the buffer (a vector of chars) to
     *                      be written.
     * @param length        The length of the buffer.
     * @param bytes_written The number of bytes written to the device.
     * @param timeout       The maximum amount of time to wait for data to be
     *                      available for writing.  This argument is optional
     *                      and defaults to
     *                      <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the write
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Failure</code> is returned if the write
     *         operation failed.
     *         <code>vpr::Status::InProgress</code> is returned if the handle
     *         is in non-blocking mode, and the write operation is in
     *         progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the write
     *         could not begin within the timeout interval.
     */
    Status
    write (const std::vector<vpr::Uint8>& buffer, const size_t length,
           ssize_t& bytes_written,
           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        vprASSERT( length <= buffer.size() && "length was bigger than the data given" );
        return this->write(&buffer[0], length, bytes_written,timeout);
    }

    /**
     * Tests if the I/O device is read-only.
     *
     * <b>PRE:</b> The I/O device is open.<br>
     * <b>POST:</b> The access mode is tested for read-only mode, and the
     *              result is returned to the caller.
     *
     * @return <code>true</code> is returned if the device is in read-only
     *         mode; <code>false</code> otherwise.
     */
    inline bool
    isReadOnly (void) {
        return (m_open_mode == READ_ONLY);
    }

    /**
     * Tests if the I/O device is write-only.
     *
     * <b>PRE:</b> The I/O device is open.<br>
     * <b>POST:</b> The access mode is tested for write-only mode, and the
     *              result is returned to the caller.
     *
     * @return <code>true</code> is returned if the device is in write-only
     *         mode; <code>false</code> otherwise.
     */
    inline bool
    isWriteOnly (void) {
        return (m_open_mode == WRITE_ONLY);
    }

    /**
     * Tests if the I/O device is read/write.
     *
     * <b>PRE:</b> The I/O device is open.<br>
     * <b>POST:</b> The access mode is tested for read/write mode, and the
     *              result is returned to the caller.
     *
     * @return <code>true</code> is returned if the device is in read/write
     *         mode; <code>false</code> otherwise.
     */
    inline bool
    isReadWrite (void) {
        return (m_open_mode == READ_WRITE);
    }

    /**
     * Test if reading from this I/O device will block.
     *
     * <b>PRE:</b> getHandle() returns a valid <code>vpr::IOSys::Handle</code>
     *             object.<br>
     * <b>POST:</b>
     *
     * @param timeout Give a timeout period to wait for the device to
     *                unblock for reading.  This is optional and defaults to
     *                <code>vpr::Interval::NoWait</code> which will simply poll
     *                the device and return immediately.
     *
     * @return <code>true</code> is returned if reading will block;
     *         <code>false</code> otherwise.
     */
    bool isReadBlocked(const vpr::Interval& timeout = vpr::Interval::NoWait);

    /**
     * Test if writing to this I/O device will block.
     *
     * <b>PRE:</b> getHandle() returns a valid <code>vpr::IOSys::Handle</code>
     *             object.<br>
     * <b>POST:</b>
     *
     * @param timeout Give a timeout period to wait for the device to
     *                unblock for writing.  This is optional and defaults to
     *                <code>vpr::Interval::NoWait</code> which will simply poll
     *                the device and return immediately.
     *
     * @return <code>true</code> is returned if writing will block;
     *         <code>false</code> otherwise.
     */
    bool isWriteBlocked(const vpr::Interval& timeout = vpr::Interval::NoWait);

protected:
    /**
     * Constructor.
     *
     * <b>PRE:</b> None.<br>
     * <b>POST:</b> The name object is copied into m_name; the open mode is
     *              set to read/write and blocking; the open state is set to
     *              <code>false</code>; and the blocking mode for reads and
     *              writes is set to <code>true</code>.
     *
     * @param name The name for this device.
     */
    BlockIO (const std::string& name)
        : m_name(name), m_open_mode(READ_WRITE), m_open_blocking(true),
          m_open(false), m_blocking(true)
    {
        /* Do nothing. */ ;
    }

    /**
     * Copy constructor.
     *
     * @param other A constant reference to another <code>vpr::BlockIO</code>
     *              object used as the source for the copy.
     */
    BlockIO (const BlockIO& other)
    {
        m_name          = other.m_name;
        m_open_mode     = other.m_open_mode;
        m_open_blocking = other.m_open_blocking;
        m_open          = other.m_open;
        m_blocking      = other.m_blocking;
    }


    /**
     * Destructor.
     *
     * <b>PRE:</b> None.
     * <b>POST:</b> None.
     */
    virtual ~BlockIO (void) {
        /* Do nothing. */ ;
    }

    /**
     * Implementation of the <code>read</code> template method.
     *
     * <b>PRE:</b> The device is open for reading, and the buffer is at least
     *             <code>length</code> bytes long.<br>
     * <b>POST:</b> The given buffer has length bytes copied into it from the
     *              device, and the number of bytes read successfully is
     *              returned to the caller via the <code>bytes_read</code>
     *              parameter.
     *
     * @param buffer     A pointer to the buffer where the device's buffer
     *                   contents are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     *
     * @return <code>vpr::Status::Success</code> is returned if the read
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Success</code> is returned if the read
     *         operation failed.<br>
     *         <code>vpr::Status::InProgress</code> if the device is in
     *         non-blocking mode, and the read operation is in progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the read
     *         could not begin within the timeout interval.
     */
    virtual Status read_i(void* buffer, const size_t length,
                          ssize_t& bytes_read,
                          const vpr::Interval timeout = vpr::Interval::NoTimeout) = 0;

    /**
     * Implementation of the <code>readn</code> template method.
     *
     * <b>PRE:</b> The device is open for reading, and the buffer is at least
     *             <code>length</code> bytes long.<br>
     * <b>POST:</b> The given buffer has <code>length</code> bytes copied into
     *              it from the device, and the number of bytes read
     *              successfully is returned to the caller via the
     *              <code>bytes_read</code> parameter.
     *
     * @param buffer     A pointer to the buffer where the device's buffer
     *                   contents are to be stored.
     * @param length     The number of bytes to be read.  This is optional
     *                   and can be determined from the length of the vector
     *                   if not specified.
     * @param bytes_read The number of bytes read into the buffer.
     *
     * @return <code>vpr::Status::Success</code> is returned if the read
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Success</code> is returned if the read
     *         operation failed.<br>
     *         <code>vpr::Status::InProgress</code> if the device is in
     *         non-blocking mode, and the read operation is in progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the read
     *         could not begin within the timeout interval.
     */
    virtual Status readn_i(void* buffer, const size_t length,
                           ssize_t& bytes_read,
                           const vpr::Interval timeout = vpr::Interval::NoTimeout) = 0;

    /**
     * Writes the buffer to the I/O device.
     *
     * <b>PRE:</b> The device is open for writing.<br>
     * <b>POST:</b> The given buffer is written to the I/O device, and the
     *              number of bytes written successfully is returned to the
     *              caller via the <code>bytes_written</code> parameter.
     *
     * @param buffer        A pointer to the buffer to be written.
     * @param length        The length of the buffer.
     * @param bytes_written The number of bytes written to the device.
     * @param timeout       The maximum amount of time to wait for data to be
     *                      available for writing.  This argument is optional
     *                      and defaults to
     *                      <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the write
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Failure</code> is returned if the write
     *         operation failed.<br>
     *         <code>vpr::Status::InProgress</code> is returned if the handle
     *         is in non-blocking mode, and the write operation is in
     *         progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the write
     *         could not begin within the timeout interval.
     */
    virtual Status write_i(const void* buffer, const size_t length,
                           ssize_t& bytes_written,
                           const vpr::Interval timeout = vpr::Interval::NoTimeout) = 0;
    /// The name of the I/O device.
    std::string m_name; 

    /// The open mode of the device
    _open_mode m_open_mode;

    /// Flag telling if blocking is enabled when opening the device
    bool m_open_blocking;

    /// Flag telling if the device is open
    bool m_open;

    /// Flag telling if blocking for reads and writes is enabled
    bool m_blocking;
};

}; // End of vpr namespace


#endif   /* _VPR_BLOCK_IO_H_ */
