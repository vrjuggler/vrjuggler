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

#ifndef _VPR_SIM_MESSAGE_H_
#define _VPR_SIM_MESSAGE_H_

#include <vpr/vprConfig.h>
#include <vpr/vprTypes.h>
#include <vector>
#include <boost/smart_ptr.hpp>

#include <stdlib.h>
#include <string.h>
#include <vpr/vpr.h>
#include <vpr/Util/Interval.h>
#include <vpr/Util/Assert.h>

#include <vpr/md/SIM/Network/NetworkGraph.h>


namespace vpr
{

namespace sim
{

class vpr::SocketImplSIM;

/**
 * Container class used to transmit messages between simulated network nodes.
 * Copying these objects is an expensive operation, and hence, it should be
 * done as infrequently as possible.  Passing by reference may require access
 * synchronization, however.
 */
class VPR_CLASS_API Message
{
public:
   typedef std::vector<vpr::Uint8> MessageDataType;
   typedef boost::shared_ptr<std::vector<vpr::Uint8> > MessageDataPtr;

   /**
    * Creates a new message by copying the given buffer into freshly allocated
    * memory.
    */
   Message (const void* msg, const vpr::Uint32 msg_size)
      : mSrcSock(NULL), mDestSock(NULL)
   {
      vpr::Uint8* start = (vpr::Uint8*)msg;
      mMsg = MessageDataPtr( new MessageDataType(start, start+msg_size));
      //mMsg = malloc(msg_size);
      //memcpy(mMsg, msg, msg_size);
   }

   /** Creates message by copying shared pointer to data buffer */
   Message (MessageDataPtr msgData)
      : mSrcSock(NULL), mDestSock(NULL)
   {
      mMsg = msgData;
   }

   /**
    * Copy constructor that makes a copy of the given source object.  That
    * is, now new memory is allocated for this object's copy of the message body,
    * instead we just make a copy of the shared_ptr to the message body and share
    * it with the other message
    */
   Message (const Message& msg);

   /**
    * Releases the memory allocated for the contained message body.
    */
   ~Message (void)
   {
      // Releases automatically based on shared_ptr semantics
   }

   void setStartOnWireTime (const vpr::Interval& time)
   {
      mStartOnWire = time;
   }

   const vpr::Interval& whenStartOnWire (void) const
   {
      return mStartOnWire;
   }

   void setFullyOnWireTime (const vpr::Interval& time)
   {
      mFullyOnWire = time;
   }

   const vpr::Interval& whenFullyOnWire (void) const
   {
      return mFullyOnWire;
   }

   void setArrivesFullyTime (const vpr::Interval& time)
   {
      mArrivesFully = time;
   }

   const vpr::Interval& whenArrivesFully (void) const
   {
      return mArrivesFully;
   }

   void* getBody (void) const
   {
      return (void*)&((*mMsg)[0]);
   }

   /* Returns a shared copy of the message data */
   MessageDataPtr getMessageData()
   {
      return mMsg;
   }

   vpr::Uint32 getSize (void) const
   {
      return mMsg->size();
   }

   /**
    * Determines if this message needs to be resized after a read.  If
    * the given number of bytes read from the message is less than the
    * actual size, this message is resized to contain only the remaining
    * unread bytes.
    *
    * @pre This message has had some bytes read.
    * @post The message is resized based on <bytes_read>, and the new
    *       size of this message is returned to the caller.
    *
    * @param bytes_read The number of bytes read from this message so far.
    *
    * @return 0 is returned if the message was not resized.  This
    *         indicates that all the bytes were read.<br>
    *         A value greater than 0 is returned if this message has been
    *         resized.
    */
   vpr::Uint32 resize(const vpr::Uint32 bytes_read);

   /**
    * Assigns the path that this message will follow.  The pointer to the next
    * hop in the path is set to the beginning of the path, and any previous
    * path information is lost.
    *
    * @post A new path is assigned for this message, and the pointer to the
    *       next hop is set to the beginning of the given path.
    */
   void setPath (NetworkGraph::VertexListPtr path,
                 const vpr::SocketImplSIM* source, vpr::SocketImplSIM* dest)
   {
      mMsgPath  = path;
      mNextHop  = mMsgPath->begin();
      mSrcSock  = source;
      mDestSock = dest;

      vprASSERT(mNextHop != mMsgPath->end() && "Path must have at least one value");
   }

   /**
    * Returns a constant reference to the path that this message will follow.
    */
   const NetworkGraph::VertexListPtr& getPath (void) const
   {
      return mMsgPath;
   }

   const vpr::SocketImplSIM* getSourceSocket (void) const
   {
      return mSrcSock;
   }

   vpr::SocketImplSIM* getDestinationSocket (void) const
   {
      return mDestSock;
   }

   /**
    * Retrieves the next hop (network graph vertex) in the path of this
    * message and returns it to the caller via the by-reference parameter.
    */
   const NetworkGraph::net_vertex_t& getNextHop (void) const
   {
      return *mNextHop;
   }

   /**
    * Increment the pointer to the next hop in this message's path as it moves
    * towards its destination.
    *
    * @post The pointer to the next hop in the path is incremented if
    *       increment_next_hop is true.
    */
   void incNextHop (bool& end_of_path)
   {
      ++mNextHop;
      end_of_path = (mMsgPath->end() == mNextHop);
   }

private:
   MessageDataPtr   mMsg;    /**< The body of the message */
   //void*       mMsg;      /**< The body of the message */
   //vpr::Uint32 mMsgSize;  /**< The size of the message body */


   vpr::Interval mStartOnWire;   /**< The time at which the transmission of
                                      the first bit of this message can begin */
   vpr::Interval mFullyOnWire;   /**< The time at which the last bit of this
                                      message is put on the wire */
   vpr::Interval mArrivesFully;  /**< The time at which the full message
                                      arrives at the destination */

   NetworkGraph::VertexListPtr         mMsgPath;
   NetworkGraph::VertexList::iterator  mNextHop;
   const vpr::SocketImplSIM*           mSrcSock;
   vpr::SocketImplSIM*                 mDestSock;
};

} // End of sim namespace

} // End of vpr namespace


#endif /* _VPR_SIM_MESSAGE_H_ */
