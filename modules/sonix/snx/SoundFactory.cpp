/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * sonix
 *
 * Original Authors:
 *   Kevin Meinert
 *
 ****************** <SNX heading END do not edit this line> ******************/

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

#include <snx/snxConfig.h>

#include <iostream>
#include <fstream>
#include <string>

#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/DynLoad/LibraryFinder.h>

#include <snx/Util/Debug.h>
#include <snx/ISoundImplementation.h>
#include <snx/StubSoundImplementation.h> /* in case lookup fails... */

#include <snx/SoundFactory.h> /* my header. */


namespace fs = boost::filesystem;

namespace snx
{

/**
 * Create the plugin.
 * Symbol name to look up from the shared lib is "newPlugin".
 */
typedef snx::ISoundImplementation* (*new_plugin_func)(void);

/**
 * Gets the name of the plugin.
 * Symbol name to lookup from the shared lib is "getName".
 */
typedef char* (*get_name_func)(void);

/**
 * Get the version of sonix plugin was compiled against.
 * Symbol name to lookup from the shared lib is "getVersion".
 */
typedef char* (*get_version_func)(void);

// This singleton must be removed after the snx::sonix singleton in case the
// code for a plug-in needs to be referenced by snx::sonix::~sonix().
vprSingletonImpLifetime(SoundFactory, 2);

SoundFactory::SoundFactory()
{
   const std::string envvar("SNX_BASE_DIR");
   std::string base_dir_env;

   if ( ! vpr::System::getenv(envvar, base_dir_env) )
   {
      vprDEBUG(snxDBG, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING:")
         << " SNX_BASE_DIR environment variable not set!\n" << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(snxDBG, vprDBG_WARNING_LVL)
         << "Sonix plug-in loading may fail.\n" << vprDEBUG_FLUSH;
   }

   std::vector<fs::path> search_paths;

   try
   {
      const fs::path snx_base_dir(base_dir_env, fs::native);

#if defined(VPR_OS_IRIX) && defined(_ABIN32)
      const fs::path snx_lib_dir = snx_base_dir / "lib32";
#elif defined(VPR_OS_IRIX) && defined(_ABI64) || \
      defined(VPR_OS_Linux) && defined(__x86_64__)
      const fs::path snx_lib_dir = snx_base_dir / "lib64";
#else
      const fs::path snx_lib_dir = snx_base_dir / "lib";
#endif

      const std::string sonix_subdir_base("sonix");

      // If versioning is enabled, then the name of the directory containing
      // the Sonix plug-ins must contain version information.
#if defined(SNX_USE_VERSIONING)
      std::string sonix_ver_str;
      const std::string sonix_version("SNX_VERSION");

      // If $SNX_VERSION is set, use the value of that environment variable
      // as the version component of the plug-in subdirectory name. Otherwise,
      // use the compile-time value provided by SNX_VERSION_DOT.
      if ( ! vpr::System::getenv(sonix_version, sonix_ver_str) )
      {
         sonix_ver_str = SNX_VERSION_DOT;
      }

      fs::path sonix_subdir(
         snx_lib_dir / (sonix_subdir_base + "-" + sonix_ver_str) / "plugins"
      );

      // If versioning is not enabled, then the directory containing the
      // Sonix plug-ins will not incorporate version information.
#else
      fs::path sonix_subdir(snx_lib_dir / sonix_subdir_base / "plugins");
#endif

#if defined(SNX_DEBUG)
#if defined(_DEBUG)
      sonix_subdir /= "dbgrt";
#else
      sonix_subdir /= "dbg";
#endif
#else
      sonix_subdir /= "opt";
#endif

      search_paths.push_back(sonix_subdir);

      std::string home_dir;
      if ( vpr::System::getenv("HOME", home_dir) )
      {
         search_paths.push_back(
            fs::path(home_dir, fs::native) / fs::path(".sonix", fs::native) /
               "plugins"
         );
      }
   }
   catch (fs::filesystem_error& ex)
   {
      vprDEBUG(snxDBG, vprDBG_WARNING_LVL)
         << clrOutNORM(clrRED, "WARNING")
         << ": File system exception caught while constructing search "
         << "path: " << std::endl << ex.what() << std::endl
         << vprDEBUG_FLUSH;
   }

#if defined(VPR_OS_Windows)
   const std::string driver_ext("dll");
#elif defined(VPR_OS_Darwin)
   const std::string driver_ext("dylib");
#else
   const std::string driver_ext("so");
#endif

   typedef std::vector<fs::path>::iterator iter_type;
   for ( iter_type p = search_paths.begin(); p != search_paths.end(); ++p )
   {
      const std::string cur_dir = (*p).native_directory_string();
      vprDEBUG(snxDBG, vprDBG_CONFIG_LVL) << "Finding plug-ins in " << cur_dir
                                          << std::endl << vprDEBUG_FLUSH;

      try
      {
         if ( fs::exists(*p) )
         {
            vpr::LibraryFinder finder(cur_dir, driver_ext);
            vpr::LibraryFinder::LibraryList libs = finder.getLibraries();
            this->loadPlugins(libs);

#ifdef SNX_DEBUG
            vprDEBUG(snxDBG, vprDBG_CONFIG_LVL) << "Plug-in file list:\n"
                                                << vprDEBUG_FLUSH;
            for ( unsigned int i = 0; i < libs.size(); ++i )
            {
               vprDEBUG_NEXT(snxDBG, vprDBG_CRITICAL_LVL) 
                  << libs[i]->getName() << std::endl << vprDEBUG_FLUSH;
            }
#endif
         }
         else
         {
            vprDEBUG(snxDBG, vprDBG_STATE_LVL)
               << "The directory does not exist: '" << cur_dir << "'\n"
               << vprDEBUG_FLUSH;
         }
      }
      catch (fs::filesystem_error& fsEx)
      {
         vprDEBUG(snxDBG, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED, "ERROR:")
            << " File system exception caught: " << fsEx.what() << std::endl
            << vprDEBUG_FLUSH;
      }
   }
}

void SoundFactory::errorOutput(vpr::LibraryPtr lib, const char* test)
{
   vprDEBUG(snxDBG, vprDBG_WARNING_LVL)
      << "ERROR: Plugin '" << lib->getName() << "' does not export symbol '"
      << test << "'\n" << vprDEBUG_FLUSH;
}

bool SoundFactory::isPlugin(vpr::LibraryPtr lib)
{
   if (lib->findSymbol( "newPlugin" ) == NULL)
   {
      errorOutput(lib, "newPlugin");
      return false;
   }
   if (lib->findSymbol( "getVersion" ) == NULL)
   {
      errorOutput(lib, "getVersion");
      return false;
   }
   if (lib->findSymbol( "getName" ) == NULL)
   {
      errorOutput(lib, "getName");
      return false;
   }

   // @todo give sonix an internal version number string!
   //get_version_func get_version = (get_version_func) lib.lookup("getVersion");
   //if (get_version != NULL && get_version() != snx::version) return false;
   vprDEBUG(snxDBG, vprDBG_CONFIG_STATUS_LVL) << "Plugin '" << lib->getName()
                                              << "' is a valid Sonix plugin.\n"
                                              << vprDEBUG_FLUSH;
   return true;
}

void SoundFactory::loadPlugins(vpr::LibraryFinder::LibraryList libs)
{
   for ( unsigned int x = 0; x < libs.size(); ++x )
   {
      try
      {
         // Open the library.
         libs[x]->load();

         // If the plug-in implements the nessiasry interface,
         if ( this->isPlugin(libs[x]) )
         {
            // get the name..
            get_name_func get_name =
               (get_name_func) libs[x]->findSymbol("getName");

            if ( get_name != NULL )
            {
               std::string name = get_name();

               // Check to see it plug-in is already registered
               int plugins_with_name =  mRegisteredImplementations.count(name);
               if ( plugins_with_name < 1 )
               {
                  // Keep track of the plug-ins we actual use
                  mPlugins.push_back(libs[x]);

                  vprDEBUG(snxDBG, vprDBG_STATE_LVL)
                     << "Got plug-in '" << name << "'--registering\n"
                     << vprDEBUG_FLUSH;

                  // create the implementation
                  new_plugin_func new_plugin =
                     (new_plugin_func) libs[x]->findSymbol("newPlugin");

                  if ( new_plugin != NULL )
                  {
                     ISoundImplementation* si = new_plugin();
                     if (NULL != si)
                     {
                        this->reg(name, si);
                     }
                  }
               }
               else
               {
                  // Plug in was already registered so we can unload it.
                  libs[x]->unload();
               }
            }
         }
      }
      catch (vpr::Exception& ex)
      {
         // Library failed to load.
         vprDEBUG(snxDBG, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrRED, "ERROR") << ": Plugin '"
            << libs[x]->getName() << "' Failed to load\n" << ex.what()
            << std::endl << vprDEBUG_FLUSH;
      }
   }
}

void SoundFactory::unloadPlugins()
{
   for ( unsigned int x = 0; x < mPlugins.size(); ++x )
   {
      // get the name..
      get_name_func get_name =
         (get_name_func) mPlugins[x]->findSymbol("getName");

      ISoundImplementation* impl(NULL);
      if ( get_name != NULL )
      {
         std::string name = get_name();
         impl = mRegisteredImplementations[name];

         // unreg it.
         this->reg(name, NULL);
      }

      // delete the memory using the delete func that comes with the library...
      if ( NULL != impl )
      {
         delete impl;
      }

      // close the library
      mPlugins[x]->unload();
   }
   mPlugins.clear();
}

void SoundFactory::reg(const std::string& apiName,
                       snx::ISoundImplementation* impl)
{
   if (impl != NULL)
   {
      vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
         << "NOTICE: Registering sound API '" << apiName << "'\n"
         << vprDEBUG_FLUSH;

      impl->setName( apiName );
      mRegisteredImplementations[apiName] = impl;
   }
   else
   {
      mRegisteredImplementations.erase(apiName);

      vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
         << "NOTICE: Unregistered sound API '" << apiName << "'\n"
         << vprDEBUG_FLUSH;
   }
}

// Factory function used to create an implementation of a sound API.
void
SoundFactory::createImplementation(const std::string& apiName,
                                   snx::ISoundImplementation* &mImplementation)
{
   if ( mRegisteredImplementations.count( apiName ) > 0 )
   {
      mRegisteredImplementations[apiName]->clone( mImplementation );
   }
   else
   {
      mImplementation = new snx::StubSoundImplementation;
   }

   //mImplementation->setName( apiName );
}

} // end namespace
