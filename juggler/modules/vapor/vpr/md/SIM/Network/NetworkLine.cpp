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

#include <vpr/Util/Debug.h>
#include <vpr/md/SIM/Network/Message.h>
#include <vpr/md/SIM/Network/MessagePtr.h>
#include <vpr/md/SIM/Network/NetworkLine.h>


namespace vpr
{

namespace sim
{

NetworkLine::NetworkLine (const double miles, const double Mbps,
                          const double delay, const std::string& net_type,
                          const vpr::Uint8 net_id, const std::string& net_ip)
   : mLength(miles), mCapacity(Mbps), mDelay(delay), mNetworkID(net_id),
     mNetworkIP(0), mNetworkIPStr(net_ip)
{
   // 5 is the approximate number of microseconds it takes light to move
   // one mile (i.e., 5 usec/mile).
   mLatency = 5.0f * miles;

   if ( net_type.compare("WAN") == 0 )
   {
      mNetworkType = NetworkLine::WAN;
   }
   else if ( net_type.compare("MAN") == 0 )
   {
      mNetworkType = NetworkLine::MAN;
   }
   else
   {
      mNetworkType = NetworkLine::LAN;
   }
}

// ============================================================================
// Private methods.
// ============================================================================

vpr::ReturnStatus NetworkLine::getReadyMessage (const vpr::Interval& event_time,
                                                vpr::sim::MessagePtr& msg,
                                                msg_queue_t& queue)
{
   vpr::ReturnStatus status(vpr::ReturnStatus::Fail);

   if ( queue.size() > 0 )
   {
      msg_queue_t::iterator front    = queue.begin();
      vpr::sim::MessagePtr front_msg = (*front).second;

      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "NetworkLine::getReadyMessage() [" << mNetworkIPStr
         << "]: Next event occurs at "
         << front_msg->whenStartOnWire().getBaseVal() << ", event time is "
         << event_time.getBaseVal() << "\n" << vprDEBUG_FLUSH;

      if ( front_msg->whenStartOnWire() <= event_time )
      {
         msg = front_msg;
         status.setCode(vpr::ReturnStatus::Succeed);
      }
   }

   return status;
}

vpr::ReturnStatus NetworkLine::getTransmittedMessage (const vpr::Interval& event_time,
                                                      vpr::sim::MessagePtr& msg,
                                                      msg_queue_t& queue)
{
   vpr::ReturnStatus status(vpr::ReturnStatus::Fail);

   if ( queue.size() > 0 )
   {
      msg_queue_t::iterator front    = queue.begin();
      vpr::sim::MessagePtr front_msg = (*front).second;

      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "NetworkLine::getTransmittedMessage() [" << mNetworkIPStr
         << "]: Next event occurs at "
         << front_msg->whenFullyOnWire().getBaseVal() << ", event time is "
         << event_time.getBaseVal() << "\n" << vprDEBUG_FLUSH;

      if ( front_msg->whenFullyOnWire() <= event_time )
      {
         msg = front_msg;
         status.setCode(vpr::ReturnStatus::Succeed);
      }
   }

   return status;
}

vpr::ReturnStatus NetworkLine::getArrivedMessage (const vpr::Interval& event_time,
                                                  vpr::sim::MessagePtr& msg,
                                                  msg_queue_t& queue)
{
   vpr::ReturnStatus status(vpr::ReturnStatus::Fail);

   if ( queue.size() > 0 )
   {
      msg_queue_t::iterator front    = queue.begin();
      vpr::sim::MessagePtr front_msg = (*front).second;

      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "NetworkLine::getArrivedMessage() [" << mNetworkIPStr
         << "]: Next event occurs at "
         << front_msg->whenArrivesFully().getBaseVal() << ", event time is "
         << event_time.getBaseVal() << "\n" << vprDEBUG_FLUSH;

      if ( front_msg->whenArrivesFully() <= event_time )
      {
         msg = front_msg;
         status.setCode(vpr::ReturnStatus::Succeed);
      }
   }

   return status;
}

void NetworkLine::calculateMessageEventTimes (vpr::sim::MessagePtr msg,
                                              const vpr::Interval& cur_time,
                                              msg_queue_t& queue)
{
   // Need to set the message's times for wire stuff...
   if ( queue.size() > 0 )
   {
      msg_queue_t::reverse_iterator last = queue.rbegin();
      vpr::sim::MessagePtr tail_msg      = (*last).second;

      // Set the start time for the transmission of the given message to the
      // max of the current time and the full transmission time of the last
      // message in the queue.
      vpr::Interval wire_ready(tail_msg->whenFullyOnWire().getBaseVal() + 1,
                               vpr::Interval::Base);
      vpr::Interval start_time = (wire_ready < cur_time ? cur_time
                                                        : wire_ready);

      msg->setStartOnWireTime(start_time);
      msg->setFullyOnWireTime(msg->whenStartOnWire() +
                                 getWireAccessTime(msg->getSize() * 8));
      msg->setArrivesFullyTime(msg->whenFullyOnWire() + getBitTransmissionTime());
   }
   else
   {
      msg->setStartOnWireTime(cur_time);
      msg->setFullyOnWireTime(cur_time + getWireAccessTime(msg->getSize() * 8));
      msg->setArrivesFullyTime(msg->whenFullyOnWire() + getBitTransmissionTime());
   }
}

void NetworkLine::addMessage (vpr::sim::MessagePtr msg, msg_queue_t& queue)
{
   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
      << "NetworkLine::addMessage(): Adding new message to queue for "
      << mNetworkIPStr << " (" << msg->whenStartOnWire().getBaseVal() << ", "
      << msg->whenFullyOnWire().getBaseVal() << ", "
      << msg->whenArrivesFully().getBaseVal() << ")\n" << vprDEBUG_FLUSH;
   queue[msg->whenStartOnWire()] = msg;
}

} // End of sim namespace

} // End of vpr namespace
