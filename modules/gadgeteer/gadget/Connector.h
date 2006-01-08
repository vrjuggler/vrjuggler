/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _GADGET_CONNECTOR_H
#define _GADGET_CONNECTOR_H

#include <gadget/gadgetConfig.h>

namespace vpr
{
   class ReturnStatus;
}

namespace gadget
{
class Node;

/** \class Connector Connector.h gadget/Connector.h
 *
 * Simple connector type.
 */
class GADGET_CLASS_API Connector
{
private:
   /**
    * Hide copy constructor and assignment operator.
    */
   Connector(const Connector&)
   {;}
   void operator=(const Connector&)
   {;}
public:
   Connector()
   {;}
   virtual ~Connector()
   {;}
   
   /**
    * Attempt to connect to the given node.
    */
   vpr::ReturnStatus attemptConnect( Node* node );
};

} // end namespace gadget

#endif /* _GADGET_CONNECTOR_H */
