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
#include <vpr/System.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>
#include <jccl/RTRC/ConfigChunkHandler.h>
#include <jccl/RTRC/ConfigManager.h>

#include <gadget/InputManager.h>
#include <gadget/Util/Debug.h>
#include <gadget/RemoteInputManager/NetUtils.h>
#include <gadget/RemoteInputManager/RecvBuffer.h>
#include <gadget/Type/NetPosition.h>
#include <gadget/Type/NetDigital.h>
#include <gadget/RemoteInputManager/RemoteInputManager.h>


namespace gadget
{

// SingletonImp(RemoteInputManager);

RemoteInputManager::RemoteInputManager(InputManager* input_manager)
{
   mAcceptThread = NULL;
   mListenPort = 7394; // will always be changed
   mInputManager = input_manager;

   // store my local hostname
   const int host_str_size = 64;
   char host_str[host_str_size];
   gethostname(host_str, host_str_size);
   mLongHostname = host_str;

   // now strip the tail of the address to just get the computer name
   mShortHostname = getShortHostnameFromLong(mLongHostname);

   // mHostnameFromConfig = "";

   // mConfigMutex is unlocked to start with
   // mMetStartingRequirements = 0;
   // mMinConnections = 6;
   mListenWasInitialized = 0;
   mRmtMgrChunkExists = RIM_UNKNOWN;
   // mNetworkInitted = false;
   // setWaitingForHostsFlagOff();
   mManagerId.generate();
   vprDEBUG(gadgetDBG_INPUT_MGR,2) << "Remote Input Manager Id: "
                                   << mManagerId << std::endl
                                   << vprDEBUG_FLUSH;
   mNetClockSync.setClockSrcId(mManagerId);
}

RemoteInputManager::~RemoteInputManager()
{
   shutdown();  // kills thread, removes devices
}


// need to do this before we can accept any connections in accept loop
bool RemoteInputManager::startListening()
{
   if ( mAcceptThread == NULL )
   {     // if we haven't started already
      if ( mListenPort > 0 )
      {
         mListenAddr.setPort(mListenPort);

         // start a thread to monitor port
         vpr::ThreadMemberFunctor<RemoteInputManager>* memberFunctor =
         new vpr::ThreadMemberFunctor<RemoteInputManager>
         (this, &RemoteInputManager::acceptLoop, NULL);

         mAcceptThread = new vpr::Thread(memberFunctor);
         vprASSERT(mAcceptThread->valid());
         return true;
      }
      else
      {
         vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL)
            << "RemoteInputManager:startListening: cannot listen on port: "
            << mListenPort << std::endl << vprDEBUG_FLUSH;
         return false;
      }
   }
   else
   {
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL)
         << "RemoteInputManager:startListening: listening thread already active. "
         << std::endl << vprDEBUG_FLUSH;
      return false;
   }
}

void RemoteInputManager::acceptLoop(void* nullParam)
{
   // this->initNetwork(); // needed for windows
   vpr::ReturnStatus status;
   vpr::SocketStream* client_sock = new vpr::SocketStream;

   // Create an acceptor socket that listens on port.
   vpr::SocketStream sock(mListenAddr, vpr::InetAddr::AnyAddr);
   // Open in server mode.
   if ( sock.openServer().success() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrCYAN, "RemoteInputManager")
         << " listening on port: "
         << clrOutNORM(clrMAGENTA, mListenAddr.getPort() ) << std::endl
         << vprDEBUG_FLUSH;
      //vpr::SocketStream client_sock;
      //vpr::Status status;
      //ssize_t bytes;
      // char buffer[] = "Hello there!";
      //        std::string buffer = "Hello there!";
      status = sock.setReuseAddr(true);
   }
   else
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:") << "RemoteInputManager: unable to open listening socket on port: " << mListenAddr.getPort() << std::endl << vprDEBUG_FLUSH;
      exit(0);
   }


   while ( 1 )
   {
      // Wait for an incoming connection.
      status = sock.accept(*client_sock, vpr::Interval::NoTimeout);// vpr::Interval(60, vpr::Interval::Sec));
      if ( status.success() )
      {
         // Using the new socket, send the buffer to the client and close                // the socket.
         // status = client_sock.write(buffer, sizeof(buffer), bytes);
         //                status = client_sock.write(buffer, buffer.length(), bytes);
         // client_sock.close();
         // handshake, get the device name

         mConfigMutex.acquire();   // so we don't add/remove devices/connections while they are in use

         std::string streamHostname,streamManagerId; // name of device requested by the stream
         int streamPort;
         char local_port_str[64];
         sprintf(local_port_str, "%i", mListenPort);

         if ( receiveHandshake(streamHostname, streamPort, streamManagerId, client_sock) )
         {  // read the name from stream
            vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL)
               << " Adding connection: " << streamHostname << ", "
               << streamPort << ", " << streamManagerId << std::endl
               << vprDEBUG_FLUSH;

            // create an alias_name to pass to addConnection
            char stream_port_str[32];
            sprintf(stream_port_str, "%d", streamPort);
            std::string streamAlias = streamHostname + ":" + stream_port_str;

            NetConnection* new_connection = addConnection(streamAlias, streamHostname, streamPort, streamManagerId, client_sock);  // pass the host:port as the alias_name also.
            if ( new_connection != NULL )
            {
               // if connection is successfully added, send handshake
               this->sendHandshake(mShortHostname, local_port_str, mManagerId.toString(), client_sock);   // send my name: send my hostname & port
               mNetClockSync.syncWithMyClock(new_connection);
               client_sock = new vpr::SocketStream;
            }
            else
            {
               // connection already exists or could not be made, so send rejection
               sendRejectionHandshake(mShortHostname, local_port_str, mManagerId.toString(), client_sock);
               // prepare new socket
               client_sock->close();
               delete client_sock;
               client_sock = new vpr::SocketStream;
            }
         }

         else
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
               << "failed connnection handshake" << vprDEBUG_FLUSH;
            sendRejectionHandshake(mShortHostname, local_port_str, mManagerId.toString(), client_sock);   // send my manager's name
            // failed handshake so prepare new socket
            client_sock->close();
            delete client_sock;
            client_sock = new vpr::SocketStream;
         }

         mConfigMutex.release();   // so we don't add/remove devices while the list is in use

      }
      else if ( status == vpr::ReturnStatus::Timeout )
      {
         // will never happen since timeout is infinite
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "RemoteInputManager::No connections within timeout period!\n"
            << vprDEBUG_FLUSH;

         client_sock->close();
         delete client_sock;
         client_sock = new vpr::SocketStream;
      }
   }   // end infinite while
}


void RemoteInputManager::shutdown()
{
   if ( mAcceptThread )
   {
      mAcceptThread->kill();
      mAcceptThread = NULL;
   }

   while ( mConnections.size() > 0 )
   {
      delete *(mConnections.begin()) ;
      mConnections.pop_front();
   }
}


