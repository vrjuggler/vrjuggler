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

#include <vpr/md/SIM/Network/Message.h>


namespace vpr
{

namespace sim
{


Message::Message (const Message& msg)
   : mStartOnWire(msg.mStartOnWire),
     mFullyOnWire(msg.mFullyOnWire), mArrivesFully(msg.mArrivesFully),
     mMsgPath(msg.mMsgPath), mNextHop(msg.mNextHop), mSrcSock(msg.mSrcSock),
     mDestSock(msg.mDestSock)
{
   vprASSERT(false && "In copy constructor");
   mMsg = msg.mMsg;     // Just copy the shared pointer.  VERY cheap

   /*
   if ( (msg.mMsg.get() != NULL) && (!msg.mMsg->empty()) )
   {
      // Yikes, this could get expensive!
      // XXX: Find out if this is REALLY needed
      mMsg = MessageDataPtr( new MessageDataType(msg.mMsg->begin(), msg.mMsg->end()));
      //mMsg = malloc(msg.mMsgSize);
      //memcpy(mMsg, msg.mMsg, msg.mMsgSize);
   }
   */
}

vpr::Uint32 Message::resize (const vpr::Uint32 bytes_read)
{
   vpr::Uint32 resize_amount;

   resize_amount = getSize() - bytes_read;

   // Only resize the message if 0 < bytes_read < mMessageSize.
   if ( resize_amount > 0 && resize_amount != getSize() ) {

      mMsg->erase(mMsg->begin(), mMsg->begin()+bytes_read);
      vprASSERT(resize_amount == mMsg->size());    // Must be this size

      /*
      char* data;
      vpr::Uint32 i;

      data = (char*) mMsg;

      // If there is still stuff left in the buffer, move it over.
      for ( i = 0; i < resize_amount; i++ ) {
         data[i] = data[bytes_read + i];
      }

      data[i]  = '\0';
      mMsgSize = resize_amount;
      */
   }

   return resize_amount;
}

vpr::ReturnStatus Message::getNextHop (NetworkGraph::net_vertex_t& hop,
                                       const bool increment_next_hop)
{
   vpr::ReturnStatus status;

   if ( mMsgPath->size() > 0 && mNextHop != mMsgPath->end() )
   {
      hop = *mNextHop;

      if ( increment_next_hop )
      {
         ++mNextHop;
      }
   }
   else
   {
      status.setCode(vpr::ReturnStatus::Fail);
   }

   return status;
}

} // End of sim namespace

} // End of vpr namespace
