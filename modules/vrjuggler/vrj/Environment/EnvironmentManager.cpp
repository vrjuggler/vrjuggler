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

#include <vrj/vjConfig.h>

#include <vrj/Kernel/Kernel.h>
#include <vrj/Environment/Connect.h>
#include <vrj/Performance/PerfDataBuffer.h>
#include <vrj/Config/ChunkDescDB.h>
#include <vrj/Config/ConfigChunkDB.h>
#include <vrj/Config/ParseUtil.h>
#include <vrj/Environment/TimedUpdate.h>

#include <vrj/Kernel/ConfigManager.h>
#include <vrj/Environment/EnvironmentManager.h>

namespace vrj
{
   
EnvironmentManager::EnvironmentManager():
                          connections(),
                          perf_buffers(),
                          connections_mutex(),
                          perf_buffers_mutex() {

    /* I want some hardcoded defaults, yes? */
    Port = 4450;
    listen_thread = NULL;
    listen_socket = NULL;
    configured_to_accept = false;
    perf_refresh_time = 500;

    perf_target_name = "";
    perf_target = NULL;
    current_perf_config = NULL;
}



EnvironmentManager::~EnvironmentManager() {
    rejectConnections();
    connections_mutex.acquire();
    killConnections();
    connections_mutex.release();
}



bool EnvironmentManager::isAccepting() {
    return (listen_thread != NULL);
}



void EnvironmentManager::addPerfDataBuffer (PerfDataBuffer *b) {
    vjDEBUG (vjDBG_PERFORMANCE, 4) << "EM adding perf data buffer " << b->getName().c_str() << "\n"
                                   << vjDEBUG_FLUSH;
    perf_buffers_mutex.acquire();
    perf_buffers.push_back(b);
    activatePerfBuffers();
    perf_buffers_mutex.release();
}




void EnvironmentManager::removePerfDataBuffer (PerfDataBuffer *b) {
    std::vector<PerfDataBuffer*>::iterator it;

    vjDEBUG (vjDBG_PERFORMANCE, 4) << "EM removing perf data buffer " << b->getName().c_str()
                                   << "\n" << vjDEBUG_FLUSH;

    perf_buffers_mutex.acquire();
    b->deactivate();
    if (perf_target)
        perf_target->removeTimedUpdate (b);
    // this is one of those things I really hate:
    for (it = perf_buffers.begin(); it != perf_buffers.end(); it++) {
        if (*it == b) {
            perf_buffers.erase(it);
            break;
        }
    }
    perf_buffers_mutex.release();

}



//: tells EM that a connection has died (ie by gui disconnecting)
//  not for the case of removal by configRemove
void EnvironmentManager::connectHasDied (Connect* con) {
    std::string s = con->getName();

    connections_mutex.acquire();
    removeConnect(con);
    connections_mutex.release();
    ConfigManager::instance()->lockActive();
    ConfigManager::instance()->getActiveConfig()->removeNamed(s);
    ConfigManager::instance()->unlockActive();
    sendRefresh();
}



void EnvironmentManager::sendRefresh() {
   connections_mutex.acquire();
   for (unsigned int i = 0; i < connections.size(); i++) {
       connections[i]->sendRefresh();
   }
   connections_mutex.release();
}



//: ConfigChunkHandler stuff
//! PRE: configCanHandle(chunk) == true
//! RETURNS: success
bool EnvironmentManager::configAdd(ConfigChunk* chunk) {
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

        Connect* new_perf_target = getConnect(perf_target_name);
        if (new_perf_target != perf_target)
            setPerformanceTarget (NULL);

        if (networkingchanged) {
            Port = newport;
            if (isAccepting())
                rejectConnections();
            if (configured_to_accept)
                acceptConnections();
            else
                killConnections();
        }
        if (new_perf_target)
            setPerformanceTarget(new_perf_target);
        connections_mutex.release();

        return true;
    }
    else if (!vjstrcasecmp (s, "PerfMeasure")) {
        current_perf_config = new ConfigChunk (*chunk);
        perf_buffers_mutex.acquire();
        activatePerfBuffers();
        perf_buffers_mutex.release();
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
            vjDEBUG (vjDBG_ENV_MGR, 1) << "EM adding connection: " << vn->getName().c_str() << '\n'
                                       << vjDEBUG_FLUSH;
            connections_mutex.acquire();
            connections.push_back (vn);
            vn->startProcess();
            if (!vjstrcasecmp (vn->getName(), perf_target_name))
                setPerformanceTarget (vn);
            connections_mutex.release();
        }
        return true;
    }
    return false;
}



