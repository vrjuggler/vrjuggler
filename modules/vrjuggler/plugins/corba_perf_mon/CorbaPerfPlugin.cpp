/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <vrj/Performance/PluginConfig.h>

#include <stdlib.h>
#include <boost/concept_check.hpp>

#include <vpr/vpr.h>
#include <vpr/Util/Debug.h>

#include <jccl/RTRC/ConfigManager.h>

#include <vrj/Util/Debug.h>
#include <vrj/Performance/PerformanceMediator.h>

#include <corba_perf_mon/PerformanceMonitorSubjectImpl.h>
#include <corba_perf_mon/CorbaPerfPlugin.h>


extern "C"
{

   VRJ_PLUGIN_EXPORT(vrj::PerfPlugin*) initPlugin(vrj::PerformanceMediator* mediator, jccl::ConfigManager* configMgr)
   {
      return new vrj::CorbaPerfPlugin(mediator, configMgr);
   }

}

namespace vrj
{

   CorbaPerfPlugin::CorbaPerfPlugin(vrj::PerformanceMediator* mediator, jccl::ConfigManager* configMgr)
   : mMediator(mediator)
   , mConfigManager(configMgr)
   , mCorbaManager(NULL)
   , mInterface(NULL)
   , mEnabled(false)
   , mInterfaceName("CorbaPerfPlugin")
   {
      mConfigManager->addConfigElementHandler(this);
   }

   CorbaPerfPlugin::~CorbaPerfPlugin()
   {
      mConfigManager->removeConfigElementHandler(this);
      mConfigManager = NULL;
      mMediator = NULL;
      stopCorba();
   }

   bool CorbaPerfPlugin::configCanHandle(jccl::ConfigElementPtr element)
   {
      return element->getID() == std::string("corba_performance_monitor");
   }

