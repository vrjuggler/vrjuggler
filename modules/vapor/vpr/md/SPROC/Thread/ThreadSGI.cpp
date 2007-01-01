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
#include <sstream>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <errno.h>
#include <boost/bind.hpp>

#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadManager.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/IllegalArgumentException.h>
#include <vpr/Util/ResourceException.h>
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
   vprASSERT(! functor.empty());

   mUserThreadFunctor = functor;
}

void ThreadSGI::start()
{
   if ( mRunning )
   {
      throw vpr::Exception("Thread already started", VPR_LOCATION);
   }
   else if ( mUserThreadFunctor.empty() )
   {
      throw vpr::IllegalArgumentException("No functor set", VPR_LOCATION);
   }
   else
   {
      // Spawn the thread.
      mStartFunctor = boost::bind(&ThreadSGI::startThread, this);

      try
      {
         spawn();
         mRunning = true;

         ThreadManager::instance()->lock();
         {
            registerThread(true);
         }
         ThreadManager::instance()->unlock();
      }
      catch (vpr::Exception& ex)
      {
         ThreadManager::instance()->lock();
         {
            registerThread(false);     // Failed to create
         }
         ThreadManager::instance()->unlock();

         // Rethrow the exception.
         throw;
      }
   }
}

void ThreadSGI::spawn()
{
   mThreadPID = sproc(vprThreadFunctorFunction, PR_SADDR | PR_SFDS,
                      &mStartFunctor);

   if ( mThreadPID == -1 )
   {
      std::ostringstream msg_stream;
      msg_stream << "Cannot create thread: " << strerror(errno);

      if ( ENOMEM == errno || EAGAIN == errno )
      {
         throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
      }
      else if ( EINVAL == errno )
      {
         throw vpr::IllegalArgumentException(msg_stream.str(), VPR_LOCATION);
      }
      else
      {
         throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
      }
   }
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
void ThreadSGI::join(void** status)
{
   pid_t pid;

   do
   {
      pid = ::waitpid(mThreadPID, &exit_status, 0);
   } while ( WIFSTOPPED(exit_status) != 0 );

   if ( pid > -1 )
   {
      vprASSERT(pid == mThreadPID);

      if ( WIFEXITED(exit_status) != 0 && arg != NULL )
      {
         **((int**) arg) = WEXITSTATUS(exit_status);
      }
      else if ( WIFSIGNALED(exit_status) != 0 && arg != NULL )
      {
         **((int**) arg) = WTERMSIG(exit_status);
      }
   }
   else
   {
      throw vpr::IllegalArgumentException("Cannot join an invalid thread",
                                          VPR_LOCATION);
   }

   if ( mCaughtException )
   {
      throw mException;
   }
}

BaseThread::VPRThreadPriority ThreadSGI::getPrio()
{
   int prio = getpriority(PRIO_PROCESS, mThreadPID);

   if ( prio == -1 )
   {
      switch ( errno )
      {
         case ESRCH:
            throw vpr::IllegalArgumentException(
               "Cannot query priority for invalid thread", VPR_LOCATION
            );
            break;
      }
   }

   return unixThreadPriorityToVPR(prio);
}

void ThreadSGI::setPrio(const VPRThreadPriority prio)
{
   const int result = setpriority(PRIO_PROCESS, mThreadPID,
                                  vprThreadPriorityToUNIX(prio));

   if ( result == -1 )
   {
      switch ( errno )
      {
         case ESRCH:
            throw vpr::IllegalArgumentException(
               "Cannot set priority for invalid thread", VPR_LOCATION
            );
            break;
         case EPERM:
            {
               std::ostringstream msg_stream;
               msg_stream << "Permission denied when setting thread priority: "
                          << strerror(errno);
               throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
            }
            break;
         case EACCESS:
            {
               std::ostringstream msg_stream;
               msg_stream << "Only super-user can lower a process priority: "
                          << strerror(errno);
               throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
            }
            break;
      }
   }

   vprASSERT(result == 0);
}

void ThreadSGI::kill(const int signum)
{
   const int result = ::kill(mThreadPID, signum);

   if ( result == -1 )
   {
      switch ( errno )
      {
         case EINVAL:
            {
               std::ostringstream msg_stream;
               msg_stream << "Invalid signal number " << signum;
               throw vpr::IllegalArgumentException(msg_stream.str(),
                                                   VPR_LOCATION);
            }
            break;
         case ESRCH:
            {
               std::ostringstream msg_stream;
               msg_stream << "Cannot kill an invallid thread: "
                          << strerror(errno);
               throw vpr::IllegalArgumentException(msg_stream.str(),
                                                   VPR_LOCATION);
            }
            break;
         case EPERM:
            {
               std::ostringstream msg_stream;
               msg_stream << "Permission denied when sending signal to thread: "
                          << strerror(errno);
               throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
            }
            break;
      }
   }

   vprASSERT(result == 0);
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

int ThreadSGI::vprThreadPriorityToUNIX(const VPRThreadPriority priority)
{
   int unix_prio(0);

   switch ( priority )
   {
      case VPR_PRIORITY_LOW:
         unix_prio = 20;
         break;
      case VPR_PRIORITY_NORMAL:
         unix_prio = 0;
         break;
      case VPR_PRIORITY_HIGH:
         unix_prio = -10;
         break;
      case VPR_PRIORITY_URGENT:
         unix_prio = -20;
         break;
   }

   return unix_prio;
}

BaseThread::VPRThreadPriority ThreadSGI::
unixThreadPriorityToVPR(const int priority)
{
   VPRThreadPriority vpr_prio(VPR_PRIORITY_NORMAL);

   // XXX: This are sort of magic numbers.
   if ( priority >= 10 )
   {
      vpr_prio = VPR_PRIORITY_LOW;
   }
   else if ( priority < -5 && priority > -15 )
   {
      vpr_prio = VPR_PRIORITY_HIGH;
   }
   else if ( priority <= -15 )
   {
      vpr_prio = VPR_PRIORITY_URGENT;
   }

   return vpr_prio;
}

} // End of vpr namespace
