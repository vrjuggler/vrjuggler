
#include <vjConfig.h>
#include <strings.h>    // For bzero()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <Environment/vjNetworkConnect.h>
#include <Kernel/vjKernel.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjConfigChunkDB.h>



vjNetworkConnect::vjNetworkConnect(int s) {
    sock = s;
    chunkdb = vjKernel::instance()->getChunkDB();
    descdb = chunkdb->getChunkDescDB();
    connect_thread = NULL;
    fout = NULL;
}



vjNetworkConnect::~vjNetworkConnect() {
    stopProcess();
}



bool vjNetworkConnect::startProcess() {
    // Create a new thread to handle the control
    vjThreadMemberFunctor<vjNetworkConnect> *memberFunctor =
	new vjThreadMemberFunctor<vjNetworkConnect>(this,
						    &vjNetworkConnect::controlLoop,
						    NULL);
    connect_thread = new vjThread (memberFunctor, 0);
    return (connect_thread != NULL);

}



bool vjNetworkConnect::stopProcess() {

    stopAllUpdates();

    // destroy process
    if (connect_thread)
	connect_thread->kill();
    if (fout) {
	delete fout;
	fout = NULL;
    }

    return 1;
}



void vjNetworkConnect::transmitData(char* s) {
    if (fout)
	*fout << s << flush;
}



void vjNetworkConnect::transmitData(vjConfigChunk* c) {
    if (fout)
	*fout << *c << flush;
}



void vjNetworkConnect::stopAllUpdates() {
    std::vector<vjTimedUpdate*>::iterator i;
    for (i = updates.begin(); i < updates.end(); i++) {
	delete(*i);
    }
    updates.erase(updates.begin(), updates.end());
}



void vjNetworkConnect::stopUpdate(char* s) {
    std::vector<vjTimedUpdate*>::iterator i;
    for (i = updates.begin(); i < updates.end(); i++) {
	if (!strcasecmp((*i)->name, s)) {
	    delete(*i);
	    updates.erase(i);
	}
    }
}



void vjNetworkConnect::controlLoop(void* nullParam) {
   char        c;
   char*       s;
   const int   buflen = 512;
   char        rbuf[buflen];    // HACK! can't handle lines longer than buflen

   vjDEBUG(5) << "vjNetworkConnect started control loop.\n"
   << vjDEBUG_FLUSH;

   chunkdb = vjKernel::instance()->getChunkDB();

   /* attach iostreams to socket */
   ifstream fin(sock);
   fout = new ofstream(sock);

   for (;;)
   {
      if (!fin.get(rbuf,buflen,'\n'))
      {
         cerr << "vjNetworkConnect read '" << rbuf
         << "' and exits" << endl;
         break;
      }
      vjDEBUG(7) << "vjNetworkConnect:: socket read: '" << rbuf << "'.\n" << vjDEBUG_FLUSH;

      fin.get(c);
      if (c != '\n')
         vjDEBUG(1) << "Error: vjNetworkConnect:: oops - "
         "didn't completely get command line\n"
         "from the socket.  This is a bug!\n"
         << vjDEBUG_FLUSH;

      s = strtok (rbuf, " \t\n");

      if (!strcasecmp (s, "get"))
      {
         s = strtok (NULL, " \t\n");
         if (!strcasecmp (s, "descriptions"))
         {
            // send out descriptions;
            *fout << "descriptions all\n" << *descdb << flush;
         }
         else if (!strcasecmp (s,"chunks"))
         {
            // send out chunks;
            *fout << "chunks all\n" << *chunkdb << flush;
         }
         else
         {
            vjDEBUG(1) << "Error: vjNetworkConnect:: Received "
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
         s = strtok (NULL, " \t\n");
         if (!strcasecmp (s, "all") && (chunkdb->isEmpty()))
            descdb->removeAll();
         fin >> *descdb;
      }

      else if (!strcasecmp (s, "chunks"))
      {
         /* message contains one or more chunks.  If the
          * descriptions line contains "all", we should
          * clear the db first
          */
         s = strtok (NULL, " \t\n");
         if (!strcasecmp (s, "all"))
            chunkdb->removeAll();
         fin >> *chunkdb;
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
               descdb->remove(s);
            }
         }
         else if (!strcasecmp (s, "chunks"))
         {
            while (s = strtok (NULL, " \t\n"))
            {
               chunkdb->removeNamed(s);
            }
         }
         else
            vjDEBUG(1) << "Error: vjNetworkConnect: Unknown remove type: "
            << s << endl << vjDEBUG_FLUSH;
      }

      else if (!strncasecmp (s, "update", 5))
      {
         s = strtok (NULL, " \t\n");
         if (!strcasecmp (s, "start"))
         {
            s = strtok (NULL, "\n");
            vjTimedUpdate *t = vjTimedUpdateFactory::generate(this, s);
            updates.push_back(t);
            t->startProcess();
         }
         else if (!strcasecmp (s, "stop"))
         {
            cerr << "got a request to stop updates. not implemented yet" << endl;
            s = strtok (NULL, " \t\n");
            if (!strcasecmp(s, "all"))
            {
               stopAllUpdates();
            }
            else
               stopUpdate(s);
         }
      }
      else
      {
         cerr << "Error: vjNetworkConnect:: Unknown command '"
         << s << "'" << endl;
      }
   }
   stopProcess();
} // end processBody()