//: Remove the chunk from the current configuration
//! PRE: configCanHandle(chunk) == true
//!RETURNS: success
bool EnvironmentManager::configRemove(ConfigChunk* chunk) {

    std::string s = chunk->getType();
    if (!vjstrcasecmp (s, "EnvironmentManager")) {
        // this could be trouble if the chunk being removed isn't the chunk
        // we were configured with...
        rejectConnections();
        Port = 4450;
        configured_to_accept = false;
        return true;
    }
    else if (!vjstrcasecmp (s, "PerfMeasure")) {
        if (current_perf_config) {
            if (!vjstrcasecmp (current_perf_config->getProperty ("Name"),
                               chunk->getProperty ("Name"))) {
                delete (current_perf_config);
                current_perf_config = NULL;
                connections_mutex.acquire();
                deactivatePerfBuffers ();
                connections_mutex.release();
            }
        }
        return true;
    }
    else if (!vjstrcasecmp (s, "FileConnect")) {
        vjDEBUG (vjDBG_ENV_MGR,1) << "EM Removing connection: "
                                  << chunk->getProperty ("Name") << '\n' << vjDEBUG_FLUSH;
        connections_mutex.acquire();
        Connect* c = getConnect (chunk->getProperty ("Name"));
        if (c) {
            removeConnect (c);
        }
        connections_mutex.release();
        vjDEBUG (vjDBG_ENV_MGR,4) << "EM completed connection removal\n" << vjDEBUG_FLUSH;
        return true;
    }

    return false;
}



//: Can the handler handle the given chunk?
//! RETURNS: true - Can handle it
//+          false - Can't handle it
bool EnvironmentManager::configCanHandle(ConfigChunk* chunk) {
    std::string s = chunk->getType();
    return (!vjstrcasecmp (s, "EnvironmentManager") ||
            !vjstrcasecmp (s, "PerfMeasure") ||
            !vjstrcasecmp (s, "FileConnect"));
}



//-------------------- PRIVATE MEMBER FUNCTIONS -------------------------

// should only be called when we own connections_mutex
void EnvironmentManager::removeConnect (Connect* con) {
    if (!con)
        return;
    if (con == perf_target)
        setPerformanceTarget (NULL);
    std::vector<Connect*>::iterator i;
    for (i = connections.begin(); i != connections.end(); i++)
        if (con == *i) {
            connections.erase (i);
            delete con;
            break;
        }
}



// should only be called when we own connections_mutex
void EnvironmentManager::setPerformanceTarget (Connect* con) {
    if (con == perf_target)
        return;
    perf_buffers_mutex.acquire();
    deactivatePerfBuffers();
    perf_target = con;
    activatePerfBuffers();
    perf_buffers_mutex.release();
}



// should only be called when we own connections_mutex
Connect* EnvironmentManager::getConnect (const std::string& s) {
    for (unsigned int i = 0; i < connections.size(); i++)
        if (s == connections[i]->getName())
            return connections[i];
    return NULL;
}



