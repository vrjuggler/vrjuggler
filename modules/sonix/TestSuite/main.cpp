#include <framework/TestSuite.h>
#include <textui/TestRunner.h>

#include <TestCases/ajMathTest.h>
#include <TestCases/exampleTest.h>
//#include <Utils/Debug.h>


//using namespace vpr;

int main (int ac, char **av)
{
//    vprDEBUG(0,0) << "Starting test\n" << vprDEBUG_FLUSH;       

    TestRunner runner;

   //------------------------------------
   //  noninteractive
   //------------------------------------	
   // create non-interactive test suite
   TestSuite* noninteractive_suite = new TestSuite("NonInteractive");

   // add tests to the suite
   noninteractive_suite->addTest( ajTest::exampleTest::suite());
   noninteractive_suite->addTest( ajTest::ajMathTest::suite());

   // Add the test suite to the runner
   runner.addTest( "noninteractive", noninteractive_suite );

   // create test suite #2
   TestSuite* interactive_suite = new TestSuite("Interactive");

   // add tests to the suite
   //interactive_suite->addTest(ThreadTest::suite());

   // Add the test suite to the runner
   runner.addTest("interactive", interactive_suite);

   // run all test suites
   runner.run( ac, av );


   return 0;
}
