#include <vpr/vprConfig.h>

#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/MetricRegistry.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
//#include <DumperListener.h>

#include <MySuites.h>

#include <exception>
#include <stdexcept>

#ifdef USE_QT_TESTRUNNER
   #include <qapplication.h>
   #include <cppunit/ui/qt/TestRunner.h>
#endif

#include <cppunit/extensions/MetricRegistry.h>

#ifdef VPR_SIMULATOR
#  include <vpr/md/SIM/Controller.h>
#endif

#include <vpr/Thread/Thread.h>
#include <vpr/Util/Debug.h>
#include <vpr/System.h>

//#define vprtest_RANDOM_SEED 1

#ifdef VPR_SIMULATOR
static void run (void* arg)
{
   while ( true )
   {
      vpr::sim::Controller::instance()->processNextEvent();
      vpr::Thread::yield();
   }
}
#endif

int main (int argc, char **argv)
{
#ifdef USE_QT_TESTRUNNER
   QApplication app( argc, argv );
#endif

   // Commandline parameter is the test path to use
   std::string test_path = (argc > 1) ? std::string(argv[1]) : "noninteractive";

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

   vprDEBUG(vprDBG_ALL,0) << " Random seed: " << random_seed << std::endl
                          << vprDEBUG_FLUSH;

   srandom(random_seed);
   srand(random_seed);

#ifdef VPR_SIMULATOR       // ------ CONFIGURE SIM NETWORK ------ //
   std::string path_base(GRAPH_PATH);
   vpr::sim::Controller::instance()->constructNetwork(path_base.append("/test/TestSuite/test_network.vsn"));
   vpr::Thread sim_thread(run);
#endif

   // -------- CONFIGURE METRIC REGISTRY ------- //
   CppUnit::MetricRegistry* metric_reg = CppUnit::MetricRegistry::instance();
   std::string metric_prefix;    // Prefix for all metric labels (mode/hostname)

   std::string host_name = vpr::System::getHostname();
   metric_prefix = host_name + "/";
#ifdef _DEBUG
   metric_prefix += "Debug/";
#endif
#ifdef _OPT
   metric_prefix += "Opt/";
#endif
#ifdef VPR_SIMULATOR
   metric_prefix += "Sim/";
#endif

   std::cout << "Setting up metrics for host: " << host_name << std::endl;
   std::cout << "                     prefix: " << metric_prefix << std::endl;

   metric_reg->setPrefix(metric_prefix);
   metric_reg->setFilename("vapor_metrics.txt");
   metric_reg->setMetric("Main/MetricTest", 1221.75f);

   //------------------------------------
   //  Test suites
   //------------------------------------
   CppUnit::TestFactoryRegistry& global_registry = CppUnit::TestFactoryRegistry::getRegistry();

   // create non-interactive test suite
   CppUnit::TestSuite* noninteractive_suite = new CppUnit::TestSuite("noninteractive");
   noninteractive_suite->addTest( global_registry.makeTest());

   // create interactive test suite
   CppUnit::TestSuite* interactive_suite = new CppUnit::TestSuite( vprTest::MySuites::interactive() );
   interactive_suite->addTest( CppUnit::TestFactoryRegistry::getRegistry(vprTest::MySuites::interactive()).makeTest());

   // create interactive test suite
   CppUnit::TestSuite* metric_suite = new CppUnit::TestSuite( vprTest::MySuites::metric() );
   metric_suite->addTest( CppUnit::TestFactoryRegistry::getRegistry(vprTest::MySuites::metric()).makeTest());

#ifdef USE_QT_TESTRUNNER
   CppUnit::QtUi::TestRunner runner;
#else
   // Use a text runner
   CppUnit::TextUi::TestRunner runner;

   // Make it use a compiler outputter
   CppUnit::Outputter* run_outputter = CppUnit::CompilerOutputter::defaultOutputter(
                                       &runner.result(), std::cout );
   runner.setOutputter( run_outputter );

   CppUnit::TestResult& result_event_manager = runner.eventManager();

   // Add a listener that print test names as the tests progress
   CppUnit::BriefTestProgressListener progress;
   result_event_manager.addListener( &progress );

   // Listener for dumping a lot of stuff
   //DumperListener dumper(true);
   //result_event_manager.addListener( &dumper );
#endif

   // -- ADD SUITES --- //
   runner.addTest( noninteractive_suite );
   runner.addTest( interactive_suite );
   runner.addTest( metric_suite );

   bool was_successfull(false);

#ifdef USE_QT_TESTRUNNER
   runner.run();
#else
   // Run the tests
   try
   {
     std::cout << "Running "  <<  test_path << std::endl;
     was_successfull = runner.run( test_path );
     //was_successfull = runner.run( );
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
   return was_successfull;
}

