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

// implementation of Environment Manager
//
// author: Christopher Just

#include <vrj/vrjConfig.h>

//  #include <vrj/Kernel/Kernel.h>
//  #include <vrj/Environment/Connect.h>
//  #include <vrj/Performance/PerfDataBuffer.h>
//  #include <vrj/Config/jccl::ChunkDescDB.h>
//  #include <vrj/Config/jccl::ConfigChunkDB.h>
//  #include <vrj/Config/ParseUtil.h>
//  #include <vrj/Environment/TimedUpdate.h>

//  #include <vrj/Kernel/ConfigManager.h>
#include <vrj/Environment/EnvironmentManager.h>

namespace vrj
{

EnvironmentManager::EnvironmentManager() {

    jackal_server = new jccl::JackalServer();
    config_manager = jccl::ConfigManager::instance();
    performance_monitor = new jccl::PerformanceMonitor();

    jackal_server->addJackalControl (config_manager);
    jackal_server->addJackalControl (performance_monitor);

    config_manager->addConfigChunkHandler (jackal_server);
    config_manager->addConfigChunkHandler (performance_monitor);

}



EnvironmentManager::~EnvironmentManager() {
    // delete stuff
}



jccl::ConfigManager* EnvironmentManager::getConfigManager() {
    return config_manager;
}


jccl::PerformanceMonitor* EnvironmentManager::getPerformanceMonitor() {
    return performance_monitor;
}


//: jccl::ConfigChunkHandler stuff
//! PRE: configCanHandle(chunk) == true
//! RETURNS: success
bool EnvironmentManager::configAdd(jccl::ConfigChunk* chunk) {
    return false;

//      bool networkingchanged = false;
//      int newport;

//      std::string s = chunk->getType();
//      if (!vjstrcasecmp (s, "EnvironmentManager")) {
//          configured_to_accept = chunk->getProperty ("AcceptConnections");
//          newport = chunk->getProperty("Port");

//          if (newport == 0)
//              newport = Port;
//          if ((newport != Port) || (configured_to_accept != isAccepting()))
//              networkingchanged = true;
//          perf_target_name = (std::string)chunk->getProperty ("PerformanceTarget");
//          connections_mutex.acquire();

//          Connect* new_perf_target = getConnect(perf_target_name);
//          if (new_perf_target != perf_target)
//              setPerformanceTarget (NULL);

//          if (networkingchanged) {
//              Port = newport;
//              if (isAccepting())
//                  rejectConnections();
//              if (configured_to_accept)
//                  acceptConnections();
//              else
//                  killConnections();
//          }
//          if (new_perf_target)
//              setPerformanceTarget(new_perf_target);
//          connections_mutex.release();

//          return true;
//      }
//      else if (!vjstrcasecmp (s, "PerfMeasure")) {
//          current_perf_config = new jccl::ConfigChunk (*chunk);
//          perf_buffers_mutex.acquire();
//          activatePerfBuffers();
//          perf_buffers_mutex.release();
//          return true;
//      }
//      else if (!vjstrcasecmp (s, "FileConnect")) {
//          // I wanted to just look if the fileconnect had been added yet.
//          // however I seem to have a chicken/egg problem.
//          // so the kludge we'll do now is to not directly add a chunk that's
//          // of type VJC_INTERACTIVE. sigh.
//          // Unfortunately, this means that for other cases (such as attaching
//          // to a named pipe) we're still broken
//          if ((int)chunk->getProperty("Mode") != VJC_INTERACTIVE) {
//              // it's new to us
//              Connect* vn = new Connect (chunk);
//              vprDEBUG (vrjDBG_ENV_MGR, 1) << "EM adding connection: " << vn->getName().c_str() << '\n'
//                                         << vprDEBUG_FLUSH;
//              connections_mutex.acquire();
//              connections.push_back (vn);
//              vn->startProcess();
//              if (!vjstrcasecmp (vn->getName(), perf_target_name))
//                  setPerformanceTarget (vn);
//              connections_mutex.release();
//          }
//          return true;
//      }
//      return false;
}



//: Remove the chunk from the current configuration
//! PRE: configCanHandle(chunk) == true
//!RETURNS: success
bool EnvironmentManager::configRemove(jccl::ConfigChunk* chunk) {
    return false;
//      std::string s = chunk->getType();
//      if (!vjstrcasecmp (s, "EnvironmentManager")) {
//          // this could be trouble if the chunk being removed isn't the chunk
//          // we were configured with...
//          rejectConnections();
//          Port = 4450;
//          configured_to_accept = false;
//          return true;
//      }
//      else if (!vjstrcasecmp (s, "PerfMeasure")) {
//          if (current_perf_config) {
//              if (!vjstrcasecmp (current_perf_config->getProperty ("Name"),
//                                 chunk->getProperty ("Name"))) {
//                  delete (current_perf_config);
//                  current_perf_config = NULL;
//                  connections_mutex.acquire();
//                  deactivatePerfBuffers ();
//                  connections_mutex.release();
//              }
//          }
//          return true;
//      }
//      else if (!vjstrcasecmp (s, "FileConnect")) {
//          vprDEBUG (vrjDBG_ENV_MGR,1) << "EM Removing connection: "
//                                    << chunk->getProperty ("Name") << '\n' << vprDEBUG_FLUSH;
//          connections_mutex.acquire();
//          Connect* c = getConnect (chunk->getProperty ("Name"));
//          if (c) {
//              removeConnect (c);
//          }
//          connections_mutex.release();
//          vprDEBUG (vrjDBG_ENV_MGR,4) << "EM completed connection removal\n" << vprDEBUG_FLUSH;
//          return true;
//      }

//      return false;
}



//: Can the handler handle the given chunk?
//! RETURNS: true - Can handle it
//+          false - Can't handle it
bool EnvironmentManager::configCanHandle(jccl::ConfigChunk* chunk) {
    return false;
//      std::string s = chunk->getType();
//      return (!vjstrcasecmp (s, "EnvironmentManager") ||
//              !vjstrcasecmp (s, "PerfMeasure") ||
//              !vjstrcasecmp (s, "FileConnect"));
}



};
