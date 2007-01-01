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

#include <boost/utility.hpp>
#include <vpr/Util/Interval.h>
#include <vpr/Util/Debug.h>
#include <vpr/Thread/Thread.h>
#include <vpr/System.h>

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

void Controller::addMessageEvent (const vpr::Interval& event_time,
                                  const NetworkGraph::net_edge_t edge,
                                  const NetworkLine::LineDirection dir)
{
   vprDEBUG(vprDBG_SIM, vprDBG_HVERB_LVL)
      << "Controller::addMessageEvent(): Adding message event scheduled for time "
      << event_time.getBaseVal() << " on edge " << edge << "\n"
      << vprDEBUG_FLUSH;
   mEvents.insert(std::pair<vpr::Interval, EventData>(event_time, EventData(edge, dir)));
}

void Controller::addConnectionEvent (const vpr::Interval& event_time,
                                     vpr::SocketImplSIM* acceptor_sock)
{
   vprDEBUG(vprDBG_SIM, vprDBG_HVERB_LVL)
      << "Controller::addConnectionEvent(): Adding connection request event scheduled for time "
      << event_time.getBaseVal() << "\n" << vprDEBUG_FLUSH;
   mEvents.insert(std::pair<vpr::Interval, EventData>(event_time, EventData(acceptor_sock, EventData::CONNECTION_INIT)));
}

void Controller::addConnectionCompletionEvent (const vpr::Interval& event_time,
                                               vpr::SocketImplSIM* connector_sock)
{
   vprDEBUG(vprDBG_SIM, vprDBG_HVERB_LVL)
      << "Controller::addConnectionCompletionEvent(): Adding connection "
      << "completion event scheduled for time " << event_time.getBaseVal()
      << "\n" << vprDEBUG_FLUSH;
   mEvents.insert(std::pair<vpr::Interval, EventData>(event_time, EventData(connector_sock, EventData::CONNECTION_COMPLETE)));
}

void Controller::addLocalhostDeliveryEvent (const vpr::Interval& event_time,
                                            vpr::SocketImplSIM* connector_sock)
{
   vprDEBUG(vprDBG_SIM, vprDBG_HVERB_LVL)
      << "Controller::addConnectionCompletionEvent(): Adding localhost "
      << "delivery event scheduled for time " << event_time.getBaseVal()
      << "\n" << vprDEBUG_FLUSH;
   mEvents.insert(std::pair<vpr::Interval, EventData>(event_time, EventData(connector_sock, EventData::LOCALHOST_DELIVERY)));
}

void Controller::flushPath (const vpr::SocketImplSIM* sock,
                            vpr::sim::NetworkGraph::VertexListPtr path)
{
vpr::DebugOutputGuard dbg_output(vprDBG_SIM, vprDBG_STATE_LVL,
                                 std::string("Controller::flushPath()\n"),
                                 std::string("End of Controller::flushPath()\n"));

   vpr::sim::NetworkGraph::VertexList::iterator source, dest;
   vpr::sim::NetworkGraph::net_edge_t current_line;
   bool got_next_line;

   source = dest = path->begin();
   ++dest;

   for ( ; dest != path->end(); ++dest )
   {
      boost::tie(current_line, got_next_line) = mGraph.getEdge(*source, *dest);

      if ( got_next_line )
      {
         vprDEBUG(vprDBG_SIM, vprDBG_STATE_LVL)
            << "Controller::flushPath(): Working on line " << current_line
            << std::endl << vprDEBUG_FLUSH;

         vpr::sim::NetworkLine& line_prop = mGraph.getLineProperty(current_line);
         vpr::sim::NetworkLine::LineDirection dir;
         std::vector<vpr::Interval> event_times;

         dir = mGraph.isSource(*source, current_line) ? NetworkLine::FORWARD
                                                      : NetworkLine::REVERSE;

         vprDEBUG(vprDBG_SIM, vprDBG_VERB_LVL)
            << "Controller::flushPath(): Removing active messages on line "
            << current_line << " in the "
            << ((dir == NetworkLine::FORWARD) ? "forward" : "reverse")
            << " queue\n" << vprDEBUG_FLUSH;

         line_prop.removeActiveMessages(sock, event_times, dir);

         // If the event_times vector has elements in it, these specify the
         // times at which flushed messages were scheduled to arrive.  We must
         // remove these events from the mEvents container.
         if ( ! event_times.empty() )
         {
            EventData test_event(current_line, dir);    // Message event
            event_map_t::iterator event_iter, end_iter;

            // For each of the scheduled event times, look up all the events
            // in the event container with that time and find the one that
            // matches the message flushed from the line above.
            for ( std::vector<vpr::Interval>::iterator i = event_times.begin();
                  i != event_times.end();
                  ++i )
            {
               vprDEBUG(vprDBG_SIM, vprDBG_STATE_LVL)
                  << "Controller::flushPath(): Looking for events scheduled to occur at "
                  << (*i).getBaseVal() << std::endl << vprDEBUG_FLUSH;

               boost::tie(event_iter, end_iter) = mEvents.equal_range(*i);

               // XXX: I'm not sure if std::remove_if() can be used here
               // because mEvents is currently an associative container.
               for ( ; event_iter != end_iter; ++event_iter )
               {
                  if ( test_event == (*event_iter).second )
                  {
                     vprDEBUG(vprDBG_SIM, vprDBG_VERB_LVL)
                        << "Controller::flushPath(): Removing event\n"
                        << vprDEBUG_FLUSH;
                     mEvents.erase(event_iter);
                  }
               }
            }
         }
      }

      // Finally, move the source to point to the current destination.  The
      // destination will be incremented as part of the for loop's
      // iteration.
      source = dest;
   }
}

