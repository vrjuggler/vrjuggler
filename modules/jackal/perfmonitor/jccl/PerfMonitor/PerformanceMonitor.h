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


#ifndef _JCCL_PERFORMANCE_MONITOR_H_
#define _JCCL_PERFORMANCE_MONITOR_H_

#include <jccl/jcclConfig.h>
#include <jccl/JackalServer/JackalControl.h>
#include <jccl/Plugins/ConfigManager/ConfigChunkHandler.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Sync/Mutex.h>

namespace jccl {

    class Connect;
    class PerfDataBuffer;
    class JackalServer;
    class PeriodicCommand;

//-------------------------------------
//: Jackal performance-monitoring agent.
//
//
// @author  Christopher Just
//
// Date 9-12-01
//---------------------------------------

class JCCL_CLASS_API PerformanceMonitor: public JackalControl, public ConfigChunkHandler {

public:

    //: constructor
    //! PRE: None
    //! POST: Object is constructed
    PerformanceMonitor();



    virtual ~PerformanceMonitor();



    //: creates a buffer containing perf data... 
    PerfDataBuffer*  getPerfDataBuffer (const std::string& _name, 
                                        int _numbufs, 
                                        int _nindex);


    //: destroys a buffer of perf data
    void releasePerfDataBuffer (PerfDataBuffer *v);



    //--------------- JackalControl Stuff -----------------------

    virtual void addConnect (Connect *c);
    virtual void removeConnect (Connect* c);


    //---------------- ConfigChunkHandler Stuff -----------------

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
    struct buffer_element {
        PerfDataBuffer* buffer;
        PeriodicCommand* command;
    };

    std::string               perf_target_name;
    Connect*                  perf_target;
    std::vector<buffer_element> perf_buffers;
    float                     perf_refresh_time;  // in milliseconds
    ConfigChunk*            current_perf_config;
    vpr::Mutex                   perf_buffers_mutex;

    vpr::Mutex                connections_mutex;
    std::vector<Connect*>     connections;

    // PRIVATE utility functions

//      void controlLoop (void* nullParam);

    void activatePerfBuffers();
    void deactivatePerfBuffers();

    void setPerformanceTarget (Connect* con);

    /* connections needs to be locked */
    Connect* getConnect (const std::string&);





    // These are needed to appease Visual C++ in its creation of DLLs.
    PerformanceMonitor(const PerformanceMonitor&) {;}
    void operator=(const PerformanceMonitor&) {;}

}; // end PerformanceMonitor

};

#endif
