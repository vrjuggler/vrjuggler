
// commands used by connections...

#ifndef _VJ_COMMAND_H_
#define _VJ_COMMAND_H_

#include <vjConfig.h>
#include <Performance/vjTimeStamp.h>

class vjCommand {
public:
    virtual void call (ostream& out) = 0;
    void resetFireTime (vjTimeStamp& ts) {
	next_fire_time = ts.usecs()/1000 + refresh_time;
    }

    int operator < (const vjCommand& cmd2) {
	// used in priority queue
	// true if self should be called _after_ cmd2
	return (next_fire_time < cmd2.next_fire_time);
    }
    
    virtual std::string getName () {
	return (std::string)"generic vjCommand";
    }

    float next_fire_time; // milliseconds
    float refresh_time;      // millisecs
};

class vjCommandRefresh: public vjCommand {
public:
    vjCommandRefresh() {
	next_fire_time = refresh_time = 0.0;
    }
    
    virtual void call (ostream& out) {
	out << "Refresh all" << endl;
    }
};



class vjCommandSendChunkDB: public vjCommand {
private:
    vjConfigChunkDB* db;
    bool all;
    
public:
    vjCommandSendChunkDB (vjConfigChunkDB* _db, bool _all = false) {
	db = _db;
	all = _all;
    }

    virtual void call (ostream& out) {
	if (all)
	    out << "chunks all\n";
	else
	    out << "chunks\n";
	out << *db << flush;
    }
};



class vjCommandSendDescDB: public vjCommand {
private:
    vjChunkDescDB* db;
    bool all;

public:
    vjCommandSendDescDB (vjChunkDescDB* _db, bool _all = false) {
	db = _db;
	all = _all;
    }
    
    virtual void call (ostream& out) {
	if (all)
	    out << "descriptions all\n";
	else
	    out << "descriptions\n";
	out << *db << flush;
    }
};



class vjCommandTimedUpdate: public vjCommand {
public:
    vjTimedUpdate* timed_update;
    
    vjCommandTimedUpdate (vjTimedUpdate* _tu, float _refresh_time) {
	timed_update = _tu;
	refresh_time = _refresh_time;
	next_fire_time = 0;
    }
    
    virtual void call (ostream& out) {
	timed_update->write (out);
    }

    virtual std::string getName () {
	return (std::string)"Timed Update Command; tu obj is " + timed_update->getName();
    }
};


#endif
