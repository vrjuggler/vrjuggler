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

#ifndef _VJ_BLOCK_IO_H_
#define _VJ_BLOCK_IO_H_

#include <vjConfig.h>

#ifdef __FreeBSD__
#include <sys/types.h>
#endif

#if defined __WIN32__ || defined WIN32 || defined _Windows || defined _WIN32
typedef int ssize_t;
#endif

#include <vector>
#include <string>


// ----------------------------------------------------------------------------
//: Block-style (as opposed to streaming) I/O interface.
// ----------------------------------------------------------------------------
//!PUBLIC_API:
class VJ_CLASS_API vjBlockIO {
public:
    enum _open_mode {
        READ_ONLY,        // Open read-only
        WRITE_ONLY,       // Open write-only
        READ_WRITE        // Open read/write
    };

    // ------------------------------------------------------------------------
    //: Get the name of this I/O device.  The significance of the name depends
    //+ on the specific device type.
    //
    //! PRE: None.
    //! POST: A constant reference to the m_name object is returned to the
    //+       caller.
    //
    //! RETURNS: A constant reference to a std::string object contianing the
    //+          name of this device.
    // ------------------------------------------------------------------------
    inline virtual const std::string&
    getName (void) {
        return m_name;
    }

    // ------------------------------------------------------------------------
    //: Set the open flags so that the I/O device is opened in read-only mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the device is opened, it
    //+       is opened in read-only mode.  If the device is already open,
    //+       this has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenReadOnly (void) {
        m_open_mode = READ_ONLY;
    }

    // ------------------------------------------------------------------------
    //: Set the open flags so that the I/O device is opened in write-only
    //+ mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the device is opened, it
    //+       is opened in write-only mode.  If the device is already open,
    //+       this has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenWriteOnly (void) {
        m_open_mode = WRITE_ONLY;
    }

    // ------------------------------------------------------------------------
    //: Set the open flags so that the I/O device is opened in read/write
    //+ mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the device is opened, it
    //+       is opened in read/write mode.  If the device is already open,
    //+       this has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenReadWrite (void) {
        m_open_mode = READ_WRITE;
    }

    // ------------------------------------------------------------------------
    //: Set the blocking flags so that the I/O device is opened in blocking
    //+ mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the device is opened, it
    //+       is opened in blocking mode.  If the device is already open, this
    //+       has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenBlocking (void) {
        m_open_blocking = true;
    }

    // ------------------------------------------------------------------------
    //: Set the blocking flags so that the I/O device is opened in
    //+ non-blocking mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the device is opened, it
    //+       is opened in non-blocking mode.  If the device is already open,
    //+       this has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenNonBlocking (void) {
        m_open_blocking = false;
    }

    // ------------------------------------------------------------------------
    //: Open the I/O device.
    //
    //! PRE: The device is not already open.
    //! POST: An attempt is made to open the I/O device.  The resulting status
    //+       is returned to the caller.  If the I/O device is opened, m_open
    //+       is set to true.
    //
    //! RETURNS: true  - The device was opened successfully.
    //! RETURNS: false - The device could not be opened for some reason.
    // ------------------------------------------------------------------------
    virtual bool open(void) = 0;

    // ------------------------------------------------------------------------
    //: Close the I/O device.
    //
    //! PRE: The device is open.
    //! POST: An attempt is made to close the I/O device.  The resulting
    //+       status is returned to the caller.  If the I/O device is closed,
    //+       m_open is set to false.
    //
    //! RETURNS: true  - The device was closed successfully.
    //! RETURNS: false - The device could not be closed for some reason.
    // ------------------------------------------------------------------------
    virtual bool close(void) = 0;

    // ------------------------------------------------------------------------
    //: Get the open state of this I/O device.
    //
    //! PRE: None.
    //! POST: The boolean value in m_open is returned to the caller.
    //
    //! RETURNS: true  - The device is in the open state.
    //! RETURNS: false - The device is in the closed state.
    // ------------------------------------------------------------------------
    inline virtual bool
    isOpen (void) {
        return m_open;
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the I/O device so that it is in blocking mode.
    //
    //! PRE: The device is open.
    //! POST: Processes will block when accessing the device.
    //
    //! RETURNS: true  - The blocking mode was changed successfully.
    //! RETURNS: false - The blocking mode could not be changed for some
    //+                  reason.
    // ------------------------------------------------------------------------
    virtual bool enableBlocking(void) = 0;

    // ------------------------------------------------------------------------
    //: Reconfigure the I/O device so that it is in non-blocking mode.
    //
    //! PRE: The device is open.
    //! POST: Processes will not block when accessing the device.
    //
    //! RETURNS: true  - The blocking mode was changed successfully.
    //! RETURNS: false - The blocking mode could not be changed for some
    //+                  reason.
    // ------------------------------------------------------------------------
    virtual bool enableNonBlocking(void) = 0;

    // ------------------------------------------------------------------------
    //: Read at most the specified number of bytes from the I/O device into
    //+ the given buffer.
    //
    //! PRE: The device is open for reading, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       device, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer where the device's buffer
    //+                contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the I/O
    //+                device.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t read(void* buffer, const size_t length) = 0;

    // ------------------------------------------------------------------------
    //: Read at most the specified number of bytes from the I/O device into
    //+ the given buffer.
    //
    //! PRE: The device is open for reading, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       device, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of unsigned chars)
    //+                where the device's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the I/O
    //+                device.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t read(unsigned char* buffer, const size_t length) = 0;

    // ------------------------------------------------------------------------
    //: Read at most the specified number of bytes from the I/O device into
    //+ the given buffer.
    //
    //! PRE: The device is open for reading, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       device, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of chars) where the
    //+                device's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the I/O
    //+                device.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t read(char* buffer, const size_t length) = 0;

    // ------------------------------------------------------------------------
    //: Read at most the specified number of bytes from the I/O device into
    //+ the given buffer.
    //
    //! PRE: The device is open for reading, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       device, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::string object) where
    //+                the device's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.  This is optional and
    //+                can be determined from the length of the string object
    //+                if not specified.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the I/O
    //+                device.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t read(std::string& buffer, const size_t length = 0) = 0;

    // ------------------------------------------------------------------------
    //: Read at most the specified number of bytes from the I/O device into
    //+ the given buffer.
    //
    //! PRE: The device is open for reading, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       device, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (a vector of chars) where the
    //+                device's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.  This is optional and
    //+                can be determined from the length of the vector if not
    //+                specified.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the I/O
    //+                device.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t read(std::vector<char>& buffer,
                         const size_t length = 0) = 0;

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the I/O device into
    //+ the given buffer.
    //
    //! PRE: The device is open for reading, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       device, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer where the device's buffer
    //+                contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the I/O
    //+                device.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t readn(void* buffer, const size_t length) = 0;

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the I/O device into
    //+ the given buffer.
    //
    //! PRE: The device is open for reading, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       device, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of unsigned chars)
    //+                where the device's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the I/O
    //+                device.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t readn(unsigned char* buffer, const size_t length) = 0;

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the I/O device into
    //+ the given buffer.
    //
    //! PRE: The device is open for reading, and the buffer is at least length
    //+       bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       device, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of chars) where the
    //+                device's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the I/O
    //+                device.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t readn(char* buffer, const size_t length) = 0;

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the I/O device into
    //+ the given buffer.
    //
    //! PRE: The device is open for reading, and the buffer is at least length
    //+       bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       device, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::string object) where
    //+                the device's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.  This is optional and
    //+                can be determined from the length of the string object
    //+                if not specified.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the I/O
    //+                device.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t readn(std::string& buffer, const size_t length = 0) = 0;

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the I/O device into
    //+ the given buffer.
    //
    //! PRE: The device is open for reading, and the buffer is at least length
    //+       bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       device, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (a vector of chars) where the
    //+                device's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.  This is optional and
    //+                can be determined from the length of the vector if not
    //+                specified.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the I/O
    //+                device.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual ssize_t readn(std::vector<char>& buffer,
                          const size_t length = 0) = 0;

    // ------------------------------------------------------------------------
    //: Write the buffer to the I/O device.
    //
    //! PRE: The device is open for writing.
    //! POST: The given buffer is written to the I/O device, and the number of
    //+       bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer to be written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the I/O
    //+                device.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    virtual ssize_t write(const void* buffer, const size_t length) = 0;

    // ------------------------------------------------------------------------
    //: Write the buffer to the I/O device.
    //
    //! PRE: The device is open for writing.
    //! POST: The given buffer is written to the I/O device, and the number of
    //+       bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of unsigned chars) to
    //+                be written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the I/O
    //+                device.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    virtual ssize_t write(const unsigned char* buffer, const size_t length) = 0;

    // ------------------------------------------------------------------------
    //: Write the buffer to the I/O device.
    //
    //! PRE: The device is open for writing.
    //! POST: The given buffer is written to the I/O device, and the number of
    //+       bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of chars) to be
    //+                written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the I/O
    //+                device.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    virtual ssize_t write(const char* buffer, const size_t length) = 0;

    // ------------------------------------------------------------------------
    //: Write the buffer to the I/O device.
    //
    //! PRE: The device is open for writing.
    //! POST: The given buffer is written to the I/O device, and the number of
    //+       bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::string object) to be
    //+                written.
    //! ARGS: length - The length of the buffer.  This is optional and can be
    //+                determined from the length of the string object if not
    //+                specified.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the I/O
    //+                device.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    virtual ssize_t write(const std::string& buffer,
                          const size_t length = 0) = 0;

    // ------------------------------------------------------------------------
    //: Write the buffer to the I/O device.
    //
    //! PRE: The device is open for writing.
    //! POST: The given buffer is written to the I/O device, and the number of
    //+       bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer (a vector of chars) to be
    //+                written.
    //! ARGS: length - The length of the buffer.  This is optional and can be
    //+                determined from the length of the vector if not
    //+                specified.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the I/O
    //+                device.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    virtual ssize_t write(const std::vector<char>& buffer,
                          const size_t length = 0) = 0;

    // ------------------------------------------------------------------------
    //: Test if the I/O device is read-only.
    //
    //! PRE: The I/O device is open.
    //! POST: The access mode is tested for read-only mode, and the result is
    //+       returned to the caller.
    //
    //! RETURNS: true  - The device is in read-only mode.
    //! RETURNS: false - The device is not in read-only mode.
    // ------------------------------------------------------------------------
    inline bool
    isReadOnly (void) {
        return (m_open_mode == READ_ONLY);
    }

    // ------------------------------------------------------------------------
    // Test if the I/O device is write-only.
    //
    // PRE: The I/O device is open.
    // POST: The access mode is tested for write-only mode, and the result is
    //       returned to the caller.
    //
    //! RETURNS: true  - The device is in write-only mode.
    //! RETURNS: false - The device is not in write-only mode.
    // ------------------------------------------------------------------------
    inline bool
    isWriteOnly (void) {
        return (m_open_mode == WRITE_ONLY);
    }

    // ------------------------------------------------------------------------
    // Test if the I/O device is read/write.
    //
    // PRE: The I/O device is open.
    // POST: The access mode is tested for read/write mode, and the result is
    //       returned to the caller.
    //
    //! RETURNS: true  - The device is in read/write mode.
    //! RETURNS: false - The device is not in read/write mode.
    // ------------------------------------------------------------------------
    inline bool
    isReadWrite (void) {
        return (m_open_mode == READ_WRITE);
    }

protected:
    // ------------------------------------------------------------------------
    //: Constructor.
    //
    //! PRE: None.
    //! POST: The name object is copied into m_name; the open mode is set to
    //+       read/write and blocking; the open state is set to false; and 
    //+       the blocking mode for reads and writes is set to true.
    //
    //! ARGS: name - The name for this device.
    // ------------------------------------------------------------------------
    vjBlockIO (const std::string& name)
        : m_name(name), m_open_mode(READ_WRITE), m_open_blocking(true),
          m_open(false), m_blocking(true)
    {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    //: Destructor.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    virtual ~vjBlockIO (void) {
        /* Do nothing. */ ;
    }

    std::string m_name;          //: The name of the I/O device
    _open_mode  m_open_mode;     //: The open mode of the device
    bool        m_open_blocking; //: Flag telling if blocking is enabled when
				 //+ opening the device
    bool        m_open;          //: Flag telling if the device is open
    bool        m_blocking;      //: Flag telling if blocking for reads and
				 //+ writes is enabled
};


#endif	/* _VJ_BLOCK_IO_H_ */
