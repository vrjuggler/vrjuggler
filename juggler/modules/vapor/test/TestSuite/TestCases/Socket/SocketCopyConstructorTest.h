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
       const vpr::Uint16 local_port(12345);
       const vpr::Uint16 remote_port(13579);
       vpr::InetAddr remote_addr;
       vpr::InetAddr local_addr;
       local_addr.setPort(local_port);
       local_addr.setPort(remote_port);
       
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
       assertTest(newSock1->open() && "Can not open copied socket");
       assertTest(newSock2->open() && "Can not open copied socket");
       
       assertTest(newSock2->getType() == newSock1->getType());
       assertTest(newSock2->getRemoteAddr() == newSock1->getRemoteAddr());
       assertTest(newSock2->getLocalAddr() == newSock1->getLocalAddr());
   }
   
   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("SocketCopyConstructorTest");
      
      test_suite->addTest( new TestCaller<SocketCopyConstructorTest>("simpleTest", &SocketCopyConstructorTest::simpleTest));
      return test_suite;
   }

private:
   bool             mThreadAssertTest; // true for no error

   
};

}       // namespace
#endif          
