#ifndef _NB_SOCKET_TEST_H
#define _NB_SOCKET_TEST_H

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <vpr/IO/Socket/Socket.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Socket/InetAddr.h>

#include <vpr/IO/IOSys.h>
#include <vpr/IO/Selector.h>
#include <vpr/Util/Status.h>

#include <vpr/System.h>

#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/CondVar.h>

#include <vector>

namespace vprTest
{

class NonBlockingSocketTest : public TestCase
{
public:
   NonBlockingSocketTest( std::string name ) : TestCase (name), mThreadAssertTest( true )
   {
   }

   void threadAssertTest( bool testcase, std::string text=std::string("none") )
   {
      if (testcase == false)
      {
         mThreadAssertTest = false;
         std::cerr << "threadAssertTest - failed: " << text << std::endl;
      }
      //assertTest( testcase );     -- Causes crash
   }
   bool mThreadAssertTest; // true for no error

   virtual ~NonBlockingSocketTest()
   {
   }


   // Set OpenNonBlocking
   void testSetOpenNonBlocking()
   {
      int port = 6275;
      //bool result = false;
      vpr::InetAddr local_addr;
      local_addr.setPort( port );
      vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );

      acceptor_socket.setOpenNonBlocking(); // for opening

      assertTest( acceptor_socket.getNonBlocking() );
   }

   // Set OpenNonBlocking
   // Open
   // Close
   void testSetOpenNonBlockingThenOpenThenClose()
   {
      int port = 6275;
      bool result = false;
      vpr::InetAddr local_addr;
      local_addr.setPort( port );
      vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );

      acceptor_socket.setOpenNonBlocking();

      result = acceptor_socket.open().success();
      assertTest( result );

