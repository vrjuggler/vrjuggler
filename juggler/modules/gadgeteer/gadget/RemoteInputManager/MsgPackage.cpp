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

#include <gadget/gadgetConfig.h>
#include <jccl/RTRC/ConfigManager.h>
#include <gadget/RemoteInputManager/NetUtils.h>
#include <gadget/Util/Debug.h>
#include <gadget/RemoteInputManager/MsgPackage.h>
#include <gadget/Type/DeviceFactory.h>

namespace gadget
{
   
   MsgPackage::MsgPackage()
   {
      mObjectWriter = new vpr::ObjectWriter;
      mTempWriter = new vpr::ObjectWriter;
   }

   void MsgPackage::sendString(std::string str)
   {
      /*if (str.size() % 4 != 0)
      {
         unsigned temp = (4 - (str.size() % 4));
         mTempWriter->writeUint32(str.size()+ temp);
   
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrRED)
         << "String Length: " << str.size() 
         << " not aligned, fixing it by adding, " << temp << " EL's \n" << clrRESET << vprDEBUG_FLUSH;
   
         mTempWriter->writeString(str);   
   
         char tchar = '\0';
         for (unsigned i=0;i<temp;i++)
         {
            vpr::Uint8 temp2 = (vpr::Uint8)tchar;
            mTempWriter->writeRaw(&temp2);
         }
   
      }
      else
      { */
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrYELLOW) << "SEND str_len: " << str.size() << "\n" << clrRESET << vprDEBUG_FLUSH;
         mTempWriter->writeUint16(str.size());
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrYELLOW) << "SEND string: " << str << "\n" << clrRESET << vprDEBUG_FLUSH;
         mTempWriter->writeString(str);   
      //}
   
   
   }


   void MsgPackage::sendAndClear(vpr::SocketStream* sock_stream)
   {
      if (sock_stream != NULL && sock_stream->isOpen())
      {
         vpr::Uint32 bytes_just_sent = 0;
         if ( !mObjectWriter->getData()->empty() )
         {
            sock_stream->send(*(mObjectWriter->getData()),mObjectWriter->getData()->size(),bytes_just_sent);
         }
         mTempWriter->mData->clear();
         mTempWriter->mCurHeadPos = 0;

         mObjectWriter->mData->clear();
         mObjectWriter->mCurHeadPos = 0;
      }
      else
      {  
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrRED,"ERROR: Can not send packet, connection has been lost. ")
               <<  clrOutBOLD(clrRED,"And NetConnection should be deleted. \n") << vprDEBUG_FLUSH;
         mTempWriter->mData->clear();
         mTempWriter->mCurHeadPos = 0;

         mObjectWriter->mData->clear();
         mObjectWriter->mCurHeadPos = 0;

         //vprASSERT(sock_stream->isOpen() && "Socket no longer open!!");
      }
      

   }

   void MsgPackage::createDeviceRequest(vpr::Uint16 device_id,
                                        const std::string device_name)
   {
      ///////////////////////////////////////////////////////////////////////
      //                                                                   //
      // CONSTRUCT ACK PACKET 											   //				
      //	FORM:		 vpr::Uint16		vpr::Uint16		 vpr::Uint32   //				
      //			 -------------------------------------------------	   //			
      //			| RIM_PACKET_MSG | MSG_DEVICE_REQ |   length     |	   //			
      //			-------------------------------------------------	   //			
      //																   //				
      //			   vpr::Uint16	 	uint    std::string  			   //			
      //			 -------------------------------------  			   //			
      //			| device_id  |	str_len | device_name |  			   //			
      //			-------------------------------------				   //			
      //																   //
      ///////////////////////////////////////////////////////////////////////

      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrCYAN,"[Create Packet]CREATING A DEVICE REQUEST\n") << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Name:      " << device_name << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Remote ID: " << device_id << "\n" << vprDEBUG_FLUSH;
      
      /////////////////
      // CREATE BODY //
      /////////////////

      // Device ID
      mTempWriter->writeUint16(device_id);

      // Device Name
      //mTempWriter->writeUint16(device_name.size());
      //mTempWriter->writeString(device_name);
      sendString(device_name);
      // Set the size of the data
      vpr::Uint32 length = mTempWriter->getData()->size();

      ///////////////////
      // CREATE HEADER //
      ///////////////////
      mObjectWriter->writeUint16(RIM_PACKET_MSG);
      mObjectWriter->writeUint16(MSG_DEVICE_REQ);    
      mObjectWriter->writeUint32(length + RIM_HEAD_LENGTH);
      //std::cout << "		LENGTH OF PACKET " << length + RIM_HEAD_LENGTH << std::endl;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "LENGTH OF PACKET: " << length + RIM_HEAD_LENGTH <<"\n" << vprDEBUG_FLUSH;
      ///////////////////////
      // ADD HEADER & BODY //
      ///////////////////////

      mObjectWriter->getData()->insert(mObjectWriter->getData()->end(),mTempWriter->getData()->begin(),mTempWriter->getData()->end());
      mObjectWriter->setCurPos(length + RIM_HEAD_LENGTH);
      vprDEBUG_END(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrCYAN,"[Create Packet]CREATING A DEVICE REQUEST\n") << vprDEBUG_FLUSH;
      
   }

   
   void MsgPackage::createDeviceAck(const vpr::Uint16 remote_device_id,
                                    const vpr::Uint16 local_device_id,
                                    const std::string device_name,
                                    const std::string base_type)
   {
      /////////////////////////////////////////////////////////////////////
      //																 //
      // CONSTRUCT ACK PACKET 											 //
      //	FORM:		 vpr::Uint16		vpr::Uint16		 vpr::Uint32 //
      //			 -------------------------------------------------	 //
      //			| RIM_PACKET_MSG | MSG_DEVICE_ACK |   length     |   //
      //			-------------------------------------------------	 //
      //																 //
      //				 vpr::Uint16		   vpr::Uint16		  		 //
      //			 ---------------------------------------			 //
      //			| remote_device_id  | local_device_id  |			 //
      //			---------------------------------------				 //
      //																 //
      // 			uint      std::string    uint     std::string		 //
      //			 -----------------------------------------------	 //
      //			|	str_len | device_name | str_len | base_type | 	 //
      //   	        -----------------------------------------------		 //
      /////////////////////////////////////////////////////////////////////


      /////////////////
      // CREATE BODY //
      /////////////////

      //std::cout << "CREATE DEVICE_ACK" << std::endl;
      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrCYAN,"[Create Packet]CREATE DEVICE_ACK\n") << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Name:      " << device_name << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Remote ID: " << remote_device_id << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Local ID:  " << local_device_id << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "BaseType:  " << base_type << "\n" << vprDEBUG_FLUSH;
      // Remote and Local ID's
      mTempWriter->writeUint16(remote_device_id);
      mTempWriter->writeUint16(local_device_id);

      // Device Name
      //mTempWriter->writeUint16(device_name.size());
      //mTempWriter->writeString(device_name);
      sendString(device_name);

      // Base Type
      //mTempWriter->writeUint16(base_type.size());
      //mTempWriter->writeString(base_type);
      sendString(base_type);

      // Set the size of the packet
      vpr::Uint32 length = mTempWriter->getData()->size();



      ///////////////////
      // CREATE HEADER //
      ///////////////////
      mObjectWriter->writeUint16(RIM_PACKET_MSG);
      mObjectWriter->writeUint16(MSG_DEVICE_ACK);    
      mObjectWriter->writeUint32(length + RIM_HEAD_LENGTH);
      //std::cout << "		LENGTH OF PACKET " << length + RIM_HEAD_LENGTH << std::endl;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "LENGTH OF PACKET: " << length + RIM_HEAD_LENGTH <<"\n" << vprDEBUG_FLUSH;
      ///////////////////////
      // ADD HEADER & BODY //
      ///////////////////////

      mObjectWriter->getData()->insert(mObjectWriter->getData()->end(),mTempWriter->getData()->begin(),mTempWriter->getData()->end());
      mObjectWriter->setCurPos(length + RIM_HEAD_LENGTH);
      vprDEBUG_END(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrCYAN,"[Create Packet]CREATE DEVICE_ACK\n") << vprDEBUG_FLUSH;

   }

   void MsgPackage::createDeviceNack(const vpr::Uint16 remote_device_id, const vpr::Uint16 local_device_id, const std::string device_name)
   {
      ///////////////////////////////////////////////////////////////////////
      //																   //
      // CONSTRUCT NACK PACKET 											   //
      //	FORM:		 vpr::Uint16		vpr::Uint16		 vpr::Uint32   //
      //			 -------------------------------------------------	   //
      //			| RIM_PACKET_MSG | MSG_DEVICE_NACK |   length     |	   //
      //			-------------------------------------------------	   //
      //																   //
      //				 vpr::Uint16		   vpr::Uint16		  		   //
      //			 ---------------------------------------			   //
      //			| remote_device_id  | local_device_id  |			   //
      //			---------------------------------------				   //
      //																   //
      // 			uint      std::string    uint     std::string		   //
      //			 -----------------------------------------------	   //
      //			|	str_len | device_name | str_len | base_type | 	   //
      //   	   -----------------------------------------------			   //
      ///////////////////////////////////////////////////////////////////////


      /////////////////
      // CREATE BODY //
      /////////////////
      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrCYAN,"[Create Packet]CREATE DEVICE_NACK\n") << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Name:      " << device_name << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Remote ID: " << remote_device_id << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Local ID:  " << local_device_id << "\n" << vprDEBUG_FLUSH;
      
      
      //std::cout << "CREATE DEVICE_NACK" << std::endl;
      // Remote and Local ID's
      mTempWriter->writeUint16(remote_device_id);

      mTempWriter->writeUint16(local_device_id);

      // Device Name
      //mTempWriter->writeUint16(device_name.size());
      //mTempWriter->writeString(device_name);
      sendString(device_name);

      // Base Type
      std::string base_type = "I Don't have it!";
      //mTempWriter->writeUint16(base_type.size());
      //mTempWriter->writeString(base_type);
      sendString(base_type);

      // Set the size of the packet
      vpr::Uint32 length = mTempWriter->getData()->size();



      ///////////////////
      // CREATE HEADER //
      ///////////////////
      mObjectWriter->writeUint16(RIM_PACKET_MSG);
      mObjectWriter->writeUint16(MSG_DEVICE_NACK);   
      mObjectWriter->writeUint32(length + RIM_HEAD_LENGTH);
      //std::cout << "		LENGTH OF PACKET " << length + RIM_HEAD_LENGTH << std::endl;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "LENGTH OF PACKET: " << length + RIM_HEAD_LENGTH <<"\n" << vprDEBUG_FLUSH;
      ///////////////////////
      // ADD HEADER & BODY //
      ///////////////////////

      mObjectWriter->getData()->insert(mObjectWriter->getData()->end(),mTempWriter->getData()->begin(),mTempWriter->getData()->end());
      mObjectWriter->setCurPos(length + RIM_HEAD_LENGTH);
      vprDEBUG_END(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrCYAN,"[Create Packet]CREATE DEVICE_NACK\n") << vprDEBUG_FLUSH;

   }


   bool  MsgPackage::receiveDeviceAck(vpr::ObjectReader* object_reader)
   {
      ///////////////////////////////////////////////////////////////////////
      //																   //
      // CONSTRUCT ACK PACKET 										       //
      //	FORM:		 vpr::Uint16		vpr::Uint16		 vpr::Uint32   //
      //			 -------------------------------------------------	   //
      //			| RIM_PACKET_MSG | MSG_DEVICE_ACK |   length     |	   //
      //			-------------------------------------------------	   //
      //			A						 					    A	   //
      //			|_______________________________________________|	   //
      //									Already Parsed				   //
      //																   //
      //				 vpr::Uint16		   vpr::Uint16		  		   //
      //			 ---------------------------------------			   //
      //			| remote_device_id  | local_device_id  |			   //
      //			---------------------------------------				   //
      //																   //
      // 			uint      std::string    uint     std::string		   //
      //			 -----------------------------------------------	   //
      //			|	str_len | device_name | str_len | base_type | 	   //
      //   	        -----------------------------------------------		   //
      ///////////////////////////////////////////////////////////////////////

      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrCYAN,"[Parse Packet]RECEIVE DEVICE_ACK\n") << vprDEBUG_FLUSH;
      //std::cout << "RECEIVE DEVICE_ACK" << std::endl;
      //std::cout << "		LENGTH OF PACKET " << object_reader->mData->size() + RIM_HEAD_LENGTH << std::endl;
      
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "LENGTH OF PACKET: " << object_reader->mData->size() + RIM_HEAD_LENGTH <<"\n" << vprDEBUG_FLUSH;
      // Remote and Local ID's
      mReceiverId = object_reader->readUint16();
      mSenderId = object_reader->readUint16();

      // Device Name
      vpr::Uint16 temp_string_len = object_reader->readUint16();
      mDataString = object_reader->readString(temp_string_len);

      // Base Type
      temp_string_len = object_reader->readUint16();
      mBaseType = object_reader->readString(temp_string_len);
      
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Name:      " << mDataString << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Remote ID: " << mSenderId << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Local ID:  " << mReceiverId << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "BaseType:  " << mBaseType << "\n" << vprDEBUG_FLUSH;

      vprDEBUG_END(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrCYAN,"[Parse Packet]RECEIVE DEVICE_ACK\n") << vprDEBUG_FLUSH;
      
      return true;
   }
   bool MsgPackage::receiveDeviceRequest(vpr::ObjectReader* object_reader)
   {
      ///////////////////////////////////////////////////////////////////////
      //																   //
      // CONSTRUCT ACK PACKET 										       //
      //	FORM:		 vpr::Uint16		vpr::Uint16		 vpr::Uint32   //
      //			 -------------------------------------------------	   //
      //			| RIM_PACKET_MSG | MSG_DEVICE_REQ |   length     |	   //
      //			-------------------------------------------------	   //
      //			A												A	   //
      //			|_______________________________________________|	   //
      //								Already Parsed					   //
      //																   //
      //			   vpr::Uint16	 	uint    std::string  			   //
      //			 -------------------------------------  			   //
      //			| device_id  |	str_len | device_name |  			   //
      //			-------------------------------------				   //
      //																   //
      ///////////////////////////////////////////////////////////////////////

      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrCYAN,"[Parse Packet]RECEIVEING A DEVICE REQUEST\n") << vprDEBUG_FLUSH;
      //std::cout << "RECEIVEING A DEVICE REQUEST" << std::endl;
      //std::cout << "		LENGTH OF PACKET " << object_reader->mData->size() + RIM_HEAD_LENGTH << std::endl;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "LENGTH OF PACKET: " << object_reader->mData->size() + RIM_HEAD_LENGTH <<"\n" << vprDEBUG_FLUSH;
      // Device ID
      mSenderId = object_reader->readUint16();

      // Device Name
      vpr::Uint16 temp_name_len = object_reader->readUint16();
      mDataString = object_reader->readString(temp_name_len);
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Name: " << mDataString << "\n" << vprDEBUG_FLUSH;
      vprDEBUG_END(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrCYAN,"[Parse Packet]RECEIVEING A DEVICE REQUEST\n") << vprDEBUG_FLUSH;
      return true;
   }

   bool MsgPackage::createDeviceDataPacket(NetDevice* net_device)
   {
      ////////////////////////////////////////////////////////////////////
      //														        //
      // CONSTRUCT DEVICE PACKET 										//
      //																//
      //	FORM:		 vpr::Uint16		vpr::Uint16	 vpr::Uint32    //
      //			 ------------------------------------------------   //
      //			| RIM_PACKET_MSG | MSG_DEVICE_DATA | length     |   //
      //			------------------------------------------------    //
      //																//
      //             vpr::Uint16	 			                        //
      //           --------------------	                                //
      //          | device_id  | data |	                                //
      //          --------------------		                            //
      ////////////////////////////////////////////////////////////////////

      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  clrOutBOLD(clrCYAN,"[Create Packet]CREATING A DEVICE DATA PACKET\n") << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "Name:      " << net_device->getSourceName() << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "Remote ID: " << net_device->getRemoteId() << "\n" << vprDEBUG_FLUSH;
      //std::cout << "CREATING A DEVICE DATA PACKET" << std::endl;

      /////////////////
      // CREATE BODY //
      /////////////////
      //mTempWriter->writeUint16(net_device->getRemoteId());   
      //net_device->getRealDevice()->writeObject(mTempWriter);
      mTempWriter->mData = net_device->getObjectWriter()->getData();
      // Set the size of the packet
      //vpr::Uint32 length = mTempWriter->getData()->size();
      vpr::Uint32 length = net_device->getObjectWriter()->getData()->size();
      
      ///////////////////
      // CREATE HEADER //
      ///////////////////
      mObjectWriter->writeUint16(RIM_PACKET_MSG);    
      mObjectWriter->writeUint16(MSG_DEVICE_DATA);
      mObjectWriter->writeUint32(length + RIM_HEAD_LENGTH + 2);
      
      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "LENGTH OF PACKET: " << length + RIM_HEAD_LENGTH + 2 <<"\n" << vprDEBUG_FLUSH;
      mObjectWriter->writeUint16(net_device->getRemoteId());   
      ///////////////////////
      // ADD HEADER & BODY //
      ///////////////////////

      mObjectWriter->getData()->insert(mObjectWriter->getData()->end(),net_device->getObjectWriter()->getData()->begin(),net_device->getObjectWriter()->getData()->end());
      mObjectWriter->setCurPos(length + RIM_HEAD_LENGTH);

      vprDEBUG_END(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  clrOutBOLD(clrCYAN,"[PACKET]CREATING A DEVICE DATA PACKET\n") << vprDEBUG_FLUSH;

      // NO createEndBlock(); becasue the point of this call is to end all connection data,
      // 							not just after each device's data
      return true;
   }

   bool MsgPackage::recieveDeviceDataPacket(vpr::ObjectReader* object_reader, Input* virtual_device)
   {
      //////////////////////////////////////////////////////////////////////////////////////
      //																			      //
      // CONSTRUCT DEVICE PACKET 													      //
      //																			      //
      //	FORM:		 vpr::Uint16		vpr::Uint16	 vpr::Uint32  vpr::Uint16	 	  //
      //			--------------------------------------------------------------------- //
      //			| RIM_PACKET_MSG | MSG_DEVICE_DATA | length     | device_id  | data | //
      //			--------------------------------------------------------------------  //
      //			A															A		  //
      //			|___________________________________________________________|		  //
      //							 Already Parsed										  //
      //																				  //
      //////////////////////////////////////////////////////////////////////////////////////
      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  clrOutBOLD(clrCYAN,"[Parse Packet]RECEIVEING A DEVICE DATA PACKET\n") << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "LENGTH OF PACKET: " << object_reader->mData->size() + RIM_HEAD_LENGTH <<"\n" << vprDEBUG_FLUSH;
      
         // object_reader and virtual_device should never be NULL
      vprASSERT(virtual_device != NULL && "Virtual Device does not exist!!!");
      vprASSERT(object_reader != NULL && "Object Reader does not exist!!!");
      
         //Read the device data from the packet
      virtual_device->readObject(object_reader);

      vprDEBUG_END(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  clrOutBOLD(clrCYAN,"[Parse Packet]RECEIVEING A DEVICE DATA PACKET\n") << vprDEBUG_FLUSH;
      return true;
   }

   void MsgPackage::createEndBlock()
   {
      /////////////////////////////////////////////////////
      //										         //
      // CONSTRUCT END BLOCK` 							 //
      //	FORM:		 vpr::Uint16		vpr::Uint16	 //
      //			 ---------------------------------	 //
      //			| RIM_PACKET_MSG | MSG_END_BLOCK |	 //
      //			---------------------------------	 //
      //												 //
      /////////////////////////////////////////////////////

      //mObjectWriter->writeUint32(12);
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrCYAN,"[Create Packet]CREATE END BLOCK\n") << vprDEBUG_FLUSH;
      /////////////////
      // CREATE BODY //
      /////////////////
      //mTempWriter->writeUint32(12);
      
      // Set the size of the packet
      vpr::Uint32 length = mTempWriter->getData()->size();
      
      ///////////////////
      // CREATE HEADER //
      ///////////////////
      mObjectWriter->writeUint16(RIM_PACKET_MSG);    
      mObjectWriter->writeUint16(MSG_END_BLOCK);
      mObjectWriter->writeUint32(length + RIM_HEAD_LENGTH);
      
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "LENGTH OF PACKET: " << length + RIM_HEAD_LENGTH <<"\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "LENGTH OF DATA: " << length <<"\n" << vprDEBUG_FLUSH;
      
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrCYAN,"[Create Packet]CREATE END BLOCK\n") << vprDEBUG_FLUSH;
      //mObjectWriter->getData()->insert(mObjectWriter->getData()->end(),mTempWriter->getData()->begin(),mTempWriter->getData()->end());
      //mObjectWriter->setCurPos(length + RIM_HEAD_LENGTH);
   }

   /*void MsgPackage::createDeviceDelete(const vpr::Uint16 remote_device_id)
   {
      /////////////////////////////////////////////////////////////////////
      //																 //
      // CONSTRUCT ACK PACKET 											 //
      //	FORM:		 vpr::Uint16		vpr::Uint16		 vpr::Uint32 //
      //			 -------------------------------------------------	 //
      //			| RIM_PACKET_MSG | MSG_DEVICE_ACK |   length     |   //
      //			-------------------------------------------------	 //
      //																 //
      //				 vpr::Uint16		   vpr::Uint16		  		 //
      //			 ---------------------------------------			 //
      //			| remote_device_id  | local_device_id  |			 //
      //			---------------------------------------				 //
      //																 //
      // 			uint      std::string    uint     std::string		 //
      //			 -----------------------------------------------	 //
      //			|	str_len | device_name | str_len | base_type | 	 //
      //   	        -----------------------------------------------		 //
      /////////////////////////////////////////////////////////////////////


      /////////////////
      // CREATE BODY //
      /////////////////

      std::cout << "CREATE DEVICE_ACK" << std::endl;
      // Remote and Local ID's
      mTempWriter->writeUint16(remote_device_id);

      // Set the size of the packet
      vpr::Uint32 length = mTempWriter->getData()->size();
      

      ///////////////////
      // CREATE HEADER //
      ///////////////////
      mObjectWriter->writeUint16(RIM_PACKET_MSG);
      mObjectWriter->writeUint16(MSG_DELETE_DEVICE);    
      mObjectWriter->writeUint32(length + RIM_HEAD_LENGTH);
      std::cout << "		LENGTH OF PACKET " << length + RIM_HEAD_LENGTH << std::endl;

      ///////////////////////
      // ADD HEADER & BODY //
      ///////////////////////

      mObjectWriter->getData()->insert(mObjectWriter->getData()->end(),mTempWriter->getData()->begin(),mTempWriter->getData()->end());
      mObjectWriter->setCurPos(length + RIM_HEAD_LENGTH);


      createEndBlock();
   } */

   /*vpr::Uint16 MsgPackage::recieveDeviceDelete(vpr::ObjectReader* object_reader)
   {
      ////////////////////////////////////////////////////////////////////
      //																 //
      // CONSTRUCT ACK PACKET 											 //
      //	FORM:		 vpr::Uint16		vpr::Uint16		 vpr::Uint32 //
      //			 -------------------------------------------------	 //
      //			| RIM_PACKET_MSG | MSG_DEVICE_ACK |   length     |   //
      //			-------------------------------------------------	 //
      //																 //
      //				 vpr::Uint16		   vpr::Uint16		  		 //
      //			 ---------------------------------------			 //
      //			| remote_device_id  | local_device_id  |			 //
      //			---------------------------------------				 //
      //																 //
      // 			uint      std::string    uint     std::string		 //
      //			 -----------------------------------------------	 //
      //			|	str_len | device_name | str_len | base_type | 	 //
      //   	        -----------------------------------------------		 //
      ////////////////////////////////////////////////////////////////////


      /////////////////
      // CREATE BODY //
      /////////////////

      std::cout << "DELETE DEVICE" << std::endl;
      // Remote and Local ID's
      
      return(object_reader->readUint16()); 
   } */






   void MsgPackage::createClockSyncClientRequest()
   {
      /////////////////////////////////////////////////////
      //										         //
      // CONSTRUCT END BLOCK` 							 //
      //	FORM:		 vpr::Uint16		vpr::Uint16	 //
      //			 ---------------------------------	 //
      //			| RIM_PACKET_MSG | MSG_END_BLOCK |	 //
      //			---------------------------------	 //
      //												 //
      /////////////////////////////////////////////////////
      
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrCYAN,"[Create Packet]Requesting a clock sync client.\n") << vprDEBUG_FLUSH;
      mObjectWriter->writeUint16(RIM_PACKET_MSG);    
      mObjectWriter->writeUint16(MSG_END_BLOCK);
      mObjectWriter->writeUint32(10);
      mObjectWriter->writeUint16(10);
   }

   bool MsgPackage::createHandshake(bool accept_reject, const std::string& host, const vpr::Uint16& port, std::string manager_id)
   {
      ///////////////////////////////////////////////////////////////////////
      //													               //
      // CONSTRUCT NACK PACKET 											   //
      //	FORM:		 vpr::Uint16		vpr::Uint16		 vpr::Uint32   //
      //			 -------------------------------------------------	   //
      //			| RIM_PACKET_MSG | MSG_HANDSHAKE |   length     |	   //
      //			-------------------------------------------------	   //
      //																   //
      // 			bool 			 uint      std::string   vpr::Uint16   //
      //			 --------------------------------------------------	   //
      //			| rejection? | str_len |    host    |    port     |    //
      //   	        --------------------------------------------------	   //
      // 			uint      std::string  								   //
      //			 -------------------------							   //
      //			|	str_len | manager_id | 							   //
      //   	        ------------------------- 							   //
      ///////////////////////////////////////////////////////////////////////
      
      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrCYAN,"[Create Packet]CREATING HANDSHAKE\n") << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Host: " << host << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Port: " << port << "\n" << vprDEBUG_FLUSH;
      //std::cout << "CREATING HANDSHAKE" << std::endl;
      /////////////////
      // CREATE BODY //
      /////////////////

      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrYELLOW) << "SEND BOOL: " << accept_reject << "\n" << clrRESET << vprDEBUG_FLUSH;
      mTempWriter->writeBool(accept_reject);    //Must use two because of alignment issues
      
      
      // Write hostname
      mTempWriter->writeUint16(host.size());
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrYELLOW) << "SEND host_len: " << host.size() << "\n" << clrRESET << vprDEBUG_FLUSH;
      mTempWriter->writeString(host);
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrYELLOW) << "SEND host: " << host << "\n" << clrRESET << vprDEBUG_FLUSH;

      //sendString(host);

      // Write port
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrYELLOW) << "SEND Port: " << port << "\n" << clrRESET << vprDEBUG_FLUSH;
      mTempWriter->writeUint16(port);

      mTempWriter->writeUint16(manager_id.size());
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrYELLOW) << "SEND man_id_len: " << manager_id.size() << "\n" << clrRESET << vprDEBUG_FLUSH;
      mTempWriter->writeString(manager_id);
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrYELLOW) << "SEND man_id: " << manager_id << "\n" << clrRESET << vprDEBUG_FLUSH;

      //sendString(manager_id);

      // Get the length of the data
      vpr::Uint32 length = mTempWriter->getData()->size();


      ///////////////////
      // CREATE HEADER //
      ///////////////////
      mObjectWriter->writeUint16(RIM_PACKET_MSG);
      mObjectWriter->writeUint16(MSG_HANDSHAKE);     
      mObjectWriter->writeUint32(length + RIM_HEAD_LENGTH);
         
      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "LENGTH OF PACKET: " << length + RIM_HEAD_LENGTH <<"\n" << vprDEBUG_FLUSH;
      
      ///////////////////////
      // ADD HEADER & BODY //
      ///////////////////////

      mObjectWriter->getData()->insert(mObjectWriter->getData()->end(),mTempWriter->getData()->begin(),mTempWriter->getData()->end());
      mObjectWriter->setCurPos(length + RIM_HEAD_LENGTH);

      vprDEBUG_END(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrOutBOLD(clrCYAN,"[PACKET]CREATING HANDSHAKE\n") << vprDEBUG_FLUSH;

      // NO createEndBlock(); becasue the point of this call is to end all connection data,
      // 							not just after each device's data

      return true;
   }

   bool MsgPackage::receiveHandshake(std::string& receivedHostname, vpr::Uint16& receivedPort,std::string& received_manager_id, vpr::SocketStream* newStream)
   {
      ///////////////////////////////////////////////////////////////////////
      //														           //
      // CONSTRUCT NACK PACKET 											   //
      //	FORM:		 vpr::Uint16		vpr::Uint16		 vpr::Uint32   //
      //			 -------------------------------------------------	   //
      //			| RIM_PACKET_MSG | MSG_HANDSHAKE |   length     |	   //
      //			-------------------------------------------------	   //
      //																   //
      // 			bool 			 uint      std::string   vpr::Uint16   //
      //			 ---------------------------------------------------   //
      //			| rejection? | str_len |    host    |    port     |    //
      //   	        --------------------------------------------------	   //
      // 			uint      std::string  								   //
      //			 -------------------------							   //
      //			|	str_len | manager_id | 							   //
      //   	        ------------------------ 							   //
      ///////////////////////////////////////////////////////////////////////

      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL) <<  clrOutBOLD(clrCYAN,"[Parse Packet]RECEIVING HANDSHAKE\n") << vprDEBUG_FLUSH;
      //std::cout << "RECEIVING HANDSHAKE" << std::endl;

      std::vector<vpr::Uint8> packet_head(100);

      vpr::Uint32 bytes_read;
      bool accept_reject;



      // Read in the Packet Header
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) <<  "[PACKET]BEFORE READING HANDSHAKE\n" << vprDEBUG_FLUSH;
      
      while (newStream->recvn(packet_head,8,bytes_read) != vpr::ReturnStatus::Succeed)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"...Still waiting for handshake HEAD") 
            << "\n" << vprDEBUG_FLUSH;

      }
      
      //vpr::ReturnStatus status = newStream->recvn(packet_head,8,bytes_read);
      
      //if (status == vpr::ReturnStatus::WouldBlock)
      //{
         //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) <<  clrOutNORM(clrRED,"[PACKET]WouldBlock\n") << vprDEBUG_FLUSH;
         //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) <<  "[PACKET]WouldBlock\n" << vprDEBUG_FLUSH;
         //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) <<  "[PACKET]BEFORE READ\n" << vprDEBUG_FLUSH;
         //status = newStream->recvn(packet_head,8,bytes_read);
         //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) <<  "[PACKET]BEFORE READ\n" << vprDEBUG_FLUSH;
      //}
      
      //std::cout << (int)status.failure() << (int)status.inProgress() << (int)status.success()
      //            << (int)status.timeout() << (int)status.wouldBlock() << std::endl;

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) <<  "[PACKET]HANDSHAKE READ SUCCESSFUL\n" 
         << "  READ NUM BYTES: " << bytes_read << "\n" << vprDEBUG_FLUSH;
      vpr::ObjectReader* head_reader = new vpr::ObjectReader(&packet_head);

      // Read the info from the header
      vpr::Uint16 rimcode =  head_reader->readUint16();
      vpr::Uint16 opcode  =  head_reader->readUint16();
      vpr::Uint32 length  =  head_reader->readUint32();

      std::vector<vpr::Uint8> packet_data(100);
      
      while (newStream->recvn(packet_data,length-RIM_HEAD_LENGTH,bytes_read,vpr::Interval(100,vpr::Interval::Msec)) == vpr::ReturnStatus::Timeout)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"...Still waiting for handshake DATA") 
            << "\n" << vprDEBUG_FLUSH;

      }
      
      vpr::ObjectReader* reader = new vpr::ObjectReader(&packet_data);

      vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "LENGTH OF PACKET: " << reader->mData->size() + RIM_HEAD_LENGTH <<"\n" << vprDEBUG_FLUSH;
      if ( rimcode == RIM_PACKET_MSG && opcode == MSG_HANDSHAKE )
      {
         accept_reject = reader->readBool();
         vpr::Uint16 temp_string_len = reader->readUint16();
         receivedHostname = reader->readString(temp_string_len);
         receivedPort = reader->readUint16();
         temp_string_len = reader->readUint16();
         received_manager_id = reader->readString(temp_string_len);

         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrYELLOW) << "======RECEIVE HANDSHAKE INFO===== \n" << clrRESET << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrYELLOW) << "BOOL:       " << accept_reject << "\n" << clrRESET << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrYELLOW) << "str_len:    " << temp_string_len << "\n" << clrRESET << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrYELLOW) << "host:       " << receivedHostname << "\n" << clrRESET << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrYELLOW) << "port:       " << receivedPort << "\n" << clrRESET << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrYELLOW) << "str_len:    " << temp_string_len << "\n" << clrRESET << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrSetBOLD(clrYELLOW) << "manager_id: " << received_manager_id << "\n" << clrRESET << vprDEBUG_FLUSH;
         

         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Host: " << receivedHostname << "\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  "Port: " << receivedPort << "\n" << vprDEBUG_FLUSH;
         if (!accept_reject)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) <<  clrOutBOLD(clrRED, "HANDSHAKE FALSE\n") << vprDEBUG_FLUSH;
         }
            
      }
      vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) <<  clrOutBOLD(clrCYAN,"[PACKET]RECEIVING HANDSHAKE\n") << vprDEBUG_FLUSH;
      return accept_reject;
   }





}     // end namespace gadget
