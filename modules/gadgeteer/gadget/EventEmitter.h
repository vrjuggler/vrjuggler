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

#ifndef _GADGET_EVENT_EMITTER_H_
#define _GADGET_EVENT_EMITTER_H_

#include <gadget/gadgetConfig.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/version.hpp>

#if BOOST_VERSION >= 103600
#  include <boost/unordered_map.hpp>
#else
#  include <map>
#endif

#include <vpr/vprDomain.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Interval.h>

#include <jccl/RTRC/ConfigElementHandler.h>

#include <gadget/Event/EventGeneratorPtr.h>
#include <gadget/EventEmitterPtr.h>


namespace gadget
{

class AbstractEventInterface;

/** \class EventEmitter EventEmitter.h gadget/EventEmitter.h
 *
 * This class is responsible for emitting non-immediate events. There are
 * two such event classifications: syncronous and periodic. Synchronous events
 * are emitted when gadget::InputManager::updateAllDevices() is invoked.
 * Periodic events are emitted from a thread controlled by an instance of
 * this class at a set rate.
 *
 * @note This class was renamed from \c gadget::InputHandler in 2.1.24.
 *
 * @since 2.1.2
 */
class GADGET_API EventEmitter
   : public jccl::ConfigElementHandler
   , public boost::enable_shared_from_this<EventEmitter>
   , private boost::noncopyable
{
private:
   EventEmitter();

public:
   static EventEmitterPtr create();

   ~EventEmitter();

   /** @name jccl::ConfigElementHandler Interface Implementation */
   //@{
   /**
    * @since 2.1.25
    */
   bool configCanHandle(jccl::ConfigElementPtr element);

   /**
    * @since 2.1.25
    */
   bool configAdd(jccl::ConfigElementPtr element);

   /**
    * @since 2.1.25
    */
   bool configRemove(jccl::ConfigElementPtr element);
   //@}

   /**
    * @since 2.1.25
    */
   void shutdown();

   /** @name Interface Registration */
   //@{
   /**
    * Registers the given event interface so that its even tgenerator will
    * emit events while the thread managed by this object executes.
    *
    * @pre \p iface will return a valid event generator when asked to do so.
    *
    * @param iface The abstract event interface to register.
    *
    * @see AbstractEventInterface::getEventGenerator()
    */
   void registerPeriodicInterface(AbstractEventInterface* iface);

   void unregisterPeriodicInterface(AbstractEventInterface* iface);

   /**
    * Registers the given event interface so that its even tgenerator will
    * emit events once per frame when the Input Manager invokes the sync()
    * method.
    *
    * @pre \p iface will return a valid event generator when asked to do so.
    *
    * @param iface The abstract event interface to register.
    *
    * @see AbstractEventInterface::getEventGenerator()
    * @see InputManager::updateAllDevices()
    * @see sync()
    */
   void registerSynchronizedInterface(AbstractEventInterface* iface);

   void unregisterSynchronizedInterface(AbstractEventInterface* iface);
   //@}

   /**
    * Causes all registered synchronized event generators to emit their
    * pending events.
    *
    * @since 2.1.3
    */
   void sync();

private:
   /**
    * Starts the thread for periodic event emission.
    *
    * @pre If \c mRunning is false, then \c mThread is NULL.
    * @post \c mThread is not NULL.
    *
    * @param interval The wait interval for the periodic event emission loop.
    *                 This must represent a number of milliseconds greater
    *                 than 0.
    */
   void start(const vpr::Interval& interval);

   /**
    * Stops the thread for periodic event emission.
    *
    * @pre If \c mRunning is true, then \c mThread is not NULL.
    * @post \c mRunning is false, and \c mThread is NULL.
    */
   void stop();

   /**
    * @since 2.1.22
    */
   void run();

   /**
    *
    */
   //@{
   bool          mRunning;
   vpr::Thread*  mThread;
   vpr::Uint32   mWaitInterval;
   //@}

#if BOOST_VERSION >= 103600
   typedef boost::unordered_map<AbstractEventInterface*, EventGeneratorPtr>
      iface_map_type;
#else
   typedef std::map<AbstractEventInterface*, EventGeneratorPtr> iface_map_type;
#endif

   iface_map_type mPeriodicIfaces;
   iface_map_type mSyncIfaces;

   vpr::Mutex mPeriodicIfacesLock;
   vpr::Mutex mSyncIfacesLock;
};

}


#endif /* _GADGET_EVENT_EMITTER_H_ */
