/*
 * vjTimedUpdate.h
 * class definition of vjTimedUpdate and its children,
 *   vjTrackerTimedUpdate
 *
 * 6/9/98
 *
 * author: Christopher Just
 */


// TimedUpdate is probably gonna be a parent class of a whole bunch of
// different timed updates, for input devices, performance monitoring,
// and so on.

#ifndef _VJ_TIMED_UPDATE_H_
#define _VJ_TIMED_UPDATE_H_

#include <config.h>
#include <Threads/vjThread.h>
#include <Threads/vjThreadFunctor.h>

class vjNetworkConnect;



class vjTimedUpdate {

public:
    vjNetworkConnect*   parent;
    float               refresh_time;
    vjThread*           control_thread;
    char*               name;

  vjTimedUpdate( vjNetworkConnect *_parent, float _refresh_time) {
    parent = _parent;
    refresh_time = _refresh_time;
    control_thread = NULL;
  }

  ~vjTimedUpdate () {
    stopProcess();
  }

  bool startProcess() {
    // Create a new thread to handle the control
    vjThreadMemberFunctor<vjTimedUpdate> *memberFunctor = 
      new vjThreadMemberFunctor<vjTimedUpdate>(this, &vjTimedUpdate::controlLoop, NULL);
    
    control_thread = new vjThread(memberFunctor, 0);
    cerr << "TimedUpdate::startProcess: Just started control loop.  "
	 << endl;
    
    return (control_thread != NULL);
    
  }

  bool stopProcess() {
    if (control_thread)
	control_thread->kill();
    return 1;
  }

  virtual void controlLoop(void* nullParam) {
  }

};




class vjTrackerTimedUpdate: public vjTimedUpdate {

public:
  //  char pname[9];

  vector<int> proxies;

  vjTrackerTimedUpdate (vjNetworkConnect* _parent,
		      float _refresh_time): vjTimedUpdate(_parent, 
							_refresh_time) {
    name = new char[9];
    strcpy (name, "Position");
  }

  virtual void controlLoop (void* nullParam);
};




class vjTimedUpdateFactory {
  // parses a command line & creates an appropriate vjTimedUpdate object
public:
  static vjTimedUpdate* generate (vjNetworkConnect* parent, char* name) {
    if (!strcasecmp (name, "position"))
      return new vjTrackerTimedUpdate(parent, 300);    
    else
      return NULL;
  }
};

#endif