// send and receive data from network devices
void RemoteInputManager::updateAll()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "RemoteInputManager::updateAll()\n" << vprDEBUG_FLUSH;

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
      << "RemoteInputManager: num_connections:" << mConnections.size()
      << "\n" << vprDEBUG_FLUSH;
   std::list<NetConnection*>::iterator i;

   for ( i = mConnections.begin(); i != mConnections.end(); i++ )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL) << "\t"
         << (*i)->getName() << "\n" << vprDEBUG_FLUSH;
   }

   //E while(! (this->checkStartingRequirements()))       // loop here until minimum connections have been accepted
   //E return; // System::sleep(1);  // let devices get loaded -- Should be a yield() instead of sleep

   mConfigMutex.acquire();  // Don't allow devices to be added or removed when we are accessing them

   // if any devices are still requesting to be connected, do that here
   resendRequestsForNackedDevices();

   // local devices sending data
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "Sending data from local net devices.\n" << vprDEBUG_FLUSH;
   sendDeviceNetData();
   sendEndBlocks();  // tell other instance that everything has been sent in this iteration

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "Receiving data from remote net devices.\n" << vprDEBUG_FLUSH;
   receiveDeviceNetData();

   updateManagerStatus();  // for any messages just received that require separate negotiation (i.e. Time Sync)

   mConfigMutex.release();  // Don't allow devices to be added or removed when we are accessing them

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "RemoteInputManager::updateAll()  finished.\n" << vprDEBUG_FLUSH;
}

void RemoteInputManager::receiveDeviceNetData()
{
   // receive network data while all devices not finished receiving data confirmations
   // vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
   //    << "Entering receiveDeviceNetData" << std::endl << vprDEBUG_FLUSH;
   do
   {
      // vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
      //    << "Beginning of loop in receiveDeviceNetData" << std::endl
      //    << vprDEBUG_FLUSH;
      std::list<NetConnection*>::iterator i = 0;
      for ( i = mConnections.begin(); i != mConnections.end();i++ )
      {
         // mSockStream->recvn(mData->getDataPtr(), mData->getDataByteLength(), bytes_read);

         // only try to read more network data if we need it this iteration
         if ( ! (*i)->allDataReceived() )
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
               << "RemoteInputManager::receiveNetworkData(): "
               << (*i)->getName() << " start." << std::endl
               << vprDEBUG_FLUSH;

            if ( this->processRecvBuff(*i) )
            {
               (*i)->setDataReceived();     // see if there are any leftover messages in buffer from last time.
            }
            else
            {
               (*i)->receiveNetworkData();  // read new network messages
            }
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
               << "RemoteInputManager::receiveNetworkData(): "
               << (*i)->getName() << " end." << std::endl
               << vprDEBUG_FLUSH;

            //(*i)->mSockStream->recv((*i)->recv_buffers->getWritePtr(), (*i)->recv_buffers->getNumWriteableSize(), bytes_read);
            //(*i)->recv_buffers->increaseReadableSize(bytes_read);

            // retrieve messages from recv buffer, which also frees space for next message
            //if (this->processRecvBuff(*i))
            //   (*i)->setDataReceived();
         }
      }
   } while ( !this->allDataReceived() );

   markDataUnreceived();  // prepare for next time by setting flag

   // mData->updateFromRemoteSource();   // convert raw data into specific net device data: digital, pos, etc.

   /*
   // Send confirmation byte
   char a = 1;
   ssize_t bytes_sent = 0;
   //C std::cout << "Sending confirmation byte" << std::endl;
   while(bytes_sent < 1){
     mSockStream->send((void*) &a, 1, bytes_sent);
   }
   */
   //C std:: cout << "Confirmation byte sent" << std::endl;
}

