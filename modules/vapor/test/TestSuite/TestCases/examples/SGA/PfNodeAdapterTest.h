#include <gfxConfig.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <Performer/pf.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pf/pfGroup.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pr/pfObject.h>

#include <SGA/PfNodeAdapters.h>

// Containers to test with it
#include <SGA/ObbNodeContainer.h>


#include <gfxConfig.h>
#include <Util/Assert.h>


namespace sgaTest
{

class PfNodeAdapterTest : public TestCase
{
public:
   PfNodeAdapterTest( std::string name )
   : TestCase (name)
   //PfTriangleListAdapterTest()
   {;}

   virtual ~PfNodeAdapterTest()
   {}


   virtual void setUp()
   {
      if(!pfIsInited())
      {
         pfInit();
         pfInitState(NULL);
      }

      if(!pfIsConfiged())
      {
         pfConfig();
      }
   }

   virtual void tearDown()
   {
   }

   // Verify that performer returns NULL by default for unassigned slots
   void testNullSlotDefault()
   {
      int test_slot_id;
      test_slot_id = pfObject::getNamedUserDataSlot("PfNodeAdapterTest_testNull");
      std::cout << "test_slot_id: " << test_slot_id << std::endl;

      //pfNode* test_node = new pfNode;
      pfGroup* test_group = new pfGroup;
      pfDCS* test_dcs = new pfDCS;

      //assertTest(test_node->getUserData(test_slot_id) == NULL);
      assertTest(test_group->getUserData(test_slot_id) == NULL);
      assertTest(test_dcs->getUserData(test_slot_id) == NULL);
   }

   void testNodeAdapterInit()
   {
      pfGroup* test_group = new pfGroup;
      sga::NodeAdapter adapted_group(test_group);

      assertTest((&adapted_group.contHolder()) != NULL);
   }

   void testNodeAdapterContainer()
   {
      pfGroup* test_group = new pfGroup;
      sga::NodeAdapter adapted_group(test_group);

      sga::ObbNodeContainer* obb_cont = new sga::ObbNodeContainer;
      sga::ObbNodeContainer* ret_obb_cont;

      ret_obb_cont = adapted_group.contHolder().getContainer<sga::ObbNodeContainer>();
      assertTest(ret_obb_cont == NULL);

      adapted_group.contHolder().setContainer(obb_cont);
      ret_obb_cont = adapted_group.contHolder().getContainer<sga::ObbNodeContainer>();
      assertTest(obb_cont == ret_obb_cont);

      adapted_group.contHolder().setContainer<sga::ObbNodeContainer>(obb_cont);
      ret_obb_cont = adapted_group.contHolder().getContainer<sga::ObbNodeContainer>();
      assertTest(obb_cont == ret_obb_cont);
   }


   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("PfNodeAdapterTest");
      test_suite->addTest( new TestCaller<PfNodeAdapterTest>("testNullSlotDefault", &PfNodeAdapterTest::testNullSlotDefault));
      test_suite->addTest( new TestCaller<PfNodeAdapterTest>("testNodeAdapterInit", &PfNodeAdapterTest::testNodeAdapterInit));
      test_suite->addTest( new TestCaller<PfNodeAdapterTest>("testNodeAdapterContainer", &PfNodeAdapterTest::testNodeAdapterContainer));
      return test_suite;
   }

   static Test* interactiveSuite()
   {
      TestSuite* test_suite = new TestSuite ("InteractivePfTriangleListAdapterTest");
      //test_suite->addTest( new TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }
};

};
