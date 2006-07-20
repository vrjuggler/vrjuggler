#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <boost/bind.hpp>

#include <cppunit/TestCaller.h>

#include <vpr/vpr.h>
#include <vpr/IO/Socket/Socket.h>
#include <vpr/IO/Socket/SocketDatagram.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Socket/SocketAcceptor.h>
#include <vpr/IO/Socket/SocketConnector.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/WouldBlockException.h>

#include <vpr/IO/IOSys.h>
#include <vpr/IO/Selector.h>
#include <vpr/Util/Debug.h>

#include <vpr/System.h>

#include <vpr/Thread/Thread.h>

#include <TestCases/Socket/NonBlockingSocketsTest.h>

#include <boost/concept_check.hpp>


namespace vprTest
{
CPPUNIT_TEST_SUITE_REGISTRATION( NonBlockingSocketTest );

void NonBlockingSocketTest::testSetOpenNonBlockingThenOpenThenClose()
{
   int port = 6275;
   vpr::InetAddr local_addr;
   local_addr.setPort( port );
   vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );

   acceptor_socket.setBlocking(false);

   CPPUNIT_ASSERT_NO_THROW(acceptor_socket.open());

   CPPUNIT_ASSERT_NO_THROW(acceptor_socket.close());
}

void NonBlockingSocketTest::testSetOpenNonBlockingThenOpenThenEnableNonBlockThenClose()
{
   int port = 6275;
   vpr::InetAddr local_addr;
   local_addr.setPort( port );
   vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );

   acceptor_socket.setBlocking(false); // for opening
   CPPUNIT_ASSERT_NO_THROW(acceptor_socket.open());

   CPPUNIT_ASSERT(! acceptor_socket.isBlocking());

   acceptor_socket.setBlocking(true); // for reads and writes
   CPPUNIT_ASSERT(acceptor_socket.isBlocking());

   CPPUNIT_ASSERT_NO_THROW(acceptor_socket.close());
}

void NonBlockingSocketTest::testConnect2NonBlockingSockets()
{
   int port = 6275;
   vpr::InetAddr local_addr;
   local_addr.setPort( port );
   vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );
   vpr::SocketStream connector_socket( vpr::InetAddr::AnyAddr, local_addr );

   // a/c: Open
   CPPUNIT_ASSERT_NO_THROW( acceptor_socket.open());
   CPPUNIT_ASSERT_NO_THROW( connector_socket.open() );

   // a/c: enableNonBlock
   acceptor_socket.setBlocking(false);
   CPPUNIT_ASSERT(! acceptor_socket.isBlocking());
   connector_socket.setBlocking(false);
   CPPUNIT_ASSERT(! connector_socket.isBlocking());

   // a:   bind
   CPPUNIT_ASSERT_NO_THROW(acceptor_socket.bind());

   // a:   listen
   CPPUNIT_ASSERT_NO_THROW(acceptor_socket.listen());

   // c:   connect
   try
   {
      connector_socket.connect();
   }
   catch (vpr::WouldBlockException& ex)
   {
      // Do nothing.
   }
   catch (vpr::IOException& ex)
   {
      CPPUNIT_ASSERT_NO_THROW(throw);
   }

   // a:   accept
   vpr::SocketStream spawned_socket;
   try
   {
      acceptor_socket.accept( spawned_socket );
   }
   catch (vpr::WouldBlockException& ex)
   {
      // Do nothing.
   }
   catch (vpr::IOException& ex)
   {
      // TODO: Should be able to just try.
      CPPUNIT_ASSERT(false && "Accept failed.");
   }

   // a/c/s: Close
   CPPUNIT_ASSERT_NO_THROW(acceptor_socket.close());
   CPPUNIT_ASSERT_NO_THROW(connector_socket.close());
   CPPUNIT_ASSERT_NO_THROW(spawned_socket.close());
}

void NonBlockingSocketTest::testNonBlockingTransfer ()
{
   mState        = NOT_READY;                        // Initialize
   mAcceptorPort = 34567;
   mMessage      = "The sixth sheik's sixth sheep's sick";
   mMessageLen   = mMessage.length();

   // Spawn acceptor thread
   vpr::Thread acceptor_thread(
      boost::bind(&NonBlockingSocketTest::testNonBlockingTransfer_acceptor,
                  this)
   );

   // Spawn connector thread
   vpr::Thread connector_thread(
      boost::bind(&NonBlockingSocketTest::testNonBlockingTransfer_connector,
                  this)
   );

   // Wait for threads
   acceptor_thread.join();
   connector_thread.join();
}

