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

#include <vrj/vrjConfig.h>

#include <boost/filesystem/path.hpp>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/DynLoad/LibraryLoader.h>
#include <vpr/IO/Socket/InetAddr.h>

#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/ElementFactory.h>
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/RTRC/DependencyManager.h>
#include <jccl/RTRC/ConfigElementHandler.h>
#include <jccl/Util/Debug.h>

#include <vrj/Performance/PerformanceMediator.h>
#include <vrj/Performance/PerfPlugin.h>

namespace fs = boost::filesystem;

namespace vrj
{

   PerformanceMediator::PerformanceMediator()
   : mPerfIf(NULL)
   {
      try
      {
         loadPerfPlugin();
      }
      catch (vpr::Exception& ex)
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << clrOutNORM(clrYELLOW, "WARNING:")
            << " Caught exception: " << ex.getExtendedDescription()
            << std::endl << vprDEBUG_FLUSH;
      }
   }

   PerformanceMediator::~PerformanceMediator()
   {
      if ( NULL != mPerfIf && mPerfIf->isEnabled() )
      {
         mPerfIf->disable();

         delete mPerfIf;
         mPerfIf = NULL;
      }
   }

/**
 * This struct implements a callable object (a functor, basically).  An
 * instance can be passed in where a boost::function1<bool, void*> is expected.
 * In vrj:PerformanceMediator::loadPerfPlugin(), instances are used to handle
 * dynamic loading of plug-ins via vpr::LibraryLoader.
 */
   struct Callable
   {
      Callable(vrj::PerformanceMediator* mgr) : med(mgr)
      {
      }

      /**
       * This will be invoked as a callback by methods of vpr::LibraryLoader.
       *
       * @param func A function pointer for the entry point in a dynamically
       *             loaded plug-in.  This must be cast to the correct signature
       *             before being invoked.
       */
      bool operator()(void* func)
      {
         vrj::PerfPlugin* (*init_func)(vrj::PerformanceMediator* mediator, jccl::ConfigManager* configMgr);

         // Cast the entry point function to the correct signature so that we can
         // call it.  All dynamically plug-ins must have an entry point function
         // that takes no argument and returns a pointer to an implementation of
         // the vrj::RemoteReconfig interface.
         init_func = (vrj::PerfPlugin* (*)(vrj::PerformanceMediator*, jccl::ConfigManager*)) func;

         jccl::ConfigManager* mgr = jccl::ConfigManager::instance();

         // Call the entry point function.
         vrj::PerfPlugin* plugin = (*init_func)(med, mgr);

         if ( NULL != plugin )
         {
            med->setPerfPlugin(plugin);
            return true;
         }
         else
         {
            return false;
         }
      }

      vrj::PerformanceMediator* med;
   };

   void PerformanceMediator::loadPerfPlugin()
   {
      vprASSERT(NULL == mPerfIf && "PerformanceMediator interface object already instantiated.");

      const std::string vj_base_dir("VJ_BASE_DIR");
      std::string base_dir;

      if ( ! vpr::System::getenv(vj_base_dir, base_dir) )
      {
         return;
      }

      const std::string no_perf_plugin("NO_PERF_PLUGIN");
      std::string junk;

      // If the user has the environment variable NO_PERF_PLUGIN set (to any
      // value), do not attempt to load the plug-in.
      if ( vpr::System::getenv(no_perf_plugin, junk) )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
            << "Remote performance visualization plug-in loading disabled "
            << "via NO_PERF_PLUGIN." << std::endl << vprDEBUG_FLUSH;
         return;
      }

#if defined(VPR_OS_IRIX) && defined(_ABIN32)
      const std::string bit_suffix("32");
#elif defined(VPR_OS_IRIX) && defined(_ABI64) || \
      defined(VPR_OS_Linux) && defined(__x86_64__)
      const std::string bit_suffix("64");
#else
      const std::string bit_suffix("");
#endif

      const std::string vrjuggler_subdir_base("vrjuggler");

      // If versioning is enabled, then the name of the directory containing
      // the VR Juggler plug-ins must contain version information.