bool RemoteInputManager::processRecvBuff(NetConnection* net_connection)
{
   if ( net_connection == NULL )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "processRecvBuff: net_connection IS NULL! " << std::endl
         << vprDEBUG_FLUSH;
      return false;
   }

   static const int OPCODE_SIZE = 2;   // the number of bytes in a short integer
   RecvBuffer* recv_buff = net_connection->getRecvBuff();

   if ( recv_buff == NULL )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "processRecvBuff: net_connection " << net_connection->getName()
         << " returns NO RECV BUFFER! " << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   char opcode_str[OPCODE_SIZE + 1];
   ushort opcode;
   // loop while there is still enough space for and opcode and a terminating semicolon;
   // we will also break out of while loop if part of a message was not received yet.
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
      << "processRecvBuff " << net_connection->getName() << ": buff size = "
      << recv_buff->getReadableSize() << std::endl << vprDEBUG_FLUSH;

   while ( recv_buff->getReadableSize() >= OPCODE_SIZE + 1 )
   {
      vprASSERT(recv_buff != NULL);
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
      << "processRecvBuff:while: Readable Bytes:"
      << recv_buff->getReadableSize() << std::endl << vprDEBUG_FLUSH;
      // FORMAT OF MESSAGES:  2 chars + semicolon == 3 bytes for the minimum message
      int processed_bytes = 0;
      // 0 - 399 can be device ids used when sending data
      // 400 - 511  is for control messages such as initiating device connections
      // 400 = requestDevice
      // 410 = end of an update sequence
      // if more than 400 devices are needed, change the code in this function and the send function to use more bytes

      // strncpy(opcode_str, recv_buff->getReadPtr(), OPCODE_SIZE); // Duh, can't use strncpy on raw data
      for ( int i = 0; i < OPCODE_SIZE; i++ )
      {
         opcode_str[i] = (recv_buff->getReadPtr())[i];
      }

      // opcode = ((int)opcode_str[1]) * 256 + (int)opcode_str[0];
      opcode = binaryToUshort(opcode_str,2);
      opcode = ntohs(opcode);
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "processRecvBuff: opcode = " << opcode << std::endl
         << vprDEBUG_FLUSH;

      processed_bytes = OPCODE_SIZE; // so far we have used at least 2 bytes

      std::string device_name;
      char* ptr;
      int len;
      // char tmp_device_name[64];
      // int name_length;
      Input* dev_ptr;
      Proxy* proxy_ptr;
      NetInput* tmp_net_input;
      int bytes_used;
      vprASSERT(recv_buff != NULL);
      switch ( opcode )
      {
         case MSG_DEVICE_REQ:  // device requested

            // extra data from buffer
            bytes_used = mMsgPackage.receiveDeviceRequest(recv_buff->getReadPtr() + OPCODE_SIZE,
                                                          recv_buff->getReadableSize() - OPCODE_SIZE);
            if ( bytes_used > 0 )
            {
               recv_buff->markAsRead(bytes_used + OPCODE_SIZE);
               recv_buff->eraseAlreadyReadBytes();  // make more space availabe for recving.

               std::string device_name = mMsgPackage.getDataString();

               // find the device in the input manager
               dev_ptr = mInputManager->getDevice(device_name);  // see if we have the device requested
               proxy_ptr = mInputManager->getProxy(device_name);

               // check if device was not found
               if ( dev_ptr != NULL )
               {
                  // Check if there is already an active remote connection, otherwise create one.
                  configureTransmittingNetInput(device_name, mMsgPackage.getSenderId(), net_connection); // important to allocate a structure to hold network data for this local device
                  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
                     << "processRecvBuff:Received request for device: "
                     << device_name << std::endl << vprDEBUG_FLUSH;
                  // break;         // device not found
               }
               else if ( proxy_ptr != NULL )
               {
                  // Check if there is already an active remote connection, otherwise create one.
                  configureTransmittingNetInput(device_name, mMsgPackage.getSenderId(), net_connection); // important to allocate a structure to hold network data for this local device
                  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
                     << "processRecvBuff:Received request for proxy: "
                     << device_name << std::endl << vprDEBUG_FLUSH;
               }
               else
               {
                  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
                     << clrOutNORM(clrCYAN,"WARNING:")
                     << "RemoteInputManager: remote device/proxy request: ("
                     << device_name << ") not found" << std::endl
                     << vprDEBUG_FLUSH;
                  // exit(0);  // will keep being retried
                  break;
               }

            }
            else
            {
               recv_buff->eraseAlreadyReadBytes();  // make more space availabe for recving.
               return false; // message wasn't completely read, we'll wait for more packets to arrive
            }

            /*
             // format:  opcode(2) id(2) name(n) ;

             ptr = recv_buff->getReadPtr() + OPCODE_SIZE; // will read device name that is located after opcode
             len = recv_buff->getReadableSize() - OPCODE_SIZE;

             endline = 0;
             // read device name from buffer
             for(index = 0; index < len; index++){
                 if (ptr[index] == ';'){
                     endline = 1;
                     index++;  // we want to count the semicolon as a character that's been read
                     break;      // we at least
                 }
             }
             if(!endline){ // couldn't read a full line, so try again later when there will be more data
                 vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                    << "processRecvBuff: couldn't read full line yet"
                    << std::endl << vprDEBUG_FLUSH;
                 return 0;
             }

             // read the name of the device
             name_length = index - 1;  // don't include the semicolon in the name
             strncpy(tmp_device_name, ptr, name_length);
             tmp_device_name[name_length] = '\0';  // terminate string
             device_name = tmp_device_name;
             // find the device in the input manager
             dev_ptr = mInputManager->getDevice(device_name);  // see if we have the device requested

             // check if device was not found
             if(dev_ptr == NULL){
                 processed_bytes += index;
                 recv_buff->markAsRead(processed_bytes);
                 vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                    << "processRecvBuff: device not found:" << device_name
                    << std::endl << vprDEBUG_FLUSH;
                 break;         // device not found
             }

             // Check if it already has an active remote connection, otherwise create one.
             configureTransmittingNetInput(device_name, net_connection); // important to allocate a structure to hold network data for this local device
             vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                << "processRecvBuff:Received request for device: "
                << device_name << std::endl << vprDEBUG_FLUSH;
             recv_buff->markAsRead(processed_bytes + index);
             recv_buff->eraseAlreadyReadBytes();  // make more space availabe for recving.
             */
            // break;
         case MSG_DEVICE_ACK:
            bytes_used = mMsgPackage.receiveDeviceAck(recv_buff->getReadPtr() + OPCODE_SIZE,
                                                      recv_buff->getReadableSize() - OPCODE_SIZE);
            if ( bytes_used > 0 )
            {
               recv_buff->markAsRead(bytes_used + OPCODE_SIZE);
               recv_buff->eraseAlreadyReadBytes();  // make more space availabe for recving

               tmp_net_input = this->findDeviceByLocalId(mMsgPackage.getReceiverId());
               if ( tmp_net_input != NULL )
               {
                  tmp_net_input->setRemoteId(mMsgPackage.getSenderId());
                  tmp_net_input->setWasInitializedTrue();
                  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
                     << "setWasInitializedTrue() (Ack)" << std::endl
                     << vprDEBUG_FLUSH;
               }
               else
               {
                  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
                     << "processRecvBuff:recvAck: Unable to match LocalId: "
                     << mMsgPackage.getReceiverId() << std::endl
                     << vprDEBUG_FLUSH;
               }
            }
            else
            {
               recv_buff->eraseAlreadyReadBytes();  // make more space availabe for recving.
               return false; // message wasn't completely read, we'll wait for more packets to arrive
            }
            break;
         case MSG_DEVICE_NACK:
            // parse the same way an ACK is parsed
            bytes_used = mMsgPackage.receiveDeviceAck(recv_buff->getReadPtr() + OPCODE_SIZE,
                                                      recv_buff->getReadableSize() - OPCODE_SIZE);
            if ( bytes_used > 0 )
            {
               recv_buff->markAsRead(bytes_used + OPCODE_SIZE);
               recv_buff->eraseAlreadyReadBytes();  // make more space availabe for recving

               tmp_net_input = this->findDeviceByLocalId(mMsgPackage.getReceiverId());
               if ( tmp_net_input != NULL )
               {
                  tmp_net_input->setRemoteId(mMsgPackage.getSenderId());
                  tmp_net_input->setNeedToResendRequestFlag();
                  // tmp_net_input->setWasInitializedTrue();
                  // vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                  //    << "setWasInitializedTrue() (Ack)" << std::endl
                  //    << vprDEBUG_FLUSH;
               }
               else
               {
                  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
                     << "processRecvBuff:recvAck: Unable to match LocalId: "
                     << mMsgPackage.getReceiverId() << std::endl
                     << vprDEBUG_FLUSH;
               }
            }
            else
            {
               recv_buff->eraseAlreadyReadBytes();  // make more space availabe for recving.
               return false; // message wasn't completely read, we'll wait for more packets to arrive
            }
            break;
         case MSG_END_BLOCK:
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
               << "processRecvBuff: received end block code" << std::endl
               << vprDEBUG_FLUSH;
            ptr = recv_buff->getReadPtr() + OPCODE_SIZE;
            len = recv_buff->getReadableSize() - OPCODE_SIZE;
            if ( len > 0 )
            {
               if ( ptr[0] == ';' )
               {
                  recv_buff->markAsRead(processed_bytes + 1);
                  recv_buff->eraseAlreadyReadBytes();  // make more space availabe for recving.
                  return true;  // ONLY RETURN TRUE IF WE HAVE REACHED THE END OF A BLOCK
               }
            }
            break;
         default:
            // vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            //    << "processRecvBuff: unrecognized code:" << opcode
            //    << std::endl << vprDEBUG_FLUSH;
            break;
      }

      if ( opcode <= MAX_DEVICE_ID )
      { // then opcode is the local_id of the device
         // read rest of data here
         NetInput* net_input_recvr = net_connection->findReceivingNetInputByLocalId(opcode);
         if ( net_input_recvr == NULL )
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
               << "processRecvBuff: Local net device " << opcode
               << " not found." << std::endl << vprDEBUG_FLUSH;
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
               << "processRecvBuff: readable size: "
               << recv_buff->getReadableSize() << std::endl << vprDEBUG_FLUSH;
            // for(int i = 0; i < recv_buff->getReadableSize(); i++)
            // std::cout << recv_buff->getReadPtr()[i];
            exit(0);
         }

         // make sure buffer has enough bytes to contain all the data
         if ( net_input_recvr->getDataByteLength() <= recv_buff->getReadableSize() )
         { // opcode size is included in getDataByteLength, so we won't subtract it from getReadableSize()
            net_input_recvr->updateFromRemoteSource(recv_buff->getReadPtr(), recv_buff->getReadableSize());
            // XXX DEBUG
            if ( dynamic_cast<NetDigital*> (net_input_recvr) )
            {
               NetDigital* net_dig = dynamic_cast<NetDigital*> (net_input_recvr);
               if ( net_dig != NULL )
               {
                  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
                     << "checking Data **************: "
                     << net_dig->getDigitalData().getDigital() << std::endl
                     << vprDEBUG_FLUSH;
               }
            }
            // XXX END DEBUG
            recv_buff->markAsRead(net_input_recvr->getDataByteLength());
         }
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
            << "Data received from " << opcode << std::endl << vprDEBUG_FLUSH;
      }

      // recv_buff->markAsRead(processed_bytes);  // moves read ptr
   }

   vprASSERT(recv_buff != NULL);

   recv_buff->eraseAlreadyReadBytes();  // make more space availabe for recving.
   return false;  // haven't received end block yet
}

