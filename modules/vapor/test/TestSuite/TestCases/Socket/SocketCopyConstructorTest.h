#ifndef _SOCKET_COPYCONSTRUCTOR_TEST_H
#define _SOCKET_COPYCONSTRUCTOR_TEST_H

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <IO/Socket/Socket.h>
#include <IO/Socket/SocketStream.h>
#include <IO/Socket/InetAddr.h>

#include <include/System.h>

#include <Threads/Thread.h>
#include <Threads/ThreadFunctor.h>
#include <Sync/Mutex.h>
#include <Sync/Cond.h>

#include <vector>

namespace vprTest
{

class SocketCopyConstructorTest : public TestCase
{
public:
   SocketCopyConstructorTest(std::string name)
   : mThreadAssertTest(false), TestCase (name)
   {;}

   virtual ~SocketCopyConstructorTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {
   }
   
   // use this within your threads (CppUnit doesn't catch the assertTest there)
   // then test mThreadAssertTest with assertTest in your parent func.
   // then reset it to true.
   void threadAssertTest( bool testcase, std::string text = std::string("none") )
   {
       if(!testcase)
       {
           mThreadAssertTest = testcase; 
           std::cerr << "threadAssertTest: " << text << std::endl;
       }
       else
           std::cout << "." << std::flush;
   }
   void testAssertReset()
   { mThreadAssertTest = true; }
   
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
       assertTest(sock->getType() == newSock1->getType());       
       assertTest(sock->getRemoteAddr() == newSock1->getRemoteAddr());
       assertTest(sock->getLocalAddr() == newSock1->getLocalAddr());
       
       delete sock;
       assertTest(newSock1->open() && "Can not open copied socket1");
       assertTest(newSock2->open() && "Can not open copied socket2");
       
       assertTest(newSock2->getType() == newSock1->getType());
       assertTest(newSock2->getRemoteAddr() == newSock1->getRemoteAddr());
       assertTest(newSock2->getLocalAddr() == newSock1->getLocalAddr());
       
       assertTest(newSock1->close() && "Can not close copied socket1");
       assertTest(newSock2->close() && "Can not close copied socket2");
       
       delete newSock1;
       delete newSock2;
   }
   
   // =========================================================================
   // testCopyConnectedSocket -- spawns two threads as connector and acceptor
   // =========================================================================
   void testCopyConnectedSocket()
   {
      testAssertReset();
      
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
      
      assertTest( mThreadAssertTest );
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
       
       result = connector_socket.open();
       threadAssertTest((result != false), "Can not open connector socket");
       
       // WAIT for READY 
       mCondVar.acquire();
       {
          while(mState != READY)
             mCondVar.wait();
       }
       mCondVar.release();
       
       result1 = connector_socket.connect();
       threadAssertTest((result1 != false), "connect fails");
       
       copy_connector = new vpr::SocketStream (connector_socket);
       
       if (result1 != false) {
         result = copy_connector->read (buffer, 40);
         if (result>0)
            copy_connector->close();
       }
       
       threadAssertTest(copy_connector->getType() == connector_socket.getType());       
       threadAssertTest(copy_connector->getRemoteAddr() == connector_socket.getRemoteAddr());
       threadAssertTest(copy_connector->getLocalAddr() == connector_socket.getLocalAddr());
       
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
      const int backlog = 5;
      bool result = 0;
      char buffer[]="Hello!";
      vpr::InetAddr local_addr;
      local_addr.setPort(port);
      vpr::SocketStream acceptor_socket(local_addr, vpr::InetAddr::AnyAddr);
       
      vpr::SocketStream* child_socket(NULL);
      vpr::SocketStream* copy_acceptor(NULL);
      vpr::SocketStream* copy_child(NULL);
       
      result = acceptor_socket.open();
      threadAssertTest((result != false), "Can not open acceptor socket");
      
      result = acceptor_socket.bind();
      
      if (result == false)
         threadAssertTest((false), "Can not bind acceptor socket");
      else { 
         result = acceptor_socket.listen( backlog );
         threadAssertTest((result != false), "acceptor listening fails");
         // READY - Tell everyone that we are ready to accept
            mCondVar.acquire();
            {
               mState = READY;
               mCondVar.signal();       // Tell any waiting threads
            }
            mCondVar.release();       
         child_socket = acceptor_socket.accept();
         threadAssertTest((child_socket != NULL), "accept() fails");
      }
       
      copy_acceptor = new vpr::SocketStream (acceptor_socket);
      if (child_socket != NULL) copy_child = new vpr::SocketStream (*child_socket);
       
      threadAssertTest(copy_acceptor->getType() == acceptor_socket.getType());
      threadAssertTest(copy_acceptor->getRemoteAddr() == acceptor_socket.getRemoteAddr());
      threadAssertTest(copy_acceptor->getLocalAddr() == acceptor_socket.getLocalAddr());
       
      if (copy_child != NULL) {
         copy_child->write(buffer, sizeof(buffer));
         threadAssertTest(copy_child->getType() == child_socket->getType());
         threadAssertTest(copy_child->getRemoteAddr() == child_socket->getRemoteAddr());
         threadAssertTest(copy_child->getLocalAddr() == child_socket->getLocalAddr());
      }
      
      // WAIT for close
      mCondVar.acquire();         
         while(mState != CONNECTOR_CLOSED)
            mCondVar.wait();             
      mCondVar.release();
      
      result = acceptor_socket.close();
      threadAssertTest((result != false), "acceptor close fails");
      if (child_socket != NULL) child_socket->close();
       
      copy_acceptor->close();
      if (copy_child != NULL) copy_child->close();
       
      if (child_socket != NULL) delete child_socket;
      delete copy_acceptor;
      if (copy_child != NULL) delete copy_child;
   }
   
   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("SocketCopyConstructorTest");      
      test_suite->addTest( new TestCaller<SocketCopyConstructorTest>("simpleTest", &SocketCopyConstructorTest::simpleTest));
      test_suite->addTest( new TestCaller<SocketCopyConstructorTest>("testCopyConnectedSocket", &SocketCopyConstructorTest::testCopyConnectedSocket));
      return test_suite;
   }

private:
   bool             mThreadAssertTest; // true for no error
   
protected:
    vpr::Cond       mCondVar;       // Condition variable
    enum State { READY, NOT_READY, CONNECTOR_CLOSED
    };
    State           mState;         // State variable   
};

}       // namespace
#endif          
