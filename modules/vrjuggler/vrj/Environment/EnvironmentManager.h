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
#include <vpr/Sync/Mutex.h>
#include <vpr/Thread/Thread.h>
#include <jccl/JackalServer/JackalServer.h>
#include <jccl/Plugins/ConfigManager/ConfigManager.h>
//#include <jccl/Plugins/ConfigManager/ConfigChunkHandler.h>
#include <jccl/Plugins/PerformanceMonitor/PerformanceMonitor.h>

namespace vrj
{



//-------------------------------------
//: Communications and configuration agent
//
//      The Environment Manager handles communications between
//      Juggler and UI elements.  This includes data logging and
//      interactive connections to the VR Juggler control program,
//      Control.
//      The EM's most important function is to communicate
//      configuration information and changes between the GUI and
//      the Kernel.  It is also the owner and manager of Juggler
//      Performance measurement code.
//      A near-future addition will be device status/data monitoring.
//
//      Which means that its main duties are:
//         - handle file and socket connections
//         - handle PerfDataBuffers
//
// @author  Christopher Just
//
// Date 2-27-98
//---------------------------------------

class VJ_CLASS_API EnvironmentManager: public jccl::ConfigChunkHandler {

public:

    //: constructor
    //! PRE: None
    //! POST: Object is constructed
    EnvironmentManager();



    virtual ~EnvironmentManager();



    jccl::ConfigManager* getConfigManager();

    jccl::PerformanceMonitor* getPerformanceMonitor();


    //: jccl::ConfigChunkHandler stuff
    //! PRE: configCanHandle(chunk) == true
    //! RETURNS: success
    virtual bool configAdd(jccl::ConfigChunkPtr chunk);



    //: Remove the chunk from the current configuration
    //! PRE: configCanHandle(chunk) == true
    //!RETURNS: success
    virtual bool configRemove(jccl::ConfigChunkPtr chunk);



    //: Can the handler handle the given chunk?
    //! RETURNS: true - Can handle it
    //+          false - Can't handle it
    virtual bool configCanHandle(jccl::ConfigChunkPtr chunk);



private:

    jccl::JackalServer*             jackal_server;
    jccl::ConfigManager*            config_manager;
    jccl::PerformanceMonitor*       performance_monitor;

    // These are needed to appease Visual C++ in its creation of DLLs.
    EnvironmentManager(const EnvironmentManager&) {;}
    void operator=(const EnvironmentManager&) {;}

}; // end EnvironmentManager



};
#endif