void Controller::processNextEvent (vpr::SocketImplSIM** recvSocket)
{
vpr::DebugOutputGuard dbg_output(vprDBG_SIM, vprDBG_STATE_LVL,
                                 std::string("Controller::processNextEvent()\n"),
                                 std::string("End of Controller::processNextEvent()\n"));

   if ( recvSocket != NULL )
   {
      (*recvSocket) = NULL;
   }

   event_map_t::iterator cur_event = mEvents.begin();

   if ( cur_event != mEvents.end() )
   {
      vpr::Interval event_time = (*cur_event).first;

      vprDEBUG(vprDBG_SIM, vprDBG_VERB_LVL)
         << "Controller::processNextEvent() [time = "
         << mClock.getCurrentTime().getBaseVal()
         << "]: Processing event scheduled to occur at time "
         << event_time.getBaseVal() << " (moving clock forward)\n"
         << vprDEBUG_FLUSH;

      mClock.setCurrentTime(event_time);

      if ( (*cur_event).second.type == EventData::MESSAGE )
      {
         NetworkGraph::net_edge_t event_edge = (*cur_event).second.edge;
         NetworkLine::LineDirection dir      = (*cur_event).second.direction;
         vpr::sim::NetworkLine& line         = mGraph.getLineProperty(event_edge);
         vpr::sim::MessagePtr msg;
         vpr::ReturnStatus status;

         // -------------------------------------------------------------------
         // Process event in the line's transmission queue.
         // -------------------------------------------------------------------

         status = line.getArrivedMessage(event_time, msg, dir);
         vprASSERT(status.success() && "No arrived message at this time");

         vprDEBUG(vprDBG_SIM, vprDBG_HVERB_LVL)
            << "Controller::processNextEvent(): Event is the arrival of a message for "
            << msg->getDestinationSocket()->getLocalAddr() << " on "
            << ((dir == vpr::sim::NetworkLine::FORWARD) ? "forward" : "reverse")
            << " queue of line " << line.getNetworkAddressString() << "\n"
            << vprDEBUG_FLUSH;

         moveMessage(msg, event_time, recvSocket);
      }
      else if ( (*cur_event).second.type == EventData::CONNECTION_COMPLETE )
      {
         vprDEBUG(vprDBG_SIM, vprDBG_HVERB_LVL)
            << "Controller::processNextEvent(): Event is a connection completion for "
            << (*cur_event).second.socket->getLocalAddr() << "\n"
            << vprDEBUG_FLUSH;

         if ( recvSocket != NULL )
         {
            *recvSocket = (*cur_event).second.socket;
         }
      }
      else if ( (*cur_event).second.type == EventData::CONNECTION_INIT )
      {
         vprDEBUG(vprDBG_SIM, vprDBG_HVERB_LVL)
            << "Controller::processNextEvent(): Event is a connection request to "
            << (*cur_event).second.socket->getLocalAddr() << "\n"
            << vprDEBUG_FLUSH;

         if ( recvSocket != NULL )
         {
            *recvSocket = (*cur_event).second.socket;
         }
      }
      else
      {
         vprDEBUG(vprDBG_SIM, vprDBG_HVERB_LVL)
            << "Controller::processNextEvent(): Event is a localhost message delivery to "
            << (*cur_event).second.socket->getLocalAddr() << "\n"
            << vprDEBUG_FLUSH;

         if ( recvSocket != NULL )
         {
            *recvSocket = (*cur_event).second.socket;
         }
      }

      mEvents.erase(cur_event);

      if ( mSleepTime != 0 )
      {
         vpr::System::usleep(mSleepTime);
      }
   }
}

