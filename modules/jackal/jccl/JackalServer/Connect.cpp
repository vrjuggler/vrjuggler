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


// File output for Environment Manager
//
// author: Christopher Just


#include <vjConfig.h>

#include <Environment/vjConnect.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjConfigChunkDB.h>
#include <Config/vjChunkFactory.h>
#include <Environment/vjEnvironmentManager.h>
#include <Environment/vjTimedUpdate.h>
#include <Kernel/vjConfigManager.h>
#include <Config/vjConfigTokens.h>
#include <vpr/System.h>
//#include <Config/vjXMLConfigIO.h>

#include <Environment/vjXMLConfigCommunicator.h>

vjConnect::vjConnect(vjSocket* s, const std::string& _name,
           vjConnectMode _mode): name(""), filename(""), commands_mutex(), communicators() {
    vjDEBUG(vjDBG_ENV_MGR,4) << "EM: Creating vjConnect to file or socket\n"
          << vjDEBUG_FLUSH;
    sock = s;
    mode = _mode;

    write_alive = false;
    filename = "Socket/Pipe";
    name = _name;
    read_connect_thread = NULL;
    write_connect_thread = NULL;
    read_die = write_die = false;

    // populate communicators vector here.
    communicators.push_back (new vjXMLConfigCommunicator());

    outstream = sock->getOutputStream();
    instream = sock->getInputStream();

    // we need to add a chunk describing ourself
    vjConfigChunk* ch = vjChunkFactory::instance()->createChunk ("FileConnect");
    if (ch) {
        ch->setProperty ("Name", name);
        ch->setProperty ("Mode", VJC_INTERACTIVE);
        ch->setProperty ("filename", filename);
        ch->setProperty ("Enabled", true);
        vjConfigManager::instance()->addActive(ch);              // Add to active config
    }

    // communicators initialize connection
    // send initconnections...
    std::vector<vjNetCommunicator*>::iterator i;
    for (i = communicators.begin(); i != communicators.end(); i++) {
        (*i)->initConnection(this);
    }
}



vjConnect::vjConnect(vjConfigChunk* c): commands_mutex(), communicators() {

    sock = NULL;
    filename = (std::string)c->getProperty ("FileName");
    name = (std::string)c->getProperty ("Name");
    mode = (vjConnectMode)(int)c->getProperty ("Mode");

    read_die = write_die = false;
    read_connect_thread = NULL;
    write_connect_thread = NULL;

    switch (mode) {
    case VJC_OUTPUT:
        outstream = new std::ofstream (filename.c_str(), std::ios::out);
        if (!outstream)
            vjDEBUG(vjDBG_ALL,0) << clrOutNORM(clrRED, "ERROR:") << " file open failed for \"" << filename.c_str()
                                 << "\"\n" << vjDEBUG_FLUSH;
        break;
    case VJC_INPUT:
        instream = new std::ifstream (filename.c_str(), std::ios::in);
        if (!instream)
            vjDEBUG(vjDBG_ALL,0) << clrOutNORM(clrRED, "ERROR:") << " file open failed for \"" << filename.c_str()
                                 << "\"\n" << vjDEBUG_FLUSH;
        break;
    case VJC_INTERACTIVE:
        instream = new std::fstream (filename.c_str(),
                                     std::ios::in | std::ios::out);
        if (!instream)
            vjDEBUG(vjDBG_ALL,0) << clrOutNORM(clrRED, "ERROR:") << " file open failed for \"" << filename.c_str()
                                 << "\"\n" << vjDEBUG_FLUSH;
        outstream = (std::fstream*)instream;
        break;
    }

    // logging information to output file...
    if (mode == VJC_OUTPUT)
        *outstream << "VR Juggler FileConnect output " << name.c_str()
                   << std::endl;

    // populate communicators vector
    // send initconnections...
    std::vector<vjNetCommunicator*>::iterator i;
    for (i = communicators.begin(); i != communicators.end(); i++) {
        (*i)->initConnection(this);
    }
}



vjConnect::~vjConnect() {
    // send shutdownconnections...
    std::vector<vjNetCommunicator*>::iterator i;
    for (i = communicators.begin(); i != communicators.end(); i++) {
        (*i)->shutdownConnection();
    }

    stopProcess();
    //close (fd);
}



