#include <gfxConfig.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <GMTL/Quat.h>
#include <GMTL/Matrix.h>

namespace gmtlTest
{

class QuatTest : public TestCase
{
public:
   QuatTest( std::string name )
   : TestCase (name)
   {;}

   virtual ~QuatTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {
   }

   void testQuatTestCreation()
   {
      gmtl::Quat test_quat;
      test_quat.makeAxisAngle(50,Vec3(0,0,1));
   }

   void testGetSetAxes()
   {
      gmtl::Matrix mat;
      gmtl::Vec3 xAxis1,yAxis1,zAxis1;
      gmtl::Vec3 xAxis2,yAxis2,zAxis2;
      gmtl::Quat quat0;

      // Simple rotation around X axis
      mat.makeRot(90,gmtl::Vec3::UNIT_X);
      mat.getAxes(xAxis1,yAxis1,zAxis1);
      quat0.makeAxes(xAxis1,yAxis1,zAxis1);
      quat0.getAxes(xAxis2,yAxis2,zAxis2);
      assertTest(xAxis1.equal(xAxis2,0.01));
      assertTest(yAxis1.equal(yAxis2,0.01));
      assertTest(zAxis1.equal(zAxis2,0.01));

      // More complex - Just get some orthonormal axis
      ///*
      xAxis1.set(7, 11, 21);     xAxis1.normalize();
      yAxis1.set(12, 21, 75);    yAxis1.normalize();
      zAxis1 = xAxis1.cross(yAxis1);   zAxis1.normalize();
      //*/
      /*
      mat.makeXYZEuler(45.0f, -35.0f, 13.0f);
      mat.getAxes(xAxis1,yAxis1,zAxis1);
      */

      // Test with Matrix
      /*
      mat.makeAxes(xAxis1,yAxis1,zAxis1);
      mat.getAxes(xAxis2,yAxis2,zAxis2);

      assertTest(xAxis1.equal(xAxis2,0.01));
      assertTest(yAxis1.equal(yAxis2,0.01));
      assertTest(zAxis1.equal(zAxis2,0.01));

      // Test with quat
      quat0.makeAxes(xAxis1,yAxis1,zAxis1);
      quat0.getAxes(xAxis2,yAxis2,zAxis2);

      assertTest(xAxis1.equal(xAxis2,0.01));
      assertTest(yAxis1.equal(yAxis2,0.01));
      assertTest(zAxis1.equal(zAxis2,0.01));
      */

   }

   void testGetSetMat()
   {
      gmtl::Vec3 xAxis1,yAxis1,zAxis1;
      gmtl::Matrix mat1, mat2;
      gmtl::Quat quat0, quat1;

      // Simple rotation around XAxis
      mat1.makeIdent();
      quat0.makeQuat(mat1);
      quat0.getMat(mat2);
      assertTest(mat1.equal(mat2));

      // Simple rotation around XAxis
      mat1.makeXYZEuler(45, -89, 32.45);
      quat0.makeQuat(mat1);
      quat0.getMat(mat2);
      assertTest(mat1.equal(mat2));

      // Make orthonormal matrix
      xAxis1.set(7, 11, 21);
      yAxis1.set(12, 21, 75);
      zAxis1 = xAxis1.cross(yAxis1);
      xAxis1.normalize();
      yAxis1.normalize();
      zAxis1.normalize();

      mat1.makeAxes(xAxis1,yAxis1,zAxis1);
      quat0.makeQuat(mat1);
      quat0.getMat(mat2);
      quat1.makeQuat(mat2);

      assertTest(quat0.equal(quat1) || quat0.equal(-quat1));
   }


   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("QuatTest");
      test_suite->addTest( new TestCaller<QuatTest>("testMatrixCreate", &QuatTest::testQuatTestCreation));
      test_suite->addTest( new TestCaller<QuatTest>("testGetSetAxes", &QuatTest::testGetSetAxes));
      test_suite->addTest( new TestCaller<QuatTest>("testGetSetMat", &QuatTest::testGetSetMat));

      return test_suite;
   }

   static Test* interactiveSuite()
   {
      TestSuite* test_suite = new TestSuite ("InteractiveQuatTest");
      //test_suite->addTest( new TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:

};

};
