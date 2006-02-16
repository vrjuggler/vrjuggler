#ifndef _SELECTOR_TEST_H
#define _SELECTOR_TEST_H

#include <vector>
#include <string>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/vpr.h>
#include <vpr/Sync/CondVar.h>


using namespace vpr;

namespace vprTest
{

class SelectorTest : public CppUnit::TestCase
{
CPPUNIT_TEST_SUITE(SelectorTest);
#ifndef VPR_SIMULATOR
CPPUNIT_TEST( testAcceptorPoolSelection );
CPPUNIT_TEST( testSendThenPoll );
#endif
CPPUNIT_TEST_SUITE_END();

public:
   SelectorTest()
   : CppUnit::TestCase ()
   {;}

   SelectorTest(std::string name)
   : CppUnit::TestCase (name)
   {;}

   virtual ~SelectorTest()
   {}

   virtual void setUp()
   {
   }

   virtual void tearDown()
   {
   }

   // Test an acceptor pool
   // This test is based upon the idea of having a pool of acceptors that
   // Another process keeps connecting too.  A selector is used to determine
   // which acceptor is actually being connected to so that only a single
   // accept is needed (that never should really block)
   void testAcceptorPoolSelection();
   void testAcceptorPoolSelection_acceptor();
   void testAcceptorPoolSelection_connector();

   // Test sending data to a group of recievers that are pooled
   // This test is based upon the idea of having a pool of
   // connected sockets.  The connector side is sending, and the acceptor side is recieving.
   //
   // The connector side selects some subgroup of the sockets, marks them as selected, and then sends data on them
   // The acceptor side then selects on the sockets and hopefully it gets that same group back as the ones with pending data
   //
   void testSendThenPoll();
   void testSendThenPoll_acceptor();
   void testSendThenPoll_connector();


protected:
    unsigned     mNumIters;

    vpr::Uint16     mRendevousPort;       // The port the acceptor will be waiting on
    vpr::Uint16     mNumRendevousPorts;   // The number of ports to use

    vpr::Uint16     mMessageLen;        // Length of message to send
    std::string     mMessageValue;      // The value of the message that is supposed to be sent (and recieved)

    enum State { ACCEPTOR_READY, NOT_READY, CONNECTOR_CLOSED, DATA_SENT, DONE_READING };
    State           mState;         // State variable
    vpr::CondVar    mCondVar;       // Condition variable

    std::vector<vpr::Uint16>     mSelectedPorts;      // List of ports that were selected to send data

};

}       // namespace

#endif