bool vjConnect::startProcess() {
   if (write_connect_thread)
      return true;

   bool success = true;

   read_die = write_die = false;

   if (mode == VJC_OUTPUT || mode == VJC_INTERACTIVE)
   {
      vpr::ThreadMemberFunctor<vjConnect> *writeMemberFunctor =
      new vpr::ThreadMemberFunctor<vjConnect>(this,
                                           &vjConnect::writeControlLoop,
                                           NULL);
      write_connect_thread = new vpr::Thread (writeMemberFunctor);
      success = success && write_connect_thread;
   }
   if (mode == VJC_INPUT || mode == VJC_INTERACTIVE)
   {
      vpr::ThreadMemberFunctor<vjConnect> *readMemberFunctor =
      new vpr::ThreadMemberFunctor<vjConnect>(this,
                                           &vjConnect::readControlLoop,
                                           NULL);
      read_connect_thread = new vpr::Thread (readMemberFunctor);
      success = success && read_connect_thread;
   }


   return success;
}



bool vjConnect::stopProcess() {
//      sendDisconnect();
    read_die = write_die = true;
    if (read_connect_thread) {
        read_connect_thread->kill();
        delete read_connect_thread;
        read_connect_thread = NULL;
    }
    if (write_connect_thread) {
        while (write_alive)
            std::cout << "waiting for write to die" << std::endl;
        //write_connect_thread->kill();
        delete write_connect_thread;
        write_connect_thread = NULL;
    }
    if (sock) {
        delete (sock);
    }
    else {
        delete outstream;
        if (outstream != (void*)instream)
            delete instream;
    }
    instream = 0;
    outstream = 0;
    sock = 0;
    return true;
}


void vjConnect::sendDescDB (vjChunkDescDB* db) {
    if (mode != VJC_INPUT)
        commands.push (new vjCommandSendDescDB (db));
}


void vjConnect::sendDisconnect () {
    std::cerr << "vjConnect::sendDisconnect not implemented!!!" << std::endl;
    //    if (mode != VJC_INPUT)
    //   commands.push (new vjCommandDisconnect());
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
   vjDEBUG(vjDBG_ENV_MGR,5) << "vjConnect " << name.c_str()
             << " started read control loop.\n"
             << vjDEBUG_FLUSH;
   while (!read_die) {
       if (!(*instream) || instream->eof())
      break;
      if (!readCommand (*instream))
          ; // do some error handling...
   }
   vjDEBUG(vjDBG_ENV_MGR,5) << "vjConnect " << name.c_str()
             <<" ending read control loop.\n" << vjDEBUG_FLUSH;

   read_connect_thread = NULL;
   read_die = write_die = true;
//**//   vjKernel::instance()->getEnvironmentManager()->connectHasDied(this);
}



void vjConnect::writeControlLoop(void* nullParam) {
    /* this probably needs considerable revision */
    vjCommand*  cmd;
    write_alive = true;

//              *outstream << "another test : ( \n" << flush;

    vjDEBUG(vjDBG_ENV_MGR,5) << "vjConnect " << name.c_str()
                             << " started write control loop.\n"
                             << vjDEBUG_FLUSH;

    while (!write_die) {
//          cout << "writing in write loop " << flush;
//          *outstream << "yet another test : ( \n" << flush;
//          cout << " -done\n" << flush;

        vpr::System::usleep (300000); // half a sec - find a better way to do this...
        if (!outstream)
            break;

        commands_mutex.acquire();

        while (!commands.empty()) {
            cmd = commands.front();
            commands.pop();
            vjDEBUG (vjDBG_ENV_MGR, 5) << "calling EM command "
                                       << cmd->getName().c_str()
                                       <<vjDEBUG_FLUSH;
            cmd->call (*outstream);
            vjDEBUG (vjDBG_ENV_MGR, 5) << " -- done.\n" << vjDEBUG_FLUSH;
            delete cmd;
        }

        current_time.set();

        while (!timed_commands.empty()) {
            cmd = timed_commands.top();
            if (current_time.usecs() < (cmd->next_fire_time * 1000))
                break;
            timed_commands.pop();
            cmd->call (*outstream);
            cmd->resetFireTime (current_time);
            timed_commands.push (cmd);
        }

        commands_mutex.release();

    } // end main loop
    vjDEBUG (vjDBG_ENV_MGR,5) << "vjConnect " << name.c_str() << " ending write loop.\n" << vjDEBUG_FLUSH;
    //write_connect_thread = NULL;
    write_alive = false;
}

