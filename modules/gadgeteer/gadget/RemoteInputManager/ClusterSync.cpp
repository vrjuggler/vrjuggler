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
      //while ( mAccept == false )
      //{
      //   recieveHandshake();
      //   createHandshake();
      //   recieveExpectedTime();
      //   createResponce();
      //   clearIntervals();
      //}     
   }
   vpr::Interval ClusterSync::getClusterSync(vpr::SocketStream* socket_stream)
   {
      mSocketStream = socket_stream;
      //while ( mAccept == false )
      //{
      //   createHandshake();
      //   recieveHandshake();
      //   createExpectedTime();
      //   recieveResponce();
      //   clearIntervals();
      //}
      return(mDelta);
   }

   // Sent on initital handshake
   /*inline*/ void ClusterSync::createHandshake()
   {
      mLocalSendTime.setNow();
      if ( mLocalReceiveTime.getBaseVal() != 0 )//					Makes a little more accurate
      {
         mWriter.writeUint64(mLocalReceiveTime.getBaseVal());
      }
      else
      {
         mWriter.writeUint64(mLocalSendTime.getBaseVal());
      }
      sendAndClear();
   }
   /*inline*/ void ClusterSync::recieveHandshake()
   {
      getPacket(8);
      mLocalReceiveTime.setNow();
      mRemoteSendTime.set(mReader->readUint64(), vpr::Interval::Base);
   }
   /*inline*/ void ClusterSync::createExpectedTime()
   {
      vprDEBUG(gadgetDBG_RIM,1) << "[SYNC] Calculate Expected Time: " << mRemoteSendTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      mLatencyTime.set( (mLocalReceiveTime.getBaseVal()-mLocalSendTime.getBaseVal())/2, vpr::Interval::Base);
      vprDEBUG(gadgetDBG_RIM,1) << "[SYNC] 	Latency: " << mLatencyTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      mDelta=(mRemoteSendTime-mLocalSendTime-mLatencyTime);
      vprDEBUG(gadgetDBG_RIM,1) << "[SYNC] 	Delta: " << mDelta.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      mLocalSendTime.setNow();
      mExpectedRemoteTime = mLocalSendTime + mDelta + mLatencyTime;     
      vprDEBUG(gadgetDBG_RIM,1) << "[SYNC] 	Sent Expected Time: " << mExpectedRemoteTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      //mWriter.writeUint16(RIM_PACKET_MSG);
      //mWriter.writeUint16(MSG_SYNC_EXP_TIME);
      //mWriter.writeUint32(16);			//Length
      mWriter.writeUint64(mExpectedRemoteTime.getBaseVal());
      sendAndClear();
   }
   /*inline*/ void ClusterSync::recieveExpectedTime()
   {
      vprDEBUG(gadgetDBG_RIM,1) << "[SYNC] Recieve Expected Time: " << mRemoteSendTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      getPacket(8);
      mLocalReceiveTime.setNow();
      vprDEBUG(gadgetDBG_RIM,1) << "[SYNC] 	Got Expected Time at: " << mLocalReceiveTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      //vpr::Uint16 temp = mReader->readUint16();
      //temp = mReader->readUint16();
      //vpr::Uint32 length = mReader->readUint32();
      mRemoteSendTime.set(mReader->readUint64(), vpr::Interval::Base);
      vprDEBUG(gadgetDBG_RIM,1) << "[SYNC] 	Got Expected Time: " << mRemoteSendTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;
      mErrorTime = mRemoteSendTime - mLocalReceiveTime;
      vprDEBUG(gadgetDBG_RIM,1) << "[SYNC] 	Error of: " << mErrorTime.getBaseVal() << "\n" << vprDEBUG_FLUSH;

      if ( mErrorTime.getBaseVal() < 0 )
      {
         mErrorTime = mLocalReceiveTime - mRemoteSendTime;
      }
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
      vprDEBUG(gadgetDBG_RIM,1) << "[SYNC] Createing Responce" << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,1) << "[SYNC] 	mTol: " << mTol << " Accept?: " << mAccept << "\n" << vprDEBUG_FLUSH;
      mWriter.writeBool(mAccept);
      sendAndClear();
   }

   /*inline*/ void ClusterSync::recieveResponce()
   {
      getPacket(1);
      mAccept = mReader->readBool();
      if ( mAccept == false )
      {
         vprDEBUG(gadgetDBG_RIM,1) << clrOutNORM(clrRED,"[SYNC]FAILED SYNC") << "\n" << vprDEBUG_FLUSH;
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
