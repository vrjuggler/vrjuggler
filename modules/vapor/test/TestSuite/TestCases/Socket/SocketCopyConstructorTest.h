#ifndef _SOCKET_COPYCONSTRUCTOR_TEST_H
#define _SOCKET_COPYCONSTRUCTOR_TEST_H

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/ThreadTestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/IO/Socket/Socket.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Socket/InetAddr.h>

#include <vpr/System.h>

#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/CondVar.h>

#include <vector>

namespace vprTest
{

class SocketCopyConstructorTest : public CppUnit::ThreadTestCase
{
public:
   SocketCopyConstructorTest()
   : CppUnit::ThreadTestCase ()
   {;}

   SocketCopyConstructorTest(std::string name)
   : CppUnit::ThreadTestCase (name)
   {;}

   virtual ~SocketCopyConstructorTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {;}

   // =========================================================================
   // Simple test-- only test copy unconnected socket
   // =========================================================================
   void simpleTest()
   {
       const vpr::Uint16 remote_port(13579);
       vpr::InetAddr remote_addr;
       remote_addr.setPort(remote_port);

       vpr::SocketStream*	sock(NULL);
       vpr::SocketStream*  newSock1(NULL);
       vpr::SocketStream*  newSock2(NULL);
       sock = new vpr::SocketStream( vpr::InetAddr::AnyAddr, remote_addr );

       newSock1= new vpr::SocketStream (*sock);
       newSock2= new vpr::SocketStream (*sock);
       CPPUNIT_ASSERT(sock->getType() == newSock1->getType());
       CPPUNIT_ASSERT(sock->getRemoteAddr() == newSock1->getRemoteAddr());
       CPPUNIT_ASSERT(sock->getLocalAddr() == newSock1->getLocalAddr());

       delete sock;
       CPPUNIT_ASSERT(newSock1->open().success() && "Can not open copied socket1");
       CPPUNIT_ASSERT(newSock2->open().success() && "Can not open copied socket2");

       CPPUNIT_ASSERT(newSock2->getType() == newSock1->getType());
       CPPUNIT_ASSERT(newSock2->getRemoteAddr() == newSock1->getRemoteAddr());
       CPPUNIT_ASSERT(newSock2->getLocalAddr() == newSock1->getLocalAddr());

       CPPUNIT_ASSERT(newSock1->close().success() && "Can not close copied socket1");
       CPPUNIT_ASSERT(newSock2->close().success() && "Can not close copied socket2");

       delete newSock1;
       delete newSock2;
   }

   // =========================================================================
   // testCopyConnectedSocket -- spawns two threads as connector and acceptor
   // =========================================================================
   void testCopyConnectedSocket()
   {
      threadAssertReset();

      mState = NOT_READY;                        // Initialize

      // Spawn acceptor thread
      vpr::ThreadMemberFunctor<SocketCopyConstructorTest>
            acceptor_functor( this, &SocketCopyConstructorTest::testCopyConstructor_acceptor );
      vpr::Thread acceptor_thread( &acceptor_functor );

      // spawn a connector thread
      vpr::ThreadMemberFunctor<SocketCopyConstructorTest>
            connector_functor( this, &SocketCopyConstructorTest::testCopyConstructor_connector );
      vpr::Thread connector_thread( &connector_functor );

      connector_thread.join();
      acceptor_thread.join();

      checkThreadAssertions();
   }

