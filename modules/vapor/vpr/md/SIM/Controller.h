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

#ifndef _VPR_SIM_CONTROLLER_H_
#define _VPR_SIM_CONTROLLER_H_

#include <vpr/vprConfig.h>

#include <queue>
#include <utility>
#include <map>
#include <vpr/vprTypes.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/TSObjectProxy.h>

#include <vpr/md/SIM/Clock.h>
#include <vpr/md/SIM/SocketManager.h>
#include <vpr/md/SIM/Network/NetworkGraph.h>


namespace vpr
{

class Interval;

namespace sim
{

/**
 * Socket simulation controller.  This is used to step through a simulation
 * being controlled by the Sim Socket Manager.  This class is a thread-specific
 * singleton so that each running thread can have its own unique instance.  It
 * can also be used as a traditional global singleton.  To use it as a
 * thread-specific singleton, call setInstance() as the first step of a newly
 * spawned thread.  This is crucial to its functionality.
 */
class VPR_CLASS_API Controller
{
public:
   /**
    * Initializes the socket simulation.
    *
    * @post An instance of the Sim Socket Manager is retrieved, and the
    *       simulation state is set to not started.
    */
   Controller(void);

   /**
    * Resets the Sim Socket Manager's clock to 0.
    *
    * @post The Sim Socket Manager's clock is reset to 0 so that the same
    *       instance may be used by another simulation.
    */
   ~Controller (void)
   {
      /* Do nothing. */ ;
   }

   static void setInstance (Controller* c)
   {
      mInstance->setObject(c);
   }

   static Controller* instance (void)
   {
      // WARNING! race condition possibility, creation of static vars
      // are not thread safe.  This is only an issue when creating
      // your first thread, since it uses a singleton thread manager,
      // the two threads might both try to call instance at the same time
      // which then the creation of the following mutex would not be certain.
      static vpr::Mutex singleton_lock;

      if ( mInstance->getObject() == NULL )
      {
         vpr::Guard<vpr::Mutex> guard(singleton_lock);

         if ( mPrimordialInstance == NULL )
         {
            mPrimordialInstance = new Controller;
         }

         if ( mInstance->getObject() == NULL )
         {
            mInstance->setObject(mPrimordialInstance);
         }

         vprASSERT(mInstance->getObject() != NULL && "No instance defined");
      }

      return mInstance->getObject();
   }

   vpr::ReturnStatus constructNetwork(const std::string& graph_file);

   /**
    * Prepares the simulation for execution.  This initializes the simulation
    * clock to 0.
    *
    * @pre  The simulation has not already been started.
    * @post The Sim Socket Manager's simulation clock is reset to 0 so that the
    *       simulation may begin.
    */
   void start (void)
   {
      vprASSERT((mGraph.isValid() && ! m_started) && "Simulation already running!");
      m_started = true;
   }

   /**
    * Executes a step in the simulation.  If the simulation is not already
    * running, it is started.
    *
    * @post At least one step is taken in the socket simulation.
    *
    * @param max_interval The maximum step to take.  This is used to jump
    *                     over intervals when it is known that no activity
    *                     will occur.  This is an optional, and it defaults
    *                     to 1 (take only a single step).
    */
/*
   void step (vpr::Uint32 max_interval = 1)
   {
      if ( ! m_started )
      {
         start();
      }

      vpr::sim::Controller::instance()->getSocketManager().step(max_interval);
   }
*/

   /**
    * Queries the running state of this socket simulation.  The simulation is
    * considered running if it has been started and if the Sim Socket
    * Manager still has active sockets registered.
    *
    * @post The running state of this simulation is returned to the caller.
    */
   bool isRunning (void)
   {
      return m_started && mSocketManager.hasActiveSockets();
   }

   /**
    * Adds an event scheduled to occur at the given time to the queue of
    * upcoming events.
    */
   void addEvent(const vpr::Interval& event_time,
                 const NetworkGraph::net_edge_t edge);

   void processNextEvent(void);

   const vpr::sim::Clock& getClock (void) const
   {
      return mClock;
   }

   vpr::sim::SocketManager& getSocketManager (void)
   {
      return mSocketManager;
   }

   vpr::sim::NetworkGraph& getNetworkGraph (void)
   {
      return mGraph;
   }

private:
   void moveMessage(vpr::sim::MessagePtr, const vpr::Interval& cur_time);

   class ControllerTS
   {
   public:
      ControllerTS (void) : mObj(NULL)
      {
         /* Do nothing. */ ;
      }

      Controller* getObject (void) const
      {
         return mObj;
      }

      void setObject (Controller* c)
      {
         mObj = c;
      }

   private:
      Controller* mObj;
   };

   static Controller* mPrimordialInstance;
   static vpr::TSObjectProxy<ControllerTS> mInstance;

   bool m_started; /**< Flag telling the running state of the simulation */

   vpr::sim::Clock         mClock;
   vpr::sim::SocketManager mSocketManager;
   vpr::sim::NetworkGraph  mGraph;

   // This map of intervals to events is always sorted so that we can
   // iterate over it in increasing order of event times.
   typedef std::multimap<vpr::Interval, NetworkGraph::net_edge_t> event_map_t;
   event_map_t mEvents;
};

} // End of sim namespace

} // End of vpr namespace


#endif