// makeConnection
// attempts to connect to a running RemoteInputManager at connection_hostname:connection_port:
// if successful, it is added to our list of NetConnections and true is returned
// if unsuccessful, false is returned and this function will be called again later.
bool RemoteInputManager::makeConnection(const std::string& connection_alias, const std::string& connection_hostname, const int connection_port)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << "------------MAKECONNECTION: " << connection_alias << ", "
      << connection_hostname <<":"<< connection_port << std::endl
      << vprDEBUG_FLUSH;
   // this->initNetwork(); // needed for windows
   vpr::SocketStream* sock_stream;
   vpr::InetAddr inet_addr;

   // CONNECTING (code from VPR example)
   if ( ! inet_addr.setAddress(connection_hostname, connection_port).success() )
   {
      // if ( ! inet_addr.setAddress(connection_name).success() ) {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "Failed to set address " << connection_hostname <<":"
         << connection_port << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   sock_stream = new vpr::SocketStream(vpr::InetAddr::AnyAddr, inet_addr);

   if ( sock_stream->open().success() )
   {
      // Try to Connect to the server.
      if ( sock_stream->connect().success() )
      {
         // std::cout << "Repeatedly attempting to connect to: " << mDeviceName << " at " << mHostName << ":" << mHostPort << std::endl;
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "RemoteInputManager: Connected to " << connection_hostname
            <<":"<< connection_port << std::endl << vprDEBUG_FLUSH;
      }
      else
      {
         // std::cout << "initConnections(): Failed to connect to " << inet_addr << std::endl;
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "RemoteInputManager: Unable to make connection to "
            << connection_hostname <<":"<< connection_port
            << ".  Will retry to connect." << std::endl << vprDEBUG_FLUSH;
         return false;
      }
   }
   else
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "RemoteInputManager: Failed to open socket" << std::endl
         << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "RemoteInputManager: Unable to make connection to "
         << connection_hostname <<":"<< connection_port
         << ".  Will retry to connect." << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL) << "connected, yeah\n"
                                                   << vprDEBUG_FLUSH;
   char local_port_str[64];
   sprintf(local_port_str, "%i", mListenPort);
   this->sendHandshake(mShortHostname, local_port_str, mManagerId.toString(), sock_stream);   // send my name: send my hostname & port
   std::string received_hostname, received_manager_id;
   int received_port;
   if ( this->receiveHandshake(received_hostname, received_port, received_manager_id, sock_stream) )
   { // receive parameters from other manager, but to addConnection we'll actually use the variable names passed in from above
      NetConnection* new_connection = this->addConnection(connection_alias, connection_hostname, connection_port, received_manager_id, sock_stream);    // hostname and port are saved to identify the connection.
      vprASSERT(new_connection != NULL); // addConnection should never fail since we requested connection
      mNetClockSync.syncWithOtherClock(new_connection);
   }
   return true;
}

// config from a Remote Input Manager chunk
bool RemoteInputManager::config(jccl::ConfigChunkPtr chunk)
{
   vprASSERT(mListenWasInitialized == false);

   // vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CRITICAL_LVL)
   //    << "RemoteInputManager:config" << std::endl << vprDEBUG_FLUSH;
   mListenPort = chunk->getProperty<int>("listen_port");

   // int num_connections = chunk->getProperty("num_connections");
   mInstanceName = chunk->getFullName();

   std::string forcedHostname = chunk->getProperty<std::string>("forced_hostname");

   if ( forcedHostname.size() > 0 )
   {
      mLongHostname = forcedHostname;
      mShortHostname = getShortHostnameFromLong(mLongHostname);
   }

   /*
   for(int i = 0; i < num_connections; i++)
   {
      std::string connection_name = chunk->getProperty<std::string>( "connections" , i);
      makeConnection( connection_name );
   }
   */

   mListenWasInitialized = true;
   startListening();
   return true;
}

// No manager config chunk exists so we get our listening port info from a "Remote Connection" entry
// that lists our local hostname and a port number.
bool RemoteInputManager::configFromLocalConnection(jccl::ConfigChunkPtr chunk)
{
   vprASSERT(mListenWasInitialized == false);

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
      << "configFromLocalConnection" << std::endl << vprDEBUG_FLUSH;

   // strip the port from the chunk name
   // std::string connection_name = chunk->getProperty<std::string>("hostname_n_port");
   std::string connection_name = chunk->getProperty<std::string>("hostname");
   int connection_port = chunk->getProperty<int>("port");

   //R std::string::size_type found_pos = connection_name.find(":");
   //R if(found_pos == std::string::npos){ // colon not found
   //R vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
   //R    << "configFromLocalConnection: ERROR configuring hostname:port  "
   //R    << connection_name << std::endl << vprDEBUG_FLUSH;
   //R return false;
   //R }

   //R finishing assigning listening port
   //R int port_str_len = found_pos + 1;
   //R std::string port = connection_name.substr(port_str_len, connection_name.size() - port_str_len);
   //R mListenPort = atoi(port.c_str());
   mListenPort = connection_port;

   std::string connection_alias = chunk->getName();
   mInstanceName = connection_alias;

   // finish initializing RemoteInputManager and listen for connections.
   mListenWasInitialized = true;
   startListening();                // will start listening thread unless listening port is zero or a thread is already listening
   return true;
}

