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

#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <vpr/md/SIM/Network/Message.h>
#include <vpr/md/SIM/Network/NetworkLine.h>


namespace vpr
{

namespace sim
{

NetworkLine::NetworkLine(const double miles, const double Mbps,
                         const double delay, const std::string& netType,
                         const vpr::Uint8 netID, const std::string& netIP)
   : mLength(miles)
   , mCapacity(Mbps)
   , mDelay(delay)
   , mNetworkID(netID)
   , mNetworkIP(0)
   , mNetworkIPStr(netIP)
{
   // 5 is the approximate number of microseconds it takes light to move
   // one mile (i.e., 5 usec/mile).
   mLatency = 5.0f * miles;

   if ( netType.compare("WAN") == 0 )
   {
      mNetworkType = NetworkLine::WAN;
   }
   else if ( netType.compare("MAN") == 0 )
   {
      mNetworkType = NetworkLine::MAN;
   }
   else if ( netType.compare("LAN") == 0 )
   {
      mNetworkType = NetworkLine::LAN;
   }
   else
   {
      mNetworkType = NetworkLine::LOOPBACK;
   }
}

// ============================================================================
// Private methods.
// ============================================================================

vpr::ReturnStatus NetworkLine::getArrivedMessageFromQueue(const vpr::Interval& eventTime,
                                                          vpr::sim::MessagePtr& msg,
                                                          msg_queue_t& queue)
{
   vpr::ReturnStatus status(vpr::ReturnStatus::Fail);

   vprASSERT(! queue.empty() && "Queue must have an event!");

   msg = queue.front().second;
   vprASSERT(msg->whenArrivesFully() == eventTime && "This event must be the message on the front of the queue");

   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
      << "NetworkLine::getArrivedMessage() [" << mNetworkIPStr
      << "]: Message arrived at "
      << msg->whenArrivesFully().getBaseVal() << ", current event time is "
      << eventTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;

   queue.pop_front();
   status.setCode(vpr::ReturnStatus::Succeed);

   return status;
}

void NetworkLine::calculateMessageEventTimes(vpr::sim::MessagePtr msg,
                                             const vpr::Interval& curTime,
                                             msg_queue_t& queue)
{
   // XXX: Syncronization issue here!!!!

   // Need to set the message's times for wire stuff...
   if ( ! queue.empty() )
   {
      vpr::sim::MessagePtr tail_msg = queue.back().second;

      // Set the start time for the transmission of the given message to the
      // max of the current time and the full transmission time of the last
      // message in the queue.
      vpr::Interval wire_ready(tail_msg->whenFullyOnWire().getBaseVal() + 1,
                               vpr::Interval::Base);
      vpr::Interval start_time = (wire_ready < curTime ? curTime
                                                       : wire_ready);

      vpr::Interval on_wire_time(start_time + getWireAccessTime(msg->getSize() * 8));

      msg->setStartOnWireTime(start_time);
      msg->setFullyOnWireTime(on_wire_time);
      msg->setArrivesFullyTime(on_wire_time + getBitTransmissionTime());
   }
   else
   {
      vpr::Interval on_wire_time(curTime + getWireAccessTime(msg->getSize() * 8));
      msg->setStartOnWireTime(curTime);
      msg->setFullyOnWireTime(on_wire_time);
      msg->setArrivesFullyTime(on_wire_time + getBitTransmissionTime());
   }
}

void NetworkLine::addMessageToQueue(vpr::sim::MessagePtr msg,
                                    msg_queue_t& queue)
{
   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
      << "NetworkLine::addMessage(): Adding new message to queue for "
      << mNetworkIPStr << " (" << msg->whenStartOnWire().getBaseVal() << ", "
      << msg->whenFullyOnWire().getBaseVal() << ", "
      << msg->whenArrivesFully().getBaseVal() << ")\n" << vprDEBUG_FLUSH;
#ifdef VPR_DEBUG
   if ( ! queue.empty() )
   {
      vpr::Interval back_time = queue.back().first;
      vpr::Interval new_arrive_time = msg->whenArrivesFully();
      bool ordered = (back_time < new_arrive_time);
      bool ordered2 = (back_time < new_arrive_time);
      vprASSERT(queue.back().first < msg->whenArrivesFully() && "Message queued out of order");
   }
#endif

   queue.push_back(std::pair<vpr::Interval, vpr::sim::MessagePtr>(msg->whenArrivesFully(), msg));
}

template<class T>
struct RemovePred
{
   RemovePred (const vpr::SocketImplSIM* o)
      : sock(o)
   {
      ;
   }

   bool operator() (T thingy)
   {
      return thingy.second->getDestinationSocket() == sock;
   }

   const vpr::SocketImplSIM* sock;
};

void NetworkLine::removeMessagesFromQueue (const vpr::SocketImplSIM* sock,
                                           std::vector<vpr::Interval>& eventTimes,
                                           msg_queue_t& queue)
{
   msg_queue_t::iterator i =
      std::remove_if(queue.begin(), queue.end(),
                     RemovePred<msg_queue_entry_t>(sock));

#ifdef VPR_DEBUG
   if ( i != queue.end() )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "NetworkLine::removeMessagesFromQueue() [" << mNetworkIPStr
         << "]: Removing messages\n" << vprDEBUG_FLUSH;
   }
#endif

   // It might be possible to do this faster.
   for ( msg_queue_t::iterator j = i; j != queue.end(); j++ )
   {
      eventTimes.push_back((*j).first);
   }

   queue.erase(i, queue.end());
}

} // End of sim namespace

} // End of vpr namespace
