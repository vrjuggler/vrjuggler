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
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>

#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <vpr/DynLoad/LibraryFinder.h>
#include <vpr/DynLoad/DsoLoader.h>


namespace vpr
{

DsoLoader::DsoLoader()
   : mInitFuncName("")
{
}

DsoLoader::DsoLoader(const std::string& initFuncName)
   : mInitFuncName(initFuncName)
{
}

DsoLoader::DsoLoader(const std::string& initFuncName,
                     const std::string& dsoName,
                     DsoLoader::CreatorArg& creatorArg) throw(vpr::LoaderError)
   : mInitFuncName(initFuncName)
{
   this->load(dsoName, creatorArg);
}

DsoLoader::DsoLoader(const std::string& initFuncName,
                     const std::vector<std::string>& dsoList,
                     DsoLoader::CreatorArg& creatorArg)
   : mInitFuncName(initFuncName)
{
   this->load(dsoList, creatorArg);
}

void DsoLoader::load(const std::string& dsoName, CreatorArg& creatorArg)
   throw(vpr::LoaderError)
{
   vpr::LibraryPtr lib = vpr::LibraryPtr(new vpr::Library(dsoName));
   this->loadDSO(lib, creatorArg);
}

void DsoLoader::load(const std::vector<std::string>& dsoList,
                     DsoLoader::CreatorArg& creatorArg)
{
   for ( std::vector<std::string>::const_iterator i = dsoList.begin();
         i != dsoList.end();
         ++i )
   {
      if ( ! (*i).empty() )
      {
         vpr::LibraryPtr lib = vpr::LibraryPtr(new vpr::Library(*i));

         try
         {
            this->loadDSO(lib, creatorArg);
         }
         catch(vpr::LoaderError& loaderEx)
         {
            vprDEBUG(vprDBG_ERROR, vprDBG_WARNING_LVL)
               << clrOutBOLD(clrYELLOW, "WARNING:") << loaderEx.what()
               << std::endl << vprDEBUG_FLUSH;
         }
      }
   }
}

void DsoLoader::loadFromDirectory(const std::string& dirName,
                                  const std::string& dsoExt,
                                  DsoLoader::CreatorArg& creatorArg)
{
   // The vpr::LibraryFinder will throw an exception if dirName is
   // (somehow) an invalid path.
   try
   {
      if (boost::filesystem::exists(dirName))
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "Searching for DSOs in '" << dirName << "'\n"
            << vprDEBUG_FLUSH;

         vpr::LibraryFinder finder(dirName, dsoExt);
         vpr::LibraryFinder::LibraryList libs = finder.getLibraries();

         for ( vpr::LibraryFinder::LibraryList::iterator lib = libs.begin();
               lib != libs.end();
               ++lib )
         {
            try
            {
               this->loadDSO(*lib, creatorArg);
            }
            catch(vpr::LoaderError& loaderEx)
            {
               vprDEBUG(vprDBG_ERROR, vprDBG_WARNING_LVL)
                  << clrOutBOLD(clrYELLOW, "WARNING:") << loaderEx.what()
                  << std::endl << vprDEBUG_FLUSH;
            }
         }
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Invalid directory for DSO's: " << dirName
            << std::endl << vprDEBUG_FLUSH;
      }
   }
   catch(boost::filesystem::filesystem_error& fsEx)
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:")
         << " File system exception caught: " << fsEx.what()
         << std::endl << vprDEBUG_FLUSH;
   }
}

// XXX: This should probably throw an exception if something goes wrong, but
// I haven't decided how that will work yet.
void DsoLoader::loadDSO(vpr::LibraryPtr dsoPtr,
                        DsoLoader::CreatorArg& creatorArg)
   throw(vpr::LoaderError)
{
   vprASSERT(dsoPtr.get() != NULL && "Invalid vpr::LibraryPtr object");

   const int lib_name_width(50);

   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "Loading dynamic shared object: " << std::setiosflags(std::ios::right)
      << std::setfill(' ') << std::setw(lib_name_width) << dsoPtr->getName()
      << std::resetiosflags(std::ios::right) << "     " << vprDEBUG_FLUSH;

   // Load the driver
   vpr::ReturnStatus status = dsoPtr->load();

   if ( status.success() )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "Loaded DSO success.\n" << vprDEBUG_FLUSH;

      InitFuncType creator;

      vprASSERT(! mInitFuncName.empty() && "No creator function given");
      creator = (InitFuncType) dsoPtr->findSymbol(mInitFuncName);

      if ( NULL != creator )
      {
         vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
            << "[ " << clrSetNORM(clrGREEN) << "OK" << clrRESET << " ]\n"
            << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Got pointer to DSO factory function\n" << vprDEBUG_FLUSH;

         mDsoList.push_back(dsoPtr);
         (*creator)(creatorArg.paramType.c_str(), creatorArg.param);
      }
      else
      {
         vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_LVL)
            << "[ " << clrSetNORM(clrRED) << "FAILED lookup" << clrRESET
            << " ]\n" << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ERROR, vprDBG_WARNING_LVL)
            << clrOutNORM(clrYELLOW, "WARNING")
            << ": Failed to look up factory function in DSO '"
            << dsoPtr << "'\n" << vprDEBUG_FLUSH;

         throw vpr::LoaderError("vpr::DsoLoader::loadDSO()",
                                "Factory function lookup failed",
                                vpr::LoaderError::SYM_LOOKUP_ERROR);
      }
   }
   else
   {
      vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "[ " << clrSetNORM(clrRED) << "FAILED" << clrRESET << " ]\n"
         << vprDEBUG_FLUSH;

      throw vpr::LoaderError("vpr::DsoLoader::loadDSO()",
                             "Library load failed",
                             vpr::LoaderError::FILE_LOAD_ERROR);
   }
}

} // End of vpr namespace
