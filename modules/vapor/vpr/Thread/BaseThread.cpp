/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <iomanip>

#include <vpr/Sync/Guard.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadManager.h>
#include <vpr/Thread/BaseThread.h>


namespace vpr
{

BaseThread::state_signal_t BaseThread::sStartSignal;
BaseThread::state_signal_t BaseThread::sExitSignal;
vpr::Mutex sStartSignalLock;
vpr::Mutex sExitSignalLock;

vpr::Int32 BaseThread::mNextThreadId = 0;
vpr::TSTable BaseThread::sTSTable;

boost::signals::connection BaseThread::
addThreadStartCallback(BaseThread::state_signal_t::slot_function_type slot)
{
   vpr::Guard<vpr::Mutex> g(sStartSignalLock);
   return sStartSignal.connect(slot);
}

boost::signals::connection BaseThread::
addThreadExitCallback(BaseThread::state_signal_t::slot_function_type slot)
{
   vpr::Guard<vpr::Mutex> g(sExitSignalLock);
   return sExitSignal.connect(slot);
}

BaseThread::BaseThread()
   : mThreadId(-1)
{
   ;
}

BaseThread::~BaseThread()
{
   /* Do nothing. */ ;
}

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

void BaseThread::registerThread(const bool succesfulCreation)
{
   if ( succesfulCreation )   // Succeed
   {
      createThreadId();
      // Add the thread to the table
      ThreadManager::instance()->addThread(static_cast<Thread*>(this));
   }
   else
   {
      mThreadId = -1;      // We have an invalid thread
   }
}

void BaseThread::unregisterThread()
{
   vpr::ThreadManager::instance()->removeThread(
      static_cast<vpr::Thread*>(this)
   );
}

//--------------------------------------------
// This is the actual function that is called.
//--------------------------------------------
#if defined(VPR_USE_PTHREADS)
void* vprThreadFunctorFunction(void* arg)
{
   // Wait until this thread has been registered with the thread
   // manager before continuing execution.
   ThreadManager::instance()->lock();
   ThreadManager::instance()->unlock();

   vpr::thread_func_t& func = *((vpr::thread_func_t*) arg);
   vprASSERT(! func.empty());
   func();

   return (void*) NULL;
}
#elif defined(VPR_USE_WINTHREADS)
unsigned __stdcall vprThreadFunctorFunction(void* arg)
{
   vpr::thread_func_t& func = *((vpr::thread_func_t*) arg);
   vprASSERT(! func.empty());
   func();
   return 0;
}
#else
void vprThreadFunctorFunction(void* arg)
{
   vpr::thread_func_t& func = *((vpr::thread_func_t*) arg);
   vprASSERT(! func.empty());
   func();
}
#endif

}
