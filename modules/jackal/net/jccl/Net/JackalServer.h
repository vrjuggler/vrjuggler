/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_ENVIRONMENT_MANAGER_H_
#define _VJ_ENVIRONMENT_MANAGER_H_

#include <vjConfig.h>
#include <Kernel/vjConfigChunkHandler.h>
#include <Threads/vjThread.h>

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

class vjEnvironmentManager: public vjConfigChunkHandler {

public:

    //: constructor
    //! PRE: None
    //! POST: Object is constructed
    vjEnvironmentManager();



    ~vjEnvironmentManager();



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
    int                       listen_socket;
    vjConnect*                perf_target;
    float                     perf_refresh_time;  // in milliseconds
    bool                      configured_to_accept;
    vjConfigChunk*            current_perf_config;
    vjMutex                   connections_mutex;

    // PRIVATE utility functions

    void controlLoop (void* nullParam);

    //! NOTE: calling thread must own connections_mutex
    void activatePerfBuffers();

    //! NOTE: calling thread must own connections_mutex
    void deactivatePerfBuffers();

    //! NOTE: calling thread must own connections_mutex
    void setPerformanceTarget (vjConnect* con);

    //: closes and removes a connection
    //! RETURNS: true - if a matching connection was found and removed
    //! RETURNS: false - con was NULL or no match was found
    //! NOTE: calling thread must own connections_mutex
    bool removeConnect (vjConnect* con);

    //: returns a pointer to a connection with the given name
    //! NOTE: calling thread must own connections_mutex
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



}; // end vjEnvironmentManager



#endif
