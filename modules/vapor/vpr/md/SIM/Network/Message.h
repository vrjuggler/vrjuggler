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

#include <stdlib.h>
#include <string.h>
#include <vpr/vpr.h>
#include <vpr/Util/Interval.h>

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
   /**
    * Creates a new message by copying the given buffer into freshly allocated
    * memory.
    */
   Message (const void* msg, const vpr::Uint32 msg_size)
      : mMsg(NULL), mMsgSize(msg_size), mSrcSock(NULL), mDestSock(NULL)
   {
      mMsg = malloc(msg_size);
      memcpy(mMsg, msg, msg_size);
   }

   /**
    * Copy constructor that makes a deep copy of the given source object.  That
    * is, new memory is allocated for this object's copy of the message body,
    * and the message is copied into that memory.  As a result, this could be
    * a very expensive operation in terms of time and space.
    */
   Message (const Message& msg)
      : mMsg(NULL), mMsgSize(msg.mMsgSize), mStartOnWire(msg.mStartOnWire),
        mFullyOnWire(msg.mFullyOnWire), mArrivesFully(msg.mArrivesFully),
        mMsgPath(msg.mMsgPath), mNextHop(msg.mNextHop), mSrcSock(msg.mSrcSock),
        mDestSock(msg.mDestSock)
   {
      if ( msg.mMsg != NULL && msg.mMsgSize > 0 )
      {
         // Yikes, this could get expensive!
         mMsg = malloc(msg.mMsgSize);
         memcpy(mMsg, msg.mMsg, msg.mMsgSize);
      }
   }

   /**
    * Releases the memory allocated for the contained message body.
    */
   ~Message (void)
   {
      if ( mMsg != NULL )
      {
         free(mMsg);
      }
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
      return mMsg;
   }

   vpr::Uint32 getSize (void) const
   {
      return mMsgSize;
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

   const vpr::SocketImplSIM* getDestinationSocket (void) const
   {
      return mDestSock;
   }

   /**
    * Retrieves the next hop (network graph vertex) in the path of this
    * message and returns it to the caller via the by-reference parameter.
    * The pointer to the next hop is incremented as a side effect of this call
    * if the parameter increment_next_hop is true.
    *
    * @post The pointer to the next hop in the path is incremented if
    *       increment_next_hop is true.
    *
    * @param hop                Storage for the vertex that is the next hop in
    *                           this message's path.
    * @param increment_next_hop A Boolean value stating whether to increment
    *                           the pointer to the next hop in this message's
    *                           path.  It is optional, and it defaults to true.
    */
   vpr::ReturnStatus getNextHop(NetworkGraph::net_vertex_t& hop,
                                const bool increment_next_hop = true);

private:
   void*       mMsg;      /**< The body of the message */
   vpr::Uint32 mMsgSize;  /**< The size of the message body */

   vpr::Interval mStartOnWire;   /**< The time at which the transmission of
                                      the first bit of this message can begin */
   vpr::Interval mFullyOnWire;   /**< The time at which the last bit of this
                                      message is put on the wire */
   vpr::Interval mArrivesFully;  /**< The time at which the full message
                                      arrives at the destination */

   NetworkGraph::VertexListPtr mMsgPath;
   NetworkGraph::VertexList::iterator mNextHop;
   const vpr::SocketImplSIM* mSrcSock;
   vpr::SocketImplSIM* mDestSock;
};

} // End of sim namespace

} // End of vpr namespace


#endif /* _VPR_SIM_MESSAGE_H_ */