void NonBlockingSocketTest::testNonBlockingTransfer_acceptor()
{
   vpr::Uint32 bytes_written;
   vpr::SocketAcceptor acceptor;
   vpr::SocketStream client_sock;
   vpr::InetAddr acceptor_addr;
   acceptor_addr.setPort(mAcceptorPort);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to open acceptor", acceptor.open(acceptor_addr));

   // The acceptor must be non-blocking so that the connected socket it
   // returns will also be non-blocking.  *sigh*
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable non-blocking for accepted socket",
      acceptor.getSocket().setBlocking(false));

   mCondVar.acquire();
   {
      mState = ACCEPTOR_READY;
      mCondVar.signal();
   }
   mCondVar.release();
   
   bool running(true);
   while (running)
   {
      try
      {
         acceptor.accept(client_sock);
         running = false;
      }
      catch (vpr::WouldBlockException& ex)
      {
         // Do nothing.
      }
      catch (vpr::Exception& ex)
      {
         CPPUNIT_ASSERT_NO_THROW(throw);
      }
   }

   CPPUNIT_ASSERT(client_sock.isOpen() && "Accepted socket should be open");
   CPPUNIT_ASSERT(! client_sock.isBlocking() &&
                    "Connected client socket should be non-blocking");

   client_sock.setNoDelay(true);
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to send message to client",
      client_sock.send(mMessage, mMessageLen, bytes_written));

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

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Could not close acceptor side of client socket", client_sock.close());

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Could not close acceptor", acceptor.close());
}

void NonBlockingSocketTest::testNonBlockingTransfer_connector()
{
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

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to open connector socket", con_sock.open());

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable non-blocking for connector", con_sock.setBlocking(false));

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Connector can't connect",
      connector.connect(con_sock, remote_addr, vpr::Interval(5, vpr::Interval::Sec)));

   CPPUNIT_ASSERT(! con_sock.isBlocking() && "Connector should be non-blocking");

   mCondVar.acquire();
   {
      while ( mState != DATA_SENT )
      {
         mCondVar.wait();
      }
   }
   mCondVar.release();

   try
   {
      con_sock.recv(data, mMessageLen, bytes_read);
   }
   catch (vpr::WouldBlockException& ex)
   {
      // Make sure we got all the data, then close.
      while ( con_sock.isReadBlocked() )
      {
         vpr::System::usleep(10);
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Connector waiting for data\n"
                                 << vprDEBUG_FLUSH;
      }
   }
   catch(...)
   {
      CPPUNIT_ASSERT(false && "Connector could not receive message");
   }
   CPPUNIT_ASSERT(bytes_read == mMessageLen &&
                    "Connector received message of wrong size");

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
   vpr::InetAddr local_addr;
   local_addr.setPort( port );
   vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );
   vpr::SocketStream connector_socket( vpr::InetAddr::AnyAddr, local_addr );

   // a/c: Set OpenNonBlocking
   acceptor_socket.setBlocking(false);
   connector_socket.setBlocking(false);

   // a/c: Open
   CPPUNIT_ASSERT_NO_THROW(acceptor_socket.open());
   CPPUNIT_ASSERT_NO_THROW(connector_socket.open());

   CPPUNIT_ASSERT(! acceptor_socket.isBlocking());
   CPPUNIT_ASSERT(! connector_socket.isBlocking());

   // a: getHandle
   vpr::IOSys::Handle handle = acceptor_socket.getHandle();

   // Add acceptor's handle to a selector
   vpr::Selector selector;
   selector.addHandle( handle );
   selector.setIn( handle, vpr::Selector::Read | vpr::Selector::Write | vpr::Selector::Except );

   // a/c: enableNonBlock
   acceptor_socket.setBlocking(false);
   CPPUNIT_ASSERT(! acceptor_socket.isBlocking());
   connector_socket.setBlocking(false);
   CPPUNIT_ASSERT(! connector_socket.isBlocking());

   // a:   bind
   CPPUNIT_ASSERT_NO_THROW(acceptor_socket.bind());

   // a:   listen
   CPPUNIT_ASSERT_NO_THROW(acceptor_socket.listen());

   // c:   connect
   try
   {
      connector_socket.connect();
   }
   catch (vpr::WouldBlockException& ex)
   {
      // Do nothing.
   }
   catch (vpr::IOException& ex)
   {
      // TODO: Should be able to just try.
      CPPUNIT_ASSERT(false && "Connect failed.");
   }

   // block until connection request goes through
   vpr::Uint16 num_events;
   selector.select( num_events, vpr::Interval(50000,vpr::Interval::Msec) );
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
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("accepted socket is NULL",  acceptor_socket.accept( spawned_socket ));
   CPPUNIT_ASSERT( spawned_socket.isOpen() && "accepted socket should be open" );

   // s:    write...
   std::string message = "Hi Maynard, My leg hurts";
   vpr::Uint32 bytes_written;
   CPPUNIT_ASSERT_NO_THROW_MESSAGE( "Problem writing in acceptor",
      spawned_socket.write( message, message.size(), bytes_written ));

   // s:    write the max size...
   size_t size;
   CPPUNIT_ASSERT_NO_THROW_MESSAGE( "couldn't get the max size for sending data with socket",
      spawned_socket.getSendBufferSize( size ));
   message.resize( size );
   CPPUNIT_ASSERT_NO_THROW_MESSAGE( "maxsize test failed",
      spawned_socket.write( message, message.size(), bytes_written ));

   // block until data is sent
