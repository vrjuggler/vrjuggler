#include <gfxConfig.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <GMTL/Matrix.h>

namespace gmtlTest
{

class MatrixTest : public TestCase
{
public:
   MatrixTest( std::string name )
   : TestCase (name)
   {;}

   virtual ~MatrixTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {
   }

   void testMatrixCreation()
   {
      gmtl::Matrix test_mat;
      test_mat.set(0, 1, 2, 3,
                   4, 5, 6, 7,
                   8, 9, 10, 11,
                   12, 13, 14, 15);
      //std::cout << "testMatrixCreation: mat:\n" << test_mat << std::endl << std::endl;
   }

   void testMatrixTranspose()
   {
      gmtl::Matrix test_mat, res_mat;
      test_mat.set(0, 1, 2, 3,
                   4, 5, 6, 7,
                   8, 9, 10, 11,
                   12, 13, 14, 15);
      res_mat.set(0,   4,   8,  12,
                  1,   5,   9,  13,
                  2,   6,  10,  14,
                  3,   7,  11,  15);
      assertTest(res_mat == test_mat.transpose());
   }

   void testMatrixAddSub()
   {
      gmtl::Matrix test_mat1, test_mat2, res_mat, ans_mat;
      test_mat1.set(0, 1, 2, 3,
                   4, 5, 6, 7,
                   8, 9, 10, 11,
                   12, 13, 14, 15);
      test_mat2 = test_mat1;

      ans_mat.set(0, 2, 4, 6,
                   8, 10, 12, 14,
                   16, 18, 20, 22,
                   24, 26, 28, 30);
      res_mat = test_mat1 + test_mat2;    // rm = m1 + m2

      assertTest(res_mat == ans_mat);

      gmtl::Matrix diff_mat;
      diff_mat = res_mat - test_mat1;

      assertTest(diff_mat != res_mat);
      assertTest(diff_mat == test_mat2);

   }

   void testMultOp()
   {
      gmtl::Matrix mat1, mat2, mat3, res_mat;

      mat1.set(1.1000,2.2000,3.3000,4.4000,
               5.5000,6.6000,7.7000,8.8000,
               9.9000,10.1000,11.1100,12.1200,
              13.1300,14.1400,15.1500,16.1600);
      mat2.set( 43,-8,-4,7,
                23,   22,   72,   69,
               -34,  -23,   99,  -48,
                12,   16,   21,   18);
      mat3 = mat1*mat2;

      res_mat.set(38.500,    34.100,   573.100,    80.300,
                 232.100,    64.900,  1400.300,   282.700,
                 425.700,    81.390,  2042.010,   451.080,
                 568.630,   116.150,  2804.770,   631.250);

      /*
      gmtl::Matrix diff_mat = (res_mat - mat3);
      std::cout << "m1:\n" << mat1 << std::endl << "*\n" << mat2 << "\n=\n" << mat3 << std::endl;
      std::cout << "Diff mat:\n" << diff_mat << std::endl;
      */

      assertTest(res_mat.equal(mat3));

      mat3 = mat2*mat1;
      assertTest(!res_mat.equal(mat3));

      res_mat.set( 55.610,   100.380,   141.910,   183.440,
                 1765.070,  1898.660,  2090.570,  2282.480,
                  185.960,    94.580,    83.390,    72.200,
                  545.440,   598.620,   668.810,   739.000 );

      assertTest(res_mat.equal(mat3));

      // test matrix::op*=(mat)
      mat3 = mat2;
      mat3 *= mat1;
      assertTest(res_mat.equal(mat3));
   }

   void testMatEqualTest()
   {
      gmtl::Matrix mat1, mat2;
      mat1.set( 0.78,  1.4,  2.9,  3.45,
                4.21, 57.9, 65.9, 74.6,
               89.2,  99.2, 10.9, 11.9,
               12.5,  13.9, 14.78, 15.6);
      mat1 = mat2;
      assertTest(mat1 == mat2);

      // Test that != works on all elements
      for(int i=0;i<4;i++)
      {
         for(int j=0;j<4;j++)
         {
            mat2[i][j] = 1221.0f;
            assertTest(mat1 != mat2);
            mat2[i][j] = mat1[i][j];
         }
      }

      // Test for epsilon equals working
      assertTest(mat1.equal(mat2));
      for(int i=0;i<4;i++)
      {
         for(int j=0;j<4;j++)
         {
            mat2[i][j] = mat1[i][j]-(gmtl::GMTL_EPSILON/2.0f);
            assertTest(mat1.equal(mat2));
            assertTest(!mat1.equal(mat2,gmtl::GMTL_EPSILON/3.0f));
            mat2[i][j] = mat1[i][j];
         }
      }

   }

