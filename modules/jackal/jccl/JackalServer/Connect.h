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


#ifndef _VJ_CONNECT_H_
#define _VJ_CONNECT_H_

#include <vjConfig.h>
#include <queue>
#include <Environment/vjSocket.h>
#include <Environment/vjNetCommunicator.h>
#include <VPR/Threads/vjThread.h>
#include <Performance/vjTimeStamp.h>
#include <Environment/vjCommand.h>


class vjTimedUpdate;
class vjConfigChunk;

enum vjConnectMode { VJC_INTERACTIVE, VJC_INPUT, VJC_OUTPUT };

//--------------------------------------------------
//: vjConnect reads/writes to a file, pipe, or socket.
//
// @author  Christopher Just
//
//---------------------------------------
class vjConnect {
 public:

    vjConnect (vjSocket* s, const std::string& _name="unnamed", 
	       vjConnectMode _mode = VJC_INTERACTIVE);



    vjConnect (vjConfigChunk* c);


    //: destructor
    //! PRE:  True
    //! POST: all dynamically-allocated memory associated
    //+       with self is freed.
    //+       If ControlPID is non-NULL, the process it refers
    //+       to is stopped.
    ~vjConnect();



    //: returns the name of this connection
    //! NOTE: The name is the same as the name of the ConfigChunk that
    //+       represents it.
    std::string getName () {
	//cout << "name of this thing is " << name << endl;
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
    void sendDisconnect();

    //: Attaches a timed update object to this connection
    //! ARGS: _tu - a vjTimedUpdate* 
    //! ARGS: _refresh_time - time between refreshes, in milliseconds
    void addTimedUpdate (vjTimedUpdate* _tu, float _refresh_time);


    //: Detaches a timed update object from this connection
    void removeTimedUpdate (vjTimedUpdate* _tu);


private:


    std::ostream*     outstream;
    std::istream*      instream;
    bool                    shutdown;        // set to stop procs
    std::string             name;
    std::string             filename;
    vjThread*               read_connect_thread;
    vjThread*               write_connect_thread;
    vjSocket*               sock;
    vjConnectMode           mode;
    bool                    read_die;    // if true, thread suicide
    bool                    write_die;   // if true, thread suicide
    bool                    write_alive; // true when thread running

    //: used for storing vjCommand* in a priority queue
    struct vjCommandPtrCmp {
	bool operator() (const vjCommand* a, const vjCommand* b) {
	    return (a->next_fire_time > b->next_fire_time);
	}
    };


    std::priority_queue<vjCommand*, std::vector<vjCommand*>, vjCommandPtrCmp>
                               timed_commands; // used as heap
    std::queue<vjCommand*>     commands;

    //: controls access to commands & timed_commands queues.
    //  could we dispense with this???
    vjMutex                    commands_mutex;

    //: used to see if it's time to spring a timed_command
    vjTimeStamp             current_time;

    //: body of network process.
    void readControlLoop (void* nullParam);
    void writeControlLoop (void* nullParam);

    //: utility for controlLoop()
    bool readCommand (std::istream& fin);

    std::vector<vjNetCommunicator*> communicators;

}; // end vjConnect


#endif
