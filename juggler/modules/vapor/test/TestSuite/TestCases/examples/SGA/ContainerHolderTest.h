#include <gfxConfig.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <SGA/ContainerBase.h>
#include <SGA/ContainerHolder.h>

// Containers to test with it
#include <SGA/ObbNodeContainer.h>

namespace sgaTest
{

class ContainerHolderTest : public TestCase
{
public:
   ContainerHolderTest( std::string name )
   : TestCase (name)
   {;}

   virtual ~ContainerHolderTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {
   }

   void testHolderCreation()
   {
      sga::ContainerHolder holder;
      sga::ObbNodeContainer* obb_cont = new sga::ObbNodeContainer;
      sga::ObbNodeContainer* ret_obb_cont;

      ret_obb_cont = holder.getContainer<sga::ObbNodeContainer>();
      assertTest(ret_obb_cont == NULL);

      holder.setContainer(obb_cont);
      ret_obb_cont = holder.getContainer<sga::ObbNodeContainer>();
      assertTest(obb_cont == ret_obb_cont);

      holder.setContainer<sga::ObbNodeContainer>(obb_cont);
      ret_obb_cont = holder.getContainer<sga::ObbNodeContainer>();
      assertTest(obb_cont == ret_obb_cont);
   }

   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("ContainerHolderTest");
      test_suite->addTest( new TestCaller<ContainerHolderTest>("testHolderCreation", &ContainerHolderTest::testHolderCreation));
      return test_suite;
   }

   static Test* interactiveSuite()
   {
      TestSuite* test_suite = new TestSuite ("InteractiveContainerHolderTest");
      //test_suite->addTest( new TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:

};

};
