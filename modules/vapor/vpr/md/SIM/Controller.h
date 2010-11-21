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
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

/** \class Controller Controller.h vpr/md/SIM/Controller.h
 *
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
    * Initializes the socket simulation.  This should only be called when a
    * thread-specific instance is needed.  Otherwise, use the static
    * instance() method to get a thread-specific or global instance depending
    * on what is available.
    *
    * @post An instance of the Sim Socket Manager is retrieved, and the
    *       simulation state is set to not started.
    */
   Controller();

   /**
    * Resets the Sim Socket Manager's clock to 0.
    *
    * @post The Sim Socket Manager's clock is reset to 0 so that the same
    *       instance may be used by another simulation.
    */
   ~Controller()
   {
      /* Do nothing. */ ;
   }

   /**
    * Set the instance to be returned in this thread to the given object
    * pointer.  This should be done as the first step by all threads wishing to
    * have a thread-specific instance of this singleton class.
    */
   static void setInstance (Controller* c)
   {
      mInstance->setObject(c);
   }

   /**
    * Returns an instance of this thread-specific singleton class.  If a
    * thread-specific instance was set by the currently active thread using
    * setInstance(), that instance is returned.  If no instance is associated
    * with the current thread, the primordial (global) instance is returned.
    */
   static Controller* instance()
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

   void destroyNetworkGraph()
   {
      mGraph.clear();
   }

   /**
    * Queries the running state of this socket simulation.  The simulation is
    * considered running if it has been started and if the Sim Socket
    * Manager still has active sockets registered.
    *
    * @post The running state of this simulation is returned to the caller.
    */
   bool isRunning()
   {
      vprASSERT(false && "Not supported right now.  If you want it, then implement it.");
      //return mGraph.isValid() && mSocketManager.hasActiveSockets();
      return false;
   }

   /**
    * Sets the amount of time (in microseconds) to sleep after processing a
    * single event.
    */
   void setSimulationPauseTime(const vpr::Uint32 sleep_time)
   {
      mSleepTime = sleep_time;
   }

   /**
    * Adds an event scheduled to occur at the given time to the queue of
    * upcoming events.
    */
   void addMessageEvent(const vpr::Interval& event_time,
                        const NetworkGraph::net_edge_t edge,
                        const NetworkLine::LineDirection dir);

   void addConnectionEvent(const vpr::Interval& event_time,
                           vpr::SocketImplSIM* acceptor_sock);

   void addConnectionCompletionEvent(const vpr::Interval& event_time,
                                     vpr::SocketImplSIM* connector_sock);

   void addLocalhostDeliveryEvent(const vpr::Interval& event_time,
                                  vpr::SocketImplSIM* connector_sock);

   /**
    * Flushes all messages (and associated events) on the given path destined
    * for the given socket.
    */
   void flushPath(const vpr::SocketImplSIM* sock,
                  vpr::sim::NetworkGraph::VertexListPtr path);

   /**
    * Processes the next event in the event queue no matter how far into the
    * (simulated) future it occurs.  If there is an event in the queue, it will
    * be processed by this method.
    * @param recvSocket The socket that recv'ed the event (NULL if none)
    */
   void processNextEvent(vpr::SocketImplSIM** recvSocket = NULL);

   /**
    * Limits the time frame for the occurrence of the next events to the given
    * time step.  There may not be an event in the queue that occurs between
    * the current time and the time step, and in that case, no event will be
    * processed.
    *
    * @param time_step The maximum time step allowed for the simulation.
    */
   void processEvents(const vpr::Interval& time_step);

   /**
   * Return the number of events pending in the system
   */
   vpr::Uint32 getNumPendingEvents()
   {
      return mEvents.size();
   }

   const vpr::sim::Clock& getClock() const
   {
      return mClock;
   }

   vpr::sim::SocketManager& getSocketManager()
   {
      return mSocketManager;
   }

   vpr::sim::NetworkGraph& getNetworkGraph()
   {
      return mGraph;
   }

private:
   void moveMessage(vpr::sim::MessagePtr, const vpr::Interval& cur_time,
                    vpr::SocketImplSIM** recvSocket);

   class ControllerTS
   {
   public:
      ControllerTS() : mObj(NULL)
      {
         /* Do nothing. */ ;
      }

      Controller* getObject() const
      {
         return mObj;
      }

      void setObject(Controller* c)
      {
         mObj = c;
      }

   private:
      Controller* mObj;
   };

   static Controller* mPrimordialInstance;
   static vpr::TSObjectProxy<ControllerTS> mInstance;

protected:  // --- Data members --- //
   // These two have to be here because Visual C++ will try to make them
   // exported public symbols.  This causes problems because copying vpr::Mutex
   // objects is not allowed.  We do not want to copy sim Controller instances
   // anyway, so this should be fine.
   Controller(const Controller& o) {;}
   void operator=(const Controller& o) {;}

   vpr::sim::Clock         mClock;              /**< The global clock that we are using */
   vpr::sim::SocketManager mSocketManager;      /**< The socket manager that we are using */
   vpr::sim::NetworkGraph  mGraph;              /**< The network graph used for the simulation */

   struct EventData
   {
      enum EventType
      {
         MESSAGE,             /**< Message arrival event */
         CONNECTION_INIT,     /**< Connection request event */
         CONNECTION_COMPLETE, /**< Connection completion event */
         LOCALHOST_DELIVERY   /**< Immediate localhost-delivery-of-crap event */
      };

      EventData(const NetworkGraph::net_edge_t _edge,
                const NetworkLine::LineDirection _dir)
         : type(MESSAGE), edge(_edge), direction(_dir)
      {
         ;
      }

      EventData(vpr::SocketImplSIM* sock, const EventType _type)
         : type(_type), socket(sock)
      {
         ;
      }

      bool operator==(const EventData& obj) const
      {
         bool status = false;

         if ( type == obj.type )
         {
            // This may need to be expanded to a full if/else if/else block
            // if the possible message data types expands beyond the above
            // four.
            status = (type == MESSAGE) ? (edge == obj.edge && direction == obj.direction)
                                       : (socket == obj.socket);
         }

         return status;
      }

      EventType                  type;
      NetworkGraph::net_edge_t   edge;
      NetworkLine::LineDirection direction;
      vpr::SocketImplSIM*        socket;
   };

   // This map of intervals to events is always sorted so that we can
   // iterate over it in increasing order of event times.
   typedef std::multimap<vpr::Interval, EventData> event_map_t;
   event_map_t mEvents;

   vpr::Uint32 mSleepTime;
};

} // End of sim namespace

} // End of vpr namespace


#endif
