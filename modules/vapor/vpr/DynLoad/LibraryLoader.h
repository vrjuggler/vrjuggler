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

#ifndef _VPR_LIBRARY_LOADER_H_
#define _VPR_LIBRARY_LOADER_H_
#define _VPR_LIBRARY_LOADER_H_

#include <vpr/vprConfig.h>
#include <string>
#include <vector>
#include <algorithm>
#include <vpr/DynLoad/Library.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>


namespace vpr
{

template<typename INIT_OBJ>
class LibraryLoader
{
public:
   typedef std::vector<vpr::LibraryPtr> dso_list_t;

   LibraryLoader()
      : mLoadedLibs()
   {
      /* Do nothing. */ ;
   }

   ~LibraryLoader()
   {
      for ( dso_list_t::iterator i = mLoadedLibs.begin(); i != mLoadedLibs.end(); ++i )
      {
         (*i)->unload();
      }
   }

   /**
    * Loads the dynamic shared object (DSO) from the given vpr::LibraryPtr
    * object and invokes its factory function.
    *
    * @pre vpr::LibraryPtr contains a pointer to a valid vpr::Library object.
    */
   vpr::ReturnStatus loadAndInitDSO(vpr::LibraryPtr dso,
                                    const std::string& factoryFuncName,
                                    INIT_OBJ* initObj)
   {
      vprASSERT(dso.get() != NULL && "Invalid vpr::LibraryPtr object");

      const int lib_name_width(50);

      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "Loading library: " << std::setiosflags(std::ios::right)
         << std::setfill(' ') << std::setw(lib_name_width) << dso->getName()
         << std::resetiosflags(std::ios::right) << "     " << vprDEBUG_FLUSH;

      // Load the DSO.
      vpr::ReturnStatus status = dso->load();

      if ( status.success() )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Loaded DSO successfully.\n" << vprDEBUG_FLUSH;

         // Look up the DSO's factory function and store it in the function
         // pointer called "creator."
         void (*creator)(INIT_OBJ*);
         creator = (void (*)(INIT_OBJ*)) dso->findSymbol(factoryFuncName);

         if ( NULL != creator )
         {
            vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
               << "[ " << clrSetNORM(clrGREEN) << "OK" << clrRESET << " ]\n"
               << vprDEBUG_FLUSH;
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
               << "Got pointer to DSO factory function.\n" << vprDEBUG_FLUSH;

            // XXX: This is a pretty lame way of managing the loaded DSOs.
            mLoadedLibs.push_back(dso);

            // Invoke the DSO's factory function with the given initialization
            // object.
            (*creator)(initObj);
         }
         else
         {
            vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_LVL)
               << "[ " << clrSetNORM(clrRED) << "FAILED lookup" << clrRESET
               << " ]\n" << vprDEBUG_FLUSH;
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
               << clrOutNORM(clrYELLOW, "WARNING")
               << ": Failed to look up factory function '" << factoryFuncName
               << "' in DSO '" << dso << "'\n" << vprDEBUG_FLUSH;
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

   vpr::ReturnStatus unloadDSO(vpr::LibraryPtr dso)
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

   const dso_list_t& getDsoList() const
   {
      return mLoadedLibs;
   }

private:
   dso_list_t mLoadedLibs;
};

}


#endif /* _VPR_LIBRARY_LOADER_H_ */
