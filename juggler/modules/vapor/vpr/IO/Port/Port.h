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

#ifndef _VPR_PORT_H_
#define _VPR_PORT_H_

#include <vpr/vprConfig.h>

#include <string>

#include <vpr/IO/BlockIO.h>


namespace vpr {

/**
 * A cross-platform interface to using a computer's I/O ports (serial,
 * parallel, IR, etc.).
 */
class VPR_CLASS_API Port : public BlockIO {
public:

protected:
    /**
     * Constructor.
     *
     * <b>PRE:</b> None.
     * <b>POST:</b> The given port name is passed on to the
     *              <code>vpr::BlockIO</code> constructor.
     *
     * @param port_name The name of the port in use.
     */
    Port(const std::string& port_name);

    /**
     * Destructor.  This does nothing.
     */
    virtual ~Port(void);
};

}; // End of vpr namespace


#endif	/* _VPR_PORT_H_ */
