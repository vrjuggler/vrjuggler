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



#include <Environment/vjCommand.h>
#include <Environment/vjTimedUpdate.h>
#include <Config/vjConfigChunkDB.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjConfigIO.h>

// generic vjCommand

void vjCommand::resetFireTime (vjTimeStamp& ts) {
    next_fire_time = ts.usecs()/1000 + refresh_time;
}


int vjCommand::operator < (const vjCommand& cmd2) const {
    // used in priority queue
    // true if self should be called _after_ cmd2
    return (next_fire_time < cmd2.next_fire_time);
}
    



// vjCommandRefresh

/*static*/ const std::string vjCommandRefresh::command_name ("Refresh Command");


vjCommandRefresh::vjCommandRefresh() {
    next_fire_time = refresh_time = 0.0;
}

    
/*virtual*/ void vjCommandRefresh::call (std::ostream& out) const {
    out << "<protocol handler=\"xml_config\">\n"
        "<refresh_all/>\n"
        "</protocol>\n" << std::flush;
}


/*virtual*/ const std::string& vjCommandRefresh::getName () const {
    return command_name;
}



// vjCommandSendChunkDB

/*static*/ const std::string vjCommandSendChunkDB::command_name ("Send ChunkDB Command");


vjCommandSendChunkDB::vjCommandSendChunkDB (vjConfigChunkDB* _db, bool _all) {
    db = _db;
    all = _all;
}


/*virtual*/ void vjCommandSendChunkDB::call (std::ostream& out) const {
    out << "<protocol handler=\"xml_config\">\n";
    if (all)
        out << "<apply_chunks all=\"true\">\n";
    else
        out << "<apply_chunks>\n";
    vjConfigIO::instance()->writeConfigChunkDB (out, *db, "xml_config");
    out << "</apply_chunks>\n</protocol>\n" << std::flush;
}


/*virtual*/ const std::string& vjCommandSendChunkDB::getName () const {
    return command_name;
}



//vjCommandSendDescDB

/*static*/ const std::string vjCommandSendDescDB::command_name ("Send DescDB Command");


vjCommandSendDescDB::vjCommandSendDescDB (vjChunkDescDB* _db, bool _all) {
    db = _db;
    all = _all;
}

    
/*virtual*/ void vjCommandSendDescDB::call (std::ostream& out) const {

    out << "<protocol handler=\"xml_config\">\n";
    if (all)
        out << "<apply_descs all=\"true\">\n";
    else
        out << "<apply_descs>\n";
    vjConfigIO::instance()->writeChunkDescDB (out, *db, "xml_config");
    out << "</apply_descs>\n</protocol>\n" << std::flush;
}


/*virtual*/ const std::string& vjCommandSendDescDB::getName () const {
    return command_name;
}



// vjCommandTimedUpdate

/*static*/ const std::string vjCommandTimedUpdate::command_name ("Timed Update Command");


vjCommandTimedUpdate::vjCommandTimedUpdate (vjTimedUpdate* _tu, float _refresh_time) {
    timed_update = _tu;
    refresh_time = _refresh_time;
    next_fire_time = 0;
}

    
/*virtual*/ void vjCommandTimedUpdate::call (std::ostream& out) const {
    out << "<protocol handler=\"" << timed_update->getProtocolHandlerName()
        << "\">\n";
    timed_update->write (out);
    out << "</protocol>\n" << std::flush;
}


/*virtual*/ const std::string& vjCommandTimedUpdate::getName () const {
    return command_name;
}