//
bool RemoteInputManager::configConnection(jccl::ConfigChunkPtr chunk)
{
   std::string connection_hostname = chunk->getProperty<std::string>("hostname");
   int connection_port = chunk->getProperty<int>("port");
   // std::string connection_name = connection_host + connection_port;
   // std::string connection_ip = getIpFromHostname(connect_name);
   //int connection_port_int = atoi(connection_port.c_str());

   //R if(!this->nameContainsColon(connection_name)){    // note if no port is specified, don't try to create a connection to it
   if ( connection_port == 0 )
   {                           // note if no port is specified, don't try to create a connection to it
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "RmtInptMgr::configConnection(): listening port == 0.  Won't create connection with this chunk."
         << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   if ( this->hostnameAndPortMatchLocal(connection_hostname, connection_port) )
   {
      // this entry contains our local hostname
      if ( mListenWasInitialized == false )      // use this to configure our listening port
      {
         return configFromLocalConnection(chunk);
      }
      else
      {  // listening port already configured

         //R if( this->hostPortMatchesListeningPort(connection_name)){
         if ( mListenPort == connection_port )
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
               << "RmtInptMgr::configConnection(): Config chunk Host listening port already being used by this host's Remote Input Manager."
               << std::endl << vprDEBUG_FLUSH;
            return false;
         }
         // else fall through and create connection to localhost with a different port
      }
   }

   std::string connection_alias = chunk->getName();

   // vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CRITICAL_LVL) << "RemoteInputManager:config: trying to connect to:" << connection_name << std::endl << vprDEBUG_FLUSH;

   if ( makeConnection( connection_alias, connection_hostname, connection_port) )
      return true;
   else
   {
      return true;  // XXX for now, don't retry connections -- hosts need to be started in correct order
      //if((getConnectionByHostAndPort(connection_name) != NULL )    // check if we have connection already
      if ( (getConnectionByHostAndPort(connection_hostname, connection_port) != NULL )    // check if we have connection already
           || (getConnectionByAliasName(connection_alias) != NULL)   // check if we have connection already
           // || (stripPortFromName(connection_name) == 0) ){
           || (connection_port == 0) )
      {           // check if we don't need to connect -- no listening port or port == 0 is always viewed as a new connection to allow for multiple generic connections
         //   so we don't need to try to connect to a host with listening port of zero (which is invalid anyway)
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "getConnectionByHostAndPort()"
            << getConnectionByHostAndPort(connection_hostname, connection_port)
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "getConnectionByAliasName()"
            << getConnectionByAliasName(connection_alias) << std::endl
            << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "connnection_port == 0?" << (connection_port == 0) << std::endl
            << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "RemoteInputManager: Connection " << connection_hostname
            <<":"<< connection_port
            << " already established ( or connecting port is zero)"
            << std::endl << vprDEBUG_FLUSH;

         // Allow a connection to have more than one name (alias)

         NetConnection* connection_by_address = getConnectionByHostAndPort(connection_hostname, connection_port);
         NetConnection* connection_by_alias = getConnectionByAliasName(connection_alias);
         if ( connection_by_address != NULL )
         {    // if we have a connection at this host:port
            if ( connection_by_alias == NULL )          // and we don't have a connection matched with this alias yet
            {
               connection_by_address->addAliasName(connection_alias);  // then store the alias with the connection
            }
         }

         return true;  // connection already exists.  Returning true will prevent us from retrying to connect.
      }
      else
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL) << "RemoteInputManager: makeConnection to " << connection_hostname<<":"<<connection_port << " failed." << std::endl << vprDEBUG_FLUSH;
         jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();
         cfg_mgr->delayStalePendingList();  // delay config becoming STALE, so we can retry to connect
         // setWaitingForHostsFlagOn(); // flag checked by Input Manager to see if we are not done configuring
         return false;  // we will keep trying and not allow Juggler to stop configuring until th connection is made.
      }
   }
}

bool RemoteInputManager::sendHandshake(const std::string& host, const std::string& port, const std::string& manager_id, vpr::SocketStream* newStream)
{
   char str[64];
   sprintf(str, "vjNet:1:%s:%s:%s;", host.c_str(), port.c_str(), manager_id.c_str());
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
      << "handshake to send: " << str << ".  " << strlen(str) << " bytes."
      << std::endl << vprDEBUG_FLUSH;
   sendAtOnce(*newStream, str, strlen(str));
   return true;
}

bool RemoteInputManager::sendRejectionHandshake(const std::string& host, const std::string& port, const std::string& manager_id, vpr::SocketStream* newStream)
{
   char str[64];
   sprintf(str, "vjNet:0:%s:%s:%s;", host.c_str(), port.c_str(), manager_id.c_str());
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
      << "Rejection handshake to send: " << str << ".  " << strlen(str)
      << " bytes." << std::endl << vprDEBUG_FLUSH;
   sendAtOnce(*newStream, str, strlen(str));
   return true;
}

// Receives handshake on newStream and stores newly received hostname and port values in receivedHostname and receivedPort
bool RemoteInputManager::receiveHandshake(std::string& receivedHostname, int& receivedPort,std::string& received_manager_id, vpr::SocketStream* newStream)
{
   const int HSHAKE_BUFFSIZE = 256;
   char buffer[HSHAKE_BUFFSIZE];
   buffer[0] = '\0';
   unsigned int bytes_read = 0; // ssize_t bytes_read = 0;
   unsigned int temp_bytes_read = 0; //ssize_t temp_bytes_read = 0;
   int buff_size;
   receivedHostname = "";
   receivedPort = 0;

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
      << "RemoteInputManager: entering receiveHandshake() " << std::endl
      << vprDEBUG_FLUSH;

   // Handshake message in the form:   "vjNet:accept/reject byte:others_manager_name;"
   // wait for entire message -- ended with a semicolon
   while ( (strlen(buffer) < 9) ||      // minimum message: vjNet:x:;  // if no name is sent
           ((strstr(buffer, "vjNet:") == NULL) || (strstr(buffer, ";") == NULL)) )  // partially received
   {
      // only allow ONE byte to be read at a time.  Otherwise, we'll accidently recv part of the
      // next message(s) that need(s) to be processed elsewhere.
      if ( HSHAKE_BUFFSIZE - bytes_read - 1 > 0 )    // - 1 is for string terminating character
      {
         buff_size = 1;
      }
      else
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:") << " RemoteInputManager: Handshake buffer size is too small.  Increase its size in receiveHandshake()." << std::endl << vprDEBUG_FLUSH;
         buff_size = 0;   // buffer is full
      }
      // recv bytes into the buffer
      newStream->recvn((void*) (buffer + bytes_read), buff_size, temp_bytes_read);
      bytes_read += temp_bytes_read;

      // buffer[bytes_read] = '\0';
      // std::cout << "temp_bytes_read: " << temp_bytes_read << std::endl;
      // std::cout << "handshake so far: " << buffer << std::endl;
   }

   // check if the handshake sent was a rejection
   if ( buffer[6] == '0' )
   {
      return false;
   }
   // else buffer[6] == 1, and continue.

   // read connection name

   int i;
   // the name starts on the 8th byte
   for ( i = 8; buffer[i] != ':' && buffer[i] != ';'; i++ )
   {
      receivedHostname += buffer[i];
   }

   i++; // move iterator past colon

   std::string port_str;
   for ( ; buffer[i] != ':' && buffer[i] != ';'; i++ )
   {
      port_str += buffer[i];
   }
   receivedPort = atoi(port_str.c_str());

   i++; // move iterator past colon

   received_manager_id = "";
   for ( ; buffer[i] != ':' && buffer[i] != ';'; i++ )
   {
      received_manager_id += buffer[i];
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
      << "Handshake received connection: " << receivedHostname <<":"
      << receivedPort <<":"<< received_manager_id << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
      << "Handshake, buffer[i]= " << buffer[i] << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
      << "Handshake, last character recv'd: " << buffer[bytes_read - 1]
      << std::endl << vprDEBUG_FLUSH;

   if ( receivedHostname.length() > 0 )
   {
      return true;
   }
   else
   {
      return false;
   }
}

