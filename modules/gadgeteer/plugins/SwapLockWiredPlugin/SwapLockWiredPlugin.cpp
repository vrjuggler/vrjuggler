/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <cluster/PluginConfig.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <boost/concept_check.hpp>

#include <vpr/Thread/Thread.h>

#include <vpr/Util/ReturnStatus.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Port/SerialPort.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>
#include <cluster/ClusterDepChecker.h>

#include <cluster/ClusterManager.h>
#include <cluster/ClusterNetwork.h>
#include <gadget/Node.h>

#include <cluster/Packets/PacketFactory.h>
#include <cluster/Packets/SyncRequest.h>
#include <cluster/Packets/SyncAck.h>

#include <plugins/SwapLockWiredPlugin/SwapLockWiredPlugin.h> // my header...

extern "C"
{
   GADGET_CLUSTER_PLUGIN_EXPORT(vpr::Uint32) getGadgeteerVersion()
   {
      return __GADGET_version;
   }

   GADGET_CLUSTER_PLUGIN_EXPORT(void) initPlugin(cluster::ClusterManager* mgr)
   {
      mgr->addPlugin(new cluster::SwapLockWiredPlugin());
   }
}

namespace cluster
{
   /** Add the pending element to the configuration.
   *  PRE: configCanHandle (element) == true.
   *  @return true iff element was successfully added to configuration.
   */
   bool SwapLockWiredPlugin::configAdd(jccl::ConfigElementPtr element)
   {
      // We need to make sure that the ClusterManager has been configured so that we
      // can ensure that the barrier master's config element has also been configured.
      if(!ClusterManager::instance()->isClusterActive())
      {
         // XXX: This could be made into a dependancy also.
         return false;
      }

      /////////////////////////////////////////
      //  SwapLock Barrier Stuff
      //
      // -Set flag we have started configuring the cluster
      // -Get Sync Machine Element Name
      // -Get ElementPtr to this element
      // -Get the Hostname of this node

      std::string barrier_machine_element_name = element->getProperty<std::string>(std::string("sync_server"));
      jccl::ConfigElementPtr barrier_machine_element = ClusterManager::instance()->getConfigElementPointer(barrier_machine_element_name);
      vprASSERT(NULL != barrier_machine_element.get() && "SwapLockWiredPlugin element must have a sync_server.");
      mBarrierMasterHostname = barrier_machine_element->getProperty<std::string>(std::string("host_name"));
      
      //mTCPport = element->getProperty<int>(std::string("listen_port"));

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[SwapLockWiredPlugin] ")
         << "SwapLock Master Element Name is: " << barrier_machine_element_name << std::endl << vprDEBUG_FLUSH;         
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[SwapLockWiredPlugin] ")
         << "SwapLock Master Hostname is: " << mBarrierMasterHostname << std::endl << vprDEBUG_FLUSH;         
      // Starting Barrier Stuff
      /////////////////////////////////////         
     

      bool slave_one = element->getProperty<bool>(std::string("slave_one"));
      bool slave_two = element->getProperty<bool>(std::string("slave_two"));
      bool slave_three = element->getProperty<bool>(std::string("slave_three"));
      bool slave_four = element->getProperty<bool>(std::string("slave_four"));
      bool slave_five = element->getProperty<bool>(std::string("slave_five"));

      mMasterWaitByte = 0x0;

      if (slave_one)
      {
         mMasterWaitByte |= 0x8;
      }
      if (slave_two)
      {
         mMasterWaitByte |= 0x10;
      }
      if (slave_three)
      {
         mMasterWaitByte |= 0x20;
      }
      if (slave_four)
      {
         mMasterWaitByte |= 0x40;
      }
      if (slave_five)
      {
         mMasterWaitByte |= 0x80;
      }
      
      if(ClusterNetwork::isLocalHost(mBarrierMasterHostname))
      {
         mIsMaster = true;
         mActive = true;
      }
      else
      {
         mIsMaster = false;
         mActive = true;
      }
      Init();
      return true;
   }
   
