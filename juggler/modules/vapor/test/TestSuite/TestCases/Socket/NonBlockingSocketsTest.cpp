#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>

#include <cppunit/TestCaller.h>

#include <vpr/vpr.h>
#include <vpr/IO/Socket/Socket.h>
#include <vpr/IO/Socket/SocketDatagram.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Socket/SocketAcceptor.h>
#include <vpr/IO/Socket/SocketConnector.h>
#include <vpr/IO/Socket/InetAddr.h>

#include <vpr/IO/IOSys.h>
#include <vpr/IO/Selector.h>
#include <vpr/Util/ReturnStatus.h>

#include <vpr/System.h>

#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>

#include <NonBlockingSocketsTest.h>


namespace vprTest
{

void NonBlockingSocketTest::testSetOpenNonBlockingThenOpenThenClose()
{
   int port = 6275;
   bool result = false;
   vpr::InetAddr local_addr;
   local_addr.setPort( port );
   vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );

   acceptor_socket.setOpenNonBlocking();

   result = acceptor_socket.open().success();
   CPPUNIT_ASSERT( result );

   result = acceptor_socket.close().success();
   CPPUNIT_ASSERT( result );
}

void NonBlockingSocketTest::testSetOpenNonBlockingThenOpenThenEnableNonBlockThenClose()
{
   int port = 6275;
   bool result = false;
   vpr::InetAddr local_addr;
   local_addr.setPort( port );
   vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );

   acceptor_socket.setOpenNonBlocking(); // for opening
   result = acceptor_socket.open().success();
   CPPUNIT_ASSERT( result );

   CPPUNIT_ASSERT( acceptor_socket.getNonBlocking() );

   acceptor_socket.enableBlocking(); // for reads and writes
   CPPUNIT_ASSERT( acceptor_socket.getBlocking() );

   result = acceptor_socket.close().success();
   CPPUNIT_ASSERT( result );
}

void NonBlockingSocketTest::testConnect2NonBlockingSockets()
{
   int port = 6275;
   //bool result = false;
   vpr::InetAddr local_addr;
   vpr::ReturnStatus status;
   local_addr.setPort( port );
   vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );
   vpr::SocketStream connector_socket( vpr::InetAddr::AnyAddr, local_addr );

   // a/c: Open
   status = acceptor_socket.open();
   CPPUNIT_ASSERT( status.failure() != true );
   status = connector_socket.open();
   CPPUNIT_ASSERT( status.failure() != true );

   // a/c: enableNonBlock
   acceptor_socket.enableNonBlocking();
   CPPUNIT_ASSERT( acceptor_socket.getNonBlocking() );
   connector_socket.enableNonBlocking();
   CPPUNIT_ASSERT( connector_socket.getNonBlocking() );

   // a:   bind
   status = acceptor_socket.bind();
   CPPUNIT_ASSERT( status.failure() != true );

   // a:   listen
   status = acceptor_socket.listen();
   CPPUNIT_ASSERT( status.failure() != true );

   // c:   connect
   status = connector_socket.connect();
   CPPUNIT_ASSERT( status.failure() != true );
   if ( status.success() != true )
   {
      CPPUNIT_ASSERT( status.inProgress() == true );
   }

   // a:   accept
   vpr::SocketStream spawned_socket;
   status = acceptor_socket.accept( spawned_socket );
   CPPUNIT_ASSERT( status.failure() != true );
   //CPPUNIT_ASSERT( spawned_socket.isOpen() );
   if ( status.success() != true )
   {
      CPPUNIT_ASSERT( status.inProgress() == true );
   }

   // a/c/s: Close
   status = acceptor_socket.close();
   CPPUNIT_ASSERT( status.failure() != true );
   status = connector_socket.close();
   CPPUNIT_ASSERT( status.failure() != true );
   status = spawned_socket.close();
   CPPUNIT_ASSERT( status.failure() != true );
}

void NonBlockingSocketTest::testNonBlockingTransfer ()
{
   threadAssertReset();

   mState        = NOT_READY;                        // Initialize
   mAcceptorPort = 34567;
   mMessage      = "The sixth sheik's sixth sheep's sick";
   mMessageLen   = mMessage.length();

   // Spawn acceptor thread
   vpr::ThreadMemberFunctor<NonBlockingSocketTest>* acceptor_functor =
         new vpr::ThreadMemberFunctor<NonBlockingSocketTest>(this, &NonBlockingSocketTest::testNonBlockingTransfer_acceptor);
   vpr::Thread acceptor_thread( acceptor_functor);

   // Spawn connector thread
   vpr::ThreadMemberFunctor<NonBlockingSocketTest>* connector_functor =
      new vpr::ThreadMemberFunctor<NonBlockingSocketTest>(this, &NonBlockingSocketTest::testNonBlockingTransfer_connector);
   vpr::Thread connector_thread( connector_functor);

   // Wait for threads
   acceptor_thread.join();
   connector_thread.join();

   checkThreadAssertions();
}

