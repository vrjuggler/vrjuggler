/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


#include <vjConfig.h>

#include <VPR/Threads/vjBaseThread.h>
#include <VPR/Threads/vjThreadManager.h>

VJ_IMPLEMENT_DATA(int32_t) vjBaseThread::mNextThreadId = 0;

	// ---- Ouput operator ---- //
std::ostream& operator<<(std::ostream& out, vjBaseThread* threadPtr)
{
   if(threadPtr != NULL)
      threadPtr->outStream(out);
   else
      out << " TID: N/A  ";

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