/*
Input* RemoteInputManager::findDevice(std::string device_name)
{
   tDevTableType::iterator dev_found;
   dev_found = mDevTable.find(instName);
   if(dev_found != mDevTable.end())
   {
      return dev_found->second;
   }
   else
   {
      return getProxy(device_name);
   }
}*/

bool RemoteInputManager::configureTransmittingNetInput(std::string device_name, VJ_NETID_TYPE requester_device_id, NetConnection* net_connection)
{
   // first check if we have already set up this device to transmit on the network
   NetInput* net_input = this->findTransmittingNetInput(device_name);
   if ( net_input != NULL )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL) << "configureTransmittingNetInput: transmitting net input found" << device_name << std::endl << vprDEBUG_FLUSH;
      net_input->addDependency(); // this->addTransmittingNetInput(net_input);  // actually just increments counter if already exists: net_input->addDependency();
      net_connection->addTransmittingNetInput(net_input); // only does something if this net_connection isn't already requesting this Input's data
   }
   else
   {  // net_input == NULL.
      //  We didn't find it yet...so check our local Input Manager
      Input* input_ptr = mInputManager->getDevice(device_name);
      if ( input_ptr != NULL )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "configureTransmittingNetInput: Input found: "
            << device_name << std::endl << vprDEBUG_FLUSH;
         net_input = this->createTransmittingNetInput(device_name, input_ptr, requester_device_id);  // create object that can send data
         // this is done in the create function --> net_connection->addTransmittingNetInput(net_input);
         this->addTransmittingNetInput(net_input, net_connection);
      }
      else
      {  // No devices found, search the proxies
         Proxy* proxy_ptr = mInputManager->getProxy(device_name);
         if ( proxy_ptr != NULL )
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "configureTransmittingNetInput: Proxy found: " << device_name
               << std::endl << vprDEBUG_FLUSH;
            // std::cout << "Going to create NetInput from Proxy: " << device_name << std::endl;
            net_input = this->createTransmittingNetInput(device_name, proxy_ptr, requester_device_id);  // create object that can send data
            // this is done in the create function --> net_connection->addTransmittingNetInput(net_input);
            this->addTransmittingNetInput(net_input, net_connection);
            // mTransmittingDevices.push_back(input_ptr);
         }
         else
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "configureTransmittingNetInput: device " << device_name
               << " not found" << std::endl << vprDEBUG_FLUSH;
         }
      }
   }

   // now send acknowledgement if we have found the device
   if ( net_input != NULL )
   {
      // Send Acknowledgement and Id back to requester
      mMsgPackage.createDeviceAck(net_input->getRemoteId(), net_input->getLocalId(), device_name);
      net_connection->sendMsg(mMsgPackage);
      net_input->setWasInitializedTrue();
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "setWasInitializedTrue()" << device_name << std::endl
         << vprDEBUG_FLUSH;
      return true;
   }
   else
   {
      return false;  // input device not found
   }
}

bool RemoteInputManager::configureReceivingNetInput(jccl::ConfigChunkPtr chunk, NetConnection* net_connection)
{
   std::string device_name = chunk->getFullName();

   // check if we are already receiving this device's data.
   NetInput* net_input = net_connection->findReceivingNetInput(device_name);
   if ( net_input != NULL )
   {  // we already are receiving from this device, so mostly just increment it's dependency count
      // help us avoid receiving data from the same device more than once
      net_input->addDependency(); // net_connection->addReceivingNetInput(net_input); //
      return true;
   }
   else
   {  // setup new structures to facilitate receiving and managing data from remote device
      NetInput* recv_net_input = net_connection->createReceivingNetInput(chunk);
      if ( recv_net_input != NULL )
      {
         this->addReceivingNetInput(recv_net_input, net_connection);
         //mInputManager->addDevice(recv_net_input);
         //net_connection->addReceivingNetInput(recv_net_input);
         return true;
      }
      else
      {
         vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL) << "RemoteInputManager: unable to connect to device: " << device_name << std::endl << vprDEBUG_FLUSH;
         return false;
      }
   }

}

void RemoteInputManager::sendDeviceNetData()
{
   std::list<NetConnection*>::iterator i = 0;
   for ( i = mConnections.begin(); i != mConnections.end();i++ )
   {
      // vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL) << "RemoteInputManager::sendNetworkData() start." << std::endl << vprDEBUG_FLUSH;
      (*i)->sendNetworkData();
      // vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL) << "RemoteInputManager::sendNetworkData() end." << std::endl << vprDEBUG_FLUSH;
   }
}

void RemoteInputManager::findIfMgrChunkExists()
{
   //std::cout << "findIfMgrChunkExists1: " << mRmtMgrChunkExists << "." << std::endl;
   if ( mRmtMgrChunkExists != RIM_UNKNOWN )
   {
      return;
   }

   //std::cout << "findIfMgrChunkExists2: " << mRmtMgrChunkExists << "." << std::endl;

   jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();

   // Check to see if Manager chunk exists

   if ( (cfg_mgr->isChunkTypeInActiveList(std::string("RemoteInputManager")))
        || (cfg_mgr->isChunkTypeInPendingList(std::string("RemoteInputManager"))) )
   {
      mRmtMgrChunkExists = RIM_TRUE;
   }
   else
   {
      mRmtMgrChunkExists = RIM_FALSE;
   }

   // std::cout << "findIfMgrChunkExists3: " << mRmtMgrChunkExists << "." << std::endl;

   if ( mRmtMgrChunkExists == RIM_TRUE )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL)
         << "findIfMgrChunkExists: rmt input mgr chunk exists:TRUE "
         << std::endl << vprDEBUG_FLUSH;
   }
   else if ( mRmtMgrChunkExists == RIM_FALSE )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL)
         << "findIfMgrChunkExists: rmt input mgr chunk exists:FALSE "
         << std::endl << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL)
         << "findIfMgrChunkExists: rmt input mgr chunk exists:UNKNOWN "
         << std::endl << vprDEBUG_FLUSH;
   }
}

