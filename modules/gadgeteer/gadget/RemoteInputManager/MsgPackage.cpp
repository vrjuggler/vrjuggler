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

#include <gadget/RemoteInputManager/NetUtils.h>
#include <gadget/Util/Debug.h>
#include <gadget/RemoteInputManager/MsgPackage.h>


namespace gadget
{


// Constructor

MsgPackage::MsgPackage()
{
   mDataLength = 0;
}


// send contents of current package to the socket specified

void MsgPackage::sendContents(vpr::SocketStream& sock_stream) const
{
   if ( mDataLength > 0 )
   {
      sendAtOnce(sock_stream, mBuffer, mDataLength);
   }
}


// send contents of current package to the socket specified
// and then clear the message

void MsgPackage::sendAndClear(vpr::SocketStream& sock_stream)
{
   if ( mDataLength > 0 )
   {
      sendAtOnce(sock_stream, mBuffer, mDataLength);
   }
   clear();
}


// creates a device request message with the specified id and name

void MsgPackage::createDeviceRequest(vpr::Uint16 device_id,
                                     const std::string device_name)
{
   vpr::Uint16 code = MSG_DEVICE_REQ;  // stands for Device Request
   code = htons(code);
   char code_str[2];
   vpr::Uint16 net_device_id = htons(device_id);
   char device_id_str[2];
   ushortTo2Bytes(code_str, code);
   ushortTo2Bytes(device_id_str, net_device_id);

   sprintf(mBuffer, "%c%c%c%c%s;", code_str[0], code_str[1], device_id_str[0],
           device_id_str[1], device_name.c_str());

   // DEBUGGING
   char some_name[64];
   uint i;
   for ( i = 4; i < device_name.length() + 4 ; i++ )
   {
      some_name[i-4] = mBuffer[i];
   }
   some_name[i-4] = '\0';
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
      << "createDeviceRequest:code=" << binaryToUshort(code_str,2)
      << ", name = " << some_name << std::endl << vprDEBUG_FLUSH;
   // END DEBUGGING

   // length of message is 2 bytes of instruction code + 2 bytes id + name length + 1 semicolon
   mDataLength = 5 + device_name.size();
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
      << "createDeviceRequest: last char = " << mBuffer[mDataLength - 1]
      << std::endl << vprDEBUG_FLUSH;
}


// creates a device acknowledgement message with the specified id and name

void MsgPackage::createDeviceAck(const vpr::Uint16 remote_device_id,
                                 const vpr::Uint16 local_device_id,
                                 const std::string device_name)
{
   vpr::Uint16 code = MSG_DEVICE_ACK;  // stands for Device Acknowledgement
   code = htons(code);
   char code_str[2];
   vpr::Uint16 net_local_device_id = htons(local_device_id);        // sender id
   vpr::Uint16 net_remote_device_id = htons(remote_device_id);      // receiver id
   char remote_device_id_str[2];
   char local_device_id_str[2];
   ushortTo2Bytes(code_str, code);
   ushortTo2Bytes(remote_device_id_str, net_remote_device_id);
   ushortTo2Bytes(local_device_id_str, net_local_device_id);
   // std::cout << "Creating ACK code: " << binaryToUshort(code_str,2) << std::endl;
   // std::cout << "Creating ACK code, local_dev id: " << local_device_id << ", " << binaryToUshort(local_device_id_str,2) << std::endl;
   // std::cout << "Creating ACK code, remote_dev id: " << remote_device_id << ", " << binaryToUshort(remote_device_id_str,2) << std::endl;

   // the device name string isn't absolutely necessary in the ack, but it helps prevent errors in what's only a one-time config message
   sprintf(mBuffer, "%c%c%c%c%c%c%s;", code_str[0], code_str[1],
           remote_device_id_str[0], remote_device_id_str[1],
           local_device_id_str[0], local_device_id_str[1],
           device_name.c_str());
   mDataLength = 7 + device_name.size();
}


// creates a rejection (negative acknowledgement) message with the specified id and name
// for simplicity, same as ACK, but with different opcode

void MsgPackage::createDeviceNack(const vpr::Uint16 remote_device_id, const vpr::Uint16 local_device_id, const std::string device_name)
{
   vpr::Uint16 code = MSG_DEVICE_NACK;  // stands for Negative Device Acknowledement
   code = htons(code);
   char code_str[2];
   vpr::Uint16 net_local_device_id = htons(local_device_id);        // sender id
   vpr::Uint16 net_remote_device_id = htons(remote_device_id);      // receiver id
   char remote_device_id_str[2];
   char local_device_id_str[2];
   ushortTo2Bytes(code_str, code);
   ushortTo2Bytes(remote_device_id_str, net_remote_device_id);
   ushortTo2Bytes(local_device_id_str, net_local_device_id);
   // std::cout << "Creating NACK code: " << binaryToUshort(code_str,2) << std::endl;
   // std::cout << "Creating NACK code, local_dev id: " << local_device_id << ", " << binaryToUshort(local_device_id_str,2) << std::endl;
   // std::cout << "Creating NACK code, remote_dev id: " << remote_device_id << ", " << binaryToUshort(remote_device_id_str,2) << std::endl;

   // the device name string isn't absolutely necessary in the ack, but it helps prevent errors in what's only a one-time config message
   sprintf(mBuffer, "%c%c%c%c%c%c%s;", code_str[0], code_str[1],
           remote_device_id_str[0], remote_device_id_str[1],
           local_device_id_str[0], local_device_id_str[1],
           device_name.c_str());
   mDataLength = 7 + device_name.size();
}


// processes a device request message
// returns the number of bytes used from buffer (excluding the already read opcode)
// A return value of zero means unable to process -- complete message not received yet.

int MsgPackage::receiveDeviceRequest(char* ptr, int len)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HEX_LVL)
      << "receiveDeviceRequest: len=" << len << std::endl << vprDEBUG_FLUSH;
   // read sender's id from buffer
   VJ_NETID_TYPE tmp = binaryToUshort(ptr,len);
   mSenderId = ntohs(tmp);


   // read device name from buffer
   int index;
   int endline = 0;
   for ( index = 2; index < len; index++ )
   {
      if ( ptr[index] == ';' )
      {
         endline = 1;
         index++;  // we want to count the semicolon as a character that's been read
         break;      // we at least
      }
   }

   if ( !endline )
   { // couldn't read a full line, so try again later when there will be more data
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
         << "receiveDeviceRequest: couldn't read full line yet"
         << std::endl << vprDEBUG_FLUSH;
      // for(int i = 0; i < len; i++)
      // std::cout << ptr[i];
      // std::cout << std::endl;
      return 0;
   }

   // DEBUGGING
   char some_name[64];
   int i;
   for ( i = 2; i < len - 1 - 2; i++ )
   {
      some_name[i-2] = ptr[i];
   }
   some_name[i-2] = '\0';
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
      << "receiveDeviceRequest:SenderId=" << mSenderId
      << ", name = " << some_name << std::endl << vprDEBUG_FLUSH;
   // END DEBUGGING

   // read the name of the device
   int name_length = index - 1 - 2;  // don't include the semicolon(-1) or the device_id(-2) in the name
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
      << "receiveDeviceRequest:name_length=" << name_length << std::endl
      << vprDEBUG_FLUSH;

   // copy the Device Name string
   mDataString = "";
   for ( int j = 0; j < name_length ; j++ )
   {
      mDataString += ptr[j+2];             // + 2 is to bypass sender id;
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "receiveDeviceRequest: name=" << mDataString << std::endl
      << vprDEBUG_FLUSH;
   return (2 + name_length + 1);  // sender id(2) name(n) semicolon(1)
}