/*
   selector.select( num_events, 50000 );
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
   connector_socket.read( message, message.size(), bytes_written );
   CPPUNIT_ASSERT( status.success() && "read test failed" );
*/

   // a/c/s: Close
   CPPUNIT_ASSERT_NO_THROW(acceptor_socket.close());
   CPPUNIT_ASSERT_NO_THROW(connector_socket.close());
   CPPUNIT_ASSERT_NO_THROW(spawned_socket.close());
}

void NonBlockingSocketTest::testSendUDP ()
{
   mState        = NOT_READY;                        // Initialize
   mReceiverPort = 34567;
   mSenderPort   = mReceiverPort + 1;
   mMessage      = "The sixth sheik's sixth sheep's sick";
   mMessageLen   = mMessage.length();

   // Spawn acceptor thread
   vpr::Thread receiver_thread(
      boost::bind(&NonBlockingSocketTest::testSendUDP_receiver, this)
   );

   // Spawn connector thread
   vpr::Thread sender_thread(
      boost::bind(&NonBlockingSocketTest::testSendUDP_sender, this)
   );

   // Wait for threads
   receiver_thread.join();
   sender_thread.join();
}

void NonBlockingSocketTest::testSendUDP_receiver()
{
   vpr::InetAddr my_addr, from_addr;
   my_addr.setPort(mReceiverPort);
   std::string data;
   vpr::Uint32 bytes_read;
   vpr::SocketDatagram recv_sock(my_addr, vpr::InetAddr::AnyAddr);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to open receiver socket", recv_sock.open());

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable non-blocking for receiver",
      recv_sock.setBlocking(false));

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to bind receiver socket", recv_sock.bind());

   mCondVar.acquire();
   {
      mState = RECEIVER_READY;
      mCondVar.signal();
   }
   mCondVar.release();

   // TODO: Fix with readn?
   bool running(true);
   while (running)
   {
      try
      {
         recv_sock.recvfrom(data, mMessageLen, from_addr, bytes_read);
         running = false;
      }
      catch (vpr::WouldBlockException& ex)
      {
         vpr::System::usleep(10);
         // Do nothing.
      }
      catch (vpr::Exception& ex)
      {
         CPPUNIT_ASSERT_NO_THROW(throw);
      }
   }
   
   mCondVar.acquire();
   {
      mState = DATA_RECEIVED;
      mCondVar.signal();
   }
   mCondVar.release();

   CPPUNIT_ASSERT(bytes_read == mMessageLen && "Did not receive entire message");
   CPPUNIT_ASSERT(data == mMessage);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Could not close receiver socket", recv_sock.close());
}

void NonBlockingSocketTest::testSendUDP_sender()
{
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

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to open sender socket", send_sock.open());

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable non-blocking for sender", send_sock.setBlocking(false));

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to send to receiver",
      send_sock.sendto(mMessage, mMessageLen, remote_addr, bytes));

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Could not close sender socket",
      send_sock.close());
}

} // End of vprTest namespace
