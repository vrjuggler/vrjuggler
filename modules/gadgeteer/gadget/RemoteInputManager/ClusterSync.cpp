/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#include <vpr/vprConfig.h>
#include <vpr/System.h>

#include <vector>

#include <boost/static_assert.hpp>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/ObjectWriter.h>
#include <vpr/IO/ObjectReader.h>
#include <gadget/RemoteInputManager/ClusterSync.h>
#include <gadget/RemoteInputManager/NetUtils.h>
#include <gmtl/Math.h>


namespace gadget
{
   void ClusterSync::clientClusterSync(vpr::SocketStream* socket_stream)
   {
      mSocketStream = socket_stream;
      while ( mAccept == false )
      {
         recieveHandshake();
         createHandshake();
         recieveExpectedTime();
         createResponce();
         clearIntervals();
      }     
   }
   vpr::Interval ClusterSync::getClusterSync(vpr::SocketStream* socket_stream)
   {
      mSocketStream = socket_stream;
      while ( mAccept == false )
      {
         createHandshake();
         recieveHandshake();
         createExpectedTime();
         recieveResponce();
         clearIntervals();
      }
      return(mDelta);
   }

   // Sent on initital handshake
   /*inline*/ void ClusterSync::createHandshake()
   {
      mLocalSendTime.setNow();
         // If this is not the first handshake, then actually send the last receive time.
      if ( mLocalReceiveTime.getBaseVal() != 0 )
      {
         mWriter.writeUint64(mLocalReceiveTime.getBaseVal());
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SYNC] Send Handshake: " << mLocalReceiveTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      }
      else
      {
         mWriter.writeUint64(mLocalSendTime.getBaseVal());
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SYNC] Send Handshake: " << mLocalSendTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      }
      sendAndClear();
   }
   /*inline*/ void ClusterSync::recieveHandshake()
   {
      getPacket(8);
      mLocalReceiveTime.setNow();
      vpr::Uint64 temp =  mReader->readUint64();
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SYNC] Received Time: " << temp << "\n" << vprDEBUG_FLUSH;
      mRemoteSendTime.set(temp, vpr::Interval::Base);
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SYNC] Received Time: " << mRemoteSendTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
   }
   /*inline*/ void ClusterSync::createExpectedTime()
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SYNC] Calculate Expected Time: " << mRemoteSendTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      mLatencyTime.set( (mLocalReceiveTime.getBaseVal()-mLocalSendTime.getBaseVal())/2, vpr::Interval::Base);
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SYNC] 	Latency: " << mLatencyTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      mDelta=(mRemoteSendTime-mLocalSendTime-mLatencyTime);
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SYNC] 	Delta: " << mDelta.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      mLocalSendTime.setNow();
      mExpectedRemoteTime = mLocalSendTime + mDelta + mLatencyTime;     
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SYNC] 	Sent Expected Time: " << mExpectedRemoteTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      //mWriter.writeUint16(RIM_PACKET_MSG);
      //mWriter.writeUint16(MSG_SYNC_EXP_TIME);
      //mWriter.writeUint32(16);   //Length
      mWriter.writeUint64(mExpectedRemoteTime.getBaseVal());
      sendAndClear();
   }
   /*inline*/ void ClusterSync::recieveExpectedTime()
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SYNC] Recieve Expected Time: " << mRemoteSendTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      getPacket(8);
      mLocalReceiveTime.setNow();
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SYNC] 	Got Expected Time at: " << mLocalReceiveTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      //vpr::Uint16 temp = mReader->readUint16();
      //temp = mReader->readUint16();
      //vpr::Uint32 length = mReader->readUint32();
      mRemoteSendTime.set(mReader->readUint64(), vpr::Interval::Base);
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SYNC] 	Got Expected Time: " << mRemoteSendTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      if (mRemoteSendTime > mLocalReceiveTime)
      {
         mErrorTime = mRemoteSendTime - mLocalReceiveTime;
      }
      else
      {
         mErrorTime = mLocalReceiveTime - mRemoteSendTime;
      }
      
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SYNC] 	Error of: " << mErrorTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;

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
   /*inline*/ void ClusterSync::createResponce()
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SYNC] Createing Responce" << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SYNC] 	mTol: " << mTol << " Accept?: " << mAccept << "\n" << vprDEBUG_FLUSH;
      mWriter.writeBool(mAccept);
      sendAndClear();
   }

   /*inline*/ void ClusterSync::recieveResponce()
   {
      getPacket(1);
      mAccept = mReader->readBool();
      if ( mAccept == false )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrRED,"[SYNC]FAILED SYNC") << "\n" << vprDEBUG_FLUSH;
      }
   }

   /*inline*/ void ClusterSync::sendAndClear()
   {
      vpr::Uint32 bytes_just_sent = 0;
      if ( !mWriter.getData()->empty() )
      {
         mSocketStream->send(*(mWriter.getData()),mWriter.getData()->size(),bytes_just_sent);
      }
      mWriter.mData->clear();
      mWriter.mCurHeadPos = 0;
   }
   /*inline*/ void ClusterSync::getPacket(unsigned num)
   {                                      // Need to delete the old Object Readers
      vpr::Uint32 bytes_read;
      syncPacket.clear();
      //vpr::ReturnStatus status = mSocketStream->readn(syncPacket,SYNC_PACKET_LENGTH,bytes_read);
      mSocketStream->readn(syncPacket,num ,bytes_read);
      mReader = new vpr::ObjectReader(&syncPacket);
   }
   /*inline*/ void ClusterSync::clearIntervals()
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
};
