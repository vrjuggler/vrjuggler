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


#ifndef _VJ_TIMED_UPDATE_H_
#define _VJ_TIMED_UPDATE_H_

#include <vjConfig.h>


//------------------------------------------------
//: Parent class for any object whose value can be read periodically by EM
//
//       Right now, the only such objects are vjPerfDataBuffer
//       Additional classes may include an Input reader object...
//
// @author  Christopher Just
//
// Date: 6-9-98
//------------------------------------------------

class vjTimedUpdate {

    public:

    //: writes the current contents of this vjTimedUpdate to out
    virtual void write (std::ostream& out) = 0;
    
    //: gets a name associated with this object
    virtual const std::string& getName () const = 0;
    
    //: gets a name of a protocol handler associated with this object
    //  This is used by the network communication system.
    virtual const std::string& getProtocolHandlerName() const = 0;

};


#endif
