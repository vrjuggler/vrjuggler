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

#ifndef _VRJ_ENVIRONMENT_MANAGER_H_
#define _VRJ_ENVIRONMENT_MANAGER_H_

#include <vrj/vrjConfig.h>

#include <jccl/Net/JackalServer.h>
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/PerfMonitor/PerformanceMonitor.h>


namespace vrj
{

/**
 * Communications and configuration agent.
 * The Environment Manager handles communications between
 * Juggler and UI elements.  This includes data logging and
 * interactive connections to the VR Juggler control program,
 * VjControl.
 * The EM's most important function is to communicate
 * configuration information and changes between the GUI and
 * the Kernel.  It is also the owner and manager of Juggler
 * Performance measurement code.
 * A near-future addition will be device status/data monitoring.
 *
 * Which means that its main duties are:
 *    - handle file and socket connections
 *    - handle PerfDataBuffers
 *
 * @date 2-27-98
 */
class VJ_CLASS_API EnvironmentManager: public jccl::ConfigChunkHandler
{

public:

   /**
    * Constructor.
    *
    * @pre None
    * @post Object is constructed
    */
   EnvironmentManager ()
   {
      jccl::JackalServer::instance()->addJackalControl(jccl::ConfigManager::instance());
      jccl::JackalServer::instance()->addJackalControl(jccl::PerformanceMonitor::instance());

      jccl::ConfigManager::instance()->addConfigChunkHandler(jccl::JackalServer::instance());
      jccl::ConfigManager::instance()->addConfigChunkHandler(jccl::PerformanceMonitor::instance());
   }

   virtual ~EnvironmentManager ()
   {
      /* Do nothing. */ ;
   }

   jccl::ConfigManager* getConfigManager ()
   {
      return jccl::ConfigManager::instance();
   }

   jccl::PerformanceMonitor* getPerformanceMonitor ()
   {
      return jccl::PerformanceMonitor::instance();
   }

   /**
    * jccl::ConfigChunkHandler stuff.
    *
    * @return false
    */
   virtual bool configAdd (jccl::ConfigChunkPtr chunk)
   {
      return false;
   }

   /**
    * Removes the chunk from the current configuration.
    *
    * @return false
    */
   virtual bool configRemove (jccl::ConfigChunkPtr chunk)
   {
      return false;
   }

   /**
    * Can the handler handle the given chunk?
    *
    * @return false
    */
   virtual bool configCanHandle (jccl::ConfigChunkPtr chunk)
   {
      return false;
   }

private:

   // These are needed to appease Visual C++ in its creation of DLLs.
   EnvironmentManager(const EnvironmentManager&)
      : ConfigChunkHandler() 
   {;}
   void operator=(const EnvironmentManager&) {;}

};

} // End of vrj namespace


#endif /* _VRJ_ENVIRONMENT_MANAGER_H_ */
