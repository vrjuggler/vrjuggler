#include <vpr/vpr.h>
#include <vpr/Util/GUID.h>
//#include <vpr/Util/GUIDFactory.h>
#include <vpr/Util/Interval.h>

#include <vpr/Util/Debug.h>

#include <TestCases/Perf/PerfTest.h>
#include <vpr/Perf/ProfileManager.h>      // Get all the profile stuff
#include <vpr/System.h>


namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION( PerfTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( PerfMetricTest, MySuites::metric() );

void PerfTest::testConstructTree()
{
   vpr::ProfileManager::startProfile("First");
      vpr::ProfileManager::startProfile("First-one");
      vpr::ProfileManager::stopProfile();
      vpr::ProfileManager::startProfile("First-two");
      vpr::ProfileManager::stopProfile();
      vpr::ProfileManager::startProfile("First-three");
         vpr::ProfileManager::startProfile("First-three-one");
         vpr::ProfileManager::stopProfile();
      vpr::ProfileManager::stopProfile();
      vpr::ProfileManager::startProfile("First-four");
      vpr::ProfileManager::stopProfile();
   vpr::ProfileManager::stopProfile();
   vpr::ProfileManager::startProfile("Second");
   vpr::ProfileManager::stopProfile();

   vpr::ProfileNode* root_node = vpr::ProfileManager::getRootNode();
   vpr::ProfileNode* first_node = root_node->getChild();
   vpr::ProfileNode* first_one_node = first_node->getChild();

   CPPUNIT_ASSERT(first_node->getParent() == root_node);
   CPPUNIT_ASSERT(first_one_node->getParent() == first_node);

   std::cout << "Printing tree:\n" << std::flush;
   root_node->printTree();
}

void PerfTest::testNamedLookupSample()
{
   vpr::ProfileManager::startProfile("myNamedProfile");
   vpr::System::usleep(25);
   vpr::ProfileManager::stopProfile();

   float f = vpr::ProfileManager::getNamedNodeSample("myNamedProfile");
   CPPUNIT_ASSERT(f != 0.0f);
}

void PerfTest::testReset()
{
   //CPPUNIT_ASSERT(false);
}

// ------------------ Perf Metric --------------------- //
void PerfMetricTest::testTreeOverhead()
{
   const vpr::Uint32 iters(1000);
   vpr::Uint32 loops = iters;
   vpr::GUID guid1;

   CPPUNIT_METRIC_START_TIMING();

   while(loops--)
   {
     vpr::ProfileManager::startProfile("MyFirst");
     vpr::ProfileManager::startProfile("MySecond");
     vpr::ProfileManager::stopProfile();
     vpr::ProfileManager::startProfile("MySecondAgain");
     vpr::ProfileManager::stopProfile();
     vpr::ProfileManager::stopProfile();
   }

   CPPUNIT_METRIC_STOP_TIMING();
   CPPUNIT_ASSERT_METRIC_TIMING_LE("PerfTest/TreeOverhead", iters, 0.05f, 0.1f);
}

} // End of vprTest namespace
