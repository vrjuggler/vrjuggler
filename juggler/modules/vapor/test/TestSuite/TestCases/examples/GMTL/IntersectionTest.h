#ifndef _INTERSECTION_TEST_H_
#define _INTERSECTION_TEST_H_

#include <gfxConfig.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <GMTL/Vec3.h>
#include <GMTL/Point3.h>
#include <GMTL/OOBox.h>
#include <GMTL/Intersection.h>
#include <GMTL/gmtlOutput.h>


namespace gmtlTest
{
   using namespace gmtl;

class IntersectionTest : public TestCase
{
public:
   IntersectionTest( std::string name )
   : TestCase (name)
   {;}

   virtual ~IntersectionTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {;}

   void testStaticObbObbIntersection()
   {
      OOBox box0, box1;

      // Axis A0
      box0.axis(0) = Vec3(1,0,0);  box0.axis(1) = Vec3(0,1,0); box0.axis(2) = Vec3(0,0,1);
      box0.center().set(0,0,0);
      box0.halfLen(0) = box0.halfLen(1) = box0.halfLen(2) = 1.0f;
      box1.axis(0) = Vec3(1,0,0);  box1.axis(1) = Vec3(0,1,0); box1.axis(2) = Vec3(0,0,1);
      box1.center().set(3,0,0);
      box1.halfLen(0) = box1.halfLen(1) = box1.halfLen(2) = 1.0f;
      assertTest(TestIntersect(box0,box1) == false);
      assertTest(TestIntersect(1.0f,box0,Vec3(0,0,0),box1,Vec3(0,0,0)) == false);

      // Axis A1
      box1.center().set(0,3,0);
      assertTest(TestIntersect(box0,box1) == false);
      assertTest(TestIntersect(1.0f,box0,Vec3(0,0,0),box1,Vec3(0,0,0)) == false);

      // Axis A2
      box1.center().set(0,0,3);
      assertTest(TestIntersect(box0,box1) == false);
      assertTest(TestIntersect(1.0f,box0,Vec3(0,0,0),box1,Vec3(0,0,0)) == false);

      // Axis B0
      // Axis B1
      // Axis B2


      // --- Intersections --- //
      // intersection on X Axis
      box0.axis(0) = Vec3(1,0,0);  box0.axis(1) = Vec3(0,1,0); box0.axis(2) = Vec3(0,0,1);
      box0.center().set(0,0,0);
      box0.halfLen(0) = box0.halfLen(1) = box0.halfLen(2) = 1.0f;
      box1.axis(0) = Vec3(1,0,0);  box1.axis(1) = Vec3(0,1,0); box1.axis(2) = Vec3(0,0,1);
      box1.center().set(1,0,0);
      box1.halfLen(0) = box1.halfLen(1) = box1.halfLen(2) = 1.0f;
      assertTest(TestIntersect(box0,box1) == true);
      assertTest(TestIntersect(1.0f,box0,Vec3(0,0,0),box1,Vec3(0,0,0)) == true);


      // ---- Other tests --- //
      // From graph test prob
      // ( cntr: [0, -9.31322e-09, 7.45058e-09]
      // ax: [0.816496, 0.408249, -0.408248] xh:0.942809
      // ay: [0.577351, -0.57735, 0.577349] yh:1
      // az: [-1.49011e-08, -0.707106, -0.707107] zh:1
      box0.center().set(0, -9.31322e-09, 7.45058e-09);
      box0.axis(0).set(0.816496, 0.408249, -0.408248);      box0.halfLen(0) = 0.942809;
      box0.axis(1).set(0.577351, -0.57735, 0.577349);       box0.halfLen(1) = 1.0;
      box0.axis(2).set(-1.49011e-08, -0.707106, -0.707107); box0.halfLen(2) = 1.0;

      // ( cntr: [1.87644e-08, 1.39422e-08, 3.96674e-10]
      // ax: [0, -4.65661e-10, -1] xh:0.25
      // ay: [1, 0, 0] yh:2.25
      // az: [0, -1, 4.65661e-10] zh:2.25
      box1.center().set(1.87644e-08, 1.39422e-08, 3.96674e-10);
      box1.axis(0).set(0, -4.65661e-10, -1);    box1.halfLen(0) = 0.25;
      box1.axis(1).set(1, 0, 0);                box1.halfLen(1) = 2.25;
      box1.axis(2).set(0, -1, 4.65661e-10);     box1.halfLen(2) = 2.25;
      assertTest(TestIntersect(box0,box1) == true);

   }

