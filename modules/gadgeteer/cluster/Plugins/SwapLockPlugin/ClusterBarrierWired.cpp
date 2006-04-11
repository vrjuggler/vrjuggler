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

#include <vpr/vprConfig.h>
#include <vpr/System.h>

#include <cluster/Plugins/SwapLockPlugin/ClusterBarrier.h>

#include <gadget/Util/Debug.h>

#include <vector>

#include <boost/static_assert.hpp>

#include <vpr/Util/ReturnStatus.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Port/SerialPort.h>

namespace cluster
{
   ClusterBarrierWired::ClusterBarrierWired()
   {
      mWire = -1;
   }
   ClusterBarrierWired::~ClusterBarrierWired()
   {
   }
   vpr::ReturnStatus ClusterBarrierWired::Init()
   {
	   vpr::ReturnStatus status;
	   status = ConnectToWiredParallel(); 
	   while(status != vpr::ReturnStatus::Succeed)
	   {
	      status = ConnectToWiredParallel();
	      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Barrier could not be initialized, FileHandle = " 
		      << mWire << "\n" << vprDEBUG_FLUSH;
	      
	   }
	   mActive = true;
	   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Barrier has been Init() is success!" << vprDEBUG_FLUSH;
	   return(status); 
   }

   
   void ClusterBarrierWired::AddBarrierSlave(vpr::SocketStream* sock_stream)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "ClusterBarrierTCP: Adding Barrier Slave\n"<< vprDEBUG_FLUSH;
   }
   
   vpr::ReturnStatus ClusterBarrierWired::ConnectToWiredParallel()
   {
      mWire = open("/dev/wiredparallel",O_RDWR);      
      if (mWire != -1)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
		 << clrOutBOLD(clrGREEN,"ClusterBarrrier::ConnectToWiredParallel() Successfully connected to /dev/wiredParallel\n") 
		 << vprDEBUG_FLUSH;
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
		 << clrOutBOLD(clrRED,"ClusterBarrier::ConnectToWiredParallel() ERROR: Could not connect to /dev/wiredparallel\n") 
		 << vprDEBUG_FLUSH;
         return(vpr::ReturnStatus::Fail);
      }      
      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "ClusterBarrier::ConnecttoWiredParallel() Setting initial value to 0\n" << vprDEBUG_FLUSH;
      
      char send_char = 0; 
      write(mWire,&send_char,1);
      return(vpr::ReturnStatus::Succeed);
   }
     // MasterReceive should act identical to SlaveSend since we have no concept of Master/Server with this Sync Method
   void ClusterBarrierWired::MasterReceive()
   {
      SlaveSend();
   }
     // MasterSend should act identical to MasterReceive since we have no concept of Master/Server with this Sync Method
   void ClusterBarrierWired::MasterSend()
   {
      SlaveReceive();
   }
   void ClusterBarrierWired::SlaveSend()
   {
      vprASSERT(mSyncServer!=NULL && "mSyncServer is NULL!");
      vprASSERT(mActive==true && "Barrier is not active!");
      // Send Signal tating that we are at the barrier waiting 
      char send_char = 255; 
      write(mWire,&send_char,1);
   }
   void ClusterBarrierWired::SlaveReceive()
   {
      // - Wait for signal
      // - Set the local signal wire low again
      
      vprASSERT(mSyncServer!=NULL && "mSyncServer is NULL!");
      vprASSERT(mActive==true && "Barrier is not active!");
          
      char buf;
      
        // Mask the value in the Parallel Port register to care 
        // only are about bit 4, which has the decimal value of 8
      read(mWire,&buf,1);
      buf=buf&0x8;
      while((int)buf != 0)
      {
          read(mWire,&buf,1);
          buf=buf&0x8;
      }

      // Set the local signal wire low again   
      char send_char = 0; 
      write(mWire,&send_char,1);
   }
};
