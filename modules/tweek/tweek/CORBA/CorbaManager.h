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

#ifndef _TWEEK_CORBA_MANAGER_H_
#define _TWEEK_CORBA_MANAGER_H_

#include <tweek/tweekConfig.h>

#include <stdlib.h>
#include <omnithread.h>
#include <omniORB3/CORBA.h>
#include <vpr/Thread/Thread.h>

#include <tweek/CORBA/SubjectManagerImpl.h>


namespace tweek
{

class CorbaManager
{
public:
   /**
    * Default constructor.
    */
   CorbaManager (void) : m_my_thread(NULL)
   {
      /* Do nothing. */ ;
   }

   /**
    * Gets CORBA ready.
    * Creates a thread and runs the CORBA server.
    */
   void init(int argc = 0, char** argv = NULL);

   /**
    * Binds the interface object.
    */
   void registerSubjectManager(tweek::SubjectManagerImpl* mgr);

   const PortableServer::POA_var& getPOA (void)
   {
      return m_poa;
   }

private:
   /**
    * Runs the server.
    */
   void run(void* args);

   vpr::Thread* m_my_thread;

   CORBA::ORB_var m_orb;
   PortableServer::POA_var m_poa;
   PortableServer::ObjectId_var m_subj_mgr_id;
   CosNaming::NamingContext_var m_root_context;
};

} // End of tweek namespace

#endif /* _TWEEK_CORBA_MANAGER_H_ */
