
// Begun: 2/27/98

// The Environment Manager handles network connections to 
// UI components.
// Which means that its main duties are:
//   - create a socket
//   - spin off a process to listen() for connections
//   - spin off a process to handle an active connection
//
// the above assumes we want to allow multiple connections 
// at once.  I'm not sure if that's neccessary, but its 
// probably best to design with that idea in mind.  We 
// might end up with separate monitors & stuff.

#ifndef _VJ_ENVIRONMENT_MANAGER_H_
#define _VJ_ENVIRONMENT_MANAGER_H_

#include <vjConfig.h>
#include <Environment/vjConnect.h>
#include <Environment/vjTuPerfBufReader.h>



//-------------------------------------
//: vjEnvironmentManager controls the network socket to the GUI.
//
//---------------------------------------


class vjEnvironmentManager /* way too long a name */ {

public:

    //: constructor
    //! PRE: None
    //! POST: Object is constructed
    vjEnvironmentManager();



    ~vjEnvironmentManager();



    void activate();
    void deactivate();

    //: is the EM accepting connections across the network?
    //! RETURNS: true - EM is accepting connections
    //! RETURNS: false - EM is not accepting connections
    bool isAccepting();



    //: registers a buffer containing perf data... 
    void addPerfDataBuffer (vjPerfDataBuffer *v);



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



    //: returns a pointer to a connection with the given name
    vjConnect* getConnect (char* _name);



private:
    vjConfigChunkDB*          chunkdb;
    vector<vjConnect*> connections;
    vector<vjTimedUpdate*>    updaters;
    vjThread*                 listen_thread;
    int                       Port;
    int                       listen_socket;
    vjTuPerfBufReader*     perf_buffer_reader;
    bool                      activated;
    bool                      configured_to_accept;
    void controlLoop (void* nullParam);
    void reconfigure();

    
}; // end vjEnvironmentManager



#endif