void NonBlockingSocketTest::testNonBlockingTransfer_acceptor (void* arg)
{
   vpr::ReturnStatus status;
   vpr::Uint32 bytes_written;
   vpr::SocketAcceptor acceptor;
   vpr::SocketStream client_sock;
   vpr::InetAddr acceptor_addr(mAcceptorPort);

   status = acceptor.open(acceptor_addr);
   assertTestThread(status.success() && "Failed to open acceptor");

   // The acceptor must be non-blocking so that the connected socket it
   // returns will also be non-blocking.  *sigh*
   status = acceptor.getSocket().enableNonBlocking();
   assertTestThread(status.success() &&
                    "Failed to enable non-blocking for accepted socket");

   mCondVar.acquire();
   {
      mState = ACCEPTOR_READY;
      mCondVar.signal();
   }
   mCondVar.release();

   do
   {
      status = acceptor.accept(client_sock);
   }
   while ( status == vpr::ReturnStatus::WouldBlock );

   assertTestThread(status.success() && "Accept failed");

   assertTestThread(client_sock.isOpen() && "Accepted socket should be open");
   assertTestThread(client_sock.getNonBlocking() &&
                    "Connected client socket should be non-blocking");

   client_sock.setNoDelay(true);
   status = client_sock.send(mMessage, mMessageLen, bytes_written);
   assertTestThread(! status.failure() && "Failed to send message to client");

   mCondVar.acquire();
   {
      mState = DATA_SENT;
      mCondVar.signal();
   }
   mCondVar.release();

   mCondVar.acquire();
   {
      while ( mState != DATA_RECEIVED )
      {
         mCondVar.wait();
      }
   }
   mCondVar.release();

   status = client_sock.close();
   assertTestThread(status.success() &&
                    "Could not close acceptor side of client socket");

   status = acceptor.close();
   assertTestThread(status.success() && "Could not close acceptor");
}

void NonBlockingSocketTest::testNonBlockingTransfer_connector (void* arg)
{
   vpr::ReturnStatus status;
   vpr::InetAddr remote_addr;
   vpr::SocketConnector connector;
   vpr::SocketStream con_sock;
   std::string data;
   vpr::Uint32 bytes_read;

   remote_addr.setAddress("localhost", mAcceptorPort);

   mCondVar.acquire();
   {
      while ( mState != ACCEPTOR_READY )
      {
         mCondVar.wait();
      }
   }
   mCondVar.release();

   status = con_sock.open();
   assertTestThread(status.success() && "Failed to open connector socket");

   status = con_sock.enableNonBlocking();
   assertTestThread(status.success() &&
                    "Failed to enable non-blocking for connector");

   status = connector.connect(con_sock, remote_addr,
                              vpr::Interval(5, vpr::Interval::Sec));
   assertTestThread(! status.failure() && "Connector can't connect");

   assertTestThread(con_sock.getNonBlocking() &&
                    "Connector should be non-blocking");

   mCondVar.acquire();
   {
      while ( mState != DATA_SENT )
      {
         mCondVar.wait();
      }
   }
   mCondVar.release();

   status = con_sock.recv(data, mMessageLen, bytes_read);

   if ( status == vpr::ReturnStatus::WouldBlock )
   {
      // Make sure we got all the data, then close.
      while ( con_sock.isReadBlocked() )
      {
         vpr::System::usleep(10);
         vprDEBUG(vprDBG_ALL, 0) << "Connector waiting for data\n"
                                 << vprDEBUG_FLUSH;
      }
   }
   else if ( status.success() )
   {
      assertTestThread(bytes_read == mMessageLen &&
                       "Connector received message of wrong size");
   }
   else
   {
      assertTestThread(false && "Connector could not receive message");
   }

   mCondVar.acquire();
   {
      mState = DATA_RECEIVED;
      mCondVar.signal();
   }
   mCondVar.release();

   con_sock.close();
}