   void testMatInvert()
   {
      gmtl::Matrix mat1, mat1inv_ans, result, identity;
      identity.makeIdent();

      mat1.set( 0.78,  1.4,  2.9,  3.45,
                4.21, 57.9, 65.9, 74.6,
                89.2,  99.2, 10.9, 11.9,
                12.5,  13.9, 14.78, 15.6);
      mat1inv_ans.set( 0.3071733,  -0.0239700,   0.0034853,   0.0440345,
                      -0.2891106,   0.0216826,   0.0079218,  -0.0457924,
                      -3.0532152,   0.0305681,  -0.0547335,   0.5708037,
                       2.9041982,  -0.0290744,   0.0420053,  -0.4711792);
      // Make sure our pre-computed answer is right
      result = mat1* mat1inv_ans;
      assertTest(result.equal(identity));

      // Test inversion
      result.makeIdent();
      result.invert(mat1);
      assertTest(result.equal(mat1inv_ans));

      // Test rotation inversions
      gmtl::Matrix rot_mat1, rot_mat1_inv; // rot_mat2, rot_mat2_inv;
      rot_mat1.makeXYZEuler(30.0f, 45.0f, 60.0f);
      rot_mat1_inv.invert(rot_mat1);
      result = rot_mat1*rot_mat1_inv;
      assertTest(result.equal(identity));

      // Test translation matrix inversion
      gmtl::Matrix trans_mat1, trans_mat1_inv;
      trans_mat1.makeTrans(21.0f, -23.45f, 0.045f);
      trans_mat1_inv.invert(trans_mat1);
      result = trans_mat1*trans_mat1_inv;
      assertTest(result.equal(identity));
   }

   void testGetSetAxes()
   {
      gmtl::Matrix mat1, mat2;
      gmtl::Vec3 xAxis1,yAxis1,zAxis1;
      gmtl::Vec3 xAxis2,yAxis2,zAxis2;

      // Simple rotation around X axis
      mat1.makeRot(90,gmtl::Vec3::UNIT_X);
      mat1.getAxes(xAxis1,yAxis1,zAxis1);
      mat2.makeAxes(xAxis1,yAxis1,zAxis1);
      mat2.getAxes(xAxis2,yAxis2,zAxis2);

      assertTest(xAxis1.equal(xAxis2,0.01));
      assertTest(yAxis1.equal(yAxis2,0.01));
      assertTest(zAxis1.equal(zAxis2,0.01));

      // More complex Euler rotation
      mat1.makeXYZEuler(45.0f, -35.0f, 13.0f);
      mat1.getAxes(xAxis1,yAxis1,zAxis1);
      mat2.makeAxes(xAxis1,yAxis1,zAxis1);
      mat2.getAxes(xAxis2,yAxis2,zAxis2);

      assertTest(xAxis1.equal(xAxis2,0.01));
      assertTest(yAxis1.equal(yAxis2,0.01));
      assertTest(zAxis1.equal(zAxis2,0.01));

      // Use orthonormal axis
      xAxis1.set(7, 11, 21);     xAxis1.normalize();
      yAxis1.set(12, 21, 75);    yAxis1.normalize();
      zAxis1 = xAxis1.cross(yAxis1);   zAxis1.normalize();

      mat2.makeAxes(xAxis1,yAxis1,zAxis1);
      mat2.getAxes(xAxis2,yAxis2,zAxis2);

      assertTest(xAxis1.equal(xAxis2,0.01));
      assertTest(yAxis1.equal(yAxis2,0.01));
      assertTest(zAxis1.equal(zAxis2,0.01));
   }


   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("MatrixTest");
      test_suite->addTest( new TestCaller<MatrixTest>("testMatrixCreate", &MatrixTest::testMatrixCreation));
      test_suite->addTest( new TestCaller<MatrixTest>("testMatEqualTest", &MatrixTest::testMatEqualTest));
      test_suite->addTest( new TestCaller<MatrixTest>("testMatrixTranspose", &MatrixTest::testMatrixTranspose));
      test_suite->addTest( new TestCaller<MatrixTest>("testMultOp", &MatrixTest::testMultOp));
      test_suite->addTest( new TestCaller<MatrixTest>("testMatrixAddSub", &MatrixTest::testMatrixAddSub));
      test_suite->addTest( new TestCaller<MatrixTest>("testMatInvert", &MatrixTest::testMatInvert));
      test_suite->addTest( new TestCaller<MatrixTest>("testGetSetAxes", &MatrixTest::testGetSetAxes));
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
