#include <cppunit/TestSuite.h>
#include <cppunit/TextTestRunner.h>

#include <TestCases/VarValueTest.h>
#include <TestCases/ChunkDescTest.h>
#include <TestCases/IncludesTest.h>

#include <vpr/Util/Debug.h>
#include <vpr/System.h>

//#define vprtest_RANDOM_SEED 1


int main (int ac, char **av)
{
   vprDEBUG(vprDBG_ALL,0) << "\n\n-----------------------------------------\n" << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,0) << "Starting test\n" << vprDEBUG_FLUSH;       // Do this here to get init text out of the way

    // Init random number generation
    unsigned int random_seed;

#ifdef vprtest_RANDOM_SEED
    timeval cur_time;
    vpr::System::gettimeofday(&cur_time);
    random_seed = cur_time.tv_usec;
    vprDEBUG(vprDBG_ALL,0) << "timeval.usec: " << cur_time.tv_usec << std::endl << vprDEBUG_FLUSH;
#else
    random_seed = 1;                // Use this for repeatability
#endif

    vprDEBUG(vprDBG_ALL,0) << " Random seed: " << random_seed << std::endl << vprDEBUG_FLUSH;

    srandom(random_seed);
    srand(random_seed);

   CppUnit::TextTestRunner runner;

   //------------------------------------
   //  noninteractive
   //------------------------------------	
   // create non-interactive test suite
   CppUnit::TestSuite* noninteractive_suite = new CppUnit::TestSuite("noninteractive");

   // add tests to the suite
   noninteractive_suite->addTest(jcclTest::VarValueTest::suite());
   noninteractive_suite->addTest(jcclTest::ChunkDescTest::suite());
   noninteractive_suite->addTest(jcclTest::IncludesTest::suite());

   // Add the test suite to the runner
   runner.addTest( noninteractive_suite );

   // ------------------------------
   // METRICS
   // ------------------------------
   CppUnit::TestSuite* metrics_suite = new CppUnit::TestSuite("metrics");

//     metrics_suite->addTest(vprTest::IntervalTest::metric_suite());
//     metrics_suite->addTest(vprTest::GUIDTest::metric_suite());
//     metrics_suite->addTest(vprTest::SocketBandwidthIOStatsTest::metric_suite());

   runner.addTest(metrics_suite);

   //noninteractive_suite->addTest(metrics_suite);


   // -------------------------------
   // INTERACTIVE
   // -------------------------------
   CppUnit::TestSuite* interactive_suite = new CppUnit::TestSuite("interactive");

//     interactive_suite->addTest(vprTest::ThreadTest::suite());

   runner.addTest(interactive_suite);

   // run all test suites
   if ( ac > 1 )
   {
      if ( strcmp(av[1], "interactive") == 0 )
      {
         runner.run("interactive");
      }
      else if ( strcmp(av[1], "noninteractive") == 0 )
      {
         runner.run("noninteractive");
      }
      else if ( strcmp(av[1], "metrics") == 0 )
      {
         runner.run("metrics");
      }
      else if ( strcmp(av[1], "all") == 0 )
      {
         runner.run("noninteractive");
         runner.run("metrics");
         runner.run("interactive");
      }
   }
   else
   {
      runner.run("noninteractive");
      runner.run("metrics");
      runner.run("interactive");
   }

   return 0;
}
