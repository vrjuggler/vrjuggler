#include <vpr/vpr.h>
#include <vpr/Util/GUID.h>
//#include <vpr/Util/GUIDFactory.h>
#include <vpr/Util/Interval.h>

#include <vpr/Util/Debug.h>

#include <TestCases/Perf/ProfileNodeTest.h>
#include <vpr/Perf/ProfileManager.h>      // Get all the profile stuff
#include <vpr/Perf/ProfileNode.h>
#include <vpr/System.h>


namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION( ProfileNodeTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ProfileNodeMetricTest, MySuites::metric() );

void ProfileNodeTest::testConstructionAndBasicInterface()
{
   const char test_node_name[] = "test node";
   const char child_node_name[] = "child node";
   const char child_node2_name[] = "child node";

   vpr::ProfileNode* root_node(NULL);
   vpr::ProfileNode* child_node1(NULL);
   vpr::ProfileNode* temp_node(NULL);

   // Test construction
   root_node = new vpr::ProfileNode(test_node_name);
   child_node1 = new vpr::ProfileNode(child_node_name);

   CPPUNIT_ASSERT(root_node->getName() == test_node_name);
   CPPUNIT_ASSERT(child_node1->getName() == child_node_name);
   CPPUNIT_ASSERT(NULL == root_node->getParent());
   CPPUNIT_ASSERT(NULL == child_node1->getParent());

   // Test adding child
   root_node->addChild(child_node1);
   CPPUNIT_ASSERT(root_node == child_node1->getParent());
   CPPUNIT_ASSERT(root_node->getChild() == child_node1);    // Test getChild

   // Test subnode call
   temp_node = root_node->getSubNode(child_node_name);
   CPPUNIT_ASSERT(temp_node == child_node1);
   vpr::ProfileNode* child_node2 = root_node->getSubNode(child_node2_name);
   CPPUNIT_ASSERT(child_node2 != child_node1);
   CPPUNIT_ASSERT(child_node2->getParent() == root_node);      // Check parenting works

   // Test getSibling
   temp_node = root_node->getSibling();
   CPPUNIT_ASSERT(NULL == temp_node);

   temp_node = child_node1->getSibling();
   CPPUNIT_ASSERT(temp_node == child_node2);

   // Test metric initialization
   CPPUNIT_ASSERT(root_node->getTotalCalls() == 0);
   CPPUNIT_ASSERT(root_node->getTotalTime().getBaseVal() == 0);

   // Test getting child using name
   temp_node = root_node->getChild(child_node_name);  // Lookup by pointer
   CPPUNIT_ASSERT(temp_node == child_node1);
   temp_node = root_node->getChild("child node");     // Should fail
   CPPUNIT_ASSERT(NULL == temp_node);
   temp_node = root_node->getNamedChild("child node");   // Should succed
   CPPUNIT_ASSERT(temp_node == child_node1);
}

void ProfileNodeTest::testSamplingInterface()
{
   const char test_node_name[] = "test node";

   // Create node with queue size of 3
   vpr::ProfileNode* test_node = new vpr::ProfileNode(test_node_name, 3);

   vpr::Uint64 prev_total;

   // Make sure everything starts out empty
   CPPUNIT_ASSERT(test_node->getTotalCalls() == 0);
   CPPUNIT_ASSERT(test_node->getTotalTime().getBaseVal() == 0);
   CPPUNIT_ASSERT(test_node->getAverage().getBaseVal() == 0);
   CPPUNIT_ASSERT(test_node->getSTA().getBaseVal() == 0);
   prev_total = test_node->getTotalTime().getBaseVal();

   test_node->startSample();
   vpr::System::usleep(25);
   test_node->stopSample();

   // Make sure everything increments
   CPPUNIT_ASSERT(test_node->getTotalCalls() == 1);
   CPPUNIT_ASSERT(test_node->getTotalTime().getBaseVal() > prev_total);
   CPPUNIT_ASSERT(test_node->getAverage().getBaseVal() != 0);
   CPPUNIT_ASSERT(test_node->getSTA().getBaseVal() != 0);
   prev_total = test_node->getTotalTime().getBaseVal();

   test_node->startSample();
   vpr::System::usleep(25);
   test_node->stopSample();

   // Make sure everything increments
   CPPUNIT_ASSERT(test_node->getTotalCalls() == 2);
   CPPUNIT_ASSERT(test_node->getTotalTime().getBaseVal() > prev_total);
   prev_total = test_node->getTotalTime().getBaseVal();

   // Test resets
   test_node->reset();
   CPPUNIT_ASSERT(test_node->getTotalCalls() == 0);
   CPPUNIT_ASSERT(test_node->getTotalTime().getBaseVal() == 0);
   CPPUNIT_ASSERT(test_node->getAverage().getBaseVal() == 0);
   CPPUNIT_ASSERT(test_node->getSTA().getBaseVal() == 0);
}


// ------------------ Perf Metric --------------------- //
void ProfileNodeMetricTest::testOverhead()
{
/*
   const vpr::Uint32 iters(1000);
   vpr::Uint32 loops = iters;
   vpr::GUID guid1;

   CPPUNIT_METRIC_START_TIMING();


   CPPUNIT_METRIC_STOP_TIMING();
   CPPUNIT_ASSERT_METRIC_TIMING_LE("ProfileNode/TreeOverhead", iters, 0.05f, 0.1f);
*/
}

} // End of vprTest namespace
