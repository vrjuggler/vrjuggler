/*
 * vjTimedUpdate.h
 * class definition of vjTimedUpdate and its children,
 *   vjTrackerTimedUpdate
 *
 * 6/9/98
 *
 * author: Christopher Just
 */

#ifndef _VJ_TU_PERFBUFREADER_H_
#define _VJ_TU_PERFBUFREADER_H_

#include <vjConfig.h>
#include <Environment/vjTimedUpdate.h>
#include <Performance/vjPerfDataBuffer.h>
#include <vector.h>

//: TimedUpdate for reading from Performance Data buffers
//
// A vjTuPerfBufReader periodically samples one or more
// vjPerfDataBuffers, transmitting the data to its target.
// The target and the time between samples are controllable.
// Target is currently a vjNetworkConnect.  It would be
// nice if that were more generic...
class vjTuPerfBufReader: public vjTimedUpdate {

public:
    vector<vjPerfDataBuffer*>   buffers;

    vjTuPerfBufReader( vjConnect *_target, 
		       float _refresh_time );



    ~vjTuPerfBufReader ();



    void addBuffer (vjPerfDataBuffer *b);



    bool startProcess();
    


    bool stopProcess();



    void controlLoop(void* nullParam);

};



#endif
