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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


#ifndef _JCCL_DEP_MGR_H_
#define _JCCL_DEP_MGR_H_

#include <jccl/jcclConfig.h>
#include <vector>
#include <jccl/RTRC/DepChecker.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <typeinfo>
#include <vpr/Util/Singleton.h>

namespace jccl
{

/** A dependency checker for dynamically added components.
 *  The DependencyManager works in conjunction with the
 *  ConfigManager, and checks for dependencies for any
 *  configuration add request.
 *  The default behavior is to simply check the ConfigManager's
 *  active list for all ConfigChunk's that the request's
 *  ConfigChunk refers to.  However, additional DepCheckers
 *  can be registered to provide specialized functionality.
 *
 *  Note that this class is a singleton - primarily so that the
 *  master list of DepCheckers can be appended to by any entity
 *  in the system.
 */
class JCCL_CLASS_API DependencyManager
{

private:
   /** Constructor. Private since this is a singleton. */
   DependencyManager ();


public:

   /** registers a new dependency checker. */
   void registerChecker (DepChecker* checker);


   /** Checks if dependencies are satisfied for the given chunk.
    *  @return true iff dependencies for chunk are satisfied.
    */
   bool isSatisfied (ConfigChunkPtr chunk);


   /** Prints information about chunk's dependencies. */
   void debugOutDependencies (ConfigChunkPtr chunk, int dbg_lvl);


private:

   /** Finds a DepChecker that can handle chunk.
    *  DepCheckers list the ConfigChunk types that they know how to
    *  handle; this is checked versus chunk.getDescToken().
    *  @return A DepChecker that knows how to handle chunk.  If no
    *          specific checker is found, the default checker is
    *          returned.  If multiple specific DepCheckers claim to
    *          be able to handle chunk, the choice is implementation
    *          dependent.
    */
   DepChecker* findDepChecker(ConfigChunkPtr chunk);


   /** Prints information about the DependencyManager's state to vprDEBUG. */
   void debugDump();


private:

   /** List of dependency checkers. */
   std::vector<DepChecker*> mDepCheckers;

   /** Default dependency checker. */
   DepChecker               mDefaultChecker;

   vprSingletonHeader(DependencyManager);


}; // class DependencyManager

} // namespace jccl


#endif
