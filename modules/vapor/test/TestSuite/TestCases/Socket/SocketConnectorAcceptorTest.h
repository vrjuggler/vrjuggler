#ifndef _SOCKETCONNECTOR_ACCEPTOR_TEST_H
#define _SOCKETCONNECTOR_ACCEPTOR_TEST_H

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <IO/Socket/Socket.h>
#include <IO/Socket/SocketStream.h>
#include <IO/Socket/InetAddr.h>
#include <IO/Socket/SocketAcceptor.h>

#include <include/System.h>

#include <Threads/Thread.h>
#include <Threads/ThreadFunctor.h>
#include <Sync/Mutex.h>

#include <vector>


namespace vprTest
{

class SocketConnectorAcceptorTest : public TestCase
{
public:
   SocketConnectorAcceptorTest( std::string name )
   : mThreadAssertTest(false), TestCase (name)
   {;}

   virtual ~SocketConnectorAcceptorTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {
   }
   
   // use this within your threads (CppUnit doesn't catch the assertTest there)
   // then test mThreadAssertTest with assertTest in your parent func.
   // then reset it to true.
   void threadAssertTest( bool testcase )
   { mThreadAssertTest = testcase; }
   void testAssertReset()
   { mThreadAssertTest = false; }


   // Test the acceptor wrapper construction
   // Things to test
   //  -- Opening acceptor
   //    - With all param type
   //    - Constructor and non-constructor open
   //  -- Closing
   void testAcceptorConstruction()
   {
       const vpr::Uint16 server_port(12021);
       vpr::InetAddr local_addr;
       local_addr.setPort(server_port);
       vpr::SocketAcceptor* test_acceptor(NULL);

       // Test address in constructor
       test_acceptor = new vpr::SocketAcceptor(local_addr);
       assertTest(test_acceptor != NULL);
       assertTest(test_acceptor->getSocket().isOpen());
       test_acceptor->close();
       delete(test_acceptor);

       // Test address in constructor (with backlog)
       test_acceptor = new vpr::SocketAcceptor(local_addr,7);
       assertTest(test_acceptor != NULL);
       assertTest(test_acceptor->getSocket().isOpen());
       test_acceptor->close();
       delete(test_acceptor);

       // Test default constructor
       test_acceptor = new vpr::SocketAcceptor();
       assertTest(test_acceptor != NULL);
       test_acceptor->open(local_addr);
       assertTest(test_acceptor->getSocket().isOpen());
       test_acceptor->close();
       delete(test_acceptor);

       // Test default constructor (with backlog)
       test_acceptor = new vpr::SocketAcceptor();
       assertTest(test_acceptor != NULL);
       test_acceptor->open(local_addr,7);
       assertTest(test_acceptor->getSocket().isOpen());
       test_acceptor->close();
       delete(test_acceptor);
   }

   
   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("SocketConnectorAcceptorTest");

      test_suite->addTest( new TestCaller<SocketConnectorAcceptorTest>("testAcceptorConstruction", &SocketConnectorAcceptorTest::testAcceptorConstruction));      
      return test_suite;
   }

private:   
   bool mThreadAssertTest; // true for no error

protected:
   
};

}       // namespace

#endif          