   void testDynamicObbObbIntersection()
   {
      OOBox box0, box1;

      // Axis A0
      box0.axis(0) = Vec3(1,0,0);  box0.axis(1) = Vec3(0,1,0); box0.axis(2) = Vec3(0,0,1);
      box0.center().set(0,0,0);
      box0.halfLen(0) = box0.halfLen(1) = box0.halfLen(2) = 1.0f;
      box1.axis(0) = Vec3(1,0,0);  box1.axis(1) = Vec3(0,1,0); box1.axis(2) = Vec3(0,0,1);
      box1.center().set(3,0,0);
      box1.halfLen(0) = box1.halfLen(1) = box1.halfLen(2) = 1.0f;
      assertTest(TestIntersect(0.5f,box0,Vec3(1,0,0),box1,Vec3(0,0,0)) == false);
      assertTest(TestIntersect(0.5f,box0,Vec3(0,0,0),box1,Vec3(-1,0,0)) == false);
      assertTest(TestIntersect(1.0f,box0,Vec3(1,0,0),box1,Vec3(0,0,0)) == true);
      assertTest(TestIntersect(1.0f,box0,Vec3(0,0,0),box1,Vec3(-1,0,0)) == true);
      assertTest(TestIntersect(2.0f,box0,Vec3(1,0,0),box1,Vec3(0,0,0)) == true);
      assertTest(TestIntersect(2.0f,box0,Vec3(0,0,0),box1,Vec3(-1,0,0)) == true);


      // Axis A1
      box1.center().set(0,3,0);
      assertTest(TestIntersect(0.5f,box0,Vec3(0,1,0),box1,Vec3(0,0,0)) == false);
      assertTest(TestIntersect(0.5f,box0,Vec3(0,0,0),box1,Vec3(0,-1,0)) == false);
      assertTest(TestIntersect(1.0f,box0,Vec3(0,1,0),box1,Vec3(0,0,0)) == true);
      assertTest(TestIntersect(1.0f,box0,Vec3(0,0,0),box1,Vec3(0,-1,0)) == true);
      assertTest(TestIntersect(2.0f,box0,Vec3(0,1,0),box1,Vec3(0,0,0)) == true);
      assertTest(TestIntersect(2.0f,box0,Vec3(0,0,0),box1,Vec3(0,-1,0)) == true);

      // Axis A2
      box1.center().set(0,0,3);
      assertTest(TestIntersect(0.5f,box0,Vec3(0,0,1),box1,Vec3(0,0,0)) == false);
      assertTest(TestIntersect(0.5f,box0,Vec3(0,0,0),box1,Vec3(0,0,-1)) == false);
      assertTest(TestIntersect(1.0f,box0,Vec3(0,0,1),box1,Vec3(0,0,0)) == true);
      assertTest(TestIntersect(1.0f,box0,Vec3(0,0,0),box1,Vec3(0,0,-1)) == true);
      assertTest(TestIntersect(2.0f,box0,Vec3(0,0,1),box1,Vec3(0,0,0)) == true);
      assertTest(TestIntersect(2.0f,box0,Vec3(0,0,0),box1,Vec3(0,0,-1)) == true);

      // Axis B0
      // Axis B1
      // Axis B2


      // --- X crossing --- //
      // intersection on X,Y plane
      box0.axis(0) = Vec3(1,0,0);  box0.axis(1) = Vec3(0,1,0); box0.axis(2) = Vec3(0,0,1);
      box0.center().set(-2,-2,0);
      box0.halfLen(0) = box0.halfLen(1) = box0.halfLen(2) = 0.5f;
      box1.axis(0) = Vec3(1,0,0);  box1.axis(1) = Vec3(0,1,0); box1.axis(2) = Vec3(0,0,1);
      box1.center().set(2,2,0);
      box1.halfLen(0) = box1.halfLen(1) = box1.halfLen(2) = 0.5f;
      Vec3 urVec(1,1,0);      // Vector moving up and right
      assertTest(TestIntersect(0.5f,box0,urVec,box1,-urVec) == false);
      assertTest(TestIntersect(1.0f,box0,urVec,box1,-urVec) == false);
      assertTest(TestIntersect(1.5f,box0,urVec,box1,-urVec) == true);
      assertTest(TestIntersect(2.0f,box0,urVec,box1,-urVec) == true);
      assertTest(TestIntersect(2.5f,box0,urVec,box1,-urVec) == true);
   }


