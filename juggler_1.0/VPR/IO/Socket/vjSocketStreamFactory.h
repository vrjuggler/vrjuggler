/*************** <auto-copyright.pl BEGIN do not edit this line> ************** *
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

#ifndef _VJ_SOCKET_STREAM_FACTORY_H_
#define _VJ_SOCKET_STREAM_FACTORY_H_

#include <vjConfig.h>

#include <string>

#ifdef WIN32
#include <VPR/md/WIN32/vjSocketStreamImpWinSock.h>
#else
#include <VPR/md/POSIX/vjSocketStreamImpBSD.h>
#endif


// ----------------------------------------------------------------------------
//: Stream socket implementation abstract factory.
// ----------------------------------------------------------------------------
class VJ_CLASS_API vjSocketStreamFactory {
public:
    // ------------------------------------------------------------------------
    //: Create a platform-specific object that is a subclass of
    //+ vjSocketStreamImp and return it to the caller.
    //
    //! PRE: None.
    //! POST: A platform-specific implementation of the vjSocketStreamImp
    //+       interface is instantiated.
    //
    //! RETURNS: A pointer to a vjSocketStreamImp object.
    // ------------------------------------------------------------------------
    vjSocketStreamImp*
    create (void) {
#ifdef WIN32
        return new vjSocketStreamImpWinSock();
#else
        return new vjSocketStreamImpBSD();
#endif
    }

    // ------------------------------------------------------------------------
    //: Create a platform-specific object that is a subclass of
    //+ vjSocketStreamImp and return it to the caller.
    //
    //! PRE: None.
    //! POST: A platform-specific implementation of the vjSocketStreamImp
    //+       interface is instantiated using the given address, port number
    //+       and domain and is returned to the caller.
    //
    //! ARGS: port_name - The name of the serial port.
    //
    //! RETURNS: A pointer to a vjSocketStreamImp object.
    // ------------------------------------------------------------------------
    vjSocketStreamImp*
    create (const std::string& address, const unsigned short port,
            const vjSocketTypes::Domain domain)
    {
#ifdef WIN32
        return new vjSocketStreamImpWinSock(address, port, domain);
#else
        return new vjSocketStreamImpBSD(address, port, domain);
#endif
    }
};


#endif	/* _VJ_SOCKET_STREAM_FACTORY_H_ */