void NonBlockingSocketTest::testConnect2NonBlockingSocketsUsingSelect()
{
   int port = 6275;
   //bool result = false;
   vpr::InetAddr local_addr;
   vpr::ReturnStatus status;
   local_addr.setPort( port );
   vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );
   vpr::SocketStream connector_socket( vpr::InetAddr::AnyAddr, local_addr );

   // a/c: Set OpenNonBlocking
   acceptor_socket.setOpenNonBlocking();
   connector_socket.setOpenNonBlocking();

   // a/c: Open
   status = acceptor_socket.open();
   CPPUNIT_ASSERT( status.failure() != true );
   status = connector_socket.open();
   CPPUNIT_ASSERT( status.failure() != true );

   CPPUNIT_ASSERT( acceptor_socket.getNonBlocking() );
   CPPUNIT_ASSERT( connector_socket.getNonBlocking() );

   // a: getHandle
   vpr::IOSys::Handle handle = acceptor_socket.getHandle();

   // Add acceptor's handle to a selector
   vpr::Selector selector;
   selector.addHandle( handle );
   selector.setIn( handle, vpr::Selector::Read | vpr::Selector::Write | vpr::Selector::Except );

   // a/c: enableNonBlock
   acceptor_socket.enableNonBlocking();
   CPPUNIT_ASSERT( acceptor_socket.getNonBlocking() );
   connector_socket.enableNonBlocking();
   CPPUNIT_ASSERT( connector_socket.getNonBlocking() );

   // a:   bind
   status = acceptor_socket.bind();
   CPPUNIT_ASSERT( status.failure() != true );

   // a:   listen
   status = acceptor_socket.listen();
   CPPUNIT_ASSERT( status.failure() != true );

   // c:   connect
   status = connector_socket.connect();
   CPPUNIT_ASSERT( status.failure() != true );
   if ( status.success() != true )
   {
      CPPUNIT_ASSERT( status.inProgress() == true );
   }

   // block until connection request goes through
   vpr::Uint16 num_events;
   status = selector.select( num_events, vpr::Interval(50000,vpr::Interval::Msec) );
   for (int j = 0; j < selector.getNumHandles(); ++j)
   {
      if (selector.getOut( selector.getHandle(j) ) & (vpr::Selector::Read | vpr::Selector::Except))
      {
         CPPUNIT_ASSERT( handle == selector.getHandle( j ) &&
                         "Handle doesn't match" );
      }
   }
   CPPUNIT_ASSERT( num_events > 0 && "no events" );


   // a:   accept
   vpr::SocketStream spawned_socket;
   status = acceptor_socket.accept( spawned_socket );
   CPPUNIT_ASSERT( status.success() == true && "accepted socket is NULL" );
   CPPUNIT_ASSERT( spawned_socket.isOpen() && "accepted socket should be open" );

   // s:    write...
   std::string message = "Hi Maynard, My leg hurts";
   vpr::Uint32 bytes_written;
   status = spawned_socket.write( message, message.size(), bytes_written );
   CPPUNIT_ASSERT( status.success() && "Problem writing in acceptor" );

   // s:    write the max size...
   int size;
   status = spawned_socket.getSendBufferSize( size );
   CPPUNIT_ASSERT( status.success() && "couldn't get the max size for sending data with socket" );
   message.resize( size );
   status = spawned_socket.write( message, message.size(), bytes_written );
   CPPUNIT_ASSERT( status.success() && "maxsize test failed" );

   // block until data is sent
/*
   status = selector.select( num_events, 50000 );
   for (int j = 0; j < selector.getNumHandles(); ++j)
   {
      if (selector.getOut( selector.getHandle(j) ) & (vpr::Selector::Read | vpr::Selector::Except))
      {
         CPPUNIT_ASSERT( handle == selector.getHandle( j ) &&
                         "Handle doesn't match" );
      }
   }
   CPPUNIT_ASSERT( num_events > 0 && "no events" );

   // c:     read
   status = connector_socket.read( message, message.size(), bytes_written );
   CPPUNIT_ASSERT( status.success() && "read test failed" );
*/

   // a/c/s: Close
   status = acceptor_socket.close();
   CPPUNIT_ASSERT( status.failure() != true );
   status = connector_socket.close();
   CPPUNIT_ASSERT( status.failure() != true );
   status = spawned_socket.close();
   CPPUNIT_ASSERT( status.failure() != true );
}

