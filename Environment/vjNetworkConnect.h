
#ifndef _VJ_NETWORK_CONNECT_
#define _VJ_NETWORK_CONNECT_

//#include <Kernel/vjKernel.h>
#include <Threads/vjThread.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjConfigChunkDB.h>
#include <Environment/vjTimedUpdate.h>

//-------------------------------------
//: vjNetworkConnect handles a single network connection.
//
// vjNetworkConnect is a wholly-owned subsidiary of the 
// Environment Manager.  It is instantiated in response 
// to an acceptConnection() by the vjNetworkControl process, 
// and lasts until it is terminated by the Environment 
// Manager or when the client it talks to closes the connnection.
//---------------------------------------
class vjNetworkConnect {
 public:

    //: default constructor
    //! PRE:  s identifies a network socket (as returned by 
    //+       acceptConnection)
    //! POST: self is initialized (socket = s, controlPID is NULL)
    vjNetworkConnect(int s);



    //: destructor
    //! PRE:  True
    //! POST: all dynamically-allocated memory associated 
    //+       with self is freed.
    //+       All TimedUpdates associated with self are 
    //+       stopped and destroyed.
    //+       If ControlPID is non-NULL, the process it refers 
    //+       to is stopped.
    ~vjNetworkConnect();



    //: Starts the network connection process.
    //! PRE:  None
    //! POST: A thread has been created to handle the network 
    //+       connection (or an error has occurred while 
    //+       attempting to do so).
    //+       controlPID is set to the PID of the generated process.
    //! RETURNS: True - successfully created the thread.
    //! RETURNS: False - unable to create thread.
    bool startProcess();
  


    //: Stops the network connection process.
    //! PRE:  None
    //! POST: If self had a thread associated with it, that 
    //+       thread is terminated.
    //! RETURNS: always True
    bool stopProcess();



    //: transmits string data to the client.
    //! PRE:  s points to a 0-terminated string.
    //! POST: If a network socket is open, s is passed to that socket.
    void transmitData(char* s);



    //: transmits ConfigChunk data to the client.
    //! PRE:  c points to a valid ConfigChunk
    //! POST: If a network socket is open, c is passed to that socket.
    void transmitData(vjConfigChunk* c);



    //: stops all TimedUpdates associated with self.
    //! PRE:  True
    //! POST: Any TimedUpdates associated with self are stopped and
    //+       destroyed.
    void stopAllUpdates();



    //: stops a single named TimedUpdate associated with self.
    //! PRE:  s points to a 0-terminated string.
    //! POST: Any TimedUpdates associated with self, and with a
    //+       name matching s, are stopped and destroyed.
    void stopUpdate(char* s);



private:

    int                  sock;
    vjConfigChunkDB*       chunkdb;
    vjChunkDescDB*         descdb;
    vjThread*          connect_thread;
    ostream*            fout;
    vector<vjTimedUpdate*> updates;

    //: body of network process.
    void controlLoop(void* nullParam);
    
}; // end vjNetworkConnect()


#endif
