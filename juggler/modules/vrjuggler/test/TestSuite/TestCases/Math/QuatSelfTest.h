#ifndef QUATERNION_CLASS_SELF_TEST_H
#define QUATERNION_CLASS_SELF_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vrj/Math/Math.h>
#include <vrj/Math/Quat.h>

#include <JugglerTest.h>

/*****************************************************************
 tests out the functionality expected of vrj::Quat
*******************************************************************/

namespace vrjTest
{

class QuatSelfTest : public CppUnit::TestCase, public JugglerTest
{
public:
   QuatSelfTest() : CppUnit::TestCase ()
   {
   }
   
   
   virtual ~QuatSelfTest()
   {
   }

   void identTest()
   {
      // the default identity is currently the multiplicative identity [1,0,0,0]
      // not the addition identity... [0,0,0,0]
      // consider allowing both kinds of identity to be made in the quat API...
      vrj::Quat quat;
      quat.makeIdent();
      CPPUNIT_ASSERT( quat == vrj::Quat::identity() );
      CPPUNIT_ASSERT( quat[VJ_W] == 1.0f && quat[VJ_X] == 0.0f && 
                      quat[VJ_Y] == 0.0f && quat[VJ_Z] == 0.0f );
   }
      
   void makeRotTest()
   {
      //std::cout<<"make sure that normalize doesn't change the rotation...\n"<<std::flush;
      //std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;

      vrj::Quat q1, q2, q3, q4;
      q1.makeRot( vrj::Math::deg2rad( 45.0f ), 0,1,0 );
      q2.makeRot( vrj::Math::deg2rad( 90.0f ), 1,0,0 );
      q3 = q1; 
      q4 = q2;
      q3.normalize();
      q4.normalize();

      /*
      std::cout<<"before: \n"<<std::flush;
      std::cout<<"q1:           "<<q1<<" .. "<<q1[VJ_W]<<" "<<q1[VJ_X]<<" "<<q1[VJ_Y]<<" "<<q1[VJ_Z]<<"\n"<<std::flush;
      std::cout<<"q2:           "<<q2<<" .. "<<q2[VJ_W]<<" "<<q2[VJ_X]<<" "<<q2[VJ_Y]<<" "<<q2[VJ_Z]<<"\n"<<std::flush;
      std::cout<<"after: \n"<<std::flush;
      std::cout<<"q1:           "<<q1<<" .. "<<q1[VJ_W]<<" "<<q1[VJ_X]<<" "<<q1[VJ_Y]<<" "<<q1[VJ_Z]<<"\n"<<std::flush;
      std::cout<<"q2:           "<<q2<<" .. "<<q2[VJ_W]<<" "<<q2[VJ_X]<<" "<<q2[VJ_Y]<<" "<<q2[VJ_Z]<<"\n"<<std::flush;
      std::cout<<"\n"<<std::flush;
      */
   
            // make sure that normalize doesn't change the rotation...         
      CPPUNIT_ASSERT( q1.isEqual( q3, VJ_QUAT_EPSILON ) );
      CPPUNIT_ASSERT( q2.isEqual( q4, VJ_QUAT_EPSILON ) );
   }

   void xformVecSweepTest()
   {
      //std::cout<<"xform vector by quat (sweep over range of rotations)\n"<<std::flush;
      //std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;
      vrj::Quat q;
      vrj::Vec3 v(0,0,1);
      q.makeIdent();

      //std::cout<<"Should go from 0,0,1 to 0,-1,0 to 0,0,-1 ....\n"<<std::flush;
      for (float x = 0; x <= 180.0f; x+=90)
      {
         q.makeRot( vrj::Math::deg2rad( x ), 1,0,0 );

         vrj::Vec3 result = q * v;
         if (x == 0)
            CPPUNIT_ASSERT( result[0] < 0.001 && result[1] < 0.001 && result[2] > 0.999 );
         if (x == 90)
            CPPUNIT_ASSERT( result[0] < 0.001 && result[1] < -0.999 && result[2] < 0.001 );
         if (x == 180)
            CPPUNIT_ASSERT( result[0] < 0.001 && result[1] < 0.001 && result[2] < -0.999 );
         //std::cout<<result<<"\n"<<std::flush;
      }
      //std::cout<<"\n"<<std::flush;
   }

