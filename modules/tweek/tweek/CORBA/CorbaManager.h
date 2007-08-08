/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
#include TWEEK_INCLUDE_CORBA_H

#ifdef TWEEK_USE_TAO
#include <orbsvcs/CosNamingC.h>
#endif

#include <vpr/Thread/Thread.h>

#include <tweek/CORBA/SubjectManagerImpl.h>


namespace tweek
{

class BeanDeliverySubjectImpl;

/** \class CorbaManager CorbaManager.h tweek/CORBA/CorbaManager.h
 *
 * Wrapper around ORB features to simplify startup and shutdown.
 */
class TWEEK_CLASS_API CorbaManager
{
public:
   /**
    * Default constructor.
    */
   CorbaManager();

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
   ~CorbaManager();

   /**
    * Initializes the ORB and POA associated with this object.  A child POA of
    * the root POA (RootPOA) is created, and all servants registered with this
    * manager are activated within that child POA.
    *
    * @param localID     A string providing a unique identifier for the local
    *                    POA.
    * @param argc        The size of the following argument vector.  This will
    *                    be modified if any elements are removed from argv.
    * @param argv        The command-line arguments passed to the application.
    *                    These may include parameters defining the ORB's
    *                    behavior.  Those recognized as ORB arguments are
    *                    removed from the array leaving application parameters.
    * @param nsHost      The hostname (or IP address) of the machine where
    *                    the Naming Service is running.  This parameter is
    *                    optional, and it defaults to an empty string.  If no
    *                    value is given, the root naming context reference will
    *                    be requested using the "resolve initial references"
    *                    mechanism in CORBA.  This may require the use of an
    *                    external configuration file or command-line options
    *                    given in the argv parameter.
    * @param nsPort      The port number on which the Naming Service is
    *                    listening.  Normally, this will be 2809.  It defaults
    *                    to 2809.
    * @param iiopVersion The version of IIOP to use when communicating with
    *                    the Naming Service.  Common values are "1.0" and
    *                    "1.2".  It defaults to "1.0".
    */
   bool init(const std::string& localID, int& argc, char** argv,
             const std::string& nsHost = std::string(""),
             const vpr::Uint16& nsPort = vpr::Uint16(2809),
             const std::string& iiopVersion = std::string("1.0"));

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

   /**
    * Binds the interface object.
    */
   bool createSubjectManager();

   /**
    * Removes the Subject Manager created for use with this CORBA Manager
    * object.  The servant memory is released, the servant is deactivated in
    * the local POA, and the reference is unbound from the Naming Service.
    * All of this is done if and only if the Subject Manager was created
    * successfully in createSubjectManager().
    *
    * @return \c false will be returned if the servant could not be
    *         destroyed successfully.
    */
   bool destroySubjectManager();

   /**
    * Returns this CORBA Manager's SubjectManagerImpl instance to the caller.
    * Users will need this so that they may register subjects. It is the
    * caller's responsibility to call tweek::SubjectManagerImpl::_add_ref()
    * and tweek::SubjectManagerImpl::_remove_ref() as necessary.
    */
   tweek::SubjectManagerImpl* getSubjectManager() const
   {
      return mSubjectManager;
   }

   PortableServer::POA_var getRootPOA() const
   {
      return mRootPOA;
   }

   PortableServer::POA_var getChildPOA() const
   {
      return mChildPOA;
   }

   /**
    * Returns this CORBA Manager's BeanDeliverySubjectImpl instance to the
    * caller. It is the caller's responsibility to call the methods
    * tweek::BeanDeliverySubjectImpl::_add_ref() and
    * tweek::BeanDeliverySubjectImpl::_remove_ref() as necessary.
    */
   BeanDeliverySubjectImpl* getBeanDeliverySubject() const
   {
      return mBeanDeliverySubject;
   }

   /**
    * Runs the server.  This should not be invoked by user code.  It is for
    * use with the internally managed ORB thread.
    */
   void run();

private:
   bool createChildPOA(const std::string& localID);

   std::string mAppName;

   vpr::Thread* mOrbThread;

   CORBA::ORB_var mORB;
   PortableServer::POA_var mRootPOA;
   PortableServer::POA_var mChildPOA;
   CosNaming::NamingContext_var mRootContext;
   CosNaming::NamingContext_var mLocalContext;

   /** @name CORBA Servants */
   //@{
   tweek::SubjectManagerImpl* mSubjectManager;
   BeanDeliverySubjectImpl*   mBeanDeliverySubject;
   //@}

   /** The name of the default Bean Delivery Subject. */
   static const std::string DELIVERY_SUBJECT_NAME;
};

} // End of tweek namespace

#endif /* _TWEEK_CORBA_MANAGER_H_ */
