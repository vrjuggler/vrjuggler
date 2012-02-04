/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


#ifndef _JCCL_DEP_CHECKER_H_
#define _JCCL_DEP_CHECKER_H_

#include <jccl/jcclConfig.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <vpr/Util/Debug.h>

namespace jccl
{


/** \class DepChecker DepChecker.h jccl/RTRC/DepChecker.h
 *
 * Base class for dependency checkers.  A dependency checker is responsible for
 * figuring out if the system has all the required dependencies filled for a
 * given jccl::ConfigElement.
 *
 * This class also implements a default behavior for dependency checkers, which
 * just looks at any jccl::ConfigElement pointer properties in the given
 * element and returns true if all the elements named by those properties exist
 * in the ConfigManager's current configuration.
 *
 * Configuration information with special dependency requirements should
 * specialize this class and register an instance with the Dependency Manager.
 *
 * @note The DepChecker must be registered BEFORE a element of the given type
 *       is checked for dependencies.
 */
class JCCL_API DepChecker
{
public:
   /** Constructor. */
   DepChecker();

   /** Destructor. */
   virtual ~DepChecker()
   {;}

   /** Return a string name for the checker.
    *  This is used to output messages in checker listings.
    */
   virtual std::string getCheckerName();

   /** Checks if this DepChecker knows how to handle element.
    *  @return true if we understand the element.  The default
    *          behavior returns true (the default checker can
    *          check anything).
    */
   virtual bool canHandle(ConfigElementPtr element);

   /** Checks if element's dependencies are satisfied.
    *  @return true iff element's dependencies are satisfied.
    */
   virtual bool depSatisfied(ConfigElementPtr element);

   /** Write element's dependencies to vprDEBUG.
    *  @param element  The element whose dependencies will be printed.
    *  @param dbgLevel Output priority level for vprDEBUG.
    */
   virtual void debugOutDependencies(ConfigElementPtr element,
                                      int dbgLevel = vprDBG_WARNING_LVL);

}; // class DepChecker

} // namespace jccl


#endif
