#ifndef _VPRTEST_SOCKET_COPYCONSTRUCTOR_TEST_H
#define _VPRTEST_SOCKET_COPYCONSTRUCTOR_TEST_H

#include <cppunit/TestCase.h>
#include <cppunit/ThreadTestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/vpr.h>
#include <vpr/Sync/CondVar.h>


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
   void simpleTest();

   // =========================================================================
   // testCopyConnectedSocket -- spawns two threads as connector and acceptor
   // =========================================================================
   void testCopyConnectedSocket();
   void testCopyConstructor_connector(void* arg);
   void testCopyConstructor_acceptor(void* arg);

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
