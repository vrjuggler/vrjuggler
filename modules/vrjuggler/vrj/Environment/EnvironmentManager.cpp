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
#include <vrj/Environment/EnvironmentManager.h>


namespace vrj {

EnvironmentManager::EnvironmentManager() {

    jackal_server = jccl::JackalServer::instance();
    config_manager = jccl::ConfigManager::instance();
    performance_monitor = jccl::PerformanceMonitor::instance();

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
bool EnvironmentManager::configAdd(jccl::ConfigChunkPtr chunk) {
    return false;
}



//: Remove the chunk from the current configuration
//! PRE: configCanHandle(chunk) == true
//!RETURNS: success
bool EnvironmentManager::configRemove(jccl::ConfigChunkPtr chunk) {
    return false;
}



//: Can the handler handle the given chunk?
//! RETURNS: true - Can handle it
//+          false - Can't handle it
bool EnvironmentManager::configCanHandle(jccl::ConfigChunkPtr chunk) {
    return false;
}



};
