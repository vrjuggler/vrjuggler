#include <gfxConfig.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <GMTL/Point3.h>

namespace gmtlTest
{

class Point3Test : public TestCase
{
public:
   Point3Test( std::string name )
   : TestCase (name)
   {;}

   virtual ~Point3Test()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {;}

   void testPoint3Creation()
   {
      gmtl::Point3  test_point;
      test_point.set(1,2,3);

      gmtl::Point3 new_point(test_point);    // Test copy constructor
      gmtl::Point3 another_pt;
      another_pt = test_point;               // Test it again
      another_pt = new_point;

      gmtl::Vec3 vec_pt(3,4,5);
      gmtl::Point3 p1(vec_pt);
      gmtl::Point3 p2;
      p2 = vec_pt;
      p1 = p2;
      assertTest(p1 == vec_pt);           // Should be able to construct one

   }



   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("Point3Test");
      test_suite->addTest( new TestCaller<Point3Test>("testPoint3Creation", &Point3Test::testPoint3Creation));
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