bool vjConnect::readCommand(std::istream& fin) {
    const int buflen(512);
    char buf[buflen];
    char *c1, *c2;
    char* protocol_start_string = "<protocol handler=\"";
    int protocol_start_string_len = strlen (protocol_start_string);
    char* protocol_name;
    vjNetCommunicator *communicator = 0;

    // this is a little inflexible, but nice & quick.
    if (!fin.getline (buf, buflen, '\n'))
        return false;
    if (strncmp (buf, protocol_start_string, protocol_start_string_len)) {
        vjDEBUG (vjDBG_ENV_MGR, 3) << "Connection: Invalid protocol identifier." << std::endl << vjDEBUG_FLUSH;
        do {
            if (!fin.getline (buf, buflen, '\n'))
                return false;
        } while (strncmp (buf, protocol_start_string, protocol_start_string_len));
    }
    c1 = buf + protocol_start_string_len;
    c2 = strstr (c1, "\"");
    if (c2) {
        int len = c2-c1;
        protocol_name = new char[len+1];
        strncpy (protocol_name, c1, len);
        protocol_name[len] = 0;
    }
    else {
        vjDEBUG (vjDBG_ENV_MGR, 3) << "foo\n" << vjDEBUG_FLUSH;
        return false;
    }

//      protocol_name = strtok_r (buf+protocol_start_string_len, "\"", &c);
//      cout << "using protocol " << protocol_name << std::endl;
        
    // find a communicator for this protocol.
    std::vector<vjNetCommunicator*>::iterator i;
    for (i = communicators.begin(); i != communicators.end(); i++) {
        if ((*i)->acceptsStreamIdentifier(protocol_name)) {
            communicator = *i;
            break;
        }
    }

    if (communicator) {
        return communicator->readStream (fin, buf);
    }
    else {
        vjDEBUG (vjDBG_ENV_MGR, 1) << "Connection: Couldn't find protocol handler for '" << protocol_name << "'\n" << vjDEBUG_FLUSH;
        // should we scan for </protocol> here?
        return false;
    }
}


//  bool vjConnect::readCommand(std::istream& fin) {
//      // reads one command.  called from controlloop
//      const int   buflen = 512;
//      char        rbuf[buflen];    // HACK! can't handle lines longer than buflen
//      char*       s;

//      if (!fin.getline(rbuf,buflen,'\n'))
//          return false;

//      vjDEBUG(vjDBG_ENV_MGR,4) << "vjConnect:: read: '" << rbuf
//                               << "'.\n" << vjDEBUG_FLUSH;

//      s = strtok (rbuf, " \t\n");
//      if (!s) {
//          vjDEBUG(vjDBG_ERROR,1) << "couldn't get a token.  something's really wrong in vjConnect\n"
//                                 << vjDEBUG_FLUSH;
//      }

//      if (!strcasecmp (s, get_TOKEN)) {
//          s = strtok (NULL, " \t\n");
//          if (!strcasecmp (s, descriptions_TOKEN)) {
//              vjChunkDescDB* db = vjChunkFactory::instance()->getChunkDescDB();
//              vjDEBUG(vjDBG_ENV_MGR,4) << "vjConnect: Sending (requested) chunkdesc.\n" << vjDEBUG_FLUSH;
//              vjDEBUG(vjDBG_ENV_MGR,5) << *db << std::endl << vjDEBUG_FLUSH;
//              sendDescDB (db);
//          }
//          else if (!strcasecmp (s,chunks_TOKEN)) {
//              vjConfigManager::instance()->lockActive();
//              vjConfigChunkDB* db = new vjConfigChunkDB((*(vjConfigManager::instance()->getActiveConfig())));   // Make a copy
//              vjConfigManager::instance()->unlockActive();

//              vjDEBUG(vjDBG_ENV_MGR,4) << "vjConnect: Sending (requested) chunkdb.\n" << vjDEBUG_FLUSH;
//              vjDEBUG(vjDBG_ENV_MGR,5) << *db << std::endl << vjDEBUG_FLUSH;
//              sendChunkDB (db, true);
//          }
//          else {
//              vjDEBUG(vjDBG_ERROR,1)
//                 << "Error: vjConnect:: Received unknown GET: " << s
//                 << std::endl << vjDEBUG_FLUSH;
//          }
//      }

