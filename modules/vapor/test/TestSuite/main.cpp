#include <string.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TextTestResult.h>

#include <TestCases/Socket/SocketTest.h>
#include <TestCases/Socket/NonBlockingSocketsTest.h>
#include <TestCases/Socket/SocketCopyConstructorTest.h>
#include <TestCases/Socket/SocketConnectorAcceptorTest.h>

#include <TestCases/Thread/ThreadTest.h>
#include <TestCases/Thread/SignalTest.h>

#include <TestCases/IO/Socket/InetAddrTest.h>
#include <TestCases/IO/SelectorTest.h>

#include <TestCases/IO/Stats/SocketBandwidthIOStatsTest.h>

#include <TestCases/Util/ReturnStatusTest.h>
#include <TestCases/Util/IntervalTest.h>
#include <TestCases/Util/GUIDTest.h>

#include <TestCases/BoostTest.h>
#include <TestCases/SystemTest.h>

#include <vpr/Util/Debug.h>
#include <vpr/System.h>

//#define vprtest_RANDOM_SEED 1

//using namespace vpr;

static void addNoninteractive (CppUnit::TestSuite* suite)
{
   static vprTest::ReturnStatusTest return_test;
   static vprTest::BoostTest boost_test;
   static vprTest::SystemTest system_test;
   static vprTest::IntervalTest interval_test;
   static vprTest::GUIDTest guid_test;
   static vprTest::InetAddrTest inet_addr_test;
   static vprTest::SocketTest sock_test;
   static vprTest::NonBlockingSocketTest non_block_test;
   static vprTest::SocketCopyConstructorTest sock_copy_test;
   static vprTest::SocketConnectorAcceptorTest sock_conn_accept_test;
   static vprTest::SelectorTest selector_test;
//   static vprTest::SignalTest signal_test;

   // add tests to the suite
   return_test.registerTests(suite);
   boost_test.registerTests(suite);
   system_test.registerTests(suite);
   interval_test.registerTests(suite);
   guid_test.registerTests(suite);
   inet_addr_test.registerTests(suite);
   sock_test.registerTests(suite);
   non_block_test.registerTests(suite);
//   sock_copy_test.registerTests(suite);
   sock_conn_accept_test.registerTests(suite);
   selector_test.registerTests(suite);
//   signal_test.registerTests(suite);
}

static void addMetrics (CppUnit::TestSuite* suite)
{
   static vprTest::IntervalTest interval_test;
   static vprTest::GUIDTest guid_test;
   static vprTest::SocketBandwidthIOStatsTest sock_bw_test;

   interval_test.registerMetricsTests(suite);
   guid_test.registerMetricsTests(suite);
   sock_bw_test.registerMetricsTests(suite);
}

static void addInteractive (CppUnit::TestSuite* suite)
{
   static vprTest::ThreadTest thread_test;

   thread_test.registerTests(suite);
}

int main (int ac, char **av)
{
   vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "\n\n-----------------------------------------\n" << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "Starting test\n" << vprDEBUG_FLUSH;       // Do this here to get init text out of the way

    // Init random number generation
    unsigned int random_seed;

#ifdef vprtest_RANDOM_SEED
    timeval cur_time;
    vpr::System::gettimeofday(&cur_time);
    random_seed = cur_time.tv_usec;
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "timeval.usec: " << cur_time.tv_usec << std::endl << vprDEBUG_FLUSH;
#else
    random_seed = 1;                // Use this for repeatability
#endif

    vprDEBUG(vprDBG_ALL,0) << " Random seed: " << random_seed << std::endl << vprDEBUG_FLUSH;

    srandom(random_seed);
    srand(random_seed);

   CppUnit::TestSuite suite;

   if ( ac > 1 && strcmp(av[1], "all") != 0 ) {
      for ( int i = 1; i < ac; i++ ) {
         //------------------------------------
         //  noninteractive
         //------------------------------------	
         // create non-interactive test suite
         if ( strcmp(av[i], "noninteractive") == 0 ) {
            addNoninteractive(&suite);
         }
         // ------------------------------
         // METRICS
         // ------------------------------
         else if ( strcmp(av[i], "metrics") == 0 ) {
            addMetrics(&suite);
         }
         // -------------------------------
         // INTERACTIVE
         // -------------------------------
         else if ( strcmp(av[i], "interactive") == 0 ) {
            addInteractive(&suite);
         }
         else {
            std::cerr << "WARNING: Unknown suite name " << av[i] << std::endl;
         }
      }
   }
   else {
      addNoninteractive(&suite);
      addMetrics(&suite);
      addInteractive(&suite);
   }

   CppUnit::TextTestResult result;
   suite.run(&result);
   result.print(std::cout);

   return 0;
}
