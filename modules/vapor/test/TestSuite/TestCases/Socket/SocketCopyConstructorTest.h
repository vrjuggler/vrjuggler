#ifndef _VPRTEST_SOCKET_COPYCONSTRUCTOR_TEST_H
#define _VPRTEST_SOCKET_COPYCONSTRUCTOR_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/vpr.h>
#include <vpr/Sync/CondVar.h>


namespace vprTest
{

class SocketCopyConstructorTest : public CppUnit::TestCase
{
CPPUNIT_TEST_SUITE(SocketCopyConstructorTest);
//CPPUNIT_TEST( simpleTest );
//CPPUNIT_TEST( testCopyConnectedSocket );
CPPUNIT_TEST_SUITE_END();

public:
   SocketCopyConstructorTest()
   : CppUnit::TestCase ()
   {;}

   SocketCopyConstructorTest(std::string name)
   : CppUnit::TestCase (name)
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
   void testCopyConstructor_connector();
   void testCopyConstructor_acceptor();

protected:
    vpr::CondVar    mCondVar;       // Condition variable
    enum State { READY, NOT_READY, CONNECTOR_CLOSED
    };
    State           mState;         // State variable
};

}       // namespace
#endif
