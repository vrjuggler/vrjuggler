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

#ifndef _VJ_SERIAL_PORT_FACTORY_H_
#define _VJ_SERIAL_PORT_FACTORY_H_

#include <string>

#ifdef WIN32
#include <Utils/Port/vjSerialPortImpWin32.h>
#else
#include <Utils/Port/vjSerialPortImpTermios.h>
#endif


// ----------------------------------------------------------------------------
//: Serial port implementation abstract factory.
// ----------------------------------------------------------------------------
class vjSerialPortFactory {
public:
    // ------------------------------------------------------------------------
    //: Create a platform-specific object that is a subclass of
    //+ vjSerialPortImp and return it to the caller.
    //
    //! PRE: None.
    //! POST: A platform-specific implementation of the vjSerialPortImp
    //+       interface is instantiated using the given port name and returned
    //+       to the caller.
    //
    //! ARGS: port_name - The name of the serial port.
    //
    //! RETURNS: A pointer to a vjSerialPortImp object.
    // ------------------------------------------------------------------------
    vjSerialPortImp*
    create (std::string port_name) {
#ifdef WIN32
        return new vjSerialPortImpWin32(port_name);
#else
        return new vjSerialPortImpTermios(port_name);
#endif
    }
};


#endif	/* _VJ_SERIAL_PORT_FACTORY_H_ */
