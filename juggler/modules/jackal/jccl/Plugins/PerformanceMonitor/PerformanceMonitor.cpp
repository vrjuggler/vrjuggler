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


// implementation of Performance Monitor
//
// author: Christopher Just


#include <jccl/Plugins/PerformanceMonitor/PerformanceMonitor.h>
#include <jccl/JackalServer/JackalServer.h>
#include <jccl/JackalServer/Connect.h>
#include <jccl/Plugins/PerformanceMonitor/PerfCommand.h>
#include <jccl/Plugins/PerformanceMonitor/PerfDataBuffer.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/Config/ParseUtil.h>


namespace jccl {

PerformanceMonitor::PerformanceMonitor():
                          perf_buffers(),
                          perf_buffers_mutex(),
                          connections(),
                          connections_mutex() {

    perf_refresh_time = 500;

    perf_target_name = "";
    perf_target = NULL;
    current_perf_config = NULL;
}



PerformanceMonitor::~PerformanceMonitor() {

}



/*virtual*/ void PerformanceMonitor::addConnect (Connect* con) {
    vprASSERT (con != NULL);

    connections_mutex.acquire();
    connections.push_back (con);
    if (con->getName() == perf_target_name)
        setPerformanceTarget (con);
    connections_mutex.release();
}


/*virtual*/ void PerformanceMonitor::removeConnect (Connect* con) {
    vprASSERT (con != NULL);

    connections_mutex.acquire();

    std::vector<Connect*>::iterator i;
    for (i = connections.begin(); i != connections.end(); i++) {
        if (con == *i) {
            connections.erase (i);
            delete con;
            break;
        }
    }

    if (con == perf_target)
        setPerformanceTarget (NULL);
    connections_mutex.release();
}


PerfDataBuffer*  PerformanceMonitor::getPerfDataBuffer (const std::string& _name, 
                                            int _numbufs, 
                                            int _nindex ) {
    buffer_element b;
    b.buffer =  new PerfDataBuffer (_name, _numbufs, _nindex);
    b.command = new CommandWritePerfData (b.buffer, perf_refresh_time);
    vprDEBUG (jcclDBG_PERFORMANCE, 4) << "Adding perf data buffer " 
                                     << _name.c_str() << "\n"
                                     << vprDEBUG_FLUSH;
    perf_buffers_mutex.acquire();
    perf_buffers.push_back(b);
    activatePerfBuffers();
    perf_buffers_mutex.release();

    return b.buffer;
}




void PerformanceMonitor::releasePerfDataBuffer (PerfDataBuffer *b) {
    std::vector<buffer_element>::iterator it;

    vprDEBUG (jcclDBG_PERFORMANCE, 4) << "Releasing perf data buffer " 
                                     << b->getName().c_str()
                                     << "\n" << vprDEBUG_FLUSH;

    perf_buffers_mutex.acquire();
    // this is one of those things I really hate:
    for (it = perf_buffers.begin(); it != perf_buffers.end(); it++) {
        if (it->buffer == b) {
            it->buffer->deactivate();
            if (perf_target)
                perf_target->removePeriodicCommand (it->command);
            perf_buffers.erase(it);
            break;
        }
    }
    perf_buffers_mutex.release();

    delete b;
}



//: ConfigChunkHandler stuff
//! PRE: configCanHandle(chunk) == true
//! RETURNS: success
bool PerformanceMonitor::configAdd(ConfigChunk* chunk) {

    std::string s = chunk->getType();
    if (!vjstrcasecmp (s, "PerfMeasure")) {
        current_perf_config = new ConfigChunk (*chunk);

        perf_target_name = (std::string)chunk->getProperty ("PerformanceTarget");
        connections_mutex.acquire();

        Connect* new_perf_target = getConnect(perf_target_name);
        if (new_perf_target != perf_target)
            setPerformanceTarget (new_perf_target);
        else {
            // setPerformanceTarget above will activatePerfBuffers,
            // but we want to make suer that happens regardless to get
            // any changes to the individual buffer states.
            perf_buffers_mutex.acquire();
            activatePerfBuffers();
            perf_buffers_mutex.release();
        }
        connections_mutex.release();

        return true;
    }
    return false;
}



//: Remove the chunk from the current configuration
//! PRE: configCanHandle(chunk) == true
//!RETURNS: success
bool PerformanceMonitor::configRemove(ConfigChunk* chunk) {

    std::string s = chunk->getType();
    if (!vjstrcasecmp (s, "PerfMeasure")) {
        if (current_perf_config) {
            if (!vjstrcasecmp (current_perf_config->getProperty ("Name"),
                               chunk->getProperty ("Name"))) {
                delete (current_perf_config);
                current_perf_config = NULL;
                deactivatePerfBuffers ();
            }
        }
        return true;
    }
    return false;
}



//: Can the handler handle the given chunk?
//! RETURNS: true - Can handle it
//+          false - Can't handle it
bool PerformanceMonitor::configCanHandle(ConfigChunk* chunk) {
    std::string s = chunk->getType();
    return (!vjstrcasecmp (s, "PerfMeasure"));
}



//-------------------- PRIVATE MEMBER FUNCTIONS -------------------------




void PerformanceMonitor::setPerformanceTarget (Connect* con) {
    //std::cout << "setting performance target" << std::endl;
    if (con == perf_target)
        return;
    perf_buffers_mutex.acquire();
    deactivatePerfBuffers();
    perf_target = con;
    activatePerfBuffers();
    perf_buffers_mutex.release();
}


    /* connections needs to be locked */
    Connect* PerformanceMonitor::getConnect (const std::string& s) {
        vprASSERT (connections_mutex.test() == 1 && "Must be locked");

        for (unsigned int i = 0; i < connections.size(); i++)
            if (s == connections[i]->getName())
                return connections[i];
        return NULL;
    }


    void PerformanceMonitor::deactivatePerfBuffers () {
        std::vector<buffer_element>::iterator i;
        for (i = perf_buffers.begin(); i != perf_buffers.end(); i++) {
            i->buffer->deactivate();
            if (perf_target)
                perf_target->removePeriodicCommand (i->command);
        }
    }



    void PerformanceMonitor::activatePerfBuffers () {
        // activates all perf buffers configured to do so
        // this is still a bit on the big and bulky side.

        std::cout << "activating perfbuffers" << std::endl;
        
        if (perf_buffers.empty())
            return;
        
        if (perf_target == NULL || current_perf_config == NULL) {
            deactivatePerfBuffers();
            return;
        }
        
        std::cout << "activating perfbuffers 2" << std::endl;
        
        std::vector<VarValue*> v = current_perf_config->getAllProperties ("TimingTests");
        std::vector<buffer_element>::const_iterator b;
        std::vector<VarValue*>::const_iterator val;
        bool found;
        ConfigChunk* ch;

        for (b = perf_buffers.begin(); b != perf_buffers.end(); b++) {
            found = false;
            for (val = v.begin(); val != v.end(); val++) {
                ch = *(*val); // this line demonstrates a subtle danger
                if ((bool)ch->getProperty ("Enabled")) {
                    if (!vjstrncasecmp(ch->getProperty("Prefix"), b->buffer->getName()))
                        found = true;
                }
            }
            if (found) {
                std::cout << "adding periodic command" << std::endl;
                b->buffer->activate();
                perf_target->addPeriodicCommand (b->command);
            }
            else if (b->buffer->isActive()) {
                b->buffer->deactivate();
                perf_target->removePeriodicCommand (b->command);
            }
        }
        for (val = v.begin(); val != v.end(); val++) {
            delete (*val);
        }
    }


};

