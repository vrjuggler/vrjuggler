#include <gfxConfig.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <GMTL/Tri.h>

namespace gmtlTest
{
   using namespace gmtl;

class TriTest : public TestCase
{
public:
   TriTest( std::string name )
   : TestCase (name)
   {;}

   virtual ~TriTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {
   }

   void testTriCreation()
   {
      gmtl::Tri  test_tri(Vec3::ZERO, Vec3::UNIT_X, Vec3::UNIT_Y);
      assertTest(test_tri[0] == Vec3::ZERO);
      assertTest(test_tri[1] == Vec3::UNIT_X);
      assertTest(test_tri[2] == Vec3::UNIT_Y);
   }

   void testEdges()
   {
      gmtl::Tri  test_tri(Vec3::ZERO, Vec3::UNIT_X, Vec3::UNIT_Y);
      Vec3 edge0 = Vec3::ZERO - Vec3::UNIT_X;
      Vec3 edge1 = Vec3::UNIT_Y - Vec3::UNIT_X;

      assertTest(test_tri.edge0() == edge0);
      assertTest(test_tri.edge1() == edge1);
   }

   void testCenter()
   {
      gmtl::Tri  test_tri(Vec3::ZERO, Vec3::UNIT_X, Vec3::UNIT_Y);

      Point3 center(0.333, 0.333, 0.0f);
      assertTest(center.equal(test_tri.center(), 0.01));
   }


   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("TriTest");
      test_suite->addTest( new TestCaller<TriTest>("testTriCreation", &TriTest::testTriCreation));
      test_suite->addTest( new TestCaller<TriTest>("testEdges", &TriTest::testEdges));
      test_suite->addTest( new TestCaller<TriTest>("testCenter", &TriTest::testCenter));
      return test_suite;
   }

   static Test* interactiveSuite()
   {
      TestSuite* test_suite = new TestSuite ("InteractiveTriTest");
      //test_suite->addTest( new TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:

};

};
