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

class TWEEK_CLASS_API CorbaManager
{
public:
   /**
    * Default constructor.
    */
   CorbaManager (void) : m_my_thread(NULL), m_subj_mgr(NULL)
   {
      /* Do nothing. */ ;
   }

   /**
    * This will destroy the Subject Manager associated with this CORBA
    * Manager (if there is one), and it will shut down the ORB.  These steps
    * may be performed manually using destroySubjectManager() and shutdown()
    * respectively.
    *
    * @post The activated Subject Manager is deactivated, unbound in the Naming
    *       Service, and destroyed.  The ORB is shut down, and its thread
    *       stopped.
    */
   ~CorbaManager (void)
   {
      // If the Subject Manager exists, we need to deactivate it, remove it
      // from the Naming Service, and free its memory.
      if ( m_subj_mgr != NULL )
      {
         destroySubjectManager();
      }

      shutdown();

      if ( m_my_thread != NULL )
      {
         delete m_my_thread;
         m_my_thread = NULL;
      }
   }

   /**
    * Initializes the ORB and POA associated with this object.  A child POA of
    * the root POA (RootPOA) is created, and all servants registered with this
    * manager are activated within that child POA.
    *
    * @param local_id A string providing a unique identifier for the local POA.
    * @param argc     The size of the following argument vector.  This will be
    *                 modified if any elements are removed from argv.
    * @param argv     The command-line arguments passed to the application.
    *                 These may include parameters defining the ORB's
    *                 behavior.  Those recognized as ORB arguments are removed
    *                 from the array leaving application parameters.
    */
   vpr::ReturnStatus init(const std::string& local_id, int& argc, char** argv);

   /**
    * Shuts down the ORB and the POA (if they were successfully initialized).
    *
    * @post If the ORB and root POA were initialized successfully in init(),
    *       they are destroyed and shut down.
    *
    * @param wait_for_completion If true, block until all pending requests and
    *                            events are completed.  This parameter is
    *                            optional and defaults to true.
    */
   void shutdown (bool wait_for_completion = true);

   /**
    * Checks the validity of this service object to ensure that initialization
    * completed successfully.
    *
    * @return true if init() the ORB and POA references were initialized
    *         successfully.
    */
   bool isValid (void) const
   {
      return ! (CORBA::is_nil(m_orb) || CORBA::is_nil(m_root_poa));
   }

   /**
    * Binds the interface object.
    */
   vpr::ReturnStatus createSubjectManager(void);

   /**
    * Removes the Subject Manager created for use with this CORBA Manager
    * object.  The servant memory is released, the servant is deactivated in
    * the local POA, and the reference is unbound from the Naming Service.
    * All of this is done if and only if the Subject Manager was created
    * successfully in createSubjectManager().
    *
    * @return vpr::ReturnStatus::Fail will be returned if the servant could not
    *         be destroyed successfully.
    */
   vpr::ReturnStatus destroySubjectManager(void);

   /**
    * Returns this CORBA managaer's SubjectManagerImpl instance to the caller.
    * Users will need this so that they may register subjects.
    */
   tweek::SubjectManagerImpl* getSubjectManager (void) const
   {
      return m_subj_mgr;
   }

   const PortableServer::POA_var& getRootPOA (void) const
   {
      return m_root_poa;
   }

   const PortableServer::POA_var& getChildPOA (void) const
   {
      return m_child_poa;
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
   CosNaming::NamingContext_var m_root_context;
   CosNaming::NamingContext_var m_local_context;

   tweek::SubjectManagerImpl*   m_subj_mgr;
   PortableServer::ObjectId_var m_subj_mgr_id;
   CosNaming::Name              m_subj_mgr_name;
};

} // End of tweek namespace

#endif /* _TWEEK_CORBA_MANAGER_H_ */
