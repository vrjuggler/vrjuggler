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
   , mEnabled(false)
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
   std::string err_msg("Error occured during initialization");
   vpr::ReturnStatus status;

   try
   {
      int dummy_int(0);

      // Attempt to initialize the CORBA Manager.
      status = mCorbaManager->init("corba_rtrc", dummy_int, NULL);

      // Test to see if init succeeded.
      if ( status.success() )
      {
         try
         {
            // Attempt to create the Subject Manager.
            status = mCorbaManager->createSubjectManager();

            if ( status.success() )
            {
               // Customize the Subject Manager information a little to help
               // users.
               mCorbaManager->getSubjectManager()->addInfoItem("Usage",
                                                               "Config Manager");

               // Create an intstance of our subject.
               mInterface = new RemoteReconfigSubjectImpl;
            }
            else
            {
               err_msg = "Tweek Subject Manager failed to initialize.";
               throw std::exception();
            }
         }
         catch(CORBA::Exception& ex)
         {
            err_msg = "Caught a CORBA exception while creating the Subject Manager";
            throw;  // Rethrow the exception, which will be caught below
         }
      }
      else
      {
         err_msg = "Could not initialize tweek::CorbaManager.";
         throw std::exception();
      }
   }
   catch(...)
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrRED, "ERROR:") << " [CorbaRemoteReconfig::init()]: "
         << err_msg << std::endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << "Disabling CORBA remote run-time reconfiguration.\n"
         << vprDEBUG_FLUSH;

      delete mCorbaManager;
      mCorbaManager = NULL;
      status.setCode(vpr::ReturnStatus::Fail);
   }

   return status;
}

vpr::ReturnStatus CorbaRemoteReconfig::enable()
{
   vpr::ReturnStatus status;

   //Make sure the corba manager has been initialized
   if ( mInterface == NULL || mCorbaManager == NULL )
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << "CorbaRemoteReconfig::enable: Cannot enable interface until it has been initialized\n"
         << vprDEBUG_FLUSH;

      status.setCode(vpr::ReturnStatus::Fail);
   }
   else
   {
      // Register the subject with the CORBA Manager.
      try
      {
         // Attempt to register the subject.
         mCorbaManager->getSubjectManager()->registerSubject(mInterface, mInterfaceName.c_str());
         mEnabled = true;
      }
      catch(...)
      {
         vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrRED, "ERROR:")
            << " [CorbaRemoteReconfig::enable()]: Caught an exception while "
            << "trying to register subject.\n" << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
            << "Disabling CORBA remote run-time reconfiguration.\n"
            << vprDEBUG_FLUSH;
         status.setCode(vpr::ReturnStatus::Fail);
      }
   }

   return status;
}

bool CorbaRemoteReconfig::isEnabled() const
{
   return mEnabled;
}

void CorbaRemoteReconfig::disable()
{
   //Make sure the corba manager has been initialized
   if ( mInterface == NULL || mCorbaManager == NULL || ! mEnabled )
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING:")
         << " [CorbaRemoteReconfig::disable()]: Cannot disable interface "
         << "until it has been initialized!\n" << vprDEBUG_FLUSH;

      return;
   }

   try
   {
      // Unregister the subject (note that observers must handle this
      // disconnection).
      vpr::ReturnStatus status;

      // Attempt to un-register the subject.
      status = mCorbaManager->getSubjectManager()->unregisterSubject(mInterfaceName.c_str());

      if ( ! status.success() )
      {
         vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " [CorbaRemoteReconfig::disable()]: Could not unregister subject."
            << std::endl << vprDEBUG_FLUSH;
      }
   }
   catch (...)
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING:")
         << " [CorbaRemoteReconfig::disable()]: Caught an exception while "
         << "trying to unregister subject.\n" << vprDEBUG_FLUSH;
   }

   mCorbaManager->shutdown();
   mEnabled = false;
}

} // End of jccl namespace
