#include <Threads/vjThread.h>
#include <Threads/vjBaseThread.h>
#include <Threads/vjThreadManager.h>

long vjBaseThread::mNextThreadId = 0;

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
// in derived class's constructor
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
      mThreadId = -1;
}
