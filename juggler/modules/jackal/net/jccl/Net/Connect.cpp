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


#include <jccl/JackalServer/Connect.h>
#include <jccl/Config/ChunkDescDB.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/Config/ChunkFactory.h>
#include <jccl/Config/ConfigTokens.h>
#include <vpr/System.h>

#include <jccl/JackalServer/XMLConfigCommunicator.h>

namespace jccl {

Connect::Connect(Socket* s, const std::string& _name,
           ConnectMode _mode): name(""), filename(""), commands_mutex(), communicators() {
    vprDEBUG(vprDBG_ENV_MGR,4) << "EM: Creating Connect to file or socket\n"
          << vprDEBUG_FLUSH;
    sock = s;
    mode = _mode;

    write_alive = false;
    filename = "Socket/Pipe";
    name = _name;
    read_connect_thread = NULL;
    write_connect_thread = NULL;
    read_die = write_die = false;

//      // populate communicators vector here.
//      communicators.push_back (new XMLConfigCommunicator());

    outstream = sock->getOutputStream();
    instream = sock->getInputStream();

    // we need to add a chunk describing ourself
    connect_chunk = ChunkFactory::instance()->createChunk ("FileConnect");
    if (connect_chunk) {
        connect_chunk->setProperty ("Name", name);
        connect_chunk->setProperty ("Mode", VJC_INTERACTIVE);
        connect_chunk->setProperty ("filename", filename);
        connect_chunk->setProperty ("Enabled", true);
        //ConfigManager::instance()->addActive(ch);              // Add to active config
    }

    // communicators initialize connection
    // send initconnections...
    std::vector<NetCommunicator*>::iterator i;
    for (i = communicators.begin(); i != communicators.end(); i++) {
        (*i)->initConnection(this);
    }
}



Connect::Connect(ConfigChunk* c): commands_mutex(), communicators() {

    connect_chunk = c;
    sock = NULL;
    filename = (std::string)c->getProperty ("FileName");
    name = (std::string)c->getProperty ("Name");
    mode = (ConnectMode)(int)c->getProperty ("Mode");

    read_die = write_die = false;
    read_connect_thread = NULL;
    write_connect_thread = NULL;

    switch (mode) {
    case VJC_OUTPUT:
        outstream = new std::ofstream (filename.c_str(), std::ios::out);
        if (!outstream)
            vprDEBUG(vprDBG_ALL,0) << clrOutNORM(clrRED, "ERROR:") << " file open failed for \"" << filename.c_str()
                                 << "\"\n" << vprDEBUG_FLUSH;
        break;
    case VJC_INPUT:
        instream = new std::ifstream (filename.c_str(), std::ios::in);
        if (!instream)
            vprDEBUG(vprDBG_ALL,0) << clrOutNORM(clrRED, "ERROR:") << " file open failed for \"" << filename.c_str()
                                 << "\"\n" << vprDEBUG_FLUSH;
        break;
    case VJC_INTERACTIVE:
        instream = new std::fstream (filename.c_str(),
                                     std::ios::in | std::ios::out);
        if (!instream)
            vprDEBUG(vprDBG_ALL,0) << clrOutNORM(clrRED, "ERROR:") << " file open failed for \"" << filename.c_str()
                                 << "\"\n" << vprDEBUG_FLUSH;
        outstream = (std::fstream*)instream;
        break;
    }

    // logging information to output file...
    if (mode == VJC_OUTPUT)
        *outstream << "VR Juggler FileConnect output " << name.c_str()
                   << std::endl;

    // populate communicators vector
    // send initconnections...
    std::vector<NetCommunicator*>::iterator i;
    for (i = communicators.begin(); i != communicators.end(); i++) {
        (*i)->initConnection(this);
    }
}



Connect::~Connect() {
    // send shutdownconnections...
    std::vector<NetCommunicator*>::iterator i;
    for (i = communicators.begin(); i != communicators.end(); i++) {
        (*i)->shutdownConnection();
    }

    stopProcess();
    //close (fd);
}



bool Connect::startProcess() {
   if (write_connect_thread)
      return true;

   bool success = true;

   read_die = write_die = false;

   if (mode == VJC_OUTPUT || mode == VJC_INTERACTIVE)
   {
      vpr::ThreadMemberFunctor<Connect> *writeMemberFunctor =
      new vpr::ThreadMemberFunctor<Connect>(this,
                                           &Connect::writeControlLoop,
                                           NULL);
      write_connect_thread = new vpr::Thread (writeMemberFunctor);
      success = success && write_connect_thread;
   }
   if (mode == VJC_INPUT || mode == VJC_INTERACTIVE)
   {
      vpr::ThreadMemberFunctor<Connect> *readMemberFunctor =
      new vpr::ThreadMemberFunctor<Connect>(this,
                                           &Connect::readControlLoop,
                                           NULL);
      read_connect_thread = new vpr::Thread (readMemberFunctor);
      success = success && read_connect_thread;
   }


   return success;
}



bool Connect::stopProcess() {
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


//  void Connect::sendDescDB (ChunkDescDB* db) {
//      if (mode != VJC_INPUT)
//          commands.push (new CommandSendDescDB (db));
//  }


void Connect::sendDisconnect () {
    std::cerr << "Connect::sendDisconnect not implemented!!!" << std::endl;
    //    if (mode != VJC_INPUT)
    //   commands.push (new CommandDisconnect());
}


//  void Connect::sendChunkDB (ConfigChunkDB* db, bool all) {
//      if (mode != VJC_INPUT)
//          commands.push (new CommandSendChunkDB (db, all));
//  }


//  void Connect::sendRefresh () {
//      if (mode == VJC_INTERACTIVE)
//          commands.push (new CommandRefresh);
//  }


void Connect::addCommand (Command* cmd) {
    vprASSERT (cmd != 0);

    if (mode != VJC_INPUT) {
        commands_mutex.acquire();
        commands.push (cmd);
        commands_mutex.release();
    }
}


//! ARGS: _tu - a TimedUpdate*
//! ARGS: _refresh_time - time between refreshes, in milliseconds
void Connect::addPeriodicCommand (PeriodicCommand* pc) {
    if (mode != VJC_INPUT) {
        commands_mutex.acquire();
        periodic_commands.push (pc);
        commands_mutex.release();
    }
}



void Connect::removePeriodicCommand (PeriodicCommand* pc1) {
    // this better not be called often - it's gotta be nlogn or something.
    // still, there'll probably never be more than a couple dozen
    // items in the periodic_commands queue anyway.
    std::priority_queue<PeriodicCommand*, std::vector<PeriodicCommand*>, CommandPtrCmp> newq;
    PeriodicCommand* pc2;
    commands_mutex.acquire();
    while (!periodic_commands.empty()) {
        pc2 = periodic_commands.top();
        periodic_commands.pop();
        if (pc1 != pc2)
            newq.push (pc2);
    }
    periodic_commands = newq;
    commands_mutex.release();
}


//----------------- PRIVATE utility functions ---------------------------


void Connect::readControlLoop(void* nullParam) {
   vprDEBUG(vprDBG_ENV_MGR,5) << "Connect " << name.c_str()
             << " started read control loop.\n"
             << vprDEBUG_FLUSH;
   while (!read_die) {
       if (!(*instream) || instream->eof())
      break;
      if (!readCommand (*instream))
          ; // do some error handling...
   }
   vprDEBUG(vprDBG_ENV_MGR,5) << "Connect " << name.c_str()
             <<" ending read control loop.\n" << vprDEBUG_FLUSH;

   read_connect_thread = NULL;
   read_die = write_die = true;
//**//   Kernel::instance()->getEnvironmentManager()->connectHasDied(this);
}



void Connect::writeControlLoop(void* nullParam) {
    /* this probably needs considerable revision */
    Command*  cmd;
    PeriodicCommand* pcmd;
    write_alive = true;

//              *outstream << "another test : ( \n" << flush;

    vprDEBUG(vprDBG_ENV_MGR,5) << "Connect " << name.c_str()
                             << " started write control loop.\n"
                             << vprDEBUG_FLUSH;

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
            vprDEBUG (vprDBG_ENV_MGR, 5) << "calling EM command; protocol="
                                       << cmd->getProtocolName().c_str()
                                       <<vprDEBUG_FLUSH;
            *outstream << "<protocol handler=\"" << cmd->getProtocolName()
                       << "\">\n";
            cmd->call (*outstream);
            *outstream << "</protocol>\n" << std::flush;
            vprDEBUG (vprDBG_ENV_MGR, 5) << " -- done.\n" << vprDEBUG_FLUSH;
            delete cmd;
        }

        current_time.set();

        while (!periodic_commands.empty()) {
            pcmd = periodic_commands.top();
            if (current_time.usecs() < (pcmd->next_fire_time * 1000))
                break;
            periodic_commands.pop();
            *outstream << "<protocol handler=\"" << pcmd->getProtocolName()
                       << "\">\n";
            pcmd->call (*outstream);
            *outstream << "</protocol>\n" << std::flush;
            pcmd->resetFireTime (current_time);
            periodic_commands.push (pcmd);
        }

        commands_mutex.release();

    } // end main loop
    vprDEBUG (vprDBG_ENV_MGR,5) << "Connect " << name.c_str() << " ending write loop.\n" << vprDEBUG_FLUSH;
    //write_connect_thread = NULL;
    write_alive = false;
}

