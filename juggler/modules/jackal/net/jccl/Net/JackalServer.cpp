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


#include <jccl/JackalServer/JackalServer.h>
#include <jccl/JackalServer/JackalControl.h>
#include <jccl/JackalServer/Connect.h>
#include <jccl/Config/ChunkDescDB.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/Config/ParseUtil.h>

#include <jccl/Plugins/ConfigManager/ConfigManager.h>

namespace jccl {

JackalServer::JackalServer():
                          connections(),
                          jackal_controls(),
                          connections_mutex(),
                          jackal_controls_mutex() {

    /* I want some hardcoded defaults, yes? */
    Port = 4450;
    listen_thread = NULL;
    listen_socket = NULL;
    configured_to_accept = false;
}



JackalServer::~JackalServer() {
    rejectConnections();
    connections_mutex.acquire();
    killConnections();
    connections_mutex.release();
}



bool JackalServer::isAccepting() {
    return (listen_thread != NULL);
}



void JackalServer::addJackalControl (JackalControl* jc) {
    vprASSERT (jc != NULL);

    jackal_controls_mutex.acquire();
    jackal_controls.push_back (jc);
    // eeks... scary deadlock warnings are going off in my head...
    connections_mutex.acquire();
    for (unsigned int i = 0, n = connections.size(); i < n; i++)
        jc->addConnect (connections[i]);
    connections_mutex.release();
    jackal_controls_mutex.release();

}


void JackalServer::removeJackalControl (JackalControl* jc) {
    vprASSERT (jc != NULL);

    std::vector<JackalControl*>::iterator it;
    jackal_controls_mutex.acquire();
    for (it = jackal_controls.begin(); it != jackal_controls.end(); it++) {
        if (*it == jc) {
            jackal_controls.erase(it);
            break;
        }
    }
    jackal_controls_mutex.release();
}



//: tells EM that a connection has died (ie by gui disconnecting)
//  not for the case of removal by configRemove
void JackalServer::connectHasDied (Connect* con) {
    std::string s = con->getName();

    connections_mutex.acquire();
    removeConnect(con);
    connections_mutex.release();
    ConfigManager::instance()->lockActive();
    ConfigManager::instance()->getActiveConfig()->removeNamed(s);
    ConfigManager::instance()->unlockActive();
    sendRefresh();
}



void JackalServer::sendRefresh() {
//     connections_mutex.acquire();
//     for (unsigned int i = 0; i < connections.size(); i++) {
//         connections[i]->sendRefresh();
//     }
//     connections_mutex.release();
}



//: ConfigChunkHandler stuff
//! PRE: configCanHandle(chunk) == true
//! RETURNS: success
bool JackalServer::configAdd(ConfigChunk* chunk) {
    bool networkingchanged = false;
    int newport;

    std::string s = chunk->getType();
    if (!vjstrcasecmp (s, "EnvironmentManager")) {
        configured_to_accept = chunk->getProperty ("AcceptConnections");
        newport = chunk->getProperty("Port");

        if (newport == 0)
            newport = Port;
        if ((newport != Port) || (configured_to_accept != isAccepting()))
            networkingchanged = true;
        perf_target_name = (std::string)chunk->getProperty ("PerformanceTarget");
        connections_mutex.acquire();

//          Connect* new_perf_target = getConnect(perf_target_name);
//          if (new_perf_target != perf_target)
//              setPerformanceTarget (NULL);

        if (networkingchanged) {
            Port = newport;
            if (isAccepting())
                rejectConnections();
            if (configured_to_accept)
                acceptConnections();
            else
                killConnections();
        }
//          if (new_perf_target)
//              setPerformanceTarget(new_perf_target);
        connections_mutex.release();

        return true;
    }
    else if (!vjstrcasecmp (s, "FileConnect")) {
        // I wanted to just look if the fileconnect had been added yet.
        // however I seem to have a chicken/egg problem.
        // so the kludge we'll do now is to not directly add a chunk that's
        // of type VJC_INTERACTIVE. sigh.
        // Unfortunately, this means that for other cases (such as attaching
        // to a named pipe) we're still broken
        if ((int)chunk->getProperty("Mode") != VJC_INTERACTIVE) {
            // it's new to us
            Connect* vn = new Connect (chunk);
            vprDEBUG (jcclDBG_SERVER, 1) << "EM adding connection: " << vn->getName().c_str() << '\n'
                                       << vprDEBUG_FLUSH;
            connections_mutex.acquire();
            connections.push_back (vn);
            vn->startProcess();
//              if (!vjstrcasecmp (vn->getName(), perf_target_name))
//                  setPerformanceTarget (vn);
            connections_mutex.release();
        }
        return true;
    }
    return false;
}



//: Remove the chunk from the current configuration
//! PRE: configCanHandle(chunk) == true
//!RETURNS: success
bool JackalServer::configRemove(ConfigChunk* chunk) {

    std::string s = chunk->getType();
    if (!vjstrcasecmp (s, "EnvironmentManager")) {
        // this could be trouble if the chunk being removed isn't the chunk
        // we were configured with...
        rejectConnections();
        Port = 4450;
        configured_to_accept = false;
        return true;
    }
    else if (!vjstrcasecmp (s, "FileConnect")) {
        vprDEBUG (jcclDBG_SERVER,1) << "EM Removing connection: "
                                  << chunk->getProperty ("Name") << '\n' << vprDEBUG_FLUSH;
        connections_mutex.acquire();
        Connect* c = getConnect (chunk->getProperty ("Name"));
        if (c) {
            removeConnect (c);
        }
        connections_mutex.release();
        vprDEBUG (jcclDBG_SERVER,4) << "EM completed connection removal\n" << vprDEBUG_FLUSH;
        return true;
    }

    return false;
}



//: Can the handler handle the given chunk?
//! RETURNS: true - Can handle it
//+          false - Can't handle it
bool JackalServer::configCanHandle(ConfigChunk* chunk) {
    std::string s = chunk->getType();
    return (!vjstrcasecmp (s, "EnvironmentManager") ||
            !vjstrcasecmp (s, "FileConnect"));
}



//-------------------- PRIVATE MEMBER FUNCTIONS -------------------------

// should only be called when we own connections_mutex
void JackalServer::removeConnect (Connect* con) {
    vprASSERT (con != 0);

    std::vector<Connect*>::iterator i;
    for (i = connections.begin(); i != connections.end(); i++) {
        if (con == *i) {
            connections.erase (i);
            delete con;
            break;
        }
    }
}



// should only be called when we own connections_mutex
Connect* JackalServer::getConnect (const std::string& s) {
    for (unsigned int i = 0; i < connections.size(); i++)
        if (s == connections[i]->getName())
            return connections[i];
    return NULL;
}



void JackalServer::controlLoop (void* nullParam) {
    // Child process used to listen for new network connections
    //struct sockaddr_in servaddr;
    Socket* servsock;
    //int len;
    Connect* connection;

    vprDEBUG(jcclDBG_SERVER,4) << "JackalServer started control loop.\n"
          << vprDEBUG_FLUSH;

    for (;;) {
        servsock = listen_socket->accept();
        char name[128];
        //sprintf (name, "Network Connect %d", servsock->getID());
        vprDEBUG(jcclDBG_SERVER,vprDBG_CONFIG_LVL) << "JackalServer: Accepted connection: id: "
                                                << servsock->getID() << " on port: N/A\n" << vprDEBUG_FLUSH;
        connection = new Connect (servsock, (std::string)name);
        connections_mutex.acquire();
        connections.push_back( connection );
        connection->startProcess();
        connections_mutex.release();
    }
}



bool JackalServer::acceptConnections() {

    if (listen_thread != NULL)
        return true;

    listen_socket = new Socket ();
    if (!listen_socket->listen (Port)) {
        vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) <<  clrOutNORM(clrRED,"ERROR:") << "Jackal Server couldn't open socket\n"
                                                << vprDEBUG_FLUSH;
        return false;
    }
    else
        vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrCYAN, "Jackal Server")
            << " listening on port " << clrOutNORM(clrMAGENTA, Port) << "\n"
            << vprDEBUG_FLUSH;

    /* now we ought to spin off a thread to do the listening */
    vpr::ThreadMemberFunctor<JackalServer>* memberFunctor =
        new vpr::ThreadMemberFunctor<JackalServer>(this,
                                                        &JackalServer::controlLoop,
                                                        NULL);
    listen_thread = new vpr::Thread(memberFunctor);

    return (listen_thread != NULL);
}




bool JackalServer::rejectConnections () {
    if (listen_thread) {
        listen_thread->kill();
        listen_thread = NULL;
        delete listen_socket;
        listen_socket = 0;
    }
    return 1;
}



// should only be called while we own connections_mutex
void JackalServer::killConnections() {
    unsigned int i;

    //connections_mutex.acquire();
    for (i = 0; i < connections.size(); i++) {
        connections[i]->stopProcess();
        delete (connections[i]);
    }
    connections.erase (connections.begin(), connections.end());
    //connections_mutex.release();
}

};

