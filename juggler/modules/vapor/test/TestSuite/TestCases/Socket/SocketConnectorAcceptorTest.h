#ifndef _SOCKETCONNECTOR_ACCEPTOR_TEST_H
#define _SOCKETCONNECTOR_ACCEPTOR_TEST_H

#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/ThreadTestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/vpr.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/CondVar.h>

#include <vector>


namespace vprTest
{

class SocketConnectorAcceptorTest : public CppUnit::ThreadTestCase
{
public:
   SocketConnectorAcceptorTest()
   : CppUnit::ThreadTestCase ()
   {;}

   SocketConnectorAcceptorTest(std::string name)
   : CppUnit::ThreadTestCase (name)
   {;}

   virtual ~SocketConnectorAcceptorTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {;}

   // Test the acceptor wrapper construction
   // Things to test
   //  -- Opening acceptor
   //    - With all param type
   //    - Constructor and non-constructor open
   //  -- Closing
   void testAcceptorConstruction();

   // Test Acceptor connector
   // This tests spawns off a thread that starts up an acceptor.
   // Then the main thread starts using a connector to connect to the connector
   // After connection the acceptor sends a message, the connector recieves it and closes the socket
   void testSpawnedAcceptor();
   void testSpawnedAcceptor_acceptor(void* arg);
   void testSpawnedAcceptor_connector(void* arg);

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("SocketConnectorAcceptorTest");

#ifndef VPR_SIMULATOR
      test_suite->addTest( new CppUnit::TestCaller<SocketConnectorAcceptorTest>("testAcceptorConstruction", &SocketConnectorAcceptorTest::testAcceptorConstruction));
      test_suite->addTest( new CppUnit::TestCaller<SocketConnectorAcceptorTest>("testSpawnedAcceptor", &SocketConnectorAcceptorTest::testSpawnedAcceptor));
#endif

      return test_suite;
   }

protected:
    vpr::Uint16     mRendevousPort;     // The port the acceptor will be waiting on
    int             mNumItersA;        // Number of primary iterations
    int             mNumItersB;        // Number of secondary iterations

    vpr::Uint16     mMessageLen;        // Length of message to send
    std::string     mMessageValue;      // The value of the message that is supposed to be sent (and recieved)

    vpr::CondVar    mCondVar;       // Condition variable
    vpr::Mutex      mSyncMutex;     // Mutex used to do synchronization

    enum State { READY, NOT_READY, CONNECTOR_CLOSED
    };
    State           mState;         // State variable

    bool            mReadyFlag1;
    bool            mReadyFlag2;
};

}       // namespace

#endif
