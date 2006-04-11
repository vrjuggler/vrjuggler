/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _JCCL_RTRC_INTERFACE_H_
#define _JCCL_RTRC_INTERFACE_H_

#include <jccl/jcclConfig.h>

#include <stdlib.h>
#include <string>

#include <tweek/CORBA/CorbaManager.h>


namespace jccl 
{

class RTRCInterfaceSubjectImpl;

/** CORBA RTRC interface object. Handles configuration of the Tweek corba
 *  manager and creation of the interface subject.
 *
 *  @date July 31, 2002
 */

class JCCL_CLASS_API RTRCInterface
{

public:
   RTRCInterface();

   ~RTRCInterface();

   /**
    *  Initialize the RTRC interface
    */
   void init();

   /**
    *  Turn on the interface to RTRC (allow incoming connections)
    */   
   void enable();

   /**
    *  Turn off the interface to RTRC (disallow incoming connections)
    */
   void disable();

private:
   tweek::CorbaManager* mCorbaManager;
   RTRCInterfaceSubjectImpl* mInterface;
   std::string mInterfaceName;

};

} // namespace jccl

#endif
