/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */



#include <Environment/vjCommand.h>
#include <Environment/vjTimedUpdate.h>
#include <Config/vjConfigChunkDB.h>
#include <Config/vjChunkDescDB.h>

void vjCommand::resetFireTime (vjTimeStamp& ts) {
    next_fire_time = ts.usecs()/1000 + refresh_time;
}

int vjCommand::operator < (const vjCommand& cmd2) {
    // used in priority queue
    // true if self should be called _after_ cmd2
    return (next_fire_time < cmd2.next_fire_time);
}
    
std::string vjCommand::getName () {
    return (std::string)"generic vjCommand";
}



// vjCommandRefresh
vjCommandRefresh::vjCommandRefresh() {
    next_fire_time = refresh_time = 0.0;
}
    
void vjCommandRefresh::call (ostream& out) {
    out << "Refresh all" << endl;
}



// vjCommandSendChunkDB

vjCommandSendChunkDB::vjCommandSendChunkDB (vjConfigChunkDB* _db, bool _all) {
    db = _db;
    all = _all;
}

void vjCommandSendChunkDB::call (ostream& out) {
    if (all)
	out << "chunks all\n";
    else
	out << "chunks\n";
    out << *db << flush;
}

std::string vjCommandSendChunkDB::getName () {
    return (std::string)"Send ChunkDB command";
}


//vjCommandSendDescDB

vjCommandSendDescDB::vjCommandSendDescDB (vjChunkDescDB* _db, bool _all) {
    db = _db;
    all = _all;
}
    
void vjCommandSendDescDB::call (ostream& out) {
    if (all)
	out << "descriptions all\n";
    else
	out << "descriptions\n";
    out << *db << flush;
}


// vjCommandTimedUpdate

vjCommandTimedUpdate::vjCommandTimedUpdate (vjTimedUpdate* _tu, float _refresh_time) {
    timed_update = _tu;
    refresh_time = _refresh_time;
    next_fire_time = 0;
}
    
void vjCommandTimedUpdate::call (ostream& out) {
    timed_update->write (out);
}

std::string vjCommandTimedUpdate::getName () {
    return (std::string)"Timed Update Command; tu obj is " + timed_update->getName();
}

