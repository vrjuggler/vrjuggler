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
#include <jccl/Config/ConfigChunkPtr.h>
#include <jccl/Plugins/ConfigManager/ConfigChunkHandler.h>
#include <jccl/JackalServer/Socket.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Singleton.h>

namespace jccl {

class Connect;
class JackalControl;


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



    /** Returns if the JackalServer is accepting network connections.
     *  @return True if accepting connections, false otherwise.
     */
    bool isAccepting();



    /** Registers a JackalControl. */
    void addJackalControl (JackalControl* jc);


    /** Removes a JackalControl. */
    void removeJackalControl (JackalControl* jc);



    /** Notification that a connection has died (e.g. by having the other
     *  side drop).
     */
    void connectHasDied (Connect* con);



    /** Acquires the master list of connections.
     *  Note: Calling this function locks the connections mutex.  The
     *  caller MUST use releaseConnections() when finished with the 
     *  vector.
     */
    std::vector<Connect*>& getConnections();

    void releaseConnections();


    //------------------- ConfigChunkHandler Stuff ---------------------

    virtual bool configAdd(ConfigChunkPtr chunk);

    virtual bool configRemove(ConfigChunkPtr chunk);

    virtual bool configCanHandle(ConfigChunkPtr chunk);



private:
    std::vector<Connect*>        connections;
    std::vector<JackalControl*>  jackal_controls;
    vpr::Thread*                 listen_thread;
    int                          Port;
    Socket*                      listen_socket;
    bool                         configured_to_accept;
    vpr::Mutex                   connections_mutex;
    vpr::Mutex                   jackal_controls_mutex;

    // PRIVATE utility functions

    void controlLoop (void* nullParam);

    void removeConnection (Connect* con);

    void addConnection (Connect* con);

    //: returns a pointer to a connection with the given name
    Connect* getConnection (const std::string& _name);


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

    vprSingletonHeader(JackalServer);

}; // end JackalServer

};

#endif
