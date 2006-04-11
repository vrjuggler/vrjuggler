/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>
#include <gadget/Util/Debug.h>

#include <vector>

#include <boost/static_assert.hpp>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/BufferObjectReader.h>
#include <cluster/ClusterDelta.h>
#include <gmtl/Math.h>


namespace cluster
{

   ClusterDelta::ClusterDelta() : syncPacket(12)
   {
      mTol = 2;
      mAccept = false;
      mReader = new vpr::BufferObjectReader(&syncPacket);
   }

   void ClusterDelta::clientClusterDelta(vpr::SocketStream* socket_stream)
   {
      mSocketStream = socket_stream;
      while ( mAccept == false )
      {
         clearIntervals();
         receiveHandshake();
         createHandshake();
         receiveExpectedTime();
         createResponce();
      }
   }
   vpr::Interval ClusterDelta::getClusterDelta(vpr::SocketStream* socket_stream)
   {
      mSocketStream = socket_stream;
      while ( mAccept == false )
      {
         clearIntervals();
         createHandshake();
         receiveHandshake();
         createExpectedTime();
         receiveResponce();
      }
      return(mDelta);
   }

   // Sent on initital handshake
   void ClusterDelta::createHandshake()
   {
      mLocalSendTime.setNow();
         // If this is not the first handshake, then actually send the last receive time.
      //if ( mLocalReceiveTime.getBaseVal() != 0 )
      //{
      //   mWriter.writeUint64(mLocalReceiveTime.getBaseVal());
      //               vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[SYNC] Send Handshake: " << mLocalReceiveTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      //}
      //else
      //{
         mWriter.writeUint64(mLocalSendTime.getBaseVal());
                     vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[SYNC] Send Handshake: " << mLocalSendTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      //}
      sendAndClear();
   }
   void ClusterDelta::receiveHandshake()
   {
      getPacket(8);
      mLocalReceiveTime.setNow();
      vpr::Uint64 temp =  mReader->readUint64();
                     vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[SYNC] Received Time: " << temp << "\n" << vprDEBUG_FLUSH;
      mRemoteSendTime.set(temp, vpr::Interval::Base);
                     vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[SYNC] Received Time: " << mRemoteSendTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
   }
   void ClusterDelta::createExpectedTime()
   {
                     vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[SYNC] Calculate Expected Time: " << mRemoteSendTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      mLatencyTime.set( (mLocalReceiveTime.getBaseVal()-mLocalSendTime.getBaseVal())/2, vpr::Interval::Base);
                     vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[SYNC]     Latency: " << mLatencyTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;

      mDelta=(mRemoteSendTime-mLocalSendTime-mLatencyTime);

      std::cout << "   " << mRemoteSendTime.getBaseVal() << std::endl;
      std::cout << " - " << mRemoteSendTime.getBaseVal() << std::endl;
      std::cout << " - " << mRemoteSendTime.getBaseVal() << std::endl;
      std::cout << " = " << mRemoteSendTime.getBaseVal() - mRemoteSendTime.getBaseVal() - mRemoteSendTime.getBaseVal() << std::endl;

      vpr::Int64 mNewDelta = mRemoteSendTime.getBaseVal() - mRemoteSendTime.getBaseVal() - mRemoteSendTime.getBaseVal();

      std::cout << "New Delta: " << mNewDelta << std::endl;

      std::cout << "mDelta" << mDelta.getBaseVal() << std::endl;

                     vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[SYNC]     Delta: " << mDelta.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      mLocalSendTime.setNow();
      mExpectedRemoteTime = mLocalSendTime + mDelta + mLatencyTime;
                     vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[SYNC]     Sent Expected Time: " << mExpectedRemoteTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      mWriter.writeUint64(mExpectedRemoteTime.getBaseVal());
      sendAndClear();
   }
   void ClusterDelta::receiveExpectedTime()
   {

      getPacket(8);
      mLocalReceiveTime.setNow();
                     vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[SYNC]     Got Expected Time at: " << mLocalReceiveTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      mExpectedRemoteTime.set(mReader->readUint64(), vpr::Interval::Base);
                     vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[SYNC]     Got Expected Time: " << mExpectedRemoteTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      if (mExpectedRemoteTime > mLocalReceiveTime)
      {
         mErrorTime = mExpectedRemoteTime - mLocalReceiveTime;
      }
      else
      {
         mErrorTime = mLocalReceiveTime - mExpectedRemoteTime;
      }

                     vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[SYNC]     Error of: " << mErrorTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;

      if ( mErrorTime.getBaseVal() < mTol )
      {
         mAccept = true;
      }
      else
      {
         mAccept = false;
         mTol = gmtl::Math::pow(mTol,(float)1.5);
      }
   }
   void ClusterDelta::createResponce()
   {
                     vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[SYNC] Createing Responce" << "\n" << vprDEBUG_FLUSH;
                     vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[SYNC]     mTol: " << mTol << " Accept?: " << mAccept << "\n" << vprDEBUG_FLUSH;
      mWriter.writeBool(mAccept);
      sendAndClear();
   }

   void ClusterDelta::receiveResponce()
   {
      getPacket(1);
      mAccept = mReader->readBool();
      if ( mAccept == false )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutNORM(clrRED,"[SYNC]FAILED SYNC") << "\n" << vprDEBUG_FLUSH;
      }
   }

   void ClusterDelta::sendAndClear()
   {
      vpr::Uint32 bytes_just_sent = 0;
      if ( !mWriter.getData()->empty() )
      {
         mSocketStream->send(*(mWriter.getData()),mWriter.getData()->size(),bytes_just_sent);
      }
      mWriter.mData->clear();
      mWriter.mCurHeadPos = 0;
   }
   void ClusterDelta::getPacket(unsigned num)
   {                                      // Need to delete the old Object Readers
      vpr::Uint32 bytes_read;
      syncPacket.clear();
      mReader->setCurPos(0);
      //vpr::ReturnStatus status = mSocketStream->readn(syncPacket,SYNC_PACKET_LENGTH,bytes_read);
      mSocketStream->readn(syncPacket,num ,bytes_read);
   }
   void ClusterDelta::clearIntervals()
   {
      mLocalSendTime.set(0,vpr::Interval::Base);
      mLocalReceiveTime.set(0,vpr::Interval::Base);
      mRemoteSendTime.set(0,vpr::Interval::Base);
      mRemoteReceiveTime.set(0,vpr::Interval::Base);
      mExpectedRemoteTime.set(0,vpr::Interval::Base);
      mLatencyTime.set(0,vpr::Interval::Base);
      mDelta.set(0,vpr::Interval::Base);
      mErrorTime.set(0,vpr::Interval::Base);
   }

} // End of gadget namespace