   void testFirstContactObbObbIntersection()
   {
      OOBox box0, box1;

      // Axis A0
      box0.axis(0).set(1,0,0);  box0.axis(1).set(0,1,0); box0.axis(2).set(0,0,1);
      box0.center().set(0,0,0);
      box0.halfLen(0) = box0.halfLen(1) = box0.halfLen(2) = 1.0f;

      box1.axis(0).set(1,0,0);  box1.axis(1).set(0,1,0); box1.axis(2).set(0,0,1);
      box1.center().set(3,0,0);
      box1.halfLen(0) = box1.halfLen(1) = box1.halfLen(2) = 1.0f;

      float contactTime;

      // --- Axis A0 --- //
      // Move towards, but don't hit
      assertTest(TestIntersect(0.5f,box0,Vec3(1,0,0),box1,Vec3(0,0,0),contactTime) == false);
      assertTest(TestIntersect(0.5f,box0,Vec3(0,0,0),box1,Vec3(-1,0,0),contactTime) == false);
      assertTest(TestIntersect(0.25f,box0,Vec3(1,0,0),box1,Vec3(-1,0,0),contactTime) == false);
      // Move towards, and hit at end of time
      assertTest(TestIntersect(1.0f,box0,Vec3(1,0,0),box1,Vec3(0,0,0),contactTime) == true);
      assertDoublesEqual(1.0f,contactTime,0.001);
      assertTest(TestIntersect(1.0f,box0,Vec3(0,0,0),box1,Vec3(-1,0,0),contactTime) == true);
      assertDoublesEqual(1.0f,contactTime,0.001);
      assertTest(TestIntersect(0.5f,box0,Vec3(1,0,0),box1,Vec3(-1,0,0),contactTime) == true);
      assertDoublesEqual(0.5f,contactTime,0.001);
      // Move towards, and hit during
      assertTest(TestIntersect(1.0f,box0,Vec3(2,0,0),box1,Vec3(0,0,0),contactTime) == true);
      assertDoublesEqual(0.5f,contactTime,0.001);
      assertTest(TestIntersect(2.0f,box0,Vec3(0,0,0),box1,Vec3(-2,0,0),contactTime) == true);
      assertDoublesEqual(0.5f,contactTime,0.001);
      assertTest(TestIntersect(10.0f,box0,Vec3(0.25,0,0),box1,Vec3(-0.5,0,0),contactTime) == true);
      assertDoublesEqual(1.333333f,contactTime,0.001);

      /*
      // Axis A1
      box1.center().set(0,3,0);
      assertTest(TestIntersect(0.5f,box0,Vec3(0,1,0),box1,Vec3(0,0,0)) == false);
      assertTest(TestIntersect(0.5f,box0,Vec3(0,0,0),box1,Vec3(0,-1,0)) == false);
      assertTest(TestIntersect(1.0f,box0,Vec3(0,1,0),box1,Vec3(0,0,0)) == true);
      assertTest(TestIntersect(1.0f,box0,Vec3(0,0,0),box1,Vec3(0,-1,0)) == true);
      assertTest(TestIntersect(2.0f,box0,Vec3(0,1,0),box1,Vec3(0,0,0)) == true);
      assertTest(TestIntersect(2.0f,box0,Vec3(0,0,0),box1,Vec3(0,-1,0)) == true);

      // Axis A2
      box1.center().set(0,0,3);
      assertTest(TestIntersect(0.5f,box0,Vec3(0,0,1),box1,Vec3(0,0,0)) == false);
      assertTest(TestIntersect(0.5f,box0,Vec3(0,0,0),box1,Vec3(0,0,-1)) == false);
      assertTest(TestIntersect(1.0f,box0,Vec3(0,0,1),box1,Vec3(0,0,0)) == true);
      assertTest(TestIntersect(1.0f,box0,Vec3(0,0,0),box1,Vec3(0,0,-1)) == true);
      assertTest(TestIntersect(2.0f,box0,Vec3(0,0,1),box1,Vec3(0,0,0)) == true);
      assertTest(TestIntersect(2.0f,box0,Vec3(0,0,0),box1,Vec3(0,0,-1)) == true);

      // Axis B0
      // Axis B1
      // Axis B2


      // --- X crossing --- //
      // intersection on X,Y plane
      box0.axis(0) = Vec3(1,0,0);  box0.axis(1) = Vec3(0,1,0); box0.axis(2) = Vec3(0,0,1);
      box0.center().set(-2,-2,0);
      box0.halfLen(0) = box0.halfLen(1) = box0.halfLen(2) = 0.5f;
      box1.axis(0) = Vec3(1,0,0);  box1.axis(1) = Vec3(0,1,0); box1.axis(2) = Vec3(0,0,1);
      box1.center().set(2,2,0);
      box1.halfLen(0) = box1.halfLen(1) = box1.halfLen(2) = 0.5f;
      Vec3 urVec(1,1,0);      // Vector moving up and right
      assertTest(TestIntersect(0.5f,box0,urVec,box1,-urVec) == false);
      assertTest(TestIntersect(1.0f,box0,urVec,box1,-urVec) == false);
      assertTest(TestIntersect(1.5f,box0,urVec,box1,-urVec) == true);
      assertTest(TestIntersect(2.0f,box0,urVec,box1,-urVec) == true);
      assertTest(TestIntersect(2.5f,box0,urVec,box1,-urVec) == true);
      */
   }


   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("IntersectionTest");
      test_suite->addTest( new TestCaller<IntersectionTest>("testStaticObbObbIntersection", &IntersectionTest::testStaticObbObbIntersection));
      test_suite->addTest( new TestCaller<IntersectionTest>("testDynamicObbObbIntersection", &IntersectionTest::testDynamicObbObbIntersection));
      test_suite->addTest( new TestCaller<IntersectionTest>("testFirstContactObbObbIntersection", &IntersectionTest::testFirstContactObbObbIntersection));

      return test_suite;
   }

   static Test* interactiveSuite()
   {
      TestSuite* test_suite = new TestSuite ("IntersectionTestInteractive");
      //test_suite->addTest( new TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:

};

};

#endif
