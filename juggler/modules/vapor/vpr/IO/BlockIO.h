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

#include <vpr/IO/Stats/BaseIOStatsStrategy.h>


namespace vpr {


/**
 * Block-style (as opposed to streaming) I/O interface.
 *
 * @author Patrick Hartling
 */
class VPR_CLASS_API BlockIO {
public:
    /**
     * Gets the name of this I/O device.  The significance of the name depends
     * on the specific device type.
     *
     * @pre None.
     * @post A constant reference to the <code>m_name</code> object is
     *       returned to the caller.
     *
     * @return An object containing the name of this device.
     */
    virtual const std::string&
    getName (void) {
        return m_name;
    }

    /**
     * Sets the blocking flags so that the I/O device is opened in blocking
     * mode.
     *
     * @pre None.
     * @post The open flags are updated so that when the device is opened,
     *       is opened in blocking mode.  If the device is already open,
     *       this has no effect.
     */
    virtual void
    setOpenBlocking (void) {
        m_open_blocking = true;
    }

    /**
     * Sets the blocking flags so that the I/O device is opened in
     * non-blocking mode.
     *
     * @pre None.
     * @post The open flags are updated so that when the device is opened, it
     *       is opened in non-blocking mode.  If the device is already open,
     *       this has no effect.
     */
    virtual void
    setOpenNonBlocking (void) {
        m_open_blocking = false;
    }

    /**
     * Opens the I/O device.
     *
     * @pre The device is not already open.
     * @post An attempt is made to open the I/O device.  The resulting status
     *       is returned to the caller.  If the I/O device is opened,
     *       <code>m_open</code> is set to true.
     *
     * @return vpr::Status::Success is returned if the device is opened
     *         successfully.<br>
     *         vpr::Status::Failure is returned otherwise.
     */
    virtual Status open(void) = 0;

    /**
     * Closes the I/O device.
     *
     * @pre The device is open.
     * @post An attempt is made to close the I/O device.  The resulting status
     *       is returned to the caller.  If the I/O device is closed,
     *       <code>m_open</code> is set to false.
     *
     * @return vpr::Status::Success is returned if the device is closed
     *         successfully.<br>
     *         vpr::Status::Failure is returned otherwise.
     */
    virtual Status close(void) = 0;

    /**
     * Gets the open state of this I/O device.
     *
     * @pre None.
     * @post The boolean value in m_open is returned to the caller.
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
     * @pre The device is open.
     * @post Processes will block when accessing the device.
     *
     * @return vpr::Status::Success is returned if the device's blocking mode
     *         is set to blocking.<br>
     *         vpr::Status::Failure is returned otherwise.
     */
    virtual Status enableBlocking(void) = 0;

    /**
     * Reconfigures the I/O device so that it is in non-blocking mode.
     *
     * @pre The device is open.
     * @post Processes will not block when accessing the device.
     *
     * @return vpr::Status::Success is returned if the device's blocking mode
     *         is set to non-blocking.<br>
     *         vpr::Status::Failure is returned otherwise.
     */
    virtual Status enableNonBlocking(void) = 0;

    /**
     * Gets the current blocking state for the I/O device.
     *
     * @pre <code>m_blocking</code> is set correctly.
     * @post
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
     * @pre m_blocking is set correctly.
     * @post
     *
     * @return <code>true</code> is returned if the device is in non-blocking
     *         mode.  Otherwise, <code>false</code> is returned.
     */
    virtual bool
    getNonBlocking (void) const {
        return (!m_blocking);
    }

    /**
     * Gets the vpr::IOSys::Handle object for this I/O device.
     *
     * @return vpr::IOSys::NullHandle is returned if the device
     *         has no handle or if the handle could not be returned for some
     *         reason.
     *
     */
    virtual IOSys::Handle getHandle(void) = 0;

    /**
     * Reads at most the specified number of bytes from the I/O device into
     * the given buffer.
     *
     * @pre The device is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has <code>length</code> bytes copied into it from
     *       the device, and the number of bytes read successfully is returned
     *       to the caller via the <code>bytes_read</code> argument.
     *
     * @param buffer     A pointer to the buffer where the device's buffer
     *                   contents are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to vpr::Interval::NoTimeout.
     *
     * @return vpr::Status::Success is returned if the read operation
     *         completed successfully.<br>
     *         vpr::Status::WouldBlock if the device is in non-blocking mode,
     *         and there is no data to read.<br>
     *         vpr::Status::Timeout is returned if the read could not begin
     *         within the timeout interval.<br>
     *         vpr::Status::Failure is returned if the read operation failed.
     */
    Status
    read (void* buffer, const vpr::Uint32 length, vpr::Uint32& bytes_read,
          const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return this->read_s(buffer, length, bytes_read, timeout);
    }

