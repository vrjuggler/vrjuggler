
#ifndef _VJ_CONNECT_
#define _VJ_CONNECT_

#include <Threads/vjThread.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjConfigChunkDB.h>
#include <Environment/vjSyncWriter.h>


//--------------------------------------------------
//: vjConnect reads/writes to a file, pipe, or socket.
//---------------------------------------
class vjConnect {
 public:

    vjConnect (int fd, char* _name="unnamed");


    //: default constructor
    //! POST: self is initialized (filename = s, controlPID is NULL)
    //! ARGS: s - name of file to open; a non-NULL C string
    vjConnect(char* s, char* _name);



    vjConnect (vjConfigChunk* c);


    //: destructor
    //! PRE:  True
    //! POST: all dynamically-allocated memory associated 
    //+       with self is freed.
    //+       If ControlPID is non-NULL, the process it refers 
    //+       to is stopped.
    ~vjConnect();



    //: Starts the file connection process.
    //! PRE:  None
    //! POST: A thread has been created to read from the file/pipe 
    //+       connection (or an error has occurred while 
    //+       attempting to do so).
    //+       controlPID is set to the PID of the generated process.
    //! RETURNS: True - successfully created the thread.
    //! RETURNS: False - unable to create thread.
    bool startProcess();
  


    //: Stops file connection process.
    //! PRE:  None
    //! POST: If self had a thread associated with it, that 
    //+       thread is terminated.
    //! RETURNS: always True
    bool stopProcess();


    void sendDescDB (vjChunkDescDB* db);
    void sendChunkDB (vjConfigChunkDB* db, bool all);

    vjSyncWriter            output;
    char*                   name;
    char*                   filename;
    vjConfigChunkDB*        chunkdb;
    vjChunkDescDB*          descdb;
    vjThread*               connect_thread;
    int                     fd;
    bool                    readable;

    static char* default_name;

    //: body of network process.
    void controlLoop(void* nullParam);

    
}; // end vjConnect


#endif
