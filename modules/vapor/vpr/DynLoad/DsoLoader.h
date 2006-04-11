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

#ifndef _VPR_DSO_LOADER_H_
#define _VPR_DSO_LOADER_H_

#include <vpr/vprConfig.h>

#include <stdlib.h>
#include <string>
#include <vector>

#include <vpr/DynLoad/Library.h>
#include <vpr/DynLoad/LoaderError.h>


namespace vpr
{

/**
 * A helper class built on top of vpr::Library and vpr::LibraryFinder.  This
 * class is designed to offload the repetitive work of loading dynamic
 * shared objects (DSOs), looking up their factory function, and invoking the
 * factory function.  The factory function must have the signature
 * (void (*)(const char*, void*)) in order for proper invocation.  However, the
 * factory function can have a user-specified name.  It is this name that is
 * actually looked up in the DSO.
 *
 * @see vpr::Library
 * @see vpr::LibraryFinder
 */
class VPR_CLASS_API DsoLoader
{
public:
   typedef void (*InitFuncType)(const char*, void*);

   /**
    * Simple holder for the arguments that will be passed to the DSO's
    * factory function.
    */
   struct CreatorArg
   {
      CreatorArg() : paramType(""), param(NULL)
      {
         /* Do nothing. */ ;
      }

      CreatorArg(const std::string& t, void* p) : paramType(t), param(p)
      {
         /* Do nothing. */ ;
      }

      std::string paramType;   /**< The string form of param's type. */
      void*       param;       /**< The parameter needed for DSO initialization. */
   };

   /** Default constructor. */
   DsoLoader();

   /**
    * Non-loading constructor.
    *
    * @param initFuncName The name of the factory function that will be looked
    *                     up when a DSO is loaded.
    */
   DsoLoader(const std::string& initFuncName);

   /**
    * Single-load constructor.  This will load the named dynamic shared object
    * file and calls the named factory function.
    *
    * @post The named DSO will be loaded or an exception will be thrown that
    *       indicates what went wrong.
    *
    * @param initFuncName The name of the factory function that will be looked
    *                     up when a DSO is loaded.
    * @param dsoName      The name of DSO file that will be loaded.
    * @param creatorArg   The argument that will be passed to the DSO's factory
    *                     function.
    */
   DsoLoader(const std::string& initFuncName, const std::string& dsoName,
             CreatorArg& creatorArg)
      throw(vpr::LoaderError);

   /**
    * Multi-load constructor.  This will load the given list of dynamic shared
    * objects.
    *
    * @post All valid dynamic shared objects in dsoList will be loaded and
    *       initialized.
    *
    * @param initFuncName The name of the factory function that will be looked
    *                     up when a DSO is loaded.
    * @param dsoList      The list of dynamic shared objects to load.  This
    *                     should be the name(s) of the file(s).
    * @param creatorArg   The argument that will be passed to the DSO's factory
    *                     function.
    *
    * @note Currently, there is no good way to determiine if all DSOs in
    *       dsoList were loaded successfully.
    */
   DsoLoader(const std::string& initFuncName,
             const std::vector<std::string>& dsoList,
             CreatorArg& creatorArg);

   /** Copy constructor. */
   DsoLoader(const DsoLoader& obj)
      : mInitFuncName(obj.mInitFuncName), mDsoList(obj.mDsoList)
   {
   }

   /**
    * Destructor.
    *
    * @post The loaded DSOs will be unloaded.
    */
   ~DsoLoader()
   {
      /* Do nothing. */ ;
   }

   /** Sets the name of the DSO factory function. */
   void setInitFuncName(const std::string& name)
   {
      mInitFuncName = name;
   }

   /** Returns the name of the DSO factory function. */
   const std::string& getInitFuncName() const
   {
      return mInitFuncName;
   }

   /**
    * Loads the named dynamic shared object, using the given argument as the
    * parameter to the DSO factory function.
    *
    * @pre The factory function name has been set.
    * @post The named DSO will be loaded or a vpr::LoaderError exception will
    *       be thrown indicating what went wrong.
    *
    * @param dsoName    The name of DSO file that will be loaded.
    * @param creatorArg The argument that will be passed to the DSO's factory
    *                   function.
    *
    * @throws vpr::LoaderError
    */
   void load(const std::string& dsoName, CreatorArg& creatorArg)
      throw(vpr::LoaderError);

   // XXX: Should this throw an exception or something?
   /**
    * Loads the given list of dynamic shared objects, using the given
    * argument as the parameter to the DSO factory function.
    *
    * @pre The factory function name has been set.
    *
    * @param dsoList    The names of DSOs that will be loaded.
    * @param creatorArg The argument that will be passed to the DSO's factory
    *                   function.
    *
    * @note Currently, there is no good way to determiine if all DSOs in
    *       dsoList were loaded successfully.
    */
   void load(const std::vector<std::string>& dsoList, CreatorArg& creatorArg);

   /**
    * Loads all the dynamic shared objects found in the given directory.
    * A DSO is identified by ending in the given file extension (dsoExt).
    *
    * @pre The factory function name has been set.
    *
    * @param dirName    The name of the directory containing the DSO's that
    *                   will be loaded.
    * @param dsoExt     The platform-specific extension that is used to
    *                   identify shared libraries.  This should be of the form
    *                   "so" (for most UNIX-based systems) or "dll" (for
    *                   Win32).  Note that the "." is not included in the
    *                   enxtension string.
    * @param creatorArg The argument that will be passed to the DSO's factory
    *                   function.
    *
    * @note Currently, there is no good way to determiine if all DSOs in
    *       dirName were loaded successfully.
    */
   void loadFromDirectory(const std::string& dirName,
                          const std::string& dsoExt, CreatorArg& creatorArg);

   unsigned int getDsoCount() const
   {
      return mDsoList.size();
   }

private:
   /**
    * Performs the actual DSO loading.
    *
    * @pre The factory function name has been set.
    *
    * @param dsoPtr     The vpr::Library object that will be used for loading
    *                   the DSO from disk.
    * @param creatorArg The argument that will be passed to the factory
    *                   function.
    *
    * @throws vpr::LoaderError
    */
   void loadDSO(vpr::LibraryPtr dsoPtr, CreatorArg& creatorArg)
      throw(vpr::LoaderError);

   std::string                  mInitFuncName;
   std::vector<vpr::LibraryPtr> mDsoList;
};

} // End of vpr namespace


#endif /* _VPR_DSO_LOADER_H_ */
