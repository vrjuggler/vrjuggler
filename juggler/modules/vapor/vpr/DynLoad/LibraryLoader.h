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

#include <vpr/vprConfig.h>

#include <string>
#include <vector>
#include <boost/function.hpp>

#include <vpr/DynLoad/LibraryPtr.h>


namespace vpr
{

/**
 * A helper for loading a specific class of dynamic shared objects (DSOs).
 * Through (relatively) high-level methods, this class introduces a uniform
 * naming scheme for DSOs.  More details are provided in the documentation for
 * individual methods.
 */
class VPR_CLASS_API LibraryLoader
{
public:
   typedef std::vector<vpr::LibraryPtr> dso_list_t;

   LibraryLoader()
      : mLoadedLibs()
   {
      /* Do nothing. */ ;
   }

   ~LibraryLoader();

   /**
    * Searches for a dynamic shared object (DSO) with the "base" name given in
    * dsoBaseName.  The base name is a platform-independent, abstract name
    * that makes up a substring of the name of the actual file that will be
    * loaded.  A platform-specific extension (.so, .dll, etc) will be appended,
    * and for an instantiation of this class in debug-enabled code, the suffix
    * "_d" will be appended to the DSO's base name.  For example, given the
    * base name mydso, a debug build on Windows would search for the file
    * mydso_d.dll.  For a release (optimized) build, it would search for
    * mysdso.dll.  The search will be performed using list of directories
    * given in searchPath.
    *
    * @param dsoBaseName  The base name of the DSO to be loaded.  The
    *                     platform-specific file extension will be appended
    *                     prior to the search.
    * @param searchPath   A list of directories where the target DSO may
    *                     exist.
    * @param initFuncName The name of the initialization function in the DSO
    *                     that will be invoked to initialize the DSO's code.
    * @param initFunc     A Boost.Function object (that is, a callable object)
    *                     that will be invoked as a callback to initialize the
    *                     DSO.  It will be passed a void* pointer to the
    *                     initialization function looked up in the DSO.
    *
    * @note The vpr::Library object instantiated to hold the DSO is not
    *       returned to the caller.  It can be retrieved through the
    *       getDsoList() method.
    *
    * @see getDsoList, loadAndInitDSO
    */
   vpr::ReturnStatus findAndInitDSO(const std::string& dsoBaseName,
                                    const std::vector<std::string>& searchPath,
                                    const std::string& initFuncName,
                                    boost::function1<bool, void*> initFunc);

   /**
    * Loads the dynamic shared object (DSO) from the given vpr::LibraryPtr
    * object and invokes its initialization function.  This method serves
    * to hide the details of looking up and invoking the initialization
    * function.  No extra steps are taken with the vpr::Library object to
    * handle platform-specific issues.  It is assumed that by this point,
    * the file has been found on the local file system and is ready to be
    * loaded.
    *
    * @pre vpr::LibraryPtr contains a pointer to a valid vpr::Library object.
    *
    * @param dso          A pointer to the vpr::Library object holding the
    *                     DSO information.
    * @param initFuncName The name of the initialization function in the DSO
    *                     that will be invoked to initialize the DSO's code.
    * @param initFunc     A Boost.Function object (that is, a callable object)
    *                     that will be invoked as a callback to initialize the
    *                     DSO.  It will be passed a void* pointer to the
    *                     initialization function looked up in the DSO.
    */
   vpr::ReturnStatus loadAndInitDSO(vpr::LibraryPtr dso,
                                    const std::string& initFuncName,
                                    boost::function1<bool, void*> initFunc);

   vpr::ReturnStatus unloadDSO(vpr::LibraryPtr dso);

   const dso_list_t& getDsoList() const
   {
      return mLoadedLibs;
   }

private:
   dso_list_t mLoadedLibs;
};

}


#endif /* _VPR_LIBRARY_LOADER_H_ */
