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
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_STREAM_LOCK_H_
#define _VJ_STREAM_LOCK_H_
//#pragma once

#include <iostream.h>
#include <Sync/vjMutex.h>

//: Class to lock a stream for multi-process output
// This class can be used to share a stream between two processes
// It is used by simply outputing in the stream a vjStreamLock constructed using a mutex <br>
// <br>
// EX: cout << vjStreamLock(mutex) << "Hello world" << flush << vjStreamUnLock(mutex);
class vjStreamLock
{
public:
   explicit vjStreamLock(vjMutex& mutex) : mMutex(mutex)
   {;}

   friend ostream& operator<<(ostream&, const vjStreamLock&);
	
private:
   vjMutex& mMutex;
};



//: Class to unlock a stream that has been previously locked
// This class can be used to share a stream between two processes
// It is used by simply outputing in the stream a vjStreamUnLock constructed using a mutex <br>
// <br>
// EX: cout << vjStreamLock(mutex) << "Hello world" << flush << vjStreamUnLock(mutex);
class vjStreamUnLock
{
public:
   explicit vjStreamUnLock(vjMutex& mutex) : mMutex(mutex)
   {;}

   friend ostream& operator<<(ostream&, const vjStreamUnLock&);
	
private:
   vjMutex& mMutex;
};


#endif