#if defined(VRJ_USE_VERSIONING)
      std::string vrjuggler_ver_str;
      const std::string vrjuggler_version("VRJ_VERSION");

      // If $VRJ_VERSION is set, use the value of that environment variable
      // as the version component of the plug-in subdirectory name. Otherwise,
      // use the compile-time value provided by VRJ_VERSION_DOT.
      if ( ! vpr::System::getenv(vrjuggler_version, vrjuggler_ver_str) )
      {
         vrjuggler_ver_str = VRJ_VERSION_DOT;
      }

      std::string vrjuggler_subdir(vrjuggler_subdir_base + std::string("-") +
                                   vrjuggler_ver_str);

      // If versioning is not enabled, then the directory containing the
      // VR Juggler plug-ins will not incorporate version information.
#else
      std::string vrjuggler_subdir(vrjuggler_subdir_base);
#endif

      std::vector<fs::path> search_path(1);
      search_path[0] = fs::path(base_dir, fs::native) /
                          (std::string("lib") + bit_suffix) /
                          std::string(vrjuggler_subdir) /
                          std::string("plugins");

      try
      {
         // In the long run, we may not want to hard-code the base name of the
         // plug-in we load.  If we ever reach a point where we have multiple
         // ways of implementing remote performance monitoring, we could have
         // options for which plug-in to load.
         const std::string perf_mon_dso("corba_perf_mon");
         const std::string init_func("initPlugin");
         Callable functor(this);
         vpr::LibraryLoader::findDSOAndCallEntryPoint(perf_mon_dso,
                                                      search_path, init_func,
                                                      functor, mPluginDSO);
      }
      catch (vpr::Exception&)
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Failed to load the remote performance monitoring plug-in."
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Remote performance monitoring is disabled." << std::endl
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "(This is not a fatal error.)" << std::endl << vprDEBUG_FLUSH;

         // The plug-in is not usable, so we can unload it.
         if ( mPluginDSO.get() != NULL )
         {
            if ( mPluginDSO->isLoaded() )
            {
               mPluginDSO->unload();
            }

            mPluginDSO.reset();
         }
      }
   }

   void PerformanceMediator::setPerfPlugin(vrj::PerfPlugin* plugin)
   {
      // If we already have a remote performance monitoring plug-in, discard it
      // first.
      if ( NULL != mPerfIf )
      {
         vprDEBUG(jcclDBG_RECONFIG, vprDBG_STATE_LVL)
         << "[PerformanceMediator::setPerfPlugin()] "
            << "Removing old remote performance monitoring plug-in\n"
            << vprDEBUG_FLUSH;

         if ( mPerfIf->isEnabled() )
         {
            mPerfIf->disable();
         }

         delete mPerfIf;
      }

      vprDEBUG(jcclDBG_RECONFIG, vprDBG_VERB_LVL)
      << "[PerformanceMediator::setPerfPlugin()] "
         << "Enabling new remote performance monitoring plug-in\n"
         << vprDEBUG_FLUSH;
      mPerfIf = plugin;

      if ( NULL != mPerfIf )
      {
         // Attempt to initialize the remote performance monitoring component.
         if ( mPerfIf->init() )
         {
            // Now, attempt to enable remote performance monitoring hooks.
            if ( ! mPerfIf->enable() )
            {
               vprDEBUG(jcclDBG_RECONFIG, vprDBG_WARNING_LVL)
               << clrOutBOLD(clrYELLOW, "WARNING:")
               << " Failed to enable remote performance monitoring hooks.\n"
                  << vprDEBUG_FLUSH;
               delete mPerfIf;
               mPerfIf = NULL;
            }
         }
         // Initialization failed.
         else
         {
            vprDEBUG(jcclDBG_RECONFIG, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Failed to initialize remote performance monitoring hooks.\n"
               << vprDEBUG_FLUSH;
            delete mPerfIf;
            mPerfIf = NULL;
         }
      }
   }

} // namespace vrj