   void testCopyConstructor_connector(void* arg)
   {
       const vpr::Uint16 port(13579);
       vpr::InetAddr remote_addr;
       bool result = 0;
       bool result1 = 0;
       char buffer[40];
       remote_addr.setPort(port);
       vpr::SocketStream connector_socket(vpr::InetAddr::AnyAddr, remote_addr);
       vpr::SocketStream* copy_connector(NULL);

       result = connector_socket.open().success();
       assertTestThread((result != false) && "Can not open connector socket");

       // WAIT for READY
       mCondVar.acquire();
       {
          while(mState != READY)
             mCondVar.wait();
       }
       mCondVar.release();

       result1 = connector_socket.connect().success();
       assertTestThread((result1 != false) && "connect fails");

       copy_connector = new vpr::SocketStream (connector_socket);

       if (result1 != false) {
         vpr::Uint32 bytes_read;
         vpr::ReturnStatus status = copy_connector->read (buffer, 40, bytes_read);
         if (status.success())
            copy_connector->close();
       }

       assertTestThread(copy_connector->getType() == connector_socket.getType());
       assertTestThread(copy_connector->getRemoteAddr() == connector_socket.getRemoteAddr());
       assertTestThread(copy_connector->getLocalAddr() == connector_socket.getLocalAddr());

       connector_socket.close();
       copy_connector->close();

       // Tell everyone that we closed
       mCondVar.acquire();
       {
          mState = CONNECTOR_CLOSED;
          mCondVar.signal();       // Tell any waiting threads
       }
       mCondVar.release();

       delete copy_connector;
   }

    void testCopyConstructor_acceptor(void* arg)
   {
      const vpr::Uint16 port(13579);
      bool result = 0;
      char buffer[]="Hello!";
      vpr::InetAddr local_addr;
      local_addr.setPort(port);
      vpr::SocketStream acceptor_socket(local_addr, vpr::InetAddr::AnyAddr);

      vpr::SocketStream child_socket;
      vpr::SocketStream* copy_acceptor(NULL);
      vpr::SocketStream* copy_child(NULL);
      vpr::ReturnStatus child_create_status;

      result = acceptor_socket.open().success();
      assertTestThread((result != false) && "Can not open acceptor socket");

      result = acceptor_socket.bind().success();

      if (result == false)
         assertTestThread((false) && "Can not bind acceptor socket");
      else {
         result = acceptor_socket.listen().success();
         assertTestThread((result != false) && "acceptor listening fails");
         // READY - Tell everyone that we are ready to accept
            mCondVar.acquire();
            {
               mState = READY;
               mCondVar.signal();       // Tell any waiting threads
            }
            mCondVar.release();
         child_create_status = acceptor_socket.accept(child_socket);
         assertTestThread(child_create_status.success() && "accept() fails");
      }

      copy_acceptor = new vpr::SocketStream (acceptor_socket);
      if (child_create_status.success()) {
         copy_child = new vpr::SocketStream (child_socket);
      }

      assertTestThread(copy_acceptor->getType() == acceptor_socket.getType());
      assertTestThread(copy_acceptor->getRemoteAddr() == acceptor_socket.getRemoteAddr());
      assertTestThread(copy_acceptor->getLocalAddr() == acceptor_socket.getLocalAddr());

      if (copy_child != NULL) {
         vpr::Uint32 bytes_written;
         copy_child->write(buffer, sizeof(buffer), bytes_written);
         assertTestThread(copy_child->getType() == child_socket.getType());
         assertTestThread(copy_child->getRemoteAddr() == child_socket.getRemoteAddr());
         assertTestThread(copy_child->getLocalAddr() == child_socket.getLocalAddr());
      }

      // WAIT for close
      mCondVar.acquire();
         while(mState != CONNECTOR_CLOSED)
            mCondVar.wait();
      mCondVar.release();

      result = acceptor_socket.close().success();
      assertTestThread((result != false) && "acceptor close fails");
      if (child_create_status.success()) child_socket.close();

      copy_acceptor->close();
      if (copy_child != NULL) copy_child->close();

      delete copy_acceptor;
      if (copy_child != NULL) delete copy_child;
   }

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite *test_suite = new CppUnit::TestSuite ("SocketCopyConstructorTest");
      test_suite->addTest( new CppUnit::TestCaller<SocketCopyConstructorTest>("simpleTest", &SocketCopyConstructorTest::simpleTest));
      test_suite->addTest( new CppUnit::TestCaller<SocketCopyConstructorTest>("testCopyConnectedSocket", &SocketCopyConstructorTest::testCopyConnectedSocket));
      return test_suite;
   }

protected:
    vpr::CondVar    mCondVar;       // Condition variable
    enum State { READY, NOT_READY, CONNECTOR_CLOSED
    };
    State           mState;         // State variable
};

}       // namespace
#endif
