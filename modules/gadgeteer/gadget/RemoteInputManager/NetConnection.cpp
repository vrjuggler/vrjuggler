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

#include <vpr/vpr.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <jccl/Config/ConfigChunk.h>

#include <gadget/Util/Debug.h>
#include <gadget/RemoteInputManager/RecvBuffer.h>
#include <gadget/Type/NetInput.h>
#include <gadget/Type/NetDigital.h>
#include <gadget/Type/NetPosition.h>
#include <gadget/RemoteInputManager/NetConnection.h>


namespace gadget
{

NetConnection::NetConnection()
{
   mSockStream = NULL;
   mRecvBuffer = new RecvBuffer;
   mAllDataReceived = 0;
   mRemoteIdGen.setMaxId(399);
   mSendIterations = 0;
   mRecvIterations = 0;
}

NetConnection::NetConnection(const std::string& alias_name,
                             const std::string& hostname, const int port,
                             const std::string& manager_id,
                             vpr::SocketStream* sock_stream)
{
   char port_str[32];
   sprintf(port_str, "%d", port);
   mName = hostname + ":" + port_str; // name contains "host:port"

   mHostname = hostname;
   mPort = port;
   mManagerId = vpr::GUID(manager_id); // set the id of the other computer's remote manager
   mAliasList.push_back(alias_name);
   mSockStream = sock_stream;
   mRecvBuffer = new RecvBuffer;
   mAllDataReceived = 0;
   mRemoteIdGen.setMaxId(399);
   mSendIterations = 0;
   mRecvIterations = 0;
}

NetConnection::~NetConnection()
{
   delete mSockStream;
   delete mRecvBuffer;
}

void NetConnection::receiveNetworkData()
{
   unsigned int bytes_read;

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "recvNetworkData(): " << mName << ".  recv iteration:"
      << (mRecvIterations + 1) << std::endl << vprDEBUG_FLUSH;


   // XXX DEBUG LOOP
   //vprDEBUG(gadgetDBG_INPUT_MGR, vrjDBG_DETAILED_LVL)
   //   << "receiveNetworkData:pre: " << std::endl << vprDEBUG_FLUSH;
   //for(int i = 0; i < mRecvBuffer->getReadableSize(); i++){
   //   std::cout << mRecvBuffer->getReadPtr()[i];
   //   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
   //      << mRecvBuffer->getReadPtr()[i] << vprDEBUG_FLUSH;
   //}
   //vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL) << std::endl
   //                                                   << vprDEBUG_FLUSH;

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
      << "receiveNetworkData: " << mName << " Calling recv" << std::endl
      << vprDEBUG_FLUSH;
   mSockStream->recv(mRecvBuffer->getWritePtr(), mRecvBuffer->getWriteableSize(), bytes_read);

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
      << "receiveNetworkData: " << mName << " Finished recv" << std::endl
      << vprDEBUG_FLUSH;
   if ( bytes_read > 0 )
   {
      mRecvBuffer->increaseReadableSize(bytes_read);
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "receiveNetworkData: " << mName << " recv'd " << bytes_read
      << " bytes, for a readable size of " << mRecvBuffer->getReadableSize()
      << std::endl << vprDEBUG_FLUSH;

   // XXX DEBUG LOOP
   //vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
   //   << "receiveNetworkData:post: " << vprDEBUG_FLUSH;
   //for(int i = 0; i < mRecvBuffer->getReadableSize(); i++){
   //   std::cout << mRecvBuffer->getReadPtr()[i];
   //   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
   //      << mRecvBuffer->getReadPtr()[i] << vprDEBUG_FLUSH;
   //}
   //vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL) << std::endl
   //                                                   << vprDEBUG_FLUSH;

}

void NetConnection::sendNetworkData()
{
   mSendIterations++;
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "sendNetworkData(): " << mName << ".  send iteration:"
      << mSendIterations << std::endl << vprDEBUG_FLUSH;
   for ( std::list<NetInput*>::iterator i = mTransmittingInputs.begin();
         i != mTransmittingInputs.end();
         i++ )
   {
      // vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
      //    << "sendNetworkData() pre" << std::endl << vprDEBUG_FLUSH;
      if ( (*i)->getWasInitialized() )
      {
         (*i)->updateFromLocalSource();
         mSendBuffer.store( (*i)->getDataPtr(), (*i)->getDataByteLength() );
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
            << "sent device data " << (*i)->getDataByteLength()
            << " bytes to " << mName << std::endl << vprDEBUG_FLUSH;
      }
   }
}

void NetConnection::addTransmittingNetInput(NetInput* net_input)
{
   if ( std::find(mTransmittingInputs.begin(), mTransmittingInputs.end(), net_input) == mTransmittingInputs.end() )
   {
      mTransmittingInputs.push_back(net_input);
   }
}

void NetConnection::addReceivingNetInput(NetInput* net_input)
{
   if ( std::find(mReceivingInputs.begin(), mReceivingInputs.end(), net_input) == mReceivingInputs.end() )
   {
      mReceivingInputs.push_back(net_input);
   }

   sendDeviceRequest(net_input);
   // net_input->addDependency();
}

NetInput* NetConnection::createReceivingNetInput(jccl::ConfigChunkPtr chunk)
{
   NetInput* net_input;
   std::string chunk_type = chunk->getDescToken();
   if ( chunk_type == "DigProxy" )
   {
      NetDigital* net_digital = new NetDigital(chunk, this->generateLocalId());
      net_input = dynamic_cast<NetInput*> (net_digital);
   }
   else if ( chunk_type == "PosProxy" )
   {
      NetPosition* net_pos = new NetPosition(chunk, this->generateLocalId());
      net_input = dynamic_cast<NetInput*> (net_pos);
   }
   else
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "createReceivingNetInput: chunk_type: " << chunk_type
         << " unrecognized." << std::endl << vprDEBUG_FLUSH;
      net_input = NULL;
   }

   if ( net_input != NULL )
   {
      mReceivingInputs.push_back(net_input);
      net_input->addDependency();
   }

   return net_input;
}

NetInput* NetConnection::findReceivingNetInput(const std::string& device_name)
{
   for ( std::list<NetInput*>::iterator i = mReceivingInputs.begin();
       i!= mReceivingInputs.end(); i++ )
   {
      if ( (*i)->getSourceName() == device_name )
      {
         return *i;
      }
   }
   return NULL;
}

NetInput* NetConnection::findReceivingNetInputByLocalId(VJ_NETID_TYPE local_id)
{
   for ( std::list<NetInput*>::iterator i = mReceivingInputs.begin();
       i!= mReceivingInputs.end(); i++ )
   {
      if ( (*i)->getLocalId() == local_id )
      {
         return *i;
      }
   }
   return NULL;
}

void NetConnection::sendDeviceRequest(NetInput* net_input )
{
   if ( mSockStream == NULL )
   {
      return;
   }

   mMsgPackage.createDeviceRequest(net_input->getLocalId(), net_input->getSourceName());
   sendMsg(mMsgPackage);
   mMsgPackage.clear();

/*
   char request_msg[128];
   ushort code = MSG_DEVICE_REQ;

   char code_str[2];
   //code_str[0] = code / 256;
   //code_str[1] = code % 256;
   shortTo2Bytes(code_str, code);
   std::string device_name = net_input->getInstanceName();
   sprintf(request_msg, "%c%c%s;", code_str[0], code_str[1], device_name.c_str());

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
      << "!!!!!!!!SendDeviceRequest:code=" << binaryToShort(code_str,2)
      << std::endl << vprDEBUG_FLUSH;

   // length of message is 2 bytes of instruction code + name length + 1 semicolon
   int msg_length = 3 + device_name.size();
   sendAtOnce(*mSockStream, request_msg, msg_length);
   // vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIGURE_LVL)
   //    << "!!!!!!!!Request sent for device: " << device_name << std::endl
   //    << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
      << "!!!!!!!!Request sent for device: " << device_name << std::endl
      << vprDEBUG_FLUSH;
*/
}

void NetConnection::resendRequestsForNackedDevices()
{
   for ( std::list<NetInput*>::iterator i = mReceivingInputs.begin();
       i!= mReceivingInputs.end(); i++ )
   {
      if ( (*i)->needsToResendRequest() )
      {
         sendDeviceRequest(*i);
         (*i)->clearNeedToResendRequestFlag();
      }
   }
}

void NetConnection::sendEndBlock()
{
   if ( mSockStream == NULL )
   {
      return;
   }

   char msg[8];
   ushort code = 410;
   code = htons(code);
   char code_str[2];
   // code_str[0] = code / 256;
   // code_str[1] = code % 256;
   ushortTo2Bytes(code_str, code);
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
      << "!!!!!!!!SendEndBlock:code= " << binaryToUshort(code_str,2)
      << std::endl << vprDEBUG_FLUSH;

   sprintf(msg, "%c%c;", code_str[0], code_str[1]);
   // length of message is 2 bytes of instruction code + name length + 1 semicolon
   int msg_length = 3;
   mSendBuffer.store( msg, msg_length );
   mSendBuffer.sendAllAndClear(*mSockStream);
   // vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIGURE_LVL)
   //    << "!!!!!!!!Request sent for device: " << device_name << std::endl
   //    << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
      << "!!!!!!!!EndBlock sent for connection: " << mName << std::endl
      << vprDEBUG_FLUSH;
}

bool NetConnection::allDataReceived()
{
   return mAllDataReceived;
}

void NetConnection::setDataUnreceived()
{
   mAllDataReceived = false;
}

void NetConnection::setDataReceived()
{
   mRecvIterations++; // for debugging
   mAllDataReceived = true;
}

void NetConnection::sendMsg(const MsgPackage& msg_package)
{
   msg_package.sendContents(*mSockStream);
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
      << "NetConnection: sent " << msg_package.getDataLength()
      << " bytes to " << mName << "." << std::endl << vprDEBUG_FLUSH;
}

// adds this name to or list of names for this connection
void NetConnection::addAliasName(const std::string& alias_name)
{
   if ( !hasAliasName(alias_name) )
   {
      mAliasList.push_back(alias_name);
   }
}

// returns true if test_name is one of this connection's aliases
bool NetConnection::hasAliasName(const std::string& test_name)
{
   std::vector<std::string>::iterator i;
   for ( i = mAliasList.begin(); i != mAliasList.end(); i++ )
   {
      if ( test_name == *i )
      {
         return true; // the test_name is in our list
      }
   }

   // there were no matches
   return false;
}

// for debugging output
// put all the alias names into one string so they can be used with vprDEBUG
std::string NetConnection::printAliasNamesToString()
{
   std::string out;
   std::vector<std::string>::iterator i;
   for ( i = mAliasList.begin(); i != mAliasList.end(); i++ )
   {
      out += *i + (i != mAliasList.end() ? ", " : "");
   }
   return out;
}

}  // end namespace gadget
