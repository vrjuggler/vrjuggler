#include <gfxConfig.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <GMTL/Vec3.h>
#include <GMTL/matVecFuncs.h>

namespace gmtlTest
{

class Vec3Test : public TestCase
{
public:
   Vec3Test( std::string name )
   : TestCase (name)
   {;}

   virtual ~Vec3Test()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {
   }

   void testVec3Creation()
   {
      gmtl::Vec3  float_vec3;
      float_vec3.set(1,2,3);
   }

   void testVec3Equal()
   {
      gmtl::Vec3 v1(1.0,2.0,3.0);
      gmtl::Vec3 v2(1.00001, 2.00002, 3.00003);
      gmtl::Vec3 v3(1.1, 2.2, 3.3);

      assertTest(v1 != v2);
      assertTest(v1.equal(v2));
      assertTest(!v1.equal(v3,0.001));
   }

   void testVec3Cross()
   {
      gmtl::Vec3 v1(gmtl::Vec3::UNIT_X);
      gmtl::Vec3 v2(gmtl::Vec3::UNIT_Y);
      gmtl::Vec3 v3(gmtl::Vec3::UNIT_Z);
      gmtl::Vec3 cross;

      // Base Vectors
      cross = v1.cross(v2);
      assertTest(cross.equal(v3));
      cross = v2.cross(v1);
      assertTest(cross.equal(v3 * -1.0f));

      v1.set(13.45, -7.8, 0.056);
      v2.set(0.777, 5.333,12.21);
      v3.set(-95.537, -164.181, 77.789);

      cross = v1.cross(v2);
      assertTest(cross.equal(v3,0.01));
      cross = v2.cross(v1);
      assertTest(cross.equal(v3 * -1.0f,0.01));
   }


   void testVec3Dot()
   {
      gmtl::Vec3 v1(gmtl::Vec3::UNIT_X);
      gmtl::Vec3 v2(gmtl::Vec3::UNIT_Y);
      gmtl::Vec3 v3(gmtl::Vec3::UNIT_Z);
      float dot, ans;

      // Base Vectors
      assertTest(v1.dot(v2) == 0.0f);
      assertTest(v1.dot(v3) == 0.0f);
      assertTest(v2.dot(v3) == 0.0f);

      // Other Vectors
      v1.set(13.45, -7.8, 0.056);
      v2.set(0.777, 5.333,12.21);
      v3.set(3.4, -1.6, 0.23);

      ans = -30.463;
      dot = v1.dot(v2);
      assertTest(GMTL_NEAR(dot,ans,0.01));
      dot = v2.dot(v1);
      assertTest(GMTL_NEAR(dot,ans,0.01));
      ans = -3.0827;
      dot = v2.dot(v3);
      assertTest(GMTL_NEAR(dot,ans,0.01));
      dot = v3.dot(v2);
      assertTest(GMTL_NEAR(dot,ans,0.01));
   }


   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("Vec3Test");
      test_suite->addTest( new TestCaller<Vec3Test>("testVec3Create", &Vec3Test::testVec3Creation));
      test_suite->addTest( new TestCaller<Vec3Test>("testVec3Equal", &Vec3Test::testVec3Equal));
      test_suite->addTest( new TestCaller<Vec3Test>("testVec3Dot", &Vec3Test::testVec3Dot));
      test_suite->addTest( new TestCaller<Vec3Test>("testVec3Cross", &Vec3Test::testVec3Cross));
      return test_suite;
   }

   static Test* interactiveSuite()
   {
      TestSuite* test_suite = new TestSuite ("InteractiveThreadTest");
      //test_suite->addTest( new TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:

};

};