      result = acceptor_socket.close().success();
      assertTest( result );
   }

   // Set OpenNonBlocking
   // Open
   // enableNonBlock
   // Close
   void testSetOpenNonBlockingThenOpenThenEnableNonBlockThenClose()
   {
      int port = 6275;
      bool result = false;
      vpr::InetAddr local_addr;
      local_addr.setPort( port );
      vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );

      acceptor_socket.setOpenNonBlocking(); // for opening

      assertTest( acceptor_socket.getNonBlocking() );

      result = acceptor_socket.open().success();
      assertTest( result );

      acceptor_socket.enableNonBlocking(); // for reads and writes
      assertTest( acceptor_socket.getNonBlocking() );

      result = acceptor_socket.close().success();
      assertTest( result );
   }

   // 2 sockets...
   // a/c: Set OpenNonBlocking
   // a/c: Open
   // a/c: enableNonBlock
   // a:   bind
   // a:   listen
   // c:   connect
   // a:   accept
   // a/c/s: Close
   void testConnect2NonBlockingSockets()
   {
      int port = 6275;
      //bool result = false;
      vpr::InetAddr local_addr;
      vpr::Status status;
      local_addr.setPort( port );
      vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );
      vpr::SocketStream connector_socket( vpr::InetAddr::AnyAddr, local_addr );

      // a/c: Set OpenNonBlocking
      acceptor_socket.setOpenNonBlocking();
      connector_socket.setOpenNonBlocking();

      assertTest( acceptor_socket.getNonBlocking() );
      assertTest( connector_socket.getNonBlocking() );

      // a/c: Open
      status = acceptor_socket.open();
      assertTest( status.failure() != true );
      status = connector_socket.open();
      assertTest( status.failure() != true );

      // a/c: enableNonBlock
      acceptor_socket.enableNonBlocking();
      assertTest( acceptor_socket.getNonBlocking() );
      connector_socket.enableNonBlocking();
      assertTest( connector_socket.getNonBlocking() );

      // a:   bind
      status = acceptor_socket.bind();
      assertTest( status.failure() != true );

      // a:   listen
      status = acceptor_socket.listen();
      assertTest( status.failure() != true );

      // c:   connect
      status = connector_socket.connect();
      assertTest( status.failure() != true );
      if ( status.success() != true )
      {
         assertTest( status.inProgress() == true );
      }

      // a:   accept
      vpr::SocketStream spawned_socket;
      status = acceptor_socket.accept( spawned_socket );
      assertTest( status.failure() != true );
      //assertTest( spawned_socket.isOpen() );
      if ( status.success() != true )
      {
         assertTest( status.inProgress() == true );
      }

      // a/c/s: Close
      status = acceptor_socket.close();
      assertTest( status.failure() != true );
      status = connector_socket.close();
      assertTest( status.failure() != true );
      status = spawned_socket.close();
      assertTest( status.failure() != true );
   }

   // 2 sockets... use select...
   // a/c:   Set OpenNonBlocking
   // a/c:   Open
   // a:     getHandle
   //        Add acceptor's handle to a selector
   // a/c:   enableNonBlock
   // a:     bind
   // a:     listen
   // c:     connect
   //        select (block) until connection request goes through
   // a:     accept
   // s:     write...
   // s:     write the max size...
   // a/c/s: Close
   void testConnect2NonBlockingSocketsUsingSelect()
   {
      int port = 6275;
      //bool result = false;
      vpr::InetAddr local_addr;
      vpr::Status status;
      local_addr.setPort( port );
      vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );
      vpr::SocketStream connector_socket( vpr::InetAddr::AnyAddr, local_addr );

      // a/c: Set OpenNonBlocking
      acceptor_socket.setOpenNonBlocking();
      connector_socket.setOpenNonBlocking();

      assertTest( acceptor_socket.getNonBlocking() );
      assertTest( connector_socket.getNonBlocking() );

      // a/c: Open
      status = acceptor_socket.open();
      assertTest( status.failure() != true );
      status = connector_socket.open();
      assertTest( status.failure() != true );

      // a: getHandle
      vpr::IOSys::Handle handle = acceptor_socket.getHandle();

      // Add acceptor's handle to a selector
      vpr::Selector selector;
      selector.addHandle( handle );
      selector.setIn( handle, vpr::Selector::Read | vpr::Selector::Write | vpr::Selector::Except );

      // a/c: enableNonBlock
      acceptor_socket.enableNonBlocking();
      assertTest( acceptor_socket.getNonBlocking() );
      connector_socket.enableNonBlocking();
      assertTest( connector_socket.getNonBlocking() );

      // a:   bind
      status = acceptor_socket.bind();
      assertTest( status.failure() != true );

      // a:   listen
      status = acceptor_socket.listen();
      assertTest( status.failure() != true );

      // c:   connect
      status = connector_socket.connect();
      assertTest( status.failure() != true );
      if ( status.success() != true )
      {
         assertTest( status.inProgress() == true );
      }

      // block until connection request goes through
      vpr::Uint16 num_events;
      status = selector.select( num_events, vpr::Interval(50000,vpr::Interval::Msec) );
      for (int j = 0; j < selector.getNumHandles(); ++j)
      {
         if (selector.getOut( selector.getHandle(j) ) & (vpr::Selector::Read | vpr::Selector::Except))
         {
            threadAssertTest( handle == selector.getHandle( j )
                  && "Handle doesn't match" );
         }
      }
      assertTest( num_events > 0 && "no events" );


      // a:   accept
      vpr::SocketStream spawned_socket;
      status = acceptor_socket.accept( spawned_socket );
      assertTest( status.success() == true && "accepted socket is NULL" );
      assertTest( spawned_socket.isOpen() && "accepted socket should be open" );

      // s:    write...
      std::string message = "Hi Maynard, My leg hurts";
      int bytes_written;
      status = spawned_socket.write( message, message.size(), bytes_written );
      threadAssertTest( status.success(), "Problem writing in acceptor" );

      // s:    write the max size...
      int size;
      status = spawned_socket.getSendBufferSize( size );
      threadAssertTest( status.success(), "couldn't get the max size for sending data with socket" );
      message.resize( size );
      status = spawned_socket.write( message, message.size(), bytes_written );
      threadAssertTest( status.success(), "maxsize test failed" );

      // block until data is sent
      /*
      status = selector.select( num_events, 50000 );
      for (int j = 0; j < selector.getNumHandles(); ++j)
      {
         if (selector.getOut( selector.getHandle(j) ) & (vpr::Selector::Read | vpr::Selector::Except))
         {
            threadAssertTest( handle == selector.getHandle( j )
                  && "Handle doesn't match" );
         }
      }
      assertTest( num_events > 0 && "no events" );

      // c:     read
      status = connector_socket.read( message, message.size(), bytes_written );
      threadAssertTest( status.success(), "read test failed" );
      */

      // a/c/s: Close
      status = acceptor_socket.close();
      assertTest( status.failure() != true );
      status = connector_socket.close();
      assertTest( status.failure() != true );
      status = spawned_socket.close();
      assertTest( status.failure() != true );
   }

   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("NonBlockingSocketTest");

      test_suite->addTest( new TestCaller<NonBlockingSocketTest>("testSetOpenNonBlocking", &NonBlockingSocketTest::testSetOpenNonBlocking));
      test_suite->addTest( new TestCaller<NonBlockingSocketTest>("testSetOpenNonBlockingThenOpenThenClose", &NonBlockingSocketTest::testSetOpenNonBlockingThenOpenThenClose));
      test_suite->addTest( new TestCaller<NonBlockingSocketTest>("testSetOpenNonBlockingThenOpenThenEnableNonBlockThenClose", &NonBlockingSocketTest::testSetOpenNonBlockingThenOpenThenEnableNonBlockThenClose));
      test_suite->addTest( new TestCaller<NonBlockingSocketTest>("testConnect2NonBlockingSockets", &NonBlockingSocketTest::testConnect2NonBlockingSockets));
      test_suite->addTest( new TestCaller<NonBlockingSocketTest>("testConnect2NonBlockingSocketsUsingSelect", &NonBlockingSocketTest::testConnect2NonBlockingSocketsUsingSelect));

      return test_suite;
   }

protected:
   vpr::Mutex     mMutex;
};

}

#endif
