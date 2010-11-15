/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _JCCL_DEP_MGR_H_
#define _JCCL_DEP_MGR_H_

#include <jccl/jcclConfig.h>
#include <vector>

#include <vpr/Util/Singleton.h>
#include <jccl/RTRC/DepChecker.h>
#include <jccl/Config/ConfigElementPtr.h>

namespace jccl
{

/** \class DependencyManager DependencyManager.h jccl/RTRC/DependencyManager.h
 *
 * A dependency checker for dynamically added components.  The Dependency
 * Manager works in conjunction with the Config Manager, and checks for
 * dependencies for any configuration add request.  The default behavior is to
 * simply check the Config Manager's active list for all jccl::ConfigElement
 * objects that the request's jccl::ConfigElement refers to.  However,
 * additional DepCheckers can be registered to provide specialized
 * functionality.
 *
 * Note that this class is a singleton, primarily so that the master list of
 * DepCheckers can be appended to by any entity in the system.
 *
 * @see jccl::ConfigManager
 */
class JCCL_CLASS_API DependencyManager
{
private:
   /** Constructor. Private since this is a singleton. */
   DependencyManager ();

   ~DependencyManager();

public:

   /** Registers a new dependency checker. */
   void registerChecker (DepChecker* checker);

   /**
    * Un-registers a new dependency checker.
    *
    * @since 1.1.2
    */
   void unregisterChecker(DepChecker* checker);

   /** Checks if dependencies are satisfied for the given element.
    *  @return true iff dependencies for element are satisfied.
    */
   bool isSatisfied (ConfigElementPtr element);

   /** Prints information about element's dependencies. */
   void debugOutDependencies (ConfigElementPtr element, int dbg_lvl);

private:

   /** Finds a DepChecker that can handle element.
    *  DepCheckers list the jccl::ConfigElement types that they know how to
    *  handle; this is checked versus element.getDefToken().
    *  @return A DepChecker that knows how to handle element.  If no
    *          specific checker is found, the default checker is
    *          returned.  If multiple specific DepCheckers claim to
    *          be able to handle element, the choice is implementation
    *          dependent.
    */
   DepChecker* findDepChecker(ConfigElementPtr element);

   /** Prints information about the DependencyManager's state to vprDEBUG. */
   void debugDump();

private:

   /** List of dependency checkers. */
   std::vector<DepChecker*> mDepCheckers;

   /** Default dependency checker. */
   DepChecker mDefaultChecker;

   vprSingletonHeader(DependencyManager);

}; // class DependencyManager

} // namespace jccl


#endif
