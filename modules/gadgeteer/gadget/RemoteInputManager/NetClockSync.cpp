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

#include <gadget/RemoteInputManager/MsgPackage.h>
#include <gadget/Util/Debug.h>
#include <gadget/RemoteInputManager/NetClockSync.h>


namespace gadget
{

// send 1 msg, recv 1 msg, send 1 msg
void NetClockSync::firstIterationAsInitiator(SyncConnection& sconn)
{
   /*vprDEBUG(gadgetDBG_INPUT_MGR, 5)
      << "NetClockSync::firstIterationAsInitiator() " << std::endl
      << vprDEBUG_FLUSH;

   MsgPackage msg;
   bool clock_is_synced = true;
   bool other_clock_is_synced;
   float clock1(0), clock2(0), returned_clock1, returned_clock2;

   // first msg is to transfer control to Responder
   msg.createClockSync(clock1, clock2, clock_is_synced);
   sconn.connection->sendMsg(msg);
   msg.clear();

   // receive our delta and other clock time
   uint bytes_to_recv = 2 + 2 * sizeof(float) + 1 + 1;
   uint tmp_bytes_received;
   sconn.connection->getSockStream()->recvn( (char*)&(sconn.recv_buffer[0]),
                                             bytes_to_recv, tmp_bytes_received );
   msg.receiveClockSync( (char*)(&(sconn.recv_buffer[0])) + 2, bytes_to_recv - 2,
                         returned_clock1, returned_clock2, other_clock_is_synced);

   // send return msg
   msg.createClockSync(clock1, returned_clock2, clock_is_synced);
   sconn.connection->sendMsg(msg);
   msg.clear();*/
}

// recv 1 msg, send 1 msg, recv 1 msg
float NetClockSync::firstIterationAsResponder(SyncConnection& sconn, float& returned_clock1)
{
   /*vprDEBUG(gadgetDBG_INPUT_MGR, 5)
      << "NetClockSync::firstIterationAsResponder() " << std::endl
      << vprDEBUG_FLUSH;

   MsgPackage msg;
   bool clock_is_synced = true;
   bool other_clock_is_synced;
   float clock1(0), clock2(0), returned_clock2;

   // recv 1st message to get control
   uint bytes_to_recv = 2 + 2 * sizeof(float) + 1 + 1;
   uint tmp_bytes_received;
   sconn.connection->getSockStream()->recvn( (char*)&(sconn.recv_buffer[0]),
                                             bytes_to_recv,tmp_bytes_received );
   msg.receiveClockSync((char*) (&(sconn.recv_buffer[0])) + 2, bytes_to_recv-2,
                        returned_clock1, returned_clock2,
                        other_clock_is_synced);

   // Now send a fresh clock2
   vpr::Interval clock2_stamp;
   clock2_stamp.setNow();
   clock2 = clock2_stamp.usecf();
   clock1 = returned_clock1;
   msg.createClockSync(clock1, clock2, clock_is_synced);
   sconn.connection->sendMsg(msg);
   msg.clear();

   // recv clock1
   bytes_to_recv = 2 + 2 * sizeof(float) + 1 + 1;
   sconn.connection->getSockStream()->recvn((char*) &(sconn.recv_buffer[0]),
                                            bytes_to_recv ,tmp_bytes_received);
   msg.receiveClockSync((char*)(&(sconn.recv_buffer[0])) + 2, bytes_to_recv-2,
                        returned_clock1, returned_clock2,
                        other_clock_is_synced);

   // calculate delta
   vpr::Interval new_clock2;
   new_clock2.setNow();
   float delta = (new_clock2.usecf()-clock2)/2.0f;

   return delta;*/
}

// recv 1 msg, send 1 msg
// returns true if other comp needs clock and sync should continue
bool NetClockSync::iterationLoopAsInitiator(SyncConnection& sconn)
{
   /*vprDEBUG(gadgetDBG_INPUT_MGR, 5)
      << "NetClockSync::iterationLoopAsInitiator() " << std::endl
      << vprDEBUG_FLUSH;

   MsgPackage msg;
   bool other_clock_is_synced;
   bool clock_is_synced = true;  // we are synced with ourselves, the global clock
   float clock1, clock2, returned_clock1, returned_clock2;

   // receive other clock time
   uint bytes_to_recv = 2 + 2 * sizeof(float) + 1 + 1;
   uint tmp_bytes_received;
   sconn.connection->getSockStream()->recvn((char*)&(sconn.recv_buffer[0]),
                                            bytes_to_recv, tmp_bytes_received);
   msg.receiveClockSync((char*)(&(sconn.recv_buffer[0])) + 2, bytes_to_recv - 2,
                        returned_clock1, returned_clock2, other_clock_is_synced );

   // return our clock to sync with
   vpr::Interval clock1_stamp;
   clock1_stamp.setNow();
   clock1=clock1_stamp.usecf();
   clock2 = returned_clock2;
   msg.createClockSync(clock1, clock2, clock_is_synced);
   sconn.connection->sendMsg(msg);
   msg.clear();

   return other_clock_is_synced;*/
}

// send 1 msg, recv 1 msg, return delta
float NetClockSync::iterationLoopAsResponder(SyncConnection& sconn, const float& clock1, float& clock2,
                                             float& returned_clock1, float& returned_clock2, const bool clock_is_synced)
{
   /*vprDEBUG(gadgetDBG_INPUT_MGR, 5)
      << "NetClockSync::iterationLoopAsResponder() " << std::endl
      << vprDEBUG_FLUSH;

   MsgPackage msg;
   bool other_clock_is_synced;
   vpr::Interval clock2_stamp;
   clock2_stamp.setNow();
   clock2 = clock2_stamp.usecf();

   // send msg
   msg.createClockSync(clock1, clock2, clock_is_synced);
   sconn.connection->sendMsg(msg);
   msg.clear();

   // recv msg, returned_clock1 contains clock
   uint bytes_to_recv = 2 + 2 * sizeof(float) + 1 + 1;
   uint tmp_bytes_received;
   sconn.connection->getSockStream()->recvn((char*)(&(sconn.recv_buffer[0])),
                                            bytes_to_recv, tmp_bytes_received);
   msg.receiveClockSync((char*)(&(sconn.recv_buffer[0])) + 2, bytes_to_recv,
                        returned_clock1, returned_clock2, other_clock_is_synced);

   // calculate delta
   vpr::Interval new_clock2;
   new_clock2.setNow();
   float delta = (new_clock2.usecf() - clock2) / 2.0f;

   return delta;*/
}

void NetClockSync::initiateExchange(SyncConnection& sconn)
{
   /*vprDEBUG(gadgetDBG_INPUT_MGR, 5) << "NetClockSync::initiateExchange() "
      << std::endl << vprDEBUG_FLUSH;
   // clock synchronization disabled per Eric's suggestion.  This needs to
   // be revisited with vpr intervals and some kind of clock offset per
   // connection or some such - CDJ
// #if JCCL_PERFORMANCE == JCCL_PERF_POSIX

//    sconn.recv_buffer.resize(64);
//    MsgPackage msg;

//    // 1st step, see if other host needs synchronizing --------------------------------
//    // Note the remote input managers have ensured the streams are empty while we sync
//    // create a msg with the name of our clock
//    bool clock_is_synced = true;  // we are synced with ourselves, the main clock
//    msg.createClockSyncHaveSrc(mSrcManagerId, clock_is_synced);
//    sconn.connection->sendMsg(msg);
//    msg.clear();

//    bool returned_clock_is_synced;
//    uint bytes_to_recv = 2 + vpr::GUID().toString().length() + 1 + 1; // opcode(2) + id(n) + bool + semicolon(1)
//    uint tmp_bytes_received;
//    sconn.connection->getSockStream()->recvn( (char*)&(sconn.recv_buffer[0]), bytes_to_recv, tmp_bytes_received);
//    vpr::GUID received_id;
//    msg.receiveClockSyncHaveSrc((char*)(&(sconn.recv_buffer[0])) + 2, bytes_to_recv - 2, received_id, returned_clock_is_synced);

//    if(returned_clock_is_synced == true) {
//       vprDEBUG(gadgetDBG_INPUT_MGR, 3)
//          << "initiate sync: clock already synced with "
//          << mSrcManagerId.toString() << std::endl << vprDEBUG_FLUSH;
//       return;   // the other host and its connections are already based on the same clock
//    }
//    else{
//       vprDEBUG(gadgetDBG_INPUT_MGR, 3)
//          << "initiate sync: clock sync requested with "
//          << mSrcManagerId.toString() << std::endl << vprDEBUG_FLUSH;
//    }

//    // initial clock exchange --------------------------------------------------------

//    firstIterationAsInitiator(sconn);

//    // loop until responder has in sync clock ----------------------------------------

//    bool other_clock_is_synced = false;
//    while (other_clock_is_synced == false)
//      other_clock_is_synced = iterationLoopAsInitiator(sconn);

// #endif
	  */
}

void NetClockSync::respondExchange(SyncConnection& sconn)
{
   /*vprDEBUG(gadgetDBG_INPUT_MGR, 5) << "NetClockSync::respondExchange() "
      << std::endl << vprDEBUG_FLUSH;

   // SPROC does not have resync function yet
   // This code is effectively not functional for the time being. It used
   // the deprecated methods in jccl::TimeStamp, and needs to be reevaluated.
   // -CDJ, per suggestion from Eric
// #if JCCL_PERFORMANCE == JCCL_PERF_POSIX

//    sconn.recv_buffer.resize(64);
//    MsgPackage msg;

//    uint bytes_to_recv = 2 + vpr::GUID().toString().length() + 1 + 1; // opcode(2) + id(n) + bool + semicolon(1)
//    uint tmp_bytes_received;
//    if ( sconn.recv_buffer.size() < bytes_to_recv)
//       sconn.recv_buffer.resize(bytes_to_recv);

//    // 1st step, see if we need synchronizing by comparing clock src --------------------
//    // Note the remote input manager has ensured the stream only contains time sync data
//    sconn.connection->getSockStream()->recvn( (char*)&(sconn.recv_buffer[0]), bytes_to_recv, tmp_bytes_received);
//    vpr::GUID received_id;
//    bool returned_clock_is_synced;
//    msg.receiveClockSyncHaveSrc( (char*)&(sconn.recv_buffer[0]) + 2, bytes_to_recv - 2, received_id, returned_clock_is_synced);
//    msg.clear();
//    vpr::GUID empty_id;
//    if(received_id == mSrcManagerId){
//       vprDEBUG(gadgetDBG_INPUT_MGR, 3)
//          << "respond sync: clock already synced with "
//          << received_id.toString() << std::endl << vprDEBUG_FLUSH;
//       bool clock_is_synced = true;
//       msg.createClockSyncHaveSrc(empty_id, clock_is_synced);
//       sconn.connection->sendMsg(msg);
//       msg.clear();
//       return;
//    }
//    else{
//       vprDEBUG(gadgetDBG_INPUT_MGR, 3)
//          << "respond sync: clock sync requested with "
//          << received_id.toString() << std::endl << vprDEBUG_FLUSH;
//       bool clock_is_synced = false;
//       msg.createClockSyncHaveSrc(empty_id, clock_is_synced);
//       sconn.connection->sendMsg(msg);
//       msg.clear();
//       // After the syncing is finished, our clock src will be based on this manager's clock
//       mSrcManagerId = received_id;
//    }

//    // initial clock exchange --------------------------------------------------------

//    float returned_clock1;
//    float delta = firstIterationAsResponder(sconn, returned_clock1);

//    float clock1, clock2, returned_clock2;
//    bool clock_is_synced = false;

//    // first attempt to sync
//    // commented out until the RIM is rewritten to have offsets applied at
//    // each message transfer.  Eric sez this is ok in the meantime. -CDJ
//    //jccl::TimeStamp::resync(returned_clock1 + delta);  // resync not implemented in SPROC yet

//    // loop until threshold is met ---------------------------------------------------

//    float test_threshold_usecs = 1000000;           // large initial value (secs) to enter while loop
//    const float secs_to_usecs = 100000;    // changes usecs to secs

//    float error_threshold_usecs = mErrorThreshold * secs_to_usecs;
//    const int retries = 4;                   // number of retries per precision
//    int try_count = 0;
//    float last_test_threshold_usecs = 0;           // help track error in clock setting

//    while(fabsf(test_threshold_usecs) > error_threshold_usecs){
//       clock1 = returned_clock1;
//       delta = iterationLoopAsResponder(sconn, clock1, clock2, returned_clock1, returned_clock2, clock_is_synced);
//       vpr::Interval estimate_stamp;
//       estimate_stamp.setNow();
// //       jccl::TimeStamp estimate_stamp;
//       // calculate difference between clocks, with help of delta (latency)
//       test_threshold_usecs = ( estimate_stamp.usecf() - (returned_clock1 + delta) );   // positive means our stamp value is larger

//       // resync if not synced well yet.
//       if(fabsf(test_threshold_usecs) > error_threshold_usecs){
//          // Take into account error from last setting by adding last_test_threshold
//          // resync disabled per Eric's OK. CDJ
//          //jccl::TimeStamp::resync(returned_clock1 + delta - last_test_threshold_usecs);  // no resync in sproc
//          last_test_threshold_usecs += test_threshold_usecs;
//       }

//       vprDEBUG(gadgetDBG_INPUT_MGR, 3) << "respond sync: clock difference in usecs: " << fabsf(test_threshold_usecs) << " (" << test_threshold_usecs << ")" << std::endl << vprDEBUG_FLUSH;
//       vprDEBUG(gadgetDBG_INPUT_MGR, 3) << "respond sync: acceptable clock difference in usecs: " << error_threshold_usecs << std::endl << vprDEBUG_FLUSH;
//       vprDEBUG(gadgetDBG_INPUT_MGR, 3) << "respond sync: latency in usecs: " << delta << std::endl << vprDEBUG_FLUSH;

//       try_count++;
//       if(try_count >= retries){
//          try_count = 0;
//          error_threshold_usecs *= 2;
//          last_test_threshold_usecs = 0;
//       }
//    }

//    // tell initiator our clock is synchronized ---------------------------------------
//    clock_is_synced = true;
//    iterationLoopAsResponder(sconn, clock1, clock2, returned_clock1, returned_clock2, clock_is_synced);

// #endif
	  */
}

bool NetClockSync::syncNeeded()
{
   /*if ( mConnections.size() > 0 )
   {
      return true;
   }
   else
   {
      return false;
   } */
}

void NetClockSync::sync()
{
   /*vprDEBUG(gadgetDBG_INPUT_MGR, 5) << "NetClockSync::sync() " << std::endl
                                    << vprDEBUG_FLUSH;

   std::vector<SyncConnection*>::iterator i;
   for ( i = mConnections.begin(); i != mConnections.end(); i++ )
   {
      if ( (*i)->state == SYNC_INITIATE )
      {
         initiateExchange(*(*i));
      }
      else if ( (*i)->state == SYNC_RESPOND )
      {
         respondExchange(*(*i));
      }

      delete (*i);  // we have synced this item so delete it
   }
   mConnections.clear();*/
}

void NetClockSync::syncWithMyClock(NetConnection* conn)
{
   /*vprDEBUG(gadgetDBG_INPUT_MGR, 5) << "NetClockSync::syncWithMyClock() "
                                    << std::endl << vprDEBUG_FLUSH;
   SyncConnection* sconn = new SyncConnection;
   sconn->state = SYNC_INITIATE;
   sconn->connection = conn;
   mConnections.push_back(sconn);
	*/
}

void NetClockSync::syncWithOtherClock(NetConnection* conn)
{
   /*vprDEBUG(gadgetDBG_INPUT_MGR, 5) << "NetClockSync::syncWithOtherClock() "
                                    << std::endl << vprDEBUG_FLUSH;
   SyncConnection* sconn = new SyncConnection;
   sconn->state = SYNC_RESPOND;
   sconn->connection = conn;
   mConnections.push_back(sconn);*/
}

}  // end namespace gadget
