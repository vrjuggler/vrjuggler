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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

#include <vpr/vprConfig.h>

#include <vpr/Thread/BaseThread.h>
#include <vpr/Thread/ThreadManager.h>

vpr::Int32 vpr::BaseThread::mNextThreadId = 0;
vpr::TSTable vpr::BaseThread::gTSTable;


	// ---- Ouput operator ---- //
std::ostream& vpr::operator<<(std::ostream& out, vpr::BaseThread* threadPtr)
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
void vpr::BaseThread::registerThread(bool succesfulCreation)
{
   if(succesfulCreation)   // Success
   {
      create_thread_id();
      ThreadManager::instance()->addThread(this); // Add the thread to the table
   }
   else
   {
      mThreadId = -1;      // We have an invalid thread
   }
}
