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
#include <string>
#include <omnithread.h>
#include <omniORB3/CORBA.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Util/ReturnStatus.h>

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
   vpr::ReturnStatus init(const std::string& local_id, int argc = 0,
                          char** argv = NULL);

   /**
    * Checks the validity of this service object to ensure that initialization
    * completed successfully.
    *
    * @return true if init() the ORB and POA references were initialized
    *         successfully.
    */
   bool isValid (void)
   {
      return ! (CORBA::is_nil(m_orb) || CORBA::is_nil(m_root_poa));
   }

   /**
    * Binds the interface object.
    */
   vpr::ReturnStatus registerSubjectManager(tweek::SubjectManagerImpl* mgr);

   const PortableServer::POA_var& getRootPOA (void)
   {
      return m_root_poa;
   }

private:
   vpr::ReturnStatus createChildPOA(const std::string& local_id);

   /**
    * Gets the root context for the naming service and ensures that a
    * sub-context exists for this memory space.
    */
   vpr::ReturnStatus initNamingService(const std::string& ref_name,
                                       const std::string& local_id);

   /**
    * Runs the server.
    */
   void run(void* args);

   vpr::Thread* m_my_thread;

   CORBA::ORB_var m_orb;
   PortableServer::POA_var m_root_poa;
   PortableServer::POA_var m_child_poa;
   PortableServer::ObjectId_var m_subj_mgr_id;
   CosNaming::NamingContext_var m_root_context;
   CosNaming::NamingContext_var m_local_context;
};

} // End of tweek namespace

#endif /* _TWEEK_CORBA_MANAGER_H_ */
