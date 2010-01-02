/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _TWEEK_CORBA_SERVICE_H_
#define _TWEEK_CORBA_SERVICE_H_

#include <tweek/tweekConfig.h>

#include <string>
#include <list>
#include TWEEK_INCLUDE_CORBA_H

#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <tweek/idl/SubjectManager.h>


namespace tweek
{

/** \class CorbaService CorbaService.h tweek/Client/CorbaService.h
 *
 * This is a convenience class that wraps all CORBA functionality that will be
 * needed by Tweek client code.
 */
class CorbaService
{
public:
   /**
    * Creates a new instance of this class and initializes the URI that will
    * be used to contact the CORBA Naming Service.
    *
    * @param nsHost       The hostname (or IP address) of the machine where
    *                     the Naming Service is running.
    * @param nsPort       The port number on which the Naming Service is
    *                     listening.  Normally, this will be 2809.  It defaults
    *                     to 2809.
    * @param iiopVersion  The version of IIOP to use when communicating with
    *                     the Naming Service.  Common values are "1.0" and
    *                     "1.2".  It defaults to "1.0".
    * @param subContextId The identifier for the Naming subcontext.  This is
    *                     currently unused.
    */
   CorbaService(const std::string& nsHost, vpr::Uint16 nsPort = 2809,
                const std::string& iiopVersion = std::string("1.0"),
                const std::string& subContextId = std::string(""));

   /**
    * This will destroy the Portable Object Adapter associated with this CORBA
    * Service, and it will shut down the ORB.  These steps may be performed
    * manually shutdown().
    *
    * @post The activated POA is destroyed.  The ORB is shut down, and its
    *       thread is stopped.
    */
   ~CorbaService()
   {
      shutdown();

      if ( mOrbThread != NULL )
      {
         delete mOrbThread;
         mOrbThread = NULL;
      }
   }

   bool init(int& argc, char* argv[]);

   /**
    * Shuts down the ORB and the POA (if they were successfully initialized).
    *
    * @post If the ORB and root POA were initialized successfully in init(),
    *       they are destroyed and shut down.
    *
    * @param waitForCompletion If true, block until all pending requests and
    *                          events are completed.  This parameter is
    *                          optional and defaults to true.
    */
   void shutdown(bool waitForCompletion = true);

   /**
    * Checks the validity of this service object to ensure that initialization
    * completed successfully.
    *
    * @return true if init() the ORB and POA references were initialized
    *         successfully.
    */
   bool isValid() const
   {
      return ! (CORBA::is_nil(mORB) || CORBA::is_nil(mRootPOA));
   }

   const std::string& getNameServiceURI() const
   {
      return mNameServiceURI;
   }

   const CosNaming::NamingContext_var getLocalContext()
   {
      return mLocalContext;
   }

   /**
    * Retrieves a reference to all the valid CORBA objects that implement the
    * tweek::SubjectManager interface.  When the references are added to the
    * list, it is guaranteed to have only valid tweek::SubjectManager
    * references.  However, due to the nature of CORBA references, they may
    * become invalid at any time thereafter.
    *
    * @return A list containing zero or more tweek::SubjectManager references.
    *
    * @note The implementation of this method is based on code found on page
    *       806 of <i>Advanced CORBA Programming with C++</i>.
    */
   std::list<tweek::SubjectManager_var> getSubjectManagerList();

   /**
    * Sets the Subject Manager reference that will be used with this CORBA
    * service object.
    */
   void setSubjectManager(tweek::SubjectManager_var mgr)
   {
      this->mSubjectManager = mgr;
   }

   /**
    * Returns a reference to the CORBA object known as the Subject Manager.
    * Using this reference, the caller can request references to subjects
    * registered with the manager.
    *
    * @return Nil reference if the Subject Manager reference has not been set
    *         yet.
    */
   tweek::SubjectManager_var getSubjectManager() const
   {
      return mSubjectManager;
   }

   /**
    * Registers the given servant with the local Portable Object Adaptor.
    * This is necessary for tweek::Observer implementations to be referenced
    * by their corresponding subject.
    *
    * @param servant The servant (interface implementation object) to register.
    * @param name    The name of the servant within the local POA.
    *
    * @return The ID of the servant within the POA.  This should be retained
    *         so that the servant can be unregistered later.
    */
   PortableServer::ObjectId_var registerObject(PortableServer::ServantBase* servant,
                                               const std::string& name);

   /**
    * Removes the identified Servant object from the collection of activated
    * CORBA objects.  In CORBA terms, the servant is deactivated within the
    * POA and cannot be referenced after this occurs.
    *
    * @param id The unique identifier for the servant to be deactivated.
    */
   void unregisterObject(PortableServer::ObjectId_var id);

   /**
    * Runs the ORB thread.  This should not be invoked by user code.  It is
    * for use with the internally managed ORB thread.
    */
   void run();

private:
   bool initRootPOA();

   /**
    * Resolves all the CORBA objects implementing tweek::SubjectManager in
    * bindingList and stores the resulting tweek.SubjectManager object(s) in
    * mgrList.  Only valid references are stored in mgrList.  If bindingList
    * contains no such objects, mgrList will not be modified.
    *
    * @post mgrList is populated with all valid (i.e., extant)
    *       tweek::SubjectManager references.
    */
   void addSubjectManagers(const CosNaming::BindingList& bindingList,
                           std::list<tweek::SubjectManager_var>& mgrList);

   vpr::Thread* mOrbThread;

   std::string mNsHost;
   vpr::Uint16 mNsPort;
   std::string mNameServiceURI;

   CORBA::ORB_var          mORB;
   PortableServer::POA_var mRootPOA;
//   PortableServer::POA_var mChildPOA;

   std::string                  mSubContextId;
   CosNaming::NamingContext_var mRootContext;
   CosNaming::NamingContext_var mLocalContext;

   tweek::SubjectManager_var mSubjectManager;
};

} // End of Tweek namespace


#endif /* _TWEEK_CORBA_SERVICE_H_ */
