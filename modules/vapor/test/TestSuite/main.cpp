#include <CppUnit/framework/TestSuite.h>
#include <CppUnit/textui/TestRunner.h>
#include <TestCases/Socket/SocketTest.h>
#include <TestCases/Socket/SocketCopyConstructorTest.h>
#include <TestCases/Thread/ThreadTest.h>
#include <TestCases/IO/Socket/InetAddrTest.h>
#include <TestCases/StatusTest.h>
#include <TestCases/Socket/SocketConnectorAcceptorTest.h>
#include <TestCases/IO/SelectorTest.h>
#include <Utils/Debug.h>


//using namespace vpr;

int main (int ac, char **av)
{
    vprDEBUG(0,0) << "Starting test\n" << vprDEBUG_FLUSH;       // Do this here to get init text out of the way

    TestRunner runner;

   //------------------------------------
   //  noninteractive
   //------------------------------------	
   // create non-interactive test suite
   TestSuite* noninteractive_suite = new TestSuite("NonInteractive");

   // add tests to the suite
   //suite_1->addTest( /* put your test here */ );
   noninteractive_suite->addTest(vprTest::StatusTest::suite());
   noninteractive_suite->addTest(vprTest::InetAddrTest::suite());
   noninteractive_suite->addTest(vprTest::SocketTest::suite());
//   noninteractive_suite->addTest(vprTest::SocketCopyConstructorTest::suite());
   noninteractive_suite->addTest(vprTest::SocketConnectorAcceptorTest::suite());
   noninteractive_suite->addTest(vprTest::SelectorTest::suite());

   // Add the test suite to the runner
   runner.addTest( "noninteractive", noninteractive_suite );

   // create test suite #2
   TestSuite* suite_2 = new TestSuite("Interactive");

   // add tests to the suite
   suite_2->addTest(ThreadTest::suite());

   // Add the test suite to the runner
   runner.addTest("interactive", suite_2);

   // run all test suites
   runner.run( ac, av );


   return 0;
}