//      else if (!strcasecmp (s, descriptions_TOKEN)) {
//          /* message contains one or more descriptions, to
//           * be read in just like a ChunkDescDB.  If the
//           * descriptions line itself contains the word
//           * "all", then we should clear the db first.
//           */
//          // XXX: Hack!!! We need to change this. We should not
//          // change the dbs outside of kernel
//          //s = strtok (NULL, " \t\n");
//          vjDEBUG(vjDBG_ERROR,0) << "EM Receive descriptions disabled!!!\n" << vjDEBUG_FLUSH;
//          //if (!strcasecmp (s, "all") && (cachedChunkdb->isEmpty()))
//          //    cachedDescdb->removeAll();
//          //fin >> *cachedDescdb;
//      }

//      else if (!strcasecmp (s, chunks_TOKEN)) {
//          /* message contains one or more chunks.  If the
//           * descriptions line contains "all", we should
//           * clear the db first
//           */
//          //s = strtok (NULL, " \t\n");
//          // chunks 'all' option disabled for now...
//          //if (!strcasecmp (s, "all"))
//          //   chunkdb->removeAll()
//          vjDEBUG(vjDBG_ENV_MGR,1) << "vjConnect:: Read: chunks: Started\n" << vjDEBUG_FLUSH;

//          vjConfigChunkDB* newchunkdb = new vjConfigChunkDB;
//          // new cool xml testing stuff.
//          vjXMLConfigIO configio;
//          configio.readConfigChunkDB (fin, *newchunkdb);
//          //fin >> *newchunkdb;
//          vjDEBUG(vjDBG_ENV_MGR,5) << *newchunkdb << std::endl << vjDEBUG_FLUSH;
//          vjDEBUG(vjDBG_ENV_MGR,3) << "vjConnect:: Read: chunks: Completed\n" << vjDEBUG_FLUSH;
//          // ALLEN: PUT A FUNCTION HERE FOR THE KERNEL TO LOOK AT NEWCHUNKDB
//          vjConfigManager::instance()->addChunkDB(newchunkdb);    // Adds chunks to the pending list
//          vjDEBUG(vjDBG_ENV_MGR,3) << "vjConnect: Added chunks to vjConfigManager pending list to add\n" << vjDEBUG_FLUSH;
//      }

//      else if (!strcasecmp (s, remove_TOKEN)) {
//          s = strtok (NULL, " \t\n");
//          if (!strcasecmp (s, descriptions_TOKEN)) {
//              while ( (s = strtok (NULL, " \t\n")) ) {
//                  // BUG! - what if chunks exist in db using the desc we're removing?
//                  //cachedDescdb->remove(s);
//                  vjDEBUG(vjDBG_ENV_MGR,3) << "EM Remove Descriptions disabled!\n" << vjDEBUG_FLUSH;
//              }
//          }
//          else if (!strcasecmp (s, chunks_TOKEN)) {
//              vjConfigChunkDB* remove_chunk_db = new vjConfigChunkDB();

//              vjDEBUG(vjDBG_ENV_MGR,5) << "vjConnect: Remove: chunks: Starting...\n"  << vjDEBUG_FLUSH;

//              fin >> *remove_chunk_db;       // Read in the chunks to remove

//              vjDEBUG(vjDBG_ENV_MGR,5) << *remove_chunk_db << std::endl
//                                       << vjDEBUG_FLUSH;

//              // Tell config manager to remove the chunks
//              vjConfigManager::instance()->removeChunkDB(remove_chunk_db);     // Add chunks to pending list as removes
//              vjDEBUG(vjDBG_ENV_MGR,3) << "vjConnect: Remove chunks added to vjConfigManager pending list\n" << vjDEBUG_FLUSH;
//          }
//          else
//              vjDEBUG(vjDBG_ERROR,3) << "Error: vjConnect: Unknown remove type: "
//                                     << s << std::endl << vjDEBUG_FLUSH;
//      }
//      else {
//          vjDEBUG(vjDBG_ERROR,0) << "Error: vjConnect:: Unknown command '"
//                                 << s << "'\n" << vjDEBUG_FLUSH;
//      }
//      return true;
//  }
