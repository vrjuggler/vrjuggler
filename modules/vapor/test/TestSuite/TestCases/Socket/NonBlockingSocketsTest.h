#ifndef _VPRTEST_NB_SOCKET_TEST_H
#define _VPRTEST_NB_SOCKET_TEST_H

#include <string>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>
#include <cppunit/extensions/ThreadTestCase.h>

#include <vpr/vpr.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/CondVar.h>

namespace vprTest
{

class NonBlockingSocketTest : public CppUnit::ThreadTestCase
{
CPPUNIT_TEST_SUITE(NonBlockingSocketTest);
CPPUNIT_TEST( testSetOpenNonBlockingThenOpenThenClose );
CPPUNIT_TEST( testSetOpenNonBlockingThenOpenThenEnableNonBlockThenClose );
CPPUNIT_TEST( testConnect2NonBlockingSockets );
CPPUNIT_TEST( testNonBlockingTransfer );
CPPUNIT_TEST( testConnect2NonBlockingSocketsUsingSelect );
CPPUNIT_TEST( testSendUDP );
CPPUNIT_TEST_SUITE_END();

public:
   NonBlockingSocketTest()
      : CppUnit::ThreadTestCase (), mAcceptorPort(0)
   {
   }

   NonBlockingSocketTest(std::string name)
      : CppUnit::ThreadTestCase (name), mAcceptorPort(0)
   {
   }

   virtual ~NonBlockingSocketTest()
   {
   }


   // Set OpenNonBlocking
   // Open
   // Close
   void testSetOpenNonBlockingThenOpenThenClose();

   // Set OpenNonBlocking
   // Open
   // enableNonBlock
   // Close
   void testSetOpenNonBlockingThenOpenThenEnableNonBlockThenClose();

   // 2 sockets...
   // a/c: Set OpenNonBlocking
   // a/c: Open
   // a/c: enableNonBlock
   // a:   bind
   // a:   listen
   // c:   connect
   // a:   accept
   // a/c/s: Close
   void testConnect2NonBlockingSockets();

   void testNonBlockingTransfer();

   void testNonBlockingTransfer_acceptor(void* arg);

   void testNonBlockingTransfer_connector(void* arg);

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
   void testConnect2NonBlockingSocketsUsingSelect();

   void testSendUDP(void);

   void testSendUDP_receiver(void* arg);

   void testSendUDP_sender(void* arg);
   

protected:
   vpr::Mutex     mMutex;

   enum State {
      ACCEPTOR_READY,
      RECEIVER_READY,
      NOT_READY,
      CONNECTOR_CLOSED,
      DATA_SENT,
      DATA_RECEIVED,
      DONE_READING
   };

   State           mState;         // State variable
   vpr::CondVar    mCondVar;       // Condition variable
   vpr::Uint16     mAcceptorPort;
   vpr::Uint16     mReceiverPort;
   vpr::Uint16     mSenderPort;

   std::string     mMessage;
   vpr::Uint16     mMessageLen;
};

}

#endif