// This works for both ACK and NACK
// returns the number of bytes used from buffer (excluding the already read opcode)
// A return value of zero means unable to process -- complete message not received yet.

int MsgPackage::receiveDeviceAck(char* ptr, int len)
{
   // read ids from buffer
   mReceiverId = binaryToUshort(ptr,len);
   mReceiverId = ntohs(mReceiverId);
   mSenderId = binaryToUshort(ptr+2,len);
   mSenderId = ntohs(mSenderId);

   // read device name from buffer
   int endline = 0;
   int index;
   for ( index = 2; index < len; index++ )
   {
      if ( ptr[index] == ';' )
      {
         endline = 1;
         index++;  // we want to count the semicolon as a character that's been read
         break;      // we at least
      }
   }

   if ( !endline )
   { // couldn't read a full line, so try again later when there will be more data
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
         << "receiveDeviceAck: couldn't read full line yet" << std::endl
         << vprDEBUG_FLUSH;
      // for(int i = 0; i < len; i++)
      // std::cout << ptr[i];
      // std::cout << std::endl;
      return 0;
   }

   // read the name of the device
   int name_length = index - 1 - 2 - 2;  // don't include the semicolon(-1), local_device_id(-2), or remote_device_id

   // copy the Device Name string
   mDataString = "";
   for ( int j = 4; j < len - 1 ; j++ )         // + 2 is to bypass sender id;
   {
      mDataString += ptr[j];
   }

   return (4 + name_length + 1);  // sender id(2), receiver_id(2), name(n) semicolon(1)
}

