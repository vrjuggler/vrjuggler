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

#ifndef _VJ_PORT_H_
#define _VJ_PORT_H_

#include <string>

#include <Utils/vjBlockIO.h>


// ----------------------------------------------------------------------------
//: A cross-platform interface to using a computer's I/O ports (serial,
//+ parallel, IR, etc.).
// ----------------------------------------------------------------------------
//! PUBLIC_API:
class vjPort : public vjBlockIO {
public:

protected:
    // ------------------------------------------------------------------------
    //: Constructor.
    //
    //! PRE: None.
    //! POST: The given port name is passed on to the vjBlockIO constructor.
    //
    //! ARGS: port_name - The name of the port in use.
    // ------------------------------------------------------------------------
    vjPort (const std::string& port_name)
        : vjBlockIO(port_name)
    {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    //: Destructor.
    // ------------------------------------------------------------------------
    virtual ~vjPort(void) {
        /* Do nothing. */ ;
    }
};


#endif	/* _VJ_PORT_H_ */
