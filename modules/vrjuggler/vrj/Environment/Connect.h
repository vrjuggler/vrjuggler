
#ifndef _VJ_CONNECT_
#define _VJ_CONNECT_

#include <Threads/vjThread.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjConfigChunkDB.h>
#include <Environment/vjTimedUpdate.h>
#include <Environment/vjCommand.h>
#include <queue>


//--------------------------------------------------
//: vjConnect reads/writes to a file, pipe, or socket.
//---------------------------------------
class vjConnect {
 public:

    vjConnect (int fd, const std::string& _name="unnamed");


    //: default constructor
    //! POST: self is initialized (filename = s, controlPID is NULL)
    //! ARGS: s - name of file to open; a non-NULL C string
    vjConnect(const std::string& s, const std::string& _name);



    vjConnect (vjConfigChunk* c);


    //: destructor
    //! PRE:  True
    //! POST: all dynamically-allocated memory associated
    //+       with self is freed.
    //+       If ControlPID is non-NULL, the process it refers
    //+       to is stopped.
    ~vjConnect();



    std::string getName () {
	return name;
    }



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
    void sendRefresh ();


    //! ARGS: _tu - a vjTimedUpdate* 
    //! ARGS: _refresh_time - time between refreshes, in milliseconds
    void addTimedUpdate (vjTimedUpdate* _tu, float _refresh_time) {
	cout << "vjconnect " << name << " adding timedupdate request" << endl;
	commands_mutex.acquire();
	timed_commands.push (new vjCommandTimedUpdate (_tu, _refresh_time));
	commands_mutex.release();
    }

    void removeTimedUpdate (vjTimedUpdate* _tu) {
	// this better not be called often - it's gotta be nlogn or something.
	// still, there'll probably never be more than a couple dozen
	// items in the timed_commands queue anyway.
	std::priority_queue<vjCommand*> newq;
	vjCommandTimedUpdate* ctu2;
	vjCommand* ctu1;
	commands_mutex.acquire();
	while (!timed_commands.empty()) {
	    ctu1 = timed_commands.top();
	    ctu2 = dynamic_cast<vjCommandTimedUpdate*>(ctu1);
	    timed_commands.pop();
	    if (ctu2 && (ctu2->timed_update == _tu))
		    continue;
	    newq.push (ctu1);
	}
	timed_commands = newq;
	commands_mutex.release();
    }

private:

    ofstream                 output;
    std::string                  name;
    std::string                  filename;
    //vjConfigChunkDB*        cachedChunkdb;
    //vjChunkDescDB*          cachedDescdb;
    vjThread*               connect_thread;
    int                     fd;
    bool                    readable;
    bool                    writeable;
    bool                    shutdown;        // set to stop procs

    std::priority_queue<vjCommand*>          timed_commands; // used as heap
    std::queue<vjCommand*>                   commands;
    vjMutex                 commands_mutex;

    //: used to see if it's time to spring a timed_command
    vjTimeStamp             current_time;

    //: body of network process.
    void controlLoop (void* nullParam);

    //: utility for controlLoop()
    void readCommand (ifstream& fin);

}; // end vjConnect


#endif
