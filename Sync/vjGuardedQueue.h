/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
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
   vjGuardedQueue()
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

   int size() const
   { return mQ.size(); }

private:
   vjMutex           mMutexGuard;      // The mutex to guard the queue
   std::queue<value_type> mQ;
};


#endif