   /** Remove the pending element from the current configuration.
   *  PRE: configCanHandle (element) == true.
   *  @return true iff the element (and any objects it represented)
   *          were successfully removed.
   */
   bool SwapLockWiredPlugin::configRemove(jccl::ConfigElementPtr element)
   {
      if (recognizeSwapLockWiredPluginConfig(element))
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SwapLockWiredPlugin] Disabling SwapLock: " << element->getName() 
         << "\n" << vprDEBUG_FLUSH;
         return(true);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SwapLockWiredPlugin::configRemove] ERROR, Something is seriously wrong, we should never get here\n"
         << vprDEBUG_FLUSH;
         return(false);
      }
   }
   
   /** Checks if this handler can process element.
   *  Typically, an implementation of handler will check the element's
   *  description name/token to decide if it knows how to deal with
   *  it.
   *  @return true iff this handler can process element.
   */
   bool SwapLockWiredPlugin::configCanHandle(jccl::ConfigElementPtr element)
   {
      return( recognizeSwapLockWiredPluginConfig(element) );   
   }
      
   bool SwapLockWiredPlugin::recognizeSwapLockWiredPluginConfig(jccl::ConfigElementPtr element)
   {
      return(element->getID() == getElementType());
   }

   void SwapLockWiredPlugin::preDraw()
   {;
   }
   void SwapLockWiredPlugin::postPostFrame()
   {;
   }

   void SwapLockWiredPlugin::handlePacket(Packet* packet, gadget::Node* node)
   {
      boost::ignore_unused_variable_warning(packet);
      boost::ignore_unused_variable_warning(node);
   }     

   bool SwapLockWiredPlugin::isPluginReady()
   {
      // We could do some sort of signal here I guess?
      return (true);
   }
   bool SwapLockWiredPlugin::createBarrier()
   {
      // If Slave
      // - send ready to Master
      // - read ready from Server
      // Else if Master
      // - for each slave
      //   - read ready
      // - for each slave
      //   - send ready

      //vpr::Interval first_time, second_time;
      //first_time.setNow();

      if (mIsMaster)
      {
	   //std::cout << "Master-Wait-Signal-Up: " << (int)mMasterWaitByte << std::endl;
	 waitSignalUp(mMasterWaitByte);
	   //std::cout << "Master-Send-Signal-Up" << std::endl;
	 sendSignal(255);
	   //std::cout << "Master-Wait-Signal-Down" << std::endl;
	 waitSignalDown(mMasterWaitByte);
	   //std::cout << "Master-Send-Signal-Down" << std::endl;
	 sendSignal(0);
      }
      else
      {
	   //std::cout << "Slave-Send-Signal-Up" << std::endl;
	 sendSignal(255);
	   //std::cout << "Slave-Wait-Signal-Up" << std::endl;
	 waitSignalUp(0x8);
	   //std::cout << "Slave-Send-Signal-Down" << std::endl;
	 sendSignal(0);
	   //std::cout << "Slave-Wait-Signal-Down" << std::endl;
         waitSignalDown(0x8); 	 
      }

      //second_time.setNow();
      //vpr::Interval diff_time(second_time-first_time);
      //vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrSetBOLD(clrCYAN) << "Latency: " 
      //   << diff_time.getBaseVal() << " usecs\n"<< clrRESET << vprDEBUG_FLUSH;

      return(true);
   }

   vpr::ReturnStatus SwapLockWiredPlugin::Init()
   {
	   vpr::ReturnStatus status;
	   status = ConnectToWiredParallel(); 
	   while(status != vpr::ReturnStatus::Succeed)
	   {
	      status = ConnectToWiredParallel();
	      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "SwapLockWiredPlugin::ConnectToWiredParallel() Barrier could not be initialized, FileHandle = " 
		      << mWire << "\n" << vprDEBUG_FLUSH;
	      
	   }
	   mActive = true;
	   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "SwapLockWiredPlugin::ConnectToWiredParallel() Barrier has been Init() is success!" << vprDEBUG_FLUSH;
	   return(status); 
   }

   vpr::ReturnStatus SwapLockWiredPlugin::ConnectToWiredParallel()
   {
      mWire = open("/dev/wiredparallel",O_RDWR);      
      if (mWire != -1)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
		 << clrOutBOLD(clrGREEN,"SwapLockWiredPlugin::ConnectToWiredParallel() Successfully connected to /dev/wiredParallel\n") 
		 << vprDEBUG_FLUSH;
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
		 << clrOutBOLD(clrRED,"SwapLockWiredPlugin::ConnectToWiredParallel() ERROR: Could not connect to /dev/wiredparallel\n") 
		 << vprDEBUG_FLUSH;
         return(vpr::ReturnStatus::Fail);
      }      
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "SwapLockWiredPlugin::ConnectToWiredParallel() Setting initial value to 0\n" << vprDEBUG_FLUSH;
      
      // char send_char = 0;
      // Keep in mind that the hardware will invert pin 15 on the recveing side, so writeing 255 means to go low.
      // write(mWire,&send_char,1);

      // Clear the local output bits
      std::cout << "Sending low" << std::endl;
      sendSignal((unsigned char)0);
      return(vpr::ReturnStatus::Succeed);
   }
   /*
     // MasterReceive should act identical to SlaveSend since we have no concept of Master/Server with this Sync Method
   void SwapLockWiredPlugin::masterReceive()
   {
      vprASSERT(mActive==true && "Barrier is not active!");
          
      char buf;
      
      read(mWire,&buf,1);
//      std::cout << "masterRecv: " << (int)buf << std::endl;
      buf=buf&0x8;//20
//      std::cout << "masterRecv(after): " << (int)buf << std::endl;
      while((int)buf == 0)
      {
          read(mWire,&buf,1);
//          std::cout << "masterRecv: " << (int)buf << std::endl;
          buf=buf&0x8;//20
//     	  std::cout << "masterRecv(after): " << (int)buf << std::endl;
      }
      
  }
     // MasterSend should act identical to MasterReceive since we have no concept of Master/Server with this Sync Method
   void SwapLockWiredPlugin::masterSend()
   {
      char send_char = 255; 
      write(mWire,&send_char,1);

      char buf;
      
      read(mWire,&buf,1);
//      std::cout << "masterSend: " << (int)buf << std::endl;
      buf=buf&0x8;//20
//      std::cout << "masterSend(after): " << (int)buf << std::endl;
      while((int)buf != 0)
      {
          read(mWire,&buf,1);
//          std::cout << "masterSend: " << (int)buf << std::endl;
          buf=buf&0x8;//20
//          std::cout << "masterSend(after): " << (int)buf << std::endl;
      }
      char send_char2 = 0; 
      write(mWire,&send_char2,1);
   }
   void SwapLockWiredPlugin::slaveSend()
   {
      static int count = 0;
      count++;
      
      vprASSERT(mActive==true && "Barrier is not active!");
      // Send Signal stating that we are at the barrier waiting
      // Keep in mind that the hardware will invert pin 15 on the recveing side, so writeing 0 means to go high.
      char send_char = 255;
      write(mWire,&send_char,1);
      std::cout << "Sent: " << count << std::endl;
   }
   void SwapLockWiredPlugin::slaveReceive()
   {
      // - Wait for signal
      // - Set the local signal wire low again
      static int count = 0;
      count++;
      
      vprASSERT(mActive==true && "Barrier is not active!");
          
      char buf;
      
        // Mask the value in the Parallel Port register to care 
        // only are about bit 4, which has the decimal value of 8
      std::cout << "Waiting: " << count << std::endl;
      read(mWire,&buf,1);
      buf=buf&0x8;
      while((int)buf == 0)
      {
          read(mWire,&buf,1);
          buf=buf&0x8;
      }
      
      std::cout << "Got: " << count << std::endl;
      // vpr::System::msleep(100); 
      // Set the local signal wire low again   
      char send_char = 0; 
      // Keep in mind that the hardware will invert pin 15 on the recveing side, so writeing 255 means to go low.
      write(mWire,&send_char,1);
      std::cout << "Reset: " << count << std::endl;
   }
   */
	   
   void SwapLockWiredPlugin::sendSignal(const unsigned char& val)
   {
      // static int count = 0;
      // count++;
      
      vprASSERT(mActive==true && "Barrier is not active!");
      // Send Signal stating that we are at the barrier waiting
      // Keep in mind that the hardware will invert pin 15 on the recveing side, so writeing 0 means to go high.
      // char send_char = 255;
      write(mWire,&val,1);
      // std::cout << "Sent: " << count << std::endl;
   }
 
   void SwapLockWiredPlugin::waitSignalUp(const unsigned char& val)
   {
      // - wait for signal
      // - set the local signal wire low again
      // static int count = 0;
      // count++;
      
      vprASSERT(mActive==true && "barrier is not active!");
          
      unsigned char buf;
      
        // mask the value in the parallel port register to care 
        // only are about bit 4, which has the decimal value of 8
      // std::cout << "waiting: " << count << std::endl;
      read(mWire,&buf,1);
      buf=buf&val;
      while(buf != val)
      {
          read(mWire,&buf,1);
          buf=buf&val;
      }
      
      // std::cout << "got: " << count << std::endl;
      // vpr::system::msleep(100); 
      // set the local signal wire low again   
      // char send_char = 0; 
      // keep in mind that the hardware will invert pin 15 on the recveing side, so writeing 255 means to go low.
      // write(mwire,&send_char,1);
      // std::cout << "reset: " << count << std::endl;
   }  
 
   void SwapLockWiredPlugin::waitSignalDown(const unsigned char& val)
   {
      vprASSERT(mActive==true && "barrier is not active!");
          
      unsigned char buf;
      
        // mask the value in the parallel port register to care 
        // only are about bit 4, which has the decimal value of 8
      read(mWire,&buf,1);
      buf=buf&val;
      while((int)buf != 0)
      {
          read(mWire,&buf,1);
          buf=buf&val;
      }
  }
 
} // End of cluster namespace