bool Connect::readCommand(std::istream& fin) {
    const int buflen(512);
    char buf[buflen];
    char *c1, *c2;
    char* protocol_start_string = "<protocol handler=\"";
    int protocol_start_string_len = strlen (protocol_start_string);
    char* protocol_name;
    NetCommunicator *communicator = 0;

    // this is a little inflexible, but nice & quick.
    if (!fin.getline (buf, buflen, '\n'))
        return false;
    if (strncmp (buf, protocol_start_string, protocol_start_string_len)) {
        vprDEBUG (vprDBG_ENV_MGR, 3) << "Connection: Invalid protocol identifier." << std::endl << vprDEBUG_FLUSH;
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
        vprDEBUG (vprDBG_ENV_MGR, 3) << "foo\n" << vprDEBUG_FLUSH;
        return false;
    }

//      protocol_name = strtok_r (buf+protocol_start_string_len, "\"", &c);
//      cout << "using protocol " << protocol_name << std::endl;
        
    // find a communicator for this protocol.
    std::vector<NetCommunicator*>::iterator i;
    for (i = communicators.begin(); i != communicators.end(); i++) {
        if ((*i)->acceptsStreamIdentifier(protocol_name)) {
            communicator = *i;
            break;
        }
    }

    if (communicator) {
        return communicator->readStream (this, fin, buf);
    }
    else {
        vprDEBUG (vprDBG_ENV_MGR, 1) << "Connection: Couldn't find protocol handler for '" << protocol_name << "'\n" << vprDEBUG_FLUSH;
        // should we scan for </protocol> here?
        return false;
    }
}




};
