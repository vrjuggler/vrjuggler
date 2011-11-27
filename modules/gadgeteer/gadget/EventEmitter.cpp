/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>

#include <boost/bind.hpp>

#include <vpr/Util/Assert.h>
#include <vpr/Sync/Guard.h>
#include <vpr/System.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Util/Debug.h>

#include <jccl/Config/ConfigElement.h>

#include <gadget/Event/AbstractEventInterface.h>
#include <gadget/Event/EventGenerator.h>
#include <gadget/Util/Debug.h>
#include <gadget/EventEmitter.h>


namespace gadget
{

EventEmitter::EventEmitter()
   : mRunning(false)
   , mThread(NULL)
   , mWaitInterval(0)
{
   /* Do nothing. */ ;
}

EventEmitter::~EventEmitter()
{
   if (mRunning)
   {
      stop();
   }
}

EventEmitterPtr EventEmitter::create()
{
   return EventEmitterPtr(new EventEmitter());
}

bool EventEmitter::configCanHandle(jccl::ConfigElementPtr element)
{
   return "event_emitter" == element->getID();
}

bool EventEmitter::configAdd(jccl::ConfigElementPtr element)
{
   const int wait_interval(
      element->getProperty<int>("periodic_emission_interval")
   );

   if (wait_interval > 0)
   {
      start(vpr::Interval(wait_interval, vpr::Interval::Msec));
   }

   return true;
}

bool EventEmitter::configRemove(jccl::ConfigElementPtr)
{
   stop();
   return true;
}

void EventEmitter::shutdown()
{
   stop();
}

void EventEmitter::registerPeriodicInterface(AbstractEventInterface* iface)
{
   vpr::Guard<vpr::Mutex> guard(mPeriodicIfacesLock);
   vprASSERT(iface->getEventGenerator().get() != NULL);
   mPeriodicIfaces[iface] = iface->getEventGenerator();
}

void EventEmitter::unregisterPeriodicInterface(AbstractEventInterface* iface)
{
   vpr::Guard<vpr::Mutex> guard(mPeriodicIfacesLock);
   mPeriodicIfaces.erase(iface);
}

void EventEmitter::registerSynchronizedInterface(AbstractEventInterface* iface)
{
   vpr::Guard<vpr::Mutex> guard(mSyncIfacesLock);
   vprASSERT(iface->getEventGenerator().get() != NULL);
   mSyncIfaces[iface] = iface->getEventGenerator();
}

void
EventEmitter::unregisterSynchronizedInterface(AbstractEventInterface* iface)
{
   vpr::Guard<vpr::Mutex> guard(mSyncIfacesLock);
   mSyncIfaces.erase(iface);
}

void EventEmitter::sync()
{
   vpr::Guard<vpr::Mutex> guard(mSyncIfacesLock);

   typedef iface_map_type::iterator iter_type;
   for (iter_type i = mSyncIfaces.begin(); i != mSyncIfaces.end(); ++i)
   {
      (*i).second->emitEvents();
   }
}

void EventEmitter::start(const vpr::Interval& interval)
{
   vprASSERT(interval.msec() > 0);

   if (mRunning)
   {
      stop();
   }

   vprASSERT(NULL == mThread && "Cannot start thread when alredy running!");

   mWaitInterval = static_cast<vpr::Uint32>(interval.msec());
   mThread = new vpr::Thread(boost::bind(&EventEmitter::run, this));
}

void EventEmitter::stop()
{
   if (mRunning)
   {
      vprASSERT(NULL != mThread && "Cannot stop thread when not running!");

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "Waiting for input handler periorid event emission thread "
         << "to stop..." << std::endl << vprDEBUG_FLUSH;

      mRunning = false;
      mThread->join();

      delete mThread;
      mThread = NULL;
   }
}

void EventEmitter::run()
{
   vprASSERT(mWaitInterval > 0);

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "Event emitter periorid event emission thread starting; "
      << "wait interval is " << mWaitInterval << " milliseconds." << std::endl
      << vprDEBUG_FLUSH;

   mRunning = true;

   while (mRunning)
   {
      {
         vpr::Guard<vpr::Mutex> guard(mPeriodicIfacesLock);

         typedef iface_map_type::iterator iter_type;
         for (iter_type i = mPeriodicIfaces.begin();
              i != mPeriodicIfaces.end();
              ++i)
         {
            (*i).second->emitEvents();
         }
      }

      vpr::System::msleep(mWaitInterval);
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "Event emitter periorid event emission thread stopped." << std::endl
      << vprDEBUG_FLUSH;
}

}
