/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#include <iomanip>
#include <algorithm>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>

#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>
#include <vpr/DynLoad/Library.h>

#include <vpr/DynLoad/LibraryLoader.h>


namespace vpr
{

#ifdef VPR_DEBUG
static const std::string DSO_NAME_EXT("_d");
#else
static const std::string DSO_NAME_EXT("");
#endif

#if defined(VPR_OS_Win32)
static const std::string PATH_SEP("\\");
static const std::string DSO_FILE_EXT(".dll");
#else
static const std::string PATH_SEP("/");

#  if defined(VPR_OS_Darwin)
static const std::string DSO_FILE_EXT(".dylib");
#  else
static const std::string DSO_FILE_EXT(".so");
#  endif
#endif

LibraryLoader::~LibraryLoader()
{
   for ( dso_list_t::iterator i = mLoadedLibs.begin(); i != mLoadedLibs.end(); ++i )
   {
      (*i)->unload();
   }
}

vpr::ReturnStatus LibraryLoader::findAndInitDSO(const std::string& dsoBaseName,
                                                const std::vector<std::string>& searchPath,
                                                const std::string& initFuncName,
                                                boost::function1<bool, void*> initFunc)
{
   vpr::ReturnStatus status;
   bool load_attempted(false);

   const std::string dso_name = dsoBaseName + DSO_NAME_EXT + DSO_FILE_EXT;

   for ( std::vector<std::string>::const_iterator i = searchPath.begin();
         i != searchPath.end();
         ++i )
   {
      std::string temp_name = *i + PATH_SEP + dso_name;

      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "vpr::LibraryLoader::findAndInitDSO(): Looking for '"
         << temp_name << "'\n" << vprDEBUG_FLUSH;

      try
      {
         // Use a Boost FS path object here so that we can indicate that
         // native path names are allowed.
         boost::filesystem::path temp_path =
            boost::filesystem::path(temp_name, boost::filesystem::native);

         if ( boost::filesystem::exists(temp_path) )
         {
            // If any part of the driver loading fails, the object
            // dsowill go out of scope at the end of this block, thereby
            // freeing the allocated memory.
            vpr::LibraryPtr dso =
               vpr::LibraryPtr(new vpr::Library(temp_name));
            status = this->loadAndInitDSO(dso, initFuncName, initFunc);
            load_attempted = true;
            break;
         }
      }
      catch (boost::filesystem::filesystem_error& fsEx)
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED, "ERROR:")
            << " File system exception caught: " << fsEx.what()
            << std::endl << vprDEBUG_FLUSH;
      }
   }

   // If no load was attempted, that means that dso_name does not exist
   // in searchPath.  We'll give it one last shot here using whatever
   // features the run-time loader has for searching on its own.
   if ( ! load_attempted )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "vpr::LibraryLoader::findAndInitDSO(): Falling back on run-time loader search for '"
         << dso_name << "'\n" << vprDEBUG_FLUSH;
      vpr::LibraryPtr dso = vpr::LibraryPtr(new vpr::Library(dso_name));
      status = this->loadAndInitDSO(dso, initFuncName, initFunc);
   }

   return status;
}

vpr::ReturnStatus LibraryLoader::loadAndInitDSO(vpr::LibraryPtr dso,
                                                const std::string& initFuncName,
                                                boost::function1<bool, void*> initFunc)
{
   vprASSERT(dso.get() != NULL && "Invalid vpr::LibraryPtr object");

   const int lib_name_width(50);

   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
      << "Loading library: " << std::setiosflags(std::ios::right)
      << std::setfill(' ') << std::setw(lib_name_width) << dso->getName()
      << std::resetiosflags(std::ios::right) << "     " << vprDEBUG_FLUSH;

   // Load the DSO.
   vpr::ReturnStatus status = dso->load();

   if ( status.success() )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "Loaded DSO successfully.\n" << vprDEBUG_FLUSH;

      // Look up the DSO's init function and store it in the pointer
      // called "creator."
      void* creator = dso->findSymbol(initFuncName);

      if ( NULL != creator )
      {
         vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
            << "[ " << clrSetNORM(clrGREEN) << "OK" << clrRESET << " ]\n"
            << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
            << "Got pointer to DSO initialization function.\n"
            << vprDEBUG_FLUSH;

         // XXX: This is a pretty lame way of managing the loaded DSOs.
         mLoadedLibs.push_back(dso);

         // Use the caller-provided callback function to initialize the
         // DSO.  It is up to this function to cast creator to the
         // desired type and do the cool stuff with it.
         if ( ! initFunc(creator) )
         {
            status.setCode(vpr::ReturnStatus::Fail);
         }
      }
      else
      {
         vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_LVL)
            << "[ " << clrSetNORM(clrRED) << "FAILED lookup" << clrRESET
            << " ]\n" << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << clrOutNORM(clrYELLOW, "WARNING")
            << ": Failed to look up initialization function '"
            << initFuncName << "' in DSO '" << dso << "'\n"
            << vprDEBUG_FLUSH;
         status.setCode(vpr::ReturnStatus::Fail);
      }
   }
   else
   {
      vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "[ " << clrSetNORM(clrRED) << "FAILED" << clrRESET << " ]\n"
         << vprDEBUG_FLUSH;
   }

   return status;
}

vpr::ReturnStatus LibraryLoader::unloadDSO(vpr::LibraryPtr dso)
{
   vpr::ReturnStatus status = dso->unload();

   if ( status.success() )
   {
      dso_list_t::iterator i =
         std::find(mLoadedLibs.begin(), mLoadedLibs.end(), dso);

      if ( i != mLoadedLibs.end() )
      {
         mLoadedLibs.erase(i);
      }
   }

   return status;
}

}
