
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

#include "vjNetworkConnect.h"




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



    //: allows the Environment Manager to accept connections.
    //! RETURNS: reflects succesfully grabbing a port and listening.
    bool acceptConnections();


  
    //: stop listening for client connections & close connections
    //! PRE:  True
    //! POST: All connections are closed, no more are accepted.
    bool rejectConnections();



private:
    
    vjConfigChunkDB*          chunkdb;
    vector<vjNetworkConnect*> connections;
    vjThread*                 listen_thread;
    int                       Port;
    int                       listen_socket;
    
    void controlLoop (void* nullParam);
    
}; // end vjEnvironmentManager



#endif