void NonBlockingSocketTest::testSendUDP ()
{
   threadAssertReset();

   mState        = NOT_READY;                        // Initialize
   mReceiverPort = 34567;
   mSenderPort   = mReceiverPort + 1;
   mMessage      = "The sixth sheik's sixth sheep's sick";
   mMessageLen   = mMessage.length();

   // Spawn acceptor thread
   vpr::ThreadMemberFunctor<NonBlockingSocketTest>* receiver_functor =
         new vpr::ThreadMemberFunctor<NonBlockingSocketTest>(this, &NonBlockingSocketTest::testSendUDP_receiver);
   vpr::Thread receiver_thread( receiver_functor);

   // Spawn connector thread
   vpr::ThreadMemberFunctor<NonBlockingSocketTest>* sender_functor =
      new vpr::ThreadMemberFunctor<NonBlockingSocketTest>(this, &NonBlockingSocketTest::testSendUDP_sender);
   vpr::Thread sender_thread(sender_functor);

   // Wait for threads
   receiver_thread.join();
   sender_thread.join();

   checkThreadAssertions();
}

void NonBlockingSocketTest::testSendUDP_receiver (void* arg)
{
   vpr::ReturnStatus status;
   vpr::InetAddr my_addr(mReceiverPort), from_addr;
   std::string data;
   vpr::Uint32 bytes_read;
   vpr::SocketDatagram recv_sock(my_addr, vpr::InetAddr::AnyAddr);

   status = recv_sock.open();
   assertTestThread(status.success() && "Failed to open receiver socket");

   status = recv_sock.enableNonBlocking();
   assertTestThread(status.success() &&
                    "Failed to enable non-blocking for receiver");

   status = recv_sock.bind();
   assertTestThread(status.success() && "Failed to bind receiver socket");

   mCondVar.acquire();
   {
      mState = RECEIVER_READY;
      mCondVar.signal();
   }
   mCondVar.release();

   do
   {
      status = recv_sock.recvfrom(data, mMessageLen, 0, from_addr, bytes_read);
      vpr::System::usleep(10);
   } while ( status == vpr::ReturnStatus::WouldBlock );

   assertTestThread(status.success() && "Failed to receive message");

   mCondVar.acquire();
   {
      mState = DATA_RECEIVED;
      mCondVar.signal();
   }
   mCondVar.release();

   assertTestThread(bytes_read == mMessageLen && "Did not receive entire message");

   status = recv_sock.close();
   assertTestThread(status.success() && "Could not close receiver socket");
}

void NonBlockingSocketTest::testSendUDP_sender (void* arg)
{
   vpr::ReturnStatus status;
   vpr::InetAddr remote_addr;
   vpr::SocketDatagram send_sock;
   vpr::Uint32 bytes;

   remote_addr.setAddress("localhost", mReceiverPort);

   mCondVar.acquire();
   {
      while ( mState != RECEIVER_READY )
      {
         mCondVar.wait();
      }
   }
   mCondVar.release();

   status = send_sock.open();
   assertTestThread(status.success() && "Failed to open sender socket");

   status = send_sock.enableNonBlocking();
   assertTestThread(status.success() &&
                    "Failed to enable non-blocking for sender");

   status = send_sock.sendto(mMessage, mMessageLen, 0, remote_addr, bytes);
   assertTestThread(status.success() && "Failed to send to receiver");

   status = send_sock.close();
   assertTestThread(status.success() && "Could not close sender socket");
}

CppUnit::Test* NonBlockingSocketTest::suite ()
{
   CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("NonBlockingSocketTest");

   test_suite->addTest( new CppUnit::TestCaller<NonBlockingSocketTest>("testSetOpenNonBlockingThenOpenThenClose", &NonBlockingSocketTest::testSetOpenNonBlockingThenOpenThenClose));
   test_suite->addTest( new CppUnit::TestCaller<NonBlockingSocketTest>("testSetOpenNonBlockingThenOpenThenEnableNonBlockThenClose", &NonBlockingSocketTest::testSetOpenNonBlockingThenOpenThenEnableNonBlockThenClose));
   test_suite->addTest( new CppUnit::TestCaller<NonBlockingSocketTest>("testConnect2NonBlockingSockets", &NonBlockingSocketTest::testConnect2NonBlockingSockets));
   test_suite->addTest(new CppUnit::TestCaller<NonBlockingSocketTest>("testNonBlockingTransfer", &NonBlockingSocketTest::testNonBlockingTransfer));
   test_suite->addTest( new CppUnit::TestCaller<NonBlockingSocketTest>("testConnect2NonBlockingSocketsUsingSelect", &NonBlockingSocketTest::testConnect2NonBlockingSocketsUsingSelect));
   test_suite->addTest(new CppUnit::TestCaller<NonBlockingSocketTest>("testSendUDP", &NonBlockingSocketTest::testSendUDP));

   return test_suite;
}

} // End of vprTest namespace