   void makeRotGetRotSanityTest()
   {
      //std::cout<<"testing sanity of makeRot and getRot, then makeRot again...\n"<<std::flush;
      //std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;

      for (float i = -360; i <= 360; i+=20)
      {
         vrj::Quat q, q2;
         float rad, x, y, z;
         q.makeRot( vrj::Math::deg2rad(i),1,0,0 );
         //std::cout<<i<<") "<<q<<" .. "<<q[VJ_W]<<" "<<q[VJ_X]<<" "<<q[VJ_Y]<<" "<<q[VJ_Z]<<" compare to: "<<std::flush;

         q.getRot( rad, x, y, z );
         q2.makeRot( rad, x, y, z );
         //std::cout<<q2[VJ_W]<<" "<<q2[VJ_X]<<" "<<q2[VJ_Y]<<" "<<q2[VJ_Z]<<"\n"<<std::flush;

         // make i a positive by x*360, store in b.  needed so I can use the % operator with neg values..
         CPPUNIT_ASSERT( q.isEqual( q2, VJ_QUAT_EPSILON ) );

         float b = i;
         while ( b < 0.0f)
         {
            b += 360.0f;
         }      

         if (i >= 0.0f)
         {
            float c = vrj::Math::abs( (float)((int)(vrj::Math::round(b))%360) );
            float temp1 = vrj::Math::rad2deg(rad); temp1 = (float)((int)(vrj::Math::round(temp1))%360);
            float temp2 = c + 0.5;
            float temp3 = vrj::Math::rad2deg(rad); temp3 = (float)((int)(vrj::Math::round(temp3))%360);
            float temp4 = c - 0.5;
            //   std::cout<<temp1<<" "<<temp2<<" "<<temp3<<" "<<temp4<<" "<<vrj::Math::rad2deg(rad)<<" "<<b<<"\n"<<std::flush;

            CPPUNIT_ASSERT( temp1 <= temp2 && temp3 >= temp4 );     
         }    
      }
      //std::cout<<"\n"<<std::flush;
   }

   void scalarMultTest()
   {
      //std::cout<<"testing scaling...\n"<<std::flush;
      //std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;

      for (float i = -360; i <= 360; i+=20)
      {
         vrj::Quat q, q2;
         float rad, x, y, z;
         q.makeRot( vrj::Math::deg2rad(i),1,0,0 );
         q2.scaleAngle( q, 0.5 );
         q2.getRot( rad, x, y, z );

         // make i a positive by x*360, store in b.  needed so I can use the % operator with neg values..
          //std::cout<<vrj::Math::rad2deg(rad)<<" "<<(0.5 * i)<<"\n"<<std::flush;
          float half_quat_angle = vrj::Math::rad2deg(rad);
          float half_original_angle = (0.5 * i);
          half_quat_angle = vrj::Math::abs( half_quat_angle );
          half_original_angle = vrj::Math::abs( half_original_angle );

          float tol = 0.1;
          CPPUNIT_ASSERT( (half_original_angle <= (half_quat_angle + tol)) &&
               (half_original_angle >= (half_quat_angle - tol)) );
      }

      //std::cout<<"\n"<<std::flush;
   }

