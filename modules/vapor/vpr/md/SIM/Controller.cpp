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

#include <boost/utility.hpp>
#include <vpr/Util/Interval.h>
#include <vpr/Util/Debug.h>
#include <vpr/Thread/Thread.h>

#include <vpr/md/SIM/Network/Message.h>
#include <vpr/md/SIM/Network/MessagePtr.h>
#include <vpr/md/SIM/IO/Socket/SocketImplSIM.h>
#include <vpr/md/SIM/Controller.h>


namespace vpr
{

namespace sim
{

Controller* Controller::mPrimordialInstance = NULL;
vpr::TSObjectProxy<Controller::ControllerTS> Controller::mInstance;

// ============================================================================
// Public methods.
// ============================================================================

Controller::Controller ()
   : mSleepTime(0)
{
   /* Do nothing. */ ;
}

vpr::ReturnStatus Controller::constructNetwork (const std::string& graph_file)
{
   vpr::ReturnStatus status;

   if ( mGraph.isValid() )
   {
      mGraph.clear();
   }

   status = mGraph.construct(graph_file);

   if ( status.success() )
   {
      mSocketManager.setActive();
   }

   return status;
}

void Controller::addEvent (const vpr::Interval& event_time,
                           const NetworkGraph::net_edge_t edge,
                           const NetworkLine::LineDirection dir)
{
   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
      << "Controller::addEvent(): Adding event scheduled for time "
      << event_time.getBaseVal() << " on edge " << edge << "\n"
      << vprDEBUG_FLUSH;
   mEvents.insert(std::pair<vpr::Interval, EventData>(event_time, EventData(edge, dir)));
}

void Controller::processNextEvent (vpr::SocketImplSIM** recvSocket)
{
   (*recvSocket) = NULL;
   event_map_t::iterator cur_event = mEvents.begin();

   if ( cur_event != mEvents.end() )
   {
      vpr::Interval event_time            = (*cur_event).first;
      NetworkGraph::net_edge_t event_edge = (*cur_event).second.edge;
      NetworkLine::LineDirection dir      = (*cur_event).second.direction;
      vpr::sim::NetworkLine& line         = mGraph.getLineProperty(event_edge);
      vpr::sim::MessagePtr msg;
      vpr::ReturnStatus status;

      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "Controller::processNextEvent() [time = "
         << mClock.getCurrentTime().getBaseVal()
         << "]: Processing event scheduled to occur at time "
         << event_time.getBaseVal() << "\n" << vprDEBUG_FLUSH;

      // ----------------------------------------------------------------------
      // Process event in the line's transmission queue.
      // ----------------------------------------------------------------------

      status = line.getArrivedMessage(event_time, msg, dir);
      vprASSERT(status.success() && "No arrived message at this time");

      vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
         << "Controller::processNextEvent(): Event is an arrived message on "
         << ((dir == vpr::sim::NetworkLine::FORWARD) ? "forward" : "reverse")
         << " queue of line " << line.getNetworkAddressString() << "\n"
         << vprDEBUG_FLUSH;
      mClock.setCurrentTime(event_time);
      moveMessage(msg, event_time, recvSocket);

      mEvents.erase(cur_event);

      if ( mSleepTime != 0 )
      {
         vpr::Thread::usleep(mSleepTime);
      }
   }
}

void Controller::processEvents (const vpr::Interval& time_step)
{
   vpr::SocketImplSIM* recv_sock;
   vpr::Interval event_time = mClock.getCurrentTime() + time_step;
   event_map_t::iterator next_event;

   mClock.setCurrentTime(event_time);

   // The current time based on the time step is the same as or greater than
   // the time of the next event in the queue.  That means that that event is
   // eligible for processing.
   while ( (next_event = mEvents.begin()) != mEvents.end() &&
           (*next_event).first <= event_time )
   {
      processNextEvent(&recv_sock);
   }
}

// ============================================================================
// Private methods.
// ============================================================================

void Controller::moveMessage (vpr::sim::MessagePtr msg,
                              const vpr::Interval& cur_time,
                              vpr::SocketImplSIM** recvSocket)
{
   (*recvSocket) = NULL;
   NetworkGraph::net_vertex_t next_hop = msg->getNextHop();

   bool end_of_path;
   msg->incNextHop(end_of_path);

   // Pass the message on to the next line.
   if ( ! end_of_path )
   {
      NetworkGraph::net_vertex_t next_next_hop = msg->getNextHop();
      NetworkGraph::net_edge_t next_line;
      bool got_next_line;
      NetworkLine::LineDirection dir;

      boost::tie(next_line, got_next_line) = mGraph.getEdge(next_hop,
                                                            next_next_hop);
      vprASSERT(got_next_line && "Edge between nodes not found!");

      vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
         << "Controller::moveMessage(): Passing message on to next hop -- "
         << next_line << "\n" << vprDEBUG_FLUSH;

      dir = mGraph.isSource(next_hop, next_line) ? NetworkLine::FORWARD
                                                 : NetworkLine::REVERSE;

      vpr::sim::NetworkLine& next_line_prop = mGraph.getLineProperty(next_line);
      next_line_prop.calculateMessageEventTimes(msg, cur_time, dir);

      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "Controller::moveMessage(): New message times: "
         << "starts = " << msg->whenStartOnWire().getBaseVal() << ", "
         << "transmits = " << msg->whenFullyOnWire().getBaseVal() << ", "
         << "arrives = " << msg->whenArrivesFully().getBaseVal()
         << std::endl << vprDEBUG_FLUSH;

      next_line_prop.addMessage(msg, dir);
      addEvent(msg->whenArrivesFully(), next_line, dir);
   }
   // End of the path--we have reached our destination.
   else
   {
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "Controller::moveMessage() [time = "
         << mClock.getCurrentTime().getBaseVal()
         << "]: Delivering message to destination "
         << msg->getDestinationSocket()->getLocalAddr() << std::endl
         << vprDEBUG_FLUSH;

      msg->getDestinationSocket()->addArrivedMessage(msg);
      (*recvSocket) = msg->getDestinationSocket();             // Return the socket that now has the event

      // The above should be the last use of the memory held by msg, so the
      // should get deleted when the value of msg changes.
   }
}

} // End of sim namespace

} // End of vpr namespace
