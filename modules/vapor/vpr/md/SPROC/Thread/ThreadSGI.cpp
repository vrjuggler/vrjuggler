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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadManager.h>
#include <vpr/Util/Assert.h>
#include <vpr/md/SPROC/Thread/ThreadSGI.h>


namespace vpr
{

// Non-spawning constructor.  This will not start a thread.
ThreadSGI::ThreadSGI(BaseThread::VPRThreadPriority priority,
                     BaseThread::VPRThreadScope scope,
                     BaseThread::VPRThreadState state, size_t stackSize)
   : mRunning(false)
   , mCaughtException(false)
   , mException("No exception caught")
{
}

/**
 * Spawning constructor.  This will actually start a new thread that will
 * execute the specified function.
 */
ThreadSGI::ThreadSGI(const vpr::thread_func_t& func,
                     BaseThread::VPRThreadPriority priority,
                     BaseThread::VPRThreadScope scope,
                     BaseThread::VPRThreadState state, size_t stackSize)
   : mRunning(false)
   , mCaughtException(false)
   , mException("No exception caught")
{
   // Create the thread functor to start.
   setFunctor(func);
   start();
}

ThreadSGI::~ThreadSGI()
{
   ThreadManager::instance()->lock();
   {
      unregisterThread();
   }
   ThreadManager::instance()->unlock();
}

void ThreadSGI::setFunctor(const vpr::thread_func_t& functor)
{
   vprASSERT(! mRunning && "Thread already running");
   vprASSERT(! functorPtr.empty());

   mUserThreadFunctor = functor;
}

vpr::ReturnStatus ThreadSGI::start()
{
   vpr::ReturnStatus status;

   if ( mRunning )
   {
      vprASSERT(false && "Thread already running");
      status.setCode(vpr::ReturnStatus::Fail);
   }
   else if ( mUserThreadFunctor.empty() )
   {
      vprASSERT(false && "No functor set");
      status.setCode(vpr::ReturnStatus::Fail);
   }
   else
   {
      // Spawn the thread.
      mStartFunctor = boost::bind(&ThreadSGI::startThread, this);
      status = spawn();

      if ( status.success() )
      {
         mRunning = true;

         ThreadManager::instance()->lock();
         {
            registerThread(true);
         }
         ThreadManager::instance()->unlock();
      }
      else
      {
         ThreadManager::instance()->lock();
         {
            registerThread(false);     // Failed to create
         }
         ThreadManager::instance()->unlock();
      }
   }

   return status;
}

vpr::ReturnStatus ThreadSGI::spawn()
{
   vpr::ReturnStatus status;
   mThreadPID = sproc(vprThreadFunctorFunction, PR_SADDR | PR_SFDS,
                      &mStartFunctor);

   if ( mThreadPID == -1 )
   {
      status.setCode(vpr::ReturnStatus::Fail);
   }

   return status;
}

/**
 * Called by the spawn routine to start the user thread function.
 */
void ThreadSGI::startThread()
{
   // WE are a new thread... yeah!!!!
   // TELL EVERYONE THAT WE LIVE!!!!
   ThreadManager::instance()->lock();      // Lock manager
   {
      setLocalThreadPtr(this);               // Store the pointer to me
   }
   ThreadManager::instance()->unlock();

   // Tell this thread to die when its parent dies
   prctl(PR_SETEXITSIG, 0);
   prctl(PR_TERMCHILD);

   try
   {
      // --- CALL USER FUNCTOR --- //
      mUserThreadFunctor();
   }
   catch (vpr::Exception& ex)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << clrOutNORM(clrYELLOW, "WARNING:")
         << " Caught exception: " << ex.getExtendedDescription()
         << vprDEBUG_FLUSH;

      mCaughtException = true;
      mException.setException(ex);
   }
   catch (std::exception& ex)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << clrOutNORM(clrYELLOW, "WARNING:")
         << " Caught exception: " << ex.what()
         << vprDEBUG_FLUSH;

      mCaughtException = true;
      mException.setException(ex);
   }
}

/**
 * Makes the calling thread wait for the termination of the specified
 * thread.
 */
int ThreadSGI::join(void** arg)  throw (UncaughtThreadException)
{
   int status, retval;
   pid_t pid;

   do
   {
      pid = ::waitpid(mThreadPID, &status, 0);
   } while ( WIFSTOPPED(status) != 0 );

   if ( pid > -1 )
   {
      vprASSERT(pid == mThreadPID);

      if ( WIFEXITED(status) != 0 && arg != NULL )
      {
         **((int**) arg) = WEXITSTATUS(status);
      }
      else if ( WIFSIGNALED(status) != 0 && arg != NULL )
      {
         **((int**) arg) = WTERMSIG(status);
      }

      retval = 0;
   }
   else
   {
      retval = -1;
   }

   if (mCaughtException)
   {
      throw mException;
   }

   return retval;
}

std::ostream& ThreadSGI::outStream(std::ostream& out)
{
   out.setf(std::ios::right);
   out << std::setw(7) << std::setfill('0') << mThreadPID << "/";
   out.unsetf(std::ios::right);
   BaseThread::outStream(out);
   out << std::setfill(' ');
   return out;
}

} // End of vpr namespace