   bool CorbaPerfPlugin::configAdd(jccl::ConfigElementPtr element)
   {
      const unsigned int min_def_version(2);

      if ( element->getVersion() < min_def_version )
      {
         vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING") << ": Element named '"
            << element->getName() << "'" << std::endl;
         vprDEBUG_NEXTnl(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
            << "is version " << element->getVersion()
            << ", but we expect at least version " << min_def_version
            << ".\n";
         vprDEBUG_NEXTnl(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
            << "Default values will be used for some settings.\n"
            << vprDEBUG_FLUSH;
      }

      // If the ORB is already running, we need to shut it down first.  One big
      // reason for doing this is to release the resources (memory and so on)
      // allocated previously.
      if ( mInterface != NULL || mCorbaManager != NULL )
      {
         vprDEBUG(vrjDBG_PLUGIN, vprDBG_STATE_LVL)
            << "[CorbaPerfPlugin::configAdd()] Attempting to shut down "
            << "existing CORBA instance.\n" << vprDEBUG_FLUSH;

         // stopCorba() will call disable() if we are still in the enabled state.
         stopCorba();
      }

      // We'll ignore the return value for now.  startCorba() prints out enough
      // warning information on its own if something goes wrong.
      this->startCorba(element->getProperty<std::string>("endpoint_addr"),
                       element->getProperty<vpr::Uint16>("endpoint_port"));

      return true;
   }

   bool CorbaPerfPlugin::configRemove(jccl::ConfigElementPtr element)
   {
      boost::ignore_unused_variable_warning(element);

      if ( isEnabled() )
      {
         disable();
      }

      return true;
   }

   bool CorbaPerfPlugin::init()
   {
      return true;
   }

   bool CorbaPerfPlugin::enable()
   {
      return true;
   }

   bool CorbaPerfPlugin::isEnabled() const
   {
      return mEnabled;
   }

   void CorbaPerfPlugin::disable()
   {
      //Make sure the corba manager has been initialized
      if ( mInterface == NULL || mCorbaManager == NULL || ! mEnabled )
      {
         vprDEBUG(vrjDBG_PLUGIN, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING:")
         << " [CorbaPerfPlugin::disable()] Cannot disable interface "
            << "until it has been initialized!\n" << vprDEBUG_FLUSH;

         return;
      }

      try
      {
         // Unregister the subject (note that observers must handle this
         // disconnection).
         // Attempt to un-register the subject.
         const bool status =
            mCorbaManager->getSubjectManager()->unregisterSubject(mInterfaceName.c_str());

         if ( ! status )
         {
            vprDEBUG(vrjDBG_PLUGIN, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " [CorbaPerfPlugin::disable()] Could not unregister subject."
               << std::endl << vprDEBUG_FLUSH;
         }
      }
      catch ( ... )
      {
         vprDEBUG(vrjDBG_PLUGIN, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING:")
         << " [CorbaPerfPlugin::disable()] Caught an exception while "
            << "trying to unregister subject.\n" << vprDEBUG_FLUSH;
      }

      mCorbaManager->shutdown();
      mEnabled = false;
   }

   bool CorbaPerfPlugin::startCorba(const std::string& listenAddr,
                                    const vpr::Uint16 listenPort)
   {
      // Create new CORBA Manager and initialize it.
      mCorbaManager = new tweek::CorbaManager;
      std::string err_msg("Error occured during initialization");
      bool result(true);

      try
      {
         int dummy_int(0);

         // Attempt to initialize the CORBA Manager.
         bool status = mCorbaManager->initDirect("corba_perf_mon", dummy_int,
                                                 NULL, listenAddr,
                                                 listenPort);

         // Test to see if init succeeded.
         if ( status )
         {
            try
            {
               // Attempt to create the Subject Manager.
               status = mCorbaManager->createSubjectManager();

               if ( status )
               {
                  // Customize the Subject Manager information a little to help
                  // users.
                  mCorbaManager->getSubjectManager()->addInfoItem("Usage",
                                                                  "Performance Monitor");

                  // Create an intstance of our subject.
                  mInterface = new PerformanceMonitorSubjectImpl;

                  // Register the subject with the CORBA Manager.
                  try
                  {
                     // Attempt to register the subject.
                     mCorbaManager->getSubjectManager()->registerSubject(mInterface, mInterfaceName.c_str());
                     mEnabled = true;
                  }
                  catch ( ... )
                  {
                     vprDEBUG(vrjDBG_PLUGIN, vprDBG_WARNING_LVL)
                     << clrOutBOLD(clrRED, "ERROR:")
                     << " [CorbaPerfPlugin::startCorba()] Caught an "
                        << "exception while trying to register subject.\n"
                        << vprDEBUG_FLUSH;
                     vprDEBUG_NEXT(vrjDBG_PLUGIN, vprDBG_WARNING_LVL)
                     << "Disabling CORBAmremote performance monitoring.\n"
                        << vprDEBUG_FLUSH;
                     result = false;
                  }
               }
               else
               {
                  err_msg = "Tweek Subject Manager failed to initialize.";
                  throw std::exception();
               }
            }
            catch ( CORBA::Exception& ex )
            {
               boost::ignore_unused_variable_warning(ex);
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
      catch ( ... )
      {
         vprDEBUG(vrjDBG_PLUGIN, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrRED, "ERROR:")
         << " [CorbaPerfPlugin::startCorba()] " << err_msg << std::endl
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vrjDBG_PLUGIN, vprDBG_WARNING_LVL)
         << "Disabling CORBA remote performance monitoring.\n"
            << vprDEBUG_FLUSH;

         delete mCorbaManager;
         mCorbaManager = NULL;
         result = false;
      }

      return result;
   }

   void CorbaPerfPlugin::stopCorba()
   {
      //Clean up interface by disconnecting first
      if ( mInterface != NULL )
      {
         if ( isEnabled() )
         {
            disable();
         }

         delete mInterface;
         mInterface = NULL;
      }

      // Clean up the corba manager and delete it.
      if ( mCorbaManager != NULL )
      {
         // CORBA Manager has its own destructor cleanup.
         delete mCorbaManager;
         mCorbaManager = NULL;
      }
   }

} // End of vrj namespace
