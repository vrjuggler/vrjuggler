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
#ifndef GADGET_RIM_DEP_CHECKER_H_
#define GADGET_RIM_DEP_CHECKER_H_

#include <gadget/gadgetConfig.h>

// Dependency checker includes
#include <jccl/RTRC/DepChecker.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <vpr/Util/Debug.h>
#include <jccl/Config/ConfigChunk.h>

namespace cluster
{

/**
 * Dependency checker for Remote Input Manager.
 * Implement the basic stuff plus a check for whether
 * the system knows about Remote Input Manager yet.
 */
class GADGET_CLASS_API ClusterDepChecker : public jccl::DepChecker
{
public:
   ClusterDepChecker()
   {;}

   /**
    * Returns a string name of the checker.
    * Used to output messages in checker listings.
    */
   virtual std::string getCheckerName()
   { return std::string("Cluster Dependency Checker"); }

   /** We can handle only remote device configuration information. */
   virtual bool canHandle(jccl::ConfigChunkPtr chunk);

   /**
    * Are the dependencies satisfied?
    * Check whether the display system chunk is in the active config.
    * @return true if default dependencies are satisfied and the Input Manager
    *         has Remote Input Manager chunk.
    */
   virtual bool depSatisfied(jccl::ConfigChunkPtr chunk);

   /** Write out the dependencies to the vprDEBUG macro. */
   virtual void debugOutDependencies(jccl::ConfigChunkPtr chunk,int dbg_lvl=vprDBG_WARNING_LVL);

};

}  // end namespace gadget

#endif
