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


#ifndef _VJ_GUARDED_QUEUE_
#define _VJ_GUARDED_QUEUE_

#include <vjConfig.h>

#include <queue>
#include <Sync/vjMutex.h>
#include <Sync/vjGuard.h>

//: A guarded queue
// Guards an STL queue and implements the basic interface
// All the functions pass directly through to the corresponding STL
// queue function but they are guarded so that only a single
// thread can interact with the queue at once.
template <class value_type>
class vjGuardedQueue
{
public:
   vjGuardQueue()
   {;}

   ~vjGuardedQueue()
   {;}

   bool empty()
   {
   vjGuard<vjMutex> guard(mMutexGuard);
      return mQ.empty();
   }

   value_type& front()
   {
   vjGuard<vjMutex> guard(mMutexGuard);
      return mQ.front();
   }

   value_type& back()
   {
   vjGuard<vjMutex> guard(mMutexGuard);
      return mQ.back();
   }

   void push(const value_type& val)
   {
    vjGuard<vjMutex> guard(mMutexGuard);
      mQ.push(val);
   }

   void pop()
   {
   vjGuard<vjMutex> guard(mMutexGuard);
      mQ.pop();
   }

private:
   vjMutex           mMutexGuard;      // The mutex to guard the queue
   std::queue<value_type> mQ;
};


#endif