// returns if there is a chunk (processed yet or not) that will explicitly configure the Remote Input Manager
bool RemoteInputManager::mgrChunkExists()
{
//      std::cout << "mgrChunkExists" << std::endl;
//      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL) << "mgrChunkExists() " << std::endl << vprDEBUG_FLUSH;

//      std::cout << "mgrChunkExists2" << std::endl;
/*
      if(mRmtMgrChunkExists == RIM_TRUE)
         vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CRITICAL_LVL) << "mgrChunkExists pre: rmt input mgr chunk exists:TRUE " << std::endl << vprDEBUG_FLUSH;
      else if(mRmtMgrChunkExists == RIM_FALSE)
         vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CRITICAL_LVL) << "mgrChunkExists pre: rmt input mgr chunk exists:FALSE " << std::endl << vprDEBUG_FLUSH;
      else
         vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CRITICAL_LVL) << "mgrChunkExists pre: rmt input mgr chunk exists:UNKNOWN " << std::endl << vprDEBUG_FLUSH;
*/
//      std::cout << "mgrChunkExists3" << std::endl;

   if ( mRmtMgrChunkExists == RIM_UNKNOWN )
   {
      findIfMgrChunkExists();
   }

   // std::cout << "mgrChunkExists(): " << mRmtMgrChunkExists << "." << std::endl;

   if ( mRmtMgrChunkExists == RIM_TRUE )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL)
         << "mgrChunkExists post: rmt input mgr chunk exists:TRUE "
         << std::endl << vprDEBUG_FLUSH;
   }
   else if ( mRmtMgrChunkExists == RIM_FALSE )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL)
         << "mgrChunkExists post: rmt input mgr chunk exists:FALSE "
         << std::endl << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL)
         << "mgrChunkExists post: rmt input mgr chunk exists:UNKNOWN "
         << std::endl << vprDEBUG_FLUSH;
   }

   // std::cout << "mgrChunkExists5" << std::endl;

   vprASSERT(mRmtMgrChunkExists != RIM_UNKNOWN);  // mRmtMgrChunkExists should now change

   if ( mRmtMgrChunkExists == RIM_TRUE )
   {
      return true;
   }
   else // mRmtMgrChunkExists == RIM_FALSE
   {
      return false;
   }
}

/*
void RemoteInputManager::initNetwork()
{
    if (mNetworkInitted == true)
        return;

    mNetInitMutex.acquire();

    if (mNetworkInitted == true)  // so other threads waiting at mutex won't try to call WSAStartup
        return;

#if defined __WIN32__ || defined WIN32 || defined _Windows || defined _WIN32

     // Must be done at the beginning of every WinSock program
     WSADATA w;    // used to store information about WinSock version
     int error = WSAStartup (0x0202, &w);   // Fill in w

     if (error)
     { // there was an error
       vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CRITICAL_LVL) << "WSAStartup() called, there was an error." << std::endl << vprDEBUG_FLUSH;
       exit(0);
     }
     if (w.wVersion != 0x0202)
     { // wrong WinSock version!
       vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CRITICAL_LVL) << "WSAStartup() called, wrong winsock version." << std::endl << vprDEBUG_FLUSH;
       WSACleanup (); // unload ws2_32.dll
       exit(0);
     }
     vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CRITICAL_LVL) << "WSAStartup() called, network initted successfully." << std::endl << vprDEBUG_FLUSH;
#else
     ;// vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CRITICAL_LVL) << "network initted" << std::endl << vprDEBUG_FLUSH;
#endif

    mNetworkInitted = true;

    mNetInitMutex.release();
}
*/

// addConnection if it doesn't exist already
NetConnection* RemoteInputManager::addConnection(const std::string &connection_alias, const std::string& connection_hostname, const int connection_port, const std::string& manager_id, vpr::SocketStream* sock_stream)
{
   // make sure connection doesn't exist already:
   if ( getConnectionByManagerId(manager_id) != NULL )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "RemoteInputManager: Connection to " << connection_hostname
         <<" : "<< connection_port <<" : "<< manager_id << " already exists."
         << std::endl << vprDEBUG_FLUSH;
      return NULL;
   }
   else
   { // add connection
      NetConnection* connection = new NetConnection(connection_alias, connection_hostname, connection_port, manager_id, sock_stream);
      mConnections.push_back(connection);
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "RemoteInputManager: Added connection to " << connection_hostname
         <<" : "<< connection_port <<" : "<< manager_id << std::endl
         << vprDEBUG_FLUSH;
      return connection;
   }
}

NetInput* RemoteInputManager::createTransmittingNetInput(const std::string& device_name, Input* input_ptr, VJ_NETID_TYPE requester_device_id)
{
   // check if we should create a digital net device
   if ( dynamic_cast<Digital*>(input_ptr) )
   {
      // create
      NetDigital* net_dig = new NetDigital(device_name, input_ptr, this->generateLocalId(), requester_device_id);
      // store
      NetInput* net_input = dynamic_cast<NetInput*>(net_dig);
      net_input->addDependency();
      return net_input;
   }
   // or check if we should create a positional net device
   else if ( dynamic_cast<Position*> (input_ptr) != NULL )
   {
      NetPosition* net_pos = new NetPosition(device_name, input_ptr, this->generateLocalId(), requester_device_id);
      NetInput* net_input = dynamic_cast<NetInput*>(net_pos);
      net_input->addDependency();
      return net_input;
   }
   else
   {
      return NULL;
   }
}

NetInput* RemoteInputManager::createTransmittingNetInput(const std::string& device_name, Proxy* proxy_ptr, VJ_NETID_TYPE requester_device_id)
{
   // check if we should create a digital net device
   if ( dynamic_cast<DigitalProxy*>(proxy_ptr) )
   {
      // create
      NetDigital* net_dig = new NetDigital(device_name, proxy_ptr, this->generateLocalId(), requester_device_id);
      // store
      NetInput* net_input = dynamic_cast<NetInput*> (net_dig);
      net_input->addDependency();
      return net_input;
   }
   // or check if we should create a positional net device
   else if ( dynamic_cast<PositionProxy*> (proxy_ptr) != NULL )
   {
      NetPosition* net_pos = new NetPosition(device_name, proxy_ptr, this->generateLocalId(), requester_device_id);
      NetInput* net_input = dynamic_cast<NetInput*>(net_pos);
      net_input->addDependency();
      return net_input;
   }
   else
   {
      return NULL;
   }
}

