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
};



class vjCommandTimedUpdate: public vjCommand {
public:
    vjTimedUpdate* timed_update;
    
    vjCommandTimedUpdate (vjTimedUpdate* _tu, float _refresh_time);
    
    virtual void call (ostream& out);

    virtual std::string getName ();
};


#endif
