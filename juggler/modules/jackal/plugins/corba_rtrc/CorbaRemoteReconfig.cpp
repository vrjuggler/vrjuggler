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

#include <jccl/Plugins/PluginConfig.h>

#include <stdlib.h>
#include <vpr/vpr.h>
#include <vpr/Util/Debug.h>

#include <jccl/Util/Debug.h>
#include <corba_rtrc/RemoteReconfigSubjectImpl.h>
#include <corba_rtrc/CorbaRemoteReconfig.h>

jccl::RemoteReconfig* initPlugin()
{
   return new jccl::CorbaRemoteReconfig;
}

namespace jccl
{

CorbaRemoteReconfig::CorbaRemoteReconfig()
   : mCorbaManager(NULL)
   , mInterface(NULL)
   , mInterfaceName("CorbaRemoteReconfig")
{
   /* Do nothing. */ ;
}

CorbaRemoteReconfig::~CorbaRemoteReconfig()
{
   //Clean up interface by disconnecting first
   if (mInterface != NULL)
   {
      disable(); 
      delete mInterface;
      mInterface = NULL;
   }

   // Clean up the corba manager and delete it.
   if (mCorbaManager != NULL)
   {
      // CORBA Manager has its own destructor cleanup.
      delete mCorbaManager;
      mCorbaManager = NULL;
   }
}

vpr::ReturnStatus CorbaRemoteReconfig::init()
{
   // Create new CORBA Manager and initialize it.
   mCorbaManager = new tweek::CorbaManager;

   vpr::ReturnStatus status;

   try
   {
      int dummy_int(0);

      // Attempt to initialize the CORBA Manager.
      status = mCorbaManager->init("corba_rtrc", dummy_int, NULL);
   }
   catch (...)
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << "CorbaRemoteReconfig::init: Caught an unknown exception while initializing CorbaManager\n" 
         << vprDEBUG_FLUSH;

      delete mCorbaManager; 
      mCorbaManager = NULL;
      return vpr::ReturnStatus::Fail;
   }

   //Test to see if init succeeded
   if ( !status.success() )
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << "CorbaRemoteReconfig::init: Could not initialize CorbaManager\n" 
         << vprDEBUG_FLUSH;

      delete mCorbaManager; 
      mCorbaManager = NULL;
      return vpr::ReturnStatus::Fail;
   }

   try
   {
      //Attempt to create the subject manager
      status = mCorbaManager->createSubjectManager();
   }
   catch (CORBA::Exception& ex)
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << "CorbaRemoteReconfig::init: Caught an unknown CORBA exception while creating the subject manager\n" 
         << vprDEBUG_FLUSH;
   }
      
   if ( !status.success() )
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << "CorbaRemoteReconfig::init: CORBA subject manager failed to initialize\n" 
         << vprDEBUG_FLUSH;

      delete mCorbaManager; 
      mCorbaManager = NULL;
      return vpr::ReturnStatus::Fail;
   }

   //Create an intstance of our interface subject
   mInterface = new RemoteReconfigSubjectImpl;

   return status;
}

vpr::ReturnStatus CorbaRemoteReconfig::enable()
{
   //Make sure the corba manager has been initialized
   if ((mInterface == NULL) || (mCorbaManager == NULL))
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << "CorbaRemoteReconfig::enable: Cannot enable interface until it has been initialized\n" 
         << vprDEBUG_FLUSH;

      return vpr::ReturnStatus::Fail;
   }

   //Register the subject with the corba manager
   try
   {
      //Attempt to register the subject
      mCorbaManager->getSubjectManager()->registerSubject(mInterface, mInterfaceName.c_str());
      return vpr::ReturnStatus::Succeed;
   }
   catch (...)
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << "CorbaRemoteReconfig::enable: Caught an exception while trying to register subject\n" 
         << vprDEBUG_FLUSH;
      return vpr::ReturnStatus::Fail;
   }
}

bool CorbaRemoteReconfig::isEnabled() const
{
   return true;
}

void CorbaRemoteReconfig::disable()
{
   //Make sure the corba manager has been initialized
   if ((mInterface == NULL) || (mCorbaManager == NULL))
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << "CorbaRemoteReconfig::disable: Cannot disable interface until it has been initialized\n" 
         << vprDEBUG_FLUSH;

      return;
   }

   //Unregister the subject (note that observers must handle this disconnection)
   vpr::ReturnStatus status;

   try
   {
      //Attempt to register the subject
      status = mCorbaManager->getSubjectManager()->unregisterSubject(mInterfaceName.c_str());
   }
   catch (...)
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << "CorbaRemoteReconfig::disable: Caught an exception while trying to unregister subject\n" 
         << vprDEBUG_FLUSH;
   }

   if (!status.success())
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << "CorbaRemoteReconfig::disable: Could not unregister subject\n" 
         << vprDEBUG_FLUSH;
   }
}

} // End of jccl namespace
