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


#ifndef _VJ_ENVIRONMENT_MANAGER_H_
#define _VJ_ENVIRONMENT_MANAGER_H_

#include <vjConfig.h>
#include <Kernel/vjConfigChunkHandler.h>
#include <VPR/Threads/vjThread.h>
#include <Environment/vjSocket.h>
#include <VPR/Sync/vjMutex.h>

class vjConnect;
class vjPerfDataBuffer;
class vjConfigChunkDB;


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
//         - handle vjPerfDataBuffers
//
// @author  Christopher Just
//
// Date 2-27-98
//---------------------------------------

class VJ_CLASS_API vjEnvironmentManager: public vjConfigChunkHandler {

public:

    //: constructor
    //! PRE: None
    //! POST: Object is constructed
    vjEnvironmentManager();



    virtual ~vjEnvironmentManager();



    //: is the EM accepting connections across the network?
    //! RETURNS: true - EM is accepting connections
    //! RETURNS: false - EM is not accepting connections
    bool isAccepting();



    //: registers a buffer containing perf data... 
    void addPerfDataBuffer (vjPerfDataBuffer *v);


    //: unregisters a buffer of perf data
    void removePerfDataBuffer (vjPerfDataBuffer *v);



    //: tells EM that a connection has died (ie by gui disconnecting)
    void connectHasDied (vjConnect* con);


    //: sends a 'refresh' message to all open connections
    void sendRefresh();



    //: ConfigChunkHandler stuff
    //! PRE: configCanHandle(chunk) == true
    //! RETURNS: success
    virtual bool configAdd(vjConfigChunk* chunk);



    //: Remove the chunk from the current configuration
    //! PRE: configCanHandle(chunk) == true
    //!RETURNS: success
    virtual bool configRemove(vjConfigChunk* chunk);


    
    //: Can the handler handle the given chunk?
    //! RETURNS: true - Can handle it
    //+          false - Can't handle it
    virtual bool configCanHandle(vjConfigChunk* chunk);



private:
    std::string               perf_target_name;
    std::vector<vjConnect*>   connections;
    std::vector<vjPerfDataBuffer*> perf_buffers;
    vjThread*                 listen_thread;
    int                       Port;
    vjSocket*                 listen_socket;
    vjConnect*                perf_target;
    float                     perf_refresh_time;  // in milliseconds
    bool                      configured_to_accept;
    vjConfigChunk*            current_perf_config;
    vjMutex                   connections_mutex;
    vjMutex                   perf_buffers_mutex;

    // PRIVATE utility functions

    void controlLoop (void* nullParam);

    void activatePerfBuffers();
    void deactivatePerfBuffers();

    void setPerformanceTarget (vjConnect* con);

    void removeConnect (vjConnect* con);

    //: returns a pointer to a connection with the given name
    vjConnect* getConnect (const std::string& _name);


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
    vjEnvironmentManager(const vjEnvironmentManager&) {;}
    void operator=(const vjEnvironmentManager&) {;}

}; // end vjEnvironmentManager



#endif
