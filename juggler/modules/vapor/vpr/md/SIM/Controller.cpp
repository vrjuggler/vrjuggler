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

#include <vpr/md/SIM/Network/Message.h>
#include <vpr/md/SIM/Network/MessagePtr.h>
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
   : m_started(false)
{
   /* Do nothing. */ ;
}

vpr::ReturnStatus Controller::constructNetwork (const std::string& graph_file)
{
   vpr::ReturnStatus status;

   status = mGraph.construct(graph_file);

   if ( status.success() )
   {
      mSocketManager.setActive();
   }

   return status;
}

void Controller::addEvent (const vpr::Interval& event_time,
                           const NetworkGraph::net_edge_t edge)
{
   mEvents.insert(std::pair<vpr::Interval, NetworkGraph::net_edge_t>(event_time, edge));
}

void Controller::processNextEvent ()
{
   event_map_t::iterator cur_event = mEvents.begin();

   if ( cur_event != mEvents.end() )
   {
      bool event_processed                = false;
      vpr::Interval event_time            = (*cur_event).first;
      NetworkGraph::net_edge_t event_edge = (*cur_event).second;
      vpr::sim::NetworkLine line          = mGraph.getLineProperty(event_edge);
      vpr::sim::MessagePtr msg;
      vpr::ReturnStatus status;

      // ----------------------------------------------------------------------
      // Process event in the line's transmission queue.
      // ----------------------------------------------------------------------

      status = line.checkForArrivedMessage(event_time, msg,
                                           NetworkLine::FORWARD);

      if ( status.success() )
      {
         moveMessage(msg, event_time);
         event_processed = true;
      }
      else
      {
         status = line.checkForArrivedMessage(event_time, msg,
                                              NetworkLine::REVERSE);

         if ( status.success() )
         {
            moveMessage(msg, event_time);
            event_processed = true;
         }
      }

      // If the event was not processed as a message arrival, try to process
      // the event as a message that is ready to start transmission.
      if ( ! event_processed )
      {
         // -------------------------------------------------------------------
         // Process event in the line's ready queue.
         // -------------------------------------------------------------------

         status = line.checkForReadyMessage(event_time, msg,
                                            NetworkLine::FORWARD);

         if ( status.success() )
         {
            event_processed = true;
            addEvent(msg->whenArrivesFully(), event_edge);
         }
         else
         {
            status = line.checkForReadyMessage(event_time, msg,
                                               NetworkLine::REVERSE);

            if ( status.success() )
            {
               event_processed = true;
               addEvent(msg->whenArrivesFully(), event_edge);
            }
         }
      }

      vprASSERT(event_processed && "The event was not processed!");

      // If the event was processed, we remove it from the queue and return.
      if ( event_processed )
      {
         mEvents.erase(cur_event);
      }
   }
}

// ============================================================================
// Private methods.
// ============================================================================

void Controller::moveMessage (vpr::sim::MessagePtr msg,
                              const vpr::Interval& cur_time)
{
   NetworkGraph::net_vertex_t next_hop;

   if ( msg->getNextHop(next_hop).success() )
   {
      NetworkGraph::net_vertex_t next_next_hop;

      // Pass the message on to the next line.
      if ( msg->getNextHop(next_next_hop).success() )
      {
         NetworkGraph::net_edge_t next_line;
         bool got_next_line;
         vpr::sim::NetworkLine next_line_prop;
         NetworkLine::LineDirection dir;

         boost::tie(next_line, got_next_line) = mGraph.getEdge(next_hop,
                                                               next_next_hop);
         vprASSERT(got_next_line && "Edge between nodes not found!");

         dir = mGraph.isSource(next_hop, next_line) ? NetworkLine::FORWARD
                                                    : NetworkLine::REVERSE;

         next_line_prop = mGraph.getLineProperty(next_line);
         next_line_prop.calculateMessageEventTimes(msg, cur_time, dir);

         vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
            << "New message times: "
            << "starts = " << msg->whenStartOnWire().usec() << ", "
            << "transmitts = " << msg->whenFullyOnWire().usec() << ", "
            << "arrives = " << msg->whenArrivesFully().usec()
            << std::endl << vprDEBUG_FLUSH;

         next_line_prop.addReadyMessage(msg, dir);
         mGraph.setLineProperty(next_line, next_line_prop);
         addEvent(msg->whenStartOnWire(), next_line);
      }
      // End of the path--we have reached our destination.
      else
      {
         vpr::sim::NetworkNode next_hop_prop;

         next_hop_prop = mGraph.getNodeProperty(next_hop);
         next_hop_prop.deliverMessage(msg);

         // The above should be the last use of the memory held by msg,
         // so the memory should  get deleted when the value of msg
         // changes.
      }
   }
   // This should not happen!
   else
   {
      vprASSERT(false && "Tried to pass message past the last hop");
   }
}

} // End of sim namespace

} // End of vpr namespace
