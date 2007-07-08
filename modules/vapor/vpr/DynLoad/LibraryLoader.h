/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

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

#ifndef _VPR_LIBRARY_LOADER_H_
#define _VPR_LIBRARY_LOADER_H_

#include <vpr/vprConfig.h>

#include <string>
#include <vector>
#include <boost/function.hpp>
#include <boost/filesystem/path.hpp>

#include <vpr/DynLoad/Library.h>


namespace vpr
{

/** \class LibraryLoader LibraryLoader.h vpr/DynLoad/LibraryLoader.h
 *
 * A helper for loading a specific class of dynamic shared objects (DSOs).
 * Through (relatively) high-level methods, this class introduces a uniform
 * naming scheme for DSOs.  More details are provided in the documentation for
 * individual methods.  In particular, refer to the documentation for
 * makeFullDSOName.
 */
class VPR_CLASS_API LibraryLoader
{
public:
   /**
    * Searches for a dynamic shared object (DSO) with the "base" name given in
    * dsoBaseName.   The platform-specific name will be determined using
    * makeFullDSOName().  The search will be performed using the list of
    * directories given in searchPath.
    *
    * @param dsoBaseName The base name of the DSO to be loaded.  The
    *                    platform-specific file extension will be appended
    *                    prior to the search.
    * @param searchPath  A list of directories where the target DSO may
    *                    exist.  The paths given must be platform-specific
    *                    (i.e., native) paths.
    *
    * @see makeFullDSOName
    */
   static vpr::LibraryPtr findDSO(const std::string& dsoBaseName,
                                  const std::vector<std::string>& searchPath);

   /**
    * Searches for a dynamic shared object (DSO) with the "base" name given in
    * dsoBaseName.   The platform-specific name will be determined using
    * makeFullDSOName().  The search will be performed using the list of
    * directories given in searchPath.
    *
    * @param dsoBaseName The base name of the DSO to be loaded.  The
    *                    platform-specific file extension will be appended
    *                    prior to the search.
    * @param searchPath  A list of directories (specified as Boost.Filesystem
    *                    path objects) where the target DSO may exist.
    *
    * @see makeFullDSOName
    */
   static vpr::LibraryPtr findDSO(const std::string& dsoBaseName,
                                  const std::vector<boost::filesystem::path>& searchPath);

   /**
    * Searches for a dynamic shared object (DSO) with the "base" name given in
    * dsoBaseName, looks up the named function, and invokes the given callback
    * with that function as its only argument.  The platform-specific name
    * will be determined using makeFullDSOName().  The search will be
    * performed using the list of directories given in searchPath.
    *
    * @param dsoBaseName The base name of the DSO to be loaded.  The
    *                    platform-specific file extension will be appended
    *                    prior to the search.
    * @param searchPath  A list of directories where the target DSO may
    *                    exist.  The paths given must be platform-specific
    *                    (i.e., native) paths.
    * @param funcName    The name of the initialization function in the DSO
    *                    that will be invoked to initialize the DSO's code.
    * @param callback    A Boost.Function object (that is, a callable object)
    *                    that will be invoked as a callback to initialize the
    *                    DSO.  It will be passed a void* pointer to the
    *                    initialization function looked up in the DSO.
    * @param dso         A reference to a vpr::LibraryPtr object through which
    *                    the loaded DSO will be returned to the caller.
    *
    * @see makeFullDSOName, callEntryPoint
    *
    * @since 0.92.3
    *
    * @throw vpr::SymbolLookupException is thrown if \p funcName cannot be
    *        found in the discovered DSO.
    * @throw vpr::LibraryException is thrown if anything else goes wrong.
    *
    * @note The user callback may throw exceptions of its own.
    */
   static void findDSOAndCallEntryPoint(
      const std::string& dsoBaseName,
      const std::vector<std::string>& searchPath, const std::string& funcName,
      boost::function1<bool, void*> callback, vpr::LibraryPtr& dso
   );

