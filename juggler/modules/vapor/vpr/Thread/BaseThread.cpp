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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadManager.h>
#include <iomanip>

namespace vpr
{

vpr::Int32 BaseThread::mNextThreadId = 0;
vpr::TSTable BaseThread::gTSTable;

/**
 * Ouputs the state of the object.
 */
std::ostream& BaseThread::outStream(std::ostream& out)
{
   out.setf(std::ios::right);
   out << std::setw(3) << mThreadId;
   out.unsetf(std::ios::right);
   return out;
}


   // ---- Ouput operator ---- //
std::ostream& operator<<(std::ostream& out, vpr::Thread* threadPtr)
{
   if (threadPtr != NULL)
   {
      threadPtr->outStream(out);
   }
   else
   {
      out << " TID: N/A  ";
   }

   return out;
}

void BaseThread::registerThread(bool succesfulCreation)
{
   if(succesfulCreation)   // Succeed
   {
      createThreadId();
      Thread* thread_ptr = dynamic_cast<Thread*>(this);
      vprASSERT(NULL != thread_ptr);
      ThreadManager::instance()->addThread(thread_ptr); // Add the thread to the table
   }
   else
   {
      mThreadId = -1;      // We have an invalid thread
   }
}

void BaseThread::unregisterThread()
{
   vpr::Thread* thread_ptr = dynamic_cast<vpr::Thread*>(this);
   vprASSERT(NULL != thread_ptr);
   vpr::ThreadManager::instance()->removeThread(thread_ptr);
}

}