   void simpleQuatProductTest()
   {
      //std::cout<<"Quat product: no rotation * rotation\n"<<std::flush;
      //std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;

      vrj::Quat q1, q2, q3;
      q1.makeRot( 0, 1,0,0 );
      q2.makeRot( vrj::Math::deg2rad( 90.0f ), 1,0,0 );
      //std::cout<<"[0 rotation] * [90deg about x] should be [90deg about x]\n"<<std::flush;
      //std::cout<<"q1:           "<<q1<<" .. "<<q1[VJ_W]<<" "<<q1[VJ_X]<<" "<<q1[VJ_Y]<<" "<<q1[VJ_Z]<<"\n"<<std::flush;
      //std::cout<<"q2:           "<<q2<<" .. "<<q2[VJ_W]<<" "<<q2[VJ_X]<<" "<<q2[VJ_Y]<<" "<<q2[VJ_Z]<<"\n"<<std::flush;

      // first rotate by q2, then by q1
      q3 = q1 * q2;

      //std::cout<<"q3 = q1 * q2: "<<q3<<" .. "<<q3[VJ_W]<<" "<<q3[VJ_X]<<" "<<q3[VJ_Y]<<" "<<q3[VJ_Z]<<"\n"<<std::flush;
      //std::cout<<"\n"<<std::flush;

      vrj::Vec3 v( 0,1,0 ), r;
      r = q3 * v;
      //std::cout<<r<<"\n"<<std::flush;
      //std::cout<<"\n"<<std::flush;

      CPPUNIT_ASSERT( r[2] > 0.99f );
   }

   // another quaternion product test
   void secondQuatProductTest()
   {
      //std::cout<<"Quat product: rotation * rotation\n"<<std::flush;
      //std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;
      vrj::Quat q1, q2, q3;
      q1.makeRot( vrj::Math::deg2rad( 45.0f ), 0,1,0 );
      q2.makeRot( vrj::Math::deg2rad( 90.0f ), 1,0,0 );
      //std::cout<<"[45 about Y] * [90 about X] should be [90deg about .7,0,-.7]\n"<<std::flush;
      //std::cout<<"q1:           "<<q1<<" .. "<<q1[VJ_W]<<" "<<q1[VJ_X]<<" "<<q1[VJ_Y]<<" "<<q1[VJ_Z]<<"\n"<<std::flush;
      //std::cout<<"q2:           "<<q2<<" .. "<<q2[VJ_W]<<" "<<q2[VJ_X]<<" "<<q2[VJ_Y]<<" "<<q2[VJ_Z]<<"\n"<<std::flush;

      // first rotate by q2, then by q1
      q3 = q1 * q2;
      //q3.normalize();

      //std::cout<<"q3 = q1 * q2: "<<q3<<" .. "<<q3[VJ_W]<<" "<<q3[VJ_X]<<" "<<q3[VJ_Y]<<" "<<q3[VJ_Z]<<"\n"<<std::flush;
      //std::cout<<"\n"<<std::flush;

      vrj::Vec3 v( 0,1,0 ), r;
      r = q3 * v;
      //std::cout<<r<<"\n"<<std::flush;
      //std::cout<<"\n"<<std::flush;

      CPPUNIT_ASSERT( r[0] > 0.7 && r[2] > 0.7 );
   }

   void xformVecTest()
   {
      //std::cout<<"xform vec by quat\n"<<std::flush;
      //std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;
      vrj::Quat q1, q2;//, q3;
      q1.makeRot( vrj::Math::deg2rad( 45.0f ), 0,-1,0 );
      q2.makeRot( vrj::Math::deg2rad( 45.0f ), 1,0,0 );

      vrj::Vec3 v( 0,1,0 ), r;

      // (should not move)
      r = q1 * v;
      //std::cout<<r<<"\n"<<std::flush;
      CPPUNIT_ASSERT( r[1] > 0.999 );

      // rotate forward
      r = q2 * v;
      //std::cout<<r<<"\n"<<std::flush;
      CPPUNIT_ASSERT( r[1] > 0.7 && r[2] > 0.7 );

      //std::cout<<"\n"<<std::flush;
   }


