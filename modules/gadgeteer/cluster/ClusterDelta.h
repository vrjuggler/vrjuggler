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

#ifndef _GADGET_CLUSTER_DELTA_H
#define _GADGET_CLUSTER_DELTA_H

#include <gadget/gadgetConfig.h>

#include <vector>
#include <boost/static_assert.hpp>


   // Algorithm to calculate clock diff between the two nodes (as a vpr::Interval)
   // - Initialize tolerance value
   // Until(passes)
   // - Connector sends it's current time
   // - Reciever reads and sends back it's current time
   // - Connector calculates an approximate difference
   // - Connector sends a message with an approximated receive time and the calculated diff
   // - Reciever responds with either success or failure
   // - If passed exit loop
   // - else increase tolerance by 1ms
   // @todo: hton crap
   // NOTE:
   //    One of the reasons that this is so convoluted is....
   //    InitialState gets triggered explicitly when the previous handler completes
   //    Otherwise, the only way in is when there is a packet of data waiting for us
   //    thus, we can't just exist on any state.  We have to exit in a state that is waiting for data.
   //    This leads to much ugliness (like a goto. grrrrr)

namespace vpr
{
//   class SocketStream;
   class BufferObjectReader;
   class BufferObjectWriter;
}

namespace cluster
{

/** \class ClusterDelta ClusterDelta.h cluster/ClusterDelta.h
 *
 */
class GADGET_API ClusterDelta
{
public:
   ClusterDelta();
   void getPacket(unsigned num);
   void sendAndClear();
   void clearIntervals();
   void receiveExpectedTime();
   void createExpectedTime();
   void receiveHandshake();
   void createHandshake();
   void createResponce();
   void receiveResponce();
   void clientClusterDelta(vpr::SocketStream* socket_stream);
   vpr::Interval getClusterDelta(vpr::SocketStream* socket_stream);
   void setSocketStream(vpr::SocketStream* socket_stream)
   {
      mSocketStream = socket_stream;
   }

private:
   vpr::Interval mLocalSendTime;
   vpr::Interval mLocalReceiveTime;
   vpr::Interval mRemoteSendTime;
   vpr::Interval mRemoteReceiveTime;
   vpr::Interval mExpectedRemoteTime;
   vpr::Interval mLatencyTime;
   vpr::Interval mDelta;
   vpr::Interval mErrorTime;
   std::vector<vpr::Uint8> syncPacket;
   bool mAccept;

   // Tolerance
   float                   mTol;
   
   // Object Reader & Writer
   unsigned                mCurHeadPos;
   vpr::BufferObjectWriter       mWriter;
   vpr::BufferObjectReader*      mReader;
   vpr::SocketStream*      mSocketStream;
};

} // namespace gadget


#endif
