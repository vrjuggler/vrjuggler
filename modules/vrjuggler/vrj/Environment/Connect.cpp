// vjFileConnect.cpp
// File output for Environment Manager
//
// author: Christopher Just


#include <Environment/vjConnect.h>
#include <Kernel/vjKernel.h>
#include <iostream.h>
#include <fstream.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjConfigChunkDB.h>


vjConnect::vjConnect(int s, const std::string& _name): output(), commands_mutex() {
    vjDEBUG(2) << "EM: Creating vjConnect to file or socket\n"
	       << vjDEBUG_FLUSH;
    fd = s;
    readable = true;

    filename = "no_file_name";
    name = _name;
    connect_thread = NULL;
    output.attach(fd);
}



vjConnect::vjConnect(vjConfigChunk* c): output() {

    filename = (std::string)c->getProperty ("FileName");
    name = (std::string)c->getProperty ("Name");
    readable = c->getProperty ("Readable");

    connect_thread = NULL;
    fd = open (filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0660 );
    if (fd == -1) {
	vjDEBUG(0) << "ERROR: file open failed for " << filename << endl
		   << vjDEBUG_FLUSH;
    }
    output.attach (fd);
    output << "VR Juggler FileConnect output " << name << endl;
    output << flush;
    vjDEBUG(0) << "Created vjConnect for " << name << endl << vjDEBUG_FLUSH;
}



vjConnect::~vjConnect() {
    stopProcess();
    close (fd);
}



bool vjConnect::startProcess() {
    if (connect_thread)
	return true;

    vjDEBUG(0) << "starting vjConnect process for " << name << endl << vjDEBUG_FLUSH;
    // Create a new thread to handle the control
    vjThreadMemberFunctor<vjConnect> *memberFunctor =
        new vjThreadMemberFunctor<vjConnect>(this,
						 &vjConnect::controlLoop,
						 NULL);
    connect_thread = new vjThread (memberFunctor, 0);
    return (connect_thread != NULL);
}



bool vjConnect::stopProcess() {
    // should use shutdown
    if (connect_thread)
        connect_thread->kill();
    connect_thread = NULL;

    return true;
}


void vjConnect::sendDescDB (vjChunkDescDB* db) {
    commands.push (new vjCommandSendDescDB (db));
}


void vjConnect::sendChunkDB (vjConfigChunkDB* db, bool all) {
    commands.push (new vjCommandSendChunkDB (db, all));
}


void vjConnect::sendRefresh () {
    commands.push (new vjCommandRefresh);
}


void vjConnect::controlLoop(void* nullParam) {
   /* this probably needs considerable revision */
   vjCommand*  cmd;
   struct pollfd pollfdstruct;

   shutdown = 0;

   pollfdstruct.fd = fd;
   pollfdstruct.events = POLLPRI;// | POLLHUP | POLLNVAL;
   pollfdstruct.revents = 0;

   vjDEBUG(0) << "vjConnect " << name << " started control loop.\n"
   << vjDEBUG_FLUSH;

   /* attach iostreams to socket */
   ifstream fin(fd);

   while (!shutdown)
   {
       pollfdstruct.revents = 0;
       poll (&pollfdstruct, 1, 500); // check 2x/sec responsive enough?

       //cout << "connect loop " << name << " finished poll, revents = " << pollfdstruct.revents << endl;

       if ((pollfdstruct.revents & POLLHUP) || (pollfdstruct.revents & POLLNVAL)) {
	   vjDEBUG(0) << "vjConnect to file " << fd << " exiting\n"
		      << vjDEBUG_FLUSH;
	   break;
       }
       //else if ((pollfdstruct.revents & POLLIN) || (pollfdstruct.revents & POLLPRI)) {
       if (readable)
	   readCommand(fin);
	   //}

       commands_mutex.acquire();

       while (!commands.empty()) {
	   cmd = commands.front();
	   commands.pop();
	   cmd->call (output);
	   delete cmd;
       }

       current_time.set();
       //cout << "timed commands have " << timed_commands.size() << " entries\n";
       //cout << "current time is " << current_time.usecs()/1000 << " msecs" << endl;
       while (!timed_commands.empty()) {
	   cmd = timed_commands.top();
	   //cout << "examing timed command; next fire time is " << cmd->next_fire_time << endl;
	   if (current_time.usecs() < (cmd->next_fire_time * 1000))
	       break;
	   timed_commands.pop();
	   //cout << cmd->getName() << " - fire!!" << endl;
	   cmd->call (output);
	   cmd->resetFireTime (current_time);
	   timed_commands.push (cmd);
       }

       commands_mutex.release();

   } // end main loop

   stopProcess();
}