void RemoteInputManager::addTransmittingNetInput(NetInput* net_input, NetConnection* net_connection)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
      << "addTransmittingNetInput()" << std::endl << vprDEBUG_FLUSH;
   net_connection->addTransmittingNetInput(net_input);
   if ( std::find(mTransmittingInputs.begin(), mTransmittingInputs.end(), net_input) == mTransmittingInputs.end() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
         << "addTransmittingNetInput: push_back" << std::endl << vprDEBUG_FLUSH;
      mTransmittingInputs.push_back(net_input);
   }
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "addReceivingNetInput() : " << net_input->getInstanceName()
      << std::endl << vprDEBUG_FLUSH;
   mInputManager->addDevice( dynamic_cast<Input*> (net_input) );
   vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "   Successfully added dev: " << net_input->getInstanceName().c_str()
      << std::endl << vprDEBUG_FLUSH;
}

void RemoteInputManager::addReceivingNetInput(NetInput* net_input, NetConnection* net_connection)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
      << "addReceivingNetInput()" << std::endl << vprDEBUG_FLUSH;
   net_connection->addReceivingNetInput(net_input);
   if ( std::find(mReceivingInputs.begin(), mReceivingInputs.end(), net_input) == mReceivingInputs.end() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
         << "addTransmittingNetInput: push_back" << std::endl << vprDEBUG_FLUSH;
      mReceivingInputs.push_back(net_input);
   }
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "addReceivingNetInput() : " << net_input->getInstanceName()
      << std::endl << vprDEBUG_FLUSH;
   mInputManager->addDevice( dynamic_cast<Input*> (net_input) );
   vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "   Successfully added dev: " << net_input->getInstanceName()
      << std::endl << vprDEBUG_FLUSH;
   //mInputManager->addProxy( dynamic_cast<Input*> (net_input) );
}

bool RemoteInputManager::allDataReceived()
{
   /*
   // check if devices are waiting for data
   for(std::list<NetInput*>::iterator i = mReceivingInputs.begin();
      i!= mReceivingInputs.end(); i++){
      // if((*i)->isWaitingForData()){
         // std::cout << "Waiting for data: " << (*i)->getInstanceName() << std::endl;
         return false;
      }
   }
   */

   // check if connections are waiting for EndBlocks()
   for ( std::list<NetConnection*>::iterator i = mConnections.begin();
       i != mConnections.end(); i++ )
   {
      // std::cout << "Checking for match: " << (*i)->getName() << ", " << location_name << std::endl;
      if ( ! (*i)->allDataReceived() )
      {
         return false;
      }
   }

   return true;
}

void RemoteInputManager::markDataUnreceived()
{
   //for(std::list<NetInput*>::iterator j = mReceivingInputs.begin();
   //   j!= mReceivingInputs.end(); j++){
   //   (*j)->setIsNotWaitingForData();
   //}

   for ( std::list<NetConnection*>::iterator i = mConnections.begin();
       i != mConnections.end(); i++ )
   {
      (*i)->setDataUnreceived();
   }
}

NetInput* RemoteInputManager::findTransmittingNetInput(const std::string& device_name)
{
   for ( std::list<NetInput*>::iterator i = mTransmittingInputs.begin();
       i!= mTransmittingInputs.end(); i++ )
   {
      if ( (*i)->getSourceName() == device_name )
      {
         return *i;
      }
   }
   return NULL;
}

//R NetConnection* RemoteInputManager::getConnectionByHostAndPort(const std::string& location_name){
NetConnection* RemoteInputManager::getConnectionByHostAndPort(const std::string& hostname, const int port)
{
   // Method to check strings
   for ( std::list<NetConnection*>::iterator i = mConnections.begin();
       i != mConnections.end(); i++ )
   {
      // std::cout << "Checking for match: " << (*i)->getName() << ", " << location_name << std::endl;
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL) << "Checking for match: " << (*i)->getName() << ", " << hostname<<":"<<port << std::endl << vprDEBUG_FLUSH;
      if ( ((*i)->getHostname() == hostname) && ((*i)->getPort() == port) )
      {
         return *i;
      }
   }
   return NULL;
}

NetConnection* RemoteInputManager::getConnectionByAliasName(const std::string& alias_name)
{
   // Method to check strings
   for ( std::list<NetConnection*>::iterator i = mConnections.begin();
       i != mConnections.end(); i++ )
   {
      // std::cout << "Checking for match: " << (*i)->printAliasNames() << ", " << alias_name << std::endl;
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL) << "Checking for match: " << (*i)->printAliasNamesToString() << ", " << alias_name << std::endl << vprDEBUG_FLUSH;
      if ( (*i)->hasAliasName(alias_name) )
      {
         return *i;
      }
   }
   return NULL;
}

NetConnection* RemoteInputManager::getConnectionByManagerId(const vpr::GUID& manager_id)
{
   for ( std::list<NetConnection*>::iterator i = mConnections.begin();
         i != mConnections.end(); i++ )
   {
      if ( (*i)->getManagerId() == manager_id )
      {
         return *i;
      }
   }
   return NULL;
}

void RemoteInputManager::sendEndBlocks()
{
   for ( std::list<NetConnection*>::iterator i = mConnections.begin();
       i != mConnections.end(); i++ )
   {
      (*i)->sendEndBlock();
   }
}

NetInput* RemoteInputManager::findDeviceByLocalId(VJ_NETID_TYPE local_id)
{
   std::list<NetInput*>::iterator i;

   // check Transmitting Devices
   for ( i = mTransmittingInputs.begin(); i != mTransmittingInputs.end(); i++ )
   {
      if ( (*i)->getLocalId() == local_id )
      {
         return(*i);
      }
   }

   // check Receiving Devices
   for ( i = mReceivingInputs.begin(); i != mReceivingInputs.end(); i++ )
   {
      if ( (*i)->getLocalId() == local_id )
      {
         return(*i);
      }
   }

   return NULL;
}

void RemoteInputManager::resendRequestsForNackedDevices()
{
   for ( std::list<NetConnection*>::iterator i = mConnections.begin();
       i != mConnections.end(); i++ )
   {
      (*i)->resendRequestsForNackedDevices();
   }
}

void RemoteInputManager::updateManagerStatus()
{
#if JCCL_PERFORMANCE == JCCL_PERF_POSIX
#ifdef GADGET_USING_RIM_SYNC

   // jccl::TimeStamp::resync() not implemented for SPROC yet
   if ( mNetClockSync.syncNeeded() )
   {
      mNetClockSync.sync();
   }
#endif
#endif
}

}  // end namespace gadget
