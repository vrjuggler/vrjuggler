// vjFileConnect.C
// File output for Environment Manager
//
// author: Christopher Just


#include <Environment/vjConnect.h>
#include <Kernel/vjKernel.h>
#include <iostream.h>
#include <fstream.h>
#include <sys/types.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjConfigChunkDB.h>

char* vjConnect::default_name = "unnamed";

vjConnect::vjConnect(int s, char* _name) {
    vjDEBUG(2) << "EM: Creating vjConnect to file or socket\n"
	       << vjDEBUG_FLUSH;
    fd = s;
    readable = true;
    filename = strdup ("no_file_name");
    if (_name)
	   name = strdup (_name);
    else
	   name = strdup (default_name);
    cachedChunkdb = vjKernel::instance()->getChunkDB();
    cachedDescdb = cachedChunkdb->getChunkDescDB();
    connect_thread = NULL;
    output.attach(fd);
}



vjConnect::vjConnect(vjConfigChunk* c) {

    vjDEBUG(7) << "Creating vjConnect with chunk:\n"
	       << *c << vjDEBUG_FLUSH;

    filename = c->getProperty ("FileName");
    name = c->getProperty ("Name");
    readable = c->getProperty ("Readable");

    cachedChunkdb = vjKernel::instance()->getChunkDB();
    cachedDescdb = cachedChunkdb->getChunkDescDB();
    connect_thread = NULL;
    fd = open (filename, O_WRONLY | O_CREAT | O_TRUNC, 0660 );
    output.attach (fd);
    output << "VR Juggler FileConnect output " << name << endl;
}



vjConnect::~vjConnect() {
    stopProcess();
    close (fd);
    delete name;
    delete filename;
}



bool vjConnect::startProcess() {
    if (!readable)
	return false;
    if (connect_thread)
	return true;

    // Create a new thread to handle the control
    vjThreadMemberFunctor<vjConnect> *memberFunctor =
        new vjThreadMemberFunctor<vjConnect>(this,
						 &vjConnect::controlLoop,
						 NULL);
    connect_thread = new vjThread (memberFunctor, 0);
    return (connect_thread != NULL);
}



bool vjConnect::stopProcess() {
    if (connect_thread)
        connect_thread->kill();
    connect_thread = NULL;

    return true;
}


void vjConnect::sendDescDB (vjChunkDescDB* db) {
    output.lock();
    output << "descriptions\n" << *db << flush;
    output.unlock();
}

void vjConnect::sendChunkDB (vjConfigChunkDB* db, bool all) {
    output.lock();
    if (all)
	output << "chunks all\n";
    else
	output << "chunks\n";
    output << *db << flush;
    output.unlock();
}


void vjConnect::sendRefresh () {
    output.lock();
    output << "refresh\n";
    output.unlock();
}


void vjConnect::controlLoop(void* nullParam) {
   /* this probably needs considerable revision */
   char        c;
   char*       s;
   const int   buflen = 512;
   char        rbuf[buflen];    // HACK! can't handle lines longer than buflen

   vjDEBUG(5) << "vjConnect started control loop.\n"
   << vjDEBUG_FLUSH;

   cachedChunkdb = vjKernel::instance()->getChunkDB();
   cachedDescdb = cachedChunkdb->getChunkDescDB();

   /* attach iostreams to socket */
   ifstream fin(fd);

   for (;;)
   {
      if (!fin.get(rbuf,buflen,'\n'))
      {
         vjDEBUG(3) << "vjConnect to file " << fd << " exiting\n"
         << vjDEBUG_FLUSH;
         break;
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

      if (!strcasecmp (s, "get"))
      {
         s = strtok (NULL, " \t\n");
         if (!strcasecmp (s, "descriptions"))
         {
            cachedChunkdb = vjKernel::instance()->getInitialChunkDB();
            cachedDescdb = cachedChunkdb->getChunkDescDB();
            cout << "Sending descDB:\n" << cachedDescdb << endl << flush;
            sendDescDB (cachedDescdb);
            // send out descriptions;
            //output.lock();
            //output << "descriptions all\n" << *descdb << flush;
            //output.unlock();
         }
         else if (!strcasecmp (s,"chunks"))
         {
            cachedChunkdb = vjKernel::instance()->getInitialChunkDB();
            cachedDescdb = cachedChunkdb->getChunkDescDB();
            cout << "Sending chunkdb:\n" << cachedChunkdb << endl << flush;
            sendChunkDB (cachedChunkdb, true);
            // send out chunks;
            //output.lock();
            //output << "chunks all\n" << *chunkdb << flush;
            //output.unlock();
         }
         else
         {
            vjDEBUG(1) << "Error: vjConnect:: Received "
            "unknown GET "
            << s << endl << vjDEBUG_FLUSH;
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
         s = strtok (NULL, " \t\n");
         if (!strcasecmp (s, "all") && (cachedChunkdb->isEmpty()))
            cachedDescdb->removeAll();
         fin >> *cachedDescdb;
      }

      else if (!strcasecmp (s, "chunks"))
      {
         /* message contains one or more chunks.  If the
          * descriptions line contains "all", we should
          * clear the db first
          */
         s = strtok (NULL, " \t\n");
         // chunks 'all' option disabled for now...
         //if (!strcasecmp (s, "all"))
         //	chunkdb->removeAll();
         //fin >> *chunkdb;
         vjConfigChunkDB newchunkdb (cachedDescdb);
         fin >> newchunkdb;
         vjDEBUG(0) << "READ CHUNKS:\n" << newchunkdb << vjDEBUG_FLUSH;
         // ALLEN: PUT A FUNCTION HERE FOR THE KERNEL TO LOOK AT NEWCHUNKDB
         vjKernel::instance()->configAdd(&newchunkdb);      // Add new chunks
      }

      else if (!strcasecmp (s, "remove"))
      {
         cerr << "rec'd remove message" << endl;
         s = strtok (NULL, " \t\n");
         if (!strcasecmp (s, "descriptions"))
         {
            while (s = strtok (NULL, " \t\n"))
            {
               // BUG! - what if chunks exist in db using the desc we're removing?
               cachedDescdb->remove(s);
            }
         }
         else if (!strcasecmp (s, "chunks"))
         {
            while (s = strtok (NULL, " \t\n"))
            {
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
      else
      {
         cerr << "Error: vjConnect:: Unknown command '"
         << s << "'" << endl;
      }
   }
   stopProcess();
} // end processBody()
