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

#ifndef _VJ_FILE_HANDLE_H_
#define _VJ_FILE_HANDLE_H_

#include <vjConfig.h>

#include <string>
#include <vector>

#include <VPR/IO/vjBlockIO.h>


// ----------------------------------------------------------------------------
//: Extension to the vjBlockIO interface defining a cross-platform file handle
//+ interface.
// ----------------------------------------------------------------------------
//!PUBLIC_API:
class VJ_CLASS_API vjFileHandle : public vjBlockIO {
public:
    // ------------------------------------------------------------------------
    //: Reconfigure the file handle to be in append mode.
    //
    //! PRE: The file handle is open.
    //! POST: The file handle's write mode is set to append.
    //
    //! RETURNS: true  - The write mode was changed successfully.
    //! RETURNS: false - The write mode could not be changed for some reason.
    // ------------------------------------------------------------------------
    virtual bool enableAppend(void) = 0;

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle so that it is not in append mode.
    //
    //! PRE: The file handle is open.
    //! POST: The file handle's write mode is set so that writes are appended.
    //
    //! RETURNS: true  - The write mode was changed successfully.
    //! RETURNS: false - The write mode could not be changed for some reason.
    // ------------------------------------------------------------------------
    virtual bool disableAppend(void) = 0;

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle so that writes are synchronous.
    //
    //! PRE: The file handle is open.
    //! POST: Writes are performed synchronously.
    //
    //! RETURNS: true  - The write mode was changed successfully.
    //! RETURNS: false - The write mode could not be changed for some reason.
    // ------------------------------------------------------------------------
    virtual bool enableSynchronousWrite(void) = 0;

    // ------------------------------------------------------------------------
    //: Reconfigure the file handle so that writes are asynchronous.
    //
    //! PRE: The file handle is open.
    //! POST: Writes are performed asynchronously.
    //
    //! RETURNS: true  - The write mode was changed successfully.
    //! RETURNS: false - The write mode could not be changed for some reason.
    // ------------------------------------------------------------------------
    virtual bool enableAsynchronousWrite(void) = 0;

protected:
    // ------------------------------------------------------------------------
    //: Constructor.
    // ------------------------------------------------------------------------
    vjFileHandle (const std::string& file_name)
        : vjBlockIO(file_name)
    {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    //: Destructor.
    // ------------------------------------------------------------------------
    virtual ~vjFileHandle (void) {
        /* Do nothing. */ ;
    }
};


#endif	/* _VJ_FILE_HANDLE_H_ */
