#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
//#include <DumperListener.h>

#include <MySuites.h>

#include <exception>
#include <stdexcept>

#ifdef USE_QT_TESTRUNNER
#  include <qapplication.h>
#  include <cppunit/ui/qt/TestRunner.h>
#endif

#include <vpr/vpr.h>
#include <vpr/Util/Debug.h>
#include <vpr/System.h>
#include <jccl/Config/ElementFactory.h>

//#define jccltest_RANDOM_SEED 1


int main(int argc, char* argv[])
{
#ifdef USE_QT_TESTRUNNER
   QApplication app(argc, argv);
#endif

   // Commandline parameter is the test path to use
   std::string test_path = (argc > 1) ? std::string(argv[1]) : "noninteractive";

   // Do this here to get init text out of the way
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << "\n\n-----------------------------------------\n" << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << "Starting test\n" << vprDEBUG_FLUSH;

   // Init random number generation
   unsigned int random_seed;

#ifdef jccltest_RANDOM_SEED
   vpr::TimeVal cur_time;
   vpr::System::gettimeofday(&cur_time);
   random_seed = cur_time.tv_usec;
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << "timeval.usec: " << cur_time.tv_usec << std::endl << vprDEBUG_FLUSH;
#else
   random_seed = 1;                // Use this for repeatability
#endif

   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << " Random seed: " << random_seed << std::endl << vprDEBUG_FLUSH;

   srandom(random_seed);
   srand(random_seed);

   const std::string file_path(TESTFILES_PATH);
   jccl::ElementFactory::instance()->loadDefs(file_path + "definitions");

   //------------------------------------
   //  Test suites
   //------------------------------------
   CppUnit::TestFactoryRegistry& global_registry =
      CppUnit::TestFactoryRegistry::getRegistry();

   // create non-interactive test suite
   CppUnit::TestSuite* noninteractive_suite =
      new CppUnit::TestSuite("noninteractive");
   noninteractive_suite->addTest(global_registry.makeTest());

/*
   // create interactive test suite
   CppUnit::TestSuite* interactive_suite =
      new CppUnit::TestSuite(jcclTest::MySuites::interactive());
   interactive_suite->addTest(CppUnit::TestFactoryRegistry::getRegistry(jcclTest::MySuites::interactive()).makeTest());
*/

#ifdef USE_QT_TESTRUNNER
   CppUnit::QtUi::TestRunner runner;
#else
   // Use a text runner
   CppUnit::TextUi::TestRunner runner;

   // Make it use a compiler outputter
   CppUnit::Outputter* run_outputter =
      CppUnit::CompilerOutputter::defaultOutputter(&runner.result(), std::cout);
   runner.setOutputter(run_outputter);

   CppUnit::TestResult& result_event_manager = runner.eventManager();

   // Add a listener that print test names as the tests progress
   CppUnit::BriefTestProgressListener progress;
   result_event_manager.addListener(&progress);

   // Listener for dumping a lot of stuff
   //DumperListener dumper(true);
   //result_event_manager.addListener(&dumper);
#endif

   // -- ADD SUITES --- //
   runner.addTest(noninteractive_suite);
//   runner.addTest(interactive_suite);

   bool was_successful(false);

#ifdef USE_QT_TESTRUNNER
   runner.run();
#else
   // Run the tests
   try
   {
     std::cout << "Running "  <<  test_path << std::endl;
     was_successful = runner.run( test_path );
     //was_successful = runner.run();
   }
   catch ( std::invalid_argument &e )  // Test path not resolved
   {
     std::cerr  <<  std::endl
                <<  "ERROR: "  <<  e.what()
                << std::endl;
     return 0;
   }
#endif

   // Cleanup
   return was_successful;
}