   // just trying stuff out... TODO think of a good add test...
   void specialCases()
   {
      std::cout<<"special cases\n"<<std::flush;
      std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;

      vrj::Quat q( 1, 0, -0.000313354, 0 );
      float rad, x, y, z;
      q.getRot( rad, x, y, z );

      std::cout<<rad<<" "<<x<<" "<<y<<" "<<z<<"\n"<<std::flush;

      // testing...
      double half_angle = 0.000626708 * 0.5f;
      double sin_half_angle = vrj::Math::sin( half_angle );
      double w = vrj::Math::cos( half_angle );
      std::cout<<half_angle<<" "<<sin_half_angle<<" "<<w<<"\n"<<std::flush;


      //vrj::Quat qq( 0,0,0,0 );

      std::cout<<"make sure that makeRot(180,0,1,0) doesn't yield [0,0,0,0]\n"<<std::flush;
      vrj::Quat qqq;
      qqq.makeRot( vrj::Math::deg2rad( 180.0f ), 0, 1, 0 );
      std::cout<<"qqq: "<<qqq<<" .. "<<qqq[VJ_W]<<" "<<qqq[VJ_X]<<" "<<qqq[VJ_Y]<<" "<<qqq[VJ_Z]<<"\n"<<std::flush;
      CPPUNIT_ASSERT( qqq[VJ_W] < VJ_QUAT_EPSILON && qqq[VJ_W] > -VJ_QUAT_EPSILON &&
              qqq[VJ_X] == 0.0f &&
              qqq[VJ_Y] > (1.0f - VJ_QUAT_EPSILON) && qqq[VJ_Y] < (1.0f + VJ_QUAT_EPSILON) &&
              qqq[VJ_Z] == 0.0f );

      std::cout<<"\n"<<std::flush;
   }

   // just trying stuff out... TODO think of a good way to test this ability...
   // TODO: could we use a vector (pure quat) to scale a quat in a 
   //        more robust way?  pretty sure we can... rewrite scaleAngle to use this
   //        method instead....
   void vectorScaleTest()
   {
      std::cout<<"vector scale of a rotation quaternion\n"<<std::flush;
      std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;

      std::cout<<"vector scale of a rotation quaternion yields an invalid rotation (only unit-quats define a rotation).\n"<<std::flush;

      // non normalized
      for (int x = 0; x < 360; ++x)
      {
         vrj::Quat q, q2;
         q.makeRot( vrj::Math::deg2rad( (float)x ), 0.7, 0, 0.7 );
         q2.mult( q, 0.5 );

         // no longer a valid rotation (non-unit length).  
         // should be normalized to define an actual rotation.
         CPPUNIT_ASSERT( q2.length() <= (1.0f - VJ_QUAT_EPSILON) ||
                 q2.length() >= (1.0f + VJ_QUAT_EPSILON) );

         float rad, i,j,k;
         q2.getRot( rad, i,j,k );
         std::cout<<x<<") "<<vrj::Math::rad2deg( rad )<<" "<<i<<" "<<j<<" "<<k<<"\n"<<std::flush;
      }

      std::cout<<"If normalized, then the scaled quat is equal to the original."<<std::flush;
      // normalized.  
      for (int x = 0; x < 360; ++x)
      {
         vrj::Quat q, q2;
         q.makeRot( vrj::Math::deg2rad( (float)x ), 0.7, 0, 0.7 );
         q2.mult( q, 0.5 );
         q2.normalize();

         CPPUNIT_ASSERT( q2.isEqual( q, VJ_QUAT_EPSILON ) );
      }        

      std::cout<<"\n"<<std::flush;
   }

   // just trying stuff out... TODO think of a good add test...
   void quatAdd()
   {
      std::cout<<"quat add\n"<<std::flush;
      std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;


      {
         vrj::Quat q, q1, q2;
         q1.makeRot( vrj::Math::deg2rad( 90.0f ), 1, 0, 0 );
         q2.makeRot( vrj::Math::deg2rad( 180.0f ), 1, 0, 0 );

         q.add( q1, q2 );
         q.normalize();

         std::cout<<q<<"\n"<<std::flush;
      }

      vrj::Quat q, q1;//, q2;
      q1.makeRot( vrj::Math::deg2rad( 90.0f ), 1, 0, 0 );

      q = q1 * q;

      std::cout<<q<<"\n"<<std::flush;
   }