   /**
    * Searches for a dynamic shared object (DSO) with the "base" name given in
    * dsoBaseName, looks up the named function, and invokes the given callback
    * with that function as its only argument.  The platform-specific name
    * will be determined using makeFullDSOName().  The search will be
    * performed using the list of directories given in searchPath.
    *
    * @param dsoBaseName The base name of the DSO to be loaded.  The
    *                    platform-specific file extension will be appended
    *                    prior to the search.
    * @param searchPath  A list of directories (specified as Boost.Filesystem
    *                    path objects) where the target DSO may exist.
    * @param funcName    The name of the initialization function in the DSO
    *                    that will be invoked to initialize the DSO's code.
    * @param callback    A Boost.Function object (that is, a callable object)
    *                    that will be invoked as a callback to initialize the
    *                    DSO.  It will be passed a void* pointer to the
    *                    initialization function looked up in the DSO.
    * @param dso         A reference to a vpr::LibraryPtr object through which
    *                    the loaded DSO will be returned to the caller.
    *
    * @see makeFullDSOName, callEntryPoint
    *
    * @since 0.92.3
    *
    * @throw vpr::SymbolLookupException is thrown if \p funcName cannot be
    *        found in the discovered DSO.
    * @throw vpr::LibraryException is thrown if anything else goes wrong.
    *
    * @note The user callback may throw exceptions of its own.
    */
   static void findDSOAndCallEntryPoint(
      const std::string& dsoBaseName,
      const std::vector<boost::filesystem::path>& searchPath,
      const std::string& funcName, boost::function1<bool, void*> callback,
      vpr::LibraryPtr& dso
   );

   /**
    * Looks up the named function, and invokes the given callback with
    * that function as its only argument.  If the DSO has not already been
    * loaded, this happens first.  This method serves to hide the details of
    * looking up the initialization function.  No extra steps are taken with
    * the vpr::Library object to handle platform-specific issues.  It is
    * assumed that by this point, the file has been found on the local file
    * system and is ready to be loaded or is already loaded.
    *
    * This method is essentially just a wrapper around invoking
    * vpr::Library::findSymbol() and handling possible errors associated
    * with doing so.  In many cases, it is better to call
    * vpr::Library::findSymbol() directly to avoid the complications of using
    * the callback to handle invoking the entry point function.
    *
    * @pre vpr::LibraryPtr contains a pointer to a valid vpr::Library object.
    * @post The DSO is loaded.
    *
    * @param dso      A pointer to the vpr::Library object holding the
    *                 DSO information.
    * @param funcName The name of the initialization function in the DSO
    *                 that will be invoked to initialize the DSO's code.
    * @param callback A Boost.Function object (that is, a callable object)
    *                 that will be invoked as a callback to initialize the
    *                 DSO.  It will be passed a void* pointer to the
    *                 initialization function looked up in the DSO.
    *
    * @note This method was renamed from findEntryPoint() in VPR 0.92.3.
    *
    * @since 0.92.3
    *
    * @throw vpr::SymbolLookupException is thrown if \p funcName cannot be
    *        found in \p dso.
    * @throw vpr::LibraryException is thrown if \p dso is an invalid pointer
    *        or if \p callback returns false.
    *
    * @note The user callback may throw exceptions of its own.
    */
   static void callEntryPoint(vpr::LibraryPtr dso, const std::string& funcName,
                              boost::function1<bool, void*> callback);

   /**
    * Constructs the platform-specific DSO name using the given "base" name.
    * The DSO base name is a platform-independent, abstract name that makes up
    * a substring of the name of the actual file that will be loaded.  A
    * platform-specific extension (.so, .dll, etc) will be appended, and for
    * an instantiation of this class in debug-enabled code, the suffix "_d"
    * will be appended to the DSO's base name.  For example, given the base
    * name mydso, a debug build on Windows would search for the file
    * mydso_d.dll.  For a release (optimized) build, it would search for
    * mysdso.dll.  The determination of which name extension to use is made
    * at compile time, but it can be customized using setDSONameExt().  On
    * non-Windows platforms, the debug version has no special naming
    * convention by default.
    *
    * @param dsoBaseName The base name of the DSO to be loaded.
    *
    * @see setDSONameExt()
    */
   static std::string makeFullDSOName(const std::string& dsoBaseName);

   /**
    * Changes the name extension applied when makeFullDSOName() constructs
    * the name of the shared library to load.
    *
    * @parram nameExt The new name extension (such as "_x") that will be used
    *                 by makeFullDSOName().
    *
    * @since 2.1.2
    *
    * @see makeFullDSOName()
    */
   static void setDSONameExt(const std::string& nameExt)
   {
      sDsoNameExt = nameExt;
   }

   /**
    * Retrieves the current name extension applied when makeFullDSOName()
    * constructs the name of the shared library to load.
    *
    * @since 2.1.2
    *
    * @see makeFullDSOName()
    */
   static const std::string& getDSONameExt()
   {
      return sDsoNameExt;
   }

private:
   static void makeBoostFsVector(const std::vector<std::string>& strVec,
                                 std::vector<boost::filesystem::path>& boostFsVec);

   /**
    * @since 2.1.2
    */
   static std::string sDsoNameExt;
};

}


#endif /* _VPR_LIBRARY_LOADER_H_ */
