#ifndef _VPRTEST_SOCKET_SIMULATOR_TEST_H_
#define _VPRTEST_SOCKET_SIMULATOR_TEST_H_

#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ThreadTestCase.h>

#include <vpr/vpr.h>
#include <vpr/Sync/CondVar.h>


namespace vprTest
{

class SocketSimulatorTest : public CppUnit::ThreadTestCase
{
public:
   SocketSimulatorTest (std::string name)
      : CppUnit::ThreadTestCase(name)
   {
      /* Do nothing. */ ;
   }

   SocketSimulatorTest ()
      : CppUnit::ThreadTestCase()
   {
      /* Do nothing. */ ;
   }

   virtual ~SocketSimulatorTest (void) {
      /* Do nothing. */ ;
   }

   void graphConstructionTest(void);

   void singleThreadTest(void);

   void networkCommTest(void);

   void networkFlushTest(void);

   void multiThreadTest(void);

   void multiThreadTest_acceptor(void* arg);

   void multiThreadTest_connector(void* arg);

   static CppUnit::Test*
   suite (void) {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("SocketSimulatorTest");
      test_suite->addTest( new CppUnit::TestCaller<SocketSimulatorTest>("graphConstructionTest", &SocketSimulatorTest::graphConstructionTest));
      test_suite->addTest( new CppUnit::TestCaller<SocketSimulatorTest>("singleThreadTest", &SocketSimulatorTest::singleThreadTest));
      test_suite->addTest( new CppUnit::TestCaller<SocketSimulatorTest>("networkCommTest", &SocketSimulatorTest::networkCommTest));
      test_suite->addTest( new CppUnit::TestCaller<SocketSimulatorTest>("networkFlushTest", &SocketSimulatorTest::networkFlushTest));
//      test_suite->addTest(new CppUnit::TestCaller<SocketSimulatorTest>("multiThreadTest", &SocketSimulatorTest::multiThreadTest));

      return test_suite;
   }

protected:
   vpr::ReturnStatus constructGraph(void);

   enum State {
      ACCEPTOR_READY,
      NOT_READY,
      CONNECTOR_CLOSED,
      CONNECTOR_READY,
      DATA_SENT,
      DONE_READING
   };

   State        mState;
   vpr::CondVar mCondVar;
   vpr::Uint16  mAcceptorPort;
   std::string  mMessage;
   vpr::Uint16  mMessageLen;
};

}

#endif
