#include <gfxConfig.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <GMTL/Vec.h>

namespace gmtlTest
{

class VecTest : public TestCase
{
public:
   VecTest( std::string name )
   : TestCase (name)
   {;}

   virtual ~VecTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {
   }

   void testVecCreation()
   {
      gmtl::Vec<3,gmtl::Float>  float_vec3;
      gmtl::Vec<3,gmtl::Double> double_vec3;
      gmtl::Vec<4,gmtl::Float>  float_vec4;
      gmtl::Vec<4,gmtl::Double> double_vec4;
   }



   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("VecTest");
      test_suite->addTest( new TestCaller<VecTest>("testVecCreate", &VecTest::testVecCreation));
      return test_suite;
   }

   static Test* interactiveSuite()
   {
      /*
      TestSuite* test_suite = new TestSuite ("InteractiveThreadTest");
      test_suite->addTest( new TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
      */
   }

protected:

};

};
