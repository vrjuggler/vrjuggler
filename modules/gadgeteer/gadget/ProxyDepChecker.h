/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#ifndef _VRJ_PROXY_DEP_CHECKER_H_
#define _VRJ_PROXY_DEP_CHECKER_H_

#include <vrj/vrjConfig.h>

// Dependency checker includes
#include <vrj/Kernel/DependencyManager.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Kernel/DepChecker.h>


namespace vrj
{

//: Dependency checker for Proxies
class ProxyDepChecker : public DepChecker
{
public:
   ProxyDepChecker()
   {;}

   //: Return a string name of the checker
   // Used to output messages in checker listings
   virtual std::string getCheckerName()
   { return std::string("vjProxyChecker Checker"); }

   // We can handle only keyboard configuration information
   virtual bool canHandle(ConfigChunk* chunk);

   //: Are the dependencies satisfied?
   // Defaults to all being handled for it
   virtual bool depSatisfied(ConfigChunk* chunk)
   {
      return true;
   }

   // Write out the dependencies to the vjDEBUG macro
   virtual void debugOutDependencies(ConfigChunk* chunk,int dbg_lvl=vjDBG_WARNING_LVL)
   {
      ;
   }
};

};

#endif
