/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
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


#include <vjConfig.h>
#include <Threads/vjThread.h>
#include <Threads/vjBaseThread.h>
#include <Threads/vjThreadManager.h>

int32_t vjBaseThread::mNextThreadId = 0;

	// ---- Ouput operator ---- //
ostream& operator<<(ostream& out, vjBaseThread* threadPtr)
{
   if(threadPtr != NULL)
      threadPtr->outStream(out);
   else
      out << "TID: N/A ";

   return out;
}

//: After the object has been created, call this routine to complete initialization.
// Done this way, because I need to call this based on stuff that happens
// in derived class's constructor... and that means what???
//! PRE: The thread manager should be lock()'ed before calling this function so that the
//+       addThread function can execute correctly
//! POST: Thread is setup correctly to run
//+       The thread has been registered with the system
//! ARGS: successfulCreation - Did the thread get created correctly
void vjBaseThread::registerThread(bool succesfulCreation)
{
   if(succesfulCreation)   // Success
   {
      create_thread_id();
      vjThreadManager::instance()->addThread(this);      // Add the thread to the table
   }
   else
   {
      mThreadId = -1;      // We have an invalid thread
   }
}
