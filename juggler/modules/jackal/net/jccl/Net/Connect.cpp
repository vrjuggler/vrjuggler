// vjFileConnect.cpp
// File output for Environment Manager
//
// author: Christopher Just


#include <Environment/vjConnect.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjConfigChunkDB.h>
#include <Config/vjChunkFactory.h>
#include <Environment/vjTimedUpdate.h>
#include <Kernel/vjKernel.h>

#include <iostream.h>
#include <fstream.h>
#include <sys/types.h>
#include <sys/poll.h>



vjConnect::vjConnect(int s, const std::string& _name,
		     vjConnectMode _mode): output(), commands_mutex() {
    vjDEBUG(vjDBG_ENV_MGR,2) << "EM: Creating vjConnect to file or socket\n"
	       << vjDEBUG_FLUSH;
    fd = s;
    mode = _mode;

    filename = "no_file_name";
    name = _name;
    read_connect_thread = NULL;
    write_connect_thread = NULL;
    output.attach(fd);

    // we need to add a chunk describing ourself
    // THIS IS A HUGE UGLY HACK! THERE SHOULD BE A CLEANER WAY FOR
    // MANAGERS TO MANIPULATE THE ACTIVE CONFIG!!
    vjConfigChunkDB db;
    vjConfigChunk* ch = vjChunkFactory::createChunk ("FileConnect");
    if (ch) {
	ch->setProperty ("Name", name);
	ch->setProperty ("Mode", VJC_INTERACTIVE);
	ch->setProperty ("filename", "Socket/Pipe");
	ch->setProperty ("Enabled", true);
	db.addChunk(ch);
	vjKernel::instance()->configAdd(&db);
    }

}



vjConnect::vjConnect(vjConfigChunk* c): output() {


    filename = (std::string)c->getProperty ("FileName");
    name = (std::string)c->getProperty ("Name");
    //readable = c->getProperty ("Readable");
    mode = (vjConnectMode)(int)c->getProperty ("Mode");

    read_connect_thread = NULL;
    write_connect_thread = NULL;
    switch (mode) {
    case VJC_OUTPUT:
	fd = open (filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0660 );
	break;
    case VJC_INPUT:
	fd = open (filename.c_str(), O_RDONLY, 0);
	break;
    case VJC_INTERACTIVE:
	fd = open (filename.c_str(), O_RDWR | O_CREAT, 0660);
	break;
    }
    if (fd == -1) {
	vjDEBUG(vjDBG_ALL,0) << "ERROR: file open failed for \"" << filename
			     << "\"\n" << vjDEBUG_FLUSH;
    }
    output.attach (fd);

    // logging information to output file...
    if (mode == VJC_OUTPUT)
	output << "VR Juggler FileConnect output " << name << endl;
}



vjConnect::~vjConnect() {
    if (read_connect_thread) {
        read_connect_thread->kill();
	delete read_connect_thread;
    }
    if (write_connect_thread) {
	write_connect_thread->kill();
	delete write_connect_thread;
    }

    close (fd);
}



bool vjConnect::startProcess() {
   if (write_connect_thread)
      return true;

   bool success = true;

   shutdown = false;
   // Create a new thread to handle the control

   if (mode == VJC_OUTPUT || mode == VJC_INTERACTIVE)
   {
      vjThreadMemberFunctor<vjConnect> *writeMemberFunctor =
      new vjThreadMemberFunctor<vjConnect>(this,
                                           &vjConnect::writeControlLoop,
                                           NULL);
      write_connect_thread = new vjThread (writeMemberFunctor, 0);
      success = success && write_connect_thread;
   }
   if (mode == VJC_INPUT || mode == VJC_INTERACTIVE)
   {
      vjThreadMemberFunctor<vjConnect> *readMemberFunctor =
      new vjThreadMemberFunctor<vjConnect>(this,
                                           &vjConnect::readControlLoop,
                                           NULL);
      read_connect_thread = new vjThread (readMemberFunctor, 0);
      success = success && read_connect_thread;
   }

   return success;
}



bool vjConnect::stopProcess() {
    // should use shutdown
    shutdown = true;
    return true;
}


