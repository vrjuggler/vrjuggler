#include <CppUnit/framework/TestSuite.h>
#include <CppUnit/textui/TestRunner.h>
#include <TestCases/Socket/SocketTest.h>
#include <TestCases/Thread/ThreadTest.h>
#include <TestCases/IO/Socket/InetAddrTest.h>


//using namespace vpr;

int main (int ac, char **av)
{
   TestRunner runner;

   //------------------------------------
   //  noninteractive
   //------------------------------------	
   // create non-interactive test suite
   TestSuite* noninteractive_suite = new TestSuite("NonInteractive");

   // add tests to the suite
   //suite_1->addTest( /* put your test here */ );
   noninteractive_suite->addTest(vprTest::InetAddrTest::suite());
   noninteractive_suite->addTest(vprTest::SocketTest::suite());

   // Add the test suite to the runner
   runner.addTest( "noninteractive", noninteractive_suite );

   // create test suite #2
   TestSuite* suite_2 = new TestSuite("suite_2");

   // add tests to the suite
   //suite_2->addTest(gmtlTest::Vec3Test::suite());
   //suite_2->addTest(gmtlTest::MatrixTest::suite());
   //suite_2->addTest(gmtlTest::Point3Test::suite());
   suite_2->addTest(ThreadTest::suite());

   // Add the test suite to the runner
   runner.addTest("suite_2_tests", suite_2);

   // run all test suites
   runner.run( ac, av );


   return 0;
}