    /**
     * Reads at most the specified number of bytes from the I/O device into
     * the given buffer.
     *
     * @pre The device is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has <code>length</code> bytes copied into it
     *       from the device, and the number of bytes read successfully is
     *       returned to the caller via the <code>bytes_read</code> argument.
     *
     * @param buffer     A reference to the buffer (a <code>std::string</code>
     *                   object) where the device's buffer contents are to be
     *                   stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to vpr::Interval::NoTimeout.
     *
     * @return vpr::Status::Success is returned if the read operation completed
     *         successfully.<br>
     *         vpr::Status::WouldBlock if the device is in non-blocking mode,
     *         and there is no data to read.<br>
     *         vpr::Status::Timeout is returned if the read could not begin
     *         within the timeout interval.<br>
     *         vpr::Status::Failure is returned if the read operation failed.
     */
    Status
    read (std::string& buffer, const vpr::Uint32 length,
          vpr::Uint32& bytes_read,
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
     * @pre The device is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has <code>length</code> bytes copied into it
     *       from the device, and the number of bytes read successfully is
     *       returned to the caller via the <code>bytes_read</code> argument.
     *
     * @param buffer     A pointer to the buffer (a vector of
     *                   <code>char</code>s) where the device's buffer
     *                   contents are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to vpr::Interval::NoTimeout.
     *
     * @return vpr::Status::Success is returned if the read operation completed
     *         successfully.<br>
     *         vpr::Status::WouldBlock if the device is in non-blocking mode,
     *         and there is no data to read.<br>
     *         vpr::Status::Timeout is returned if the read could not begin
     *         within the timeout interval.<br>
     *         vpr::Status::Failure is returned if the read operation failed.
     */
    Status
    read (std::vector<vpr::Uint8>& buffer, const vpr::Uint32 length,
          vpr::Uint32& bytes_read,
          const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       Status status;
       buffer.resize( length );

       // Allocate the temporary buffer, zero it, and read in the current
       // buffer from the device.
       memset( &buffer[0], '\0', buffer.size() );
       status = this->read( &buffer[0], buffer.size(), bytes_read, timeout);

       // size it down if needed, if (bytes_read==length), then resize does
       // nothing...
       buffer.resize( bytes_read );

       return status;
    }

    /**
     * Reads exactly the specified number of bytes from the I/O device into
     * the given buffer.
     *
     * @pre The device is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has <code>length</code> bytes copied into it from
     *       the device, and the number of bytes read successfully is returned
     *       to the caller via the <code>bytes_read</code> parameter.
     *
     * @param buffer     A pointer to the buffer where the device's buffer
     *                   contents are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to vpr::Interval::NoTimeout.
     *
     * @return vpr::Status::Success is returned if the read operation completed
     *         successfully.<br>
     *         vpr::Status::WouldBlock if the device is in non-blocking mode,
     *         and there is no data to read.<br>
     *         vpr::Status::Failure is returned if the read operation failed.
     */
    Status
    readn (void* buffer, const vpr::Uint32 length, vpr::Uint32& bytes_read,
           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return this->readn_s(buffer, length, bytes_read, timeout);
    }

    /**
     * Reads exactly the specified number of bytes from the I/O device into
     * the given buffer.
     *
     * @pre The device is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has <code>length</code> bytes copied into it from
     *       the device, and the number of bytes read successfully is returned
     *       to the caller via the <code>bytes_read</code> parameter.
     *
     * @param buffer     A reference to the buffer (a <code>std::string</code>
     *                   object) where the device's buffer contents are to be
     *                   stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to vpr::Interval::NoTimeout.
     *
     * @return vpr::Status::Success is returned if the read
     *         operation completed successfully.<br>
     *         vpr::Status::WouldBlock if the device is in non-blocking mode,
     *         and there is no data to read.<br>
     *         vpr::Status::Failure is returned if the read operation failed.
     */
    Status
    readn (std::string& buffer, const vpr::Uint32 length,
           vpr::Uint32& bytes_read,
           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       Status status;

       // Allocate the temporary buffer, zero it, and read in the current
       // buffer from the device.
       buffer.resize( length );
       memset( &buffer[0], '\0', buffer.size() );
       status = this->readn( &buffer[0], buffer.size(), bytes_read, timeout);
       buffer.resize( bytes_read );

       return status;
    }

    /**
     * Reads exactly the specified number of bytes from the I/O device into
     * the given buffer.
     *
     * @pre The device is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has <code>length</code> bytes copied into it from
     *       the device, and the number of bytes read successfully is returned
     *       to the caller.
     *
     * @param buffer     A pointer to the buffer (a vector of
     *                   <code>char</code>s) where the device's buffer contents
     *                   are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to vpr::Interval::NoTimeout.
     *
     * @return vpr::Status::Success is returned if the read operation completed
     *         successfully.<br>
     *         vpr::Status::WouldBlock if the device is in non-blocking mode,
     *         and there is no data to read.<br>
     *         vpr::Status::Timeout is returned if the read could not begin
     *         within the timeout interval.<br>
     *         vpr::Status::Failure is returned if the read operation failed.
     */
    Status
    readn (std::vector<vpr::Uint8>& buffer, const vpr::Uint32 length,
           vpr::Uint32& bytes_read,
           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        Status status;

        // Allocate the temporary buffer, zero it, and read in the current
        // buffer from the device.
        buffer.resize( length );
        memset( &buffer[0], '\0', buffer.size() );
        status = this->readn( &buffer[0], buffer.size(), bytes_read, timeout );
        buffer.resize(bytes_read);

        return status;
    }

    /**
     * Write the buffer to the I/O device.
     *
     * @pre The device is open for writing.
     * @post The given buffer is written to the I/O device, and the number of
     *       bytes written successfully is returned to the caller via the
     *       <code>bytes_written</code> parameter.
     *
     * @param buffer        A pointer to the buffer to be written.
     * @param length        The length of the buffer.
     * @param bytes_written The number of bytes written to the device.
     * @param timeout       The maximum amount of time to wait for data to be
     *                      available for writing.  This argument is optional
     *                      and defaults to vpr::Interval::NoTimeout.
     *
     * @return vpr::Status::Success is returned if the write operation
     *         completed successfully.<br>
     *         vpr::Status::WouldBlock is returned if the handle is in
     *         non-blocking mode, and the write operation could not be
     *         completed.<br>
     *         vpr::Status::Timeout is returned if the write could not begin
     *         within the timeout interval.<br>
     *         vpr::Status::Failure is returned if the write operation failed.
     */
    Status
    write (const void* buffer, const vpr::Uint32 length,
           vpr::Uint32& bytes_written,
           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return this->write_s(buffer, length, bytes_written,timeout);
    }

    /**
     * Writes the buffer to the I/O device.
     *
     * @pre The device is open for writing.
     * @post The given buffer is written to the I/O device, and the number of
     *       bytes written successfully is returned to the caller via the
     *       <code>bytes_written</code> parameter.
     *
     * @param buffer        A reference to the buffer (a std::string object)
     *                      to be written.
     * @param length        The length of the buffer.
     * @param bytes_written The number of bytes written to the device.
     * @param timeout       The maximum amount of time to wait for data to be
     *                      available for writing.  This argument is optional
     *                      and defaults to vpr::Interval::NoTimeout.
     *
     * @return vpr::Status::Success is returned if the write operation
     *         completed successfully.<br>
     *         vpr::Status::WouldBlock is returned if the handle is in
     *         non-blocking mode, and the write operation could not be
     *         completed.<br>
     *         vpr::Status::Timeout is returned if the write could not begin
     *         within the timeout interval.<br>
     *         vpr::Status::Failure is returned if the write operation failed.
     */
    Status
    write (const std::string& buffer, const vpr::Uint32 length,
           vpr::Uint32& bytes_written,
           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       vprASSERT( length <= buffer.size() && "length was bigger than the data given" );
       return this->write(buffer.c_str(), length, bytes_written,timeout);
    }

    /**
     * Writes the buffer to the I/O device.
     *
     * @pre The device is open for writing.
     * @post The given buffer is written to the I/O device, and the number of
     *       bytes written successfully is returned to the caller via the
     *       <code>bytes_written</code> parameter.
     *
     * @param buffer        A pointer to the buffer (a vector of chars) to
     *                      be written.
     * @param length        The length of the buffer.
     * @param bytes_written The number of bytes written to the device.
     * @param timeout       The maximum amount of time to wait for data to be
     *                      available for writing.  This argument is optional
     *                      and defaults to vpr::Interval::NoTimeout.
     *
     * @return vpr::Status::Success is returned if the write operation
     *         completed successfully.<br>
     *         vpr::Status::WouldBlock is returned if the handle is in
     *         non-blocking mode, and the write operation could not be
     *         completed.<br>
     *         vpr::Status::Timeout is returned if the write could not begin
     *         within the timeout interval.<br>
     *         vpr::Status::Failure is returned if the write operation failed.
     */
    Status
    write (const std::vector<vpr::Uint8>& buffer, const vpr::Uint32 length,
           vpr::Uint32& bytes_written,
           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        vprASSERT( length <= buffer.size() && "length was bigger than the data given" );
        return this->write(&buffer[0], length, bytes_written,timeout);
    }

    /**
     * Test if reading from this I/O device will block.
     *
     * @pre getHandle() returns a valid vpr::IOSys::Handle object.
     * @post
     *
     * @param timeout Give a timeout period to wait for the device to
     *                unblock for reading.  This is optional and defaults to
     *                vpr::Interval::NoWait which will simply poll the device
     *                and return immediately.
     *
     * @return <code>true</code> is returned if reading will block;
     *         <code>false</code> otherwise.
     */
    bool isReadBlocked(const vpr::Interval& timeout = vpr::Interval::NoWait);

    /**
     * Test if writing to this I/O device will block.
     *
     * @pre getHandle() returns a valid vpr::IOSys::Handle object.
     * @post
     *
     * @param timeout Give a timeout period to wait for the device to
     *                unblock for writing.  This is optional and defaults to
     *                vpr::Interval::NoWait which will simply poll the device
     *                and return immediately.
     *
     * @return <code>true</code> is returned if writing will block;
     *         <code>false</code> otherwise.
     */
    bool isWriteBlocked(const vpr::Interval& timeout = vpr::Interval::NoWait);

    /**
     * Set the IO stats strategy to use
     */
    void setIOStatStrategy(vpr::BaseIOStatsStrategy* strat)
    {
        mStatsStrategy = strat;
        if(mStatsStrategy != NULL)                      // If we have a non-NULL strategy
            mStatsStrategy->setRealObject(this);        // Tell it about us
    }

    /**
     * Get the current IO stats strategy
     */
    vpr::BaseIOStatsStrategy* getIOStatStrategy()
    { return mStatsStrategy; }    

protected:
    /**
     * Constructor.
     *
     * @pre None.
     * @post The name object is copied into m_name; the open mode is set to
     *       blocking; the open state is set to <code>false</code>; and the
     *       blocking mode for reads and writes is set to <code>true</code>.
     *
     * @param name The name for this device.
     */
    BlockIO (const std::string& name)
        : m_name(name), m_open_blocking(true), m_open(false), m_blocking(true), mStatsStrategy(NULL)
    {
        /* Do nothing. */ ;
    }

    /**
     * Copy constructor.
     *
     * @param other A constant reference to another vpr::BlockIO object used
     *              as the source for the copy.
     */
    BlockIO (const BlockIO& other)
    {
        m_name          = other.m_name;
        m_open_blocking = other.m_open_blocking;
        m_open          = other.m_open;
        m_blocking      = other.m_blocking;
        mStatsStrategy  = NULL;
    }


    /**
     * Destructor.
     *
     * @pre None.
     * @post None.
     */
    virtual ~BlockIO (void) {
        /* Do nothing. */ ;
    }

    /**
     * read strategy
     */
    virtual Status read_s(void* buffer, const vpr::Uint32 length,
                          vpr::Uint32& bytes_read,
                          const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       Status status;

       if(mStatsStrategy != NULL)
          mStatsStrategy->read_s(status, buffer, length, bytes_read, timeout);
       else
         status = read_i(buffer, length, bytes_read, timeout);

       return status;
    }
    
    /**
     * read strategy
     */
    virtual Status readn_s(void* buffer, const vpr::Uint32 length,
                           vpr::Uint32& bytes_read,
                           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       Status status;

       if(mStatsStrategy != NULL)
         mStatsStrategy->readn_s(status, buffer, length, bytes_read, timeout);
       else
         status = readn_i(buffer, length, bytes_read, timeout);

       return status;
    }

    /**
     * write strategy
     */
    virtual Status write_s(const void* buffer, const vpr::Uint32 length,
                           vpr::Uint32& bytes_written,
                           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       Status status;

       if(mStatsStrategy != NULL)
         mStatsStrategy->write_s(status, buffer, length, bytes_written, timeout);          
       else
          status = write_i(buffer, length, bytes_written, timeout);

       return status;
    }


    /**
     * Implementation of the read template method.  This reads
     * at most the specified number of bytes from the I/O device into the
     * given buffer.
     *
     * @pre The device is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has length bytes copied into it from the device,
     *       and the number of bytes read successfully is returned to the
     *       caller via the <code>bytes_read</code> parameter.
     *
     * @param buffer     A pointer to the buffer where the device's buffer
     *                   contents are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional and
     *                   defaults to vpr::Interval::NoTimeout.
     *
     * @return vpr::Status::Success is returned if the read operation
     *         completed successfully.<br>
     *         vpr::Status::WouldBlock if the device is in non-blocking mode,
     *         and there is no data to read.<br>
     *         vpr::Status::Timeout is returned if the read could not begin
     *         within the timeout interval.<br>
     *         vpr::Status::Failure is returned if the read operation failed.
     */
    virtual Status read_i(void* buffer, const vpr::Uint32 length,
                          vpr::Uint32& bytes_read,
                          const vpr::Interval timeout = vpr::Interval::NoTimeout) = 0;

    /**
     * Implementation of the <code>readn</code> template method.  This reads
     * exactly the specified number of bytes from the I/O device into the
     * given buffer.
     *
     * @pre The device is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has <code>length</code> bytes copied into
     *       it from the device, and the number of bytes read successfully
     *       is returned to the caller via the <code>bytes_read</code>
     *       parameter.
     *
     * @param buffer     A pointer to the buffer where the device's buffer
     *                   contents are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional and
     *                   defaults to vpr::Interval::NoTimeout.
     *
     * @return vpr::Status::Success is returned if the read
     *         operation completed successfully.<br>
     *         vpr::Status::Failure is returned if the read
     *         operation failed.<br>
     *         vpr::Status::WouldBlock if the device is in non-blocking mode,
     *         and there is no data to read.<br>
     *         vpr::Status::Timeout is returned if the read
     *         could not begin within the timeoBaseIOStatsStrategyut interval.
     */
    virtual Status readn_i(void* buffer, const vpr::Uint32 length,
                           vpr::Uint32& bytes_read,
                           const vpr::Interval timeout = vpr::Interval::NoTimeout) = 0;

    /**
     * Implementation of the <code>write</code> template method.  This writes
     * the given buffer to the I/O device.
     *
     * @pre The device is open for writing.
     * @post The given buffer is written to the I/O device, and the number
     *       of bytes written successfully is returned to the caller via the
     *       <code>bytes_written</code> parameter.
     *
     * @param buffer        A pointer to the buffer to be written.
     * @param length        The length of the buffer.
     * @param bytes_written The number of bytes written to the device.
     * @param timeout       The maximum amount of time to wait for data to be
     *                      available for writing.  This argument is optional
     *                      and defaults to vpr::Interval::NoTimeout.
     *
     * @return vpr::Status::Success is returned if the write operation
     *         completed successfully.<br>
     *         vpr::Status::WouldBlock is returned if the handle is in
     *         non-blocking mode, and the write operation could not
     *         complete.<br>
     *         vpr::Status::Timeout is returned if the write could not begin
     *         within the timeout interval.<br>
     *         vpr::Status::Failure is returned if the write operation failed.
     */
    virtual Status write_i(const void* buffer, const vpr::Uint32 length,
                           vpr::Uint32& bytes_written,
                           const vpr::Interval timeout = vpr::Interval::NoTimeout) = 0;

   // Friends
   friend vpr::BaseIOStatsStrategy;    // Need it to be able to call the protected read_i, readn_i, and write_i memebers

protected:
        /// The name of the I/O device.
    std::string m_name; 

    /// Flag telling if blocking is enabled when opening the device
    bool m_open_blocking;

    /// Flag telling if the device is open
    bool m_open;

    /// Flag telling if blocking for reads and writes is enabled
    bool m_blocking;

    // Perf monitor
    vpr::BaseIOStatsStrategy*   mStatsStrategy;
};

}; // End of vpr namespace


#endif   /* _VPR_BLOCK_IO_H_ */