void Controller::processEvents(const vpr::Interval& time_step)
{
vpr::DebugOutputGuard dbg_output(vprDBG_SIM, vprDBG_STATE_LVL,
                                 std::string("Controller::processEvents()\n"),
                                 std::string("End of Controller::processEvents()\n"));

   vpr::SocketImplSIM* recv_sock;
   vpr::Interval event_time = mClock.getCurrentTime() + time_step;
   event_map_t::iterator next_event;

   vprDEBUG(vprDBG_SIM, vprDBG_VERB_LVL)
      << "Controller::processEvents [time = "
      << mClock.getCurrentTime().getBaseVal() << "]: Moving clock ahead "
      << time_step.getBaseVal() << " units to be " << event_time.getBaseVal()
      << std::endl << vprDEBUG_FLUSH;

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

      vprDEBUG(vprDBG_SIM, vprDBG_HVERB_LVL)
         << "Controller::moveMessage(): Passing message for "
         << msg->getDestinationSocket()->getLocalAddr()
         << " on to next hop -- " << next_line << "\n" << vprDEBUG_FLUSH;

      dir = mGraph.isSource(next_hop, next_line) ? NetworkLine::FORWARD
                                                 : NetworkLine::REVERSE;

      vpr::sim::NetworkLine& next_line_prop = mGraph.getLineProperty(next_line);
      next_line_prop.calculateMessageEventTimes(msg, cur_time, dir);

      vprDEBUG(vprDBG_SIM, vprDBG_VERB_LVL)
         << "Controller::moveMessage(): New message times: "
         << "starts = " << msg->whenStartOnWire().getBaseVal() << ", "
         << "transmits = " << msg->whenFullyOnWire().getBaseVal() << ", "
         << "arrives = " << msg->whenArrivesFully().getBaseVal()
         << std::endl << vprDEBUG_FLUSH;

      next_line_prop.addMessage(msg, dir);
      addMessageEvent(msg->whenArrivesFully(), next_line, dir);
   }
   // End of the path--we have reached our destination.
   else
   {
      vprDEBUG(vprDBG_SIM, vprDBG_STATE_LVL)
         << "Controller::moveMessage() [time = "
         << mClock.getCurrentTime().getBaseVal()
         << "]: Delivering message to destination\n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_SIM, vprDBG_HVERB_LVL)
         << "Controller::moveMessage(): Message path was "
         << msg->getSourceSocket()->getLocalAddr() << " ("
         << std::hex << msg->getSourceSocket() << std::dec << ") ==> "
         << msg->getDestinationSocket()->getLocalAddr() << " ("
         << std::hex << msg->getDestinationSocket() << std::dec << ")\n"
         << vprDEBUG_FLUSH;

      vprASSERT(msg->getDestinationSocket()->isOpen() && "The destination socket has been closed (or possibly destroyed)");
      msg->getDestinationSocket()->addArrivedMessage(msg);

      if ( recvSocket != NULL)
      {
         (*recvSocket) = msg->getDestinationSocket();  // Return the socket that now has the event
      }

      // The above should be the last use of the memory held by msg, so the
      // should get deleted when the value of msg changes.
   }
}

} // End of sim namespace

} // End of vpr namespace
