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


#ifndef _JCCL_JACKAL_SERVER_H_
#define _JCCL_JACKAL_SERVER_H_

#include <jccl/jcclConfig.h>
#include <jccl/Plugins/ConfigManager/ConfigChunkHandler.h>
#include <jccl/JackalServer/Socket.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Sync/Mutex.h>

namespace jccl {

class Connect;
class JackalControl;
class ConfigChunkDB;


//-------------------------------------
//: Communications and configuration agent
//
//      The Environment Manager handles communications between
//      Juggler and UI elements.  This includes data logging and
//      interactive connections to the VR Juggler control program,
//      vjControl.
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

class JCCL_CLASS_API JackalServer: public ConfigChunkHandler {

public:

    //: constructor
    //! PRE: None
    //! POST: Object is constructed
    JackalServer();



    virtual ~JackalServer();



    //: is the EM accepting connections across the network?
    //! RETURNS: true - EM is accepting connections
    //! RETURNS: false - EM is not accepting connections
    bool isAccepting();



    //: registers a Jackal control unit. 
    void addJackalControl (JackalControl* jc);


    //: unregisters a Jackal control unit.
    void removeJackalControl (JackalControl* jc);



    //: tells EM that a connection has died (ie by gui disconnecting)
    void connectHasDied (Connect* con);


    //: sends a 'refresh' message to all open connections
    void sendRefresh();



    //: ConfigChunkHandler stuff
    //! PRE: configCanHandle(chunk) == true
    //! RETURNS: success
    virtual bool configAdd(ConfigChunk* chunk);



    //: Remove the chunk from the current configuration
    //! PRE: configCanHandle(chunk) == true
    //!RETURNS: success
    virtual bool configRemove(ConfigChunk* chunk);


    
    //: Can the handler handle the given chunk?
    //! RETURNS: true - Can handle it
    //+          false - Can't handle it
    virtual bool configCanHandle(ConfigChunk* chunk);



private:
    std::string               perf_target_name;
    std::vector<Connect*>   connections;
    std::vector<JackalControl*> jackal_controls;
    vpr::Thread*                 listen_thread;
    int                       Port;
    Socket*                 listen_socket;
    bool                      configured_to_accept;
    vpr::Mutex                   connections_mutex;
    vpr::Mutex                   jackal_controls_mutex;

    // PRIVATE utility functions

    void controlLoop (void* nullParam);

    void removeConnect (Connect* con);

    //: returns a pointer to a connection with the given name
    Connect* getConnect (const std::string& _name);


    //: allows the Environment Manager to accept connections.
    //! RETURNS: reflects succesfully grabbing a port and listening.
    bool acceptConnections();


  
    //: stop listening for client connections
    //! PRE:  True
    //! POST: no new connections are accepted. open connections
    //+       are not changed.
    bool rejectConnections();


    //: Kills all open connections
    void killConnections();

    // These are needed to appease Visual C++ in its creation of DLLs.
    JackalServer(const JackalServer&) {;}
    void operator=(const JackalServer&) {;}

}; // end JackalServer

};

#endif
