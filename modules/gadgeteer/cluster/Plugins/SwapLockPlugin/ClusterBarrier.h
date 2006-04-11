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

#ifndef _GADGET_CLUSTER_BARRIER_H
#define _GADGET_CLUSTER_BARRIER_H

#include <vpr/vprConfig.h>
//#include <vpr/System.h>

#include <vector>

#include <boost/static_assert.hpp>

#include <vpr/Util/ReturnStatus.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Port/SerialPort.h>

/*namespace vpr
{
   class SerialPort;
   class SocketStream;
}*/

namespace cluster
{

class ClusterBarrier
{
public:
   ClusterBarrier()
   {
      mActive = false;
      mIsMaster = false;
   }
   virtual ~ClusterBarrier()
   {
      delete mSyncServer;
   }
   virtual void AddBarrierSlave(vpr::SocketStream* sock_stream) = 0;
   virtual void MasterSend() = 0;
   virtual void MasterReceive() = 0;
   virtual void SlaveSend() = 0;
   virtual void SlaveReceive() = 0;
   virtual vpr::ReturnStatus Init() = 0;

   void setHostname(std::string host_name)
   {
      mHostname = host_name;
   }
   void setTCPPort(int tcp_port)
   {
      mTCPport = tcp_port;
   }
   void setSerialPort(std::string port)
   {
      mSerialPortName = port;
   }
   void setBaudRate(int baud)
   {
      mBaudRate = baud;
   }
   void setMaster(bool master)
   {
      mIsMaster = master;
   }
   bool isActive()
   {
      return(mActive);
   }
   bool isMaster()
   {
      return(mIsMaster);
   }

protected:
   vpr::SocketStream*               mSyncServer;
   std::vector<vpr::SocketStream*>  mSyncClients;
   bool mActive;

   std::string          mHostname;
   vpr::Uint16          mTCPport;
   std::string          mSerialPortName;
   int                  mBaudRate;
   bool                 mIsMaster;
};

class ClusterBarrierTCP : public ClusterBarrier
{
public:
   ClusterBarrierTCP() : mAcceptThread(NULL)
   {
   }
   virtual ~ClusterBarrierTCP()
   {
      
   }

   virtual vpr::ReturnStatus Init()
   {
      vpr::ReturnStatus status;
      if (mIsMaster)
      {
         startListening();
         mActive = true;
         return(vpr::ReturnStatus::Succeed);
      }
      else
      {
         status = ConnectToMasterSocket();
         if (status.success())
         {
            mActive = true;
         }
         return(status);
      }
   }
   virtual void AddBarrierSlave(vpr::SocketStream* sock_stream);
   vpr::ReturnStatus ConnectToMasterSocket();
   virtual void MasterSend();
   virtual void MasterReceive();
   virtual void SlaveSend();
   virtual void SlaveReceive();

   bool startListening();
   void acceptLoop(void* nullParam);

private:
   vpr::Thread*   mAcceptThread;       /**< Thread that listens for incoming connections. */
   vpr::InetAddr  mListenAddr;         /**< Address to listen for incoming connections on. */

};

class ClusterBarrierSerial : public ClusterBarrier
{
public:
   ClusterBarrierSerial();
   virtual ~ClusterBarrierSerial();
   virtual vpr::ReturnStatus Init();

   virtual void AddBarrierSlave(vpr::SocketStream* sock_stream);
   vpr::ReturnStatus ConnectToMasterSocket();
   vpr::ReturnStatus ConnectToMasterSerial();

   virtual void MasterSend();
   virtual void MasterReceive();
   virtual void SlaveSend();
   virtual void SlaveReceive();
   
private:
   vpr::SerialPort*     mSerialPort;
};

class ClusterBarrierWired : public ClusterBarrier
{
public:
   ClusterBarrierWired();
   virtual ~ClusterBarrierWired();
   virtual vpr::ReturnStatus Init();

   virtual void AddBarrierSlave(vpr::SocketStream* sock_stream);
   vpr::ReturnStatus ConnectToWiredParallel();

   virtual void MasterSend();
   virtual void MasterReceive();
   virtual void SlaveSend();
   virtual void SlaveReceive();
   
private:
   int mWire;
};

}; // namespace gadget


#endif
