#ifndef _VJGuard_h_
#define _VJGuard_h_
//----------------------------------------------
// vjGuard
//
// Purpose:
//    Guard wrapper
//
//
// Author:
//	Allen Bierbaum
//
// Date: 1-31-97
//-----------------------------------------------

#include <vjConfig.h>
#include <Sync/vjSemaphore.h>
#include <Sync/vjMutex.h>

//: Guard wrapper.
//!PUBLIC_API:
template <class LOCK>
class vjGuard
{
public:
   //: Acquire the lock implicitly.
   // If block = 1 then use a blocking acquire
   vjGuard(LOCK &lock, int block = 1)
   : theLock(&lock)
   {
      lockStatus = block ? acquire() : tryAcquire();
   }

   //: Release the lock
   ~vjGuard()
   {
      if (lockStatus >= 0)
      {   // != -1
         theLock->release();
      }
   }

   //! RETURNS: 1 - Locked
   //! RETURNS: 0 - Not locked
   int locked() {
      return (lockStatus >= 0);  // != -1
   }

   //: Acquire the lock
   int acquire()
   {
      return theLock->acquire();
   }

   //: Try to acquire lock
   int tryAcquire()
   {
      return theLock->tryAcquire();
   }

   //: Explicity release the lock
   int release()
   {
      return theLock->release();
   }


private:
   LOCK* theLock;	//: The lock that we are using
   int   lockStatus;	//: Are we locked or not
};

#endif
