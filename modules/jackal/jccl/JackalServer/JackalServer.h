
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
//#include <Environment/vjTuPerfBufReader.h>
#include <Kernel/vjConfigChunkHandler.h>
#include <Performance/vjPerfDataBuffer.h>

//-------------------------------------
//: vjEnvironmentManager controls the network socket to the GUI.
//
//---------------------------------------


class vjEnvironmentManager: public vjConfigChunkHandler {

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

    //: unregisters a buffer of perf data
    void removePerfDataBuffer (vjPerfDataBuffer *v);


private:
    //: allows the Environment Manager to accept connections.
    //! RETURNS: reflects succesfully grabbing a port and listening.
    bool acceptConnections();


  
    //: stop listening for client connections
    //! PRE:  True
    //! POST: no new connections are accepted. open connections
    //+       are not changed.
    bool rejectConnections();
public:


    //: Kills all open connections
    void killConnections();



    //: returns a pointer to a connection with the given name
    vjConnect* getConnect (const std::string& _name);



    //: sends a message to all open connections that they need to 
    //+ refresh their data.
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
    vjConfigChunkDB*          chunkdb;
    std::vector<vjConnect*>   connections;
    std::vector<vjTimedUpdate*> updaters;
    std::vector<vjPerfDataBuffer*> perf_buffers;
    vjThread*                 listen_thread;
    int                       Port;
    int                       listen_socket;
    vjConnect*                perf_target;
    float                     perf_refresh_time;  // in milliseconds
    bool                      activated;
    bool                      configured_to_accept;
    void controlLoop (void* nullParam);

    // utility for reconfiguration
    void activatePerfBuffersWithPrefix (std::string prefix);
    void deactivatePerfBuffersWithPrefix (std::string prefix);

    // should this buffer be active under our current config?
    // used when a buffer is added after configure
    bool configuredToActivate (vjPerfDataBuffer* b);


    void setPerformanceTarget (vjConnect* con);



}; // end vjEnvironmentManager



#endif
