/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */



// commands used by connections...

#ifndef _VJ_COMMAND_H_
#define _VJ_COMMAND_H_

#include <vjConfig.h>
#include <Performance/vjTimeStamp.h>

class vjTimedUpdate;
class vjConfigChunkDB;
class vjChunkDescDB;

class vjCommand {
public:
    virtual void call (ostream& out) = 0;
    void resetFireTime (vjTimeStamp& ts);

    int operator < (const vjCommand& cmd2);
    
    virtual std::string getName ();

    float next_fire_time; // milliseconds
    float refresh_time;      // millisecs
};

class vjCommandRefresh: public vjCommand {
public:
    vjCommandRefresh();
    
    virtual void call (ostream& out);
};



class vjCommandSendChunkDB: public vjCommand {
private:
    vjConfigChunkDB* db;
    bool all;
    
public:
    vjCommandSendChunkDB (vjConfigChunkDB* _db, bool _all = false);

    virtual void call (ostream& out);

    virtual std::string getName();
};



class vjCommandSendDescDB: public vjCommand {
private:
    vjChunkDescDB* db;
    bool all;

public:
    vjCommandSendDescDB (vjChunkDescDB* _db, bool _all = false);
    
    virtual void call (ostream& out);

    virtual std::string getName();
};



class vjCommandTimedUpdate: public vjCommand {
public:
    vjTimedUpdate* timed_update;
    
    vjCommandTimedUpdate (vjTimedUpdate* _tu, float _refresh_time);
    
    virtual void call (ostream& out);

    virtual std::string getName ();
};


#endif