void vjConnect::readCommand(ifstream& fin) {
    // reads one command.  called from controlloop
    const int   buflen = 512;
    char        rbuf[buflen];    // HACK! can't handle lines longer than buflen
    char        c;
    char*       s;

    if (!fin.get(rbuf,buflen,'\n')) {
	//vjDEBUG(0) << "Couldn't read command for connection " << name << endl
	//	   << vjDEBUG_FLUSH;
	return;
    }
    vjDEBUG(0) << "vjConnect:: read: '" << rbuf
	       << "'.\n" << vjDEBUG_FLUSH;
    fin.get(c);
    if (c != '\n')
	vjDEBUG(1) << "Error: vjConnect:: oops - "
	    "didn't completely get command line\n"
	    "from the socket.  This is a bug!\n"
		   << vjDEBUG_FLUSH;

    s = strtok (rbuf, " \t\n");

    if (!strcasecmp (s, "get")) {
	s = strtok (NULL, " \t\n");
	if (!strcasecmp (s, "descriptions")) {
	    vjChunkDescDB* db = vjKernel::instance()->getInitialChunkDB()->getChunkDescDB();
	    //cout << "Sending descDB:\n" << *db << endl << flush;
            sendDescDB (db);
	}
	else if (!strcasecmp (s,"chunks")) {
	    vjConfigChunkDB* db = vjKernel::instance()->getInitialChunkDB();
            //cout << "Sending chunkdb:\n" << *db << endl << flush;
            sendChunkDB (db, true);
	}
	else {
            vjDEBUG(1) << "Error: vjConnect:: Received "
		"unknown GET " << s << endl << vjDEBUG_FLUSH;
	}
    }

    else if (!strcasecmp (s, "descriptions")) {
	/* message contains one or more descriptions, to
	 * be read in just like a ChunkDescDB.  If the
	 * descriptions line itself contains the word
	 * "all", then we should clear the db first.
	 */
	// XXX: Hack!!! We need to change this. We should not
	// change the dbs outside of kernel
	//s = strtok (NULL, " \t\n");
	vjDEBUG(0) << "EM Receive descriptions disabled!!!\n" << vjDEBUG_FLUSH;
	//if (!strcasecmp (s, "all") && (cachedChunkdb->isEmpty()))
        //    cachedDescdb->removeAll();
	//fin >> *cachedDescdb;
    }

    else if (!strcasecmp (s, "chunks")) {
	/* message contains one or more chunks.  If the
	 * descriptions line contains "all", we should
	 * clear the db first
	 */
	s = strtok (NULL, " \t\n");
	// chunks 'all' option disabled for now...
	//if (!strcasecmp (s, "all"))
	//	chunkdb->removeAll();
	//fin >> *chunkdb;
	vjConfigChunkDB newchunkdb (vjKernel::instance()->getInitialChunkDB()->getChunkDescDB());
	fin >> newchunkdb;
	vjDEBUG(0) << "READ CHUNKS:\n" << newchunkdb << vjDEBUG_FLUSH;
	// ALLEN: PUT A FUNCTION HERE FOR THE KERNEL TO LOOK AT NEWCHUNKDB
	vjKernel::instance()->configAdd(&newchunkdb);      // Add new chunks
    }

    else if (!strcasecmp (s, "remove")) {
	cerr << "rec'd remove message" << endl;
	s = strtok (NULL, " \t\n");
	if (!strcasecmp (s, "descriptions")) {
            while (s = strtok (NULL, " \t\n")) {
		// BUG! - what if chunks exist in db using the desc we're removing?
		//cachedDescdb->remove(s);
		vjDEBUG(0) << "EM Remove Descriptions disabled!\n" << vjDEBUG_FLUSH;
            }
	}
	else if (!strcasecmp (s, "chunks")) {
            while (s = strtok (NULL, " \t\n")) {
		// ALLEN: THIS IS WHERE THE GUI HAS SENT A COMMAND TO
		// REMOVE A CHUNK - S IS THE NAME OF THE CHUNK
		//chunkdb->removeNamed(s);
            }
	}
	else
            vjDEBUG(1) << "Error: vjConnect: Unknown remove type: "
		       << s << endl << vjDEBUG_FLUSH;
    }
#if 0
    else if (!strncasecmp (s, "update", 5))
	{
	    s = strtok (NULL, " \t\n");
	    if (!strcasecmp (s, "start"))
		{
		    s = strtok (NULL, "\n");
		    //vjTimedUpdate *t = vjTimedUpdateFactory::generate(this, s);
		    //updates.push_back(t);
		    //t->startProcess();
		    cerr << "got timedupdate start cmd" << endl;
		}
	    else if (!strcasecmp (s, "stop"))
		{
		    cerr << "got a request to stop updates. not implemented yet" << endl;
		    s = strtok (NULL, " \t\n");
		    /*		if (!strcasecmp(s, "all")) {
				stopAllUpdates();
				}
				else
				stopUpdate(s);
		    */
            cerr << "got timedupdate stop cmd" << endl;
		}
	}
#endif
    else {
	cerr << "Error: vjConnect:: Unknown command '"
	     << s << "'" << endl;
    }
}