void EnvironmentManager::controlLoop (void* nullParam) {
    // Child process used to listen for new network connections
    //struct sockaddr_in servaddr;
    Socket* servsock;
    //int len;
    Connect* connection;

    vjDEBUG(vjDBG_ENV_MGR,4) << "vjEnvironmentManager started control loop.\n"
          << vjDEBUG_FLUSH;

    for (;;) {
        servsock = listen_socket->accept();
        char name[128];
        //sprintf (name, "Network Connect %d", servsock->getID());
        vjDEBUG(vjDBG_ENV_MGR,vjDBG_CONFIG_LVL) << "vjEnvironmentManager: Accepted connection: id: "
                                                << servsock->getID() << " on port: N/A\n" << vjDEBUG_FLUSH;
        connection = new Connect (servsock, (std::string)name);
        connections_mutex.acquire();
        connections.push_back( connection );
        connection->startProcess();
        connections_mutex.release();
    }
}



// should only be called while we have the connections mutex...
void EnvironmentManager::deactivatePerfBuffers () {
    std::vector<PerfDataBuffer*>::iterator i;
    for (i = perf_buffers.begin(); i != perf_buffers.end(); i++) {
        (*i)->deactivate();
        if (perf_target)
            perf_target->removeTimedUpdate (*i);
    }
}



// should only be called while we own connections_mutex
void EnvironmentManager::activatePerfBuffers () {
    // activates all perf buffers configured to do so
    // this is still a bit on the big and bulky side.

    if (perf_buffers.empty())
        return;

    if (perf_target == NULL || current_perf_config == NULL) {
        deactivatePerfBuffers();
        return;
    }

    std::vector<VarValue*> v = current_perf_config->getAllProperties ("TimingTests");
    std::vector<PerfDataBuffer*>::const_iterator b;
    std::vector<VarValue*>::const_iterator val;
    bool found;
    ConfigChunk* ch;

    for (b = perf_buffers.begin(); b != perf_buffers.end(); b++) {
        found = false;
        for (val = v.begin(); val != v.end(); val++) {
            ch = *(*val); // this line demonstrates a subtle danger
            if ((bool)ch->getProperty ("Enabled")) {
                if (!vjstrncasecmp(ch->getProperty("Prefix"), (*b)->getName()))
                    found = true;
            }
        }
        if (found) {
            (*b)->activate();
            perf_target->addTimedUpdate ((*b), perf_refresh_time);
        }
        else if ((*b)->isActive()) {
            (*b)->deactivate();
            perf_target->removeTimedUpdate (*b);
        }
    }
    for (val = v.begin(); val != v.end(); val++) {
        delete (*val);
    }

}



bool EnvironmentManager::acceptConnections() {

    if (listen_thread != NULL)
        return true;

    listen_socket = new Socket ();
    if (!listen_socket->listen (Port)) {
        vjDEBUG(vjDBG_ERROR,vjDBG_CRITICAL_LVL) <<  clrOutNORM(clrRED,"ERROR:") << "Environment Manager couldn't open socket\n"
                                                << vjDEBUG_FLUSH;
        return false;
    }
    else
        vjDEBUG(vjDBG_ALL,vjDBG_CRITICAL_LVL)
            << clrOutNORM(clrCYAN, "Environment Manager")
            << " listening on port " << clrOutNORM(clrMAGENTA, Port) << "\n"
            << vjDEBUG_FLUSH;

    /* now we ought to spin off a thread to do the listening */
    vpr::ThreadMemberFunctor<EnvironmentManager>* memberFunctor =
        new vpr::ThreadMemberFunctor<EnvironmentManager>(this,
                                                        &EnvironmentManager::controlLoop,
                                                        NULL);
    listen_thread = new vpr::Thread(memberFunctor);

    return (listen_thread != NULL);
}




bool EnvironmentManager::rejectConnections () {
    if (listen_thread) {
        listen_thread->kill();
        listen_thread = NULL;
        delete listen_socket;
        listen_socket = 0;
    }
    return 1;
}



// should only be called while we own connections_mutex
void EnvironmentManager::killConnections() {
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
