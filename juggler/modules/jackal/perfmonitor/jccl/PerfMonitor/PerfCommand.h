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



// commands used by connections...

#ifndef _JCCL_COMMAND_H_
#define _JCCL_COMMAND_H_

#include <jccl/jcclConfig.h>
#include <jccl/Performance/TimeStamp.h>

namespace jccl {

class TimedUpdate;
class ConfigChunkDB;
class ChunkDescDB;

class VJ_CLASS_API Command {
public:
    float next_fire_time; // milliseconds
    float refresh_time;      // millisecs

public:
    virtual void call (std::ostream& out) const = 0;

    void resetFireTime (TimeStamp& ts);

    int operator < (const Command& cmd2) const;
    
    virtual const std::string& getName () const = 0;
};



class VJ_CLASS_API CommandRefresh: public Command {
private:
    static const std::string command_name;

public:
    CommandRefresh();
    
    virtual void call (std::ostream& out) const;

    virtual const std::string& getName() const;
};



class VJ_CLASS_API CommandSendChunkDB: public Command {
private:
    ConfigChunkDB* db;
    bool all;
    static const std::string command_name;
    
public:
    CommandSendChunkDB (ConfigChunkDB* _db, bool _all = false);

    virtual void call (std::ostream& out) const;

    virtual const std::string& getName() const;
};



class VJ_CLASS_API CommandSendDescDB: public Command {
private:
    ChunkDescDB* db;
    bool all;
    static const std::string command_name;

public:
    CommandSendDescDB (ChunkDescDB* _db, bool _all = false);
    
    virtual void call (std::ostream& out) const;

    virtual const std::string& getName() const;
};



class CommandTimedUpdate: public Command {
public:
    TimedUpdate* timed_update;
    static const std::string command_name;
    
    CommandTimedUpdate (TimedUpdate* _tu, float _refresh_time);
    
    virtual void call (std::ostream& out) const;

    virtual const std::string& getName () const;
};

};

#endif
