/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

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

#ifndef _TWEEK_CORBA_REGISTRY_H_
#define _TWEEK_CORBA_REGISTRY_H_

#include <tweek/tweekConfig.h>

#include <stdlib.h>
#include <string>

#include <omnithread.h>

#if OMNIORB_VER == 2
#  include <omniORB2/CORBA.h>
#elif OMNIORB_VER == 3
#  include <omniORB3/CORBA.h>
#endif

#include <vpr/Thread/Thread.h>


namespace tweek
{

class Registry
{
public:
   /**
    * Default constructor.
    */
   Registry();

   /**
    * Gets CORBA ready.
    */
   void initCORBA();

   /**
    * Binds the interface object.
    */
   void addInterface(CORBA::Object_ptr obj, std::string& objectId,
                     std::string& objectKind);

   CORBA::Object_var getInterface(std::string& objectId,
                                  std::string& objectKind);

   CORBA::BOA_ptr boa;

private:
   /**
    * Creates a thread and runs the CORBA server.
    */
   void start();

   /**
    * Runs the server.
    */
   void run(void* args);

   vpr::Thread* m_my_thread;

   CORBA::ORB_var m_orb;

   CosNaming::NamingContext_var m_naming_context;
};

} // End of tweek namespace

#endif /* _TWEEK_CORBA_REGISTRY_H_ */
