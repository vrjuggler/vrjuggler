#include <gfxConfig.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <GMTL/Plane.h>

namespace gmtlTest
{

class PlaneTest : public TestCase
{
public:
   PlaneTest( std::string name )
   : TestCase (name),
      origin(gmtl::Vec3::ZERO),
      x1_pt(gmtl::Vec3::UNIT_X),
      y1_pt(gmtl::Vec3::UNIT_Y),
      z1_pt(gmtl::Vec3::UNIT_Z)
   {;}

   virtual ~PlaneTest()
   {}

   virtual void setUp()
   {
      xy_plane = gmtl::Plane(origin,x1_pt,y1_pt);
      zx_plane = gmtl::Plane(origin,z1_pt,x1_pt);
      yz_plane = gmtl::Plane(origin,y1_pt,z1_pt);
   }

   virtual void tearDown()
   {;}

   // ------------------------
   // CREATION TESTS
   // ------------------------
   void testThreePtCreation()
   {
     // Check normals of the standard planes
     assertTest(xy_plane.mNorm == gmtl::Vec3::UNIT_Z);   // Z-Plane
     assertTest(zx_plane.mNorm == gmtl::Vec3::UNIT_Y);   // Y-Plane
     assertTest(yz_plane.mNorm == gmtl::Vec3::UNIT_X);   // Z-Plane
     assertTest(xy_plane.mOffset == 0.0f);
     assertTest(zx_plane.mOffset == 0.0f);
     assertTest(yz_plane.mOffset == 0.0f);

     // Test offset of some simple planes
     gmtl::Plane test_plane;
     test_plane = gmtl::Plane(gmtl::Point3(1.0,0.0,0.0), gmtl::Point3(1.0,1.0,0.0), gmtl::Point3(1.0,0.0,1.0));
     assertTest(test_plane.mNorm == gmtl::Vec3::UNIT_X);
     assertTest(test_plane.mOffset == 1.0f);
   }

   void testNormPtCreation()
   {
      gmtl::Plane test_plane;
      test_plane = gmtl::Plane(gmtl::Vec3::UNIT_X, origin);    // X-axis through origin
      assertTest(test_plane.mNorm == gmtl::Vec3::UNIT_X);
      assertTest(test_plane.mOffset == 0.0f);

      test_plane = gmtl::Plane(gmtl::Vec3::UNIT_X, x1_pt);    // X-axis through 1,0,0
      assertTest(test_plane.mNorm == gmtl::Vec3::UNIT_X);
      assertTest(test_plane.mOffset == 1.0f);

      test_plane = gmtl::Plane(gmtl::Vec3::UNIT_Z, x1_pt);    // Z-axis through 1,0,0
      assertTest(test_plane.mNorm == gmtl::Vec3::UNIT_Z);
      assertTest(test_plane.mOffset == 0.0f);

      test_plane = gmtl::Plane(gmtl::Vec3::UNIT_Z, gmtl::Point3(0,0,-1));    // Z-axis through 0,0,-1
      assertTest(test_plane.mNorm == gmtl::Vec3::UNIT_Z);
      assertTest(test_plane.mOffset == -1.0f);
   }

   void testNormOffsetCreation()
   {
      gmtl::Plane test_plane;
      test_plane = gmtl::Plane(gmtl::Vec3::UNIT_X, 0.0f);    // X-axis through origin
      assertTest(test_plane.mNorm == gmtl::Vec3::UNIT_X);
      assertTest(test_plane.mOffset == 0.0f);
   }

   // --------------------------
   // Dist and side tests
   // --------------------------
   void testDistToPt()
   {
      float dist;

      dist = xy_plane.distanceToPt(gmtl::Vec3::UNIT_Z);
      assertTest(dist == 1.0f);

      dist = xy_plane.distanceToPt(gmtl::Point3(-12.0,5.0,-17.0f));
      assertTest(dist == -17.0f);

      gmtl::Plane slanted_plane(gmtl::Vec3(1.0f,1.0f,1.0f), origin);
      dist = slanted_plane.distanceToPt(origin);
      assertTest(dist == 0.0f);

      dist = slanted_plane.distanceToPt(gmtl::Vec3(1.0f,1.0f,1.0f));
      assertTest(dist > 0.0f);
   }

   void testWhichSide()
   {
      gmtl::Plane::Side answer;

      answer = xy_plane.whichSide(gmtl::Point3(0.0,0.0,1.0));
      assertTest(answer == gmtl::Plane::POS_SIDE);

      answer = xy_plane.whichSide(gmtl::Point3(0.0,0.0,-12.0f));
      assertTest(answer == gmtl::Plane::NEG_SIDE);

      answer = zx_plane.whichSide(gmtl::Point3(0.0,1e-10f,0.0f));
      assertTest(answer == gmtl::Plane::POS_SIDE);

      answer = zx_plane.whichSide(gmtl::Point3(0.0,-1e-10f,0.0f));
      assertTest(answer == gmtl::Plane::NEG_SIDE);

      answer = xy_plane.whichSide(gmtl::Point3(0.0,0.0,0.0));
      assertTest(answer == gmtl::Plane::ON_PLANE);
   }

   void testFindNearestPt()
   {
      gmtl::Point3 answer, test_point;
      //float dist;

      // XY dist to point off origin
      test_point = gmtl::Point3(0.0,0.0,1.0);
      assertTest(xy_plane.distanceToPt(test_point) == xy_plane.findNearestPt(test_point, answer));
      assertTest(answer == gmtl::Point3(0.0,0.0,0.0));

      // XY dist to point at 12,21
      test_point = gmtl::Point3(12.0,-21.0,-13.0);
      assertTest(xy_plane.distanceToPt(test_point) == xy_plane.findNearestPt(test_point, answer));
      assertTest(answer == gmtl::Point3(12.0,-21.0,0.0));

      // XY dist to point on plane at -17.05, 0.334
      test_point = gmtl::Point3(-17.05,0.334,0.0);
      assertTest(xy_plane.distanceToPt(test_point) == xy_plane.findNearestPt(test_point, answer));
      assertTest(answer == test_point);
   }


   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("PlaneTest");
      test_suite->addTest( new TestCaller<PlaneTest>("testThreePtCreation", &PlaneTest::testThreePtCreation));
      test_suite->addTest( new TestCaller<PlaneTest>("testNormPtCreation", &PlaneTest::testNormPtCreation));
      test_suite->addTest( new TestCaller<PlaneTest>("testNormOffsetCreation", &PlaneTest::testNormOffsetCreation));
      test_suite->addTest( new TestCaller<PlaneTest>("testDistToPt", &PlaneTest::testDistToPt));
      test_suite->addTest( new TestCaller<PlaneTest>("testWhichSide", &PlaneTest::testWhichSide));
      test_suite->addTest( new TestCaller<PlaneTest>("testFindNearestPt", &PlaneTest::testFindNearestPt));

      return test_suite;
   }

   static Test* interactiveSuite()
   {
      TestSuite* test_suite = new TestSuite ("PlaneTestInteractive");
      //test_suite->addTest( new TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:
   gmtl::Vec3 origin;
   gmtl::Vec3 x1_pt;
   gmtl::Vec3 y1_pt;
   gmtl::Vec3 z1_pt;

   gmtl::Plane xy_plane;
   gmtl::Plane zx_plane;
   gmtl::Plane yz_plane;
};

};