void vjConnect::sendDescDB (vjChunkDescDB* db) {
    if (mode != VJC_INPUT)
	commands.push (new vjCommandSendDescDB (db));
}


void vjConnect::sendChunkDB (vjConfigChunkDB* db, bool all) {
    if (mode != VJC_INPUT)
	commands.push (new vjCommandSendChunkDB (db, all));
}


void vjConnect::sendRefresh () {
    if (mode == VJC_INTERACTIVE)
	commands.push (new vjCommandRefresh);
}



//! ARGS: _tu - a vjTimedUpdate*
//! ARGS: _refresh_time - time between refreshes, in milliseconds
void vjConnect::addTimedUpdate (vjTimedUpdate* _tu, float _refresh_time) {
    if (mode != VJC_INPUT) {
	commands_mutex.acquire();
	timed_commands.push (new vjCommandTimedUpdate (_tu, _refresh_time));
	commands_mutex.release();
    }
}



void vjConnect::removeTimedUpdate (vjTimedUpdate* _tu) {
    // this better not be called often - it's gotta be nlogn or something.
    // still, there'll probably never be more than a couple dozen
    // items in the timed_commands queue anyway.
    std::priority_queue<vjCommand*, std::vector<vjCommand*>, vjCommandPtrCmp> newq;
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


//----------------- PRIVATE utility functions ---------------------------

void vjConnect::readControlLoop(void* nullParam) {
   vjDEBUG(vjDBG_ENV_MGR,2) << "vjConnect " << name
			    << " started read control loop.\n"
			    << vjDEBUG_FLUSH;
   ifstream fin(fd);

   while (!shutdown) {
       if (fin.eof())
	   break;
       readCommand (fin);
   }
}



void vjConnect::writeControlLoop(void* nullParam) {
   /* this probably needs considerable revision */
   vjCommand*  cmd;

   vjDEBUG(vjDBG_ENV_MGR,2) << "vjConnect " << name
			    << " started write control loop.\n"
			    << vjDEBUG_FLUSH;

   while (!shutdown) {
       usleep (500000); // half a sec - find a better way to do this...

       commands_mutex.acquire();

       while (!commands.empty()) {
	   cmd = commands.front();
	   commands.pop();
	   cmd->call (output);
	   vjDEBUG (vjDBG_ENV_MGR, 5) << "called EM command " << cmd->getName() << endl <<vjDEBUG_FLUSH;
	   delete cmd;
       }

       current_time.set();

       //cout << "current time is " << current_time.usecs()/1000 << " msecs" << endl;
       while (!timed_commands.empty()) {
	   cmd = timed_commands.top();
	   if (current_time.usecs() < (cmd->next_fire_time * 1000))
	       break;
	   timed_commands.pop();
	   cmd->call (output);
	   cmd->resetFireTime (current_time);
	   timed_commands.push (cmd);
       }

       commands_mutex.release();

   } // end main loop

}



void vjConnect::readCommand(ifstream& fin) {
   // reads one command.  called from controlloop
   const int   buflen = 512;
   char        rbuf[buflen];    // HACK! can't handle lines longer than buflen
   char        c;
   char*       s;

   if (!fin.getline(rbuf,buflen,'\n'))
       return;

   vjDEBUG(vjDBG_ENV_MGR,3) << "vjConnect:: read: '" << rbuf
			    << "'.\n" << vjDEBUG_FLUSH;

   s = strtok (rbuf, " \t\n");
   if (!s) {
       cout << "couldn't get a token.  something's really wrong in vjConnect" << endl;
   }

   if (!strcasecmp (s, "get"))
   {
      s = strtok (NULL, " \t\n");
      if (!strcasecmp (s, "descriptions"))
      {
         vjChunkDescDB* db = vjChunkFactory::getChunkDescDB();
         vjDEBUG(vjDBG_ENV_MGR,1) << "vjConnect: Sending (requested) chunkdesc:\n" << vjDEBUG_FLUSH;
         vjDEBUG(vjDBG_ENV_MGR,3) << *db << endl << vjDEBUG_FLUSH;
         sendDescDB (db);
      }
      else if (!strcasecmp (s,"chunks"))
      {
         vjConfigChunkDB* db = vjKernel::instance()->getChunkDB();
         vjDEBUG(vjDBG_ENV_MGR,1) << "vjConnect: Sending (requested) chunkdb:\n" << vjDEBUG_FLUSH;
         vjDEBUG(vjDBG_ENV_MGR,3) << *db << endl << vjDEBUG_FLUSH;
         sendChunkDB (db, true);
      }
      else
      {
         vjDEBUG(vjDBG_ALL,1) << "Error: vjConnect:: Received unknown GET: " << s << endl << vjDEBUG_FLUSH;
      }
   }

   else if (!strcasecmp (s, "descriptions"))
   {
      /* message contains one or more descriptions, to
       * be read in just like a ChunkDescDB.  If the
       * descriptions line itself contains the word
       * "all", then we should clear the db first.
       */
      // XXX: Hack!!! We need to change this. We should not
      // change the dbs outside of kernel
      //s = strtok (NULL, " \t\n");
      vjDEBUG(vjDBG_ALL,0) << "EM Receive descriptions disabled!!!\n" << vjDEBUG_FLUSH;
      //if (!strcasecmp (s, "all") && (cachedChunkdb->isEmpty()))
      //    cachedDescdb->removeAll();
      //fin >> *cachedDescdb;
   }

   else if (!strcasecmp (s, "chunks"))
   {
      /* message contains one or more chunks.  If the
       * descriptions line contains "all", we should
       * clear the db first
       */
      //s = strtok (NULL, " \t\n");
      // chunks 'all' option disabled for now...
      //if (!strcasecmp (s, "all"))
      //	chunkdb->removeAll()
      vjDEBUG(vjDBG_ENV_MGR,1) << "vjConnect:: Read: chunks: Started\n" << vjDEBUG_FLUSH;

      vjConfigChunkDB newchunkdb;
      fin >> newchunkdb;
      vjDEBUG(vjDBG_ENV_MGR,3) << newchunkdb << endl << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_ENV_MGR,1) << "vjConnect:: Read: chunks: Completed\n" << vjDEBUG_FLUSH;
      // ALLEN: PUT A FUNCTION HERE FOR THE KERNEL TO LOOK AT NEWCHUNKDB
      vjKernel::instance()->configAdd(&newchunkdb);      // Add new chunks
      vjDEBUG(vjDBG_ENV_MGR,1) << "vjConnect: Kernel has added the new chunks\n" << vjDEBUG_FLUSH;
   }

   else if (!strcasecmp (s, "remove"))
   {
      s = strtok (NULL, " \t\n");
      if (!strcasecmp (s, "descriptions"))
      {
         while (s = strtok (NULL, " \t\n"))
         {
            // BUG! - what if chunks exist in db using the desc we're removing?
            //cachedDescdb->remove(s);
            vjDEBUG(vjDBG_ENV_MGR,1) << "EM Remove Descriptions disabled!\n" << vjDEBUG_FLUSH;
         }
      }
      else if (!strcasecmp (s, "chunks"))
      {
         vjConfigChunkDB remove_chunk_db;

         vjDEBUG(vjDBG_ENV_MGR,1) << "vjConnect: Remove: chunks: Starting...\n"  << vjDEBUG_FLUSH;

         fin >> remove_chunk_db;       // Read in the chunks to remove

         vjDEBUG(vjDBG_ENV_MGR,3) << remove_chunk_db << endl << vjDEBUG_FLUSH;
         vjDEBUG(vjDBG_ENV_MGR,1) << "vjConnect: Remove: chunks: Completed.\n" << vjDEBUG_FLUSH;

         // Tell kernel to remove the chunks

         vjKernel::instance()->configRemove(&remove_chunk_db);
         vjDEBUG(vjDBG_ENV_MGR,1) << "vjConnect: Kernel has removed the chunks\n" << vjDEBUG_FLUSH;
      }
      else
         vjDEBUG(vjDBG_ENV_MGR,1) << "Error: vjConnect: Unknown remove type: "
         << s << endl << vjDEBUG_FLUSH;
   }
#if 0
   // obsolete timed update stuff
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
   else
   {
      vjDEBUG(vjDBG_ALL,0) << "Error: vjConnect:: Unknown command '"
      << s << "'\n" << vjDEBUG_FLUSH;
   }
}
