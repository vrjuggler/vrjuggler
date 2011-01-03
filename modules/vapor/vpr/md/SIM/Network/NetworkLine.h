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

#ifndef _VPR_SIM_NETWORK_LINE_H_
#define _VPR_SIM_NETWORK_LINE_H_

#include <vpr/vprConfig.h>

#include <cstdlib>
#include <cmath>
#include <string>
#include <deque>
#include <vector>
#include <utility>
#include <vpr/vpr.h>
#include <vpr/Util/Interval.h>

#include <vpr/md/SIM/Network/MessagePtr.h>


namespace vpr
{

namespace sim
{

/** \class NetworkLine NetworkLine.h vpr/md/SIM/Network/NetworkLine.h
 *
 * A container class for the little collection of properties that are assigned
 * to edges (network lines) in the network graph.  Grouping them into a class
 * this way makes it easier for developers to manage the BGL property stuff,
 * and it makes it easier for users to query and update properties. This class
 * must be default constrible, assignable, and copy constructible.
 */
class VPR_CLASS_API NetworkLine
{
public:
   enum LineDirection
   {
      FORWARD,
      REVERSE
   };

   enum NetworkType
   {
      LAN,
      MAN,
      WAN,
      LOOPBACK
   };

   /**
    * Default constructor.
    */
   NetworkLine()
      : mLength(0.0f)
      , mCapacity(0.0f)
      , mDelay(0.0f)
      , mNetworkType(LAN)
      , mNetworkID(0)
      , mNetworkIP(0)
      , mLatency(0.0f)
   {
      /* Do nothing. */ ;
   }

   NetworkLine(const double miles, const double Mbps, const double delay,
               const std::string& netType, const vpr::Uint8 netID,
               const std::string& netIP);

   /**
    * Returns a value designating the "weight" of this line (edge) in the
    * network (graph).  Currently, the weight of this edge is its length cast
    * to an integer (the round-off error is acceptable).
    */
   int getWeight() const
   {
      return (int) mLength;
   }

   /**
    * Returns the length in miles of this network line.
    */
   double getLength() const
   {
      return mLength;
   }

   void setLength(const double miles)
   {
      mLength  = miles;
      mLatency = 5.0f * miles;
   }

   double getCapacity() const
   {
      return mCapacity;
   }

   void setCapacity(const double Mbps)
   {
      mCapacity = Mbps;
   }

   double getPropagationDelay() const
   {
      return mLatency;
   }

   /**
    * Calculates the amount of time needed to put the given number of bits
    * on the wire.
    */
   vpr::Interval getWireAccessTime(const vpr::Uint32 bits) const
   {
      // This gets the number of microseconds required to transmit the given
      // number of bits.  This works because mCapacity is measured in
      // _megabits_ per second but the given argument is simply bits.
      double microsec = ((double) bits / mCapacity);

      // Round up since vpr::Interval objects deal in whole numbers.
      double time = std::ceil(microsec);

      return vpr::Interval((vpr::Uint32) time, vpr::Interval::Usec);
   }

   const std::string& getNetworkAddressString() const
   {
      return mNetworkIPStr;
   }

   /**
    * Calculates the amount of time required to get a single bit down the
    * wire.
    */
   vpr::Interval getBitTransmissionTime() const
   {
      // Round up since vpr::Interval objects deal in whole numbers.
      return vpr::Interval((vpr::Uint32) ceil(getPropagationDelay()),
                           vpr::Interval::Usec);
   }

   void calculateMessageEventTimes(vpr::sim::MessagePtr msg,
                                   const vpr::Interval& curTime,
                                   const LineDirection direction)
   {
      switch (direction)
      {
         case FORWARD:
            calculateMessageEventTimes(msg, curTime, mForwardLineQueue);
            break;
         case REVERSE:
            calculateMessageEventTimes(msg, curTime, mReverseLineQueue);
            break;
      }
   }

   /**
    *
    */
   void addMessage(vpr::sim::MessagePtr msg, const LineDirection direction)
   {
      switch (direction)
      {
         case FORWARD:
            addMessageToQueue(msg, mForwardLineQueue);
            break;
         case REVERSE:
            addMessageToQueue(msg, mReverseLineQueue);
            break;
      }
   }

   void removeActiveMessages(const vpr::SocketImplSIM* sock,
                             std::vector<vpr::Interval>& eventTimes,
                             const LineDirection direction)
   {
      switch (direction)
      {
         case FORWARD:
            removeMessagesFromQueue(sock, eventTimes, mForwardLineQueue);
            break;
         case REVERSE:
            removeMessagesFromQueue(sock, eventTimes, mReverseLineQueue);
            break;
      }
   }

   /**
    */
   vpr::ReturnStatus getArrivedMessage(const vpr::Interval& eventTime,
                                       vpr::sim::MessagePtr& msg,
                                       const LineDirection direction)
   {
      vpr::ReturnStatus status;

      if ( direction == FORWARD )
      {
         status = getArrivedMessageFromQueue(eventTime, msg, mForwardLineQueue);
      }
      else
      {
         status = getArrivedMessageFromQueue(eventTime, msg, mReverseLineQueue);
      }

      return status;
   }

private:
   typedef std::pair<vpr::Interval, vpr::sim::MessagePtr> msg_queue_entry_t;
   typedef std::deque<msg_queue_entry_t> msg_queue_t;

   vpr::ReturnStatus getArrivedMessageFromQueue(const vpr::Interval& eventTime,
                                                vpr::sim::MessagePtr& msg,
                                                msg_queue_t& queue);

   void calculateMessageEventTimes(vpr::sim::MessagePtr msg,
                                   const vpr::Interval& curTime,
                                   msg_queue_t& queue);

   void addMessageToQueue(vpr::sim::MessagePtr msg, msg_queue_t& queue);

   void removeMessagesFromQueue(const vpr::SocketImplSIM* sock,
                                std::vector<vpr::Interval>& eventTimes,
                                msg_queue_t& queue);

   double      mLength;       /**< Length in miles */
   double      mCapacity;     /**< Capacity in Mbps */
   double      mDelay;
   NetworkType mNetworkType;  /**< */
   vpr::Uint8  mNetworkID;    /**< */
   vpr::Uint32 mNetworkIP;
   std::string mNetworkIPStr;

   double mLatency;       /**< Line propagation delay in microseconds */

   msg_queue_t mForwardLineQueue;
   msg_queue_t mReverseLineQueue;
};

} // End of sim namespace

} // End of vpr namespace


#endif /* _VPR_SIM_NETWORK_LINE_H_ */
