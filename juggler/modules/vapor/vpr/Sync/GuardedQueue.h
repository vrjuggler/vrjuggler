/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_GUARDED_QUEUE_H_
#define _VPR_GUARDED_QUEUE_H_

#include <vpr/vprConfig.h>

#include <queue>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>


namespace vpr
{

/**
 * A guarded queue.
 * Guards an STL queue and implements the basic interface
 * All the functions pass directly through to the corresponding STL
 * queue function but they are guarded so that only a single
 * thread can interact with the queue at once.
 */
template <class value_type>
class GuardedQueue
{
public:
   GuardedQueue()
   {;}

   ~GuardedQueue()
   {;}

   bool empty()
   {
      Guard<Mutex> guard(mMutexGuard);
      return mQ.empty();
   }

   value_type& front()
   {
      Guard<Mutex> guard(mMutexGuard);
      return mQ.front();
   }

   value_type& back()
   {
      Guard<Mutex> guard(mMutexGuard);
      return mQ.back();
   }

   void push(const value_type& val)
   {
      Guard<Mutex> guard(mMutexGuard);
      mQ.push(val);
   }

   void pop()
   {
      Guard<Mutex> guard(mMutexGuard);
      mQ.pop();
   }

   int size() const
   {
      return mQ.size();
   }

private:
   Mutex           mMutexGuard;      /**< The mutex to guard the queue */
   std::queue<value_type> mQ;
};

} // End of vpr namespace


#endif