   // using a vector to increment rotation, much like angular velocity
   // here angular velocity is defined as a vrj::Vec3 "w", using right hand rule
   // things spin at |w| speed and on the w vector.
   //
   // just trying stuff out... TODO think of a good way to test this ability...
   void pureQuatMultTest()
   {
      std::cout<<"pureQuatMultTest (angular velocity test)\n"<<std::flush;
      std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;

      for (float x = 0; x < 100; ++x)
      {
         vrj::Quat q1, q2, wq;
         vrj::Vec3 w(0, x, 0); // angular velocity
         wq.makePure( w );
         q1.makeRot( vrj::Math::deg2rad( 90.0f ), 0, 1, 0 );

         q2 = wq * q1;
         //////q2.normalize();  // don't normalize, will not work!!!!!!!!!

         std::cout<<"["<<q2<<"] == ["<<wq<<"] * ["<<q1<<"]\n"<<std::flush;
      }

      vrj::Quat q1, wq;
      vrj::Vec3 ww(0, 56, 0);
      wq.makePure( ww );
      q1.makeRot( vrj::Math::deg2rad( 90.0f ), 0, 1, 0 );

      const float& w1( wq[VJ_W] );
      const float& w2( q1[VJ_W] );
      vrj::Vec3 v1( wq[VJ_X], wq[VJ_Y], wq[VJ_Z] );
      vrj::Vec3 v2( q1[VJ_X], q1[VJ_Y], q1[VJ_Z] );

      float w = w1 * w2 - v1.dot( v2 );
      vrj::Vec3 v = (w1 * v2) + (w2 * v1) + v1.cross( v2 );

      std::cout<<"["<<w<<", "<<v[0]<<", "<<v[1]<<", "<<v[2]<<"] == ["<<wq<<"] * ["<<q1<<"]\n"<<std::flush;
      std::cout<<(w1 * v2)<<"  == (w1 * v2) \n"<<std::flush;
      std::cout<<(w2 * v1)<<"  == (w2 * v1) \n"<<std::flush;
      std::cout<<v1.cross( v2 )<<"  == v1.cross( v2 ) \n"<<std::flush;
      std::cout<<v1<<" dot "<<v2<<" == "<<v1.dot( v2 )<<"\n"<<std::flush;
   }

   //CPPUNIT_ASSERT( a != b.code() );
   
   
   void registerTests (CppUnit::TestSuite* suite)
   {
      suite->addTest( new CppUnit::TestCaller<QuatSelfTest>("identity test", &QuatSelfTest::identTest));        
      suite->addTest( new CppUnit::TestCaller<QuatSelfTest>("xformVecTest", &QuatSelfTest::xformVecTest));        
      suite->addTest( new CppUnit::TestCaller<QuatSelfTest>("xformVecSweepTest", &QuatSelfTest::xformVecSweepTest));        
      suite->addTest( new CppUnit::TestCaller<QuatSelfTest>("makeRotGetRotSanityTest", &QuatSelfTest::makeRotGetRotSanityTest));        
      suite->addTest( new CppUnit::TestCaller<QuatSelfTest>("scalarMultTest", &QuatSelfTest::scalarMultTest));        
      suite->addTest( new CppUnit::TestCaller<QuatSelfTest>("simpleQuatProductTest", &QuatSelfTest::simpleQuatProductTest));        
      suite->addTest( new CppUnit::TestCaller<QuatSelfTest>("secondQuatProductTest", &QuatSelfTest::secondQuatProductTest));        
      suite->addTest( new CppUnit::TestCaller<QuatSelfTest>("makeRotTest", &QuatSelfTest::makeRotTest));        
      //suite->addTest( new CppUnit::TestCaller<QuatSelfTest>("specialCases", &QuatSelfTest::specialCases));
      //suite->addTest( new CppUnit::TestCaller<QuatSelfTest>("vectorScaleTest", &QuatSelfTest::vectorScaleTest));
      //suite->addTest( new CppUnit::TestCaller<QuatSelfTest>("quatAdd", &QuatSelfTest::quatAdd));
      //suite->addTest( new CppUnit::TestCaller<QuatSelfTest>("pureQuatMultTest", &QuatSelfTest::pureQuatMultTest));
   }
};

}

#endif