// creates a clock sync message containing two times
void MsgPackage::createClockSync(const float& time_a, const float& time_b, const bool clock_is_synced)
{
   vpr::Uint16 code = MSG_CLOCK_SYNC;
   code = htons(code);
   ushortTo2Bytes(&(mBuffer[0]), code);
   floatTo4Bytes(&(mBuffer[2]), vj_htonf(time_a) );
   floatTo4Bytes(&(mBuffer[6]), vj_htonf(time_b) );
   mBuffer[10] = (char) clock_is_synced;
   mBuffer[11] = ';';
   mDataLength = 12; // code(2) + float(4) + float(4) + clock_is_synced(1) + semicolon(1)
   return;
}

// returns the number of bytes used from buffer (excluding the already read opcode)
// A return value of zero means unable to process -- complete message not received yet.
int MsgPackage::receiveClockSync(char* ptr, int len, float& time_a, float& time_b, bool& clock_is_synced)
{
   if ( len < 2 * sizeof(float) + 1 )
   {
      return 0;  // only partial message received, wait for more data
   }

   bytes4ToFloat(time_a, ptr);
   bytes4ToFloat(time_b, ptr + 4);
   clock_is_synced = (bool) ptr[8];
   return (4 + 4 + 1 + 1);  // float(4) + float(4) + clock_is_synced(1) + semicolon(1)
}

// creates a message with a Manager id, and whether we have it or not
void MsgPackage::createClockSyncHaveSrc(const vpr::GUID& id, const bool clock_is_synced)
{
   vpr::Uint16 code = MSG_CLOCK_SRC;
   code = htons(code);
   ushortTo2Bytes(&(mBuffer[0]), code);
   std::string id_str(id.toString());

   int i = 0;
   for ( ; i < id_str.length(); i++ )
   {
      mBuffer[2+i] = id_str[i];
   }

   mBuffer[2 + i++] = (bool) clock_is_synced;
   mBuffer[2 + i++] = ';';

   mDataLength = 2 + i; // opcode(2) + vpr::GUID length + 1 + 1
   return;
}

// returns the number of bytes used from buffer (excluding the already read opcode)
// A return value of zero means unable to process -- complete message not received yet.
int MsgPackage::receiveClockSyncHaveSrc(char* ptr, int len, vpr::GUID& id, bool& clock_is_synced)
{
   if ( len < vpr::GUID().toString().length() + 1 )
   {
      return 0;  // only partial message received, wait for more data
   }

   std::string id_str(vpr::GUID().toString());
   int i = 0;
   for ( ; i < id_str.length(); i++ )
   {
      id_str[i] = ptr[i];
   }
   id = vpr::GUID(id_str);

   clock_is_synced = (bool) ptr[i++];

   // i++; // trailing semicolon
   // vprASSERT(i == id_str.length() + 1 + 1;

   return (id_str.length() + 1 + 1);  // id_str(n) + bool(1)
}


}     // end namespace gadget
