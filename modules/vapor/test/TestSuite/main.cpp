#include <CppUnit/framework/TestSuite.h>
#include <CppUnit/textui/TestRunner.h>

#include <TestCases/Socket/SocketTest.h>
#include <TestCases/Socket/NonBlockingSocketsTest.h>
#include <TestCases/Socket/SocketCopyConstructorTest.h>
#include <TestCases/Socket/SocketConnectorAcceptorTest.h>

#include <TestCases/Thread/ThreadTest.h>

#include <TestCases/IO/Socket/InetAddrTest.h>
#include <TestCases/IO/SelectorTest.h>

#include <TestCases/StatusTest.h>
#include <TestCases/Util/IntervalTest.h>

#include <vpr/Util/Debug.h>
#include <vpr/System.h>

#define vprtest_RANDOM_SEED 1

//using namespace vpr;

int main (int ac, char **av)
{
   vprDEBUG(0,0) << "\n\n-----------------------------------------\n" << vprDEBUG_FLUSH; 
   vprDEBUG(0,0) << "Starting test\n" << vprDEBUG_FLUSH;       // Do this here to get init text out of the way

    // Init random number generation
    unsigned int random_seed;
    
#ifdef vprtest_RANDOM_SEED
    timeval cur_time;
    vpr::System::gettimeofday(&cur_time);
    random_seed = cur_time.tv_usec;
    vprDEBUG(0,0) << "timeval.usec: " << cur_time.tv_usec << std::endl << vprDEBUG_FLUSH;
#else
    random_seed = 1;                // Use this for repeatability
#endif

    vprDEBUG(0,0) << " Random seed: " << random_seed << std::endl << vprDEBUG_FLUSH;

    srandom(random_seed);
    srand(random_seed);

    TestRunner runner;

   //------------------------------------
   //  noninteractive
   //------------------------------------	
   // create non-interactive test suite
   TestSuite* noninteractive_suite = new TestSuite("NonInteractive");

   // add tests to the suite
   //suite_1->addTest( /* put your test here */ );
   noninteractive_suite->addTest(vprTest::StatusTest::suite());
   noninteractive_suite->addTest(vprTest::IntervalTest::suite());
   noninteractive_suite->addTest(vprTest::InetAddrTest::suite());
   noninteractive_suite->addTest(vprTest::SocketTest::suite());
   noninteractive_suite->addTest(vprTest::NonBlockingSocketTest::suite());
//   noninteractive_suite->addTest(vprTest::SocketCopyConstructorTest::suite());
   noninteractive_suite->addTest(vprTest::SocketConnectorAcceptorTest::suite());
   noninteractive_suite->addTest(vprTest::SelectorTest::suite());

   // Add the test suite to the runner
   runner.addTest( "noninteractive", noninteractive_suite );

   // ------------------------------
   // METRICS
   // ------------------------------
   TestSuite* metrics_suite = new TestSuite("metrics");

   metrics_suite->addTest(vprTest::IntervalTest::metric_suite());

   runner.addTest("metrics", metrics_suite);

   //noninteractive_suite->addTest(metrics_suite);


   // -------------------------------
   // INTERACTIVE
   // -------------------------------
   TestSuite* interactive_suite = new TestSuite("interactive");

   interactive_suite->addTest(ThreadTest::suite());

   runner.addTest("interactive", interactive_suite);

   // run all test suites
   runner.run( ac, av );


   return 0;
}
