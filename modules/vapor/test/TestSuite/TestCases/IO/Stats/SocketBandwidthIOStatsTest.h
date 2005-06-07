#ifndef _SOCKET_BANDWIDTH_IO_STATS_TEST_H
#define _SOCKET_BANDWIDTH_IO_STATS_TEST_H

#include <vector>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

#include <cppunit/TestCase.h>
#include <cppunit/extensions/ThreadTestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/CondVar.h>


namespace vprTest
{

class SocketBandwidthIOStatsTest : public CppUnit::ThreadTestCase
{
public:
   SocketBandwidthIOStatsTest()
   : CppUnit::ThreadTestCase ()
   {;}

   SocketBandwidthIOStatsTest(std::string name)
   : CppUnit::ThreadTestCase (name)
   {;}

   virtual ~SocketBandwidthIOStatsTest()
   {;}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {;}


   // Based on: SocketConnectorAcceptor::Test Acceptor connector
   // This tests spawns off a thread that starts up an acceptor.
   // Then the main thread starts using a connector to connect to the connector
   // After connection the acceptor sends a message, the connector recieves it and closes the socket
   void testBandwidth();
   void testBandwidth_acceptor(void* arg);
   void testBandwidth_connector(void* arg);

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("SocketBandwidthIOStatsTest");

      //test_suite->addTest( new CppUnit::TestCaller<SocketConnectorAcceptorTest>("testAcceptorConstruction", &SocketConnectorAcceptorTest::testAcceptorConstruction));
      //test_suite->addTest( new CppUnit::TestCaller<SocketConnectorAcceptorTest>("testSpawnedAcceptor", &SocketConnectorAcceptorTest::testSpawnedAcceptor));

      return test_suite;
   }

   /*
   static CppUnit::Test* metric_suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("SocketBandwidthIOStatsTest_metric");

      test_suite->addTest( new CppUnit::TestCaller<SocketBandwidthIOStatsTest>("testBandwidth", &SocketBandwidthIOStatsTest::testBandwidth));

      return test_suite;
   }
   */


protected:
    vpr::Uint16     mRendevousPort;     // The port the acceptor will be waiting on
    int             mNumItersA;        // Number of primary iterations
    int             mNumItersB;        // Number of secondary iterations

    vpr::Uint16               mMessageLen;        // Length of message to send
    std::vector<vpr::Uint8>   mMessageValue;      // The value of the message that is supposed to be sent (and recieved)

    vpr::CondVar    mCondVar;       // Condition variable
    vpr::Mutex      mSyncMutex;     // Mutex used to do synchronization

    enum State { READY, NOT_READY, CONNECTOR_CLOSED
    };
    State           mState;         // State variable

    bool            mReadyFlag1;
    bool            mReadyFlag2;
};

};

#endif
