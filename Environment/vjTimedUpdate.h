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

#include <Threads/vjThread.h>
#include <Threads/vjThreadFunctor.h>
#include <Environment/vjConnect.h>



class vjTimedUpdate {

public:
    //: target: the connect that this timedupdate is gonna
    //+ report to.
    vjConnect*   target;
    float               refresh_time;
    vjThread*           control_thread;
    char*               name;



    vjTimedUpdate( vjConnect *_target, float _refresh_time) {
	target = _target;
	refresh_time = _refresh_time;
	control_thread = NULL;
    }



    ~vjTimedUpdate () {
	stopProcess();
    }



    void setTarget (vjConnect* _target) {
	// this maybe ought to stop/restart process if running...
	target = _target;

    }



    bool startProcess() {
	// Create a new thread to handle the control

	if (control_thread)
	    return 1;

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
	control_thread = NULL;
	return 1;
    }



    virtual void controlLoop(void* nullParam) {
    }

};




class vjTrackerTimedUpdate: public vjTimedUpdate {

public:

  std::vector<int> proxies;

    vjTrackerTimedUpdate (vjConnect* _target,
			  float _refresh_time): 
	vjTimedUpdate(_target, _refresh_time) {

	name = new char[9];
	strcpy (name, "Position");
    }



    virtual void controlLoop (void* nullParam);
};




class vjTimedUpdateFactory {
  // parses a command line & creates an appropriate vjTimedUpdate object
public:
  static vjTimedUpdate* generate (vjConnect* target, char* name) {
    if (!strcasecmp (name, "position"))
	return new vjTrackerTimedUpdate(target, 300);    
    else
      return NULL;
  }
};

#endif
